#include "AndCommon.h"
#include "IVScan.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"
#include "TLegend.h"
#include "TLine.h"






void draw_AvsD( TGraphErrors* graph_anode, TGraphErrors* graph_drain, float xMax, const std::string& legendName, const std::string& saveName, const std::string& label );
TGraphErrors* invertCurrent( TGraphErrors* graph );


int main( int argc, char* argv[] ) {


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();


  for( unsigned i=3; i<=5; ++ i ) {
  //for( unsigned i=3; i<=5; ++ i ) {

    float d(i);

    IVScan ivs_anode(Form("CNTetchedOLD_Strongnew_d%d_20230927"      , i));
    IVScan ivs_drain(Form("CNTetchedOLD_Strongnew_d%d_20230927_drain"  , i));
    //IVScan ivs_anode(Form("CNTArO2Etching_AG_d%d_new"      , i));
    //IVScan ivs_drain(Form("CNTArO2Etching_AG_d%d_new_drain", i));
    //IVScan ivs_drain(Form("CNTArO2Etching_AG_d%d_new_drain_2", i));

    draw_AvsD( ivs_anode.graph(), ivs_drain.graph(), 500+100.*i, Form("d = %.1f mm", d), Form("d%d", i), "CNT strong O_{2} etching" );
    //draw_AvsD( ivs_anode.graph(), ivs_drain.graph(), Form("d = %.1f mm", d), Form("d%d", i), "CNT as grown (no etching)" );

  }

  return 0;

}



void draw_AvsD( TGraphErrors* graph_anode, TGraphErrors* graph_drain, float xMax, const std::string& legendName, const std::string& saveName, const std::string& label ) {

  TCanvas* c1 = new TCanvas( "c1", "", 800, 600 );
  c1->cd();

  int xMin = 0.;
  //int xMax = 500.;

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, -200., 1. );
  h2_axes->SetXTitle( "|#DeltaV(CNT-anode)| [V]" );
  h2_axes->SetYTitle( "I [pA]" );
  h2_axes->Draw();

  TLine* lineZero = new TLine( xMin, 0., xMax, 0. );
  lineZero->Draw("same");

  TLine* lineOne = new TLine( xMin, -1., xMax, -1. );
  lineOne->Draw("same");


  TLegend* legend = new TLegend( 0.17, 0.2, 0.35, 0.4, legendName.c_str() );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);


  TGraphErrors* graph_drain_inv = invertCurrent(graph_drain);

  c1->cd();
 
  graph_anode->SetMarkerStyle(20);
  graph_anode->SetMarkerSize(1.5);
  graph_anode->SetMarkerColor(kGray+3);
  graph_anode->SetLineColor(kGray+3);
  graph_anode->Draw("P same");

  graph_drain_inv->SetMarkerStyle(20);
  graph_drain_inv->SetMarkerSize(1.5);
  graph_drain_inv->SetMarkerColor(46);
  graph_drain_inv->SetLineColor(46);
  graph_drain_inv->Draw("P same");

  legend->AddEntry( graph_anode    , "Anode current", "P" );
  legend->AddEntry( graph_drain_inv, "Drain current (#times -1)", "P" );


  legend->Draw("same");

  gPad->RedrawAxis();

  //TPaveText* label = new TPaveText( 0.17, 0.2, 0.35, 0.4, "brNDC" );
  //label->SetTextSize(0.038);
  //label->SetTextAlign(11);
  //label->SetTextColor(46);
  //label->SetFillColor(0);
  //label->AddText( "CNT As Grown (no etching)" );
  ////label->AddText( ivs.name().c_str() );
  //label->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
  //label->AddText( Form("d = %.1f mm", ivs.d()) );
  //label->Draw("same");

  c1->SaveAs( Form("fe_AvsD_%s.pdf", saveName.c_str()) );

  delete c1;
  delete h2_axes;

}



TGraphErrors* invertCurrent( TGraphErrors* graph ) {

  TGraphErrors* graph_inv = new TGraphErrors(0);
  graph_inv->SetName( Form("inv_%s", graph->GetName()) );

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    double x,y;
    graph->GetPoint( iPoint, x, y );
    graph_inv->SetPoint( iPoint, x, -y );
    graph_inv->SetPointError( iPoint, graph->GetErrorX(iPoint), graph->GetErrorY(iPoint) );

  }

  return graph_inv;

}


