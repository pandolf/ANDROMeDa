#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"



void computeRollingAverage( float *pshape,  float *pshapeRA, int nSamples );
float computeBaseline( float *pshape, int nBaseline );
float computeAmp   ( float *pshape, float base );
float computeNegAmp( float *pshape, float base );
float computeCharge( float *pshape, float base );
int computeNcross( float *pshape, float base, float thresh );


int main( int argc, char* argv[] ) {


  std::string fileName;

  if( argc>1 ) {

    fileName = std::string(argv[1]);

  } else {

    std::cout << "USAGE: ./makeTreeLite [fileName]" << std::endl;
    exit(1);

  }

  int nSamplesRA = 20;
  int nSamplesBase = 16;

  std::string dataset = AndCommon::removePathAndSuffix(fileName);
  std::string inFileName = "./data/" + dataset + ".root";

  std::cout << "-> Opening file: " << inFileName << std::endl;

  TFile* file = TFile::Open( inFileName.c_str() );
  TTree* tree = (TTree*)file->Get("tree");

  int ev;
  int nch;
  float amp;
  float charge;
  float pshape[1024];

  tree->SetBranchAddress( "ev"    , &ev     );
  tree->SetBranchAddress( "nch"   , &nch    );
  tree->SetBranchAddress( "amp"   , &amp    );
  tree->SetBranchAddress( "charge", &charge );
  tree->SetBranchAddress( "pshape", &pshape );
  
  float pshapeRA[1024];

  std::string outfileName(Form("lite_%s.root", dataset.c_str()));

  TFile* outfile = TFile::Open( outfileName.c_str(), "RECREATE");
  TTree* treeLite = new TTree("treeLite", "");

  treeLite->Branch( "ev", &ev, "ev/I" );
  treeLite->Branch( "amp", &amp, "amp/F" );
  treeLite->Branch( "charge", &charge, "charge/F" );

  float baseRA;
  treeLite->Branch( "baseRA", &baseRA, "baseRA/F" );
  float ampRA;
  treeLite->Branch( "ampRA", &ampRA, "ampRA/F" );
  float ampNegRA;
  treeLite->Branch( "ampNegRA", &ampNegRA, "ampNegRA/F" );
  float chargeRA;
  treeLite->Branch( "chargeRA", &chargeRA, "chargeRA/F" );
  float nCross5;
  treeLite->Branch( "nCross5", &nCross5, "nCross5/I" );

  
  int nentries = tree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    if( iEntry % 5000 == 0 ) std::cout << "Event: " << iEntry << " / " << nentries << std::endl;

    tree->GetEntry( iEntry );

    computeRollingAverage( pshape, pshapeRA, nSamplesRA );

    baseRA = computeBaseline( pshapeRA, nSamplesBase );

    ampRA = computeAmp( pshapeRA, baseRA );

    ampNegRA = computeNegAmp( pshapeRA, baseRA );

    chargeRA = computeCharge( pshapeRA, baseRA );

    nCross5 = computeNcross( pshapeRA, baseRA, 0.005 );

    treeLite->Fill();

  } // for entries

  outfile->cd();

  treeLite->Write();

  std::cout << "-> You can find your lite tree in: " << outfileName << std::endl;

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



float computeAmp( float *pshape, float base ) {

  float amp = 0.;

  for( unsigned i=0; i<1024; i++ )
    if( fabs(pshape[i]-base) > fabs(amp) ) amp = (pshape[i]-base);

  return amp;

}


float computeNegAmp( float *pshape, float base ) {

  float amp = 0.;

  for( unsigned i=0; i<1024; i++ )
    if( (pshape[i]-base) < amp ) amp = (pshape[i]-base);

  return amp;

}


float computeCharge( float *pshape, float base ) {

  float charge = 0.;

  for( unsigned i=0; i<1024; i++ )
    charge = charge + (pshape[i]-base);

  return charge;

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

    

int computeNcross( float *pshape, float base, float thresh ) {
    
  bool plusReady = true;
  bool minusReady = true;

  int nCross = 0;

  for( unsigned i=0; i<1024; i++ ) {

    if( (pshape[i]-base)>thresh && plusReady ) {
      nCross += 1;
      minusReady = true;
      plusReady = false;
    }

    if( (pshape[i]-base)<-thresh && minusReady ) {
      nCross += 1;
      minusReady = false;
      plusReady = true;
    }

  } // for samples

  return nCross;

}
