#include "call_libraries.h" //header file with important libraries
#include "read_trees.h"
#include "TMath.h"
#include "DefineHistograms.h"
#include "InputParameters.h"

void Track_Analyzer(TString input_file, TString outputFileName,Bool_t is_MC,Float_t pthat_value)
{
  
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();


    //Read the track efficiency file

  
    TFile *feff = TFile::Open(trkefffile.Data());

    Inputparameters(input_file,is_MC);
    //Read the efficiency histograms
    TH2F *hEff_2D = (TH2F*) feff->Get("hEff_2D");
    TH2F *hMul_2D = (TH2F*) feff->Get("hMul_2D");
    TH2F *hFak_2D = (TH2F*) feff->Get("hFak_2D");
    TH2F *hSec_2D = (TH2F*) feff->Get("hSec_2D");

    
    // Read the input files********************************************************
    fstream inputfile;
    inputfile.open(Form("%s",input_file.Data()), ios::in);
      
    if(!inputfile.is_open()){cout << "List of input files not founded!" << endl; return;}{cout << "List of input files founded! --> " << input_file.Data() << endl;}
    

    // Make a chain and a vector of file names*************************************
    std::vector<TString> file_name_vector;
    string file_chain;
    while(getline(inputfile, file_chain)){file_name_vector.push_back(file_chain.c_str());}
    inputfile.close();
    

    // Read the trees to be added in the Chain************************************
    TChain *hlt_tree = new TChain("hltanalysis/HltTree");
    TChain *jet_tree = new TChain("ak4PFJetAnalyzer/t");
    TChain *hea_tree = new TChain("hiEvtAnalyzer/HiTree");
    TChain *ski_tree = new TChain("skimanalysis/HltTree");
    TChain *trk_tree = new TChain("ppTracks/trackTree");
    TChain *gen_tree;
    if (is_MC)
      {gen_tree = new TChain("HiGenParticleAna/hi");}

    // add all the trees to the chain**********************************************
    
    for (std::vector<TString>::iterator listIterator = file_name_vector.begin(); listIterator != file_name_vector.end(); listIterator++){
      cout<<"list operator is "<<*listIterator<<endl;          
      
      TFile *testfile = TFile::Open(*listIterator);

      if (!testfile) cout<<"file not found";
      if(!testfile || testfile->IsZombie() || testfile->TestBit(TFile::kRecovered)) cout << "File: " << *listIterator << " failed to open" << endl;
      if(!testfile || testfile->IsZombie() || testfile->TestBit(TFile::kRecovered)) continue;


      cout << "Adding file " << *listIterator << " to the chains" << endl;


      hlt_tree->Add(*listIterator);
      hea_tree->Add(*listIterator);
      jet_tree->Add(*listIterator);
      ski_tree->Add(*listIterator);
      trk_tree->Add(*listIterator);
      if (is_MC)
	{gen_tree->Add(*listIterator);}
      
    }
    
    
    hea_tree->AddFriend(trk_tree);
    hea_tree->AddFriend(hlt_tree);
    hea_tree->AddFriend(jet_tree);
    hea_tree->AddFriend(ski_tree);
    if (is_MC)
      {hea_tree->AddFriend(gen_tree);}

    

   
    int nevents = hea_tree->GetEntries(); // number of events
    if (istestcheck) nevents=1000;
    cout << "Total number of events in those files: "<< nevents << endl;
    cout << endl;
    cout << "-------------------------------------------------" << endl;
        
    // Start loop over events ****************************************************
	
    for(int i = 0; i < nevents; i++)
      {
	hEventsnoCuts->Fill(1);
	if (i%10000==0) cout<<i<<" events passed "<<endl;    
	

	//Read trees******************************************************

	read_trees(hea_tree,is_MC);
	
	
	hea_tree->GetEntry(i);
	
	//Event Cuts ***************************************************************
	if(abs(vz) <= vzcut) continue; // vertex cut

	
	if(pprimaryVertexFilter != 1) continue; //apply the skimmed event filters
      
 
      	if(nTrk <= 0) continue; // if there are no tracks in the event

     
	//Fill Event histograms ***************************************************
        Float_t ptHatw=1.;
	if (is_MC) ptHatw=weight;
	
	hpthatW->Fill(ptHatw);
	hNtrk->Fill(nTrk,ptHatw);
	hZvtx->Fill(vz,ptHatw);
	hEvents->Fill(1,ptHatw);
	hnVtx->Fill(nVtx,ptHatw);
	
	
	if (istestcheck) cout<<" Event "<<i+1<<" nvtx "<<nVtx<<" pthatw "<<ptHatw<<endl;
	

	std::vector<int> tracksPerVertex(nVtx, 0); // Make a counter for trkvtxindx, 0 for each vertex
	//Start Analyzing reco tracks *************************************************
	for (int j = 0; j < nTrk; j++)  // Track loop start
	  {
	    //Read the required track variables
	    Int_t trk_charge  =  trkchg->at(j);
	    Float_t trk_pt      =  trkPt->at(j);
	    Float_t trk_eta     =  trkEta->at(j);
	    Float_t trk_phi     =  trkPhi->at(j);
	    Bool_t  isHighPurity=  highPurity->at(j);
	    Float_t trk_pt_error=  trkPtError->at(j);
	    
            Int_t   trk_vtxindx =  trkAssociatedVtxIndx->at(j);
	    Float_t trk_dxy     =  trkDxyAssociatedVtx->at(j);
	    Float_t trk_dxyerror=  trkDxyErrAssociatedVtx->at(j);
	    Float_t trk_dz      =  trkDzAssociatedVtx->at(j);
	    Float_t trk_dzerror =  trkDzErrAssociatedVtx->at(j);

	    
	    
	    // Apply track cuts***************************************************
	    if (trk_pt==0) continue;
	    if (abs(trk_charge)!=1) continue;
	    if(!isHighPurity) continue;
	    if(fabs(trk_eta) >= trketacut) continue;
	    if(trk_pt < trkptcut) continue;
	    if(fabs(trk_dxy/trk_dxyerror) > trkDCAxycut) continue;
	    if(fabs(trk_dz/trk_dzerror)   > trkDCAzcut) continue;
	    if(trk_pt>10 && abs(trk_pt_error/trk_pt) > ptresocut) continue;
            if (trk_vtxindx >= 0 && trk_vtxindx < nVtx) {
	      tracksPerVertex[trk_vtxindx]++; // Increase track count for that vertex
	    }
	    
	    if (istestcheck)
	      {
		cout<<" Track no. "<<j+1<<" Trk pt "<<trk_pt<<" Trk eta "<<trk_eta<<" Trk phi "<<trk_phi<<endl;
		cout<<" Trk_dxy "<<trk_dxy<<" Trk dz "<<trk_dz<<endl<<endl;
	      }
	
	    htrkDCAz->Fill(trk_dz,ptHatw);
	    htrkDCAxy->Fill(trk_dz,ptHatw);
	    if (nVtx==1)
	      {
		htrknoPUevtDCAz->Fill(trk_dz,ptHatw);
		htrknoPUevtDCAxy->Fill(trk_dxy,ptHatw);
	      }
	    //Read the values from efficiency histograms
	    Float_t eff = hEff_2D->GetBinContent(hEff_2D->GetXaxis()->FindBin(trk_eta),hEff_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t fak = hFak_2D->GetBinContent(hFak_2D->GetXaxis()->FindBin(trk_eta),hFak_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t sec = hSec_2D->GetBinContent(hSec_2D->GetXaxis()->FindBin(trk_eta),hSec_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t mul = hMul_2D->GetBinContent(hMul_2D->GetXaxis()->FindBin(trk_eta),hMul_2D->GetYaxis()->FindBin(trk_pt));
	    

	    //Correction factors for tracks
	    Float_t corr_wt=((1.0 - fak)*(1.0 - sec))/(eff*(1.0 + mul));
	    //Float_t corr_wt=((1.0 - fak)*(1.0 - sec))/(eff);
	    Float_t trkwt=1.;
	    trkwt=corr_wt;
	    //Weight for the invariant yield 
	    Float_t invyield_wt= (1./(2*2*TMath::Pi()*trk_pt));
	    //Float_t invyield_wt= (1./trk_pt);
	    //Float_t speciescorrfactor = hspeciescorrfactor->GetBinContent(hspeciescorrfactor->GetYaxis()->FindBin(trk_pt));
	    Float_t speciescorrfactor=1.0;
            if (istestcheck)
	      {
		cout<<" Trk corr factor "<<corr_wt<<" Part species corr factor "<<speciescorrfactor<<endl;
	      }

	    if (!islowptfile && trk_pt < 24) continue; //Making a cut for low pt file
	    htrkpteta->Fill(trk_pt,trk_eta,ptHatw);
	    htrkinvyield->Fill(trk_pt,invyield_wt*ptHatw);
	    htrkpt->Fill(trk_pt,ptHatw);
	    htrketa->Fill(trk_eta,ptHatw);

	 
	    htrkpteta_corr->Fill(trk_pt,trk_eta,trkwt*ptHatw);
            htrkinvyield_corr->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
            htrkpt_corr->Fill(trk_pt,trkwt*ptHatw);
	    htrketa_corr->Fill(trk_pt,trkwt*ptHatw);

	    if (isSystematics)
	      {
		//High and low PU runs
		if (run==387590) htrkinvyieldhighPU->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
		if (run==387570) htrkinvyieldlowPU->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
	      }
	    
	  }//Reco track loop end

	for (size_t vtx = 0; vtx < tracksPerVertex.size(); ++vtx) {
	  htrkspervtx->Fill(tracksPerVertex[vtx], ptHatw); // Fill how many tracks in this vertex
	}
	
	//Gen loop starts (Only for MC) **************************************
	Int_t gentrksize;
	std::vector<Int_t> genmatchedindx={-9};
       
	if (is_MC)
	  { gentrksize = gentrkPt->size();
	    //cout<<"Gentrkptsize  "<<gentrksize<<"  Gentrkchgsize "<<gentrkchg->size()<<endl;
	    for (int j=0;j<gentrksize;j++)
	      {
		
		
	 	Float_t gentrk_pt = gentrkPt->at(j);
		Float_t gentrk_eta= gentrkEta->at(j);
		Float_t gentrk_phi= gentrkPhi->at(j);
	        Int_t   gentrk_chg= gentrkchg->at(j);
		Int_t   gentrk_pdg= gentrkpdg->at(j);
		
	      
		//Track cuts on gen tracks
		if (abs(gentrk_chg)!=1) continue;
		if(fabs(gentrk_eta) >= trketacut) continue;
		if(gentrk_pt < trkptcut) continue;

		
		hgentrkpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		htotalgenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		hgentrkpt->Fill(gentrk_pt,ptHatw);
		hgentrketa->Fill(gentrk_eta,ptHatw);

		if (istestcheck)
		  cout<<" gen pt "<<gentrk_pt<<" gen eta "<<gentrk_eta<<" gen phi "<<gentrk_phi<<endl;

		if (abs(gentrk_pdg)==2212) hpapgenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		else if (abs(gentrk_pdg)==211)  hpiongenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		else if (abs(gentrk_pdg)==321)  hKgenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		else if (abs(gentrk_pdg)==3222) hsigmaplusgenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		else if (abs(gentrk_pdg)==3112) hsigmaminusgenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		else hremaindergenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		
                Float_t mindeltaR=999.0;
		Float_t matchedgenpt=-999;
		Float_t matchedgeneta=-999.;
		Float_t matchedrecopt=-999;
                Float_t matchedrecoeta=-999.;
		Int_t matchedpdg=-999;
		Int_t matchedindx=-999;
	       
		// Reco loop for gen matching
		if (isgenmatching) {
		for (int k=0;k<nTrk;k++)
		  { Bool_t ismatched=0;
		    for (int l=0;l<genmatchedindx.size();l++)
		      { if (k==genmatchedindx.at(l)) 
			  {ismatched=1;
			    continue;
			  }
		      }

		    if (ismatched) continue;
		    Int_t recotrk_chg       =  trkchg->at(k);
		    Float_t recotrk_pt      =  trkPt->at(k);
		    Float_t recotrk_eta     =  trkEta->at(k);
		    Float_t recotrk_phi     =  trkPhi->at(k);
		    Bool_t  recoisHighPurity=  highPurity->at(k);
		    Float_t recotrk_pt_error=  trkPtError->at(k);
		    

		    Float_t recotrk_dxy     =  trkDxyAssociatedVtx->at(k);
		    Float_t recotrk_dxyerror=  trkDxyErrAssociatedVtx->at(k);
		    Float_t recotrk_dz      =  trkDzAssociatedVtx->at(k);
		    Float_t recotrk_dzerror =  trkDzErrAssociatedVtx->at(k);
		    
		    
		    
		    // Apply track cuts***************************************************
		    if (abs(recotrk_chg)!=1) continue;
		    if(!recoisHighPurity) continue;
		    if(fabs(recotrk_eta) >= 1.0) continue;
		    if(recotrk_pt<0.1) continue;
		    if(fabs(recotrk_dxy/recotrk_dxyerror) > 3.0) continue;
		    if(fabs(recotrk_dz/recotrk_dzerror)   > 3.0) continue;
		    if(recotrk_pt>10 && abs(recotrk_pt_error/recotrk_pt) > 0.1) continue;
		    
                    Float_t DpT= fabs(gentrk_pt-recotrk_pt);
		    Float_t Deta= fabs(gentrk_eta-recotrk_eta);
		    Float_t Dphi= fabs(TMath::ACos(TMath::Cos(gentrk_phi))-TMath::ACos(TMath::Cos(recotrk_phi)));
		    Float_t DeltaR= sqrt((Deta*Deta)+(Dphi*Dphi));
		    Float_t DeltapT=DpT/gentrk_pt;
		    
		    
		    hDeltapT->Fill(DeltapT,ptHatw);
		    hDeltaR->Fill(DeltaR,ptHatw);
		    
		    //cout<<"Deta "<<Deta<<"  Dphi  "<<Dphi<<"  DR  "<<DeltaR<<" Delta pT "<<DeltapT<<"  gentrk pt "<<gentrk_pt<<endl;
		    if (DeltaR < 0.02 && DeltapT < 0.04)
		      //&& (0.96*gentrk_pt)<recotrk_pt && recotrk_pt<(1.04*gentrk_pt))
		      {if (DeltaR < mindeltaR)
			  {mindeltaR=DeltaR;
			    matchedgenpt=gentrk_pt;
			    matchedgeneta=gentrk_eta;
			    matchedpdg=gentrk_pdg;
			    matchedrecopt=recotrk_pt;
                            matchedrecoeta=recotrk_eta;
                            matchedpdg=gentrk_pdg;
			    matchedindx=k;
			  }
		      }
		    
		    
		  }//matching loop ends
		}
		if (mindeltaR<999 ){
		  genmatchedindx.push_back(matchedindx);
		  hgenmatchedpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
		  hgenmatchedpt->Fill(matchedgenpt,ptHatw);
		  hgenmatchedeta->Fill(matchedgeneta,ptHatw);
		  
		  if (abs(matchedpdg)==2212) hpapmatchedpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
 		  else if (abs(matchedpdg)==211) hpionmatchedpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
		  else if (abs(matchedpdg)==321) hKmatchedpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
		  else if (abs(matchedpdg)==3222) hsigmaplusmatchedgenpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
		  else if (abs(matchedpdg)==3112) hsigmaminusmatchedgenpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
		  else hremaindermatchedgenpteta->Fill(matchedgenpt,matchedgeneta,ptHatw);
		  
		}
		//else cout<<"matched gen not found "<<endl;
		
		
	      }//gen trk loop ends	
	  }
      }
    
    
    delete hea_tree;      
    delete hlt_tree;      
    delete ski_tree;
    delete jet_tree;
    delete trk_tree;
    
    
    
    TFile *fout = new TFile(outputFileName, "RECREATE");

    WriteHistograms();
    fout->Write();
    fout->Close();
    delete fout;
    cout<<"finished";
    
    
    

    
} // program end

int main(int argc, char** argv){
                                TString firstArgument(argv[1]);
                                TString outfile(argv[2]);
				int mc = atoi(argv[3]);
                                Float_t pthat_value= atoi(argv[4]);
				
				Track_Analyzer(firstArgument,outfile,mc,pthat_value);
}

