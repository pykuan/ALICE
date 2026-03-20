#include <iostream>
#include <fstream>
#include <iomanip>
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TRandom3.h"
#include "TStyle.h"

using namespace std;

int someMacro(){
    gStyle->SetOptStat(0);

    ofstream out("dataPoints.log");

    TFile *file = new TFile("test.root", "RECREATE");
    TF1 *f = new TF1("f", "[0]*x*exp(-1*[1]*x)", 0, 1);
    f->SetParameters(0.1, 0.4);
    TH1F *h = new TH1F("h", "h", 1000, 0, 1);
    h->FillRandom("f", 50000);

    TProfile *prof = new TProfile("prof", "", 10, 0, 1);
    for(int i=0; i<50000; i++){
        prof->Fill(gRandom->Uniform(1.), f->GetRandom());
    }

    out << fixed;
    float x, y, yerr;
    for(int i=1; i<11; i++){
        x = prof->GetBinCenter(i);
        y = prof->GetBinContent(i);
        yerr = prof->GetBinError(i);
        out << setprecision(2) << x << " " << setprecision(8) << y << " " << yerr << endl;
        cout << "y mean:" << y << endl;
    }

    TCanvas *c = new TCanvas("c", "c", 900, 600);
    
    prof->GetXaxis()->SetTitle("x");
    prof->GetYaxis()->SetTitle("y");
    prof->SetMarkerColor(2);
    prof->SetLineColor(2);
    prof->SetMarkerStyle(21);
    prof->Draw("e1");
    c->Print("prof.png");

    out.close();
    h->Write(h->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    prof->Write(prof->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    file->Close();

    delete f;
    delete c;

    return 0;
}