//為什麼需要2 order以上correlation
#include "TMath.h"
#include "TRandom3.h"
#include "iostream"

using namespace std;

int corr_proof(){
    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(0);

    const int n = 2;
    const double dphi_k = TMath::Pi();
    const int k = 10;
    const int M = 10000;

    double phi = 0.;
    double phi_vector[M];
    int j;
    for(int i=0; i<k; i++){
        j = i*2;
        phi = gRandom->Uniform(0, 2*TMath::Pi());
        phi_vector[j] = phi;
        phi_vector[j+1] = phi + dphi_k;
    }

    for(int i=2*k; i<M; i++){
        phi = gRandom->Uniform(0, 2*TMath::Pi());
        phi_vector[i] = phi;
    }

    double cos_diff_avg = 0.;
    for(int i=0; i<2*k; i++){
        for(int j=0; j<2*k; j++){
            if(i!=j){
                cos_diff_avg += TMath::Cos(n*(phi_vector[i] - phi_vector[j]));
            }
        }
    }
    cos_diff_avg /= 2*k*(2*k-1);
    cout<<cos_diff_avg<<endl;

    cos_diff_avg = 0.;
    for(int i=2*k; i<M; i++){
        for(int j=2*k; j<M; j++){
            if(i!=j){
                cos_diff_avg += TMath::Cos(n*(phi_vector[i] - phi_vector[j]));
            }
        }
    }
    cos_diff_avg /= (M-2*k)*(M-2*k-1);
    cout<<cos_diff_avg<<endl;

    cos_diff_avg = 0.;
    for(int i=0; i<M; i++){
        for(int j=0; j<M; j++){
            if(i!=j){
                cos_diff_avg += TMath::Cos(n*(phi_vector[i] - phi_vector[j]));
            }
        }
    }
    cos_diff_avg /= M*(M-1);
    cout<<cos_diff_avg<<endl;

    return 0;
}