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

    std::cout << "USAGE: ./compareIVScans [scan1] [scan2] [scan3=0]" << std::endl;
    exit( 1);

  }

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();

  std::string name1( argv[1] );
  std::string name2( argv[2] );
  std::string name3("");
  if( argc>3 ) name3 = std::string(argv[3]);

  IVScan* ivs1 = new IVScan( name1 );
  IVScan* ivs2 = new IVScan( name2 );
  IVScan* ivs3;
  if( argc>3 ) ivs3 = new IVScan( name3 );

  int color1 = 46;
  int color2 = kGray+3;
  int color3 = 38;

  ivs1->setColor(color1);
  ivs2->setColor(color2);
  if( argc>3 ) ivs3->setColor(color3);

  TGraphErrors* gr_scan1 = ivs1->graphFN();
  TGraphErrors* gr_scan2 = ivs2->graphFN();
  TGraphErrors* gr_scan3;
  if( argc>3 ) gr_scan3 = ivs3->graphFN();


  TCanvas* c1 = new TCanvas( "c1", "", 800, 800 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, IVScan::xMinFN(), IVScan::xMaxFN(), 10, IVScan::yMinFN(), IVScan::yMaxFN() );
  h2_axes->SetXTitle( IVScan::xTitleFN().c_str() );
  h2_axes->SetYTitle( IVScan::yTitleFN().c_str() );
  h2_axes->Draw();

  gr_scan1->Draw("Psame");
  gr_scan2->Draw("Psame");
  if( argc>3 ) gr_scan3->Draw("Psame");

  TLegend* legend1 = new TLegend( 0.15, 0.25 - (3-argc)*0.05, 0.8, 0.3 - (3-argc)*0.05 );
  legend1->SetTextSize(0.028);
  legend1->SetFillColor(0);
  legend1->SetTextColor(color1);
  legend1->AddEntry( gr_scan1, Form("%s (#gamma = %.0f #pm %.0f)", ivs1->name().c_str(), ivs1->gamma(), ivs1->gamma_err()) );
  legend1->Draw("same");

  TLegend* legend2 = new TLegend( 0.15, 0.2 - (3-argc)*0.05, 0.8, 0.25 - (3-argc)*0.05 );
  legend2->SetTextSize(0.028);
  legend2->SetFillColor(0);
  legend2->SetTextColor(color2);
  legend2->AddEntry( gr_scan2, Form("%s (#gamma = %.0f #pm %.0f)", ivs2->name().c_str(), ivs2->gamma(), ivs2->gamma_err()) );
  legend2->Draw("same");

  if( argc>3 ) {

    TLegend* legend3 = new TLegend( 0.15, 0.2, 0.8, 0.25 );
    legend3->SetTextSize(0.028);
    legend3->SetFillColor(0);
    legend3->SetTextColor(color3);
    legend3->AddEntry( gr_scan3, Form("%s (#gamma = %.0f #pm %.0f)", ivs3->name().c_str(), ivs3->gamma(), ivs3->gamma_err()) );
    legend3->Draw("same");

  }

  //TLegend* legend = new TLegend( 0.15, 0.2, 0.8, 0.3 );
  //legend->SetTextSize(0.028);
  //legend->SetFillColor(0);
  //legend->SetTextColor(46);
  //legend->AddEntry( gr_scan1, Form("%s (#gamma = %.0f #pm %.0f)", ivs1.name().c_str(), ivs1.gamma(), ivs1.gamma_err()) );
  //legend->SetTextColor(kGray+3);
  //legend->AddEntry( gr_scan2, Form("%s (#gamma = %.0f #pm %.0f)", ivs2.name().c_str(), ivs2.gamma(), ivs2.gamma_err()) );
  //legend->Draw("same");

  if( argc>3 )
    c1->SaveAs( Form("plots/compare_%s_vs_%s_vs_%s.pdf", ivs1->name().c_str(), ivs2->name().c_str(), ivs3->name().c_str()) );
  else
    c1->SaveAs( Form("plots/compare_%s_vs_%s.pdf", ivs1->name().c_str(), ivs2->name().c_str()) );


  return 0;

}
