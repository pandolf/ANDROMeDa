#include <iostream>
#include <stdlib.h>

#include "interface/AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"




void computeRollingAverage( float *pshape,  float *pshapeRA, int nSamples );
void drawPulseShapes( const std::string& outdir, int ev, float *pshape, float *pshapeRA, int nSamples );
float computeAmp( float *pshape );
float computeBaseline( float *pshape, int nSamples );




int main( int argc, char* argv[] ) {


  AndCommon::setStyle();

  std::string fileName(argv[1]);
  int event_to_pick = -1;
  if( argc>2 ) event_to_pick = atoi(argv[2]);

  //float ampMax = 0.03;
  float ampMax = 0.012;
  int nSamples = 20;


  std::string dataset = AndCommon::removePathAndSuffix(fileName);
  std::string inFileName = "./data/" + dataset + ".root";

  std::cout << "-> Opening file: " << inFileName << std::endl;
  TFile* file = TFile::Open(inFileName.c_str());
  TTree* tree = (TTree*)file->Get("tree");

  int ev;
  float amp;
  float pshape[1024];

  tree->SetBranchAddress( "ev" , &ev     );
  tree->SetBranchAddress( "amp", &amp );
  tree->SetBranchAddress( "pshape", &pshape );


  float pshapeRA[1024];

  TH1D* h1_amp   = new TH1D("amp"  , "", 100, -0.01, ampMax );
  TH1D* h1_ampRA = new TH1D("ampRA", "", 100, -0.01, ampMax );

  std::string outdir( Form("plots/%s/pulseshapesRA/", dataset.c_str()) );
  system( Form("mkdir -p %s", outdir.c_str()) );


  int nentries = tree->GetEntries();
  //int nentries = 10;

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);

    //h1_amp->Fill( computeAmp(pshape) );
    h1_amp->Fill( amp );

    computeRollingAverage( pshape, pshapeRA, nSamples );

    if( ( (event_to_pick < 0) && (ev<20) ) || ( (event_to_pick >= 0) && (ev==event_to_pick) ) ) 
      drawPulseShapes( outdir, ev, pshape, pshapeRA, nSamples );

    h1_ampRA->Fill( computeAmp(pshapeRA) );

  } // for entries


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, -0.01, ampMax, 10, 0., 0.1 );
  h2_axes->SetXTitle( "Amplitude [V]" );
  h2_axes->SetYTitle( "Normalized to Unity");
  h2_axes->Draw();

  h1_amp->SetLineColor(kGray+3);
  h1_amp->SetLineWidth(2);

  h1_ampRA->SetLineColor(46);
  h1_ampRA->SetLineWidth(2);

  h1_amp  ->DrawNormalized("same");
  h1_ampRA->DrawNormalized("same");

  TLegend* legend = new TLegend( 0.2, 0.75, 0.7, 0.9, "^{55}Fe" );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->AddEntry(h1_amp, "Raw amplitude", "L");
  legend->AddEntry(h1_ampRA, Form("Rolling average (#pm%d samples)", nSamples), "L");
  legend->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs( "provaRA.pdf" );

  return 0;

} 




void computeRollingAverage( float *pshape,  float *pshapeRA, int nSamples ) {

  float thisSum = 0.;
  float thisN = 0.;

  for( unsigned i=0; i<1024; i++ ) {

    thisSum = pshape[i];
    thisN = 1.;


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

  float base = computeBaseline( pshape, 40 );

  float amp = 0.;

  for( unsigned i=0; i<1024; i++ )
    if( fabs(pshape[i]-base) > fabs(amp) ) amp = pshape[i]-base;

  return amp;

}


float computeBaseline( float *pshape, int nSamples ) {

  float sum = 0;
  float n = 0.;

  for( unsigned i=0; i<nSamples; ++i ) {

    sum += pshape[i];
    n += 1.;

  }

  return sum/n;

}


void drawPulseShapes( const std::string& outdir, int ev, float *pshape, float *pshapeRA, int nSamples ) {


  TH1D* h1_pshape   = new TH1D( Form("pshape_%d"  , ev)  , "", 1024, 0., 1024. );
  TH1D* h1_pshapeRA = new TH1D( Form("pshapeRA_%d", ev), "", 1024, 0., 1024. );

  for( unsigned i=0; i<1024; ++i ) {

    h1_pshape  ->SetBinContent( i, 1000.*pshape  [i] );
    h1_pshapeRA->SetBinContent( i, 1000.*pshapeRA[i] );

  }


  TCanvas* c1 = new TCanvas( Form("c1_%d", ev), "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( Form("axes_%d", ev), "", 10, 0., 1024., 10, -5., 10. );
  h2_axes->SetXTitle("Sample number");
  h2_axes->SetYTitle("Amplitude (mV)");
  h2_axes->Draw();

  h1_pshapeRA->SetLineColor(kRed);
  h1_pshapeRA->SetLineWidth(2);

  TLegend* legend = new TLegend( 0.5, 0.7, 0.9, 0.9 ); //, "Typical ^{55}Fe Event" );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->AddEntry(h1_pshape, "Raw amplitude", "L");
  legend->AddEntry(h1_pshapeRA, Form("Rolling average"), "L");
  legend->AddEntry(h1_pshapeRA, Form("(#pm%d samples)", nSamples), "");
  legend->Draw("same");

  h1_pshape->Draw("same");
  h1_pshapeRA->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs( Form("%s/ev_%d.pdf", outdir.c_str(), ev) );

  delete c1;
  delete h1_pshape;
  delete h1_pshapeRA;
  delete h2_axes;

}
