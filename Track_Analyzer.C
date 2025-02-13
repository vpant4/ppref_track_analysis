#include "include_libraries.h" //header file with important libraries
#include "read_trees.h"


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

    hlt_tree->AddFriend(trk_tree);
    hlt_tree->AddFriend(hea_tree);
    hlt_tree->AddFriend(jet_tree);
    hlt_tree->AddFriend(ski_tree);
  	

    //Define the relevant histograms*********************************************

    //variable pT bins
    double pTbins[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.,600.,1000.};
    int nptbins=sizeof(pTbins) / sizeof(pTbins[0]) - 1;
    //Event level histograms
    TH1D* hEventsnoCuts= new TH1D("hEventsnoCuts","",10,0,10);
    TH1D* hEvents = new TH1D("hEvents", "", 10, 0, 10);
    TH1D* hpthat = new TH1D("hpthat", "", 200, 0, 600.);
    TH1D* hpthatW = new TH1D("hpthatW", "", 200, 0, 600.);
    TH1D* hZvtx = new TH1D("hZvtx", "", 80, -20, 20);

    // 1-d and 2-d histograms to store trk info
    TH2D* htrkpteta = new TH2D("htrkpteta","htrkpteta",1000,0.,1000.,50,-2.,2.);
    TH1D* htrkpt    = new TH1D("htrkpt","htrkpt",nptbins,pTbins);
    TH1D* htrkinvyield = new TH1D("htrkinvyield","htrkinvyield",nptbins,pTbins);
    TH1D* hNtrk     = new TH1D("hNtrk","hNtrk",nptbins,pTbins);

    //Histograms to store jet information
    TH2D *hjtpteta  = new TH2D("hjtpteta","hjtpteta",1000,0.,1000.,50,-2.,2.);
    TH1D *hjtpt     = new TH1D("hjtpt","hjtpt",nptbins,pTbins);
    TH1D *hjtinvyield=new TH1D("hjtinvyield","hjtinvyield",nptbins,pTbins);
    TH1D *hjteta    = new TH1D("hjteta","hjteta",1000,-2.,2.);
    TH1D *hjtphi    = new TH1D("hjtphi","hjtphi",1000,-TMath::Pi(),TMath::Pi());

    
        
    int nevents = hlt_tree->GetEntries(); // number of events
    cout << "Total number of events in those files: "<< nevents << endl;
    cout << endl;
    cout << "-------------------------------------------------" << endl;
        
    // Start loop over events ****************************************************
	
    for(int i = 0; i < nevents; i++)
      {
	hEventsnoCuts->Fill(1);
	if (i%100000==0) cout<<i<<" events passed "<<endl;    
	

	//Read trees******************************************************

	read_trees(hlt_tree,is_MC);
	
	
	hlt_tree->GetEntry(i);
	
	//Event Cuts ***************************************************************
	if(vz <= -15. || vz >= 15.) continue; // vertex cut
	          
	if(pprimaryVertexFilter != 1) continue; //apply the skimmed event filters
 
      	if(nTrk <= 0) continue; // if there are no tracks in the event

	//	if(!HLT_AK4PFJet120_v8) continue; //Trigger cut
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

	    
	    Float_t invyield_wt= (1./(4*2*TMath::Pi()*trk_pt)); //weight for the invariant yield

	    
	    //    return;
	    // Apply track cuts***************************************************
            if(!isHighPurity) continue;
	    if(abs(trk_eta) > 2.0) continue;
	    if(trk_pt<0.1) continue;
	    if(trk_dzerror > 3.0) continue;
	    if(trk_dxyerror > 3.0) continue;

	    htrkpteta->Fill(trk_pt,trk_eta);
	    htrkinvyield->Fill(trk_pt,invyield_wt);
	    htrkpt->Fill(trk_pt);
	    
	  } // events loop end

	//jet loop starts
	int njets=(int) nref;
	for (int j=0; j<njets; j++)
	  {

	    if(trackMax[j]/rawpt[j] < 0.05)continue; // jet cut for very low pT particles
	    if(trackMax[j]/rawpt[j] > 0.95)continue; // jets where all the pT is taken by one track

	    float jet_rawpt = rawpt[j];
	    float jet_eta = jteta[j];
	    float jet_phi = jtphi[j];

	    if (abs(jet_eta)>2) continue;
            Float_t hjtinvyieldwt=(1/(4*2*TMath::Pi()))*jet_rawpt;
	    hjtpteta->Fill(jet_rawpt,jet_eta);
	    hjtinvyield->Fill(jet_rawpt,hjtinvyieldwt);
	    hjtpt->Fill(jet_rawpt);
	    hjteta->Fill(jet_eta);
	    hjtphi->Fill(jet_phi);
	    
	    
	  }//jet loop ends
	
	
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

    htrkpt->Write();
    htrkpteta->Write();
    htrkinvyield->Write();

    hjtpt->Write();
    hjtpteta->Write();
    hjtinvyield->Write();
    
    hjtphi->Write();
    hjteta->Write();
  
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


