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
    Double_t markerSize = 2.0;
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


void CompareparticlespeciesdataMC()
{
  TFile *f1=TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefMCpthat15_forest.root","READ");
  TFile *f2=TFile::Open("/Users/vipulpant/Downloads/HEP_pions_5020_pub_alice.root");
  TFile *f3=TFile::Open("/Users/vipulpant/Downloads/HEP_kaons_5020_pub_alice.root");
  TFile *f4=TFile::Open("/Users/vipulpant/Downloads/HEP_ppbar_5020_pub_alice.root");


  TH1F *hEvents = (TH1F*) f1->Get("hEvents");
  
  TDirectoryFile *d1 = (TDirectoryFile*) f2->Get("Table 2");
  TDirectoryFile *d2 = (TDirectoryFile*) f3->Get("Table 4");
  TDirectoryFile *d3 = (TDirectoryFile*) f4->Get("Table 6");
  
 
  TH2F *hincgenmatchedpteta    =  (TH2F*) f1->Get("htotalgenpteta");
  TH2F *hpmatchedpteta         =  (TH2F*) f1->Get("hpapgenpteta");
  TH2F *hapmatchedpteta        =  (TH2F*) f1->Get("hapgenpteta");
  TH2F *hpionmatchedpteta      =  (TH2F*) f1->Get("hpiongenpteta");
  TH2F *hKmatchedpteta	       =  (TH2F*) f1->Get("hKgenpteta");
  
  TH1F *hincmatchedpt = (TH1F*) hincgenmatchedpteta->ProjectionX("hincgenmatchedpt");
  TH1F *hpmatchedpt = (TH1F*) hpmatchedpteta->ProjectionX("hpgenmatchedpt");
  TH1F *hapmatchedpt = (TH1F*) hapmatchedpteta->ProjectionX("hapgenmatchedpt");
  TH1F *hpionmatchedpt = (TH1F*) hpionmatchedpteta->ProjectionX("hpiongenmatchedpt");
  TH1F *hKmatchedpt = (TH1F*) hKmatchedpteta->ProjectionX("hKgenmatchedpt");


  rescaleHist(hpmatchedpt);
  rescaleHist(hapmatchedpt);
  rescaleHist(hpionmatchedpt);
  rescaleHist(hKmatchedpt);
  rescaleHist(hincmatchedpt);

  hincmatchedpt->Scale(1./hEvents->Integral());
  hpmatchedpt->Scale(1./hEvents->Integral());
  hapmatchedpt->Scale(1./hEvents->Integral());
  hpionmatchedpt->Scale(1./hEvents->Integral());
  hKmatchedpt->Scale(1./hEvents->Integral());


  hpmatchedpt->Divide(hincmatchedpt);
  hpionmatchedpt->Divide(hincmatchedpt);
  hKmatchedpt->Divide(hincmatchedpt);

  
  TH1F *hpionspub1 = (TH1F*) d1->Get("Hist1D_y1");
  TH1F *hkaonspub1 = (TH1F*) d2->Get("Hist1D_y1");
  TH1F *hppbarpub1 = (TH1F*) d3->Get("Hist1D_y1");

  double bin_edgespub[] = {
    0.10, 0.12, 0.14, 0.16, 0.18, 0.20, 0.25, 0.30, 0.35, 0.40,
    0.45, 0.50, 0.55, 0.60, 0.65, 0.70, 0.75, 0.80, 0.85, 0.90,
    0.95, 1.00, 1.05, 1.10, 1.15, 1.20, 1.25, 1.30, 1.40, 1.60,
    2.00, 2.40, 2.80, 3.20, 3.60, 4.00, 4.50, 5.00, 6.00, 7.00,
    8.00, 9.00, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 18.0, 20.0
  };
  Int_t bin_countpub = sizeof(bin_edgespub) / sizeof(bin_edgespub[0])-1;

  cout<<" Bin count "<<bin_countpub<<endl;
  TH1F *hpionspub = new TH1F("hpionspub","hpionspub",bin_countpub,bin_edgespub);
  TH1F *hkaonspub = new	TH1F("hkaonspub","hkaonspub",bin_countpub,bin_edgespub);
  TH1F *hppbarpub = new	TH1F("hppbarpub","hppbarpub",bin_countpub,bin_edgespub);

  for (int i=1; i <=hpionspub->GetNbinsX(); i++)
    {
      Float_t bincenter=hpionspub->GetBinCenter(i);
      Int_t   bin_pionspub = hpionspub1->FindBin(bincenter);
      Int_t   bin_kaonspub = hkaonspub1->FindBin(bincenter);
      Int_t   bin_ppbarpub = hppbarpub1->FindBin(bincenter);
      
      hpionspub->SetBinContent(i,hpionspub1->GetBinContent(bin_pionspub));
      hkaonspub->SetBinContent(i,hkaonspub1->GetBinContent(bin_kaonspub));
      hppbarpub->SetBinContent(i,hppbarpub1->GetBinContent(bin_ppbarpub));
      
    }



  
  TH1F* hinclpub =  (TH1F*)  hpionspub->Clone("hinclpub");


  hinclpub->Add(hkaonspub);
  hinclpub->Add(hppbarpub);

  hpionspub->Divide(hinclpub);
  hkaonspub->Divide(hinclpub);
  hppbarpub->Divide(hinclpub);
  
  TLatex t;
  TCanvas *c2 = new TCanvas("c2","c2",800,800);
  c2->SetLogx(1);
  c2->SetLogy(1);  
                                                                          
  hpmatchedpt->GetXaxis()->SetTitle("Track p_{T} (GeV)");
  hpmatchedpt->GetYaxis()->SetTitle("Particle species fraction");
  hpmatchedpt->GetXaxis()->SetRangeUser(0.3,20);
  hpmatchedpt->GetYaxis()->SetRangeUser(0.01,10);
  set1DStyle(hpmatchedpt,0);
  set1DStyle(hapmatchedpt,2);
  set1DStyle(hpionmatchedpt,4);
  set1DStyle(hKmatchedpt,6);

   
  hpmatchedpt->Draw("p hist");
  hpionmatchedpt->Draw("p hist same");
  hKmatchedpt->Draw("p hist same");

  set1DStyle(hppbarpub,1);
  set1DStyle(hpionspub,5);
  set1DStyle(hkaonspub,7);
  
  hpionspub->Draw("p hist same");
  hkaonspub->Draw("p hist same");
  hppbarpub->Draw("p hist same");

  //  hpmatchedpt->SetTitle("Pythiaprotonfraction");
  //hpionmatchedpt->SetTitle("Pythiapionfraction");
  //hKmatchedpt->SetTitle("Pythiakaonfraction");
  //hpionspub->SetTitle("Publishedpionfraction");
  //hkaonspub->SetTitle("Publishedkaonfraction");
  //hppbarpub->SetTitle("Publishedprotonfraction");

  hpmatchedpt->SetName("Pythiaprotonfraction");
  hpionmatchedpt->SetName("Pythiapionfraction");
  hKmatchedpt->SetName("Pythiakaonfraction");
  hpionspub->SetName("Publishedpionfraction");
  hkaonspub->SetName("Publishedkaonfraction");
  hppbarpub->SetName("Publishedprotonfraction");


  
  TFile *fout= new TFile("Particlefractionfile.root","RECREATE");
  hpmatchedpt->Write();    
  hpionmatchedpt->Write();
  hKmatchedpt->Write();    
  hpionspub->Write();      
  hkaonspub->Write();      
  hppbarpub->Write();      
  
  TLegend *legend1 = new TLegend(0.55,0.63,0.87,0.89);
  legend1->AddEntry(hpmatchedpt,"p+#bar{p}","p");
  legend1->AddEntry(hpionmatchedpt,"#pi^{+} & #pi^{-}","p");
  legend1->AddEntry(hKmatchedpt,"K^{+} & K^{-}","p");

  legend1->AddEntry(hpionspub,"ALICE pub. pions","p");
  legend1->AddEntry(hkaonspub,"ALICE pub. kaons","p");
  legend1->AddEntry(hppbarpub,"ALICE pub. protons","p");

  legend1->SetBorderSize(0);
  legend1->SetTextSize(0.04);
  legend1->Draw();
  t.DrawLatexNDC(0.15, 0.93, "#bf{CMS} #it{Simulation}");
  t.DrawLatexNDC(0.6, 0.93, "pp 5.36 TeV");

  c2->SaveAs("/Users/vipulpant/ppref2024analysis/ppref_track_analysis/Plots/ComparisonparticlesdataMC.pdf");


}
