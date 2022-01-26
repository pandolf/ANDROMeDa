#include "../interface/IVScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"



IVScan::IVScan( const std::string& name ) {

  std::cout << "[IVScam] Setting up new IVScan with name: " << name << std::endl;

  name_ = name;

  p_ = 0.;
  d_ = -1.;

  graph_ = new TGraphErrors(0);
  graph_->SetName( Form("gr_%s", name_.c_str()) );

}


IVScan::~IVScan() {

  delete graph_;
  graph_ = 0;

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

} // readFile




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

  for( unsigned i=0; i<v.size(); ++i )
    mean += v[i];
  
  mean /= (float)(v.size());


  for( unsigned i=0; i<v.size(); ++i ) 
    rms += (v[i] - mean)*(v[i] - mean);
  rms /= (float)(v.size()-1);
  rms = sqrt(rms);

}



TGraphErrors* IVScan::getFNgraph() const {

  TGraphErrors* gr_fn = new TGraphErrors(0);
  gr_fn->SetName( Form("gr_fn_%s", name_.c_str()) );

  for( unsigned iPoint=0; iPoint<graph_->GetN(); ++iPoint ) {

    double i, v;
    graph_->GetPoint(iPoint, v, i);
    float i_err = graph_->GetErrorY( iPoint );
    float v_err = 1.;

    gr_fn->SetPoint     ( iPoint, 1./v, TMath::Log( i / (v*v) ) );
    gr_fn->SetPointError( iPoint, v_err/(v*v), i_err/i );

  } // for

  return gr_fn;

}

