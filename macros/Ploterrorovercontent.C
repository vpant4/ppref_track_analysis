void Ploterrorovercontent()
{
    // --- Hardcoded file and histogram name ---
    const char* filename = "/Users/vipulpant/cernbox/ppref2024/pprefzerobiasout.root";   // <-- change this to your file
    const char* histname = "htrkinvyield_corr";          // <-- change this to your histogram

    // Open the ROOT file
    TFile *f = TFile::Open(filename, "READ");
    if (!f || f->IsZombie()) {
        printf("Error opening file %s\n", filename);
        return;
    }

    // Get the histogram
    TH1* h = dynamic_cast<TH1*>(f->Get(histname));
    if (!h) {
        printf("Histogram %s not found in file %s\n", histname, filename);
        f->Close();
        return;
    }

    // Create a new histogram for Error/bin_content
    TH1* h_ratio = (TH1*)h->Clone(Form("%s_ErrorOverContent", histname));
    h_ratio->Reset(); // Clear contents

    // Loop over bins
    for (int i = 1; i <= h->GetNbinsX(); ++i) {
        double content = h->GetBinContent(i);
        double error = h->GetBinError(i);
        
        if (content != 0) {
            h_ratio->SetBinContent(i, error / content);
            h_ratio->SetBinError(i, 0); // No error on ratio unless you want to propagate
            cout<<error / content<<endl;
        } else {
            h_ratio->SetBinContent(i, 0);
            h_ratio->SetBinError(i, 0);
        }
    }

    // Draw the new histogram
    //    TCanvas *c1 = new TCanvas("c1", "Error/Content Plot", 800, 600);
    //    h_ratio->SetTitle("Bin Error / Bin Content;Bin;Error/Content");
    //h_ratio->SetMinimum(0);
    h_ratio->Draw("");

    //c1->SaveAs("ErrorOverContent.png"); // Optional: save plot

    //f->Close();
}
