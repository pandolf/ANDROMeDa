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
  float xMax = 4E-3; // corresponds to 1/250 V-1
  float yMin = -20.;
  float yMax = 5.;

  if( batchName == "CNTArO2Etching_AG_old" ) {

    scanNames.push_back( "CNTArO2Etching_AG_d3_new" );
    scanNames.push_back( "CNTArO2Etching_AG_d4_new" );
    scanNames.push_back( "CNTArO2Etching_AG_d5_new" );

    xMin = 0.2E-3;
    xMax = 1.5E-3;
    yMin = -18.999;
    yMax = -13.;
    
    legendTitle = "As Grown (No Etching)";

  } else if( batchName == "CNTArO2Etching_AG_20221220" || batchName == "CNTArO2Etching_AG" ) {

    scanNames.push_back( "CNTArO2Etching_AG_d3_20221220" );
    scanNames.push_back( "CNTArO2Etching_AG_d4_20221220" );
    scanNames.push_back( "CNTArO2Etching_AG_d5_20221220" );

    xMin = 0.2E-3;
    xMax = 1.5E-3;
    yMin = -7.499;
    yMax = 3.;
    
    legendTitle = "As Grown (No Etching)";


  } else if( batchName == "CNTArO2Etching_AG2" ) {

    scanNames.push_back( "CNTArO2Etching_AG2_d3_20221216" );
    scanNames.push_back( "CNTArO2Etching_AG2_d4_20221216" );
    scanNames.push_back( "CNTArO2Etching_AG2_d5_20221216" );

    //xMin = 0.2E-3;
    //xMax = 1.2E-3;
    //yMin = -18.999;
    //yMax = -13.;
    
    legendTitle = "As Grown (No Etching)";


  } else if( batchName == "CNTArO2Etching_N1_old" ) {

    scanNames.push_back( "CNTArO2Etching_N1_d3_20221130" );
    scanNames.push_back( "CNTArO2Etching_N1_d4_20221130" );
    scanNames.push_back( "CNTArO2Etching_N1_d5_20221130" );

    xMin = 0.;
    xMax = 5E-3;
    yMin = -19.999;
    yMax = -6.;

    legendTitle = "Ar/O_{2} Etching N.1";

  } else if( batchName == "CNTArO2Etching_N1" ) {

    scanNames.push_back( "CNTArO2Etching_N1_d3_20221221" );
    scanNames.push_back( "CNTArO2Etching_N1_d4_20221221" );
    scanNames.push_back( "CNTArO2Etching_N1_d5_20221221" );

    xMin = 0.3E-3;
    xMax = 1.4E-3;
    yMin = -7.999;
    yMax = 4.999;

    legendTitle = "Ar/O_{2} Etching N.1";

  } else if( batchName == "CNTArO2Etching_N6" ) {

    scanNames.push_back( "CNTArO2Etching_N6_d3_20221219_2" );
    scanNames.push_back( "CNTArO2Etching_N6_d4_20221219_3" );
    scanNames.push_back( "CNTArO2Etching_N6_d5_20221219_2" );

    xMin = 1.2E-3;
    xMax = 4.8E-3;
    yMin = -19.999;
    yMax = 4.999;
    
    legendTitle = "Ar/O_{2} Etching N.6";

  } else if( batchName == "CNTArO2Etching_N6_old" ) {

    scanNames.push_back( "CNTArO2Etching_N6_d3_20221219" );
    scanNames.push_back( "CNTArO2Etching_N6_d4_20221219" );
    scanNames.push_back( "CNTArO2Etching_N6_d5_20221219" );

    xMin = 1.2E-3;
    xMax = 4.2E-3;
    yMin = -19.999;
    yMax = 5.;
    
    legendTitle = "Ar/O_{2} Etching N.6";

  } else if( batchName == "CNTetchedOLD_N1new" ) {

    scanNames.push_back( "CNTetchedOLD_N1new_d4_20230517_drain" );
    scanNames.push_back( "CNTetchedOLD_N1new_d5_20230517_drain" );

    xMin = 1.2E-3;
    xMax = 4.2E-3;
    yMin = -9.999;
    yMax = 5.;
    
    legendTitle = "Ar/O_{2} Etching N.1";

  } else {

    scanNames.push_back( std::string(argv[1]));
    if( argc>2 ) scanNames.push_back( std::string(argv[2]));
    if( argc>3 ) scanNames.push_back( std::string(argv[3]));

    batchName = scanNames[0];

    for( unsigned i=1; i<scanNames.size(); ++i )
      batchName = batchName + "_vs_" + scanNames[i];

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
