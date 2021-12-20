#include "AmdCommon.h"


#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"



void addPoint( TGraphErrors* gr, float hv, std::vector<float> i_meas );
void getMeanRMS( std::vector<float> v, float& mean, float& rms );


int main() {

  AmdCommon::setStyle();

  TGraphErrors* graph = new TGraphErrors(0);

  addPoint( graph, 1499., { 291.9, 268.2, 270.2, 233.9 } );


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
