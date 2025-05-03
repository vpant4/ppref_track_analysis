void rescaleHist(TH1* h) {
  for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++) {
    Double_t val = h->GetBinContent( iBin );
    Double_t valErr = h->GetBinError( iBin );
    Double_t binWidth = h->GetBinWidth( iBin );
    h->SetBinContent( iBin, val / binWidth );
    h->SetBinError( iBin, valErr / binWidth );
  }                                                                     
  h->Scale( 1./2 );
}

void set1DStyle(TH1 *h, Int_t type = 0) {
    Int_t markerStyle = 20; // Full circle
    Double_t markerSize = 1.4;
    Int_t lineWidth = 2;
    Int_t color = 2;
    if (type == 0) {
        color = 1;        // red
	markerStyle = 20; // filled circle
    }

    else if (type == 1) {
      color = 1;        // blue
      markerStyle = 107; // open circle
    }
    else if (type == 2) {
      color = 2;        // black
      markerStyle = 20; // filled triangle
    }
    else if (type == 3) {
      color = 2;        // red
      markerStyle = 107; // open circle
    }
    else if (type == 4) {
      color = 3;        // blue
      markerStyle = 20; // filled circle
    }
    else if (type == 5) {
      color = 3;        // magenta
      markerStyle = 107; // open star
    }
    else if (type == 6) {
      color = 4;
      markerStyle = 20;
    }
    else if (type == 7) {
      color = 4;
      markerStyle = 107;
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


void CompareparticlesratiodataMC()
{
  TFile *f1=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefMCpthat15_forest.root","READ");

  TFile *f2=TFile::Open("/Users/vipulpant/Downloads/HEP_Kaonsoverpions_5020_pub_alice.root");
  TFile *f3=TFile::Open("/Users/vipulpant/Downloads/HEP_ppbaroverpions_5020_pub_alice.root");


  TH1F *hEvents = (TH1F*) f1->Get("hEvents");
  
  TDirectoryFile *d1 = (TDirectoryFile*) f2->Get("Table 8");
  TDirectoryFile *d2 = (TDirectoryFile*) f3->Get("Table 10");
  
 
  TH2F *hincgenmatchedpteta    =  (TH2F*) f1->Get("htotalgenpteta");
  TH2F *hpmatchedpteta         =  (TH2F*) f1->Get("hpapgenpteta");
  TH2F *hpionmatchedpteta      =  (TH2F*) f1->Get("hpiongenpteta");
  TH2F *hKmatchedpteta	       =  (TH2F*) f1->Get("hKgenpteta");
  
  TH1F *hincmatchedpt = (TH1F*) hincgenmatchedpteta->ProjectionX("hincgenmatchedpt");
  TH1F *hpmatchedpt = (TH1F*) hpmatchedpteta->ProjectionX("hpgenmatchedpt");
  TH1F *hpionmatchedpt = (TH1F*) hpionmatchedpteta->ProjectionX("hpiongenmatchedpt");
  TH1F *hKmatchedpt = (TH1F*) hKmatchedpteta->ProjectionX("hKgenmatchedpt");


  rescaleHist(hpmatchedpt);
  //rescaleHist(hapmatchedpt);
  rescaleHist(hpionmatchedpt);
  rescaleHist(hKmatchedpt);

  hpmatchedpt->Scale(1./hEvents->Integral());
  //  hapmatchedpt->Scale(1./hEvents->Integral());
  hpionmatchedpt->Scale(1./hEvents->Integral());
  hKmatchedpt->Scale(1./hEvents->Integral());

  TH1F *hKoverpionRatio = (TH1F*) hKmatchedpt->Clone("hpapRatio");
  TH1F *hppbaroverpionsRatio= (TH1F*) hpmatchedpt->Clone("hppbaroverpionsRatio");

  hKoverpionRatio->Divide(hpionmatchedpt);
  hppbaroverpionsRatio->Divide(hpionmatchedpt);
  
  TH1F *hkoverppub = (TH1F*) d1->Get("Hist1D_y1");
  TH1F *hppbaroverpionpub = (TH1F*) d2->Get("Hist1D_y1");
  
  TLatex t;
  TCanvas *c2 = new TCanvas("c2","c2",800,800);
  c2->SetLogx(1);
  c2->SetLogy(1);
                                                                          
  hKoverpionRatio->GetXaxis()->SetTitle("Track p_{T} (GeV)");
  hKoverpionRatio->GetYaxis()->SetTitle("Particle spectra");
  hKoverpionRatio->GetXaxis()->SetRangeUser(0.2,20);
  //  hKoverpionRatio->GetYaxis()->SetRangeUser(0.0001,1000);

  set1DStyle(hKoverpionRatio,0);
  set1DStyle(hppbaroverpionsRatio,2);
  
  hKoverpionRatio->Draw("p hist");
  hppbaroverpionsRatio->Draw("p hist same");
  
  set1DStyle(hkoverppub,1);
  set1DStyle(hppbaroverpionpub,3);
  
  hkoverppub->Draw("p hist same");
  hppbaroverpionpub->Draw("p hist same");
  
  TLegend *legend1 = new TLegend(0.1,0.52,0.2,0.86);
  
  
  legend1->AddEntry(hKoverpionRatio,"K^{+} + K^{-} / #pi + #bar{#pi}","p");
  legend1->AddEntry(hkoverppub,"K / #pi published","p");
  legend1->AddEntry(hppbaroverpionsRatio," p + #bar{p} / pi^{+} & #pi^{-}","p");
  legend1->AddEntry(hppbaroverpionpub,"p / #pi published","p");
 

  legend1->SetBorderSize(0);
  legend1->SetTextSize(0.04);
  legend1->Draw();
  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Simulation}");
  t.DrawLatexNDC(0.6, 0.93, "pp 5.36 TeV");

  c2->SaveAs("/Users/vipulpant/ppref2024analysis/ppref_track_analysis/Plots/CompareparticlesratiodataMC.pdf");


}
