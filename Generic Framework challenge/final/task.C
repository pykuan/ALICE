#include "TComplex.h"
#include "TProfile.h"
#include "TFile.h"
#include "TF1.h"
#include "TH1F.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "Riostream.h"
#include <vector>

const int nEvents = 1000;
const int nBins = 200;
const int nPar_acc = 1e6;//for acceptance, not nParticles
const int nPar = 500;//nParticles before remove
int nParticles = 0;
double phi;
double Psi;
bool bKeep;
vector<double> angles; 
vector<double> weights;
TF1* pdf = new TF1("pdf", "1./TMath::TwoPi()*( 1 + 2*[1]*cos(x-[0]) + 2*[2]*cos(2*(x-[0])) + 2*[3]*cos(3*(x-[0])) + 2*[4]*cos(4*(x-[0])) + 2*[5]*cos(5*(x-[0])) + 2*[6]*cos(6*(x-[0])) )", 0, TMath::TwoPi());
TH1F *h_dndphi_nu = new TH1F("h_dndphi_nu", "dndphi_nu", nBins, 0, TMath::TwoPi());
TH1F *h_dndphi_u = new TH1F("h_dndphi_u", "dndphi_u", nBins, 0, TMath::TwoPi());
TH1F *h_weight = new TH1F("h_weight", "weight", nBins, 0, TMath::TwoPi());
TFile* file = new TFile("file.root", "recreate");

int h1, h2, h3, h4, h5, h6, h7, h8;

// Book Q-vector components: 
const Int_t maxCorrelator = 8; // We will not go beyond 8-p correlations
const Int_t maxHarmonic = 7; //sum+1
const Int_t maxPower = maxCorrelator+1; 
TComplex Qvector[maxHarmonic][maxPower]; // All needed Q-vector components

// Store the final results here:
//  Remark: [2][maxCorrelator] => [Cos,Sin][<2>,<3>,<4>,<5>,<6>,<7>,<8>]
TProfile *recursion[2][maxCorrelator] = {{NULL}}; // Correlations calculated from Q-vector components using recursive algorithm 

double ydata1[maxHarmonic];//input values
double ydata2[maxHarmonic];//uniform acceptance
double ydata3[maxHarmonic];//non-uniform acceptance
double ydata4[maxHarmonic];//non-uniform acceptance+weights
double xdata[maxHarmonic] = {2, 3, 4, 5, 6, 7, 8};
double vn[maxCorrelator];

//=======================================================================================================================

void Acceptance()
{
  delete gRandom;
  gRandom = new TRandom3;
  gRandom->SetSeed(123);

  // acceptance dN/dphi histogram
  for(int k=0; k<nPar_acc; k++){
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
    h_weight->SetBinContent(i, (h_dndphi_nu->GetBinContent(i)>0) ? (1./h_dndphi_nu->GetBinContent(i)) *1e4 : 0.0);
  }

  pdf->Write(pdf->GetName(), TObject::kSingleKey + TObject::kOverwrite);
  h_dndphi_nu->Write(h_dndphi_nu->GetName(), TObject::kSingleKey + TObject::kOverwrite);
  h_dndphi_u->Write(h_dndphi_u->GetName(), TObject::kSingleKey + TObject::kOverwrite);
  h_weight->Write(h_weight->GetName(), TObject::kSingleKey + TObject::kOverwrite);
}

void TrueHarmonic(){
  for(int n=1; n<maxCorrelator; n++){
    vn[n-1] = 0.04+n*0.01;
  }
  ydata1[0] = pow(vn[1], 2) / pow(10., -2); //<2>
  ydata1[1] = vn[0]*vn[4]*vn[5] / pow(10., -3); //<3>
  ydata1[2] = pow(vn[1], 2)*pow(vn[2], 2) / pow(10., -4); //<4>
  ydata1[3] = pow(vn[2], 3)*vn[3]*vn[4] / pow(10., -5); //<5>
  ydata1[4] = pow(vn[0], 2)*pow(vn[1], 2)*pow(vn[2], 2) / pow(10., -6); //<6>
  ydata1[5] = pow(vn[0], 2)*vn[1]*vn[2]*vn[4]*pow(vn[5], 2) / pow(10., -7); //<7>
  ydata1[6] = vn[1]*pow(vn[2], 2)*vn[3]*pow(vn[4], 2)*pow(vn[5], 2) / pow(10., -8); //<8>
}


void MCnu()
{
  angles.clear();
  weights.clear();
  angles.reserve(nPar);
  weights.reserve(nPar);

  for(int n=1; n<maxHarmonic; n++){
    pdf->SetParameter(n, vn[n-1]);
  }
  Psi = gRandom->Uniform(0, TMath::TwoPi());
  pdf->SetParameter(0, Psi);

  for(int k=0; k<nPar; k++){

    phi = (double)pdf->GetRandom();
    bKeep = (int)gRandom->Uniform(0, 100) % 2;

    if(phi>=TMath::Pi()/3 && phi<2*TMath::Pi()/3){
      if(bKeep){
        int b = h_weight->FindBin(phi);
        if(h_weight->GetBinContent(b)<=0) continue;
        weights.push_back(h_weight->GetBinContent(b));
        angles.push_back(phi);
      }
    }
    else {
      int b = h_weight->FindBin(phi);
      if(h_weight->GetBinContent(b)<=0) continue;
      weights.push_back(h_weight->GetBinContent(b));
      angles.push_back(phi);

    } 
  }
  nParticles = angles.size();
}

void MCu()
{
  angles.clear();
  weights.clear();
  angles.reserve(nPar);
  weights.reserve(nPar);

  for(int n=1; n<maxHarmonic; n++){
    pdf->SetParameter(n, 0.04+n*0.01);
  }
  Psi = gRandom->Uniform(0, TMath::TwoPi());
  pdf->SetParameter(0, Psi);

  for(int k=0; k<nPar; k++){
    phi = (double)pdf->GetRandom();
    weights.push_back(1.);
    angles.push_back(phi);
  }
  nParticles = angles.size();
}

void Cosmetics()
{
 // Book everything here.
  
 for(Int_t cs=0;cs<2;cs++) 
 {
  for(Int_t c=0;c<maxCorrelator;c++)
  {
   recursion[cs][c] = new TProfile("","",1,0.,1.);
   recursion[cs][c]->Sumw2();
  } // end of for(Int_t c=0;c<maxCorrelator;c++)
 } // end of for(Int_t cs=0;cs<2;cs++) 

} // void Cosmetics()

void CalculateQvectors(bool bUseWeights)
{
 // Calculate Q-vectors.

 // a) Make sure all Q-vectors are initially zero;
 // b) Calculate Q-vectors for available angles and weights. 

 // a) Make sure all Q-vectors are initially zero:
 for(Int_t h=0;h<maxHarmonic;h++)
 {
  for(Int_t p=0;p<maxPower;p++)
  {
   Qvector[h][p] = TComplex(0.,0.);
  } //  for(Int_t p=0;p<maxPower;p++)
 } // for(Int_t h=0;h<maxHarmonic;h++)

 // b) Calculate Q-vectors for available angles and weights: 
 Double_t dPhi = 0.; // particle angle
 Double_t wPhi = 1.; // particle weight
 Double_t wPhiToPowerP = 1.; // particle weight raised to power p
 for(Int_t i=0;i<nParticles;i++) // loop over particles
 {
  dPhi = angles[i];
  if(bUseWeights){wPhi = weights[i];}
  for(Int_t h=0;h<maxHarmonic;h++)
  {
   for(Int_t p=0;p<maxPower;p++)
   {
    if(bUseWeights){wPhiToPowerP = pow(wPhi,p);}
    Qvector[h][p] += TComplex(wPhiToPowerP*TMath::Cos(h*dPhi),wPhiToPowerP*TMath::Sin(h*dPhi));
   } //  for(Int_t p=0;p<maxPower;p++)
  } // for(Int_t h=0;h<maxHarmonic;h++)
 } //  for(Int_t i=0;i<nParticles;i++) // loop over particles

} // void CalculateQvectors()

TComplex Q(Int_t n, Int_t p)
{
 // Using the fact that Q{-n,p} = Q{n,p}^*. 
 
 if(n>=0){return Qvector[n][p];} 
 return TComplex::Conjugate(Qvector[-n][p]);
 
} // TComplex Q(Int_t n, Int_t p)

TComplex Recursion(Int_t n, Int_t* harmonic, Int_t mult = 1, Int_t skip = 0) 
{
 // Calculate multi-particle correlators by using recursion (an improved faster version) originally developed by 
 // Kristjan Gulbrandsen (gulbrand@nbi.dk). 

  Int_t nm1 = n-1;
  TComplex c(Q(harmonic[nm1], mult));
  if (nm1 == 0) return c;
  c *= Recursion(nm1, harmonic);
  if (nm1 == skip) return c;

  Int_t multp1 = mult+1;
  Int_t nm2 = n-2;
  Int_t counter1 = 0;
  Int_t hhold = harmonic[counter1];
  harmonic[counter1] = harmonic[nm2];
  harmonic[nm2] = hhold + harmonic[nm1];
  TComplex c2(Recursion(nm1, harmonic, multp1, nm2));
  Int_t counter2 = n-3;
  while (counter2 >= skip) {
    harmonic[nm2] = harmonic[counter1];
    harmonic[counter1] = hhold;
    ++counter1;
    hhold = harmonic[counter1];
    harmonic[counter1] = harmonic[nm2];
    harmonic[nm2] = hhold + harmonic[nm1];
    c2 += Recursion(nm1, harmonic, multp1, counter2);
    --counter2;
  }
  harmonic[nm2] = harmonic[counter1];
  harmonic[counter1] = hhold;

  if (mult == 1) return c-c2;
  return c-Double_t(mult)*c2;

} // TComplex AliFlowAnalysisWithMultiparticleCorrelations::Recursion(Int_t n, Int_t* harmonic, Int_t mult = 1, Int_t skip = 0) 

void CalculateData(bool bUniform, bool bUseWeights){
  delete gRandom;
  gRandom = new TRandom3;
  gRandom->SetSeed(0);

 for(Int_t cs=0;cs<2;cs++) 
 {
  for(Int_t c=0;c<maxCorrelator;c++)
  {
   recursion[cs][c] -> Reset();
  }
 }


  for(int j=0; j<nEvents; j++){

  if(bUniform){MCu();}
  else {MCnu();}

  // Calculate Q-vectors for available angles and weights;
  CalculateQvectors(bUseWeights);

  // Calculate n-particle correlations from Q-vectors (using recursion):
  //  2-p correlations:
  h1=-2; h2=2;
  Int_t harmonics_Two_Num[2] = {h1,h2};       
  Int_t harmonics_Two_Den[2] = {0,0};       
  TComplex twoRecursion = Recursion(2,harmonics_Two_Num)/Recursion(2,harmonics_Two_Den).Re();
  Double_t wTwoRecursion = Recursion(2,harmonics_Two_Den).Re();
  recursion[0][0]->Fill(0.5,twoRecursion.Re(),wTwoRecursion); // <<cos(h1*phi1+h2*phi2)>>
  recursion[1][0]->Fill(0.5,twoRecursion.Im(),wTwoRecursion); // <<sin(h1*phi1+h2*phi2)>>
  //  3-p correlations:
  h1=-5; h2=-1; h3=6;
  Int_t harmonics_Three_Num[3] = {h1,h2,h3};       
  Int_t harmonics_Three_Den[3] = {0,0,0};       
  TComplex threeRecursion = Recursion(3,harmonics_Three_Num)/Recursion(3,harmonics_Three_Den).Re();
  Double_t wThreeRecursion = Recursion(3,harmonics_Three_Den).Re();
  recursion[0][1]->Fill(0.5,threeRecursion.Re(),wThreeRecursion); // <<cos(h1*phi1+h2*phi2+h3*phi3)>>
  recursion[1][1]->Fill(0.5,threeRecursion.Im(),wThreeRecursion); // <<sin(h1*phi1+h2*phi2+h3*phi3)>>
  //  4-p correlations:
  h1=-3; h2=-2; h3=2; h4=3;
  Int_t harmonics_Four_Num[4] = {h1,h2,h3,h4};       
  Int_t harmonics_Four_Den[4] = {0,0,0,0};       
  TComplex fourRecursion = Recursion(4,harmonics_Four_Num)/Recursion(4,harmonics_Four_Den).Re();
  Double_t wFourRecursion = Recursion(4,harmonics_Four_Den).Re();
  recursion[0][2]->Fill(0.5,fourRecursion.Re(),wFourRecursion); // <<cos(h1*phi1+h2*phi2+h3*phi3+h4*phi4)>>
  recursion[1][2]->Fill(0.5,fourRecursion.Im(),wFourRecursion); // <<<sin(h1*phi1+h2*phi2+h3*phi3+h4*phi4)>>
  //  5-p correlations:
  h1=-5; h2=-4; h3=3; h4=3; h5=3;
  Int_t harmonics_Five_Num[5] = {h1,h2,h3,h4,h5};       
  Int_t harmonics_Five_Den[5] = {0,0,0,0,0};       
  TComplex fiveRecursion = Recursion(5,harmonics_Five_Num)/Recursion(5,harmonics_Five_Den).Re();
  Double_t wFiveRecursion = Recursion(5,harmonics_Five_Den).Re();
  recursion[0][3]->Fill(0.5,fiveRecursion.Re(),wFiveRecursion); // <<cos(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5)>>
  recursion[1][3]->Fill(0.5,fiveRecursion.Im(),wFiveRecursion); // <<<sin(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5)>>
  //  6-p correlations:
  h1=-2; h2=-2; h3=-1; h4=-1; h5=3; h6=3;
  Int_t harmonics_Six_Num[6] = {h1,h2,h3,h4,h5,h6};       
  Int_t harmonics_Six_Den[6] = {0,0,0,0,0,0};       
  TComplex sixRecursion = Recursion(6,harmonics_Six_Num)/Recursion(6,harmonics_Six_Den).Re();
  Double_t wSixRecursion = Recursion(6,harmonics_Six_Den).Re();
  recursion[0][4]->Fill(0.5,sixRecursion.Re(),wSixRecursion); // <<cos(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5+h6*phi6)>>
  recursion[1][4]->Fill(0.5,sixRecursion.Im(),wSixRecursion); // <<<sin(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5+h6*phi6)>>
  //  7-p correlations:
  h1=-6; h2=-5; h3=-1; h4=1; h5=2; h6=3; h7=6;
  Int_t harmonics_Seven_Num[7] = {h1,h2,h3,h4,h5,h6,h7};       
  Int_t harmonics_Seven_Den[7] = {0,0,0,0,0,0,0};       
  TComplex sevenRecursion = Recursion(7,harmonics_Seven_Num)/Recursion(7,harmonics_Seven_Den).Re();
  Double_t wSevenRecursion = Recursion(7,harmonics_Seven_Den).Re();
  recursion[0][5]->Fill(0.5,sevenRecursion.Re(),wSevenRecursion); // <<cos(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5+h6*phi6+h7*phi7)>>
  recursion[1][5]->Fill(0.5,sevenRecursion.Im(),wSevenRecursion); // <<<sin(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5+h6*phi6+h7*phi7)>>
  //  8-p correlations:
  h1=-6; h2=-6; h3=-5; h4=2; h5=3; h6=3; h7=4; h8=5;
  Int_t harmonics_Eight_Num[8] = {h1,h2,h3,h4,h5,h6,h7,h8};       
  Int_t harmonics_Eight_Den[8] = {0,0,0,0,0,0,0,0};       
  TComplex eightRecursion = Recursion(8,harmonics_Eight_Num)/Recursion(8,harmonics_Eight_Den).Re();
  Double_t wEightRecursion = Recursion(8,harmonics_Eight_Den).Re();
  recursion[0][6]->Fill(0.5,eightRecursion.Re(),wEightRecursion); // <<cos(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5+h6*phi6+h7*phi7+h8*phi8)>>
  recursion[1][6]->Fill(0.5,eightRecursion.Im(),wEightRecursion); // <<<sin(h1*phi1+h2*phi2+h3*phi3+h4*phi4+h5*phi5+h6*phi6+h7*phi7+h8*phi8)>>
 }
 
 // e) Final printout: 
 printf("\n cosine:");
 printf("\n  2-p => Recursion:    %.12f",recursion[0][0]->GetBinContent(1)/pow(10., -2));
 printf("\n  3-p => Recursion:    %.12f",recursion[0][1]->GetBinContent(1)/pow(10., -3));
 printf("\n  4-p => Recursion:    %.12f",recursion[0][2]->GetBinContent(1)/pow(10., -4));
 printf("\n  5-p => Recursion:    %.12f",recursion[0][3]->GetBinContent(1)/pow(10., -5));
 printf("\n  6-p => Recursion:    %.12f",recursion[0][4]->GetBinContent(1)/pow(10., -6));
 printf("\n  7-p => Recursion:    %.12f",recursion[0][5]->GetBinContent(1)/pow(10., -7));
 printf("\n  8-p => Recursion:    %.12f",recursion[0][6]->GetBinContent(1)/pow(10., -8));

 printf("\n\n sinus:");
 printf("\n  2-p => Recursion:    %.12f",recursion[1][0]->GetBinContent(1)/pow(10., -2));
 printf("\n  3-p => Recursion:    %.12f",recursion[1][1]->GetBinContent(1)/pow(10., -3));
 printf("\n  4-p => Recursion:    %.12f",recursion[1][2]->GetBinContent(1)/pow(10., -4));
 printf("\n  5-p => Recursion:    %.12f",recursion[1][3]->GetBinContent(1)/pow(10., -5));
 printf("\n  6-p => Recursion:    %.12f",recursion[1][4]->GetBinContent(1)/pow(10., -6));
 printf("\n  7-p => Recursion:    %.12f",recursion[1][5]->GetBinContent(1)/pow(10., -7));
 printf("\n  8-p => Recursion:    %.12f",recursion[1][6]->GetBinContent(1)/pow(10., -8));//zero?
}

void task()
{
 // Main method.

 Acceptance();
 TrueHarmonic();
 Cosmetics();

 //nu+weight
 printf("\n non-uniform + weight");
 CalculateData(false, true);
 for(int i=2; i<maxHarmonic+2; i++){
  ydata4[i-2] = recursion[0][i-2]->GetBinContent(1)/pow(10., -1*i);
 }

 //nu
 printf("\n non-uniform");
 CalculateData(false, false);
 for(int i=2; i<maxHarmonic+2; i++){
  ydata3[i-2] = recursion[0][i-2]->GetBinContent(1)/pow(10., -1*i);
 }

 //u
 printf("\n uniform");
 CalculateData(true, false);
 for(int i=2; i<maxHarmonic+2; i++){
  ydata2[i-2] = recursion[0][i-2]->GetBinContent(1)/pow(10., -1*i);
 }

 printf("\n\n"); 

 TGraph* g1 = new TGraph(maxHarmonic, xdata, ydata1);
 TGraph* g2 = new TGraph(maxHarmonic, xdata, ydata2);
 TGraph* g3 = new TGraph(maxHarmonic, xdata, ydata3);
 TGraph* g4 = new TGraph(maxHarmonic, xdata, ydata4);

 g1->SetName("g_true");
 g2->SetName("g_u");
 g3->SetName("g_nu");
 g4->SetName("g_nuw");

 g1->Write(g1->GetName(), TObject::kSingleKey + TObject::kOverwrite);
 g2->Write(g2->GetName(), TObject::kSingleKey + TObject::kOverwrite);
 g3->Write(g3->GetName(), TObject::kSingleKey + TObject::kOverwrite);
 g4->Write(g4->GetName(), TObject::kSingleKey + TObject::kOverwrite);

 file->Close();
 delete file;
 file = nullptr;

}




