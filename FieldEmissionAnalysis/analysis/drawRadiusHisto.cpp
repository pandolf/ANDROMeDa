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




void fillHistoFromFile( const std::string& fileName, TH1D* histo, float& mean, float& mean_err );
void  getGammaFromFile( float& gamma, float& gamma_err, const std::string& file );


int main( ) {


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.16);
  style->SetPadRightMargin(0.1);
  style->SetPadTopMargin(0.06);
  style->SetPadBottomMargin(0.17);
  style->SetTitleYOffset(1.15);
  style->cd();

  std::vector<int> colors;
  colors.push_back( 46 );
  colors.push_back( 92 );
  colors.push_back( 38 );


  IVScan ivs_asgrown("CNTArO2Etching_AsGrown_INRiM_MICa_3Kplate_d0p5_IvsV_anode_90to150V_20231213_2_sweepA");
  IVScan ivs_mild("CNTArO2Etching_N1new_INRiM_MICb_3Kplate_d0p5_IvsV_drain_0to88V_20231130_sweepR");
  IVScan ivs_strong("CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR");
  IVScan ivs_pecvd("PECVD_FE_INRIM_001_MICa_3Kplate_d0p5_IvsV_anode_from205Vto265V_10032025_post_conditioning_sweepR");


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  float xMin = 0.;
  float xMax = 50.;

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., 4400. );
  h2_axes->GetYaxis()->SetTitleOffset( 1.3 );
  h2_axes->GetYaxis()->SetMaxDigits(3);
  //h2_axes->GetXaxis()->SetNdivisions(505);
  h2_axes->SetXTitle( "r_{c} (nm)" );
  h2_axes->SetYTitle( "Counts" );
  h2_axes->Draw("");



  int nBins = 1000;
  float xMin_histo = 0.;
  float xMax_histo = 1000.;

  TH1D* h1_asgrown = new TH1D( "asgrown", "", nBins, xMin_histo, xMax_histo );
  TH1D* h1_mild    = new TH1D( "mild"   , "", nBins, xMin_histo, xMax_histo );
  TH1D* h1_strong  = new TH1D( "strong" , "", nBins, xMin_histo, xMax_histo );
  TH1D* h1_pecvd   = new TH1D( "pecvd"  , "", nBins, xMin_histo, xMax_histo );

  float r_asgrown;
  float r_mild   ;
  float r_strong ;
  float r_pecvd  ;

  float rerr_asgrown;
  float rerr_mild   ;
  float rerr_strong ;
  float rerr_pecvd  ;

  fillHistoFromFile( "rc_As.csv", h1_asgrown, r_asgrown, rerr_asgrown );
  fillHistoFromFile( "rc_M.csv" , h1_mild   , r_mild   , rerr_mild    );
  fillHistoFromFile( "rc_S.csv" , h1_strong , r_strong , rerr_strong  );
  fillHistoFromFile( "rc_PE.csv", h1_pecvd  , r_pecvd  , rerr_pecvd   );

  //rerr_asgrown = sqrt( rerr_asgrown*rerr_asgrown + 2.*2. ); 
  //rerr_mild    = sqrt( rerr_mild   *rerr_mild    + 2.*2. ); 
  //rerr_strong  = sqrt( rerr_strong *rerr_strong  + 2.*2. ); 
  //rerr_pecvd   = sqrt( rerr_pecvd  *rerr_pecvd   + 2.*2. ); 

 
  h1_asgrown->SetFillColor( colors[0] );
  h1_asgrown->SetLineColor( colors[0] );
  h1_asgrown->SetLineWidth( 3 );
  h1_asgrown->SetFillStyle( 3006 );

  h1_mild->SetFillColor( colors[1] );
  h1_mild->SetLineColor( colors[1] );
  h1_mild->SetLineWidth( 3 );
  h1_mild->SetFillStyle( 3004 );

  h1_strong->SetFillColor( colors[2] );
  h1_strong->SetLineColor( colors[2] );
  h1_strong->SetLineWidth( 3 );
  h1_strong->SetFillStyle( 3005 );

  h1_pecvd->SetFillColor( kGray+3 );
  h1_pecvd->SetLineColor( kGray+3 );
  h1_pecvd->SetLineWidth( 3 );
  h1_pecvd->SetFillStyle( 0 );

  TPaveText* label = new TPaveText( 0.46, 0.8, 0.9, 0.9, "brNDC" );
  label->SetFillColor(0);
  label->SetTextAlign(12);
  label->SetTextSize(0.038);
  label->AddText( "T = 2.9 K" );
  //label->AddText( "0.56 #leq d #leq 0.62 mm" );
  //label->Draw("same");


  //TLegend* legend = new TLegend( 0.47, 0.57, 0.9, 0.79 );
  TLegend* legend = new TLegend( 0.54, 0.67, 0.9, 0.86 );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->SetTextFont(42);
  legend->AddEntry(h1_asgrown, "As grown", "F");
  legend->AddEntry(h1_mild, "Mild etch", "F");
  legend->AddEntry(h1_strong, "Strong etch", "F");
  //legend->AddEntry(h1_pecvd, "PE-CVD", "F");
  legend->Draw("same");


  h1_mild->Draw("same");
  h1_strong->Draw("same");
  h1_asgrown->Draw("same");
  //h1_pecvd->Draw("same");


  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( "radiusHisto.pdf" );

  c1->Clear();

  TH2D* h2_axes_norm = new TH2D( "axes_norm", "", 10, xMin, xMax, 10, 0., 0.1 );
  h2_axes_norm->GetYaxis()->SetTitleOffset( 1.6 );
  h2_axes_norm->GetYaxis()->SetMaxDigits(3);
  h2_axes_norm->SetXTitle( "r_{rms} (nm)" );
  h2_axes_norm->SetYTitle( "Normalized to unity" );
  h2_axes_norm->Draw("");

  //label->Draw("same");

  h1_mild->DrawNormalized("same");
  h1_strong->DrawNormalized("same");
  h1_asgrown->DrawNormalized("same");
  //h1_pecvd->DrawNormalized("same");

  legend->Draw("same");

  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( "radiusHistoNorm.pdf" ); 


  c1->Clear();
  //c1->SetLogy();

  TH2D* h2_axes_long = new TH2D( "axes_long", "", 10, xMin, 1000., 10, 0.001, 20000.);
  h2_axes_long->GetYaxis()->SetTitleOffset( 1.6 );
  h2_axes_long->GetYaxis()->SetMaxDigits(3);
  h2_axes_long->SetXTitle( "r_{rms} (nm)" );
  h2_axes_long->SetYTitle( "Normalized to Unity" );
  h2_axes_long->Draw("");

  label->Draw("same");

  h1_mild->Draw("same");
  h1_strong->Draw("same");
  h1_asgrown->Draw("same");
  //h1_pecvd->Draw("same");

  legend->Draw("same");

  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( "radiusHisto_long.pdf" ); 



  c1->Clear();

  float xMin_gamma = 0.;
  float xMax_gamma = 45.;
  float yMax_gamma = 23590.;

  TH2D* h2_axes_gamma = new TH2D( "axes_gamma", "", 10, xMin_gamma, xMax_gamma, 10, 0., yMax_gamma );
  h2_axes_gamma->GetYaxis()->SetTitleOffset( 1.3 );
  h2_axes_gamma->GetYaxis()->SetMaxDigits(3);
  //h2_axes_gamma->GetXaxis()->SetNdivisions(505);
  h2_axes_gamma->SetXTitle( "r_{c} (nm)" );
  h2_axes_gamma->SetYTitle( "Field enhancement factor #beta");
  h2_axes_gamma->Draw("");

//std::cout << std::endl << "Mean radius of curvature: " << std::endl;

  float g_asgrown, gerr_asgrown;
  getGammaFromFile( g_asgrown, gerr_asgrown, ivs_asgrown.name() );

//std::cout << "As grown: " << r_asgrown << " +/- " << rerr_asgrown << " nm" << std::endl;

  float g_mild, gerr_mild;
  getGammaFromFile( g_mild, gerr_mild, ivs_mild.name() );

//std::cout << "Mild: " << r_mild << " +/- " << rerr_mild << " nm" << std::endl;

  float g_strong, gerr_strong;
  getGammaFromFile( g_strong, gerr_strong, ivs_strong.name() );

//std::cout << "Strong: " << r_strong << " +/- " << rerr_strong << " nm" << std::endl;

  float g_pecvd, gerr_pecvd;
  getGammaFromFile( g_pecvd, gerr_pecvd, ivs_pecvd.name() );

//std::cout << "PE-CVD: " << r_pecvd << " +/- " << rerr_pecvd << " nm" << std::endl;

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

  TGraphErrors* gr_pecvd = new TGraphErrors(0);
  gr_pecvd->SetPoint     ( 0, r_pecvd, g_pecvd );
  gr_pecvd->SetPointError( 0, rerr_pecvd, gerr_pecvd );
  //gr_all  ->SetPoint     ( 3, r_pecvd, g_pecvd );
  //gr_all  ->SetPointError( 3, rerr_pecvd, gerr_pecvd );


  gr_asgrown->SetMarkerStyle(20);
  gr_asgrown->SetMarkerSize(1.6);
  gr_asgrown->SetMarkerColor(colors[0]);
  gr_asgrown->SetLineColor  (colors[0]);

  gr_mild->SetMarkerStyle(21);
  gr_mild->SetMarkerSize(1.6);
  gr_mild->SetMarkerColor(colors[1]);
  gr_mild->SetLineColor  (colors[1]);

  gr_strong->SetMarkerStyle(22);
  gr_strong->SetMarkerSize(1.6);
  gr_strong->SetMarkerColor(colors[2]);
  gr_strong->SetLineColor  (colors[2]);

  gr_pecvd->SetMarkerStyle(23);
  gr_pecvd->SetMarkerSize(1.6);
  gr_pecvd->SetMarkerColor(kGray+3);
  gr_pecvd->SetLineColor  (kGray+3);

  TF1* f1_invx = new TF1( "invx", "[0]/x", xMin_gamma, xMax_gamma );
  f1_invx->SetLineColor(kGray);
  //f1_invx->SetLineStyle(4);
  f1_invx->SetLineWidth(5);
  gr_all->Fit(f1_invx, "0");

  //TF1* f1_invx = new TF1( "invx", "[0]/(x-[1])+[2]", r_mild, xMax_gamma );
  //f1_invx->SetLineColor(kGray);
  ////f1_invx->SetLineStyle(4);
  //f1_invx->SetLineWidth(5);
  //gr_all->Fit(f1_invx, "0");

  f1_invx->Draw("same");
  gr_asgrown->Draw("P same");
  gr_mild->Draw("P same");
  gr_strong->Draw("P same");
  //gr_pecvd->Draw("P same");

  //label->Draw("same");


  TLegend* legend_gamma = new TLegend( 0.49, 0.68, 0.9, 0.9 );
  //TLegend* legend_gamma = new TLegend( 0.47, 0.57, 0.9, 0.79 );
  //TLegend* legend_gamma = new TLegend( 0.47, 0.62, 0.9, 0.9, "T = 2.8 K, d = 0.53 mm" );
  legend_gamma->SetFillColor(0);
  legend_gamma->SetTextSize(0.035);
  legend_gamma->SetTextFont(42);
  legend_gamma->AddEntry(gr_asgrown, "As grown", "PL");
  legend_gamma->AddEntry(gr_mild   , "Mild etch", "PL");
  legend_gamma->AddEntry(gr_strong , "Strong etch", "PL");
  //legend_gamma->AddEntry(gr_pecvd , "PE-CVD", "PL");
  legend_gamma->AddEntry(f1_invx, Form("#beta = (%.0f #mum) / r_{c}", f1_invx->GetParameter(0)/1000.), "L");
  //legend_gamma->AddEntry(f1_invx, "#gamma = k / r_{rms}", "L");
  //legend_gamma->AddEntry(f1_invx, "#gamma = #frac{k}{r_{rms}}", "L");
  legend_gamma->Draw("same");


  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( "gamma_vs_r.pdf" );


  c1->Clear();

  TH2D* h2_axes_gamma_vs_hor = new TH2D( "gamma_vs_hor", "", 10, 0., 30000., 10, 0., yMax_gamma );
  h2_axes_gamma_vs_hor->GetYaxis()->SetTitleOffset( 1.3 );
  h2_axes_gamma_vs_hor->GetYaxis()->SetMaxDigits(3);
  h2_axes_gamma_vs_hor->SetXTitle( "h_{cnt} / r_{c}" );
  h2_axes_gamma_vs_hor->SetYTitle( "Field enhancement factor #beta");
  h2_axes_gamma_vs_hor->Draw("");

  TGraphErrors* gr_gamma_vs_hor_asgrown = new TGraphErrors(0);
  gr_gamma_vs_hor_asgrown->SetMarkerStyle(20);
  gr_gamma_vs_hor_asgrown->SetMarkerSize(1.6);
  gr_gamma_vs_hor_asgrown->SetMarkerColor(colors[0]);
  gr_gamma_vs_hor_asgrown->SetLineColor  (colors[0]);
  gr_gamma_vs_hor_asgrown->SetPoint( 0, ivs_asgrown.h()*1000000./r_asgrown, g_asgrown );
  gr_gamma_vs_hor_asgrown->Draw("Psame");

  TGraphErrors* gr_gamma_vs_hor_mild = new TGraphErrors(0);
  gr_gamma_vs_hor_mild->SetMarkerStyle(21);
  gr_gamma_vs_hor_mild->SetMarkerSize(1.6);
  gr_gamma_vs_hor_mild->SetMarkerColor(colors[1]);
  gr_gamma_vs_hor_mild->SetLineColor  (colors[1]);
  gr_gamma_vs_hor_mild->SetPoint( 0, ivs_mild.h()*1000000./r_mild, g_mild );
  gr_gamma_vs_hor_mild->Draw("Psame");

  TGraphErrors* gr_gamma_vs_hor_strong = new TGraphErrors(0);
  gr_gamma_vs_hor_strong->SetMarkerStyle(22);
  gr_gamma_vs_hor_strong->SetMarkerSize(1.6);
  gr_gamma_vs_hor_strong->SetMarkerColor(colors[2]);
  gr_gamma_vs_hor_strong->SetLineColor  (colors[2]);
  gr_gamma_vs_hor_strong->SetPoint( 0, ivs_strong.h()*1000000./r_strong, g_strong );
  gr_gamma_vs_hor_strong->Draw("Psame");

  TGraphErrors* gr_gamma_vs_hor_pecvd = new TGraphErrors(0);
  gr_gamma_vs_hor_pecvd->SetMarkerStyle(23);
  gr_gamma_vs_hor_pecvd->SetMarkerSize(1.6);
  gr_gamma_vs_hor_pecvd->SetMarkerColor(kGray+3);
  gr_gamma_vs_hor_pecvd->SetLineColor(kGray+3);
  gr_gamma_vs_hor_pecvd->SetPoint( 0, ivs_pecvd.h()*1000000./r_pecvd, g_pecvd );
  gr_gamma_vs_hor_pecvd->Draw("Psame");

  TLegend* legend_gamma2 = new TLegend( 0.47, 0.65, 0.9, 0.9 );
  legend_gamma2->SetFillColor(0);
  legend_gamma2->SetTextSize(0.035);
  legend_gamma2->SetTextFont(42);
  legend_gamma2->AddEntry(gr_asgrown, "As grown", "PL");
  legend_gamma2->AddEntry(gr_mild   , "Mild Ar/O_{2} etch", "PL");
  legend_gamma2->AddEntry(gr_strong , "Strong O_{2} etch", "PL");
  legend_gamma2->AddEntry(gr_pecvd , "PE-CVD", "PL");
  legend_gamma2->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("gamma_vs_hor.pdf");

  c1->Clear();

  TH1D* h1_prom_asgrown = new TH1D("hprom_asgrown", "", 100, 0., 1000.);
  TH1D* h1_prom_mild    = new TH1D("hprom_mild"   , "", 100, 0., 1000.);
  TH1D* h1_prom_strong  = new TH1D("hprom_strong" , "", 100, 0., 1000.);
  TH1D* h1_prom_pecvd   = new TH1D("hprom_pecvd"  , "", 100, 0., 1000.);

  float prom_asgrown, prom_err_asgrown;
  fillHistoFromFile( "prom_As.csv", h1_prom_asgrown, prom_asgrown, prom_err_asgrown );
  float prom_mild, prom_err_mild;
  fillHistoFromFile( "prom_As.csv", h1_prom_mild, prom_mild, prom_err_mild );
  float prom_strong, prom_err_strong;
  fillHistoFromFile( "prom_As.csv", h1_prom_strong, prom_strong, prom_err_strong );
  float prom_pecvd, prom_err_pecvd;
  fillHistoFromFile( "prom_As.csv", h1_prom_pecvd, prom_pecvd, prom_err_pecvd );

  TH2D* h2_axes_gamma_vs_por = new TH2D( "gamma_vs_por", "", 10, 0., 0.04, 10, 0., yMax_gamma );
  h2_axes_gamma_vs_por->GetYaxis()->SetTitleOffset( 1.3 );
  h2_axes_gamma_vs_por->GetYaxis()->SetMaxDigits(3);
  h2_axes_gamma_vs_por->SetXTitle( "p_{cnt} / r_{c}" );
  h2_axes_gamma_vs_por->SetYTitle( "Field enhancement factor #beta");
  h2_axes_gamma_vs_por->Draw("");

  TGraphErrors* gr_gamma_vs_por_asgrown = new TGraphErrors(0);
  gr_gamma_vs_por_asgrown->SetMarkerStyle(20);
  gr_gamma_vs_por_asgrown->SetMarkerSize(1.6);
  gr_gamma_vs_por_asgrown->SetMarkerColor(colors[0]);
  gr_gamma_vs_por_asgrown->SetLineColor  (colors[0]);
  gr_gamma_vs_por_asgrown->SetPoint( 0, ivs_asgrown.h()/r_asgrown, g_asgrown );
  gr_gamma_vs_por_asgrown->Draw("Psame");

  TGraphErrors* gr_gamma_vs_por_mild = new TGraphErrors(0);
  gr_gamma_vs_por_mild->SetMarkerStyle(21);
  gr_gamma_vs_por_mild->SetMarkerSize(1.6);
  gr_gamma_vs_por_mild->SetMarkerColor(colors[1]);
  gr_gamma_vs_por_mild->SetLineColor  (colors[1]);
  gr_gamma_vs_por_mild->SetPoint( 0, ivs_mild.h()/r_mild, g_mild );
  gr_gamma_vs_por_mild->Draw("Psame");

  TGraphErrors* gr_gamma_vs_por_strong = new TGraphErrors(0);
  gr_gamma_vs_por_strong->SetMarkerStyle(22);
  gr_gamma_vs_por_strong->SetMarkerSize(1.6);
  gr_gamma_vs_por_strong->SetMarkerColor(colors[2]);
  gr_gamma_vs_por_strong->SetLineColor  (colors[2]);
  gr_gamma_vs_por_strong->SetPoint( 0, ivs_strong.h()/r_strong, g_strong );
  gr_gamma_vs_por_strong->Draw("Psame");

  TGraphErrors* gr_gamma_vs_por_pecvd = new TGraphErrors(0);
  gr_gamma_vs_por_pecvd->SetMarkerStyle(23);
  gr_gamma_vs_por_pecvd->SetMarkerSize(1.6);
  gr_gamma_vs_por_pecvd->SetMarkerColor(kGray+3);
  gr_gamma_vs_por_pecvd->SetLineColor(kGray+3);
  gr_gamma_vs_por_pecvd->SetPoint( 0, ivs_pecvd.h()/r_pecvd, g_pecvd );
  gr_gamma_vs_por_pecvd->Draw("Psame");

  legend_gamma2->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("gamma_vs_por.pdf");

  return 0;

}



void fillHistoFromFile( const std::string& fileName, TH1D* histo, float& mean, float& mean_err ) {

  std::vector<float> data;

  std::string fullFileName = "data/" + fileName;

  std::ifstream ifs( fullFileName.c_str() );

  for (std::string line; std::getline(ifs, line); ) {

    float r(std::atof(line.c_str()));

    if( r >= 2. )
      histo->Fill( r );
    data.push_back(r);


  } // for

  float rms;
  AndCommon::get_mean_and_rms( data, mean, rms );

  mean_err = rms / sqrt( (float)(data.size()) );

}


void  getGammaFromFile( float& gamma, float& gamma_err, const std::string& file ) {

  std::ifstream ifs(Form("plots/%s/gamma.dat", file.c_str()));

  ifs >> gamma >> gamma_err;

}
