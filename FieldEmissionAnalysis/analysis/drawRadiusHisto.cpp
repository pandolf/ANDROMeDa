#include "AndCommon.h"
#include "IVScanFN.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TLegend.h"





int main( ) {


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.16);
  style->SetPadRightMargin(0.1);
  style->SetPadTopMargin(0.06);
  style->SetPadBottomMargin(0.17);
  style->SetTitleYOffset(1.15);
  style->cd();

  std::vector<int> colors;
  colors.push_back( kBlack );
  colors.push_back( 46 );
  colors.push_back( 38 );



  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  float xMin = 0.;
  float xMax = 50.;

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., 550. );
  h2_axes->GetYaxis()->SetTitleOffset( 1.3 );
  h2_axes->GetYaxis()->SetMaxDigits(3);
  //h2_axes->GetXaxis()->SetNdivisions(505);
  h2_axes->SetXTitle( "r_{rms} (nm)" );
  h2_axes->SetYTitle( "Counts" );
  h2_axes->Draw("");



  int nBins = 50;

  TH1D* h1_asgrown = new TH1D( "asgrown", "", nBins, xMin, xMax );
  TH1D* h1_mild    = new TH1D( "mild"   , "", nBins, xMin, xMax );
  TH1D* h1_strong  = new TH1D( "strong" , "", nBins, xMin, xMax );

  std::ifstream ifs("data/DataTipMorphologyVACNTs.csv");

  for (std::string line; std::getline(ifs, line); ) {

    std::vector<std::string> words = AndCommon::splitString(line, ",");

    h1_asgrown->Fill( std::atof(words[0].c_str())*1E9 );
    h1_mild   ->Fill( std::atof(words[1].c_str())*1E9 );
    h1_strong ->Fill( std::atof(words[2].c_str())*1E9 );

  }
 
  h1_asgrown->SetFillColor( 46 );
  h1_asgrown->SetLineColor( 46 );
  h1_asgrown->SetLineWidth( 3 );
  h1_asgrown->SetFillStyle( 3006 );

  h1_mild->SetFillColor( 38 );
  h1_mild->SetLineColor( 38 );
  h1_mild->SetLineWidth( 3 );
  h1_mild->SetFillStyle( 3004 );

  h1_strong->SetFillColor( 92 );
  h1_strong->SetLineColor( 92 );
  h1_strong->SetLineWidth( 3 );
  h1_strong->SetFillStyle( 3005 );

  TLegend* legend = new TLegend( 0.52, 0.68, 0.9, 0.9, "T = 3 K, d = 0.53 mm" );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->SetTextFont(42);
  legend->AddEntry(h1_asgrown, "As grown", "F");
  legend->AddEntry(h1_mild, "Mild Ar/O_{2} etch", "F");
  legend->AddEntry(h1_strong, "Strong O_{2} etch", "F");
  legend->Draw("same");


  h1_mild->Draw("same");
  h1_strong->Draw("same");
  h1_asgrown->Draw("same");


  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( "radiusHisto.pdf" );



  c1->Clear();

  float xMin_gamma = 0.;
  float xMax_gamma = 30.;

  TH2D* h2_axes_gamma = new TH2D( "axes_gamma", "", 10, xMin_gamma, xMax_gamma, 10, 0., 35000. );
  h2_axes_gamma->GetYaxis()->SetTitleOffset( 1.3 );
  h2_axes_gamma->GetYaxis()->SetMaxDigits(3);
  //h2_axes_gamma->GetXaxis()->SetNdivisions(505);
  h2_axes_gamma->SetXTitle( "r_{rms} (nm)" );
  h2_axes_gamma->SetYTitle( "Field enhancement factor #gamma");
  h2_axes_gamma->Draw("");


  float g_asgrown = 6892.11;
  float gerr_asgrown = 1725.23;

  float r_asgrown = h1_asgrown->GetMean();
  //float r_asgrown = 18.;
  //float r_asgrown = 20.132;
  float rerr_asgrown = h1_asgrown->GetMeanError();
  //float rerr_asgrown = 6/sqrt(2048.);

  float g_mild = 19285.2;
  float gerr_mild = 1582.83;

  float r_mild = h1_mild->GetMean();
  //float r_mild = 7.875;
  //float r_mild = 7.;
  float rerr_mild = h1_mild->GetMeanError();
  //float rerr_mild = 2/sqrt(2048.);

  float g_strong = 12663.8;
  float gerr_strong = 2202.23;

  float r_strong = h1_strong->GetMean();
  //float r_strong = 9.;
  //float r_strong = 12.988;
  float rerr_strong = h1_strong->GetMeanError();
  //float rerr_strong = 4/sqrt(2048.);

  TGraphErrors* gr_all = new TGraphErrors(0);

  TGraphErrors* gr_asgrown = new TGraphErrors(0);
  gr_asgrown->SetPoint     ( 0, r_asgrown, g_asgrown );
  gr_asgrown->SetPointError( 0, rerr_asgrown, gerr_asgrown );
  gr_all    ->SetPoint     ( 0, r_asgrown, g_asgrown );
  gr_all    ->SetPointError( 0, rerr_asgrown, gerr_asgrown );

  TGraphErrors* gr_mild = new TGraphErrors(0);
  gr_mild->SetPoint     ( 0, r_mild, g_mild );
  gr_mild->SetPointError( 0, rerr_mild, gerr_mild );
  gr_all ->SetPoint     ( 1, r_mild, g_mild );
  gr_all ->SetPointError( 1, rerr_mild, gerr_mild );

  TGraphErrors* gr_strong = new TGraphErrors(0);
  gr_strong->SetPoint     ( 0, r_strong, g_strong );
  gr_strong->SetPointError( 0, rerr_strong, gerr_strong );
  gr_all   ->SetPoint     ( 2, r_strong, g_strong );
  gr_all   ->SetPointError( 2, rerr_strong, gerr_strong );


  gr_asgrown->SetMarkerStyle(20);
  gr_asgrown->SetMarkerSize(1.6);
  gr_asgrown->SetMarkerColor(46);
  gr_asgrown->SetLineColor(46);

  gr_mild->SetMarkerStyle(21);
  gr_mild->SetMarkerSize(1.6);
  gr_mild->SetMarkerColor(38);
  gr_mild->SetLineColor  (38);

  gr_strong->SetMarkerStyle(22);
  gr_strong->SetMarkerSize(1.6);
  gr_strong->SetMarkerColor(92);
  gr_strong->SetLineColor  (92);

  //TF1* f1_line = new TF1( "line", "[0] + [1]*x" );
  ////f1_line->SetLineColor(46);
  //graph->Fit(f1_line);
  //f1_line->Draw("same");

  TF1* f1_invx = new TF1( "invx", "[0]/x", xMin_gamma, xMax_gamma );
  f1_invx->SetLineColor(kGray);
  //f1_invx->SetLineStyle(4);
  f1_invx->SetLineWidth(5);
  gr_all->Fit(f1_invx, "0");
  f1_invx->Draw("same");
  gr_asgrown->Draw("P same");
  gr_mild->Draw("P same");
  gr_strong->Draw("P same");

  TLegend* legend_gamma = new TLegend( 0.5, 0.62, 0.9, 0.9, "T = 3 K, d = 0.53 mm" );
  legend_gamma->SetFillColor(0);
  legend_gamma->SetTextSize(0.035);
  legend_gamma->SetTextFont(42);
  legend_gamma->AddEntry(gr_asgrown, "As grown", "PL");
  legend_gamma->AddEntry(gr_mild   , "Mild Ar/O_{2} etch", "PL");
  legend_gamma->AddEntry(gr_strong , "Strong O_{2} etch", "PL");
  legend_gamma->AddEntry(f1_invx, Form("#gamma = (%.0f #mum) / r_{rms}", f1_invx->GetParameter(0)/1000.), "L");
  //legend_gamma->AddEntry(f1_invx, "#gamma = k / r_{rms}", "L");
  //legend_gamma->AddEntry(f1_invx, "#gamma = #frac{k}{r_{rms}}", "L");
  legend_gamma->Draw("same");


  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( "gamma_vs_r.pdf" );

  return 0;

}




