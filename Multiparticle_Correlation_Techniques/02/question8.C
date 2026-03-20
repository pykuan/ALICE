#include <iostream>
#include "TF1.h"
#include "TMath.h"
#include "TRandom3.h"
#include <TStopwatch.h>
#include <TComplex.h>
#include <cmath>

using namespace std;
int question8(){

    TStopwatch timer;
    timer.Start();

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    TF1* f = new TF1("f", "1/(2*TMath::Pi())*(1+2*0.05*cos(2*(x-[0])))", 0, TMath::TwoPi());
    const int nEvent = 10000;
    const int nParticle = 500;
    double Psi = 0.;
    double phi = 0.;
    TComplex Q2(0,0);
    double v2_over_particle = 0.;
    double v2_over_event = 0.;
    double v2 = 0.;

    for(int i=0; i<nEvent; i++){

        TComplex Q2(0,0);
        
        Psi = gRandom->Uniform(0., TMath::TwoPi());
        f->SetParameters(Psi);
        for(int j=0; j<nParticle; j++){
            phi = f->GetRandom();
            Q2 += TComplex::Exp(TComplex(0, 2*phi));
        }


        v2_over_particle = 1./(nParticle*(nParticle-1)) *(Q2.Rho2() - nParticle);
        v2_over_event += v2_over_particle/nEvent;
    }
    v2 = sqrt(v2_over_event);
    cout<<v2<<endl;

    timer.Stop();
    cout<<timer.CpuTime()<<"s"<<endl;

    return 0;
}