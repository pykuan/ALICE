#include <iostream>
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom3.h" 

using namespace std;

int task45(){
    const int nEntries = 10000000;
    const float h2binedge[] = {0, 20, 30, 40, 45, 50, 55, 60, 70, 80, 100};

    TFile *file45 = new TFile("file45.root", "RECREATE");

    TF1 *f = new TF1("f", "3*x/5000 *(1-x/100)", 0, 100);
    TH1F *h1 = new TH1F("h1", "h1", 100, 0, 100);
    TH1F *h2 = new TH1F("h2", "h2", 10, h2binedge);

    h1->FillRandom("f", nEntries);
    h2->FillRandom("f", nEntries);

    TH1F *hn1 = (TH1F*) h1->Clone("hn1");
    TH1F *hn2 = (TH1F*) h2->Clone("hn2");

    for(int i=1; i<=h1->GetNbinsX(); i++){
        hn1->SetBinContent(i, h1->GetBinContent(i)/(h1->Integral()*h1->GetBinWidth(i)));
    }
    for(int i=1; i<=h2->GetNbinsX(); i++){
        hn2->SetBinContent(i, h2->GetBinContent(i)/(h2->Integral()*h2->GetBinWidth(i)));
    }
    cout<<hn1->Integral("width")<<endl;

    TCanvas *c1 = new TCanvas("c1", "c1", 900, 600);
    f->Draw();
    hn1->Draw("same");
    c1->Print("task45_hn1.png");
    c1->Close();

    TCanvas *c2 = new TCanvas("c2", "c2", 900, 600);
    f->Draw();
    hn2->Draw("same");
    c2->Print("task45_hn2.png");
    c2->Close();

    f->Write(f->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    h1->Write(h1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    h2->Write(h2->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    hn1->Write(hn1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    hn2->Write(hn2->GetName(), TObject::kSingleKey + TObject::kOverwrite);        
    

    file45->Close();
    delete file45; 
    file45 = nullptr; 

    return 0;
}