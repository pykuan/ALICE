#include <iostream>
#include "TF1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TRandom3.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <cmath>

using namespace std;
int question13(){

    // TStopwatch timer;
    // timer.Start();

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    TF1* f = new TF1("f", "1/(2*TMath::Pi())*(1+2*0.05*cos(2*(x-[0])))", 0, TMath::TwoPi());
    TProfile* p2 = new TProfile("p2", "2-p correlation", 1, 0., 1.); p2->Sumw2();
    TProfile* p4 = new TProfile("p4", "4-p correlation", 1, 0., 1.); p4->Sumw2(); 
    
    const int nEvent = 10000;
    const int nSub = 10;

    TProfile* p2sub[nSub];
    TProfile* p4sub[nSub];
    for (int s=0; s<nSub; ++s){
        p2sub[s] = new TProfile(Form("p2_sub%d",s), "2-p correlation", 1, 0., 1.); p2sub[s]->Sumw2();
        p4sub[s] = new TProfile(Form("p4_sub%d",s), "4-p correlation", 1, 0., 1.); p4sub[s]->Sumw2();
    }

    int M = 0; //multiplicity 
    double Psi = 0.;
    double phi = 0.;
    TComplex Q2(0,0);
    TComplex Q4(0,0);
    double w2 = 0.;
    double w4 = 0.;
    double v2 = 0.;
    double v4 = 0.;


    for(int i=0; i<nSub; i++){

        for(int j=0; j<nEvent/nSub; j++){

            TComplex Q2(0,0);
            TComplex Q4(0,0);
            M = gRandom->Uniform(50,500);
            
            Psi = gRandom->Uniform(0., TMath::TwoPi());
            f->SetParameters(Psi);
            for(int k=0; k<M; k++){
                phi = f->GetRandom();
                Q2 += TComplex::Exp(TComplex(0, 2*phi));
                Q4 += TComplex::Exp(TComplex(0, 2*2*phi));
            }

            w2 = (double)M*(M-1);
            w4 = (double)M*(M-1)*(M-2)*(M-3);

            v2 = 1./w2 *(Q2.Rho2() - M);
            v4 = 1./w4 *(Q2.Rho2()*Q2.Rho2() + Q4.Rho2() - 2*(Q4*TComplex::Conjugate(Q2)*TComplex::Conjugate(Q2)).Re() - 4*(M-2)*Q2.Rho2() + 2*M*(M-3));

            p2->Fill(0.5, v2, w2);
            p4->Fill(0.5, v4, w4);
            p2sub[i]->Fill(0.5, v2, w2);
            p4sub[i]->Fill(0.5, v4, w4);
        }

    }

    double c2 = 0.;
    double c4 = 0.;
    c2 = pow(p2->GetBinContent(1), 0.5);
    c4 = pow(-1*(p4->GetBinContent(1)-2*p2->GetBinContent(1)*p2->GetBinContent(1)), 0.25);

    double c2sub[nSub];
    double c4sub[nSub];
    double c2sub_mean = 0.;
    double c4sub_mean = 0.;
    for(int i=0; i<nSub; i++){
        c2sub[i] = pow(p2sub[i]->GetBinContent(1), 0.5);
        c4sub[i] = pow(-1*(p4sub[i]->GetBinContent(1)-2*p2sub[i]->GetBinContent(1)*p2sub[i]->GetBinContent(1)), 0.25);    //
        c2sub_mean += c2sub[i]/nSub;
        c4sub_mean += c4sub[i]/nSub;
    }

    double c2var = 0.;
    double c4var = 0.;
    for(int i=0; i<nSub; i++){
        c2var += (c2sub[i]-c2sub_mean)*(c2sub[i]-c2sub_mean)/(nSub-1);
        c4var += (c4sub[i]-c4sub_mean)*(c4sub[i]-c4sub_mean)/(nSub-1);
    }
    c2var = sqrt(c2var/nSub);
    c4var = sqrt(c4var/nSub);

    printf("v2{2} = %f ± %f\n", c2, c2var);
    printf("v2{4} = %f ± %f\n", c4, c4var);

    // timer.Stop();
    // cout<<timer.CpuTime()<<"s"<<endl;
    delete f; f = nullptr;
    delete p2; p2 = nullptr;
    delete p4; p4 = nullptr;
    for (int s=0; s<nSub; ++s) {
        delete p2sub[s]; p2sub[s] = nullptr; 
        delete p4sub[s]; p4sub[s] = nullptr;
    }

    return 0;
}