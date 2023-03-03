#include "../interface/IScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"





IScan::IScan( const std::string& name, float scale, float xMin, float xMax ) {

  name_ = AndCommon::removePathAndSuffix(name);

  std::cout << "[IScan] Setting up new IScan with name: " << name_ << std::endl;

  p_ = 0.;
  d_ = -1.;

  graph_ = new TGraphErrors(0);
  graph_->SetName( Form("gr_%s", name_.c_str()) );
  graph_->SetMarkerSize(1.8);
  graph_->SetMarkerStyle(20);


  readFile( getDataFileName(name_), xMin, xMax );

  scaleDataPoints( scale );

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



void IScan::set_graph( TGraphErrors* graph ) {

  graph_ = graph;

}



void IScan::set_p( float p ) {

  p_ = p;

}


void IScan::set_d( float d ) {

  d_ = d;

}


void IScan::setColor( int color ) {

  graph_->SetMarkerColor( color );
  graph_->SetLineColor( color );

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

}


void IScan::readDataLine( const std::vector< std::string >& words, float xMin, float xMax ) {

  float x = std::atof( words[0].c_str() );

  if( x>xMin && x<xMax ) {

    std::vector<float> v_i;
    for( unsigned iw=1; iw<words.size(); ++iw ) {
      if( words[iw][0] == '#' ) break; // ignore from comment onwards
      v_i.push_back( std::atof(words[iw].c_str()) );
    }

    addPointToGraph( x, v_i );

  } // if x

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


void IScan::addPointToGraph( float hv, std::vector<float> i_meas ) {

  float mean, rms;
  getMeanRMS( i_meas, mean, rms );

  int iPoint = this->graph()->GetN();
  this->graph()->SetPoint     ( iPoint, hv, mean );
  this->graph()->SetPointError( iPoint, 1., rms  );

}



void IScan::readFile( const std::string& name, float xMin, float xMax ) {


  std::string fileName = (name=="") ? getDataFileName(name_) : getDataFileName(name);

  std::cout << "-> Opening data file: " << fileName << std::endl;
  if( xMin > -50000. && xMax < 50000 ) std::cout << "-> Will add only data points with " << xMin << " < x < " << xMax << std::endl;

  std::ifstream ifs( fileName.c_str() );

  if( !ifs.good() ) {
    std::cout << "[IScan::readFile] ERROR! File '" << fileName << "' does not exist. Please check name." << std::endl;
    exit(2);
  }

  std::string line;

  if( ifs.good() ) {

    while( getline(ifs,line) ) {

      //TString line_tstr(line);
      //line_tstr.ReplaceAll( "\t", " " );
      //line = (std::string)(line_tstr.Data());
      std::vector<std::string> words = AndCommon::splitString( line, " " );

      if( words.size()<2 ) continue;


      if( line[0] == '#' ) {

        readCommentLine(words);

        continue;

      }

      readDataLine( words, xMin, xMax );


    }  // while get lines

  } // if file good

  std::cout << "-> Added " << graph_->GetN() << " data points." << std::endl;

}



void IScan::getMeanRMS( std::vector<float> v, float& mean, float& rms ) {

  mean = 0.;
  rms = 0.;

  if( v.size()<2 ) {

    std::cout << "[IScan::getMeanRMS] Data vector size < 2" << std::endl;
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

