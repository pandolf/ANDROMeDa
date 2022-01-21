#include "AndCommon.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"



std::string getDataFileName( const std::string& dataName );

TGraphErrors* getGraphFromFile( const std::string& fileName );

void addPoint( TGraphErrors* gr, float hv, std::vector<float> i_meas );
void getMeanRMS( std::vector<float> v, float& mean, float& rms );
TGraphErrors* getFNgraph( TGraphErrors* g );




int main( int argc, char* argv[] ) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./drawFieldEmission [dataName]" << std::endl;
    exit(1);
  }

  std::string dataName( argv[1] );
  std::string dataFileName = getDataFileName(dataName);

  AndCommon::setStyle();

  TGraphErrors* graph = getGraphFromFile( dataFileName );

//addPoint( graph, 1499., { 291.9, 268.2, 270.2, 233.9, 258.2, 242.0, 249.6, 205.1, 263.6, 220.3 } );
//addPoint( graph, 1249., { 37.06, 35.02, 39.59, 45.18, 39.12, 37.03, 37.56, 38.53, 40.54, 41.24 } );
//addPoint( graph, 1002., { 2.171, 2.015, 2.047, 2.114, 2.159, 2.826, 2.992, 2.341, 2.391, 2.618 } );
//addPoint( graph, 1743., { 903.3, 916.8, 964.6, 902.3, 876.1, 931.2, 872.9, 951.6, 890.1 } );


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();


  TH2D* h2_axes = new TH2D( "axes", "", 10, 700., 1999., 10, 0., 950. );
  h2_axes->SetXTitle( "-#DeltaV (V)" );
  h2_axes->SetYTitle( "I (nA)" );
  h2_axes->Draw();

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);
  graph->SetMarkerColor(46);
  graph->SetLineColor  (46);

  graph->Draw("P same");

  c1->SaveAs( "iv.pdf" );

  c1->Clear();
  c1->SetLogy();

  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, 700., 1999., 10, 0.1, 2000. );
  h2_axes_log->SetXTitle( "-#DeltaV (V)" );
  h2_axes_log->SetYTitle( "I (nA)" );
  h2_axes_log->Draw();

  graph->Draw("P same");

  c1->SaveAs( "iv_log.pdf" );


  TGraphErrors* gr_fn = getFNgraph( graph );

  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->Clear();

  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, 0.0005, 0.0011, 10, -15., -5. );
  h2_axes_fn->SetXTitle( "1/V (V^{-1})" );
  h2_axes_fn->SetYTitle( "Log(I/V^{2}) (a.u.)" );
  h2_axes_fn->Draw();

  gr_fn->SetMarkerStyle(20);
  gr_fn->SetMarkerSize(1.5);
  gr_fn->SetMarkerColor(kGray+3);
  gr_fn->SetLineColor  (kGray+3);

  gr_fn->Draw("P same");

  TF1* f1_line = new TF1( "line", "[0]+[1]*x" );
  f1_line->SetLineColor(46);
  f1_line->SetLineWidth(2);
  gr_fn->Fit( f1_line );

  c1_fn->SaveAs("fn.pdf");

  float phi = 4.7; // in eV
  float d = 3.; // in mm
  float d_err = 0.5;
  float s = f1_line->GetParameter(1);
  float s_err = f1_line->GetParError(1);
  float gamma = -6.83E6*phi*sqrt(phi)*d/s;
  float gamma_err = sqrt( gamma*gamma/(s*s)*s_err*s_err + gamma*gamma/(d*d)*d_err*d_err );

  std::cout << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
  std::cout << "------------------" << std::endl;


  return 0;

}



std::string getDataFileName( const std::string& dataName ) {

  TString dataName_tstr(dataName);

  std::string dataFileName(dataName);

  if( !(dataName_tstr.BeginsWith("data/")) )
    dataFileName = "data/"+dataFileName;

  if( !(dataName_tstr.EndsWith  (".dat")) )
    dataFileName = dataFileName+".dat";

  return dataFileName;

}



TGraphErrors* getGraphFromFile( const std::string& fileName ) {

  std::cout << "-> Opening data file: " << fileName << std::endl;

  std::ifstream ifs( fileName.c_str() );

  if( !ifs.good() ) return 0;

  std::string line;
  TGraphErrors* graph = new TGraphErrors(0);
  graph->SetName(Form("fe_%s", fileName.c_str()));

  if( ifs.good() ) {

    while( getline(ifs,line) ) {

      if( line[0] == '#' ) continue;

      std::string delimiter = " ";
      size_t pos = 0;
      std::vector<std::string> words;
      std::string word;
      while ((pos = line.find(delimiter)) != std::string::npos) {
        word = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
        words.push_back(word);
      }
      words.push_back(line); // last part

      if( words.size()<2 ) continue;

      float hv = std::atof( words[0].c_str() );

      std::vector<float> v_i;
      for( unsigned iw=1; iw<words.size(); ++iw )
        v_i.push_back( std::atof(words[iw].c_str()) );

      float i_nA, err_i_nA;
      getMeanRMS( v_i, i_nA, err_i_nA );

      int nPoints = graph->GetN();
      graph->SetPoint     ( nPoints, hv, i_nA );
      graph->SetPointError( nPoints, 1., err_i_nA );

    }  // while get lines

  } // if file good

  return graph;

}


void addPoint( TGraphErrors* gr, float hv, std::vector<float> i_meas ) {

  float mean, rms;
  getMeanRMS( i_meas, mean, rms );

  int iPoint = gr->GetN();
  gr->SetPoint     ( iPoint, hv, mean );
  gr->SetPointError( iPoint, 0., rms  );

}



void getMeanRMS( std::vector<float> v, float& mean, float& rms ) {

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



TGraphErrors* getFNgraph( TGraphErrors* g ) {

  TGraphErrors* gr_fn = new TGraphErrors(0);
  gr_fn->SetName( "gr_fn" );

  for( unsigned iPoint=0; iPoint<g->GetN(); ++iPoint ) {

    double i, v;
    g->GetPoint(iPoint, v, i);
    float i_err = g->GetErrorY( iPoint );
    float v_err = 1.;

    gr_fn->SetPoint     ( iPoint, 1./v, TMath::Log( i / (v*v) ) );
    std::cout << 1./v << " " <<  TMath::Log( i / (v*v) ) << std::endl;
    gr_fn->SetPointError( iPoint, v_err/(v*v), i_err/i );

  } // for

  return gr_fn;

}

