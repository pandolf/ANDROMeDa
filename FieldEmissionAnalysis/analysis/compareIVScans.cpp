#include "AndCommon.h"
#include "IVScan.h"

#include <iostream>

#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TF1.h"
#include "TLegend.h"




int main( int argc, char* argv[] ) {


  if( argc<3 ) {

    std::cout << "USAGE: ./compareIVScans [scan1] [scan2]" << std::endl;
    exit( 1);

  }

  std::string name1( argv[1] );
  std::string name2( argv[2] );

  IVScan ivs1( name1 );
  IVScan ivs2( name2 );

  ivs1.setColor(46);
  ivs2.setColor(kGray+3);

  TGraphErrors* gr_scan1 = ivs1.graphFN();
  TGraphErrors* gr_scan2 = ivs2.graphFN();


  TCanvas* c1 = new TCanvas( "c1", "", 800, 800 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, IVScan::xMinFN(), IVScan::xMaxFN(), 10, IVScan::yMinFN(), IVScan::yMaxFN() );
  h2_axes->SetXTitle( IVScan::xTitleFN().c_str() );
  h2_axes->SetYTitle( IVScan::yTitleFN().c_str() );
  h2_axes->Draw();

  gr_scan1->Draw("Psame");

  gr_scan2->Draw("Psame");

  TLegend* legend = new TLegend( 0.2, 0.2, 0.8, 0.3 );
  legend->SetTextSize(0.038);
  legend->SetFillColor(0);
  legend->AddEntry( gr_scan1, Form("%s (#gamma = %.0f #pm %.0f)", ivs1.name().c_str(), ivs1.gamma(), ivs1.gamma_err()) );
  legend->AddEntry( gr_scan2, Form("%s (#gamma = %.0f #pm %.0f)", ivs2.name().c_str(), ivs2.gamma(), ivs2.gamma_err()) );
  legend->Draw("same");

  c1->SaveAs( Form("plots/compare_%s_vs_%s.pdf", ivs1.name().c_str(), ivs2.name().c_str()) );

  return 0;

}
