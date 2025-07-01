#include "call_libraries.h" //header file with important libraries
#include "read_trees.h"
#include "TMath.h"
#include "DefineHistograms.h"
//#include "InputParameters.h"

void Track_Analyzer(TString input_file, TString outputFileName,Bool_t is_MC,Float_t pthat_value)
{
  
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();


    //Read the track efficiency file

    Inputparameters(input_file,is_MC);
    

    TFile *feff = TFile::Open(trkefffile.Data());
    if (!feff) cout<<"pp eff file not found "<<endl;
    TFile *feffOOfake;
    if (coll_system=="OO"||coll_system=="pO") feffOOfake = TFile::Open(trkefffileOOfake.Data());
    
    TH2F *hEff_2D = (TH2F*) feff->Get("hEff_2D");
    TH2F *hMul_2D = (TH2F*) feff->Get("hMul_2D");
    TH2F *hFak_2D=nullptr;
    
    if (coll_system=="pp")
      { cout<<"Here............... "<<endl;
	hFak_2D= (TH2F*) feff->Get("hFak_2D");
      }
    if (!hFak_2D) cout<<" Fake rate histogram not found "<<endl;
    if (coll_system=="OO" || coll_system=="pO")  hFak_2D= (TH2F*) feffOOfake->Get("hFak_2D");
    TH2F *hSec_2D = (TH2F*) feff->Get("hSec_2D");
    
    //Read Tight and loose efficiency histograms (for systematics)
    TFile* feffsyst[2];
    TH2F* hEff_2Dsyst[2];
    TH2F* hMul_2Dsyst[2];
    TH2F* hFak_2Dsyst[2];
    TH2F* hSec_2Dsyst[2];
    
    for (int k = 0; k < 2; ++k) {
      feffsyst[k] = TFile::Open(trkeffsystfiles[k].Data());
      hEff_2Dsyst[k] = (TH2F*) feffsyst[k]->Get("hEff_2D");
      hMul_2Dsyst[k] = (TH2F*) feffsyst[k]->Get("hMul_2D");
      hFak_2Dsyst[k] = (TH2F*) feffsyst[k]->Get("hFak_2D");
      hSec_2Dsyst[k] = (TH2F*) feffsyst[k]->Get("hSec_2D");
    }
    

    //Function to calculate efficiency factor
    auto GetTrkCorr=[](float trk_eta, float trk_pt,
                    TH2F* hEff, TH2F* hFak, TH2F* hSec, TH2F* hMul)->Float_t
    {
      int bin_eta = hEff->GetXaxis()->FindBin(trk_eta);
      int bin_pt  = hEff->GetYaxis()->FindBin(trk_pt);

      float eff = hEff->GetBinContent(bin_eta, bin_pt);
      float fak = hFak->GetBinContent(bin_eta, bin_pt);
      float sec = hSec->GetBinContent(bin_eta, bin_pt);
      float mul = hMul->GetBinContent(bin_eta, bin_pt);

      if (eff <= 0) return 0; // prevent divide-by-zero

      float corr = ((1.0 - fak)*(1.0 - sec)) / (eff * (1.0 + mul));
      return corr;
    };

    
    
    // Read the input files********************************************************
    std::vector<TString> file_name_vector;
    
    if (input_file.EndsWith(".root")) {
      cout << "Single ROOT file detected --> " << input_file.Data() << endl;
      file_name_vector.push_back(input_file);
    } else {
      fstream inputfile;
      inputfile.open(Form("%s",input_file.Data()), ios::in);
      
      if(!inputfile.is_open()){cout << "List of input files not found!" << endl; return;}
      else {cout << "List of input files found! --> " << input_file.Data() << endl;}
      
      // Make a chain and a vector of file names*************************************
      string file_chain;
      while(getline(inputfile, file_chain)) {
	if (!file_chain.empty()) file_name_vector.push_back(file_chain.c_str());
      }
      inputfile.close();
    }
    

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
    if (istestcheck) nevents=500;
    cout << "Total number of events in those files: "<< nevents << endl;
    cout << endl;
    cout << "-------------------------------------------------" << endl;

    std::set<std::pair<int, int>> runLumi;

    // Start loop over events ****************************************************
	
    for(int i = 0; i < nevents; i++)
      {
	hEventsnoCuts->Fill(1);
	hEvents->Fill(1);
	if (i%10000==0) cout<<i<<" events passed "<<endl;    
	

	//Read trees******************************************************

	read_trees(hea_tree,is_MC);
	
	
	hea_tree->GetEntry(i);
	
	//Event Cuts ***************************************************************
	if (istestcheck) cout<<abs(vz)<<"  "<<pprimaryVertexFilter<<" "<<nTrk<<endl;

	if(pprimaryVertexFilter != 1) continue; //apply the skimmed event filters

	hEvents->Fill(2);
	if(abs(vz) > vzcut) continue; // vertex cut
        
	
	
      
	hEvents->Fill(3);
      	if(nTrk <= 0) continue; // if there are no tracks in the event

	hEvents->Fill(4);
	runLumi.insert({run, lumi});

     
	//Fill Event histograms ***************************************************
        Float_t ptHatw=1.;
	if (is_MC) ptHatw=weight;
	
	hpthatW->Fill(ptHatw);
	hNtrk->Fill(nTrk,ptHatw);
	hZvtx->Fill(vz,ptHatw);
	hEvents->Fill(5,ptHatw);
	hnVtx->Fill(nVtx,ptHatw);
	
	
	if (istestcheck) cout<<" Event "<<i+1<<" nvtx "<<nVtx<<" pthatw "<<ptHatw<<endl;
	
	//cout<<" size of isfake "<<sizeof(isFakeVtx)<<endl;
	//Start Analyzing reco tracks *************************************************
	for (int j = 0; j < nTrk; j++)  // Track loop start
	  {
	    //Read the required track variables
	    htrkcounts->Fill(1);
	    Int_t   trk_charge   =  trkchg->at(j);
	    Float_t trk_pt       =  trkPt->at(j);
	    Float_t trk_eta      =  trkEta->at(j);
	    Float_t trk_phi      =  trkPhi->at(j);
	    Bool_t  isHighPurity =  highPurity->at(j);
	    Float_t trk_pt_error =  trkPtError->at(j);
	    Int_t   trk_nhits    =  trkNHits->at(j);
	    Float_t trk_chi2     =  trkNormChi->at(j);
	    
            Int_t   trk_vtxindx  =  trkAssociatedVtxIndx->at(j);
	    Float_t trk_dxy      =  trkDxyAssociatedVtx->at(j);
	    Float_t trk_dxyerror =  trkDxyErrAssociatedVtx->at(j);
	    Float_t trk_dz       =  trkDzAssociatedVtx->at(j);
	    Float_t trk_dzerror  =  trkDzErrAssociatedVtx->at(j);
	    Float_t trk_pfecal   =  pfEcal->at(j);
	    Float_t trk_pfhcal   =  pfHcal->at(j);
	    
	    
	    
	    // Apply track cuts***************************************************
	    if (!trigger_flag) continue; //Trigger
	    htrkcounts->Fill(2);
	    if (trk_pt==0) continue;
	    htrkcounts->Fill(3);
	    if (abs(trk_charge)!=1) continue;
	    htrkcounts->Fill(4);
	    if(!isHighPurity) continue;
	    htrkcounts->Fill(5);
	    if(fabs(trk_eta) >= trketacut) continue;
	    htrkcounts->Fill(6);
	    if(trk_pt < trkptcut) continue;
	    htrkcounts->Fill(7);
	    if(trk_pt>10 && abs(trk_pt_error/trk_pt) > ptresocut) continue;
	    htrkcounts->Fill(8);
	    if (coll_system=="pp" && !islowptfile && trk_pt < 24) continue; //Making a cut for low pt files
	    htrkcounts->Fill(9);
	    //Weight for the invariant yield 
	    Float_t invyield_wt= (1./(2*2*trketacut*TMath::Pi()*trk_pt));

	    if (isSystematics){
	    //Loose and tight efficiency tables
	    for (int i = 0; i < 2; ++i) {
	      
	      if(fabs(trk_dxy/trk_dxyerror) > trkDCAxycutsyst[i]) continue;
	      if(fabs(trk_dz/trk_dzerror)   > trkDCAzcutsyst[i]) continue;
	      Float_t corr_wtsyst = GetTrkCorr(trk_eta,trk_pt,hEff_2Dsyst[i],hFak_2Dsyst[i],hSec_2Dsyst[i],hMul_2Dsyst[i]);
	      Float_t trkwtsyst = corr_wtsyst;

	      
	      htrkinvyieldeffsyst[i]->Fill(trk_pt, invyield_wt * trkwtsyst * ptHatw);
	      htrkinvyieldspliteffsyst[i]->Fill(trk_pt, invyield_wt * trkwtsyst * ptHatw);
	    }
	   
	    }
	    htrkcounts->Fill(10);
	    if(fabs(trk_dxy/trk_dxyerror) > trkDCAxycut) continue;
	    htrkcounts->Fill(11);
	    if(fabs(trk_dz/trk_dzerror)   > trkDCAzcut) continue;
	    htrkcounts->Fill(12);
	    
	    if (istestcheck)
	      {
		cout<<" Track no. "<<j+1<<" Trk pt "<<trk_pt<<" Trk eta "<<trk_eta<<" Trk phi "<<trk_phi<<endl;
		cout<<" Trk_dxy "<<trk_dxy<<" Trk dz "<<trk_dz<<endl<<endl;
	      }
	    //Correction factors for tracks (nominal)
	    Float_t corr_wt=GetTrkCorr(trk_eta,trk_pt,hEff_2D,hFak_2D,hSec_2D,hMul_2D);
	    Float_t trkwt=1.;
	    trkwt=corr_wt;
	    Float_t Et=(trk_pfecal+trk_pfhcal)/TMath::CosH(trk_eta);
	    Float_t Et_pt=(Et/trk_pt);
	    if (istestcheck)
	      {cout<<" PF energies "<<endl;
		cout<<trk_pfecal<<"  "<<trk_pfhcal<<"  "<<Et<<" "<<Et_pt<<endl;
	      }
	    hEcal->Fill(trk_pfecal,ptHatw);
	    hHcal->Fill(trk_pfhcal,ptHatw);
	    hEt->Fill(Et,ptHatw);
	    hEt_pt->Fill(Et_pt,ptHatw);

	    htrkpteta->Fill(trk_pt,trk_eta,ptHatw);
	    htrkinvyield->Fill(trk_pt,invyield_wt*ptHatw);
	    htrkinvyield_split->Fill(trk_pt,invyield_wt*ptHatw);
	    htrkpt->Fill(trk_pt,ptHatw);

	    
	    htrkpt_split->Fill(trk_pt,ptHatw);
	    htrketa->Fill(trk_eta,ptHatw);
            htrkphi->Fill(trk_phi,ptHatw);

	    
	    
	    htrkDCAz->Fill(trk_dz,ptHatw);
	    htrkDCAxy->Fill(trk_dxy,ptHatw);
	    htrkNhits->Fill(trk_nhits,ptHatw);
	    htrkNormChi->Fill(trk_chi2,ptHatw);
	    if (istestcheck)
	      {cout<<" Nhits "<<trk_nhits<<" Chi 2 "<<trk_chi2<<endl;}
	    if (nVtx==1)
	      {
		htrknoPUevtDCAz->Fill(trk_dz,ptHatw);
		htrknoPUevtDCAxy->Fill(trk_dxy,ptHatw);
	      }

	  
	    
	    
	    htrkpteta_corr->Fill(trk_pt,trk_eta,trkwt*ptHatw);
            htrkinvyield_corr->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
	    htrkinvyield_split_corr->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
            htrkpt_corr->Fill(trk_pt,trkwt*ptHatw);
	    htrketa_corr->Fill(trk_eta,trkwt*ptHatw);

	    if (isSystematics)
	      {
		//High and low PU runs
		if (run==highPUrun)
		  {
		    htrkinvyieldhighPU->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
		    htrkinvyieldsplithighPU->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
		  }
		
		if (run==lowPUrun)
		  {
		    htrkinvyieldlowPU->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
		    htrkinvyieldsplitlowPU->Fill(trk_pt,invyield_wt*trkwt*ptHatw);
		  }

		

		//Gaussian pT smearing (for momentum resolution systematics)
		Float_t smeared_pt=gRandom->Gaus(trk_pt, 0.02 * trk_pt);
		Float_t invyield_wtsmeared=(1./(2*2*trketacut*TMath::Pi()*smeared_pt));
		Float_t smeared_corrwt=GetTrkCorr(trk_eta,smeared_pt,hEff_2D,hFak_2D,hSec_2D,hMul_2D);
		htrkinvyieldsmeared->Fill(smeared_pt,invyield_wtsmeared*smeared_corrwt*ptHatw);
		htrkinvyieldsplitsmeared->Fill(smeared_pt,invyield_wtsmeared*smeared_corrwt*ptHatw);


		
	      }
	    
	  }//Reco track loop end

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

    // Generate lumi output file path beside ROOT file
    TString outbasename = gSystem->BaseName(outputFileName); 
    outbasename.ReplaceAll(".root", "");                     
    
    TString outdir = gSystem->DirName(outputFileName);       
    TString lumifilepath = outdir + "/runLumi_" + outbasename + ".txt";

    // Write to lumi text file
    std::ofstream lumifile(lumifilepath.Data());
    for (const auto& entry : runLumi) {
      lumifile << entry.first << " " << entry.second << "\n";
    }
    lumifile.close();
    
    
    TFile *fout = new TFile(outputFileName, "RECREATE");
    
    fout->cd();
    WriteHistograms();
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

