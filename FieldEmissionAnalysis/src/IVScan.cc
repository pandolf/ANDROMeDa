#include "../interface/IVScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"





IVScan::IVScan( const std::string& name, float scale ) : IScan(name, scale) {

}


IVScan::~IVScan() {

}



TGraphErrors* IVScan::graph_vsE() const {

  TGraphErrors* graph_vsE = new TGraphErrors(0);
  graph_vsE->SetName( Form("gr_E_%s", this->name().c_str()) );
  graph_vsE->SetMarkerStyle( 20 );
  graph_vsE->SetMarkerColor( this->color() );
  graph_vsE->SetLineColor  ( this->color() );

  for( unsigned iPoint=0; iPoint<this->graph()->GetN(); ++iPoint ) {

    double x,y;
    this->graph()->GetPoint( iPoint, x, y );
    double xerr = this->graph()->GetErrorX( iPoint );
    double yerr = this->graph()->GetErrorY( iPoint );

    graph_vsE->SetPoint( iPoint, x/this->d(), y );
    graph_vsE->SetPointError( iPoint, xerr/this->d(), yerr );

  } // for points

  return graph_vsE;

}



