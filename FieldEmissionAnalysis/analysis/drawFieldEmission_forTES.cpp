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




int main( int argc, char* argv[] ) {

//if( argc < 2 ) {
//  std::cout << "USAGE: ./drawFieldEmission_forTES [dataName]" << std::endl;
//  exit(1);
//}

//std::string name( argv[1] );

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();


  std::string sampleName = "CNT as grown (no etching)";

  std::vector< std::string > scans;
  scans.push_back( "CNTArO2Etching_AG_d3_new" );
  scans.push_back( "CNTArO2Etching_AG_d4_new" );
  scans.push_back( "CNTArO2Etching_AG_d5_new" );

  std::vector<int> colors = AndCommon::colors();


  TCanvas* c1 = new TCanvas( "c1", "", 800, 600 );
  c1->cd();

  int xMin = 0.;
  int xMax = 2200.;

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, -3., 1. );
  h2_axes->SetXTitle( "-#DeltaV(CNT-anode) [V]" );
  h2_axes->SetYTitle( "I(anode) [pA]" );
  h2_axes->Draw();

  TLine* lineZero = new TLine( xMin, 0., xMax, 0. );
  lineZero->Draw("same");

  TLine* lineOne = new TLine( xMin, -1., xMax, -1. );
  lineOne->Draw("same");


  TCanvas* c1_E = new TCanvas( "c1_E", "", 800, 600 );
  c1_E->cd();




  int xMax_E = 500.;

  TH2D* h2_axes_E = new TH2D( "axes_E", "", 10, xMin, xMax_E, 10, -3., 1. );
  h2_axes_E->SetXTitle( "E [V/mm]" );
  h2_axes_E->SetYTitle( "I(anode) [pA]" );
  h2_axes_E->Draw();

  TLine* lineZero_E = new TLine( xMin, 0., xMax_E, 0. );
  lineZero_E->Draw("same");

  TLine* lineOne_E = new TLine( xMin, -1., xMax_E, -1. );
  lineOne_E->SetLineStyle(2);
  lineOne_E->Draw("same");



  TLegend* legend = new TLegend( 0.17, 0.2, 0.35, 0.4, sampleName.c_str() );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  
  TLegend* legend_E = new TLegend( 0.17, 0.2, 0.35, 0.4, sampleName.c_str() );
  legend_E->SetFillColor(0);
  legend_E->SetTextSize(0.035);
  


  for ( unsigned i=0; i<scans.size(); ++i ) {

    IVScan* ivs = new IVScan( scans[i] );

    TGraphErrors* graph = ivs->graph();

    c1->cd();
 
    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);
    graph->SetMarkerColor(colors[i]);
    graph->SetLineColor(colors[i]);
    graph->Draw("P same");

    legend->AddEntry( graph, Form("d = %.1f mm", ivs->d()) );


    c1_E->cd();

    TGraphErrors* graph_E = ivs->graph_E();

    graph_E->SetMarkerStyle(20);
    graph_E->SetMarkerSize(1.5);
    graph_E->SetMarkerColor(colors[i]);
    graph_E->SetLineColor(colors[i]);
    graph_E->Draw("P same");

    legend_E->AddEntry( graph_E, Form("d = %.1f mm", ivs->d()) );


    delete ivs;

  }

  c1->cd();
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

  c1->SaveAs("fe_forTES.pdf");


  c1_E->cd();

  legend_E->Draw("same");

  gPad->RedrawAxis();

  c1_E->SaveAs("feE_forTES.pdf");

  delete c1;
  delete legend;

  return 0;

}

//
//
//
//  std::string outdir(Form("plots/%s/", ivs.name().c_str()));
//  system( Form("mkdir -p %s", outdir.c_str()) );
//
//
//  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
//  c1->cd();
//
//  float xMin = 1300.;
//  float xMax = 2999.;
//
//  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., 30000. );
//  h2_axes->SetXTitle( "-#DeltaV (V)" );
//  h2_axes->SetYTitle( "I (nA)" );
//  h2_axes->Draw();
//
//  TPaveText* pd_text = new TPaveText( 0.2, 0.75, 0.5, 0.85, "brNDC" );
//  pd_text->SetFillColor(0);
//  pd_text->SetTextSize(0.038);
//  pd_text->SetTextColor(kGray+3);
//  pd_text->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
//  pd_text->AddText( Form("d = %.1f mm"  , ivs.d()) );
//  pd_text->Draw("Same");
//
//  graph->SetMarkerStyle(20);
//  graph->SetMarkerSize(1.5);
//  graph->SetMarkerColor(46);
//  graph->SetLineColor  (46);
//
//  graph->Draw("P same");
//
//  c1->SaveAs( Form("%s/iv.pdf", outdir.c_str()) );
//
//  c1->Clear();
//  c1->SetLogy();
//
//  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, xMin, xMax, 10, 2, 200000. );
//  h2_axes_log->SetXTitle( "-#DeltaV (V)" );
//  h2_axes_log->SetYTitle( "I (nA)" );
//  h2_axes_log->Draw();
//
//  pd_text->Draw("Same");
//  graph->Draw("P same");
//
//  c1->SaveAs( Form("%s/iv_log.pdf", outdir.c_str()) );
//
//
//  TGraphErrors* gr_fn = ivs.graphFN();
//
//  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
//  c1_fn->Clear();
//
//
//  //TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, 0.0005, 0.0012, 10, IVScan::yMinFN(), IVScan::yMaxFN() );
//  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, IVScan::xMinFN(), IVScan::xMaxFN(), 10, IVScan::yMinFN(), IVScan::yMaxFN() );
//  h2_axes_fn->SetXTitle( IVScan::xTitleFN().c_str() );
//  h2_axes_fn->SetYTitle( IVScan::yTitleFN().c_str() );
//  h2_axes_fn->Draw();
//
//  gr_fn->SetMarkerStyle(20);
//  gr_fn->SetMarkerSize(1.5);
//  gr_fn->SetMarkerColor(kGray+3);
//  gr_fn->SetLineColor  (kGray+3);
//
//  gr_fn->Draw("P same");
//
//  std::cout << std::endl;
//  std::cout << "Chi^2 / NDF: " << ivs.lineFN()->GetChisquare() << " / " << ivs.lineFN()->GetNDF() <<std::endl;
//  std::cout << std::endl;
//
//  float gamma = ivs.gamma();
//  float gamma_err = ivs.gamma_err();
//
//  TPaveText* gamma_text = new TPaveText( 0.6, 0.8, 0.85, 0.9, "brNDC" );
//  gamma_text->SetFillColor(0);
//  gamma_text->SetTextSize(0.038);
//  gamma_text->SetTextColor( 46 );
//  //gamma_text->SetTextColor( kGray+3 );
//  gamma_text->AddText( Form("#gamma = %.0f #pm %.0f", gamma, gamma_err) );
//  gamma_text->Draw("same");
//
//  TPaveText* pd_text_log = new TPaveText( 0.2, 0.2, 0.5, 0.3, "brNDC" );
//  pd_text_log->SetFillColor(0);
//  pd_text_log->SetTextSize(0.038);
//  pd_text_log->SetTextColor(kGray+3);
//  pd_text_log->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
//  pd_text_log->AddText( Form("d = %.1f mm"  , ivs.d()) );
//  pd_text_log->Draw("same");
//
//
//  c1_fn->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );
//
//  std::cout << std::endl;
//  std::cout << "------------------" << std::endl;
//  std::cout << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
//  std::cout << "------------------" << std::endl;
//
//
//  TFile* outfile = TFile::Open( Form("%s/graphs.root", outdir.c_str()), "RECREATE" );
//  outfile->cd();
//
//  graph->Write();
//  gr_fn->Write();
//
//  outfile->Write();
//  outfile->Close();
//
//
//  return 0;
//
//}
//
//
