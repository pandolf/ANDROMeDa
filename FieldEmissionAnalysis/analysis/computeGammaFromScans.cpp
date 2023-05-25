#include "AndCommon.h"
#include "IVScan.h"

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


  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., 2000., 10, 0., 30. );
  h2_axes->SetXTitle( "#DeltaV [V]" );
  h2_axes->SetYTitle( "I [#muA]" );

  TH2D* h2_axesE = new TH2D( "axesE", "", 10, 0., 400., 10, 0., 30. );
  h2_axesE->SetXTitle( "#DeltaV / d [V/mm]" );
  h2_axesE->SetYTitle( "I [#muA]" );


  int nsteps = 20;
  int startstep = 0;
  float stepsize = 0.1; // in mm

  for( int istep=startstep; istep<nsteps; ++istep ) {

    c1->Clear();
    c2->Clear();

    c1->cd();
    h2_axes->Draw();

    c2->cd();
    h2_axesE->Draw();

    TLegend* legend = new TLegend( 0.2, 0.65, 0.5, 0.9, sampleName.c_str() );
    legend->SetTextSize(0.035);
    legend->SetFillColor(0);


    for( unsigned i=0; i<scans.size(); ++i ) {

      IVScan scan(scans[i]);

      scan.scaleDataPoints( 1./1000000. ); // in microA

      scan.set_d( scan.d() + istep*stepsize );

      if( istep==0 ) { // only once

        c1->cd();
        TGraphErrors* graph = scan.graph();
        graph->SetMarkerSize( 1.6 );
        graph->SetMarkerColor( colors[i] );
        graph->SetLineColor( colors[i] );
        graph->Draw( "P same" );

      } // if step =0 

      c2->cd();
      TGraphErrors* graph_vsE = scan.graph_vsE();
      graph_vsE->SetMarkerSize( 1.6 );
      graph_vsE->SetMarkerColor( colors[i] );
      graph_vsE->SetLineColor( colors[i] );
      graph_vsE->Draw( "P same" );

      legend->AddEntry( graph_vsE, Form("d = %.1f mm", scan.d()), "P" );

    }

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

    delete legend;

  } // steps


  return 0;

}





/*
  for( unsigned iPoint=0; iPoint<this->graph()->GetN(); ++iPoint ) {

    double i, v;
    this->graph()->GetPoint(iPoint, v, i);
    i = fabs(i);
    v = fabs(v);
    float i_err = this->graph()->GetErrorY( iPoint );
    float v_err = 1.;

    graphFN_->SetPoint     ( iPoint, 1./v, TMath::Log( i / (v*v) ) );
    graphFN_->SetPointError( iPoint, v_err/(v*v), i_err/i );

  } // for


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
