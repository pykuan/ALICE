#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include <string> 
using namespace std;

int validation() {

    TH1* hists[5] = {NULL};

    for(int i=0; i<5; i++){
        string string_path = "SUBDIR_"+to_string(i)+"/example.root";
        TFile *outputFile = TFile::Open(string_path.c_str(),"read");
        TDirectoryFile *directoryFile = dynamic_cast<TDirectoryFile*>(outputFile->Get("dirfile"));
        TList *list = dynamic_cast<TList*>(directoryFile->Get("list_name"));
        TH1F *hist = dynamic_cast<TH1F*>(list->FindObject("h"));
        hists[i] = hist;
    }

    float bincontent[10] = {0.};

    for(int i=0; i<10; i++){
        bincontent[i] = 0.;
        for(int j=0; j<5; j++){
            bincontent[i] += hists[j]->GetBinContent(i+1);
        }
        cout<<i+1<<": "<<bincontent[i]<<endl;
    }

    return 0;
}
//hadd merged_output.root SUBDIR_*/test.root