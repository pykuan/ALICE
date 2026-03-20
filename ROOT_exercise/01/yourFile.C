{
    gStyle->SetOptStat(0);

    TFile *file = new TFile("test.root", "RECREATE");
    TF1 *f = new TF1("f", "[0]*x*exp(-1*[1]*x)", 0, 1);
    f->SetParameters(0.1, 0.4);
    TH1F *h = new TH1F("h", "h", 1000, 0, 1);
    h->FillRandom("f", 50000);
    float h_mean = h->GetMean();
    cout<<h_mean<<endl;

    // TH1F* rebinned = (TH1F*)h->Rebin(100, "rebinned");
    TH1F* rebinned = (TH1F*) h->Clone("rebinned");
    rebinned->Rebin(100);

    TCanvas *c = new TCanvas("c", "c", 900, 600);
    c->Divide(2,1);

    c->cd(1);
    h->SetTitle("histogram");
    h->Draw();

    c->cd(2);
    rebinned->SetTitle("rebinned histogram");
    rebinned->SetMarkerStyle(20);
    rebinned->SetMarkerColor(2);
    rebinned->Draw("p");    

    c->Print("yourFile.png");

    // file->Write();
    h->Write(h->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    rebinned->Write(rebinned->GetName(), TObject::kSingleKey + TObject::kOverwrite);

    file->Close();

    delete f;
    delete c;

}