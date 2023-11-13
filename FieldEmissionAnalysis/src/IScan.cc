#include "../interface/IScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"





IScan::IScan( const std::string& name, float scale, float vMin, float vMax ) {

  name_ = AndCommon::removePathAndSuffix(name);

  std::cout << "[IScan] Setting up new IScan with name: " << name_ << std::endl;

  p_ = 0.;    // mbar
  d_ = -1.;   // mm
  t_ = 300.;  // K
  hv_ = "caen472";
  dz_ = 0.1;  // mm
  verr_ = 1.; // V
  n_ = 1; 

  graph_ = new TGraphErrors(0);
  graph_->SetName( Form("gr_%s", name_.c_str()) );
  graph_->SetMarkerSize(1.8);
  graph_->SetMarkerStyle(20);

  vMin_ = vMin;
  vMax_ = vMax;

  readFile( getDataFileName(name_) );

  if( scale!=1 ) scaleDataPoints( scale );

  setColor(46);

}


IScan::~IScan() {

//delete graph_;
//graph_ = 0;

}



std::string IScan::name() const {

  return name_;

}



TGraphErrors* IScan::graph() const {

  return graph_;

}



float IScan::pressure() const {

  return this->p();

}


float IScan::p() const {

  return p_;

}


float IScan::d() const {

  return d_;

}


float IScan::t() const {

  return t_;

}


std::string IScan::hv() const {

  return hv_;

}


float IScan::dz() const {

  return dz_;

}


float IScan::verr() const {

  return verr_;

}


int IScan::n() const {

  return n_;

}




void IScan::set_graph( TGraphErrors* graph ) {

  graph_ = graph;

}



void IScan::set_p( float p ) {

  p_ = p;

}


void IScan::set_t( float t ) {

  t_ = t;

}


void IScan::set_d( float d ) {

  std::cout << "IScan::set_d() Setting d = " << d << std::endl;
  d_ = d;

}


void IScan::set_hv( const std::string& hv ) {

  hv_ = hv;

}


void IScan::set_dz( float dz ) {

  dz_ = dz;

}


void IScan::set_verr( float verr ) {

  verr_ = verr;

}


void IScan::set_n( int n ) {

  n_ = n;

}


void IScan::setColor( int color ) {

  graph_->SetMarkerColor( color );
  graph_->SetLineColor( color );

}


float IScan::vMin() const {

  return vMin_;

}


float IScan::vMax() const {

  return vMax_;

}


void IScan::set_vMin( float vMin ) {

  vMin_ = vMin;

}


void IScan::set_vMax( float vMax ) {

  vMax_ = vMax;

}


  

std::string IScan::getDataFileName( const std::string& dataName ) {

  TString dataName_tstr(dataName);

  std::string dataFileName(dataName);

  if( !(dataName_tstr.BeginsWith("data/")) )
    dataFileName = "data/"+dataFileName;

  if( !(dataName_tstr.EndsWith  (".dat")) )
    dataFileName = dataFileName+".dat";

  return dataFileName;

}





void IScan::readCommentLine( const std::vector< std::string >& words ) {

  if( (words[0]=="#p") || (words[0]=="#" && words[1]=="p") ) {
    p_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Pressure p = " << p_ << " mbar" << std::endl;
  }
  if( (words[0]=="#d") || (words[0]=="#" && words[1]=="d") ) {
    d_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Distance d = " << d_ << " mm" << std::endl;
  }
  if( (words[0]=="#t") || (words[0]=="#" && words[1]=="t") ) {
    t_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Temperature = " << t_ << " K" << std::endl;
  }
  if( (words[0]=="#hv") || (words[0]=="#" && words[1]=="hv") ) {
    hv_ = words[words.size()-1];
    std::cout << "-> Power supply: " << hv_ << std::endl;
    if( hv_ == "caenN472" ) {
      std::cout << "[IScan::readCommentLine] Recognized power supply CAEN N472 " << std::endl;
      std::cout << " -> Setting voltage uncertainty to 1 V" << std::endl;
      verr_ = 1.;
    } else if( hv_ == "keithley6487" )  {
      verr_ = 0.1;
      std::cout << "[IScan::readCommentLine] Recognized power supply Keithley 6487 " << std::endl;
      std::cout << " -> Setting voltage uncertainty to 0.1 V" << std::endl;
    } else {
      std::cout << "[IScan::readCommentLine] Warning!! Unknown power supply model: " << hv_ << std::endl;
      std::cout << " -> Setting voltage uncertainty to 1 Volt!" << std::endl;
      verr_ = 1.;
    }
  }
  if( (words[0]=="#dz") || (words[0]=="#" && words[1]=="dz") ) {
    dz_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Uncertainty on delta(d) = " << dz_ << " mm" << std::endl;
  }
  if( (words[0]=="#verr") || (words[0]=="#" && words[1]=="verr") ) {
    verr_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Voltage uncertainty = " << verr_ << " V" << std::endl;
  }
  if( (words[0]=="#n") || (words[0]=="#" && words[1]=="n") ) {
    n_ = std::atoi(words[words.size()-1].c_str());
    std::cout << "-> Number of points in current measurements = " << n_ << std::endl;
  }

}


void IScan::readDataLine( const std::vector< std::string >& words, bool& addToGraph ) {

  float x    = std::atof( words[0].c_str() );
  float y    = std::atof( words[1].c_str() );
  float yerr = std::atof( words[2].c_str() );

  if( (x > vMin_) && (x < vMax_) ) {
    int iPoint = this->graph()->GetN();
    this->graph()->SetPoint     ( iPoint, x   , y    );
    this->graph()->SetPointError( iPoint, verr_, yerr ); 
  }

}




void IScan::scaleDataPoints( float scale ) {

  std::cout << "-> Scaling graph by factor: " << scale << std::endl;

  for( unsigned iPoint=0; iPoint<graph_->GetN(); ++iPoint ) {

    double x,y;
    graph_->GetPoint( iPoint, x, y );
    double xerr, yerr;
    xerr = graph_->GetErrorX( iPoint );
    yerr = graph_->GetErrorY( iPoint );

    graph_->SetPoint( iPoint, x, y*scale );
    graph_->SetPointError( iPoint, xerr, abs(yerr*scale) );

  } // for ipoints

}


//void IScan::addPointToGraph( float hv, std::vector<float> i_meas ) {
//
//  float mean, rms;
//  getMeanRMS( i_meas, mean, rms );
//
//  int iPoint = this->graph()->GetN();
//  this->graph()->SetPoint     ( iPoint, hv, mean );
//  this->graph()->SetPointError( iPoint, 1., rms  );
//
//}



void IScan::readFile( const std::string& name ) {


  std::string fileName = (name=="") ? getDataFileName(name_) : getDataFileName(name);

  std::cout << "-> Opening data file: " << fileName << std::endl;
  if( vMin_ > -50000. && vMax_ < 50000 ) std::cout << "-> Will add only data points with " << vMin_ << " < x < " << vMax_ << std::endl;
  else if( vMin_ > -50000. ) std::cout << "-> Will add only data points with x > " << vMin_ << std::endl;
  else if( vMax_ <  50000. ) std::cout << "-> Will add only data points with x < " << vMax_ << std::endl;

  std::ifstream ifs( fileName.c_str() );

  if( !ifs.good() ) {
    std::cout << "[IScan::readFile] ERROR! File '" << fileName << "' does not exist. Please check name." << std::endl;
    exit(2);
  }

  std::string line;

  if( ifs.good() ) {

    bool addToGraph = true; // default: all points

    while( getline(ifs,line) ) {

      // first convert tabs into spaces
      TString line_tstr(line);
      line_tstr.ReplaceAll( "\t", " " );
      line = (std::string)(line_tstr.Data());

      std::string line_clean;
      for( unsigned i=0; i<line.size(); ++i ) {
        int char_i = (int)(line[i]);
        if( char_i > 31 ) { // remove all weird formatting chars
          line_clean.push_back( line[i] );
        }
      } // for
      line = line_clean;

      // then remove double spaces (up to 10):
      for( unsigned ii=0; ii<10; ii++ ) { 
        TString line_tstr_ii(line);
        line_tstr_ii.ReplaceAll( "  ", " " );
        line = (std::string)(line_tstr_ii.Data());
      }

      std::vector<std::string> words = AndCommon::splitString( line, " " );

      if( words.size()<2 ) continue;


      if( line[0] == '#' ) {

        readCommentLine(words);

        continue;

      }
      

      // ignore commented sections:
      std::string line_nocomm = (std::string)(AndCommon::splitString( line, "#" ))[0];
      std::vector<std::string> words_nocomm = AndCommon::splitString( line_nocomm, " " );

      readDataLine( words_nocomm, addToGraph );
      //readDataLine( words, vMin, vMax );


    }  // while get lines

  } // if file good

  std::cout << "-> Added " << graph_->GetN() << " data points." << std::endl;

}



//void IScan::getMeanRMS( std::vector<float> v, float& mean, float& rms ) {
//
//  mean = 0.;
//  rms = 0.;
//
//  if( v.size()<2 ) {
//
//    std::cout << "[IScan::getMeanRMS] Data vector size < 2" << std::endl;
//    exit(2);
//
//  } else if( v.size()==2 ) { // first is mean second is RMS
//
//    mean = v[0];
//    rms  = v[1];
//
//  } else { // just a number of measurements
//
//    for( unsigned i=0; i<v.size(); ++i )
//      mean += v[i];
//    
//    mean /= (float)(v.size());
//
//
//    for( unsigned i=0; i<v.size(); ++i ) 
//      rms += (v[i] - mean)*(v[i] - mean);
//    rms /= (float)(v.size()-1);
//    rms = sqrt(rms);
//
//  }  // if
//
//}

