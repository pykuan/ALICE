#include <iostream>
#include <cmath>
#include <vector>
#include "TF1.h"
#include "TH1F.h"
#include "TFile.h"
#include "TRandom3.h"
#include "TString.h"
#include "TGraph.h"
#include "TGraphErrors.h"

using namespace std;

struct Barlow {double mean; double sigma_barlow; double sigma_stat; double sigma_sys;};
Barlow calculateBarlow(const int nEntries, bool recreate){

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    const int nBins = 100;

    double N = 1.0/(1.0 - 2.0 / exp(1));
    double mu_true = (2.0 - 5.0 / exp(1)) / (1.0 - 2.0 / exp(1));
    // cout<<mu_true<<endl;

    TFile* file = TFile::Open("file_task.root", recreate ? "RECREATE" : "UPDATE");

    TF1* f1 = new TF1("f1", "[0]*x*exp(-x)", 0, 1);
    f1->SetParameters(N);
    TF1* f2 = new TF1("f2", "[0]*x*exp(-x)", 1, 2);
    f2->SetParameters(N);

    int run = 0;    
    TH1F* h_default = new TH1F(Form("h_default_%d", nEntries), "", nBins, 0, 2);
    TH1F* h_trial = new TH1F(Form("h_trial_%d", nEntries), "", nBins, 0, 1);
    double x = 0;

    for (int i=0; i<nEntries; i++){
        run = i % 10;
        if (run == 9){
            x = f2->GetRandom();
            h_default->Fill(x);
        }
        else {
            x = f1->GetRandom();
            h_default->Fill(x);
            h_trial->Fill(x);
        }

    }

    double mu_default = h_default->GetMean();
    double err_default = h_default->GetMeanError();
    double mu_trial = h_trial->GetMean();
    double err_trial = h_trial->GetMeanError();
    double Diff = fabs(mu_trial - mu_default);
    double Err = sqrt(fabs(err_trial*err_trial - err_default*err_default));

    Barlow out;
    out.mean = mu_default;
    out.sigma_barlow = Diff/Err;
    out.sigma_stat = err_default;
    out.sigma_sys = Diff/mu_default;

    h_default->Write(h_default->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    h_trial->Write(h_trial->GetName(), TObject::kSingleKey + TObject::kOverwrite);

    file->Close();
    delete file; 
    file = nullptr; 

    return out;
};

int task(){

    vector<double> nEvents, sbarlows, means, errors;
    for(int n=10; n<100; n+=10){
        bool recreate = (n==10) ? true : false;
        Barlow b = calculateBarlow(n, recreate);
        double mean = b.mean;
        double sigma_barlow = b.sigma_barlow;
        double sigma_stat = b.sigma_stat;
        double sigma_sys = b.sigma_sys;
        // printf("N=%d, sigma_barlow=%f, sigma_stat=%f, sigma_sys=%f \n", n, b.sigma_barlow, b.sigma_stat, b.sigma_sys);

        nEvents.push_back(n);
        sbarlows.push_back(sigma_barlow);

        means.push_back(mean);
        errors.push_back(sigma_stat + sigma_sys);
    }

    TGraph* g1 = new TGraph(nEvents.size(), nEvents.data(), sbarlows.data());
    TGraphErrors* g2 = new TGraphErrors(means.size(), nEvents.data(), means.data(), 0, errors.data());

    TCanvas* c = new TCanvas("c", "", 600, 900);
    c->Divide(1,2);
    c->cd(1);
    g1->SetTitle("Barlow test");
    g1->GetXaxis()->SetTitle("N_{events}");
    g1->GetYaxis()->SetTitle("#sigma_{Barlow}");
    g1->Draw();
    c->cd(2);
    g2->SetTitle("Reporting <x>");
    g2->GetXaxis()->SetTitle("N_{events}");
    g2->GetYaxis()->SetTitle("x #pm #sigma_{stat} #pm #sigma_{sys}");
    g2->Draw();
    c->Print("Barlow_test.png");
    
    delete c;
    delete g1;
    delete g2;

    return 0;
}