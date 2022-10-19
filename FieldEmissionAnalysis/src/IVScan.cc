#include "../interface/IVScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"



IVScan::IVScan( const std::string& name ) {

  name_ = AndCommon::removePathAndSuffix(name);

  std::cout << "[IVScan] Setting up new IVScan with name: " << name_ << std::endl;

  p_ = 0.;
  d_ = -1.;

  graph_ = new TGraphErrors(0);
  //graph_->SetName( "gr_iv" );
  graph_->SetName( Form("gr_%s", name_.c_str()) );
  graph_->SetMarkerSize(1.8);
  graph_->SetMarkerStyle(20);


  readFile();

  setColor(46);

}


IVScan::~IVScan() {

//delete graph_;
//graph_ = 0;

}



std::string IVScan::name() const {

  return name_;

}



TGraphErrors* IVScan::graph() const {

  return graph_;

}



float IVScan::pressure() const {

  return this->p();

}


float IVScan::p() const {

  return p_;

}


float IVScan::d() const {

  return d_;

}



void IVScan::set_graph( TGraphErrors* graph ) {

  graph_ = graph;

}


void IVScan::set_p( float p ) {

  p_ = p;

}


void IVScan::set_d( float d ) {

  d_ = d;

}


void IVScan::setColor( int color ) {

  graph_->SetMarkerColor( color );
  graph_->SetLineColor( color );

}

  

std::string IVScan::getDataFileName( const std::string& dataName ) {

  TString dataName_tstr(dataName);

  std::string dataFileName(dataName);

  if( !(dataName_tstr.BeginsWith("data/")) )
    dataFileName = "data/"+dataFileName;

  if( !(dataName_tstr.EndsWith  (".dat")) )
    dataFileName = dataFileName+".dat";

  return dataFileName;

}




void IVScan::readFile( const std::string& name ) {


  std::string fileName = (name=="") ? getDataFileName(name_) : getDataFileName(name);

  std::cout << "-> Opening data file: " << fileName << std::endl;

  std::ifstream ifs( fileName.c_str() );

  if( !ifs.good() ) {
    std::cout << "[IVScan::readFile] ERROR! File '" << fileName << "' does not exist. Please check name." << std::endl;
    exit(2);
  }

  std::string line;

  if( ifs.good() ) {

    while( getline(ifs,line) ) {

      std::vector<std::string> words = AndCommon::splitString( line, " " );

      if( words.size()<2 ) continue;


      if( line[0] == '#' ) {

        TString line_tstr(line);

        if( line_tstr.BeginsWith("#p") || line_tstr.BeginsWith("# p") ) {
          p_ = std::atof(words[words.size()-1].c_str());
          std::cout << "-> Pressure p = " << p_ << " mbar" << std::endl;
        }
        if( line_tstr.BeginsWith("#d") || line_tstr.BeginsWith("# d") ) {
          d_ = std::atof(words[words.size()-1].c_str());
          std::cout << "-> Distance d = " << d_ << " mm" << std::endl;
        }

        continue;

      }


      float hv = std::atof( words[0].c_str() );

      std::vector<float> v_i;
      for( unsigned iw=1; iw<words.size(); ++iw )
        v_i.push_back( std::atof(words[iw].c_str()) );

      addPointToGraph( hv, v_i );

    }  // while get lines

  } // if file good


}



void IVScan::addPointToGraph( float hv, std::vector<float> i_meas ) {

  float mean, rms;
  getMeanRMS( i_meas, mean, rms );

  int iPoint = graph_->GetN();
  graph_->SetPoint     ( iPoint, hv, mean );
  graph_->SetPointError( iPoint, 1., rms  );

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

