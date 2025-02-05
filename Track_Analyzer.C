#include <TROOT.h>       // ROOT system header
#include <TFile.h>       // ROOT file I/O
#include <TTree.h>       // ROOT tree
#include <TH1.h>  // ROOT 1D histograms
#include <TH1D.h>
#include "TH3.h"
#include <vector>
#include <TH2.h>         // ROOT 2D histograms
#include <TCanvas.h>     // ROOT canvas for plotting
#include <TGraph.h>      // ROOT graph
#include <TGraph2D.h>    // ROOT 2D graph
#include <TF1.h>         // ROOT 1D functions
#include <TMath.h>       // ROOT math functions
#include <TStyle.h>      // ROOT style settings
#include <TLegend.h>     // ROOT legends
#include <TPad.h>        // ROOT pads for arranging plots
#include <TMathText.h>   // ROOT text rendering
#include <TChain.h>      // ROOT chain for combining TTree(s)
#include <TString.h>     // ROOT string class
#include <iostream>      // C++ standard I/O
#include "TROOT.h"
#include "TTree.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TVector3.h"
#include <TRandom1.h>
#include <TRandom2.h>
#include <TRandom3.h>
#include <vector>
#include <TLorentzVector.h>
#include "THnSparse.h"
#include <cstring>
#include <ctime>
#include <iostream>
#include <cmath>
#include <math.h>
#include <fstream>
#include <vector>
#include <map>
#include "TFrame.h"
#include "TBenchmark.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TProfile2D.h"
#include "TDatime.h"
#include <stdlib.h>
#include <algorithm>	
#include "TApplication.h"
#include "Rtypes.h"
#include "TObject.h"
#include "TDirectoryFile.h"
using namespace std;


void Track_Analyzer(TString input_file, TString outputFileName,int is_MC,Float_t pthat_value)
{
  
    TH1::SetDefaultSumw2();
    TH2::SetDefaultSumw2();
    TH3::SetDefaultSumw2();


 
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

    }
  	

    //Define the relevant histograms*********************************************

    //variable pT bins
    double pTbins[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.,600.,1000.};
    int nptbins=sizeof(pTbins) / sizeof(pTbins[0]) - 1;
    //Event level histograms
    TH1D* hEvents = new TH1D("hEvents", "", 10, 0, 10);
    TH1D* hpthat = new TH1D("hpthat", "", 200, 0, 600.);
    TH1D* hpthatW = new TH1D("hpthatW", "", 200, 0, 600.);
    TH1D* hZvtx = new TH1D("hZvtx", "", 80, -20, 20);


    // 1-d and 2-d histograms to store trk info
    TH2D* htrkpteta = new TH2D("htrkpteta","htrkpteta",1000,0.,500.,50,-1.,1.);
    TH1D* hinvyield = new TH1D("hinvyield","hinvyield",nptbins,pTbins);
    TH1D* hNtrk     = new TH1D("hNtrk","hNtrk",nptbins,pTbins);
    

    
        
    int nevents = hea_tree->GetEntries(); // number of events
    cout << "Total number of events in those files: "<< nevents << endl;
    cout << endl;
    cout << "-------------------------------------------------" << endl;
        
    // Start loop over events ****************************************************
	
    for(int i = 0; i < nevents; i++)
      {
	
	if (i%100000==0) cout<<i<<" events passed "<<endl;    
	

	//Event information******************************************************

	hea_tree->SetBranchStatus("*", 0);
	Float_t vz;
	Float_t weight;
	Float_t pthat;

	hea_tree->SetBranchStatus("vz", 1);
	hea_tree->SetBranchAddress("vz", &vz);

	if(is_MC)
	  {
	    hea_tree->SetBranchStatus("weight", 1);
	    hea_tree->SetBranchAddress("weight", &weight);
	  
	    hea_tree->SetBranchStatus("pthat", 1);
	    hea_tree->SetBranchAddress("pthat", &pthat);
	  } 
	    
	//HLT information*********************************************************
	hlt_tree->SetBranchStatus("*", 0);

	Int_t HLT_AK4PFJet120_v8; //Flag for Jet120 trigger
	Int_t HLT_ZeroBias_v13;   //Flag for zero bias trigger
      
	hlt_tree->SetBranchStatus("HLT_ZeroBias_v13", 1);
        hlt_tree->SetBranchAddress("HLT_ZeroBias_v13", &HLT_ZeroBias_v13);

	hlt_tree->SetBranchStatus("HLT_AK4PFJet120_v8", 1);
        hlt_tree->SetBranchAddress("HLT_AK4PFJet120_v8", &HLT_AK4PFJet120_v8);

	//Skim tree information***************************************************

	ski_tree->SetBranchStatus("*", 0);

	Int_t HBHENoiseFilterResultRun2Loose;
	Int_t pprimaryVertexFilter;
	Int_t pBeamScrapingFilter;
	Int_t phfCoincFilter;
	Int_t pVertexFilterCutdz1p0;
	Int_t pVertexFilterCutGplus;
	Int_t pVertexFilterCutVtx1;

	//	ski_tree->SetBranchStatus("HBHENoiseFilterResultRun2Loose", 1);
	//ski_tree->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose);

	ski_tree->SetBranchStatus("pprimaryVertexFilter", 1);
	ski_tree->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);

	/*ski_tree->SetBranchStatus("pBeamScrapingFilter", 1);
	ski_tree->SetBranchAddress("pBeamScrapingFilter", &pBeamScrapingFilter);

	ski_tree->SetBranchStatus("phfCoincFilter",1);
        ski_tree->SetBranchAddress("phfCoincFilter",&phfCoincFilter);

	ski_tree->SetBranchStatus("pVertexFilterCutdz1p0",1);
	ski_tree->SetBranchAddress("pVertexFilterCutdz1p0",&pVertexFilterCutdz1p0);
	
	ski_tree->SetBranchStatus("pVertexFilterCutGplus",1);
	ski_tree->SetBranchAddress("pVertexFilterCutGplus",&pVertexFilterCutGplus);

	ski_tree->SetBranchStatus("pVertexFilterCutVtx1",1);
	ski_tree->SetBranchAddress("pVertexFilterCutVtx1",&pVertexFilterCutVtx1);

	*/
       
	jet_tree->SetBranchStatus("*", 0);

	//trk tree information ****************************************************

	trk_tree->SetBranchStatus("*",0);
	const int nmax = 10000;
	
	Int_t nTrk=0;
	//float trkPt[10000];
	std::vector<Float_t>*    trkPt = nullptr;
        std::vector<Float_t>* 	 trkPtError= nullptr;
	std::vector<Float_t>*    trkEta= nullptr;
	std::vector<Float_t>*    trkPhi= nullptr;
	std::vector<Int_t>*      trkNHits= nullptr;
	std::vector<Float_t>*    trkNormChi2= nullptr;
	std::vector<Bool_t>*     highPurity= nullptr;
	std::vector<Float_t>*    pfEnergy= nullptr;
	std::vector<Float_t>*    pfEcal= nullptr;
	std::vector<Float_t>*    pfHcal= nullptr;
	std::vector<Float_t>*    trkDzErrAssociatedVtx= nullptr;
	std::vector<Float_t>*    trkDxyErrAssociatedVtx= nullptr;
        
	trk_tree->SetBranchStatus("trkPt", 1);
	trk_tree->SetBranchAddress("trkPt", &trkPt);    

        trk_tree->SetBranchStatus("trkEta", 1);
        trk_tree->SetBranchAddress("trkEta", &trkEta);

	trk_tree->SetBranchStatus("trkPhi", 1);
        trk_tree->SetBranchAddress("trkPhi", &trkPhi);

	trk_tree->SetBranchStatus("nTrk", 1);
        trk_tree->SetBranchAddress("nTrk", &nTrk);

	trk_tree->SetBranchStatus("trkNormChi2", 1);
        trk_tree->SetBranchAddress("trkNormChi2", &trkNormChi2);

	trk_tree->SetBranchStatus("highPurity", 1);
        trk_tree->SetBranchAddress("highPurity", &highPurity);

	trk_tree->SetBranchStatus("trkPtError",1);
	trk_tree->SetBranchAddress("trkPtError",&trkPtError);

	trk_tree->SetBranchStatus("trkDzErrAssociatedVtx",1);
        trk_tree->SetBranchAddress("trkDzErrAssociatedVtx",&trkDzErrAssociatedVtx);

	trk_tree->SetBranchStatus("trkDxyErrAssociatedVtx",1);
        trk_tree->SetBranchAddress("trkDxyErrAssociatedVtx",&trkDxyErrAssociatedVtx);
	
	hea_tree->GetEntry(i);
	//return;
	hlt_tree->GetEntry(i);
	//	return;
	ski_tree->GetEntry(i);
	//jet_tree->GetEntry(i);
	//return;
	trk_tree->GetEntry(i);
	//return;
	//Event Cuts ***************************************************************
	if(vz <= -15. || vz >= 15.) continue; // vertex cut
	          
	if(pprimaryVertexFilter != 1) continue; //apply the skimmed event filters
 
      	if(nTrk <= 0) continue; // if there are no tracks in the event

	if(!HLT_ZeroBias_v13) continue; //Trigger cut
	//Fill Event histograms ***************************************************
        Float_t ptHatw=1;
	hNtrk->Fill(nTrk,ptHatw);
	hZvtx->Fill(vz,ptHatw);
	hEvents->Fill(1,ptHatw);

	//Start Analyzing tracks *************************************************
	for (int j = 0; j < nTrk; j++)  // Track loop start
	  {
	    //return;
	    Float_t trk_pt  =  trkPt->at(j);
	    Float_t trk_eta =  trkEta->at(j);
	    Float_t trk_phi =  trkPhi->at(j);
	    //Int_t   trk_NHits=  trkNHits->at(j);
	    Bool_t  isHighPurity= highPurity->at(j);
	    Float_t trk_pt_error= trkPtError->at(j);
	    Float_t trk_dzerror= trkDzErrAssociatedVtx->at(j);
	    Float_t trk_dxyerror= trkDxyErrAssociatedVtx->at(j);

	    
	    Float_t invyield_wt= (1./(4*TMath::Pi()*trk_pt)); //weight for the invariant yield

	    
	    //    return;
	    // Apply track cuts***************************************************
            if(!isHighPurity) continue;
	    if(abs(trk_eta) > 1.0) continue;
	    if(trk_pt<0.1) continue;
	    if(trk_dzerror > 3.0) continue;
	    if(trk_dxyerror > 3.0) continue;

	    //	    return;
	    htrkpteta->Fill(trk_pt,trk_eta);
	    hinvyield->Fill(trk_pt,invyield_wt);
	    //	    return;

	    //cout<<trk_pt<<"  "<<trk_eta<<" "<<endl;
	    



	  } // events loop end

      }		

    delete hea_tree;      
    delete hlt_tree;      
    delete ski_tree;
    delete jet_tree;
    delete trk_tree;
	
   
    TFile *fout = new TFile(outputFileName, "RECREATE");
        
    hEvents->Write();
    hNtrk->Write();
    hZvtx->Write();
    htrkpteta->Write();
    hinvyield->Write();
    //	return;
      

  
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


