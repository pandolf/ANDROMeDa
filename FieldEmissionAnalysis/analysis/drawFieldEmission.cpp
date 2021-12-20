#include "AmdCommon.h"


#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"



void addPoint( TGraphErrors* gr, float hv, std::vector<float> i_meas );
void getMeanRMS( std::vector<float> v, float& mean, float& rms );


int main() {

  AmdCommon::setStyle();

  TGraphErrors* graph = new TGraphErrors(0);

  addPoint( graph, 1499., { 291.9, 268.2, 270.2, 233.9, 258.2, 242.0, 249.6, 205.1, 263.6, 220.3 } );
  addPoint( graph, 1249., { 37.06, 35.02, 39.59, 45.18, 39.12, 37.03, 37.56, 38.53, 40.54, 41.24 } );
  addPoint( graph, 1002., { 2.171, 2.015, 2.047, 2.114, 2.159, 2.826, 2.992, 2.341, 2.391, 2.618 } );
  addPoint( graph, 1743., { 903.3, 916.8, 964.6, 902.3, 876.1, 931.2, 872.9, 951.6, 890.1 } );


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();


  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., 2000., 10, 0., 950. );
  h2_axes->SetXTitle( "V (V)" );
  h2_axes->SetYTitle( "I (nA)" );
  h2_axes->Draw();

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);
  graph->SetMarkerColor(46);
  graph->SetLineColor  (46);

  graph->Draw("P same");

  c1->SaveAs( "iv.pdf" );

  return 0;

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
