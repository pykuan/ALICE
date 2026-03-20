#include <iostream>
#include "TF1.h"
#include "TProfile.h"
#include "TMath.h"
#include "TRandom3.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <cmath>

using namespace std;
int question10(){

    // TStopwatch timer;
    // timer.Start();

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    TF1* f = new TF1("f", "1/(2*TMath::Pi())*(1+2*0.05*cos(2*(x-[0])))", 0, TMath::TwoPi());
    TProfile* p2 = new TProfile("p2", "2-p correlation", 1, 0., 1.);
    p2->Sumw2(); 
    const int nEvent = 10000;
    int nParticle = 0; //multiplicity 
    double Psi = 0.;
    double phi = 0.;
    TComplex Q2(0,0);
    double weight = 0.;
    double v2_over_particle = 0.;
    // double v2_over_event = 0.;
    double v2 = 0.;
    double v2_err = 0.;

    for(int i=0; i<nEvent; i++){

        TComplex Q2(0,0);
        nParticle = gRandom->Uniform(50,500);
        
        Psi = gRandom->Uniform(0., TMath::TwoPi());
        f->SetParameters(Psi);
        for(int j=0; j<nParticle; j++){
            phi = f->GetRandom();
            Q2 += TComplex::Exp(TComplex(0, 2*phi));
        }


        v2_over_particle = 1./(nParticle*(nParticle-1)) *(Q2.Rho2() - nParticle);
        // v2_over_event += v2_over_particle/nEvent;
        weight = nParticle*(nParticle-1);
        p2->Fill(0.5, v2_over_particle, weight);
        // cout<<sqrt(v2_over_particle)<<endl;
    }
    v2 = sqrt(p2->GetBinContent(1));
    v2_err = 1/(2*sqrt(p2->GetBinContent(1))) * p2->GetBinError(1);
    printf("v2 = %f +- %f sigma \n", v2, abs((v2-0.05)/v2_err));

    // timer.Stop();
    // cout<<timer.CpuTime()<<"s"<<endl;

    return 0;
}