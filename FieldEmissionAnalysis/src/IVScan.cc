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

  gamma_ = 0.;
  gamma_err_ = 0.;

  graph_ = new TGraphErrors(0);
  //graph_->SetName( "gr_iv" );
  graph_->SetName( Form("gr_%s", name_.c_str()) );
  graph_->SetMarkerSize(1.8);
  graph_->SetMarkerStyle(20);

  graphFN_ = new TGraphErrors(0);
  //graphFN_->SetName( "gr_fn" );
  graphFN_->SetName( Form("graphFN_%s", name_.c_str()) );
  graphFN_->SetMarkerSize(1.8);
  graphFN_->SetMarkerStyle(20);

  readFile();

  setColor(46);

}


IVScan::~IVScan() {

  delete graph_;
  graph_ = 0;

  delete graphFN_;
  graphFN_ = 0;

}



std::string IVScan::name() const {

  return name_;

}



TGraphErrors* IVScan::graph() const {

  return graph_;

}


TGraphErrors* IVScan::graphFN() const {

  return graphFN_;

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


float IVScan::gamma() const {

  return gamma_;

}


float IVScan::gamma_err() const {

  return gamma_err_;

}


void IVScan::set_graph( TGraphErrors* graph ) {

  graph_ = graph;

  set_graphFN();

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

  graphFN_->SetMarkerColor( color );
  graphFN_->SetLineColor( color );

  graphFN_->GetFunction( Form("line_%s", name_.c_str()) )->SetLineColor( color );

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


  // now set FN graph:
  set_graphFN();
 

}



void IVScan::set_graphFN() {

  for( unsigned iPoint=0; iPoint<graph_->GetN(); ++iPoint ) {

    double i, v;
    graph_->GetPoint(iPoint, v, i);
    float i_err = graph_->GetErrorY( iPoint );
    float v_err = 1.;

    graphFN_->SetPoint     ( iPoint, 1./v, TMath::Log( i / (v*v) ) );
    graphFN_->SetPointError( iPoint, v_err/(v*v), i_err/i );

  } // for

  TF1* f1_line = new TF1( Form("line_%s", name_.c_str()), "[0]+[1]*x" );
  f1_line->SetLineColor(46);
  f1_line->SetLineWidth(2);
  graphFN_->Fit( f1_line, "Q+" );

  float phi = 4.7; // in eV
  float d = d_; // in mm
  float d_err = 0.1; // see logbook_ANDROMeDa entry 24/01/22 for details on why 0.1 mm
  float s = f1_line->GetParameter(1);
  float s_err = f1_line->GetParError(1);
  gamma_ = -6.83E6*phi*sqrt(phi)*d/s;
  gamma_err_ = sqrt( gamma_*gamma_/(s*s)*s_err*s_err + gamma_*gamma_/(d*d)*d_err*d_err );

} 


TF1* IVScan::lineFN() const {

  return graphFN_->GetFunction("line");

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

  for( unsigned i=0; i<v.size(); ++i )
    mean += v[i];
  
  mean /= (float)(v.size());


  for( unsigned i=0; i<v.size(); ++i ) 
    rms += (v[i] - mean)*(v[i] - mean);
  rms /= (float)(v.size()-1);
  rms = sqrt(rms);

}




float IVScan::xMinFN() {

  return 0.00025;

}


float IVScan::xMaxFN() {

  return 0.0007;

}


float IVScan::yMinFN() {

  return -20.;

}


float IVScan::yMaxFN() {

  return 0.;

}


std::string IVScan::xTitleFN() {

  return "1/V (V^{-1})";

}


std::string IVScan::yTitleFN() {

  return "Log(I/V^{2}) (a.u.)";

}


