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




IVScanFN analyzeFN( const std::string& name );



int main( int argc, char* argv[] ) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./drawFieldEmission [dataName]" << std::endl;
    exit(1);
  }

  std::string name( AndCommon::removePathAndSuffix(argv[1]) );

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();


  std::vector< IVScanFN > scans;

  if( name == "CNTArO2Etching_N1" ) {

    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d5_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d4_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d3_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d2_20221130" ) );

  } else if( name == "CNTArO2Etching_N1new" ) {

    scans.push_back( analyzeFN( "CNTArO2Etching_N1new_d5_20230517_drain" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1new_d4_20230517_drain" ) );

  } else {

    scans.push_back( analyzeFN(name) );

  }
    

  return 0;

}



IVScanFN analyzeFN( const std::string& name ) {


  //float hvMin = -1.;
  //float hvMax = 3000.;

  //if( name=="CNTArO2Etching_N1_d5_20221130" ) {

  //  hvMin = 480.;
  //  hvMax = 560.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_N1_d4_20221130" ) {

  //  hvMin = 385.;
  //  hvMax = 440.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_N1_d3_20221130" ) {

  //  hvMin = 280.;
  //  hvMax = 350.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_N1_d2_20221130" ) {

  //  hvMin = 175.;
  //  hvMax = 350.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_AG_d5_new" ) {

  //  hvMin = 1930.;
  //  hvMax = 2100.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_AG_d4_new" ) {

  //  hvMin = 1500.;
  //  hvMax = 1800.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_AG_d3_new" ) {

  //  hvMin = 1110.;
  //  hvMax = 1230.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_AG_d4_20221214" ) {

  //  hvMin = 789.;
  //  hvMax = 3000.;
  //  scale = -1.;

  //} else if( name=="CNTArO2Etching_AG2_d3_20221215" ) {

  //  hvMin = 210.;
  //  hvMax = 3000.;
  //  scale = -1.;

  //} else if( name=="CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain" ) {

  //  //hvMin = 730.; // first component
  //  //hvMax = 900.;
  //  hvMin = 1030.; // second component
  //  hvMax = 1500.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_N6bis_LNGS_HdM_d1p5_t30_20230315_drain" ) {

  //  //hvMin = 730.; // first component
  //  //hvMax = 900.;
  //  hvMin = 590.; // second component
  //  hvMax = 770.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_AG2_LNGS_HdM_d1p5_t20_20230404" ) {

  //  //hvMin = 730.; // first component
  //  //hvMax = 900.;
  //  //hvMin = 680.; // second component
  //  //hvMax = 880.;

  //} else if( name=="CNTetchedOLD_Strongnew_d3_20230927_2" ) {

  //  hvMin = 190.;
  //  hvMax = 255.;

  //} else if( name=="CNTetchedOLD_Strongnew_d4_20230927" ) {

  //  hvMin = 340.;
  //  //hvMax = 255.;

  //} else if( name=="CNTetchedOLD_Strongnew_d5_20230927" ) {

  //  hvMin = 390.;
  //  hvMax = 510.;

  //} else if( name=="CNTetchedOLD_Strongnew_d3_20230927_drain" ) {

  //  //hvMin = 390.;
  //  //hvMax = 510.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_Strongnew_d4_20230927_drain" ) {

  //  //hvMin = 390.;
  //  //hvMax = 510.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_Strongnew_d5_20230927_drain" ) {

  //  //hvMin = 390.;
  //  //hvMax = 510.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_Strongnew_d3_20231002_drain_2" ) {

  //  hvMin = 359.;
  //  //hvMax = 510.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_Strongnew_d4_20231002_drain_2" ) {

  //  hvMin = 560.;
  //  //hvMax = 510.;
  //  scale = +1.;

  //} else if( name=="CNTetchedOLD_Strongnew_d5_20231006_drain" ) {

  //  //hvMin = 560.;
  //  hvMax = 460.;
  //  scale = +1.;

  //}


  float scale = -1.;
  float iMin = 0.5; // default: compute FN only between 0.5 pA
  float iMax = 10.; //          and 10 pA

  TString name_tstr(name);
  if( name_tstr.Contains("drain") ) scale = +1.;

  IVScanFN ivs( name, scale, iMin, iMax );



  // ***** FIRST STEP: draw I vs V

  TGraphErrors* graph = ivs.graph();

  if( graph->GetN()==0 ) {
    std::cout << "-> ERROR! No data points in graph. Check the data file or change hvMin and hvMax to select more points." << std::endl;
    exit(1);
  }


  std::string outdir(Form("plots/%s/", ivs.name().c_str()));
  system( Form("mkdir -p %s", outdir.c_str()) );


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  //float xMin = 1300.;
  //float xMax = 2999.;
  //float xMin = 0.9*hvMin;
  //float xMax = 1.1*hvMax;


  float xMin_iv, xMax_iv, yMin_iv, yMax_iv;
  AndCommon::findGraphRanges( graph, xMin_iv, xMax_iv, yMin_iv, yMax_iv );

  xMin_iv = 0.9*xMin_iv;
  xMax_iv = 1.1*xMax_iv;
  yMin_iv = 0.9*yMin_iv;
  yMax_iv = 1.1*yMax_iv;

  if( ivs.lab() == "INRiM" ) {
    if( ivs.d() < 0.9 ) {
      xMin_iv = 0.;
      xMax_iv = 70.;
    } else {
      xMin_iv = 170.;
      xMax_iv = 220.;
    }
    yMin_iv = -1.9999;
    yMax_iv = 5.;
  } else {
    xMin_iv = 0.;
    yMin_iv = 0.;
  }


  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin_iv, xMax_iv, 10, yMin_iv, yMax_iv );
  h2_axes->SetXTitle( "-#DeltaV (V)" );
  h2_axes->SetYTitle( "I (pA)" );
  h2_axes->Draw();

  TLine* line_zero = new TLine( xMin_iv, 0., xMax_iv, 0. );
  line_zero->Draw("P same");

  TLine* line_one  = new TLine( xMin_iv, 1., xMax_iv, 1. );
  line_one->SetLineStyle(2);
  if( ivs.lab() == "INRiM" ) line_one->Draw("P same");

  TPaveText* pd_text = new TPaveText( 0.2, 0.7, 0.5, 0.85, "brNDC" );
  pd_text->SetFillColor(0);
  pd_text->SetTextSize(0.038);
  pd_text->SetTextAlign(13);
  pd_text->SetTextColor(kGray+3);
  pd_text->AddText( AndCommon::cuteSampleName( ivs.sampleName() ).c_str() );
  //if( name_tstr.Contains( "INRiM" ) ) 
  //  pd_text->AddText( Form("INRiM") );
  if( ivs.t() < 200 )
    pd_text->AddText( Form("%s, T = %.1f K"  , ivs.lab().c_str(), ivs.t()) );
  else
    pd_text->AddText( Form("%s, p =  %s mbar", ivs.lab().c_str(), AndCommon::scientific(ivs.p(), 0).c_str()) );
  pd_text->AddText( Form("d = %.1f mm"  , ivs.d()) );
  pd_text->Draw("Same");

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);
  graph->SetMarkerColor(46);
  graph->SetLineColor  (46);

  graph->Draw("P same");

  gPad->RedrawAxis();

  c1->SaveAs( Form("%s/iv.pdf", outdir.c_str()) );

  c1->Clear();
  c1->SetLogy();

  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, 0., xMax_iv, 10, 0.1*yMin_iv, 20.*yMax_iv);
  h2_axes_log->SetXTitle( "-#DeltaV (V)" );
  h2_axes_log->SetYTitle( "I (nA)" );
  h2_axes_log->Draw();

  pd_text->Draw("Same");
  graph->Draw("P same");

  c1->SaveAs( Form("%s/iv_log.pdf", outdir.c_str()) );




  // ***** SECOND STEP: draw FN plots

  TGraphErrors* gr_fn = ivs.graphFN();
  TF1* lineFN = gr_fn->GetFunction(Form("lineFN_%s", gr_fn->GetName()));


  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->Clear();

  TGaxis::SetMaxDigits(2);


  float xMinFN, xMaxFN, yMinFN, yMaxFN;
  AndCommon::findGraphRanges( gr_fn, xMinFN, xMaxFN, yMinFN, yMaxFN );

  xMinFN = 0.95*xMinFN;
  xMaxFN = 1.05*xMaxFN;

  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, xMinFN, xMaxFN, 10, -20, 5);
  h2_axes_fn->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axes_fn->SetYTitle( IVScanFN::yTitleFN().c_str() );
  h2_axes_fn->Draw();

  gr_fn->SetMarkerStyle(20);
  gr_fn->SetMarkerSize(1.5);
  gr_fn->SetMarkerColor(kGray+3);
  gr_fn->SetLineColor  (kGray+3);

  gr_fn->Draw("P same");

  std::cout << std::endl;
  std::cout << "Chi^2 / NDF: " << lineFN->GetChisquare() << " / " << lineFN->GetNDF() <<std::endl;
  std::cout << std::endl;


//// check outliers:
//for( unsigned iPoint=0; iPoint<gr_fn->GetN(); ++iPoint ) {

//  double x, y;
//  gr_fn->GetPoint( iPoint, x, y );

//  double yerr = gr_fn->GetErrorY( iPoint );

//  float delta = fabs( lineFN->Eval(x)-y )/yerr;

//  if( delta > 3. ) {

//    std::cout << std::endl;
//    std::cout << "   WARNING!! POINT #" << iPoint << " is an OUTLIER!" << std::endl;
//    std::cout << "     hv = " << 1/x << " Volts" << std::endl;
//    std::cout << "     y = " << y << " +/- " << yerr << std::endl;
//    std::cout << "     f(x) = " << lineFN->Eval(x) << std::endl;
//    std::cout << "     Delta = " << delta << " sigma" << std::endl;
//    std::cout << std::endl;

//  } // if outlier

//} // for points

 
  float gamma_err_corr, gamma_err_ucorr;
  float gamma = ivs.get_gamma_and_err( gamma_err_corr, gamma_err_ucorr );
  float gamma_err = sqrt( gamma_err_corr*gamma_err_corr + gamma_err_ucorr*gamma_err_ucorr );

  TPaveText* gamma_text = new TPaveText( 0.6, 0.8, 0.85, 0.9, "brNDC" );
  gamma_text->SetFillColor(0);
  gamma_text->SetTextSize(0.038);
  gamma_text->SetTextColor( 46 );
  //gamma_text->SetTextColor( kGray+3 );
  gamma_text->AddText( Form("#gamma = %.0f #pm %.0f", gamma, gamma_err) );
  gamma_text->Draw("same");

  TPaveText* pd_text_log = new TPaveText( 0.2, 0.2, 0.5, 0.3, "brNDC" );
  pd_text_log->SetFillColor(0);
  pd_text_log->SetTextSize(0.038);
  pd_text_log->SetTextColor(kGray+3);
  //pd_text_log->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
  pd_text_log->AddText( Form("d = %.1f mm"  , ivs.d()) );
  pd_text_log->Draw("same");


  c1_fn->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );

  std::cout << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << std::endl << std::endl;


  std::string graphsFileName(Form("%s/graphs.root", outdir.c_str()));

  TFile* outfile = TFile::Open( graphsFileName.c_str(), "RECREATE" );
  outfile->cd();

  graph->Write();
  gr_fn->Write();

  outfile->Write();
  outfile->Close();

  std::cout << "-> Stored graphs in: " << graphsFileName << std::endl;



  return ivs;

}


