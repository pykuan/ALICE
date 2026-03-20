#include <iostream>
#include "TF1.h"
#include "TMath.h"
#include "TRandom3.h"
#include <cmath>

using namespace std;
int question3(){

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    TF1* f = new TF1("f", "1/(2*TMath::Pi())*(1+2*0.05*cos(2*(x-[0])))", 0, TMath::TwoPi());
    const int nEvent = 10000;
    const int nParticle = 500;
    double Psi = 0.;
    double psi = 0.;
    double v2_over_particle = 0.;
    double v2_over_event = 0.;
    double v2 = 0.;

    for(int i=0; i<nEvent; i++){
        v2_over_particle = 0;
        Psi = gRandom->Uniform(0., TMath::TwoPi());
        f->SetParameters(Psi);
        for(int j=0; j<nParticle; j++){
            psi = f->GetRandom();
            v2_over_particle += TMath::Cos(2*(psi-Psi))/nParticle;
        }
        v2_over_event += v2_over_particle/nEvent;
    }
    v2 = v2_over_event;
    cout<<v2<<endl;

    return 0;
}