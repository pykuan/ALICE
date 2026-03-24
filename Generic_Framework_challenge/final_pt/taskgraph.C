#include "TFile.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TColor.h"
#include "TLegend.h"

int taskgraph(){
    TFile* file = TFile::Open("file.root", "read");
    // TGraph* g1 = (TGraph*)file->Get("g_true");
    TGraph* g2 = (TGraph*)file->Get("g_u");
    TGraph* g3 = (TGraph*)file->Get("g_nu");
    TGraph* g4 = (TGraph*)file->Get("g_nuw");

    // g1->SetMarkerStyle(20);
    g2->SetMarkerStyle(25);
    g3->SetMarkerStyle(21);
    g4->SetMarkerStyle(24);

    // g1->SetMarkerSize(0.7);

    // g1->SetMarkerColor(kBlack);
    g2->SetMarkerColor(kBlack);
    g3->SetMarkerColor(kRed);
    g4->SetMarkerColor(kBlue);

    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(" ");
    // mg->Add(g1);
    mg->Add(g2);
    mg->Add(g3);
    mg->Add(g4);
    mg->GetXaxis()->SetTitle("#LT#LTk#GT#GT");
    mg->GetYaxis()->SetTitle("#LT#LTk#GT#GT / 10^{-k}");
    mg->GetXaxis()->SetLimits(1, 9); 
    mg->SetMinimum(0);
    mg->SetMaximum(3.2);

    TLegend *leg = new TLegend(0.1, 0.7, 0.6, 0.85);
    // leg->AddEntry(g1, "input values", "p");
    leg->AddEntry(g2, "uniform acceptance", "p");
    leg->AddEntry(g3, "non-uniform acceptance", "p");
    leg->AddEntry(g4, "non-uniform acceptance + #varphi-weights", "p");
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    // TLine line(1, 0, 9, 0);
    

    TCanvas* c = new TCanvas("c", "", 800, 600);
    mg->Draw("ap");
    leg->Draw();
    // line.Draw();
    c->Print("result.png");

    delete c;
    file->Close();
    delete file;
    return 0;
}