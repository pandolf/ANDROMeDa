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


IVScan::IVScan( const IVScan& scan ) : IScan(scan) {

}


IVScan::~IVScan() {

}


void IVScan::add_deltaV( float deltaV ) {

  TGraphErrors* graph = this->graph();
//std::string name(graph->GetName());
//graph->SetName("gr_tmp");

//TGraphErrors* graph_deltaV = new TGraphErrors(0);
//graph_deltaV->SetName( name.c_str() );

  for( unsigned iPoint=0; iPoint < graph->GetN(); ++iPoint ) {

    double x, y;
    graph->GetPoint( iPoint, x, y );
    double xerr = this->graph()->GetErrorX( iPoint );
    double yerr = this->graph()->GetErrorY( iPoint );

    graph->SetPoint     ( iPoint, x+deltaV, y );
    graph->SetPointError( iPoint, xerr, yerr );

  } // for points

  this->set_graph( graph );
  //delete graph;

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
    graph_vsE->SetPointError( iPoint, xerr/this->d(), yerr/sqrt(this->n()) );

  } // for points

  return graph_vsE;

}



