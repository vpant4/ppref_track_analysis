#include "include_libraries.h" //header file with important libraries
#include "read_trees.h"
#include "TMath.h"


void Track_Analyzer(TString input_file, TString outputFileName,Bool_t is_MC,Float_t pthat_value)
{
  
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  TH3::SetDefaultSumw2();


    //Read the track efficiency file

  //          TFile *feff= TFile::Open("/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/Eff/ppref_2024_Pythia_minBias_NopU_2D_Nominla.root","READ");

  TFile *feff = TFile::Open("/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/Eff/ppref_2024_Pythia_QCDptHat15_NopU_2D_Nominla.root");
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


    //Define the relevant histograms*********************************************

    //variable pT bins
    //double pTbins[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};
    //double pTbins[]={4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};


    double pTbins[]={0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
        0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
        1.0, 1.05, 1.1, 1.15, 1.2,
        1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
        2.5, 3.0, 4.0, 5.0, 7.5, 10.0, 12.0, 15.0,
        20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 
		     180.0, 300.0, 500.0};

    
    int nptbins=sizeof(pTbins) / sizeof(pTbins[0]) - 1;
    //Event level histograms
    TH1F* hEventsnoCuts= new TH1F("hEventsnoCuts","",10,0,10);
    TH1F* hnVtx         = new TH1F("hnVtx","hnVtx",500,0,20);
    
    TH1F* hEvents = new TH1F("hEvents", "", 10, 0, 10);
    TH1F* hpthatW = new TH1F("hpthatW", "", 200, 0, 600.);
    TH1F* hZvtx = new TH1F("hZvtx", "", 80, -20, 20);

    // 1-d and 2-d histograms to store trk info
    TH2F* htrkpteta = new TH2F("htrkpteta","htrkpteta",1000,0.,1000.,200,-1.,1.);
    TH1F* htrkpt    = new TH1F("htrkpt","htrkpt",nptbins,pTbins);
    TH1F* htrketa   = new TH1F("htrketa","htrketa",200,-1.,1.);
    TH1F* htrkinvyield = new TH1F("htrkinvyield","htrkinvyield",nptbins,pTbins);
    TH1F* hNtrk     = new TH1F("hNtrk","hNtrk",nptbins,pTbins);


    //Gen matching information
    TH1F* hgenmatchedpt= new TH1F("hgenmatchedpt","hgenmatchedpt",nptbins,pTbins);
    TH1F* hgenmatchedeta= new TH1F("hgenmatchedeta","hgenmatchedeta",200,-1.,1.);
    TH1F* hDeltaR = new TH1F("hDeltaR","hDeltaR",1000,0.,1);
    TH1F* hDeltapT= new TH1F("hDeltapT","hDeltapT",1000,0.,1);

    TH2F* htotalgenpteta = new TH2F("htotalgenpteta","htotalgenpteta",nptbins,pTbins,200,-1.,1.);
    TH2F* hgenmatchedpteta=new TH2F("hgenmatchedpteta","hgenmatchedpteta",nptbins,pTbins,200,-1.,1.);
    TH2F* hpmatchedpteta=new TH2F("hpmatchedpteta","hpmatchedpteta",nptbins,pTbins,200,-1.,1.);
    TH2F* hapmatchedpteta=new TH2F("hapmatchedpteta","hapmatchedpteta",nptbins,pTbins,200,-1.,1.);
    TH2F* hpionmatchedpteta=new TH2F("hpionmatchedpteta","hpionmatchedpteta",nptbins,pTbins,200,-1.,1.);
    TH2F* hKmatchedpteta=new TH2F("hKmatchedpteta","hKmatchedpteta",nptbins,pTbins,200,-1.,1.);

    //Histograms to store gen track information (for MC)
    TH2F* hgentrkpteta = new TH2F("hgentrkpteta","hgentrkpteta",nptbins,pTbins,200,-1.,1.);
    TH1F *hgentrkpt = new TH1F("hgentrkpt","hgentrkpt",nptbins,pTbins);
    TH1F *hgentrketa= new TH1F("hgentrketa","hgentrketa",200,-1.,1.);
    TH1F* hgenNtrk     = new TH1F("hgenNtrk","hgenNtrk",nptbins,pTbins);
    
    //Histograms with track corrections
    
    TH2F* htrkpteta_corr = new TH2F("htrkpteta_corr","htrkpteta_corr",1000,0.,1000.,200,-1.,1.);
    TH1F* htrkpt_corr    = new TH1F("htrkpt_corr","htrkpt_corr",nptbins,pTbins);
    TH1F* htrketa_corr    = new TH1F("htrketa_corr","htrketa_corr",200,-1.,1.);
    TH1F* htrkinvyield_corr = new TH1F("htrkinvyield_corr","htrkinvyield_corr",nptbins,pTbins);
    
    //Histograms to store jet information
    TH2F *hjtpteta  = new TH2F("hjtpteta","hjtpteta",1000,0.,1000.,200,-1.,1.);
    TH1F *hjtpt     = new TH1F("hjtpt","hjtpt",nptbins,pTbins);
    TH1F *hjtinvyield=new TH1F("hjtinvyield","hjtinvyield",nptbins,pTbins);
    TH1F *hjteta    = new TH1F("hjteta","hjteta",200,-1.,1.);
    TH1F *hjtphi    = new TH1F("hjtphi","hjtphi",1000,-TMath::Pi(),TMath::Pi());

    
        
    int nevents = hea_tree->GetEntries(); // number of events
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
	if(vz < -15. || vz > 15.) continue; // vertex cut

	
	//if(pprimaryVertexFilter != 1) continue; //apply the skimmed event filters
      
 
      	if(nTrk <= 0) continue; // if there are no tracks in the event

	//if(!HLT_AK4PFJet120_v8) continue; //Trigger cut
	//Fill Event histograms ***************************************************
        Float_t ptHatw=1.;
	//if (is_MC) ptHatw=weight;

	hpthatW->Fill(ptHatw);
	hNtrk->Fill(nTrk,ptHatw);
	hZvtx->Fill(vz,ptHatw);
	hEvents->Fill(1,ptHatw);
	hnVtx->Fill(nVtx,ptHatw);
	//cout<<"Reco trk size "<<nTrk<<" trk chg size "<<trkchg->size()<<"  trk pt size "<<trkPt->size()<<endl;

	
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


	    Float_t trk_dxy     =  trkDxyAssociatedVtx->at(j);
	    Float_t trk_dxyerror=  trkDxyErrAssociatedVtx->at(j);
	    Float_t trk_dz      =  trkDzAssociatedVtx->at(j);
	    Float_t trk_dzerror =  trkDzErrAssociatedVtx->at(j);

	    
	    
	    // Apply track cuts***************************************************
	    if (trk_charge==0) continue;
            if(!isHighPurity) continue;
	    if(fabs(trk_eta) >= 1.0) continue;
	    if(trk_pt<0.1) continue;
	    if(fabs(trk_dxy/trk_dxyerror) > 3.0) continue;
	    if(fabs(trk_dz/trk_dzerror)   > 3.0) continue;
	    if(trk_pt>10 && abs(trk_pt_error/trk_pt) > 0.1) continue;
	    
	    	    //Read the values from efficiency histograms
	    Float_t eff = hEff_2D->GetBinContent(hEff_2D->GetXaxis()->FindBin(trk_eta),hEff_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t fak = hFak_2D->GetBinContent(hFak_2D->GetXaxis()->FindBin(trk_eta),hFak_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t sec = hSec_2D->GetBinContent(hSec_2D->GetXaxis()->FindBin(trk_eta),hSec_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t mul = hMul_2D->GetBinContent(hMul_2D->GetXaxis()->FindBin(trk_eta),hMul_2D->GetYaxis()->FindBin(trk_pt));

	    //Correction factors for tracks
	    //Float_t corr_wt=((1.0 - fak)*(1.0 - sec))/(eff*(1.0 + mul));
	    Float_t corr_wt=((1.0 - fak)*(1.0 - sec))/(eff);
	    //Float_t corr_wt=1/eff;
	    Float_t trkwt=1.;
	    trkwt=corr_wt;
	     //Weight for the invariant yield 
	    Float_t invyield_wt= (1./(2*2*TMath::Pi()*trk_pt)); 
	      

	    //cout<<"trk pt "<<trk_pt<<endl;
	    htrkpteta->Fill(trk_pt,trk_eta);
	    htrkinvyield->Fill(trk_pt,invyield_wt*ptHatw);
	    htrkpt->Fill(trk_pt);
	    htrketa->Fill(trk_eta);

	    htrkpteta_corr->Fill(trk_pt,trk_eta,trkwt);
            htrkinvyield_corr->Fill(trk_pt,invyield_wt*trkwt);
            htrkpt_corr->Fill(trk_pt,trkwt);
	    htrketa_corr->Fill(trk_pt,trkwt);

	    
	  }//Reco track loop end

	
	//Gen loop starts (Only for MC) **************************************
	Int_t gentrksize;
       
	if (is_MC)
	  { gentrksize = gentrkPt->size();
	    //cout<<"Gentrkptsize  "<<gentrksize<<"  Gentrkchgsize "<<gentrkchg->size()<<endl;
	    for (int j=0;j<gentrksize;j++)
	      {
	       
		//if (!gentrkPt->size()||!gentrkEta->size()||!gentrkPhi->size()) continue;
	 	Float_t gentrk_pt = gentrkPt->at(j);
		Float_t gentrk_eta= gentrkEta->at(j);
		Float_t gentrk_phi= gentrkPhi->at(j);
	        Int_t   gentrk_chg= gentrkchg->at(j);
		Int_t   gentrk_pdg= gentrkpdg->at(j);
		//cout<<gentrk_pt<<endl;
	      
		//Track cuts on gen tracks
		if (gentrk_chg==0) continue;
		if(fabs(gentrk_eta) >= 1.0) continue;
		if(gentrk_pt<0.1) continue;

		
		hgentrkpteta->Fill(gentrk_pt,gentrk_eta);
		htotalgenpteta->Fill(gentrk_pt,gentrk_eta,ptHatw);
		hgentrkpt->Fill(gentrk_pt);
		hgentrketa->Fill(gentrk_eta);
                Float_t mindeltaR=999.0;
		Float_t matchedgenpt=-999;
		Float_t matchedgeneta=-999.;
		Float_t matchedrecopt=-999;
                Float_t matchedrecoeta=-999.;
		Int_t matchedpdg=-999;
		// Reco loop for gen matching
		for (int k=0;k<nTrk;k++)
		  {

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
		    if (recotrk_chg==0) continue;
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

		    //		    cout<<"Deta "<<Deta<<"  Dphi  "<<Dphi<<"  DR  "<<DeltaR<<" Delta pT "<<DeltapT<<"  gentrk pt "<<gentrk_pt<<endl;
		    if (DeltaR < 0.02 && DeltapT<0.04)
		      {if (DeltaR < mindeltaR)
			  {mindeltaR=DeltaR;
			    matchedgenpt=gentrk_pt;
			    matchedgeneta=gentrk_eta;
			    matchedpdg=gentrk_pdg;
			    matchedrecopt=recotrk_pt;
                            matchedrecoeta=recotrk_eta;
                            matchedpdg=gentrk_pdg;
			   }
		      }
		    
		    
		  }//matching loop ends

		if (mindeltaR<999 ){
		  //cout<<"***  Matched delta R *** "<<mindeltaR<<" matched gen pt "<<matchedgenpt<<" matched gen pdg "<<matchedpdg<<" matched reco pt" <<matchedrecopt<<endl;
		  hgenmatchedpteta->Fill(matchedrecopt,matchedrecoeta,ptHatw);
		  hgenmatchedpt->Fill(matchedrecopt);
		  hgenmatchedeta->Fill(matchedrecoeta);
		  if (matchedpdg==2212) hpmatchedpteta->Fill(matchedrecopt,matchedrecoeta,ptHatw);
		  if (matchedpdg==-2212) hapmatchedpteta->Fill(matchedrecopt,matchedrecoeta,ptHatw);
		  if (abs(matchedpdg)==211) hpionmatchedpteta->Fill(matchedrecopt,matchedrecoeta,ptHatw);
		  if (abs(matchedpdg)==321) hKmatchedpteta->Fill(matchedrecopt,matchedrecoeta,ptHatw);

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

    hEventsnoCuts->Write();    
    hEvents->Write();
    hNtrk->Write();
    hZvtx->Write();
    hnVtx->Write();

    hDeltapT->Write();
    hDeltaR->Write();
    
    htrkpt->Write();
    htrketa->Write();
    htrkpteta->Write();
    htrkinvyield->Write();

    htrkpt_corr->Write();
    htrketa_corr->Write();
    htrkpteta_corr->Write();
    htrkinvyield_corr->Write();

    htotalgenpteta->Write();
    hgenmatchedpt->Write();
    hgenmatchedeta->Write();
    hgenmatchedpteta->Write();
    hpmatchedpteta->Write();
    hapmatchedpteta->Write();
    hKmatchedpteta->Write();
    hpionmatchedpteta->Write();
    
    hjtpt->Write();
    hjtpteta->Write();
    hjtinvyield->Write();
    
    hjtphi->Write();
    hjteta->Write();

    hgentrkpteta->Write();
    hgentrkpt->Write();
    hgentrketa->Write();
  
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
				int is_Pbgoing=atoi(argv[5]);
				
                                int pileup=atoi(argv[6]);
				
				Track_Analyzer(firstArgument,outfile,mc,pthat_value);
}


