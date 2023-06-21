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





TGraphErrors* selectPointsForFN( TGraphErrors graph );
TGraphErrors* getFNgraph( TGraphErrors* selected );
void initializeFunction( TF1* f1_line, TGraphErrors* gr_FN );
int findHighestVoltage( TGraph* graph );
void findChiSquareMinPlusOne( TGraphErrors* gr_chi2_vs_istep, float minChi2, int& iMinusSigma, int& iPlusSigma );







int main( int argc, char* argv[] ) {



  if( argc < 2 ) {
    std::cout << "USAGE: ./computeGammaFromScans [sampleName]" << std::endl;
    exit(1);
  }

  std::string sampleName( argv[1] );

  std::vector< IVScan* > scans;

  float imax = 30.;
  float vmax = 2200.;

  if( sampleName == "CNTArO2Etching_AG" ) {

    scans.push_back( new IVScan("CNTArO2Etching_AG_d3_new.dat", -1.) );
    scans.push_back( new IVScan("CNTArO2Etching_AG_d4_new.dat", -1.) );
    scans.push_back( new IVScan("CNTArO2Etching_AG_d5_new.dat", -1.) );

    imax = 5E-6;

  } else if( sampleName == "CNTetchedOLD_AGnew" ) {

    scans.push_back( new IVScan("CNTetchedOLD_AGnew_d3_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_AGnew_d4_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_AGnew_d5_20230519_drain.dat") );

  } else if( sampleName == "CNTetchedOLD_N1new" ) {

    //scans.push_back( "CNTetchedOLD_N1new_d3p3_20230517_drain.dat" );
    //scans.push_back( "CNTetchedOLD_N1new_d4p3_20230517_drain_2.dat" );
    scans.push_back( new IVScan("CNTetchedOLD_N1new_d4_20230517_drain.dat"  ) );
    scans.push_back( new IVScan("CNTetchedOLD_N1new_d4p3_20230517_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_N1new_d5_20230517_drain.dat"  ) );

  } else if( sampleName == "CNTetchedOLD_N2new" ) {

    scans.push_back( new IVScan("CNTetchedOLD_N2new_d3p3_20230518_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_N2new_d4_20230518_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_N2new_d3_20230518_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_N2new_d3p6_20230518_drain.dat") );

  } else if( sampleName == "CNTetchedOLD_Strongnew" ) {

    scans.push_back( new IVScan("CNTetchedOLD_Strongnew_d3p6_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_Strongnew_d4_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_Strongnew_d5_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_Strongnew_d3_20230519_drain.dat") );

  } else if( sampleName == "CNTetchedOLD_N3new" ) {

    scans.push_back( new IVScan("CNTetchedOLD_N3new_d3_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_N3new_d4_20230519_drain.dat") );
    scans.push_back( new IVScan("CNTetchedOLD_N3new_d5_20230519_drain.dat") );

  } else if( sampleName=="CNTArO2Etching_N1" ) {

    scans.push_back( new IVScan("CNTArO2Etching_N1_d2_20221130", -1.) );
    scans.push_back( new IVScan("CNTArO2Etching_N1_d3_20221130", -1.) );
    scans.push_back( new IVScan("CNTArO2Etching_N1_d4_20221130", -1.) );
    scans.push_back( new IVScan("CNTArO2Etching_N1_d5_20221130", -1.) );

  } else {

    std::cout << "-> Unknown sample: " << sampleName << std::endl;
    exit(3);

  }


  std::string outdir("plots/GammaScans/"+sampleName);
  system( Form("mkdir -p %s", outdir.c_str()) );


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.145);
  style->SetPadRightMargin(0.11);
  style->SetPadTopMargin(0.07);
  style->SetTitleYOffset(1.4);
  style->cd();

  std::vector<int> colors = AndCommon::colors();


  std::vector< TGraphErrors* > graphs_selected;
  std::vector< TGraphErrors* > graphsFN_selected;


  TLegend* legend = new TLegend( 0.2, 0.65, 0.5, 0.9, sampleName.c_str() );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., vmax, 10, 0., imax );
  h2_axes->SetXTitle( "#DeltaV [V]" );
  h2_axes->SetYTitle( "I [#muA]" );
  h2_axes->GetXaxis()->SetNdivisions(505);
  h2_axes->Draw();

  float xMinFN = 10.;
  float xMaxFN = 0.;
  float yMinFN = 999.;
  float yMaxFN = -999.;

  // first loop to scale data points to muA and to get the graphs
  for( unsigned i=0; i<scans.size(); ++i ) {

    scans[i]->scaleDataPoints( 1E-6 ); // in muA

    TGraphErrors* graph = scans[i]->graph();

    TGraphErrors* gr_selected = selectPointsForFN( *graph );
    gr_selected->SetMarkerSize(1.6);
    gr_selected->SetMarkerColor( colors[i] );
    gr_selected->SetLineColor  ( colors[i] );

    c1->cd();
    gr_selected->Draw("P same" );

    legend->AddEntry( gr_selected, Form("d = %.1f mm", scans[i]->d()), "P" );

    graphs_selected.push_back( gr_selected );
  
    TGraphErrors* gr_FN = getFNgraph( gr_selected );

    float xMinFN_this, xMaxFN_this, yMinFN_this, yMaxFN_this;
    AndCommon::findGraphRanges( gr_FN, xMinFN_this, xMaxFN_this, yMinFN_this, yMaxFN_this );

    if( xMinFN_this < xMinFN ) xMinFN = xMinFN_this;
    if( yMinFN_this < yMinFN ) yMinFN = yMinFN_this;
    if( xMaxFN_this > xMaxFN ) xMaxFN = xMaxFN_this;
    if( yMaxFN_this > yMaxFN ) yMaxFN = yMaxFN_this;

    TF1* f1_line = new TF1( Form("line_%s", gr_FN->GetName()), "[0]+[1]*x" );
    f1_line->SetLineColor(gr_FN->GetLineColor());
    f1_line->SetLineWidth(2);

    //initializeFunction( f1_line, gr_FN );

    gr_FN->Fit( f1_line, "Q+" );

    //c2->cd();
    //gr_FN->Draw("P same" );

    graphsFN_selected.push_back( gr_FN );

  } // for scans


  TCanvas* c2 = new TCanvas( "c2", "", 600, 600 );
  c2->cd();

  TH2D* h2_axesFN = new TH2D( "axesFN", "", 10, 0.9*xMinFN, 1.1*xMaxFN, 10, yMinFN-0.5, yMaxFN+2. );
  h2_axesFN->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axesFN->SetYTitle( IVScanFN::yTitleFN().c_str() );
  //h2_axesFN->GetYaxis()->SetMaxDigits(3);
  h2_axesFN->GetXaxis()->SetNdivisions(505);
  h2_axesFN->GetYaxis()->SetNdivisions(505);
  h2_axesFN->Draw();

  legend->Draw("same");

  for( unsigned i=0; i<graphsFN_selected.size(); ++i ) graphsFN_selected[i]->Draw("P same");


  c2->cd();
  gPad->RedrawAxis();
  c2->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );

  c1->cd();
  legend->Draw("same");
  gPad->RedrawAxis();
  c1->SaveAs( Form("%s/i_vs_v.pdf", outdir.c_str()) );


  int nsteps = 150;
  float start_delta = -0.5; // in mm, relative to central d
  float stepsize = 0.01; // in mm

  float minChi2 = 99999.;;
  int step_minChi2 = -1;
  float gamma_comb_minChi2 = 0.;
  float gamma_err_comb_minChi2 = 0.;
  float gamma_nocorr_minChi2 = 0.;
  float gamma_err_nocorr_minChi2 = 0.;

  TGraphErrors* gr_chi2_vs_istep = new TGraphErrors(0);
  gr_chi2_vs_istep->SetName("gr_chi2_vs_istep");

  TGraphErrors* gr_chi2red_vs_istep = new TGraphErrors(0);
  gr_chi2red_vs_istep->SetName("gr_chi2red_vs_istep");


  TCanvas* c3 = new TCanvas( "c3", "", 600, 600 );

  TH2D* h2_axes_vsE = new TH2D( "axes_vsE", "", 10, 0., 550., 10, 0., imax );
  h2_axes_vsE->SetXTitle( "#DeltaV / d [V/mm]" );
  h2_axes_vsE->SetYTitle( "I [#muA]" );


  //for( int istep=0; istep<3; ++istep ) {
  for( int istep=0; istep<nsteps; ++istep ) {

    c3->Clear();

    h2_axes_vsE->Draw();

    float this_delta_d = start_delta + (float)istep*stepsize;

    UncCorr uc;

    TGraphErrors* gr_selected_forFit = new TGraphErrors(0); // bunch them all together in one graph
    gr_selected_forFit->SetName( Form("gr_selected_forFit_step%d", istep) );
    gr_selected_forFit->SetMarkerStyle( 20 );
    gr_selected_forFit->SetMarkerColor( kGray+3 );
    gr_selected_forFit->SetMarkerSize ( 1.6 );
    gr_selected_forFit->SetLineColor  ( kGray+3 );


    for( unsigned i=0; i<scans.size(); ++i ) {

      float this_d = scans[i]->d() + this_delta_d;

      for( unsigned iPoint=0; iPoint<graphs_selected[i]->GetN(); ++iPoint ) {

        double x, y;
        graphs_selected[i]->GetPoint( iPoint, x, y );
        double xerr = graphs_selected[i]->GetErrorX( iPoint );
        double yerr = graphs_selected[i]->GetErrorY( iPoint );

        int n_vsE = gr_selected_forFit->GetN();
        gr_selected_forFit->SetPoint( n_vsE, x/this_d, y ); // x->x/d (so vs E)
        gr_selected_forFit->SetPointError( n_vsE, xerr/this_d, yerr ); 

      }


      TF1* f1_line = graphsFN_selected[i]->GetFunction( Form( "line_%s", graphsFN_selected[i]->GetName()) );
      
      float phi = 4.7; // in eV
      float phi_err = 0.1; // in eV
      float d = this_d;
      float d_err_corr = 0.3; // 0.1 for the syst on the position (linear shifter, see logbook_ANDROMeDa entry 24/01/22) plus 0.1 for the uncertainty on the length of the tubes
      float d_err_uncorr = 0.01; // relative uncertainty between scans
      float s = f1_line->GetParameter(1);
      float s_err = f1_line->GetParError(1);
      float b = 6.83E6; // this is 4/3 * sqrt(2m) / hbar = 6.83E6 V^-1/2 mm^-1, from FN theory

      float gamma = -b*phi*sqrt(phi)*d/s;

      float gamma_err2_phi      = 9.*gamma*gamma/(4.*phi*phi)*phi_err*phi_err;
      float gamma_err2_d_corr   = gamma*gamma/(d*d)*d_err_corr*d_err_corr; 
      float gamma_err2_d_uncorr = gamma*gamma/(d*d)*d_err_uncorr*d_err_uncorr; 
      float gamma_err2_s        = gamma*gamma/(s*s)*s_err*s_err;

      //float gamma_err2_tot = gamma_err2_phi + gamma_err2_d_corr + gamma_err2_d_uncorr + gamma_err2_s;
      //float gamma_err_tot = sqrt( gamma_err2_tot );

      float gamma_err2_tot_uncorr = gamma_err2_d_uncorr + gamma_err2_s;
      float gamma_err_tot_uncorr = sqrt( gamma_err2_tot_uncorr );

      float gamma_err2_tot_corr = gamma_err2_d_corr + gamma_err2_phi;
      float gamma_err_tot_corr = sqrt( gamma_err2_tot_corr );

      uc.addDataPoint(gamma, gamma_err_tot_uncorr, gamma_err_tot_corr);

    } // for scans


    c3->cd();

    h2_axes_vsE->Draw();

    TF1* f1_exp = new TF1( Form("exp_step%d", istep), "exp([0]+[1]*x)" );
    f1_exp->SetLineColor(46);

    gr_selected_forFit->Fit(f1_exp);
    gr_selected_forFit->Draw( "P same" );

    float thisChi2 = f1_exp->GetChisquare();
    float thisNDF = (float)(f1_exp->GetNDF());

    gr_chi2_vs_istep   ->SetPoint( gr_chi2_vs_istep   ->GetN(), this_delta_d, thisChi2         );
    gr_chi2red_vs_istep->SetPoint( gr_chi2red_vs_istep->GetN(), this_delta_d, thisChi2/thisNDF );


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


    c3->cd();
    //legend->Draw("same");
    gPad->RedrawAxis();
    c3->SaveAs( Form("%s/i_vs_e_step%d.pdf", outdir.c_str(), istep) );


  } // steps


  TCanvas* c4 = new TCanvas( "c4", "", 600, 600 );
  c4->cd();

  float xMin4, xMax4, yMin4, yMax4;
  AndCommon::findGraphRanges( gr_chi2_vs_istep, xMin4, xMax4, yMin4, yMax4 );

  xMin4 *= 0.9;
  xMax4 *= 1.1;
  yMin4 *= 0.9;
  yMax4 *= 1.1;

  TH2D* h2_axes4 = new TH2D( "axes4", "", 10, xMin4, xMax4, 10, 0., yMax4 );
  h2_axes4->SetXTitle( "#Deltad [mm]" );
  h2_axes4->SetYTitle( "#chi^{2} / NDF" );
  //h2_axes4->SetYTitle( "#chi^{2} / NDF" );
  h2_axes4->Draw();

  gr_chi2_vs_istep->SetMarkerStyle(20);
  gr_chi2_vs_istep->SetMarkerColor(kGray+3);
  gr_chi2_vs_istep->SetLineColor(kGray+3);
  gr_chi2_vs_istep->SetMarkerSize(1.1);

  //gr_chi2red_vs_istep->SetMarkerStyle(20);
  //gr_chi2red_vs_istep->SetMarkerColor(kGray+3);
  //gr_chi2red_vs_istep->SetLineColor(kGray+3);
  //gr_chi2red_vs_istep->SetMarkerSize(1.1);

  float delta_d_minChi2 = (float)step_minChi2*stepsize;

  float xMinChi2 = start_delta + delta_d_minChi2;
  TLine* line_min = new TLine( xMinChi2, 0., xMinChi2, minChi2 );
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
  lineMinusSigma->Draw("same");

  TLine* linePlusSigma = new TLine( xPlusSigma, 0., xPlusSigma, yPlusSigma );
  linePlusSigma->SetLineColor(46);
  linePlusSigma->SetLineWidth(2);
  linePlusSigma->Draw("same");

  gr_chi2_vs_istep->Draw("P same");


  gPad->RedrawAxis();

  c4->SaveAs( Form("%s/chi2Scan.pdf", outdir.c_str()) );


  float d_err_new = (xPlusSigma-xMinusSigma)/2.;

  std::cout << "-> From Chi2 scan, updated uncertainty on d: " << d_err_new << " mm (was 0.3 mm)" << std::endl;


  UncCorr uc;

  // update gamma measurement with new d uncertainty
  for( unsigned i=0; i<scans.size(); ++i ) {

    float this_d = scans[i]->d() + xMinChi2;


    TF1* f1_line = graphsFN_selected[i]->GetFunction( Form( "line_%s", graphsFN_selected[i]->GetName()) );
    
    float phi = 4.7; // in eV
    float phi_err = 0.1; // in eV
    float d = this_d; // in mm
    float d_err_corr = d_err_new;
    float d_err_uncorr = 0.01; // relative uncertainty between scans
    float s = f1_line->GetParameter(1);
    float s_err = f1_line->GetParError(1);
    float b = 6.83E6; // this is 4/3 * sqrt(2m) / hbar = 6.83E6 V^-1/2 mm^-1, from FN theory

    float gamma = -b*phi*sqrt(phi)*d/s;

    float gamma_err2_phi      = 9.*gamma*gamma/(4.*phi*phi)*phi_err*phi_err;
    float gamma_err2_d_corr   = gamma*gamma/(d*d)*d_err_corr*d_err_corr; 
    float gamma_err2_d_uncorr = gamma*gamma/(d*d)*d_err_uncorr*d_err_uncorr; 
    float gamma_err2_s        = gamma*gamma/(s*s)*s_err*s_err;

    //float gamma_err2_tot = gamma_err2_phi + gamma_err2_d_corr + gamma_err2_d_uncorr + gamma_err2_s;
    //float gamma_err_tot = sqrt( gamma_err2_tot );

    float gamma_err2_tot_uncorr = gamma_err2_d_uncorr + gamma_err2_s;
    float gamma_err_tot_uncorr = sqrt( gamma_err2_tot_uncorr );

    float gamma_err2_tot_corr = gamma_err2_d_corr + gamma_err2_phi;
    float gamma_err_tot_corr = sqrt( gamma_err2_tot_corr );

    uc.addDataPoint(gamma, gamma_err_tot_uncorr, gamma_err_tot_corr);

  } // for iscans

  float gamma_comb_new, gamma_err_comb_new;
  uc.combine( gamma_comb_new, gamma_err_comb_new );
  
  std::cout << std::endl << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << "  Found min chi2: " << minChi2 << " at step: " << step_minChi2 << std::endl;
  std::cout << "  Corresponding gamma: " << gamma_comb_minChi2 << " +/- " << gamma_err_comb_minChi2 << std::endl;
  std::cout << "  (No correlations) gamma: " << gamma_nocorr_minChi2 << " +/- " << gamma_err_nocorr_minChi2 << std::endl;
  std::cout << "  (Constraining d to chi2 scan) gamma: " << gamma_comb_new << " +/- " << gamma_err_comb_new << std::endl;
  std::cout << "---------------------------------------------------" << std::endl;
  std::cout << std::endl;


  return 0;

}




TGraphErrors* selectPointsForFN( TGraphErrors graph ) {

  TGraphErrors* selected = new TGraphErrors(0);
  selected->SetName( Form("selected_%s", graph.GetName()) );

  selected->SetMarkerStyle( graph.GetMarkerStyle() );
  selected->SetMarkerColor( graph.GetMarkerColor() );
  selected->SetMarkerSize ( graph.GetMarkerSize()  );
  selected->SetLineColor  ( graph.GetLineColor()   );


  //for( unsigned i = 0; i<2; ++i ) { // for now 5 points
  for( unsigned i = 0; i<5; ++i ) { // for now 5 points

    int iPoint = findHighestVoltage( &graph );

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



TGraphErrors* getFNgraph( TGraphErrors* selected ) {

  TGraphErrors* gr_FN = new TGraphErrors(0);
  gr_FN->SetName( Form( "fn_%s", selected->GetName()));

  gr_FN->SetMarkerStyle( selected->GetMarkerStyle() );
  gr_FN->SetMarkerColor( selected->GetMarkerColor() );
  gr_FN->SetMarkerSize ( selected->GetMarkerSize()  );
  gr_FN->SetLineColor  ( selected->GetLineColor()   );


  for( unsigned iPoint=0; iPoint<selected->GetN(); ++iPoint ) {

    double i, v;
    selected->GetPoint(iPoint, v, i);
    i = fabs(i);
    v = fabs(v);
    float i_err = selected->GetErrorY( iPoint );
    float v_err = 1.;

    gr_FN->SetPoint     ( iPoint, 1./v, TMath::Log( i / (v*v) ) );
    gr_FN->SetPointError( iPoint, v_err/(v*v), i_err/i );

  } // for

  return gr_FN;

}



void initializeFunction( TF1* f1_line, TGraphErrors* gr_FN ) {

  double x1, y1;
  gr_FN->GetPoint( 0, x1, y1 );
  
  double x2, y2;
  gr_FN->GetPoint( gr_FN->GetN(), x2, y2 );

  if( x1<x2 ) {

    f1_line->SetRange( 0.9*x1, 1.1*x2 );
    f1_line->SetParameter( 0, x1 );
    f1_line->SetParameter( 1, (y2-y1)/(x2-x1) );

  } else {

    f1_line->SetRange( 0.9*x2, 1.1*x1 );
    f1_line->SetParameter( 0, x2 );
    f1_line->SetParameter( 1, (y1-y2)/(x1-x2) );

  }

}



void findChiSquareMinPlusOne( TGraphErrors* gr_chi2_vs_istep, float minChi2, int& iMinusSigma, int& iPlusSigma ) {

  double lastx, lasty, thisx, thisy, lastDeltay=999999.;

  for( unsigned iPoint=0; iPoint<gr_chi2_vs_istep->GetN(); ++iPoint ) {

    gr_chi2_vs_istep->GetPoint( iPoint, thisx, thisy );

    float thisDeltay = thisy-minChi2;

    if( (thisDeltay<1.) && (lastDeltay>1.) ) iMinusSigma = iPoint-1;
    if( (thisDeltay>1.) && (lastDeltay<1.) ) iPlusSigma  = iPoint;

    lastx = thisx;
    lasty = thisy;
    lastDeltay = thisDeltay;

  } // for points

}
