#include "../interface/IVScanFN.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"



IVScanFN::IVScanFN( const std::string& name, float scale, float xMin, float xMax ) : IVScan( name, scale, xMin, xMax ) {

  gamma_ = 0.;
  gamma_err_ = 0.;

  graphFN_ = new TGraphErrors(0);
  //graphFN_->SetName( "gr_fn" );
  graphFN_->SetName( Form("graphFN_%s", this->name().c_str()) );
  graphFN_->SetMarkerSize(1.8);
  graphFN_->SetMarkerStyle(20);

  // set FN graph:
  set_graphFN();

  setColor(46);

}


IVScanFN::~IVScanFN() {

  //delete graphFN_;
  //graphFN_ = 0;

}



TGraphErrors* IVScanFN::graphFN() const {

  return graphFN_;

}


float IVScanFN::gamma() const {

  return gamma_;

}


float IVScanFN::gamma_err() const {

  return gamma_err_;

}




void IVScanFN::setColor( int color ) {

  IVScan::setColor(color);

  graphFN_->SetMarkerColor( color );
  graphFN_->SetLineColor( color );

  graphFN_->GetFunction( Form("line_%s", this->name().c_str()) )->SetLineColor( color );

}



void IVScanFN::set_graph( TGraphErrors* graph ) {

  IVScan::set_graph(graph);

  set_graphFN();

}




void IVScanFN::set_graphFN() {

  for( unsigned iPoint=0; iPoint<this->graph()->GetN(); ++iPoint ) {

    double i, v;
    this->graph()->GetPoint(iPoint, v, i);
    float i_err = this->graph()->GetErrorY( iPoint );
    float v_err = 1.;

    graphFN_->SetPoint     ( iPoint, 1./v, TMath::Log( i / (v*v) ) );
    graphFN_->SetPointError( iPoint, v_err/(v*v), i_err/i );

  } // for

  TF1* f1_line = new TF1( Form("line_%s", this->name().c_str()), "[0]+[1]*x" );
  f1_line->SetLineColor(46);
  f1_line->SetLineWidth(2);
  graphFN_->Fit( f1_line, "Q+" );

  float phi = 4.7; // in eV
  float d = this->d(); // in mm
  float d_err = 0.1; // see logbook_ANDROMeDa entry 24/01/22 for details on why 0.1 mm
  float s = f1_line->GetParameter(1);
  float s_err = f1_line->GetParError(1);
  gamma_ = -6.83E6*phi*sqrt(phi)*d/s;
  gamma_err_ = sqrt( gamma_*gamma_/(s*s)*s_err*s_err + gamma_*gamma_/(d*d)*d_err*d_err );

} 


TF1* IVScanFN::lineFN() const {

  return graphFN_->GetFunction(Form("line_%s", this->name().c_str()));

}



float IVScanFN::xMinFN() {

  return 0.00025;

}


float IVScanFN::xMaxFN() {

  return 0.0007;

}


float IVScanFN::yMinFN() {

  return -20.;

}


float IVScanFN::yMaxFN() {

  return 0.;

}


std::string IVScanFN::xTitleFN() {

  return "1/V (V^{-1})";

}


std::string IVScanFN::yTitleFN() {

  return "Log(I/V^{2}) (a.u.)";

}


