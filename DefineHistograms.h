#include "call_libraries.h"

//Define the relevant histograms*********************************************

//variable pT bins
//    double pTbins[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};
//    double pTbins[]={4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};


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
TH1F* htrkDCAz  = new TH1F("htrkDCAz","htrkDCAz",600,-1.,1.);
TH1F* htrkDCAxy = new TH1F("htrkDCAxy","htrkDCAxy",600,-1.,1.);

TH1F* htrknoPUevtDCAz  = new TH1F("htrknoPUevtDCAz","htrknoPUevtDCAz",600,-1.,1.);
TH1F* htrknoPUevtDCAxy = new TH1F("htrknoPUevtDCAxy","htrknoPUevtDCAxy",600,-1.,1.);
    
TH2F* htrkpteta = new TH2F("htrkpteta","htrkpteta",1000,0.,1000.,200,-1.,1.);
TH1F* htrkpt    = new TH1F("htrkpt","htrkpt",nptbins,pTbins);
TH1F* htrketa   = new TH1F("htrketa","htrketa",200,-1.,1.);
TH1F* htrkinvyield = new TH1F("htrkinvyield","htrkinvyield",nptbins,pTbins);
TH1F* hNtrk     = new TH1F("hNtrk","hNtrk",nptbins,pTbins);
TH1F* htrkspervtx=new TH1F("htrkspervtx","htrkspervtx",400,0,400.); 

//Particle species correction
TH1F *hspeciescorr= new TH1F("hspeciescorr","hspeciescorr",80,0.1,20.);

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
TH2F* hpapmatchedpteta=new TH2F("hpapmatchedpteta","hpapmatchedpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hsigmaplusmatchedgenpteta=new TH2F("hsigmaplusmatchedgenpteta","hsigmaplusmatchedgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hsigmaminusmatchedgenpteta=new TH2F("hsigmaminusmatchedgenpteta","hsigmaminusmatchedgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* homegaminusmatchedgenpteta=new TH2F("homegaminusmatchedgenpteta","homegaminusmatchedgenpteta",nptbins,pTbins,200,-1.,1.);
    
TH2F* hpgenpteta=new TH2F("hpgenpteta","hpgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hapgenpteta=new TH2F("hapgenpteta","hapgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hpapgenpteta=new TH2F("hpapgenpteta","hpapgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hpiongenpteta=new TH2F("hpiongenpteta","hpiongenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hKgenpteta=new TH2F("hKgenpteta","hKgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hsigmaplusgenpteta=new TH2F("hsigmaplusgenpteta","hsigmaplusgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hsigmaminusgenpteta=new TH2F("hsigmaminusgenpteta","hsigmaminusgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* homegaminusgenpteta=new TH2F("homegaminusgenpteta","homegaminusgenpteta",nptbins,pTbins,200,-1.,1.);


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
void WriteHistograms()
{
  hEventsnoCuts->Write();    
  hEvents->Write();
  hNtrk->Write();
  hZvtx->Write();
  hnVtx->Write();
  hpthatW->Write();
  hDeltapT->Write();
  hDeltaR->Write();
  
  
  htrkDCAz->Write();
  htrkDCAxy->Write();
  htrknoPUevtDCAz->Write();
  htrknoPUevtDCAxy->Write();
  htrkspervtx->Write();
  
  
  
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
  hpapmatchedpteta->Write();
  hKmatchedpteta->Write();
  hpionmatchedpteta->Write();
  hsigmaplusmatchedgenpteta->Write();
  hsigmaminusmatchedgenpteta->Write();
  homegaminusmatchedgenpteta->Write();
  
  
  hspeciescorr->Write();
  
  
  hpgenpteta->Write();
  hpapgenpteta->Write();
  hapgenpteta->Write();
  hKgenpteta->Write();
  hpiongenpteta->Write();
  hsigmaplusgenpteta->Write();
  hsigmaminusgenpteta->Write();
  homegaminusgenpteta->Write();
  
  hjtpt->Write();
  hjtpteta->Write();
  hjtinvyield->Write();
  
  hjtphi->Write();
  hjteta->Write();
  
  hgentrkpteta->Write();
  hgentrkpt->Write();
  hgentrketa->Write();
}
