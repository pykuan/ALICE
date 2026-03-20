#include <iostream>
#include "TH1F.h"
#include "TRandom3.h"
#include <cmath>
#include "TF2.h"
#include "TH2F.h"
#include "TString.h"

using namespace std;

TH2F* Ratio(const TF2* fxy, TH1F* hratio_distribution){

    const int nEntries = 1000000;
    const int nBins = 100;

    TH2F* h  = new TH2F(Form("h_%s",  fxy->GetName()), "", nBins, 3, 4, nBins, 2, 6);
    TH2F* hr = new TH2F(Form("hr_%s", fxy->GetName()), "", nBins, 3, 4, nBins, 2, 6);

    h->FillRandom(fxy->GetName(), nEntries);
    h->Scale(1. / h->Integral("width"));

    TH1F *marginalX = (TH1F*)h->ProjectionX();
    TH1F *marginalY = (TH1F*)h->ProjectionY(); //no "width"

    for(int x=1; x<=nBins; x++){
        double dx = h->GetXaxis()->GetBinWidth(x);
        for(int y=1; y<=nBins; y++){
            double dy = h->GetYaxis()->GetBinWidth(y);

            double fxy_value = h->GetBinContent(x, y);
            double fxfy_value = marginalX->GetBinContent(x)*marginalY->GetBinContent(y)*dx*dy;            
            double ratio = fxy_value/fxfy_value;
            hr->SetBinContent(x, y, ratio);
            // cout<<fxy->GetName()<<" "<<ratio<<endl;
            hratio_distribution->Fill(ratio);
        }
    }
    return hr;


}

int task1(){
    
    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(0);

    TFile *f = new TFile("file_task1.root", "RECREATE");

    TF2* fxy1 = new TF2("fxy1", "exp(-(x-4)^2)*log(y)", 3, 4, 2, 6);
    TF2* fxy2 = new TF2("fxy2", "exp(-(x-4)^2)+log(y)", 3, 4, 2, 6);
    TH1F* hratio_distribution_1 = new TH1F("hratio_distribution_1", "", 100, 0.5, 1.5);
    TH1F* hratio_distribution_2 = new TH1F("hratio_distribution_2", "", 100, 0.5, 1.5);
    TH2F* hratio_1 = Ratio(fxy1, hratio_distribution_1);
    TH2F* hratio_2 = Ratio(fxy2, hratio_distribution_2);

    hratio_1->Write(hratio_1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    hratio_2->Write(hratio_2->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    hratio_distribution_1->Write(hratio_distribution_1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    hratio_distribution_2->Write(hratio_distribution_2->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    f->Close();
    delete f; 
    f = nullptr; 

    return 0;
}
