#include "../interface/IVScanFN.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"






IVScanFN::IVScanFN( const std::string& name, float scale, float iMin, float iMax ) : IVScan( name, scale ) {

  iMin_ = iMin;
  iMax_ = iMax;

}


IVScanFN::IVScanFN( const IVScanFN& scan ) : IVScan(scan) {

  iMin_ = scan.iMin();
  iMax_ = scan.iMax();

}


IVScanFN::~IVScanFN() {

}



float IVScanFN::iMin() const {

  return iMin_;

}


float IVScanFN::iMax() const {

  return iMax_;

}



void IVScanFN::set_iMin( float iMin ) {

  iMin_ = iMin;

}


void IVScanFN::set_iMax( float iMax ) {

  iMax_ = iMax;

}


void IVScanFN::set_iMinMax( float iMin, float iMax ) {

  iMin_ = iMin;
  iMax_ = iMax;

}



TGraphErrors* IVScanFN::graphFN() const {  

  return IVScanFN::graphFN( this->graph(), this->iMin(), this->iMax(), this->n(), this->verr() );

}



TGraphErrors* IVScanFN::graphFN( const IScan* scan, float iMin, float iMax ) {

  return IVScanFN::graphFN( scan->graph(), iMin, iMax, scan->n(), scan->verr() );

}




TGraphErrors* IVScanFN::graphFN( TGraphErrors* graph, float iMin, float iMax, int n, float verr ) {

  std::cout << "[IVScanFN::graphFN] Getting FN graph with iMin = " << iMin << " pA and iMax = " << iMax << " pA" << std::endl; 

  int color = graph->GetMarkerColor();

  TGraphErrors* graphFN = new TGraphErrors(0);
  graphFN->SetName( Form("grFN_%s", graph->GetName()) );
  graphFN->SetMarkerSize(1.6);
  graphFN->SetMarkerStyle(20);
  graphFN->SetMarkerColor(color);
  graphFN->SetLineColor(color);

  float xMin = 999.;
  float xMax = 0.;
  float yMin = 9999.;
  float yMax = -9999.;

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    double i, v;
    graph->GetPoint(iPoint, v, i);
    i = fabs(i);
    v = fabs(v);
    float i_err = graph->GetErrorY( iPoint );
    i_err = i_err / sqrt(n); // uncertainty on mean

    if( (i>=iMin) && (i<iMax) ) {
      int iPointFN = graphFN->GetN();
      float xFN = 1./v;
      float yFN = TMath::Log( i / (v*v) );
      if( xFN < xMin ) xMin = xFN ;
      if( yFN < yMin ) yMin = yFN ;
      if( xFN > xMax ) xMax = xFN ;
      if( yFN > yMax ) yMax = yFN ;
      graphFN->SetPoint     ( iPointFN, xFN, yFN );
      graphFN->SetPointError( iPointFN, verr/(v*v), i_err/i );
    }

  } // for

  float m0 = -fabs(yMax-yMin) / (xMax-xMin);

  TF1* f1_line = new TF1( Form("lineFN_%s", graphFN->GetName()), "[0]+[1]*x" );
  f1_line->SetLineColor( color );
  f1_line->SetLineWidth(2);
  f1_line->SetRange( 0.1*xMin, 1.1*xMax );
  f1_line->SetParameter( 1, m0 );
  graphFN->Fit( f1_line, "QR+" );


  return graphFN;

}



TGraphErrors* IVScanFN::reducedgraph() const {

  TGraphErrors* graph = this->graph();

  TGraphErrors* gr_reduced = new TGraphErrors(0);
  gr_reduced->SetName( Form("red_%s", graph->GetName()) );

  gr_reduced->SetMarkerStyle( graph->GetMarkerStyle() );
  gr_reduced->SetMarkerSize ( graph->GetMarkerSize () );
  gr_reduced->SetMarkerColor( graph->GetMarkerColor() );
  gr_reduced->SetLineColor  ( graph->GetMarkerColor() );

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    double x,y,xerr,yerr;
    graph->GetPoint( iPoint, x, y );

    if( y>=iMin_ && y<iMax_ ) {

      xerr = graph->GetErrorX( iPoint );
      yerr = graph->GetErrorY( iPoint );

      int iPoint_red = gr_reduced->GetN();

      gr_reduced->SetPoint     ( iPoint_red, x, y );
      gr_reduced->SetPointError( iPoint_red, xerr, yerr );
     
    } // if iMin/Max

  } // for points

  return gr_reduced;

}

      


//float IVScanFN::get_gamma_and_err_corr( float& gamma_err_tot_uncorr, float& gamma_err_tot_corr, float derrcorr ) { // derrcorr is by default = -1
//
//  TGraphErrors* graphFN = this->graphFN();
//
//  TF1* f1_line = graphFN->GetFunction(Form("lineFN_%s", graphFN->GetName()));
//
//  float gamma = this->get_gamma_and_err_corr( gamma_err_tot_uncorr, gamma_err_tot_corr, f1_line->GetParameter(1), f1_line->GetParError(1), this->d(), this->d_err_corr() );
//
//  return gamma;
//
//}
//
//
//  
//
//float IVScanFN::get_gamma_and_err_corr( float& gamma_err_tot_uncorr, float& gamma_err_tot_corr, float s, float s_err, float d, float derrcorr ) { // derrcorr is by default = -1
//
//  float gamma = -b()*phi()*sqrt(phi())*d/s;
//
//  if( derrcorr<0. ) derrcorr = d_err_corr();
//
//  float gamma_err2_phi      = (9./4.)*gamma*gamma/(phi()*phi())*phi_err()*phi_err();
//  float gamma_err2_d_corr   = gamma*gamma/(d*d)*derrcorr*derrcorr; 
//  float gamma_err2_d_uncorr = gamma*gamma/(d*d)*d_err_uncorr()*d_err_uncorr(); 
//  float gamma_err2_s        = gamma*gamma/(s*s)*s_err*s_err;
//
//  float gamma_err2_tot_uncorr = gamma_err2_d_uncorr + gamma_err2_s;
//  gamma_err_tot_uncorr = sqrt( gamma_err2_tot_uncorr );
//
//  float gamma_err2_tot_corr = gamma_err2_d_corr + gamma_err2_phi;
//  gamma_err_tot_corr = sqrt( gamma_err2_tot_corr );
//
//  return gamma;
//
//}



float IVScanFN::get_gamma_and_err( float& gamma_err_tot ) {

  TGraphErrors* graphFN = this->graphFN();

  TF1* f1_line = graphFN->GetFunction(Form("lineFN_%s", graphFN->GetName()));

  float d, d_err;

  if( d == 0.5 && this->lab()=="INRiM") {

    float sapphire_grease = 0.5345;
    float sapphire_grease_err = 0.008;
    float scasso = 0.73;
    float scasso_err = 0.01;
    float glue = 0.0355;
    float glue_err = 0.015;
    float silicon = 0.500;
    float silicon_err = 0.025;
    float nanotubes = this->h();
    float nanotubes_err = this->h_err();

    d = sapphire_grease + scasso - glue - silicon - nanotubes;
    d_err = sqrt( sapphire_grease_err*sapphire_grease_err + scasso_err*scasso_err + glue_err*glue_err + silicon_err*silicon_err + nanotubes_err*nanotubes_err );

  } else {

    d = this->d() - this->h();
    d_err = sqrt( this->d_err()*this->d_err() + this->h_err()*this->h_err() );

  } 


  float gamma = this->get_gamma_and_err( gamma_err_tot, f1_line->GetParameter(1), f1_line->GetParError(1), d, d_err );

  return gamma;

}



float IVScanFN::get_gamma_and_err( float& gamma_err_tot, float s, float s_err, float d, float d_err ) { 

  float gamma = -b()*phi()*sqrt(phi())*d/s;

  float gamma_err2_phi = (9./4.)*gamma*gamma/(phi()*phi())*phi_err()*phi_err();
  float gamma_err2_d   = gamma*gamma/(d*d)*d_err*d_err; 
  float gamma_err2_s   = gamma*gamma/(s*s)*s_err*s_err;
  
  std::cout << "d: " << d << " +/- " << d_err << std::endl;
  std::cout << "phi: " << phi() << " +/- " << phi_err() << std::endl;
  std::cout << "s: " << s << " +/- " << s_err << std::endl;

  std::cout << "gamma_err_phi: " <<  sqrt(gamma_err2_phi) << std::endl;
  std::cout << "gamma_err_d  : " <<  sqrt(gamma_err2_d  ) << std::endl;
  std::cout << "gamma_err_s  : " <<  sqrt(gamma_err2_s  ) << std::endl;

  std::cout << "gamma_err_phi/gamma: " <<  sqrt(gamma_err2_phi)/gamma << std::endl;
  std::cout << "gamma_err_d  /gamma: " <<  sqrt(gamma_err2_d  )/gamma << std::endl;
  std::cout << "gamma_err_s  /gamma: " <<  sqrt(gamma_err2_s  )/gamma << std::endl;

  float gamma_err2_tot = gamma_err2_phi + gamma_err2_d + gamma_err2_s;
  gamma_err_tot = sqrt( gamma_err2_tot );

  return gamma;

}



float IVScanFN::phi() {

  float phi = 4.5;

  if( this->sampleName() == "CNTArO2Etching_AsGrown" )
    phi = 4.26;
  else if( this->sampleName() == "CNTArO2Etching_N1new" )
    phi = 4.48;
  else if( this->sampleName() == "CNTArO2Etching_Strongnew" )
    phi = 4.53;

  return phi;

}


float IVScanFN::phi_err() {

  return 0.05; // in eV

}


float IVScanFN::b() {

  return 6.83E6; // this is 4/3 * sqrt(2m) / hbar = 6.83E6 V^-1/2 mm^-1, from FN theory

}



float IVScanFN::d_err() {  // uncertainty on d

  float d_err = 0.1;

  if( this->lab()=="INRiM" ) {

    d_err = 0.05; // 50 microns is the uncertainty on the CNT length, but maybe it doesn't play a role?

  } else {

    d_err = 0.3; // 0.1 for the syst on the position (linear shifter, see logbook_ANDROMeDa entry 24/01/22) plus 0.1 for the uncertainty on the length of the tubes

  }

  return d_err;

}


//float IVScanFN::d_err_uncorr() {  // relative uncertainty on distance between scans, in mm
//
//  float d_err_uncorr = 0.01;
//
//  if( this->lab()=="INRiM" ) {
//
//    if( this->sampleName() == "CNTArO2Etching_AsGrown" )
//      d_err_uncorr = 0.032; // as taken from excel file "misure di distanze"
//    else if( this->sampleName() == "CNTArO2Etching_N1new" )
//      d_err_uncorr = 0.032; // as taken from excel file "misure di distanze"
//    else if( this->sampleName() == "CNTArO2Etching_Strongnew" )
//      d_err_uncorr = 0.046; // as taken from excel file "misure di distanze"
//
//  } else {
//
//    d_err_uncorr = 0.01; // in TITAN uncertainty is precision of linear shifter
//
//  }
//
//  return d_err_uncorr; 
//
//}
//
//
//
//float IVScanFN::d_err_corr() {  // correlated uncertainty on d, ie uncertainty on d_0
//
//  float d_err_corr = 0.1;
//
//  if( this->lab()=="INRiM" ) {
//
//    d_err_corr = 0.05; // 50 microns is the uncertainty on the CNT length, but maybe it doesn't play a role?
//
//  } else {
//
//    d_err_corr = 0.3; // 0.1 for the syst on the position (linear shifter, see logbook_ANDROMeDa entry 24/01/22) plus 0.1 for the uncertainty on the length of the tubes
//
//  }
//
//  return d_err_corr;
//
//}


  

std::string IVScanFN::xTitleFN() {

  return "1/V (V^{-1})";

}


std::string IVScanFN::yTitleFN() {

  return "Log(I/V^{2}) (a.u.)";

}


