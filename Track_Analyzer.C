#include "include_libraries.h" //header file with important libraries
#include "read_trees.h"


void Track_Analyzer(TString input_file, TString outputFileName,Bool_t is_MC,Float_t pthat_value)
{
  
  TH1::SetDefaultSumw2();
  TH2::SetDefaultSumw2();
  TH3::SetDefaultSumw2();


    //Read the track efficiency file

    //    TFile *feff= TFile::Open("/afs/cern.ch/user/v/vpant/pprefanalysis/filelists/Eff/ppref_2024_Pythia_minBias_NopU_2D_Nominla.root","READ");

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

    
    hlt_tree->AddFriend(trk_tree);
    hlt_tree->AddFriend(hea_tree);
    hlt_tree->AddFriend(jet_tree);
    hlt_tree->AddFriend(ski_tree);
    if (is_MC)
      {hlt_tree->AddFriend(gen_tree);}


    //Define the relevant histograms*********************************************

    //variable pT bins
    double pTbins[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};
    int nptbins=sizeof(pTbins) / sizeof(pTbins[0]) - 1;
    //Event level histograms
    TH1D* hEventsnoCuts= new TH1D("hEventsnoCuts","",10,0,10);
    TH1D* hnVtx         = new TH1D("hnVtx","hnVtx",500,0,20);
    
    TH1D* hEvents = new TH1D("hEvents", "", 10, 0, 10);
    TH1D* hpthatW = new TH1D("hpthatW", "", 200, 0, 600.);
    TH1D* hZvtx = new TH1D("hZvtx", "", 80, -20, 20);

    // 1-d and 2-d histograms to store trk info
    TH2D* htrkpteta = new TH2D("htrkpteta","htrkpteta",1000,0.,1000.,50,-2.,2.);
    TH1D* htrkpt    = new TH1D("htrkpt","htrkpt",nptbins,pTbins);
    TH1D* htrkinvyield = new TH1D("htrkinvyield","htrkinvyield",nptbins,pTbins);
    TH1D* hNtrk     = new TH1D("hNtrk","hNtrk",nptbins,pTbins);

    //Histograms to store gen track information (for MC)
    TH2D* hgentrkpteta = new TH2D("hgentrkpteta","hgentrkpteta",1000,0.,1000.,50,-2.,2.);
    TH1D *hgentrkpt = new TH1D("hgentrkpt","hgentrkpt",nptbins,pTbins);
    TH1D* hgenNtrk     = new TH1D("hgenNtrk","hgenNtrk",nptbins,pTbins);

    
    //Histograms with track corrections
    TH2D* htrkpteta_corr = new TH2D("htrkpteta_corr","htrkpteta_corr",1000,0.,1000.,50,-2.,2.);
    TH1D* htrkpt_corr    = new TH1D("htrkpt_corr","htrkpt_corr",nptbins,pTbins);
    TH1D* htrkinvyield_corr = new TH1D("htrkinvyield_corr","htrkinvyield_corr",nptbins,pTbins);
    
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
	if (i%10000==0) cout<<i<<" events passed "<<endl;    
	

	//Read trees******************************************************

	read_trees(hlt_tree,is_MC);
	
	
	hlt_tree->GetEntry(i);
	
	//Event Cuts ***************************************************************
	if(vz <= -15. || vz >= 15.) continue; // vertex cut

	cout<<"here  "<<endl;
	//if(pprimaryVertexFilter != 1) continue; //apply the skimmed event filters
	cout<<"here 1  "<<endl;
 
      	if(nTrk <= 0) continue; // if there are no tracks in the event

	//if(!HLT_AK4PFJet120_v8) continue; //Trigger cut
	//Fill Event histograms ***************************************************
        Float_t ptHatw=1;
	hNtrk->Fill(nTrk,ptHatw);
	hZvtx->Fill(vz,ptHatw);
	hEvents->Fill(1,ptHatw);
	hnVtx->Fill(nVtx,ptHatw);
	

	//Start Analyzing reco tracks *************************************************
	for (int j = 0; j < nTrk; j++)  // Track loop start
	  {
	    //Read the required track variables
	    Float_t trk_pt      =  trkPt->at(j);
	    Float_t trk_eta     =  trkEta->at(j);
	    Float_t trk_phi     =  trkPhi->at(j);
	    Bool_t  isHighPurity=  highPurity->at(j);
	    Float_t trk_pt_error=  trkPtError->at(j);
	    Float_t trk_dxy     =  trkDxyAssociatedVtx->at(j);
	    Float_t trk_dxyerror=  trkDxyErrAssociatedVtx->at(j);
	    Float_t trk_dz      =  trkDzAssociatedVtx->at(j);
	    Float_t trk_dzerror =  trkDzErrAssociatedVtx->at(j);

	    
	    //    return;
	    // Apply track cuts***************************************************
            if(!isHighPurity) continue;
	    if(abs(trk_eta) > 1.0) continue;
	    if(trk_pt<0.1) continue;
	    if(abs(trk_dxy/trk_dxyerror) >= 3.0) continue;
	    if(abs(trk_dz/trk_dzerror)   >= 3.0) continue;
	    if(trk_pt>10 && abs(trk_pt_error/trk_pt)>=0.1) continue;
	    
	    	    //Read the values from efficiency histograms
	    Float_t eff = hEff_2D->GetBinContent(hEff_2D->GetXaxis()->FindBin(trk_eta),hEff_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t fak = hFak_2D->GetBinContent(hFak_2D->GetXaxis()->FindBin(trk_eta),hFak_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t sec = hSec_2D->GetBinContent(hSec_2D->GetXaxis()->FindBin(trk_eta),hSec_2D->GetYaxis()->FindBin(trk_pt));
	    Float_t mul = hMul_2D->GetBinContent(hMul_2D->GetXaxis()->FindBin(trk_eta),hMul_2D->GetYaxis()->FindBin(trk_pt));

	    //Correction factors for tracks
	    Float_t corr_wt=((1.0 - fak)*(1.0 - sec))/(eff*(1.0 + mul));

	    Float_t weight=1.;
	    weight=corr_wt;
	    cout<<trk_pt<<" "<<trk_eta<<" "<<"Eff.."<<eff<<"Sec...."<<sec<<"fak...."<<fak<<"mul....."<<mul<<" Correction factor is:  "<<weight<<endl;
	    //Weight for the invariant yield 
	    Float_t invyield_wt= (1./(2*2*TMath::Pi()*trk_pt)); 
	      

	    htrkpteta->Fill(trk_pt,trk_eta);
	    htrkinvyield->Fill(trk_pt,invyield_wt);
	    htrkpt->Fill(trk_pt);

	    htrkpteta_corr->Fill(trk_pt,trk_eta,weight);
            htrkinvyield_corr->Fill(trk_pt,invyield_wt*weight);
            htrkpt_corr->Fill(trk_pt,weight);
	  }//Reco track loop end

	/*	//jet loop starts
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
	
	*/

	//Gen loop starts (Only for MC) **************************************
	Int_t gentrksize;
        
	if (is_MC)
	  {gentrksize = gentrkPt->size();
	    for (int j=0;j<gentrksize;j++)
	      {
	       
		//if (!gentrkPt->size()||!gentrkEta->size()||!gentrkPhi->size()) continue;
	 	Float_t gentrk_pt = gentrkPt->at(j);
		Float_t gentrk_eta= gentrkEta->at(j);
		Float_t gentrk_phi= gentrkPhi->at(j);
	      
		//cout<<gentrk_pt<<endl;
	      
		//Track cuts on gen tracks
		if(abs(gentrk_eta) > 1.0) continue;
		if(gentrk_pt<0.1) continue;
	    
		hgentrkpteta->Fill(gentrk_pt,gentrk_eta);
		hgentrkpt->Fill(gentrk_pt);
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

    htrkpt->Write();
    htrkpteta->Write();
    htrkinvyield->Write();

    htrkpt_corr->Write();
    htrkpteta_corr->Write();
    htrkinvyield_corr->Write();

    hjtpt->Write();
    hjtpteta->Write();
    hjtinvyield->Write();
    
    hjtphi->Write();
    hjteta->Write();

    hgentrkpteta->Write();
    hgentrkpt->Write();
  
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


