#include <iostream>
#include "TF1.h"
#include "TH1F.h"
#include "TMath.h"
#include "TRandom3.h"
#include <cmath>
#include <vector>

using namespace std;

struct Err {double err_bootstrap; double err_histogram;};
Err calculateErr(const int nSubsample, bool recreate){
    
    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    const int nBins = 100;
    const double lLimit = 0, uLimit = 0.1;
    TFile* file = TFile::Open("file_task.root", recreate ? "RECREATE" : "UPDATE");
    TH1F* h = new TH1F(Form("h%d", nSubsample), Form("v^{2} with N=%d", nSubsample), nBins, lLimit, uLimit);

    const double b = sqrt(0.0025/2.01);
    const double a = b/10;
    TF1* f = new TF1("f", "(x/([1]*[1])) * exp(-(x*x + [0]*[0])/(2.0*[1]*[1])) * TMath::BesselI0(x*[0]/([1]*[1]))", lLimit, uLimit);
    f->SetParameters(a, b);

    const int nSample = 1e7;
    // const int nSubsample = 10;

    vector<double> mui_array(nSubsample);
    double v = 0;
    double mui = 0;
    double mu = 0;
    
    for (int i=0; i<nSubsample; i++){
        mui = 0;
        for (int j=0; j<nSample/nSubsample; j++){
            v = f->GetRandom();
            h->Fill(v*v);
            mui += v*v/(nSample/nSubsample);
        }
        mui_array[i] = mui;
        mu += mui/nSubsample;
    }

    double var = 0;
    for (int i=0; i<nSubsample; i++){
        var += (mui_array[i]-mu)*(mui_array[i]-mu)/(nSubsample-1);
    }

    Err out;
    out.err_bootstrap = sqrt(var/nSubsample);
    out.err_histogram = h->GetMeanError();
    

    h->Write(h->GetName(), TObject::kSingleKey + TObject::kOverwrite);

    file->Close();
    delete file; 
    file = nullptr; 
    return out;
    
};

int task(){

    for (int n : {10, 20, 100}) {
        bool recreate = (n==10) ? true : false;
        Err r = calculateErr(n, recreate);
        printf("N=%d, err_bootstrap=%f, err_histogram=%f, difference=%f%% \n", n, r.err_bootstrap, r.err_histogram, abs(r.err_bootstrap-r.err_histogram)/r.err_histogram*100);
    }

    return 0;

}