#include "call_libraries.h"

//Define the relevant histograms*********************************************

//variable pT bins
double pTbins[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};
double pTbins_split[]={0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.4,1.6,1.8,2.0,2.2,2.4,3.2,4.0,4.8,5.6,6.4,7.2,8.4,9.6,10.8,12.,14.4,15.8,17.6,19.2,22.,24.,26.2,28.8,32.0,35.2,38.0,41.6,43.6,48.,54.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};
//    double pTbins[]={4.8,5.6,6.4,7.2,9.6,12.,14.4,19.2,24.,28.8,35.2,41.6,48.,60.8,73.6,86.4,103.6,120.8,140.,165.,250.,400.};


/*double pTbins[]={0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45,
  0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95,
  1.0, 1.05, 1.1, 1.15, 1.2,
  1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2.0,
  2.5, 3.0, 4.0, 5.0, 7.5, 10.0, 12.0, 15.0,
  20.0, 25.0, 30.0, 45.0, 60.0, 90.0, 120.0, 
  180.0, 300.0, 500.0};
*/  
    
int nptbins=sizeof(pTbins) / sizeof(pTbins[0]) - 1;
int nptbins_split=sizeof(pTbins_split) / sizeof(pTbins_split[0]) - 1;
//Event level histograms
TH1F* hEventsnoCuts= new TH1F("hEventsnoCuts","",10,0,10);
TH1F* hnVtx         = new TH1F("hnVtx","hnVtx",500,0,20);

TH1F* hEvents = new TH1F("hEvents", "", 10, 0, 10);
TH1F* hpthatW = new TH1F("hpthatW", "", 200, 0, 600.);
TH1F* hZvtx = new TH1F("hZvtx", "", 80, -20, 20);

// 1-d and 2-d histograms to store trk info
TH1F* htrkDCAz  = new TH1F("htrkDCAz","htrkDCAz",600,-1.,1.);
TH1F* htrkDCAxy = new TH1F("htrkDCAxy","htrkDCAxy",600,-1.,1.);
TH1F* htrkNhits = new TH1F("htrkNhits","htrkNhits",50,0.,50.);
TH1F* htrkNormChi= new TH1F("htrkNormChi","htrkNormChi",500,0.,1.);

TH1F* htrknoPUevtDCAz  = new TH1F("htrknoPUevtDCAz","htrknoPUevtDCAz",600,-1.,1.);
TH1F* htrknoPUevtDCAxy = new TH1F("htrknoPUevtDCAxy","htrknoPUevtDCAxy",600,-1.,1.);

TH1F* htrkcounts = new TH1F("htrkcounts","htrkcounts",15,0,15);
    
TH2F* htrkpteta = new TH2F("htrkpteta","htrkpteta",1000,0.,1000.,48,-2.4,2.4);
TH1F* htrkpt    = new TH1F("htrkpt","htrkpt",nptbins,pTbins);
TH1F* htrkpt_split=new TH1F("htrkpt_split","htrkpt_split",nptbins_split,pTbins_split);
TH1F* htrketa   = new TH1F("htrketa","htrketa",48,-2.4,2.4);
TH1F* htrkphi   = new TH1F("htrkphi","htrkphi",1000,-TMath::Pi(),TMath::Pi());
TH1F* htrkinvyield = new TH1F("htrkinvyield","htrkinvyield",nptbins,pTbins);
TH1F* htrkinvyield_split = new TH1F("htrkinvyield_split","htrkinvyield_split",nptbins_split,pTbins_split);

TH1F* hEcal= new TH1F("hEcal","hEcal",1000,0.,500);
TH1F* hHcal= new TH1F("hHcal","hHcal",1000,0.,500);

TH1F* hEt = new TH1F("hEt","hEt",1000,0.,500.);
TH1F* hEt_pt = new TH1F("hEt_pt","hEt_pt",1000,0.,5.);

TH1F* hNtrk     = new TH1F("hNtrk","hNtrk",nptbins,pTbins);
TH1F* htrkspervtx=new TH1F("htrkspervtx","htrkspervtx",400,0,400.); 

//Particle species correction
TH1F *hspeciescorr= new TH1F("hspeciescorr","hspeciescorr",80,0.1,20.);

//Gen matching information
TH1F* hgenmatchedpt= new TH1F("hgenmatchedpt","hgenmatchedpt",nptbins,pTbins);
TH1F* hgenmatchedeta= new TH1F("hgenmatchedeta","hgenmatchedeta",48,-2.4,2.4);
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
TH2F* hremaindermatchedgenpteta=new TH2F("hremaindermatchedgenpteta","hremaindermatchedgenpteta",nptbins,pTbins,200,-1.,1.);
    
TH2F* hpgenpteta=new TH2F("hpgenpteta","hpgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hapgenpteta=new TH2F("hapgenpteta","hapgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hpapgenpteta=new TH2F("hpapgenpteta","hpapgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hpiongenpteta=new TH2F("hpiongenpteta","hpiongenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hKgenpteta=new TH2F("hKgenpteta","hKgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hsigmaplusgenpteta=new TH2F("hsigmaplusgenpteta","hsigmaplusgenpteta",nptbins,pTbins,200,-1.,1.);
TH2F* hsigmaminusgenpteta=new TH2F("hsigmaminusgenpteta","hsigmaminusgenpteta",nptbins,pTbins,200,-1.,1.);

TH2F* hremaindergenpteta=new TH2F("hremaindergenpteta","hremaindergenpteta",nptbins,pTbins,200,-1.,1.);

//Histograms to store gen track information (for MC)
TH2F* hgentrkpteta = new TH2F("hgentrkpteta","hgentrkpteta",nptbins,pTbins,48,-2.4,2.4);
TH1F *hgentrkpt = new TH1F("hgentrkpt","hgentrkpt",nptbins,pTbins);
TH1F *hgentrketa= new TH1F("hgentrketa","hgentrketa",200,-2.4,2.4);
TH1F* hgenNtrk     = new TH1F("hgenNtrk","hgenNtrk",nptbins,pTbins);
    
//Histograms with track corrections
    
TH2F* htrkpteta_corr = new TH2F("htrkpteta_corr","htrkpteta_corr",1000,0.,1000.,48,-2.4,2.4);
TH1F* htrkpt_corr    = new TH1F("htrkpt_corr","htrkpt_corr",nptbins,pTbins);
TH1F* htrketa_corr    = new TH1F("htrketa_corr","htrketa_corr",48,-2.4,2.4);
TH1F* htrkinvyield_corr = new TH1F("htrkinvyield_corr","htrkinvyield_corr",nptbins,pTbins);
TH1F* htrkinvyield_split_corr = new TH1F("htrkinvyield_split_corr","htrkinvyield_split_corr",nptbins_split,pTbins_split);

//Systematics-specific histograms
TH1F* htrkinvyieldhighPU = new TH1F("htrkinvyieldhighPU","htrkinvyieldhighPU",nptbins,pTbins);
TH1F* htrkinvyieldlowPU = new TH1F("htrkinvyieldlowPU","htrkinvyieldlowPU",nptbins,pTbins);

TH1F* htrkinvyieldsplithighPU = new TH1F("htrkinvyieldsplithighPU","htrkinvyieldsplithighPU",nptbins_split,pTbins_split);
TH1F* htrkinvyieldsplitlowPU = new TH1F("htrkinvyieldsplitlowPU","htrkinvyieldsplitlowPU",nptbins_split,pTbins_split);

TH1F* htrkinvyieldeffsyst[2];
TH1F* htrkinvyieldspliteffsyst[2];
for (int i = 0; i < 2; ++i)
  {
    htrkinvyieldeffsyst[i] = new TH1F(Form("htrkinvyieldeffsyst_%i",i),Form("htrkinvyieldeffsyst_%i",i), nptbins, pTbins);
    htrkinvyieldspliteffsyst[i] = new TH1F(Form("htrkinvyieldspliteffsyst_%i",i),Form("htrkinvyieldspliteffsyst_%i",i), nptbins_split, pTbins_split);
  }
TH1F* htrkinvyieldsmeared= new TH1F("htrkinvyieldsmeared","htrkinvyieldsmeared",nptbins,pTbins);
TH1F* htrkinvyieldsplitsmeared= new TH1F("htrkinvyieldsplitsmeared","htrkinvyieldsplitsmeared",nptbins_split,pTbins_split);


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
  htrkNhits->Write();   
  htrkNormChi->Write();
  htrknoPUevtDCAxy->Write();
  htrkspervtx->Write();
  
  
  
  htrkpt->Write();
  htrkpt_split->Write();
  htrketa->Write();
  htrkphi->Write();
  htrkpteta->Write();
  htrkinvyield->Write();
  htrkinvyield_split->Write();
  hEcal->Write();
  hHcal->Write();
  hEt->Write();
  hEt_pt->Write();
  
  htrkpt_corr->Write();
  htrketa_corr->Write();
  htrkpteta_corr->Write();
  htrkinvyield_corr->Write();
  htrkinvyield_split_corr->Write();
  htrkcounts->Write();

  //systematics-specific histograms:
  htrkinvyieldhighPU->Write();
  htrkinvyieldlowPU->Write();
  htrkinvyieldsplithighPU->Write();
  htrkinvyieldsplitlowPU->Write();
  for (int i = 0; i < 2; ++i)
  {
    htrkinvyieldeffsyst[i]->Write();
    htrkinvyieldspliteffsyst[i]->Write();
  }
  htrkinvyieldsmeared->Write();
  htrkinvyieldsplitsmeared->Write();
  
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
  
  hremaindermatchedgenpteta->Write();
  
  
  hspeciescorr->Write();
  
  
  hpgenpteta->Write();
  hpapgenpteta->Write();
  hapgenpteta->Write();
  hKgenpteta->Write();
  hpiongenpteta->Write();
  hsigmaplusgenpteta->Write();
  hsigmaminusgenpteta->Write();

  hremaindergenpteta->Write();
  
  hjtpt->Write();
  hjtpteta->Write();
  hjtinvyield->Write();
  
  hjtphi->Write();
  hjteta->Write();
  
  hgentrkpteta->Write();
  hgentrkpt->Write();
  hgentrketa->Write();
}
