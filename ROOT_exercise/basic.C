int basic(){
    gStyle->SetOptStat(0);
    TFile *f = new TFile("basic.root", "RECREATE");
    TH1F *h1 = new TH1F("h1", "", 100, -4, 4);
    h1->Write(h1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    f->Close();
}