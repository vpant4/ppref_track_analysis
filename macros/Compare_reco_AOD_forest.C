#include "TFile.h"
#include "TH1.h"
#include "TH3.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TStyle.h"
#include "THnSparse.h"


void Rescale(TH1* h)
{
for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++) {
        Double_t val = h->GetBinContent( iBin );
        Double_t valErr = h->GetBinError( iBin );
        Double_t binWidth = h->GetBinWidth( iBin );
        h->SetBinContent( iBin, val / binWidth );
        h->SetBinError( iBin, valErr / binWidth );
    } // for (Int_t iBin=1; iBin<=h->GetNbinsX(); iBin++)
//h->Scale( 1. / h->Integral() );                                                                          
}



//________________
void setPadStyle() {
    gPad->SetTopMargin(0.1);
    gPad->SetBottomMargin(0.15);
    gPad->SetRightMargin(0.1);
    gPad->SetLeftMargin(0.15);
}

//________________
void set1DStyle(TH1 *h, Int_t type = 0, Bool_t doRenorm = kFALSE) {
    Int_t markerStyle = 20; // Full circle
    Double_t markerSize = 1.3;
    Int_t lineWidth = 2;
    Int_t color = 2;
    if (type == 0) {
        color = 2;        // red
        markerStyle = 20; // filled circle
    }
    else if (type == 1) {
        color = 4;        // blue
        markerStyle = 24; // open circle
    }
    else if (type == 2) {
        color = 1;        // black
        markerStyle = 20; // filled triangle
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

    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetLabelSize(0.06);
    h->GetXaxis()->SetNdivisions(208);
    h->GetYaxis()->SetNdivisions(208);    
    h->GetYaxis()->SetTitleOffset(1.1);

    if ( doRenorm ) {
        h->Scale( 1./h->Integral() );
    }
}


//________________
void make1DRatio(TH1 *hRat, TH1 *hDen, const Char_t *ratioName = "Ratio to default", Int_t style = 0, Bool_t isBinomial = kFALSE) {

    if ( !hDen ) {
        std::cout << "Denominator does not exist" << std::endl;
    }

    if ( isBinomial ) {
        hRat->Divide( hRat, hDen, 1., 1., "b" );
    }
    else {
        hRat->Divide( hRat, hDen, 1., 1. );
    }
    hRat->GetYaxis()->SetTitle( ratioName );
    hRat->GetYaxis()->SetRangeUser(0.9,1.2 );
    //hRat->GetXaxis()->SetRangeUser(-2.915, 3.);
    //    set1DStyle(hRat, style);
}

//________________
void plotComparison(TCanvas *c, TH1 *h1, TH1 *h2,TH1 *h3, TH1 *hRatio1, TH1 *hRatio2,
                    const char *h1Type, const char *h2Type,const char *h3Type, float xmin, float xmax,const char *xtitle )  {

    TLatex t;
    t.SetTextFont(42);
    t.SetTextSize(0.05);

    set1DStyle(h1, 0);
    set1DStyle(h2, 1);
    set1DStyle(h3,2);
    
    set1DStyle(hRatio1, 0);
    set1DStyle(hRatio2, 2);
    
    
    c->cd(1);
    gPad->SetLogy();
    gPad->SetLogx();
    setPadStyle();
    h1->Draw();
    h2->Draw("same");
    //    h3->Draw("same");
    //h1->GetYaxis()->SetRangeUser(0.0, 0.12);
    h1->GetXaxis()->SetRangeUser(xmin, xmax);
    h1->GetYaxis()->SetTitle("NEvents");
    h1->GetXaxis()->SetTitle(xtitle);
    
   

    TLegend *leg = new TLegend(0.65, 0.65, 0.9, 0.85);
    leg->SetTextSize(0.06);
    leg->AddEntry(h1, Form("%s", h1Type), "p");
    leg->AddEntry(h2, Form("%s", h2Type), "p");
    leg->AddEntry(h3, Form("%s", h3Type), "p");
    leg->SetBorderSize(0);
    leg->Draw();

    c->cd(2);
    //    gPad->SetLogy();
    gPad->SetLogx();
    setPadStyle();
    hRatio1->GetXaxis()->SetRangeUser(xmin,xmax);
    hRatio1->GetXaxis()->SetTitle(xtitle);
    //hRatio1->Draw();
    hRatio2->Draw("same");
    
    TLine *l = new TLine(0,1,500,1);
    l->SetLineStyle(2);
    l->SetLineColor(kMagenta);
    l->Draw();

}

//________________
void Compare_reco_AOD_forest() {

    // Base style
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetPalette(kBird);


    
    TFile *f = TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefMCpthat15_AOD.root","READ");

    TDirectoryFile *g1 = (TDirectoryFile*) f->Get("defaultCPDC");
    TDirectoryFile *g2 = (TDirectoryFile*) g1->Get("QAplots");

    TFile *f1 = TFile::Open("/Users/vipulpant/cernbox/ppref2024/pprefMCpthat15_forest.root","READ");

    
    TH1F *htrkptgenaod       = (TH1F*) g2->Get("hpt_inc");
    TH1F *htrkptgenforest    = (TH1F*) f1->Get("htrkpt");
  
    //    htrkptgenaod->Draw();
    //htrkptgenforest->Draw("same");
    //    return;
    
        
       set1DStyle(htrkptgenaod, 0);
    //return;
    set1DStyle(htrkptgenforest, 0);
    // return;

    Rescale(htrkptgenaod);
    Rescale(htrkptgenforest);

    cout<<htrkptgenaod->Integral()<<"  "<<htrkptgenforest->Integral()<<endl;

    //    return; 
    TH1 *hptRatioforaod = dynamic_cast<TH1*>( htrkptgenforest->Clone("htrkptrecocorr") );
    //TH1F *hptRatioreco_gen = dynamic_cast<TH1F*>( htrkptreco_corr->Clone("htrkptRatio") );
    
    //    return;
    make1DRatio(hptRatioforaod, htrkptgenaod, "forest/AOD", 2);
    //return;
    //make1DRatio(hptRatioreco_gen,htrkptgen, "reco/gen",2);
    //return;

    TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
    
    c1->Divide(1, 2,0,0);
    
    plotComparison(c1, htrkptgenforest,htrkptgenaod,htrkptgenaod,hptRatioforaod,hptRatioforaod, "Forest reco","AOD reco","AOD reco",0.1,500.,"Track pT");

    c1->SaveAs("../Plots/ComparisonrecoAODforest.pdf");
    
    

    
    

    
    
   
}

