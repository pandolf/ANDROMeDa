#include <iostream>
#include <stdlib.h>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"




int main( int argc, char* argv[] ) {

  if( argc!= 2 && argc!= 3 ) {
    std::cout << "USAGE: ./drawPulseShape [fileName] [event (default: first 20 events)]" << std::endl;
    exit(1);
  }

  AndCommon::setStyle();

  std::string fileName(argv[1]);
  int event = -1; 
  if( argc>2 ) event = (atoi(argv[2]));

  std::string dataset = AndCommon::removePathAndSuffix(fileName);
  std::string inFileName = "./data/" + dataset + ".root";

  std::cout << "-> Opening file: " << inFileName << std::endl;
  TFile* file = TFile::Open(inFileName.c_str());
  TTree* tree = (TTree*)file->Get("tree");

  int plotsToBeDrawn = 0;
  int plotsDrawn = 0;

  if( event < 0 ) {
    std::cout << "-> Will draw pulse shapes of first 20 events" << std::endl;
    plotsToBeDrawn = 20;
  } else {
    std::cout << "-> Will check pulse shape of event: " << event << std::endl;
    plotsToBeDrawn = 1;
  }

  std::string plotsDir(Form("plots/%s/pulseshapes/", dataset.c_str()));
  system( Form("mkdir -p %s", plotsDir.c_str()) );

  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
  c1->cd();

  int ev;
  float pshape[1024];

  tree->SetBranchAddress( "ev" , &ev     );
  tree->SetBranchAddress( "pshape", &pshape );

  TH1D* h1 = new TH1D("h1", "", 1024, 0., 1024. );

  int nentries = tree->GetEntries();

  for( unsigned iEntry=0; (iEntry<nentries) && (plotsDrawn<plotsToBeDrawn); ++iEntry ) {

    tree->GetEntry(iEntry);

    bool drawThis = false;
    if( event>=0 && ev==event ) drawThis = true;
    if( event <0 && ev<20  ) drawThis = true;

    if( drawThis ) {

      c1->Clear();

      for( unsigned i=0; i<1024; ++i ) 
        h1->SetBinContent( i+1, pshape[i] );

      h1->Draw();

      c1->SaveAs(Form("%s/pulseShape_ev%d.pdf",plotsDir.c_str(),ev));

      plotsDrawn += 1;

    } //drawThis

  } // for entries


  return 0;

}
