{
    const Int_t nPoints = 10; 
    Double_t x[nPoints] = {0.5,1.5,2.5,3.5,4.5,5.5,6.5,7.5,8.5,9.5}; // x-coordinates of markers
    Double_t ex[nPoints] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0}; // bin widths to which each marker corresponds to (play a bit by using some non-zero values as well)
    Double_t y[nPoints] = {4.537350e-04,5.931182e-04,7.931665e-04,1.016944e-03,1.272064e-03,
                        1.561168e-03,1.820135e-03,2.140148e-03,2.427010e-03,2.741685e-03}; // values
    Double_t ey[nPoints] = {8.835999e-05,9.907982e-05,1.102704e-04,1.216924e-04,1.319139e-04,
                            1.452088e-04,1.567778e-04,1.701033e-04,1.814373e-04,1.957709e-04}; // errors

    TGraphErrors *gr = new TGraphErrors(nPoints, x, y, ex, ey);

    TCanvas *c = new TCanvas("c", "c", 900, 600);

    gr->SetMarkerStyle(20);
    gr->SetMarkerColor(4);
    gr->SetLineColor(4);
    gr->GetYaxis()->SetTitle("v_{2}");
    gr->GetXaxis()->SetTitle("centrality");
    gr->Draw("ap");
    c->Print("Example_3.pdf");
}
