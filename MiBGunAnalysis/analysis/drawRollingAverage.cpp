#include <iostream>

#include "interface/AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"




void computeRollingAverage( float *pshape,  float *pshapeRA, int nSamples );
void drawPulseShapes( int ev, float *pshape, float *pshapeRA );
float computeAmp( float *pshape );


int main() {


  TFile* file = TFile::Open("Run_APDWL_HV380_Fe55_th6_Data_1_17_2023_Ascii.root");
  TTree* tree = (TTree*)file->Get("tree");

  int ev;
  int nch;
  float amp;
  float pshape[1024];

  tree->SetBranchAddress( "ev" , &ev     );
  tree->SetBranchAddress( "nch"   , &nch    );
  tree->SetBranchAddress( "amp", &amp );
  tree->SetBranchAddress( "pshape", &pshape );


  float pshapeRA[1024];

  TH1D* h1_amp   = new TH1D("amp"  , "", 100, 0., 0.05 );
  TH1D* h1_ampRA = new TH1D("ampRA", "", 100, 0., 0.05 );

  //int nentries = tree->GetEntries();
  int nentries = 10;

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);

    h1_amp->Fill( amp );

    computeRollingAverage( pshape,  pshapeRA, 3 );

    if( ev<10 ) 
      drawPulseShapes( ev, pshape, pshapeRA );

//  h1_ampRA->Fill( computeAmp(pshapeRA) );

  } // for entries

  TFile* outfile = TFile::Open("prova.root", "recreate");
  outfile->cd();

  h1_amp->Write();

  outfile->Close();

  return 0;

} 




void computeRollingAverage( float *pshape,  float *pshapeRA, int nSamples ) {

  float thisSum = 0.;
  float thisN = 0.;

  for( unsigned i=0; i<1024; i++ ) {

    thisSum += pshape[i];
    thisN += 1.;


    for( int j = 1; j < nSamples; j++ ) {

      int iminus = i-j;
      if( iminus >= 0 ) {
        thisSum += pshape[iminus];
        thisN += 1.;
      }

      int iplus = i+j;
      if( iplus < 1024 ) {
        thisSum += pshape[iplus];
        thisN += 1.;
      }

    } // for i < nsamples

    pshapeRA[i] = thisSum/thisN;

  } // for entries

}



float computeAmp( float *pshape ) {

  return 1.;

}



void drawPulseShapes( int ev, float *pshape, float *pshapeRA ) {


  TH1D* h1_pshape   = new TH1D( Form("pshape_%d"  , ev)  , "", 1024, 0., 1024. );
  TH1D* h1_pshapeRA = new TH1D( Form("pshapeRA_%d", ev), "", 1024, 0., 1024. );

  for( unsigned i=0; i<1024; ++i ) {

    h1_pshape  ->SetBinContent( i, pshape  [i] );
    h1_pshapeRA->SetBinContent( i, pshapeRA[i] );

  }


  TCanvas* c1 = new TCanvas( Form("c1_%d", ev), "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( Form("axes_%d", ev), "", 10, 0., 1024., 10, -0.03, 0.06 );
  h2_axes->Draw();

  h1_pshapeRA->SetLineColor(kRed);

  h1_pshape->Draw("same");
  h1_pshapeRA->Draw("same");

  c1->SaveAs( Form("plots/pulseshapes/ev_%d.pdf", ev) );

  delete c1;
  delete h1_pshape;
  delete h1_pshapeRA;
  delete h2_axes;

}
