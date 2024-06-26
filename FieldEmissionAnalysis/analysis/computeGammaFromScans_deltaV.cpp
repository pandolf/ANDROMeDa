#include "AndCommon.h"
#include "IVScan.h"
#include "IVScanFN.h"
#include "UncCorr.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TFitResult.h"
#include "TLine.h"





TGraphErrors* selectPointsForFN( TGraphErrors graph, int nPointsSelected );
int findHighestVoltage( TGraph* graph );
void findChiSquareMinPlusOne( TGraphErrors* gr_chi2_vs_istep, float minChi2, int& iMinusSigma, int& iPlusSigma );
void addGraphToGraph( TGraphErrors* graph, TGraphErrors* graph_tot, float iMin, float iMax );







int main( int argc, char* argv[] ) {



  if( argc < 2 ) {
    std::cout << "USAGE: ./computeGammaFromScans [sampleName]" << std::endl;
    exit(1);
  }

  std::string sampleName( argv[1] );

  std::vector< IVScanFN* > scans;

  float iMin = 0.8;
  float iMax = 3.;
  float vmax = 2200.;
  int nPointsSelect = 5;
  std::string legendName = "xxx";

  if( sampleName == "CNTArO2Etching_AG_old" ) {

    scans.push_back( new IVScanFN("CNTArO2Etching_AG_d3_new.dat", -1., 1110., 1230.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_AG_d4_new.dat", -1., 1500., 1800.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_AG_d5_new.dat", -1., 1930., 2100.) );

    legendName = "No etching (as grown)";
    iMax = 7.;
    nPointsSelect = -1; // all

  } else if( sampleName == "CNTArO2Etching_N1" ) {

    //sampleName = "Mild Ar/O_{2} Etching";
    //scans.push_back( new IVScanFN("CNTArO2Etching_N1_d2_20221130", -1.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d3_20221130", -1., 278., 400.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d4_20221130", -1., 381., 500.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d5_20221130", -1., 472., 600.) );
    //scans.push_back( new IVScanFN("CNTArO2Etching_N1_d3_20221130", -1.) );//, 250., 400.) );
    //scans.push_back( new IVScanFN("CNTArO2Etching_N1_d4_20221130", -1.) );//, 350., 500.) );
    //scans.push_back( new IVScanFN("CNTArO2Etching_N1_d5_20221130", -1.) );//, 450., 600.) );

    legendName = "Mild Ar/O_{2} etching";
    iMax = 7.;
    vmax = 700.;
    nPointsSelect = -1; // all

  } else if( sampleName == "CNTetchedOLD_AGnew" ) {

    scans.push_back( new IVScanFN("CNTetchedOLD_AGnew_d3_20230519_drain.dat") );
    scans.push_back( new IVScanFN("CNTetchedOLD_AGnew_d4_20230519_drain.dat") );
    scans.push_back( new IVScanFN("CNTetchedOLD_AGnew_d5_20230519_drain.dat") );

  } else if( sampleName == "CNTArO2Etching_N1new" ) {

    //scans.push_back( "CNTArO2Etching_N1new_d3p3_20230517_drain.dat" );
    //scans.push_back( "CNTArO2Etching_N1new_d4p3_20230517_drain_2.dat" );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1new_d4_20230517_drain.dat"  ) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1new_d4p3_20230517_drain.dat") );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1new_d5_20230517_drain.dat"  ) );

  } else if( sampleName == "CNTArO2Etching_N2new" ) {

    scans.push_back( new IVScanFN("CNTArO2Etching_N2new_d3p3_20230518_drain.dat") );
    scans.push_back( new IVScanFN("CNTArO2Etching_N2new_d4_20230518_drain.dat") );
    scans.push_back( new IVScanFN("CNTArO2Etching_N2new_d3_20230518_drain.dat") );
    scans.push_back( new IVScanFN("CNTArO2Etching_N2new_d3p6_20230518_drain.dat") );

  } else if( sampleName == "CNTetchedOLD_Strongnew" ) {

    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3_20231006_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d4_20231006_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d5_20231006_drain.dat") );

    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3p5_20231012_drain.dat") );
    scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d4_20231012_drain.dat") );
    scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d5_20231012_drain.dat") );

    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3_20230927_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d4_20230927_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d5_20230927_drain.dat") );

    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d4_20230927.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d5_20230927.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3_20230927.dat") );

    legendName = "Strong O_{2} etching";
    iMax = 350.;
    vmax = 650.;
    nPointsSelect = -1; // all

    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3p6_20230519_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d4_20230519_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d5_20230519_drain.dat") );
    //scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3_20230519_drain.dat") );

  } else if( sampleName == "CNTetchedOLD_Strongnew_mod" ) {

    scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d3_20230927_mod.dat", -1.) );
    scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d4_20230927.dat"    , -1.) );
    scans.push_back( new IVScanFN("CNTetchedOLD_Strongnew_d5_20230927.dat"    , -1.) );

    legendName = "Strong O_{2} etching";
    iMax = 350.;
    vmax = 650.;
    nPointsSelect = -1; // all

  } else if( sampleName == "CNTArO2Etching_N3new" ) {

    scans.push_back( new IVScanFN("CNTArO2Etching_N3new_d3_20230519_drain.dat") );
    scans.push_back( new IVScanFN("CNTArO2Etching_N3new_d4_20230519_drain.dat") );
    scans.push_back( new IVScanFN("CNTArO2Etching_N3new_d5_20230519_drain.dat") );

  } else if( sampleName=="CNTArO2Etching_N1" ) {

    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d2_20221130", -1.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d3_20221130", -1.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d4_20221130", -1.) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1_d5_20221130", -1.) );

  } else if( sampleName == "CNTArO2Etching_N1new_B_INRiM_3K" ) {

    scans.push_back( new IVScanFN("CNTArO2Etching_N1new_B_INRiM_3Kplate_d0p5_t2p7_20231120_drain_90to125V_sweepR"      ,  1., iMin, iMax) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1new_B_INRiM_MICa_3Kplate_d1p0_IvsV_drain_140to215V_20231130_sweepR",  1., iMin, iMax) );
    scans.push_back( new IVScanFN("CNTArO2Etching_N1new_B_INRiM_MICa_3Kplate_d1p5_IvsV_drain_190to295V_20231207_sweepR",  1., iMin, iMax) );
    //scans.push_back( new IVScanFN("CNTArO2Etching_N1new_B_INRiM_MICa_3Kplate_d1p5_IvsV_anode_200to295V_20231206_sweepR", -1., 0.8, 3.) );

    vmax = 250.;

  } else {

    std::cout << "-> Unknown sample: " << sampleName << std::endl;
    exit(3);

  }

  if( legendName == "xxx" ) legendName = sampleName;



  std::string outdir("plots/GammaScans_deltaV/"+sampleName);
  system( Form("mkdir -p %s/steps", outdir.c_str()) );


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.145);
  style->SetPadRightMargin(0.11);
  style->SetPadTopMargin(0.07);
  style->SetTitleYOffset(1.4);
  style->cd();

  std::vector<int> colors = AndCommon::colors();
  for( unsigned i=0; i<scans.size(); ++ i ) scans[i]->set_color(colors[i]);


  std::cout << " -> Selecting points from graphs..." << std::endl << std::endl;



  int nsteps = 100;
  //int nsteps = 120;
  float start_deltaV = 55.; // in V
  //float start_delta = -1.3; // in mm, relative to central d
  float stepsize = 0.1; // in V

  float minChi2 = 99999999.;
  float NDF = 0.;
  int step_minChi2 = -1;
  float gamma_comb_minChi2 = 0.;
  float gamma_err_comb_minChi2 = 0.;
  float gamma_nocorr_minChi2 = 0.;
  float gamma_err_nocorr_minChi2 = 0.;

  TGraphErrors* gr_chi2_vs_istep = new TGraphErrors(0);
  gr_chi2_vs_istep->SetName("gr_chi2_vs_istep");

  TGraphErrors* gr_chi2red_vs_istep = new TGraphErrors(0);
  gr_chi2red_vs_istep->SetName("gr_chi2red_vs_istep");

  //TGraphErrors* gr_gamma_vs_istep = new TGraphErrors(0);
  //gr_gamma_vs_istep->SetName("gr_gamma_vs_istep");

  std::vector<TGraphErrors*> gr_gamma_vs_istep;
  for( unsigned i=0; i<scans.size(); ++i ) {
    TGraphErrors* thisgraph = new TGraphErrors(0);
    thisgraph->SetName( Form("gr_gamma_vs_istep_%d", i) );
    thisgraph->SetMarkerStyle(20);
    thisgraph->SetMarkerColor(scans[i]->color());
    thisgraph->SetLineColor  (scans[i]->color());
    thisgraph->SetMarkerSize (1.3);
    gr_gamma_vs_istep.push_back(thisgraph);
  }

  TCanvas* c3 = new TCanvas( "c3", "", 600, 600 );

  TH2D* h2_axes_totfit = new TH2D( "axes_totfit", "", 10, 0., 300., 10, 0., iMax );
  h2_axes_totfit->SetXTitle( "#DeltaV/d (V/mm)" );
  h2_axes_totfit->SetYTitle( "I (pA)" );
  h2_axes_totfit->GetXaxis()->SetNdivisions(505);
  h2_axes_totfit->GetYaxis()->SetNdivisions(505);


  // define fitting function outside the loop so that it remembers parameter values from previous fit
  // (this drastically improves convergence probability)
  TF1* f1_exp_totfit = new TF1( "exp_step_totfit", "exp([0] + [1]*x)" );
  f1_exp_totfit->SetLineColor(46);


  // scan deltad to find minimum

  for( int istep=0; istep<nsteps; ++istep ) {

    std::cout << std::endl << std::endl;
    std::cout << "------------------------" << std::endl << std::endl;
    std::cout << " -> STARTING STEP: " << istep << std::endl << std::endl;

    c3->Clear();

    h2_axes_totfit->Draw();

    float this_deltaV = start_deltaV + (float)istep*stepsize;
    std::cout << " deltaV = " << this_deltaV << std::endl;

    UncCorr uc;

    TGraphErrors* gr_selected_totfit = new TGraphErrors(0); // bunch them all together in one graph
    gr_selected_totfit->SetName( Form("gr_selected_totfit_step%d", istep) );
    gr_selected_totfit->SetMarkerStyle( 20 );
    gr_selected_totfit->SetMarkerColor( kGray+3 );
    gr_selected_totfit->SetMarkerSize ( 1.6 );
    gr_selected_totfit->SetLineColor  ( kGray+3 );


    //TPaveText* label_d = new TPaveText( 0.2, 0.7, 0.5, 0.85, "brNDC" );
    //label_d->SetTextSize( 0.035 );
    //label_d->SetFillColor( 0 );
    //label_d->SetTextAlign( 11 );

    for( unsigned i=0; i<scans.size(); ++i ) {

      float this_d = scans[i]->d();

      //label_d->AddText( Form("d_{%d} = %.1f mm", i, this_d) );

      IVScanFN scan_deltaV(*(scans[i]));
      scan_deltaV.add_deltaV( -this_deltaV );

      addGraphToGraph( scan_deltaV.graph_vsE(), gr_selected_totfit, iMin, iMax );

      TGraphErrors* graphFN = scan_deltaV.graphFN();
      TF1* f1_line = graphFN->GetFunction( Form( "lineFN_%s", graphFN->GetName() ) );
      
      float gamma_err_tot_uncorr, gamma_err_tot_corr;
      TString name_tstr(scan_deltaV.name() );
      float derrcorr = ( name_tstr.Contains("20230927") ) ? 0.5 : -1.; // set 0.5 mm uncertainty on relative d because setup had bad parallax
      float gamma = scan_deltaV.get_gamma_and_err( gamma_err_tot_uncorr, gamma_err_tot_corr, f1_line->GetParameter(1), f1_line->GetParError(1), this_d, derrcorr );

      uc.addDataPoint(gamma, gamma_err_tot_uncorr, gamma_err_tot_corr);

      int gamma_N = gr_gamma_vs_istep[i]->GetN();
      gr_gamma_vs_istep[i]->SetPoint( gamma_N, this_deltaV, gamma );
      gr_gamma_vs_istep[i]->SetPointError( gamma_N, 0., sqrt( gamma_err_tot_uncorr*gamma_err_tot_uncorr + gamma_err_tot_corr*gamma_err_tot_corr ) );


    } // for scans


    c3->cd();

    h2_axes_totfit->Draw();

    float xMin_totfit, xMax_totfit, yMin_totfit, yMax_totfit;
    AndCommon::findGraphRanges( gr_selected_totfit, xMin_totfit, xMax_totfit, yMin_totfit, yMax_totfit );

    f1_exp_totfit->SetRange( 0.9999*xMin_totfit, 1.00001*xMax_totfit );
    //f1_exp_totfit->SetParLimits( 1, 0., 10.);
    //f1_exp_totfit->SetParameter( 0, -30. );
    //f1_exp_totfit->SetParameter( 1, 0.2 );
    f1_exp_totfit->SetParameter( 0, -2.85687e+01 );
    f1_exp_totfit->SetParameter( 1, 2.80401e-01 );
    

    gr_selected_totfit->Fit(f1_exp_totfit, "R+");
    gr_selected_totfit->Draw( "P same" );

    //label_d->Draw("same");

    float thisChi2 = f1_exp_totfit->GetChisquare();
    NDF  = (float)(f1_exp_totfit->GetNDF());

    std::cout << " -> Chi2: " << thisChi2 << std::endl;

    c3->cd();
    TPaveText* label_deltaV = new TPaveText( 0.7, 0.2, 0.9, 0.3, "brNDC" );
    label_deltaV->SetTextSize(0.038);
    label_deltaV->SetFillColor(0);
    label_deltaV->AddText( Form("#DeltaV = %.2f V", this_deltaV) );
    label_deltaV->Draw("same");

    gPad->RedrawAxis();
    c3->SaveAs( Form("%s/steps/i_vs_e_step%d.pdf", outdir.c_str(), istep) );

    if( thisChi2/NDF<0.1 ) continue;

    gr_chi2_vs_istep   ->SetPoint( gr_chi2_vs_istep   ->GetN(), this_deltaV, thisChi2     );
    gr_chi2red_vs_istep->SetPoint( gr_chi2red_vs_istep->GetN(), this_deltaV, thisChi2/NDF );

    float gamma_comb, gamma_err_comb;
    uc.combine( gamma_comb, gamma_err_comb );

    std::cout << "Combined measurement: " << gamma_comb << " +/- " << gamma_err_comb << std::endl;

    //gr_gamma_vs_istep->SetPoint     ( istep, this_delta_d, gamma_comb );
    //gr_gamma_vs_istep->SetPointError( istep, 0., gamma_err_comb );


    float gamma_nocorr, gamma_err_nocorr;
    uc.combine( gamma_nocorr, gamma_err_nocorr, false );

    std::cout << "Combined measurement (no correlations): " << gamma_nocorr << " +/- " << gamma_err_nocorr << std::endl;


    if( thisChi2<minChi2 ) {
      minChi2 = thisChi2;
      step_minChi2 = istep;
      gamma_comb_minChi2 = gamma_comb;
      gamma_err_comb_minChi2 = gamma_err_comb;
      gamma_nocorr_minChi2 = gamma_nocorr;
      gamma_err_nocorr_minChi2 = gamma_err_nocorr;
    }



  } // steps


  std::cout << "minChi2 : " <<  minChi2 << std::endl;
  std::cout << "step_minChi2  : " <<  step_minChi2  << std::endl;
  std::cout << "gamma_comb_minChi2  : " <<  gamma_comb_minChi2  << std::endl;
  std::cout << "gamma_err_comb_minChi2  : " <<  gamma_err_comb_minChi2  << std::endl;
  std::cout << "gamma_nocorr_minChi2  : " <<  gamma_nocorr_minChi2  << std::endl;
  std::cout << "gamma_err_nocorr_minChi2 : " <<  gamma_err_nocorr_minChi2  << std::endl;

  TCanvas* c4 = new TCanvas( "c4", "", 600, 600 );
  c4->cd();

  float xMin4, xMax4, yMin4, yMax4;
  AndCommon::findGraphRanges( gr_chi2_vs_istep, xMin4, xMax4, yMin4, yMax4 );

  xMin4 *= 0.9;
  xMax4 *= 1.1;
  yMin4 *= 0.9;
  yMax4 *= 1.1;

  TH2D* h2_axesChiSquare = new TH2D( "axesChiSquare", "", 10, xMin4, xMax4, 10, 0., NDF*100. );
  h2_axesChiSquare->SetXTitle( "-#DeltaV (V)" );
  h2_axesChiSquare->SetYTitle( Form("#chi^{2} (NDF = %.0f)", NDF) );
  h2_axesChiSquare->Draw();

  gr_chi2_vs_istep->SetMarkerStyle(20);
  gr_chi2_vs_istep->SetMarkerColor(kGray+3);
  gr_chi2_vs_istep->SetLineColor(kGray+3);
  gr_chi2_vs_istep->SetMarkerSize(1.1);


  float deltaV_minChi2 = start_deltaV + (float)step_minChi2*stepsize;

  TLine* line_min = new TLine( deltaV_minChi2, 0., deltaV_minChi2, minChi2 );
  line_min->SetLineColor(46);
  line_min->SetLineWidth(2);
  line_min->Draw("same");

  int iMinusSigma, iPlusSigma;
  findChiSquareMinPlusOne( gr_chi2_vs_istep, minChi2, iMinusSigma, iPlusSigma );

  double xMinusSigma, yMinusSigma;
  gr_chi2_vs_istep->GetPoint( iMinusSigma, xMinusSigma, yMinusSigma );
  double xPlusSigma, yPlusSigma;
  gr_chi2_vs_istep->GetPoint( iPlusSigma, xPlusSigma, yPlusSigma );

  TLine* lineMinusSigma = new TLine( xMinusSigma, 0., xMinusSigma, yMinusSigma );
  lineMinusSigma->SetLineColor(46);
  lineMinusSigma->SetLineWidth(2);
  lineMinusSigma->SetLineStyle(2);
  lineMinusSigma->Draw("same");

  TLine* linePlusSigma = new TLine( xPlusSigma, 0., xPlusSigma, yPlusSigma );
  linePlusSigma->SetLineColor(46);
  linePlusSigma->SetLineWidth(2);
  linePlusSigma->SetLineStyle(2);
  linePlusSigma->Draw("same");

  gr_chi2_vs_istep->Draw("P same");

  float d_err_new = (xPlusSigma-xMinusSigma)/2.;

  TPaveText* label_d_err = new TPaveText( 0.6, 0.2, 0.8, 0.3, "brNDC" );
  label_d_err->SetTextSize( 0.035 );
  label_d_err->SetFillColor(0);
  label_d_err->AddText( Form("d_{1} = (%.3f #pm %.3f) mm", deltaV_minChi2, d_err_new) );
  //label_d_err->AddText( Form("#sigma(d_{0}) = %.2f mm", d_err_new) );
  //label_d_err->Draw("same");


  gPad->RedrawAxis();

  c4->SaveAs( Form("%s/chi2Scan.pdf", outdir.c_str()) );
  c4->Clear();

  TH2D* h2_axesChiSquareRed = new TH2D( "axesChiSquareRed", "", 10, xMin4, xMax4, 10, 0., 100. );
  h2_axesChiSquareRed->SetXTitle( "-#DeltaV (V)" );
  h2_axesChiSquareRed->SetYTitle( Form("#chi^{2} / (NDF = %.0f)", NDF) );
  h2_axesChiSquareRed->Draw();

  gr_chi2red_vs_istep->SetMarkerStyle(20);
  gr_chi2red_vs_istep->SetMarkerColor(kGray+3);
  gr_chi2red_vs_istep->SetLineColor(kGray+3);
  gr_chi2red_vs_istep->SetMarkerSize(1.1);

  gr_chi2red_vs_istep->Draw("Psame");


  gPad->RedrawAxis();

  c4->SaveAs( Form("%s/chi2redScan.pdf", outdir.c_str()) );


  std::cout << std::endl << std::endl;
  std::cout << "-> From Chi2 scan, updated uncertainty on d: " << d_err_new << " mm (was 0.3 mm)" << std::endl;



  TLegend* legend = new TLegend( 0.2, 0.68, 0.5, 0.9, legendName.c_str() );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., vmax, 10, 0., iMax );
  h2_axes->SetXTitle( "#DeltaV (V)" );
  h2_axes->SetYTitle( "I (pA)"   );
  h2_axes->GetXaxis()->SetNdivisions(505);
  h2_axes->Draw();

  float xMinFN = 10.;
  float xMaxFN = 0.;
  float yMinFN = 999.;
  float yMaxFN = -999.;


  std::vector<TGraphErrors*> graphFN_optimal;

  std::string textableline = legendName;

  UncCorr uc, uc_new;

  std::cout << std::endl << std::endl;;
  std::cout << std::endl << "##############################################################################" << std::endl << std::endl;
  std::cout << "         At optimal point:" << std::endl;

  // plot I vs V and FN plots with optimal d values
  // plus update gamma measurement with new d uncertainty
  for( unsigned i=0; i<scans.size(); ++i ) {

    float this_d = scans[i]->d();

    IVScanFN scan_optimal(*(scans[i]));
    scan_optimal.add_deltaV( -deltaV_minChi2 );

    TGraphErrors* graph_optimal = scan_optimal.reducedgraph();
    TGraphErrors* graphFN_optimal_this = scan_optimal.graphFN();
 
    c1->cd();
    graph_optimal->Draw("P same" );

    legend->AddEntry( graph_optimal, Form("d = %.2f mm", this_d), "P" );

    float xMinFN_this, xMaxFN_this, yMinFN_this, yMaxFN_this;
    AndCommon::findGraphRanges( graphFN_optimal_this, xMinFN_this, xMaxFN_this, yMinFN_this, yMaxFN_this );

    TF1* f1_line = graphFN_optimal_this->GetFunction( Form( "lineFN_%s", graphFN_optimal_this->GetName() ) );
    f1_line->SetRange( 0.98*xMinFN_this, 1.02*xMaxFN_this );

    if( xMinFN_this < xMinFN ) xMinFN = xMinFN_this;
    if( yMinFN_this < yMinFN ) yMinFN = yMinFN_this;
    if( xMaxFN_this > xMaxFN ) xMaxFN = xMaxFN_this;
    if( yMaxFN_this > yMaxFN ) yMaxFN = yMaxFN_this;

    graphFN_optimal.push_back(graphFN_optimal_this);

    float gamma_err_tot_uncorr, gamma_err_tot_corr;
    float gamma = scans[i]->get_gamma_and_err( gamma_err_tot_uncorr, gamma_err_tot_corr, f1_line->GetParameter(1), f1_line->GetParError(1), this_d, -1 );

    float gamma_err_tot_uncorr_new, gamma_err_tot_corr_new;
    float gamma_new = scans[i]->get_gamma_and_err( gamma_err_tot_uncorr_new, gamma_err_tot_corr_new, f1_line->GetParameter(1), f1_line->GetParError(1), this_d, d_err_new );

    std::cout << std::endl;
    std::cout << "         Scan " << i << " (d = " << this_d << " mm)" << std::endl;
    std::cout << "         ";
    uc    .addDataPoint(gamma    , gamma_err_tot_uncorr    , gamma_err_tot_corr    );
    std::cout << "         ";
    uc_new.addDataPoint(gamma_new, gamma_err_tot_uncorr_new, gamma_err_tot_corr_new);

    std::string thisText(Form(" & $%.0f \\pm %.0f \\pm %.0f$", gamma_new, gamma_err_tot_uncorr_new, gamma_err_tot_corr_new));
    textableline = textableline + thisText;

  } // for scans

  std::cout << std::endl << "##############################################################################" << std::endl << std::endl;



  TCanvas* c2 = new TCanvas( "c2", "", 600, 600 );
  c2->cd();

  TH2D* h2_axesFN = new TH2D( "axesFN", "", 10, 0.9*xMinFN, 1.1*xMaxFN, 10, yMinFN-0.8, yMaxFN+2. );
  h2_axesFN->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axesFN->SetYTitle( IVScanFN::yTitleFN().c_str() );
  //h2_axesFN->GetYaxis()->SetMaxDigits(3);
  h2_axesFN->GetXaxis()->SetNdivisions(505);
  h2_axesFN->GetYaxis()->SetNdivisions(505);
  h2_axesFN->Draw();

  for(unsigned i=0; i<scans.size(); ++i ) graphFN_optimal[i]->Draw("P same" );

  legend->Draw("same");
  gPad->RedrawAxis();

  c2->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );

  c1->cd();
  legend->Draw("same");
  gPad->RedrawAxis();
  c1->SaveAs( Form("%s/i_vs_v.pdf", outdir.c_str()) );


  TCanvas* c1_gamma = new TCanvas( "c1_gamma", "", 600, 600 );
  c1_gamma->cd();

  TH2D* h2_axes_gamma = new TH2D( "axes_gamma", "", 10, xMin4, xMax4, 10, 0., 80000. );
  h2_axes_gamma->SetXTitle( "#DeltaV (V)" );
  h2_axes_gamma->SetYTitle( "#gamma" );
  h2_axes_gamma->Draw();

  for( unsigned iScan=0; iScan<scans.size(); ++iScan ) 
    gr_gamma_vs_istep[iScan]->Draw("Psame");
    
  gPad->RedrawAxis();

  c1_gamma->SaveAs( Form("%s/gamma_vs_istep.pdf", outdir.c_str()) );


  float gamma_comb, gamma_err_comb;
  uc.combine( gamma_comb, gamma_err_comb );
  
  float gamma_comb_new, gamma_err_comb_new;
  uc_new.combine( gamma_comb_new, gamma_err_comb_new );
  
  std::cout << std::endl << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
  std::cout << "  Found min chi2: " << minChi2 << " at step: " << step_minChi2 << std::endl;
  std::cout << "  Corresponding gamma: " << gamma_comb_minChi2 << " +/- " << gamma_err_comb_minChi2 << std::endl;
  std::cout << "  (No correlations) gamma: " << gamma_nocorr_minChi2 << " +/- " << gamma_err_nocorr_minChi2 << std::endl;
  std::cout << "  Old gamma: " << gamma_comb << " +/- " << gamma_err_comb << std::endl;
  std::cout << "  (Constraining d to chi2 scan) gamma: " << gamma_comb_new << " +/- " << gamma_err_comb_new << std::endl;
  std::cout << "------------------------------------------------------------------" << std::endl;
  std::cout << std::endl;


  textableline = textableline + (std::string)(Form(" & $%.0f \\pm %.0f $", gamma_comb_new, gamma_err_comb_new));
  textableline = textableline + " \\\\";

  std::cout << std::endl;
  std::cout << textableline << std::endl;
  std::cout << std::endl;

  return 0;

}




TGraphErrors* selectPointsForFN( TGraphErrors graph, int nPointsSelected ) {

  TGraphErrors* selected = new TGraphErrors(0);
  selected->SetName( Form("selected_%s", graph.GetName()) );

  selected->SetMarkerStyle( graph.GetMarkerStyle() );
  selected->SetMarkerColor( graph.GetMarkerColor() );
  selected->SetMarkerSize ( graph.GetMarkerSize()  );
  selected->SetLineColor  ( graph.GetLineColor()   );


  for( unsigned i = 0; i<nPointsSelected; ++i ) { 

    int iPoint = i;
    //int iPoint = findHighestVoltage( &graph );

    double x, y;
    graph.GetPoint( iPoint, x, y );

    double xerr = graph.GetErrorX( iPoint );
    double yerr = graph.GetErrorY( iPoint );

    int nSelected = selected->GetN();
    selected->SetPoint( nSelected, x, y );
    selected->SetPointError( nSelected, xerr, yerr );

    graph.RemovePoint( iPoint );
 
  }

  return selected;

}



int findHighestVoltage( TGraph* graph ) {

  int foundPoint = -1;

  float maxV = 0.;

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    float thisV = graph->GetPointX( iPoint );

    if( thisV > maxV ) {
      maxV = thisV;
      foundPoint = iPoint;
    }

  } // for iPoint

  return foundPoint;

}




void findChiSquareMinPlusOne( TGraphErrors* gr_chi2_vs_istep, float minChi2, int& iMinusSigma, int& iPlusSigma ) {

  double lastx, lasty, thisx, thisy, lastDeltay=999999.;

  for( unsigned iPoint=0; iPoint<gr_chi2_vs_istep->GetN(); ++iPoint ) {

    gr_chi2_vs_istep->GetPoint( iPoint, thisx, thisy );

    float thisDeltay = thisy-minChi2;

    if( (thisDeltay<=1.) && (lastDeltay>1.) ) iMinusSigma = iPoint-1;
    if( (thisDeltay>1.) && (lastDeltay<=1.) ) iPlusSigma  = iPoint;

    lastx = thisx;
    lasty = thisy;
    lastDeltay = thisDeltay;

  } // for points

}



void addGraphToGraph( TGraphErrors* graph, TGraphErrors* graph_tot, float iMin, float iMax ) {

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    double x, y;
    graph->GetPoint( iPoint, x, y );
    double xerr = graph->GetErrorX( iPoint );
    double yerr = graph->GetErrorY( iPoint );
    //yerr = yerr / sqrt(scans[i]->n()); // uncertainty on the mean of n measurements

    if( y < iMax && y >= iMin ) {
      int i_this = graph_tot->GetN();
      graph_tot->SetPoint     ( i_this, x   , y    ); 
      graph_tot->SetPointError( i_this, xerr, yerr );
    }

  } // for points

}
