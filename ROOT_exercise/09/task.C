#include <THnSparse.h>
#include <Riostream.h>
#include "TRandom3.h"

int task(){

    delete gRandom;
    gRandom = new TRandom3;
    gRandom->SetSeed(123);

    const int nDim = 3;
    const int nSample = 10e6;
    int nBins[nDim] = {100, 100, 100};
    double min[nDim] = {-5, -5, -5};
    double max[nDim] = {5, 5, 5};

    THnSparse* hs = new THnSparseD("hs", "", nDim, nBins, min, max);

    double x, y, z;
    double vector[nDim];
    for(int i=0; i<nSample; i++){
        vector[0] = gRandom->Gaus(0, 1);
        vector[1] = gRandom->Gaus(0, 1);
        vector[2] = gRandom->Gaus(0, 1);
        hs->Fill(vector);
    }

    int dimensions_xy[2] = {0,1};
    int dimensions_y[1] = {1};
    THnSparse* proj_xy = hs->Projection(2, dimensions_xy);
    THnSparse* proj_y = hs->Projection(1, dimensions_y);

    int binNumber[nDim] = {50, 50, 50};
    cout << hs->GetBinContent(binNumber) << endl;
    
    //.L $ROOTSYS/tutorials/tree/drawsparse.C+
    drawsparse_draw(hs);
    // drawsparse_draw(proj_xy);
    // drawsparse_draw(proj_y);

    return 0;
}