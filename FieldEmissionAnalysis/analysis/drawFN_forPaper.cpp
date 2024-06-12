#include "AndCommon.h"
#include "IVScanFN.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TLegend.h"




TGraphErrors* correctZeroAndErrors( TGraphErrors* graph );




int main( ) {


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.16);
  style->SetPadRightMargin(0.07);
  style->SetTitleYOffset(1.15);
  style->cd();

  std::vector<int> colors;
  colors.push_back( kBlack );
  colors.push_back( 46 );
  colors.push_back( 38 );

  std::vector<std::string> scanNames;
  scanNames.push_back( "CNTArO2Etching_AsGrown_INRiM_MICa_3Kplate_d0p5_IvsV_anode_90to150V_20231213_2_sweepA" );
  scanNames.push_back( "CNTArO2Etching_N1new_INRiM_MICb_3Kplate_d0p5_IvsV_drain_0to88V_20231130_sweepR" );
  scanNames.push_back( "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR" );

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  float xMin = 0.;
  float xMax = 140.;

  TH2D* h2_axes = new TH2D( "axes_all", "", 10, xMin, xMax, 10, -0.3, 3. );
  h2_axes->GetYaxis()->SetTitleOffset( 1.5 );
  h2_axes->SetXTitle( "V_{cnt} (V)" );
  h2_axes->SetYTitle( "I_{cnt} (pA)" );
  h2_axes->Draw("");

  TLine* line_zero = new TLine( xMin, 0., xMax, 0. );
  line_zero->Draw("same");

  TLegend* legend = new TLegend( 0.2, 0.68, 0.6, 0.9, "T = 3 K" );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  legend->Draw("same");

  for( unsigned i=0; i<scanNames.size(); ++i ) {

    float scale = -1.;
    float iMin = 0.5; // default: compute FN only between 0.5 pA
    float iMax = 1.5; //          and 1.5 pA

    TString name_tstr(scanNames[i]);
    if( name_tstr.Contains("drain") ) scale = +1.;

    IVScanFN* thisScan = new IVScanFN( scanNames[i], scale, iMin, iMax );

    thisScan->set_color(colors[i]);

    //TGraphErrors* thisGraph = thisScan->graph();
    TGraphErrors* thisGraph = correctZeroAndErrors( thisScan->graph() );

    thisGraph->SetLineColor( colors[i] );
    thisGraph->SetMarkerColor( colors[i] );
    thisGraph->SetMarkerStyle( 20+i );
    thisGraph->SetMarkerSize( 0.8 );
    //thisGraph->SetMarkerSize( 1.6 );
    thisGraph->Draw("P same" );

    legend->AddEntry( thisGraph, AndCommon::cuteSampleName(thisScan->sampleName()).c_str(), "PL" );

  }


  gPad->RedrawAxis();

  c1->SaveAs( Form("iv_paper.pdf") );

  return 0;

}


TGraphErrors* correctZeroAndErrors( TGraphErrors* graph ) {


  TGraphErrors* graph_corr = new TGraphErrors( 0 );
  graph_corr->SetName(Form("corr_%s", graph->GetName()));

  int i_first = 0;
  int i_last = 100;
  TString name_tstr(graph->GetName());
  if ( name_tstr.Contains( "sweepR" ) ) {
    i_first = graph->GetN()-100;
    i_last = graph->GetN();
  }


  // temp histo to compute mean and rms easily:
  TH1D* h1_temp = new TH1D( Form("temp_%s", graph->GetName()), "", 100, -1., 1. );

  for( unsigned iPoint=i_first; iPoint<i_last; ++iPoint ) {
    double x, y;
    graph->GetPoint( iPoint, x, y );
    h1_temp->Fill( y );
  }

  float mean = h1_temp->GetMean();
  float rms = h1_temp->GetRMS();

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {
    double x, y;
    graph->GetPoint( iPoint, x, y );
    graph_corr->SetPoint( iPoint, x, y-mean );
    graph_corr->SetPointError( iPoint, 0., rms );
  }

  return graph_corr;

}
