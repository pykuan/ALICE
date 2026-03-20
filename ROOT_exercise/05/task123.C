#include <iostream>
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom3.h"
#include "TComplex.h" 
#include <cmath>

using namespace std;

int task123(){

    TRandom3 rng(0); 
    const double pi = TMath::Pi();
    const int M = 500;
    const double v2 = 0.05;
    const int nEvents = 10000;

    TFile *file123 = new TFile("file123.root", "RECREATE");
    TF1 *f1 = new TF1("f1", "(1+2*[0]*cos(2*(x-[1])))/(2*pi)", 0, 2*pi);
    TF1 *f2 = new TF1("f2", "x/([1]^2) *exp(-([0]^2+x^2)/(2*([1]^2))) *TMath::BesselI0(x*[0]/([1]^2))", 0, 2*pi);
    TH1F *h = new TH1F("h", "h", 100, 0, 2*pi);
    TH1F *hn = (TH1F*) h->Clone("hn");

    for(int i=0; i<nEvents; i++){
        double psi = rng.Uniform(0, 2*pi);
        f1->SetParameters(v2, psi);
        TComplex Q; 
        for(int j=0; j<M; j++){
            double phi = f1->GetRandom();
            TComplex Q_(1.0, 2*phi, kTRUE);
            Q += Q_;
        }
        
        double q = Q.Rho()/sqrt(M);
        h->Fill(q);

    }
    
    for(int i=1; i<=h->GetNbinsX(); i++){
        hn->SetBinContent(i, h->GetBinContent(i)/(h->Integral()*h->GetBinWidth(i)));
    }

    double mu = v2*sqrt(M);
    double sigma = sqrt(0.5);
    f2->SetParameters(mu, sigma);
    f2->SetParNames("μ", "σ"); //μ = √M·v₂ 
    hn->Fit(f2);

    TCanvas *c = new TCanvas("c", "c", 900, 600);
    hn->Draw();
    c->Print("task123.png");
    c->Close();

    cout<<"original: μ = "<<mu<<", σ = "<<sigma<<endl;
    cout<<"fit: μ = "<<f2->GetParameters()[0]<<" ± "<<f2->GetParErrors()[0]
        <<", σ = "<<f2->GetParameters()[1]<<" ± "<<f2->GetParErrors()[1]<<endl;
    cout<<"χ2/ndf = "<<f2->GetChisquare()<<" / "<<f2->GetNDF()<<endl;

    f1->Write(f1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    f2->Write(f2->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    h->Write(h->GetName(), TObject::kSingleKey + TObject::kOverwrite);
    hn->Write(hn->GetName(), TObject::kSingleKey + TObject::kOverwrite);

    file123->Close();
    delete file123; 
    file123 = nullptr; 

    return 0;

}