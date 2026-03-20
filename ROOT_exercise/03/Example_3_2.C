#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm> 
#include "TGraphErrors.h" 
#include "TCanvas.h"
#include "TColor.h"
#include "TStyle.h" 
#include "loadtable.h" 
#include "ShiftAlongXaxis.h"

using namespace std;
//use TArray instead
void Example_3_2(){

    gStyle->SetOptStat(0);
    auto grs = loadtable("HEPData-ins877822-v1-Table_3.csv");
    int markers[6] = {20, 24, 21, 25, 28, 34};
    int colors[6] = {kBlue, kBlue, kRed, kRed, kBlack, kGreen+3};
    TCanvas* c = new TCanvas("c", " ", 800, 600);
    c->SetGrid();
    TLegend *legend = new TLegend(0.4, 0.2, 0.7, 0.4);
    legend->SetBorderSize(1);

    for (size_t i=0; i<grs.size(); i++){
        auto gr = grs[i];
        gr->SetMarkerStyle(markers[i]);
        gr->SetMarkerColor(colors[i]);
        gr->SetLineColor(colors[i]);
        gr->SetMarkerSize(1.4);
        gr->SetMaximum(0.12);
        ShiftAlongXaxis(gr, i);
        gr->Draw(i==0 ? "AP" : "P SAME");
        gr->GetXaxis()->SetTitle("centrality percentile");
        gr->GetYaxis()->SetTitle("v_{2}");
        legend->AddEntry(gr, gr->GetTitle(), "p");
        legend->Draw("ap");    
    }

    

    c->Print("Example_3_2.pdf");


}