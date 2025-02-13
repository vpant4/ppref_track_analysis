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
h->GetYaxis()->SetTitleOffset(1.3);
h->GetXaxis()->SetTitleOffset(1.3);
h->GetYaxis()->CenterTitle();
h->GetXaxis()->CenterTitle();
}


void htrkptdist()
{
  TFile *f1=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefzerobiasout.root","READ");
  TFile *f2=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefjet120out.root ","READ");
  

  
  TH1D *htrkptzerobias =  (TH1D*) f1->Get("htrkpt");
  TH1D *htrkptjet120   =  (TH1D*) f2->Get("htrkpt");

  TH1D *hjtptzerobias  =  (TH1D*) f1->Get("hjtpt");
  TH1D *hjtptjet120    =   (TH1D*) f2->Get("hjtpt");

  hjtptzerobias->GetYaxis()->SetRangeUser(10e-10,1.);
  hjtptjet120->GetYaxis()->SetRangeUser(10e-10,1.);

  hjtptjet120->GetXaxis()->SetRangeUser(15.,1000.);
  hjtptzerobias->GetXaxis()->SetRangeUser(15.,1000.);
  
  cout<<"Zero bias jet events > 140 GeV "<<hjtptzerobias->Integral()<<endl;
  cout<<"Jet 120 jet events > 140 GeV "<<hjtptjet120->Integral()<<endl;
  

  rescaleHist(htrkptzerobias);
  rescaleHist(htrkptjet120);
  rescaleHist(hjtptzerobias);
  rescaleHist(hjtptjet120);
  TLatex t;
  TCanvas *c1 = new TCanvas("c1","c1",800,800);
  c1->SetLogy(1);
  c1->SetLogx(1);
  //hinvzerobias->GetYaxis()->SetRangeUser(10e-17,1);
  hjtptzerobias->GetXaxis()->SetTitle("Jet p_{T} (GeV)");
  hjtptzerobias->GetYaxis()->SetTitle("#frac{1}{N_{evt}}#frac{dN}{dp_{T}}");
  set1DStyle(hjtptzerobias,0);
  set1DStyle(hjtptjet120,1);
  hjtptzerobias->Draw("p");
  hjtptjet120->Draw("psame");
  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Preliminary}");
  t.DrawLatexNDC(0.6, 0.93, "pp 5.36 TeV");

  TLegend *legend = new TLegend(0.65,0.7,0.85,0.86);
  legend->AddEntry(hjtptzerobias,"Zerobias","p");
  legend->AddEntry(hjtptjet120,"Jet120 trigger","p");
  legend->SetBorderSize(0);
  legend->Draw();

  TCanvas *c2 = new TCanvas("c2","c2",800,800);
  c2->SetLogy(1);
  c2->SetLogx(1);                                                                        
  htrkptzerobias->GetXaxis()->SetTitle("Track p_{T} (GeV)");
  htrkptzerobias->GetYaxis()->SetTitle("#frac{1}{N_{evt}}#frac{dN}{dp_{T}}");
  set1DStyle(htrkptzerobias,0);
  set1DStyle(htrkptjet120,1);

  htrkptzerobias->Draw("p");
  htrkptjet120->Draw("psame");
  
  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Preliminary}");
  t.DrawLatexNDC(0.6, 0.93, "pp 5.36 TeV");

  TLegend *legend1 = new TLegend(0.65,0.7,0.85,0.86);
  legend1->AddEntry(htrkptzerobias,"Zerobias","p");
  legend1->AddEntry(htrkptjet120,"Jet120 trigger","p");
  legend1->SetBorderSize(0);
  legend1->Draw();
  

  c1->SaveAs("/Users/vipulpant/ppref2024analysis/ppref_track_analysis/Plots/ppref24jetpt.pdf");
  c2->SaveAs("/Users/vipulpant/ppref2024analysis/ppref_track_analysis/Plots/ppref24trackpt.pdf");


}
