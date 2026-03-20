#include <iostream>
#include "TF1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TRandom3.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <cmath>

using namespace std;
int question12(){

    // TStopwatch timer;
    // timer.Start();

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    TF1* f = new TF1("f", "1/(2*TMath::Pi())*(1+2*0.05*cos(2*(x-[0])))", 0, TMath::TwoPi());
    TProfile* p4 = new TProfile("p4", "4-p correlation", 1, 0., 1.);
    p4->Sumw2(); 
    const int nEvent = 10000;
    int M = 0; //multiplicity 
    double Psi = 0.;
    double phi = 0.;
    TComplex Q2(0,0);
    TComplex Q4(0,0);
    double weight = 0.;
    double v2_over_particle = 0.;
    double v2 = 0.;
    double v2_err = 0.;

    for(int i=0; i<nEvent; i++){

        TComplex Q2(0,0);
        TComplex Q4(0,0);
        M = gRandom->Uniform(50,500);
        
        Psi = gRandom->Uniform(0., TMath::TwoPi());
        f->SetParameters(Psi);
        for(int j=0; j<M; j++){
            phi = f->GetRandom();
            Q2 += TComplex::Exp(TComplex(0, 2*phi));
            Q4 += TComplex::Exp(TComplex(0, 2*2*phi));
        }

        weight = (double)M*(M-1)*(M-2)*(M-3);

        v2_over_particle = 1./weight *(Q2.Rho2()*Q2.Rho2() + Q4.Rho2() - 2*(Q4*TComplex::Conjugate(Q2)*TComplex::Conjugate(Q2)).Re() - 4*(M-2)*Q2.Rho2() + 2*M*(M-3));

        p4->Fill(0.5, v2_over_particle, weight);

    }
    v2 = pow(p4->GetBinContent(1),0.25);
    v2_err = 1./4.*pow(p4->GetBinContent(1),-0.75) * p4->GetBinError(1); //error propagation
    
    printf("v2 = %f +- %f sigma \n", v2, abs((v2-0.05)/v2_err));

    // timer.Stop();
    // cout<<timer.CpuTime()<<"s"<<endl;
    delete p4;

    return 0;
}