#include "AndCommon.h"
#include "IVScan.h"

#include <iostream>

#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"




TGraphErrors* getFNGraph( const std::string& name );



int main( int argc, char* argv[] ) {


  if( argc<3 ) {

    std::cout << "USAGE: ./compareIVScans [scan1] [scan2]" << std::endl;
    exit( 1);

  }


  std::string name1( argv[1] );
  std::string name2( argv[2] );


  TGraphErrors* gr_scan1 = getFNGraph( name1 );
  TGraphErrors* gr_scan2 = getFNGraph( name2 );


  TCanvas* c1 = new TCanvas( "c1", "", 800, 800 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, IVScan::xMinFN(), IVScan::xMaxFN(), 10, IVScan::yMinFN(), IVScan::yMaxFN() );
  h2_axes->SetXTitle( IVScan::xTitleFN().c_str() );
  h2_axes->SetYTitle( IVScan::yTitleFN().c_str() );
  h2_axes->Draw();


  gr_scan1->SetMarkerColor( 


  return 0;

}




TGraphErrors* getFNGraph( const std::string& name ) {

  TFile* file = TFile::Open( Form("plots/%s/graphs.root", name.c_str()), "READ" );

  if( file==0 ) {
    std::cout << "[getFNGraph] Couldn't find graph for IVScan: " << name << std::endl;
    exit(2);
  }

  TGraphErrors* graph = new TGraphErrors(0);
  graph = (TGraphErrors*)file->Get("gr_fn");
  graph->SetName( Form("gr_fn_%s", name.c_str()) );

  return graph;

}
