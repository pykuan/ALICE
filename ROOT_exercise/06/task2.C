#include <iostream>
#include "TH1F.h"
#include "TRandom3.h"
#include <cmath>
#include "TF3.h"
#include "TH3F.h"
#include "TString.h"

using namespace std;

int task2(){

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(0);

    const int nEntries = 100000;
    const int nBins = 100;
    const double lLimit = 0, uLimit = 10;
    TFile *file = new TFile("file_task2.root", "RECREATE");

    TF3* f = new TF3("f", "exp(x*y+x*z+y*z)", lLimit, uLimit, lLimit, uLimit, lLimit, uLimit);
    TH3F* hxyz = new TH3F("hxyz", "", nBins, lLimit, uLimit, nBins, lLimit, uLimit, nBins, lLimit, uLimit);
    hxyz->FillRandom("f", nEntries);
    hxyz->Scale(1. / hxyz->Integral("width"));

    double dx = hxyz->GetXaxis()->GetBinWidth(1);
    double dy = hxyz->GetYaxis()->GetBinWidth(1);
    double dz = hxyz->GetZaxis()->GetBinWidth(1);

    TH1F *hx = (TH1F*)hxyz->ProjectionX(); hx->Scale(dy*dz);
    TH1F *hy = (TH1F*)hxyz->ProjectionY(); hy->Scale(dx*dz);
    TH1F *hz = (TH1F*)hxyz->ProjectionZ(); hz->Scale(dx*dy);
    TH2F *hxy = (TH2F*)hxyz->Project3D("xy"); hxy->Scale(dz);
    TH2F *hxz = (TH2F*)hxyz->Project3D("xz"); hxz->Scale(dy);
    TH2F *hyz = (TH2F*)hxyz->Project3D("yz"); hyz->Scale(dx);

    // float integral = hxy->Integral("width");
    // cout<<integral<<endl;


    double Ex=0, Ey=0, Ez=0, Exy=0, Eyz=0, Exz=0, Exyz=0, norm=0;
    for (int ix=1; ix<=hxyz->GetNbinsX(); ++ix){
        double x = hxyz->GetXaxis()->GetBinCenter(ix);
        for (int iy=1; iy<=hxyz->GetNbinsY(); ++iy){
            double y = hxyz->GetYaxis()->GetBinCenter(iy);
            for (int iz=1; iz<=hxyz->GetNbinsZ(); ++iz){
                double z = hxyz->GetZaxis()->GetBinCenter(iz);
                double f = hxyz->GetBinContent(ix,iy,iz);

                Ex += x*f;
                Ey += y*f;
                Ez += z*f;
                Exy += x*y*f;
                Exz += x*z*f;
                Eyz += y*z*f;
                Exyz += x*y*z*f;
                norm += f;
            }
        }
    }
    Ex /= norm;  Ey /= norm;  Ez /= norm; Exy /= norm; Exz /= norm; Eyz /= norm; Exyz /= norm;

    double cxy = Exy-Ex*Ey;
    double cxz = Exz-Ex*Ez;
    double cyz = Eyz-Ey*Ez;
    double cxyz = Exyz-Exy*Ez-Exz*Ey-Eyz*Ex+2*Ex*Ey*Ez;
    printf("<xy>c=%f, <xz>c=%f, <yz>c=%f, <xyz>c=%f \n", cxy, cxz, cyz, cxyz);

    hxyz->Write(hxyz->GetName(), TObject::kSingleKey + TObject::kOverwrite);

    file->Close();
    delete file; 
    file = nullptr; 

    return 0;
}
