#include "../interface/IVScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"





IVScan::IVScan( const std::string& name ) : IScan(name) {


}


IVScan::~IVScan() {

//delete graph_;
//graph_ = 0;

}



TGraphErrors* IVScan::graph_vsE() const {

  TGraphErrors* graph_vsE = new TGraphErrors(0);
  graph_vsE->SetName( Form("gr_E_%s", this->name().c_str()) );

  for( unsigned iPoint=0; iPoint<this->graph()->GetN(); ++iPoint ) {

    double x,y;
    this->graph()->GetPoint( iPoint, x, y );
    double xerr = this->graph()->GetErrorX( iPoint );
    double yerr = this->graph()->GetErrorY( iPoint );

    graph_vsE->SetPoint( iPoint, x/this->d(), y );
    graph_vsE->SetPointError( iPoint, xerr, yerr );

  } // for points

  return graph_vsE;

}



void IVScan::readDataLine( const std::vector< std::string >& words ) {

  float hv = std::atof( words[0].c_str() );

  std::vector<float> v_i;
  for( unsigned iw=1; iw<words.size(); ++iw )
    v_i.push_back( std::atof(words[iw].c_str()) );

  addPointToGraph( hv, v_i );

}



void IVScan::addPointToGraph( float hv, std::vector<float> i_meas ) {

  float mean, rms;
  getMeanRMS( i_meas, mean, rms );

  int iPoint = this->graph()->GetN();
  this->graph()->SetPoint     ( iPoint, hv, mean );
  this->graph()->SetPointError( iPoint, 1., rms  );

}



void IVScan::getMeanRMS( std::vector<float> v, float& mean, float& rms ) {

  mean = 0.;
  rms = 0.;

  if( v.size()<2 ) {

    std::cout << "[IVScan::getMeanRMS] Data vector size < 2" << std::endl;
    exit(2);

  } else if( v.size()==2 ) { // first is mean second is RMS

    mean = v[0];
    rms  = v[1];

  } else { // just a number of measurements

    for( unsigned i=0; i<v.size(); ++i )
      mean += v[i];
    
    mean /= (float)(v.size());


    for( unsigned i=0; i<v.size(); ++i ) 
      rms += (v[i] - mean)*(v[i] - mean);
    rms /= (float)(v.size()-1);
    rms = sqrt(rms);

  }  // if

}

