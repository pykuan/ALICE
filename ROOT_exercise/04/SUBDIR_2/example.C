#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
using namespace std;

int example(){

    TFile *file = new TFile("example.root", "recreate");
    TDirectoryFile *dirfile = new TDirectoryFile("dirfile", "dirfile_title");
    TList *list = new TList();
    list->SetName("list_name");

    TF1 *f = new TF1("f", "[0]*x*exp(-1*[1]*x)", 0, 1);
    f->SetParameters(0.1, 0.4);
    TH1F *h = new TH1F("h", "h", 10, 0, 1);
    h->SetDirectory(0);
    h->FillRandom("f", 10000);   
    list->Add(h); 
    dirfile->Add(list);
    dirfile->Write(dirfile->GetName(),TObject::kSingleKey);
    file->Close();
    
    return 0;
}