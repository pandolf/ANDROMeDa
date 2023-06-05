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



int main( int argc, char* argv[] ) {


  if( argc < 2 ) {
    std::cout << "USAGE: ./computeGammaFromScans [sampleName]" << std::endl;
    exit(1);
  }

  std::string sampleName( argv[1] );

  std::vector< std::string > scans;

  if( sampleName == "CNTetchedOLD_AGnew" ) {

    scans.push_back( "CNTetchedOLD_AGnew_d3_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_AGnew_d4_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_AGnew_d5_20230519_drain.dat" );

  } else if( sampleName == "CNTetchedOLD_N1new" ) {

    //scans.push_back( "CNTetchedOLD_N1new_d3p3_20230517_drain.dat" );
    //scans.push_back( "CNTetchedOLD_N1new_d4p3_20230517_drain_2.dat" );
    scans.push_back( "CNTetchedOLD_N1new_d4_20230517_drain.dat" );
    scans.push_back( "CNTetchedOLD_N1new_d4p3_20230517_drain.dat" );
    scans.push_back( "CNTetchedOLD_N1new_d5_20230517_drain.dat" );

  } else if( sampleName == "CNTetchedOLD_N2new" ) {

    scans.push_back( "CNTetchedOLD_N2new_d3p3_20230518_drain.dat" );
    scans.push_back( "CNTetchedOLD_N2new_d4_20230518_drain.dat" );
    scans.push_back( "CNTetchedOLD_N2new_d3_20230518_drain.dat" );
    scans.push_back( "CNTetchedOLD_N2new_d3p6_20230518_drain.dat" );

  } else if( sampleName == "CNTetchedOLD_Strongnew" ) {

    scans.push_back( "CNTetchedOLD_Strongnew_d3p6_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_Strongnew_d4_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_Strongnew_d5_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_Strongnew_d3_20230519_drain.dat" );

  } else if( sampleName == "CNTetchedOLD_N3new" ) {

    scans.push_back( "CNTetchedOLD_N3new_d3_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_N3new_d4_20230519_drain.dat" );
    scans.push_back( "CNTetchedOLD_N3new_d5_20230519_drain.dat" );

  } else {

    std::cout << "-> Unknown sample: " << sampleName << std::endl;
    exit(3);

  }


  std::string outdir("plots/GammaScans/"+sampleName);
  system( Form("mkdir -p %s", outdir.c_str()) );


  AndCommon::setStyle();

  std::vector<int> colors = AndCommon::colors();


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  TCanvas* c2 = new TCanvas( "c2", "", 600, 600 );
  TCanvas* c3 = new TCanvas( "c3", "", 600, 600 );


  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., 2000., 10, 0., 30. );
  h2_axes->SetXTitle( "#DeltaV [V]" );
  h2_axes->SetYTitle( "I [#muA]" );

  TH2D* h2_axesE = new TH2D( "axesE", "", 10, 0., 400., 10, 0., 30. );
  h2_axesE->SetXTitle( "#DeltaV / d [V/mm]" );
  h2_axesE->SetYTitle( "I [#muA]" );

  TH2D* h2_axesFN = new TH2D( "axesFN", "", 10, 0.0005, 0.0015, 10, -20., 0. );
  h2_axesFN->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axesFN->SetYTitle( IVScanFN::yTitleFN().c_str() );


  int nsteps = 100;
  int startstep = -50;
  float stepsize = 0.01; // in mm

  TGraphErrors* gr_chi2_vs_iStep = new TGraphErrors(0);
  gr_chi2_vs_iStep->SetName("gr_chi2_vs_iStep");

  for( int istep=startstep; istep<nsteps; ++istep ) {

    c1->Clear();
    c2->Clear();
    c3->Clear();

    c1->cd();
    h2_axes->Draw();

    c2->cd();
    h2_axesE->Draw();

    c3->cd();
    h2_axesFN->Draw();


    TLegend* legend = new TLegend( 0.2, 0.65, 0.5, 0.9, sampleName.c_str() );
    legend->SetTextSize(0.035);
    legend->SetFillColor(0);

    UncCorr uc;
    UncCorr uc_x0;

    TGraphErrors* gr_selected_forFit = new TGraphErrors(0); // bunch them all together in one graph
    gr_selected_forFit->SetName( Form("gr_selected_forFit_step%d", istep) );
    gr_selected_forFit->SetMarkerStyle( 20 );
    gr_selected_forFit->SetMarkerColor( 46 );
    gr_selected_forFit->SetMarkerSize ( 1.6 );
    gr_selected_forFit->SetLineColor  ( 46 );


    for( unsigned i=0; i<scans.size(); ++i ) {

      IVScan scan(scans[i]);

      scan.scaleDataPoints( 1E-6 ); // in microA

      scan.set_d( scan.d() + istep*stepsize );

      TGraphErrors* graph = scan.graph();

      if( istep==0 ) { // only once

        c1->cd();
        graph->SetMarkerSize( 1.6 );
        graph->SetMarkerColor( colors[i] );
        graph->SetLineColor( colors[i] );
        graph->Draw( "P same" );

      } // if step =0 

      TGraphErrors* graph_vsE = scan.graph_vsE();
      graph_vsE->SetMarkerSize( 1.6 );
      graph_vsE->SetMarkerColor( colors[i] );
      graph_vsE->SetLineColor  ( colors[i] );

      TGraphErrors* gr_selected = selectPointsForFN( *graph );
      gr_selected->SetMarkerSize(1.6);
      gr_selected->SetMarkerColor( colors[i] );
      gr_selected->SetLineColor  ( colors[i] );

      //TGraphErrors* gr_selected_forFit = new TGraphErrors(0);
      //gr_selected_forFit->SetName( Form("gr_selected_forFit_%s", gr_selected->GetName()) );
      //gr_selected_forFit->SetMarkerStyle( gr_selected->GetMarkerStyle() );
      //gr_selected_forFit->SetMarkerColor( gr_selected->GetMarkerColor() );
      //gr_selected_forFit->SetMarkerSize ( gr_selected->GetMarkerSize()  );
      //gr_selected_forFit->SetLineColor  ( gr_selected->GetLineColor()   );
      //double xmin = 9999.;
      //double xmax = 0.;
      for( unsigned iPoint=0; iPoint<gr_selected->GetN(); ++iPoint ) {
        double x, y;
        gr_selected->GetPoint( iPoint, x, y );
        double xerr = gr_selected->GetErrorX( iPoint );
        double yerr = gr_selected->GetErrorY( iPoint );

        //double thisx = x/scan.d();
        //if( thisx<xmin ) xmin = thisx;
        //if( thisx>xmax ) xmax = thisx;

        int n_vsE = gr_selected_forFit->GetN();
        gr_selected_forFit->SetPoint( n_vsE, x/scan.d(), y ); // x->x/d (so vs E)
        gr_selected_forFit->SetPointError( n_vsE, xerr/scan.d(), yerr ); 
        //gr_selected_forFit->SetPoint( n_vsE, x/scan.d(), log(y) ); // x->x/d (so vs E) and y -> log(y) to fit with line
        //gr_selected_forFit->SetPointError( n_vsE, xerr/scan.d(), yerr/y ); // propag log error -> sigma(y)/y
      }


      //TF1* f1_exp = new TF1( Form("exp_%s", gr_selected_forFit->GetName()), "exp([0]*x-[1])", 0.9*xmin, 1.1*xmax );
      //f1_exp->SetLineColor( gr_selected_forFit->GetLineColor() );
      //gr_selected_forFit->Fit(f1_exp, "R+");

      //TF1* f1_lineLog = new TF1( Form("line_%s", gr_selected_forFit->GetName()), "[0]+[1]*x", 0.9*xmin, 1.1*xmax );
      //f1_lineLog->SetLineColor( gr_selected_forFit->GetLineColor() );
      //gr_selected_forFit->Fit(f1_lineLog, "R+");

      //// want to find x of f(x) = 0
      //// f(x) = mx + q
      //float m = f1_lineLog->GetParameter(1);
      //float q = f1_lineLog->GetParameter(0);
      //float m_err = f1_lineLog->GetParError(1);
      //float q_err = f1_lineLog->GetParError(0);
      //float x0 = -q/m;
      //float x0_err = sqrt( q_err*q_err/(m*m) + q*q*m_err*m_err/(m*m*m*m) );
      //uc_x0.addDataPoint(x0, x0_err, 0.);

      //uc_x0.addDataPoint(f1_exp->GetParameter(1), f1_exp->GetParError(1), 0.);

      //c2->cd();
      //gr_selected_forFit->Draw( "P same" );
      ////graph_vsE->Draw( "P same" );

      //legend->AddEntry( graph_vsE, Form("d = %.1f mm", scan.d()), "P" );


      TGraphErrors* gr_FN = getFNgraph( gr_selected );

      TF1* f1_line = new TF1( Form("line_%s", gr_FN->GetName()), "[0]+[1]*x" );
      f1_line->SetLineColor(gr_FN->GetLineColor());
      f1_line->SetLineWidth(2);

      initializeFunction( f1_line, gr_FN );

      gr_FN->Fit( f1_line, "+" );

      c3->cd();
      gr_FN->Draw("P same");
      
      float phi = 4.7; // in eV
      float phi_err = 0.1; // in eV
      float d = scan.d(); // in mm
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


    c2->cd();

    TF1* f1_exp = new TF1( Form("exp_step%d", istep), "exp([0]+[1]*x)" );
    gr_selected_forFit->Fit(f1_exp);
    gr_selected_forFit->Draw( "P same" );

    gr_chi2_vs_iStep->SetPoint( gr_chi2_vs_iStep->GetN(), istep, f1_exp->GetChisquare()/f1_exp->GetNDF() );

    //float x0_comb, x0_err_comb;
    //uc_x0.combine( x0_comb, x0_err_comb );

    //gr_x0_vs_iStep->SetPoint( gr_x0_vs_iStep->GetN(), istep, x0_comb );
    //gr_x0_err_vs_iStep->SetPoint( gr_x0_err_vs_iStep->GetN(), istep, x0_err_comb/x0_comb );
    

    float gamma_comb, gamma_err_comb;
    uc.combine( gamma_comb, gamma_err_comb );

    std::cout << "Combined measurement: " << gamma_comb << " +/- " << gamma_err_comb << std::endl;

    float gamma_nocorr, gamma_err_nocorr;
    uc.combine( gamma_nocorr, gamma_err_nocorr, false );

    std::cout << "Combined measurement (no correlations): " << gamma_nocorr << " +/- " << gamma_err_nocorr << std::endl;


    if( istep==0 ) { // only once
      c1->cd();
      legend->Draw("same");
      gPad->RedrawAxis();
      c1->SaveAs( Form("%s/i_vs_v.pdf", outdir.c_str()) );
    }

    c2->cd();
    legend->Draw("same");
    gPad->RedrawAxis();
    c2->SaveAs( Form("%s/i_vs_e_step%d.pdf", outdir.c_str(), istep) );

    c3->cd();
    legend->Draw("same");
    gPad->RedrawAxis();
    c3->SaveAs( Form("%s/fn_step%d.pdf", outdir.c_str(), istep) );

    delete legend;

  } // steps


  TFile* outfile = TFile::Open( "test.root", "recreate" );
  outfile->cd();
  gr_chi2_vs_iStep->Write();
  outfile->Close();



  return 0;

}




TGraphErrors* selectPointsForFN( TGraphErrors graph ) {

  TGraphErrors* selected = new TGraphErrors(0);
  selected->SetName( Form("selected_%s", graph.GetName()) );

  selected->SetMarkerStyle( graph.GetMarkerStyle() );
  selected->SetMarkerColor( graph.GetMarkerColor() );
  selected->SetMarkerSize ( graph.GetMarkerSize()  );
  selected->SetLineColor  ( graph.GetLineColor()   );

  for( unsigned i = 0; i<5; ++i ) {

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



/*

  if( graphFN_->GetN() > 1 ) {

    TF1* f1_line = new TF1( Form("line_%s", this->name().c_str()), "[0]+[1]*x" );
    f1_line->SetLineColor(46);
    f1_line->SetLineWidth(2);
    graphFN_->Fit( f1_line, "Q+" );

    float phi = 4.7; // in eV
    float phi_err = 0.1; // in eV
    float d = this->d(); // in mm
    float d_err = 0.2; // 0.1 for the syst on the position (linear shifter, see logbook_ANDROMeDa entry 24/01/22) plus 0.1 for the uncertainty on the length of the tubes
    float s = f1_line->GetParameter(1);
    float s_err = f1_line->GetParError(1);
    float b = 6.83E6; // this is 4/3 * sqrt(2m) / hbar = 6.83E6 V^-1/2 mm^-1, from FN theory
    gamma_ = -b*phi*sqrt(phi)*d/s;
    gamma_err_ = sqrt( gamma_*gamma_/(s*s)*s_err*s_err + gamma_*gamma_/(d*d)*d_err*d_err + 9.*gamma_*gamma_/(4.*phi*phi)*phi_err*phi_err ); // propagazione

  }

}

*/
