#include "AndCommon.h"
#include "IVScanFN.h"

#include <iostream>

#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TF1.h"
#include "TLegend.h"
#include "TGaxis.h"




int main( int argc, char* argv[] ) {


  if( argc<2 ) {

    std::cout << "USAGE: ./compareIVScans [batchName]" << std::endl;
    exit( 1);

  }

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();


  std::string batchName( argv[1] );

  std::vector< std::string > scanNames;

  std::string legendTitle;

  float xMin = 0.;
  float xMax = 5E-3;
  float yMin = -20.;
  float yMax = -6.;

  if( batchName == "CNTArO2Etching_N1_20221130" ) {

    scanNames.push_back( "CNTArO2Etching_N1_d3_20221130" );
    scanNames.push_back( "CNTArO2Etching_N1_d4_20221130" );
    scanNames.push_back( "CNTArO2Etching_N1_d5_20221130" );

    xMin = 0.;
    xMax = 5E-3;
    yMin = -19.999;
    yMax = -6.;

    legendTitle = "Mild Ar/O_{2} Etching";

  } else if( batchName == "CNTArO2Etching_AG" ) {

    scanNames.push_back( "CNTArO2Etching_AG_d3_new" );
    scanNames.push_back( "CNTArO2Etching_AG_d4_new" );
    scanNames.push_back( "CNTArO2Etching_AG_d5_new" );

    xMin = 0.2E-3;
    xMax = 1.2E-3;
    yMin = -18.999;
    yMax = -13.;
    
    legendTitle = "As Grown (No Etching)";

  }


  gStyle->SetPadLeftMargin(0.14);
  gStyle->SetTitleYOffset(1.4);
  TGaxis::SetMaxDigits(2);

  TCanvas* c1 = new TCanvas( "c1", "", 800, 800 );
  c1->cd();


  //TH2D* h2_axes = new TH2D( "axes", "", 10, IVScanFN::xMinFN(), IVScanFN::xMaxFN(), 10, IVScanFN::yMinFN(), IVScanFN::yMaxFN() );
  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, yMin, yMax );
  h2_axes->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axes->SetYTitle( IVScanFN::yTitleFN().c_str() );
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.2, 0.35 - scanNames.size()*0.05, 0.6, 0.4, legendTitle.c_str() );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);

  std::vector<int> colors = AndCommon::colors();

  for( unsigned iScan=0; iScan<scanNames.size(); ++iScan ) {

    TFile* file = TFile::Open( Form("plots/%s/graphs.root", scanNames[iScan].c_str()) );
    TGraphErrors* gr_scan = (TGraphErrors*)file->Get( Form("gr_%s", scanNames[iScan].c_str()) );

    IVScanFN ivs( scanNames[iScan] );
    ivs.set_graph( gr_scan );
    ivs.setColor(colors[iScan]);

    TGraphErrors* grFN_scan = ivs.graphFN();

    grFN_scan->SetMarkerSize( 2 );
    grFN_scan->Draw("Psame");

    legend->AddEntry( grFN_scan, Form("d = %.1f mm (#gamma = %.0f #pm %.0f)", ivs.d(), ivs.gamma(), ivs.gamma_err()) );


  } // for

  legend->Draw("same");


  c1->SaveAs( Form("plots/compareFN_%s.pdf", batchName.c_str()) );


  return 0;

}
