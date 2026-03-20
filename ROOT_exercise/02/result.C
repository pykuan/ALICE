#include <iostream>
#include <fstream>
#include <iomanip>
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
using namespace std;

int result(){
    gStyle->SetOptStat(0);
    ifstream in;
    in.open("dataPoints.log");

    const int nPoints = 10;
    float x[nPoints], y[nPoints], xerr[nPoints], yerr[nPoints];
    for(int i=0; i<nPoints; i++){
        in >> x[i] >> y[i] >> yerr[i];
        xerr[i]=0;
    }

    TGraphErrors *gr = new TGraphErrors(10, x, y, xerr, yerr);

    TCanvas *c = new TCanvas("c", "c", 900, 600);
    
    gr->GetXaxis()->SetTitle("x");
    gr->GetYaxis()->SetTitle("y");
    gr->SetMarkerColor(4);
    gr->SetLineColor(4);
    gr->SetMarkerStyle(24);
    gr->Draw();

    TLegend *legend = new TLegend(0.6, 0.75, 0.77, 0.85);
    legend->AddEntry(gr, "dataPoints");
    legend->Draw("ap");
    legend->SetBorderSize(0);
    c->Print("gr.png");
    c->Print("gr.pdf");
    c->Print("gr.eps");

    in.close();

    delete gr;
    delete c;
    delete legend;

    return 0;
}