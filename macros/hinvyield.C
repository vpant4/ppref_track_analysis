void rescaleHist(TH1* h) {
  for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++) {
    Double_t val = h->GetBinContent( iBin );
    Double_t valErr = h->GetBinError( iBin );
    Double_t binWidth = h->GetBinWidth( iBin );
    h->SetBinContent( iBin, val / binWidth );
    h->SetBinError( iBin, valErr / binWidth );
  }                                                                     
  h->Scale( 1. / h->Integral() );
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
      markerStyle = 75; // filled triangle
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
h->GetYaxis()->SetTitleOffset(1.1);
h->GetYaxis()->CenterTitle();
h->GetXaxis()->CenterTitle();
}


void hinvyield()
{
  TFile *f1=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefzerobiasout.root","READ");
  TFile *f2=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefjet120out.root","READ");
  TFile *f3=TFile::Open("pp5020datapoints.root");
  
  
  TH1D *hinvzerobias = (TH1D*) f1->Get("hinvyield");
  TH1D *hinvjet120   = (TH1D*) f2->Get("hinvyield");
  TH1D *hpub5020     = (TH1D*) f3->Get("hout");


  rescaleHist(hinvzerobias);
  rescaleHist(hinvjet120);
  TCanvas *c = new TCanvas("c","c",800,800);
  c->SetLogy(1);
  c->SetLogx(1);
  hinvzerobias->GetYaxis()->SetRangeUser(10e-17,1);
  hinvzerobias->GetXaxis()->SetTitle("p_{T} (GeV)");
  hinvzerobias->GetYaxis()->SetTitle("#frac{1}{N_{evt}}E#frac{d^{3}N}{dpd#etad#phi}");
  set1DStyle(hinvzerobias,0);
  set1DStyle(hinvjet120,1);
  set1DStyle(hpub5020,2);
   
  hinvzerobias->Draw("p");
  hinvjet120->Draw("psame");
  hpub5020->Draw("psame");

  TLegend *legend = new TLegend(0.65,0.7,0.85,0.86);
  legend->AddEntry(hinvzerobias,"Zerobias","p");
  legend->AddEntry(hinvjet120,"Jet120 trigger","p");
  legend->AddEntry(hpub5020,"pp 5020 pub","p");
  legend->SetBorderSize(0);
  legend->Draw();
  



}
