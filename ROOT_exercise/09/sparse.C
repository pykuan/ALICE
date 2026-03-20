// Include all headers:
#include <THnSparse.h>
#include <Riostream.h>
// Main:
int sparse() {
    // Declaration:
    const int nDim = 4;
    int nBins[nDim] = {10, 20, 30, 40}; // number of bins in each dimension
    double min[nDim] = {0., 100., 1000., 2000.}; // lower bin boundary in each dimension
    double max[nDim] = {10., 120., 1030., 2040.}; // upper bin boundary in each dimension
    THnSparse* hs = new THnSparseD("hs", "someTitle", nDim, nBins, min, max);
    // Filling:
    double vector[nDim] = {5., 115.3, 1022.4, 2032.7}; // values to be filled
    hs->Fill(vector); // fill the above vector with default weight 1
    hs->Fill(vector, 2.); // fill the above vector with weight 2
    // Retrieve the content from particular bin:
    int binNumber[nDim] = {5, 15, 22, 32}; // specify bin number in each dimension
    cout << hs->GetBinContent(binNumber) << endl;
    return 0;
}