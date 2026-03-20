#include "TComplex.h"
#include "TProfile.h"
#include "TStopwatch.h"
#include "Riostream.h"
#include "TMath.h"



int test()
{

const int nEvents = 100;
const int nBins = 200;
TF1* pdf = new TF1("pdf", "1./TMath::TwoPi()*( 1 + 2*[1]*cos(x-[0]) + 2*[2]*cos(2*(x-[0])) + 2*[3]*cos(3*(x-[0])) + 2*[4]*cos(4*(x-[0])) + 2*[5]*cos(5*(x-[0])) + 2*[6]*cos(6*(x-[0])) )", 0, TMath::TwoPi());
TH1F *h_dndphi_nu = new TH1F("h_dndphi_nu", "dndphi_nu", nBins, 0, TMath::TwoPi());
TH1F *h_dndphi_u = new TH1F("h_dndphi_u", "dndphi_u", nBins, 0, TMath::TwoPi());
TH1F *h_weight = new TH1F("h_weight", "weight", nBins, 0, TMath::TwoPi());
TFile* file = new TFile("file.root", "recreate");

delete gRandom;
gRandom = new TRandom3;
gRandom->SetSeed(123);

const int nParticles_acc = 1e5;
const int nParticles = 500;
double phi;
double Psi;
bool bKeep;


// acceptance dN/dphi histogram
for(int k=0; k<nParticles_acc; k++){
  bKeep = (int)gRandom->Uniform(0, 100) % 2;
  phi = (double)gRandom->Uniform(0, 2*TMath::Pi());
  h_dndphi_u->Fill(phi);
  
  if(phi>=TMath::Pi()/3 && phi<2*TMath::Pi()/3){
    if(bKeep){h_dndphi_nu->Fill(phi);}
  }
  else {h_dndphi_nu->Fill(phi);}

}

//weights histogram
for(int i=1; i<nBins+1; i++){
  h_weight->SetBinContent(i, 1./h_dndphi_nu->GetBinContent(i));
}



vector<double> angles; 
vector<double> weight;
// for every events...
delete gRandom;
gRandom = new TRandom3;
gRandom->SetSeed(0);

for(int n=1; n<7; n++){
  pdf->SetParameter(n, 0.04+n*0.01);
}
Psi = gRandom->Uniform(0, TMath::TwoPi());
pdf->SetParameter(0, Psi);

for(int k=0; k<nParticles; k++){

  phi = (double)pdf->GetRandom();
  bKeep = (int)gRandom->Uniform(0, 100) % 2;

  if(phi>=TMath::Pi()/3 && phi<2*TMath::Pi()/3){
    if(bKeep){angles.push_back(phi);}
  }
  else {angles.push_back(phi);}
  
  if (phi >= 0 && phi < TMath::TwoPi()) {
    int b = h_weight->FindBin(phi);
    weight.push_back(h_weight->GetBinContent(b));
  } 
}

// double angles[nParticles];
// double weights[nParticles];
// copy(angles.begin(), angles.end(), angles);
// copy(weight.begin(), weight.end(), weights);

// for(int i:angles){
//   cout<<i<<endl;
// }

//I want to turn angles and weight which are vectors, into angles and weights which are arrays

pdf->Write(pdf->GetName(), TObject::kSingleKey + TObject::kOverwrite);
h_dndphi_nu->Write(h_dndphi_nu->GetName(), TObject::kSingleKey + TObject::kOverwrite);
h_dndphi_u->Write(h_dndphi_u->GetName(), TObject::kSingleKey + TObject::kOverwrite);
h_weight->Write(h_weight->GetName(), TObject::kSingleKey + TObject::kOverwrite);
file->Close();
delete file;
file = nullptr;

return 0;
}
