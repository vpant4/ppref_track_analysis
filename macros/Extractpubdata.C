void Extractpubdata()
{


  TFile *f = TFile::Open("pp5020pub.root","READ");
  TDirectoryFile *g= (TDirectoryFile*) f->Get("Table 7");
  TH1D *hist = (TH1D*) g->Get("Hist1D_y1");
  TH1D *hist_stat = (TH1D*) g->Get("Hist1D_y1_e1");

  TH1D *hout = (TH1D*) hist->Clone("hout");
  for (int i=1;i<=hout->GetXaxis()->GetNbins();i++)
    {hout->SetBinError(i,hist_stat->GetBinContent(i));
	}
  TFile *fout = new TFile("pp5020datapoints.root","RECREATE");
  hout->Write();
  fout->Close();



}
