#include "TMath.h"

void rescaleHist(TH1* h) {
  for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++) {
    Double_t val = h->GetBinContent( iBin );
    Double_t valErr = h->GetBinError( iBin );
    Double_t binWidth = h->GetBinWidth( iBin );
    h->SetBinContent( iBin, val / binWidth );
    h->SetBinError( iBin, valErr / binWidth );
  }                                                                     
  //  h->Scale(1./h->Integral());
}
void set1DStyle(TH1 *h, Int_t type = 0) {
    Int_t markerStyle = 20; // Full circle
    Double_t markerSize = 1.3;
    Int_t lineWidth = 2;
    Int_t color = 2;
    if (type == 0) {
        color = 2;        // red
	markerStyle = 107; // filled circle
    }

    else if (type == 1) {
      color = 4;        // blue
      markerStyle = 107; // open circle
    }
    else if (type == 2) {
      color = 1;        // black
      markerStyle = 107; // filled triangle
    }
    else if (type == 3) {
      color = 2;        // red
      markerStyle = 24; // open circle
    }
    else if (type == 4) {
      color = 4;        // blue
      markerStyle = 20; // filled circle
    }
    else {
      color = 6;        // magenta
      markerStyle = 30; // open star
    }

h->SetLineWidth( lineWidth );
h->SetLineColor( color );

h->SetMarkerStyle( markerStyle );
h->SetMarkerColor( color );
h->SetMarkerSize( markerSize );
h->SetStats(kFALSE);
h->SetTitle("");
h->GetYaxis()->SetTitleSize(0.03);
h->GetYaxis()->SetLabelSize(0.03);
h->GetXaxis()->SetTitleSize(0.03);
h->GetXaxis()->SetLabelSize(0.03);
h->GetXaxis()->SetNdivisions(208);
h->GetYaxis()->SetNdivisions(208);
h->GetYaxis()->SetTitleOffset(1.2);
h->GetXaxis()->SetTitleOffset(1.2);
h->GetYaxis()->CenterTitle();
h->GetXaxis()->CenterTitle();
}


void hinvyield()
{
  TFile *f1=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefzerobiasout.root","READ");
  
  TFile *f3=TFile::Open("pp5020datapoints.root");
  
  
  TH1D *hinvzerobias      = (TH1D*) f1->Get("htrkinvyield");
  TH1D *hinvzerobias_corr = (TH1D*) f1->Get("htrkinvyield_corr");
  TH1D *hEvents           = (TH1D*) f1->Get("hEvents");
  
  TH1D *hpub5020     = (TH1D*) f3->Get("hout");

  cout<<hinvzerobias_corr->GetXaxis()->GetNbins()<<" "<<hpub5020->GetXaxis()->GetNbins();
  hpub5020->Scale(70.); //Cross section 


  rescaleHist(hinvzerobias);
  rescaleHist(hinvzerobias_corr);

  //Int_t NEvents = hEvents->Integral();

  //Int_t NEvents=168000000;
  //Float_t lumi=2466200;
  Float_t lumi=(1066099.978894705*1000)/25;
  //  cout<<"Total number of events  "<<NEvents<<endl;
  hinvzerobias_corr->Scale(1./lumi);
  hinvzerobias->Scale(1./lumi);

  // hinvzerobias_corr->Draw();
  //  return;
  /*  TH1D *hRatio= (TH1D*) hinvzerobias_corr->Clone("hinvzerobias_corr");
  hRatio->Divide(hinvzerobias);
  set1DStyle(hRatio);
  */
  TF1 *fa1 = new TF1("fa1", "1.04 + 0.0256*TMath::Log(x) + 0.0127*TMath::Power(TMath::Log(x),2) - 0.00472*TMath::Power(TMath::Log(x),3) + 0.00048*TMath::Power(TMath::Log(x),4)", 0.1, 100);

  TLatex t;
  TH1D *hRatio= (TH1D*) hinvzerobias_corr->Clone("hinvzerobias_corr");                     
  hRatio->Divide(hpub5020);
  set1DStyle(hRatio);

  TCanvas *c1= new TCanvas("c1","c1",800,800);

  c1->SetLogx();
  hRatio->GetYaxis()->SetTitle("Ratio(5.36 TeV / 5.02 TeV)");
  hRatio->GetXaxis()->SetTitle("Track pT");
  set1DStyle(hRatio,2);
  hRatio->GetYaxis()->SetRangeUser(0.8,1.5);
  hRatio->GetXaxis()->SetRangeUser(0.1,100.);
  hRatio->Draw("p");

  fa1->SetLineColor(kRed);
  fa1->SetLineWidth(2);
  fa1->Draw("same");

  TLegend *legend1 = new TLegend(0.15,0.7,0.36,0.86);
  //legend->AddEntry(hinvzerobias,"Zerobias (without correction)","p");                      
  legend1->AddEntry(hRatio,"ppref Ratio (5.36 / 5.02)","lp");
  legend1->AddEntry(fa1,"Published PYTHIA 8 Ratio (5.44/5.02)","l");
  legend1->SetBorderSize(0);
  legend1->SetTextFont(22);
  legend1->SetTextSize(0.03);
  legend1->Draw();
  
  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Preliminary}");
  t.DrawLatexNDC(0.6, 0.93, "pp 5.36 TeV");
  c1->SaveAs("../Plots/Ratio_invzerobias.png");
  
  TCanvas *c = new TCanvas("c","c",800,800);
  c->SetLogy(1);
  c->SetLogx(1);
  hinvzerobias_corr->GetYaxis()->SetRangeUser(10e-17,1000);
  hinvzerobias_corr->GetXaxis()->SetTitle("p_{T} (GeV)");
  hinvzerobias_corr->GetYaxis()->SetTitle("#frac{d#sigma^{trk}}{dp_{T}}");
  set1DStyle(hinvzerobias,0);
  set1DStyle(hinvzerobias_corr,1);
  set1DStyle(hpub5020,2);
   
  //hinvzerobias->Draw("p");
  hinvzerobias_corr->Draw("psame");
  hpub5020->Draw("psame");

  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Preliminary}");
  t.DrawLatexNDC(0.6, 0.93, "pp 5.36 TeV");
  
  TLegend *legend = new TLegend(0.55,0.7,0.8,0.86);
  //legend->AddEntry(hinvzerobias,"Zerobias (without correction)","p");
  legend->AddEntry(hinvzerobias_corr,"Zerobias","p");
  legend->AddEntry(hpub5020,"pp 5020 published","p");
  legend->SetBorderSize(0);
  legend->SetTextSize(0.04);
  legend->Draw();
  c->SaveAs("../Plots/hinvzerobias.png");



}
