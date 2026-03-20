// Include all headers:
#include <THnSparse.h>
#include <Riostream.h>
// Main:
int projection() {
    // Declaration of 3-dimensional sparse histogram:
    const int nDim = 3;
    int nBins[nDim] = {10, 20, 30}; // number of bins in each dimension
    double min[nDim] = {0., 100., 1000.}; // lower bin boundary in each dimension
    double max[nDim] = {10., 120., 1030.}; // upper bin boundary in each dimension
    THnSparse* hs_xyz = new THnSparseD("hs_xyz", "someTitle", nDim, nBins, min, max);
    // Projections onto three 2-dimensional sparse histograms:
    // a) project out "2 = z", i.e. keep "0 = x" horizontal vs. "1 = y" vertical
    int dimensions_xy[2] = {0,1};
    THnSparse *proj_xy = hs_xyz->Projection(2, dimensions_xy);
    // b) project out "1 = y", i.e. keep "0 = x" horizontal vs. "2 = z" vertical
    int dimensions_xz[2] = {0,2};
    THnSparse *proj_xz = hs_xyz->Projection(2, dimensions_xz);
    // c) project out "0 = x", i.e. keep "1 = y" horizontal vs. "2 = z" vertical
    int dimensions_yz[2] = {1,2};
    THnSparse *proj_yz = hs_xyz->Projection(2, dimensions_yz);
    return 0;
}