#include "include_libraries.h"

       //Event info variables 
        Float_t vz;
	Float_t weight;
	Float_t pthat;
        Int_t nVtx;

	//Trigger variables
	Int_t HLT_AK4PFJet120_v8; //Flag for Jet120 trigger
	Int_t HLT_ZeroBias_v13;   //Flag for zero bias trigger

	//skim filters
	Int_t HBHENoiseFilterResultRun2Loose;
	Int_t pprimaryVertexFilter;
	Int_t pBeamScrapingFilter;
	Int_t phfCoincFilter;
	Int_t pVertexFilterCutdz1p0;
	Int_t pVertexFilterCutGplus;
	Int_t pVertexFilterCutVtx1;

        //Gen info variables
        Int_t Ncoll; 
        Int_t Npart;
        
        std::vector<Float_t>*    gentrkPt = nullptr;
        std::vector<Float_t>*    gentrkEta= nullptr;
        std::vector<Float_t>*    gentrkPhi= nullptr;


	//Track info variables
        Int_t nTrk=0;
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
        std::vector<Float_t>*    trkDzAssociatedVtx;
        std::vector<Float_t>*    trkDxyAssociatedVtx;


	//jet info variables
	Int_t nref;         // number of jets
	Float_t  jteta[9999]; // jet eta
	Float_t  jtphi[9999]; // jet phi
	Float_t  rawpt[9999]; // jet pT without JEC
	Float_t  trackMax[9999]; // track maximum pT in a jet
	


void read_trees(TChain *tree,bool is_MC){

 
	//reading event information from trees


        tree->SetBranchStatus("*",0);
	
	tree->SetBranchStatus("vz", 1);
	tree->SetBranchAddress("vz", &vz);

	tree->SetBranchStatus("nVtx",1);
	tree->SetBranchAddress("nVtx",&nVtx);
	if(is_MC)
	  {
	    tree->SetBranchStatus("weight", 1);
	    tree->SetBranchAddress("weight", &weight);
	  
	    tree->SetBranchStatus("pthat", 1);
	    tree->SetBranchAddress("pthat", &pthat);

	    tree->SetBranchStatus("pt",1);
	    tree->SetBranchAddress("pt",&gentrkPt);

	    tree->SetBranchStatus("eta",1);
            tree->SetBranchAddress("eta",&gentrkEta);

	    tree->SetBranchStatus("phi",1);
            tree->SetBranchAddress("phi",&gentrkPhi);

	    //tree->SetBranchStatus("ncoll",1);
	    //tree->SetBranchAddress("ncoll",&Ncoll);

	    //tree->SetBranchStatus("npart",1);
            //tree->SetBranchAddress("npart",&Npart);
	    
	  } 
	    
	//reading HLT information from trees
	
	tree->SetBranchStatus("HLT_ZeroBias_v13", 1);
        tree->SetBranchAddress("HLT_ZeroBias_v13", &HLT_ZeroBias_v13);

	tree->SetBranchStatus("HLT_AK4PFJet120_v8", 1);
        tree->SetBranchAddress("HLT_AK4PFJet120_v8", &HLT_AK4PFJet120_v8);

	//reading skim tree information

	


	//	tree->SetBranchStatus("HBHENoiseFilterResultRun2Loose", 1);
	//tree->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose);

	tree->SetBranchStatus("pprimaryVertexFilter", 1);
	tree->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);

	//tree->SetBranchStatus("pBeamScrapingFilter", 1);
	//tree->SetBranchAddress("pBeamScrapingFilter", &pBeamScrapingFilter);

	//tree->SetBranchStatus("phfCoincFilter",1);
        //tree->SetBranchAddress("phfCoincFilter",&phfCoincFilter);

	//tree->SetBranchStatus("pVertexFilterCutdz1p0",1);
	//tree->SetBranchAddress("pVertexFilterCutdz1p0",&pVertexFilterCutdz1p0);
	
	//tree->SetBranchStatus("pVertexFilterCutGplus",1);
	//tree->SetBranchAddress("pVertexFilterCutGplus",&pVertexFilterCutGplus);

	//tree->SetBranchStatus("pVertexFilterCutVtx1",1);
	//tree->SetBranchAddress("pVertexFilterCutVtx1",&pVertexFilterCutVtx1);

	
      

	//track tree information ****************************************************

	
	int nmax = 10000;

	tree->SetBranchStatus("trkPt", 1);
	tree->SetBranchAddress("trkPt", &trkPt);    

        tree->SetBranchStatus("trkEta", 1);
        tree->SetBranchAddress("trkEta", &trkEta);

	tree->SetBranchStatus("trkPhi", 1);
        tree->SetBranchAddress("trkPhi", &trkPhi);

	tree->SetBranchStatus("nTrk", 1);
        tree->SetBranchAddress("nTrk", &nTrk);

	tree->SetBranchStatus("trkNormChi2", 1);
        tree->SetBranchAddress("trkNormChi2", &trkNormChi2);

	tree->SetBranchStatus("highPurity", 1);
        tree->SetBranchAddress("highPurity", &highPurity);

	tree->SetBranchStatus("trkPtError",1);
	tree->SetBranchAddress("trkPtError",&trkPtError);

	tree->SetBranchStatus("trkDzErrAssociatedVtx",1);
        tree->SetBranchAddress("trkDzErrAssociatedVtx",&trkDzErrAssociatedVtx);

	tree->SetBranchStatus("trkDxyErrAssociatedVtx",1);
        tree->SetBranchAddress("trkDxyErrAssociatedVtx",&trkDxyErrAssociatedVtx);

	tree->SetBranchStatus("trkDzAssociatedVtx",1);
	tree->SetBranchAddress("trkDzAssociatedVtx",&trkDzAssociatedVtx);

	tree->SetBranchStatus("trkDxyAssociatedVtx",1);
	tree->SetBranchAddress("trkDxyAssociatedVtx",&trkDxyAssociatedVtx);
	//jet tree information
	tree->SetBranchStatus("nref", 1);
	tree->SetBranchAddress("nref",&nref);

	tree->SetBranchStatus("rawpt",1);
	tree->SetBranchAddress("rawpt",&rawpt);

	tree->SetBranchStatus("jteta",1);
	tree->SetBranchAddress("jteta",&jteta);

	tree->SetBranchStatus("jtphi",1);
	tree->SetBranchAddress("jtphi",&jtphi);

	tree->SetBranchStatus("trackMax",1);
	tree->SetBranchAddress("trackMax",&trackMax);




}
