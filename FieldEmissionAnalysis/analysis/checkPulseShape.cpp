#include <iostream>

#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TCanvas.h"




int main( int argc, char* argv[] ) {

  if( argc!= 3 ) {
    std::cout << "USAGE: ./checkPulseShape [rootFileName] [event]" << std::endl;
    exit(1);
  }

  std::string fileName(argv[1]);
  int theevent(atoi(argv[2]));

  TH1D* h1 = new TH1D("h1", "", 2502, 0., 2502 );

  TGraphErrors* gr = new TGraphErrors(0);
  gr->SetName("gr");


  TFile* file = TFile::Open(fileName.c_str());
  TTree* tree = (TTree*)file->Get("treeraw");

  std::cout << "-> Opened file " << fileName.c_str() << std::endl;
  std::cout << "-> Will check pulse shape of event: " << theevent << std::endl;

  int ev;
  float pshape[2502];

  tree->SetBranchAddress( "event" , &ev     );
  tree->SetBranchAddress( "pshape", pshape );

  int nentries = tree->GetEntries();
  std::cout << "-> Tree has " << nentries << " entries" << std::endl;

  bool done = false;

  for( unsigned iEntry=0; iEntry<nentries && !done; ++iEntry ) {

    tree->GetEntry(iEntry);

    if( ev==theevent ) {

      for( unsigned i=0; i<2502; ++i ) {
        gr->SetPoint(i, i, pshape[i] );
      }

      done = true;

    }

  } // for entries

  file->Close();


  TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
  c1->cd();
  gr->Draw("APE*");
  c1->SaveAs(Form("pshape_%d.pdf", theevent));

  return 0;

}
