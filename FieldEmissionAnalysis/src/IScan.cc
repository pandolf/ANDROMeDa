#include "../interface/IScan.h"
#include "interface/AndCommon.h"

#include <iostream>
#include <fstream>

#include <vector>

#include "TString.h"
#include "TMath.h"
#include "TF1.h"





IScan::IScan( const std::string& name ) {

  name_ = AndCommon::removePathAndSuffix(name);

  std::cout << "[IScan] Setting up new IScan with name: " << name_ << std::endl;

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

  TString word0_tstr(words[0]);

  if( word0_tstr.BeginsWith("#p") || (words[0]=="#" && words[1]=="p") ) {
    p_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Pressure p = " << p_ << " mbar" << std::endl;
  }
  if( word0_tstr.BeginsWith("#d") || (words[0]=="#" && words[1]=="d") ) {
    d_ = std::atof(words[words.size()-1].c_str());
    std::cout << "-> Distance d = " << d_ << " mm" << std::endl;
  }

}



void IScan::readFile( const std::string& name ) {


  std::string fileName = (name=="") ? getDataFileName(name_) : getDataFileName(name);

  std::cout << "-> Opening data file: " << fileName << std::endl;

  std::ifstream ifs( fileName.c_str() );

  if( !ifs.good() ) {
    std::cout << "[IScan::readFile] ERROR! File '" << fileName << "' does not exist. Please check name." << std::endl;
    exit(2);
  }

  std::string line;

  if( ifs.good() ) {

    while( getline(ifs,line) ) {

      std::vector<std::string> words = AndCommon::splitString( line, " " );

      if( words.size()<2 ) continue;


      if( line[0] == '#' ) {

        this->readCommentLine(words);

        continue;

      }

      this->readDataLine( words );


    }  // while get lines

  } // if file good


}

