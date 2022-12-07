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





void addCrossingPoint( TGraphErrors* gr_crossing, float graph_x, TGraphErrors* graph, float crossingPoint );



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


  std::string batchName(argv[1]);
  std::string sampleName = "CNT as grown (no etching)";

  float xMax = 2200.;
  float xMax_E = 500.;
  float yMax_cross = 0.;
  std::vector< std::string > scans;

  if( argc > 1 ) {

    if( batchName=="CNTArO2Etching_AG" ) {

      sampleName = "CNT as grown (no etching)";
      xMax = 2200.;
      xMax_E = 500.;
      yMax_cross = 2500.;
      scans.push_back( "CNTArO2Etching_AG_d3_new" );
      scans.push_back( "CNTArO2Etching_AG_d4_new" );
      scans.push_back( "CNTArO2Etching_AG_d5_new" );

    } else if( batchName=="CNTArO2Etching_N1" ) {

      sampleName = "Mild Ar/O_{2} Etching";
      xMax = 600.;
      xMax_E = 200.;
      yMax_cross = 700.;
      scans.push_back( "CNTArO2Etching_N1_d2_20221130" );
      scans.push_back( "CNTArO2Etching_N1_d3_20221130" );
      scans.push_back( "CNTArO2Etching_N1_d4_20221130" );
      scans.push_back( "CNTArO2Etching_N1_d5_20221130" );

    } else {

      scans.push_back( std::string(argv[1]) );

    }

  } else {

    std::cout << "No default argument, exiting." << std::endl;
    std::cout << "USAGE: ./drawFieldEmission_forTES [name]" << std::endl;
    exit(12);

  }


  std::vector<int> colors = AndCommon::colors();


  TCanvas* c1 = new TCanvas( "c1", "", 800, 600 );
  c1->cd();

  float xMin = 0.;

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, -3., 0.6 );
  h2_axes->SetXTitle( "-#DeltaV(CNT-anode) [V]" );
  h2_axes->SetYTitle( "I(anode) [pA]" );
  h2_axes->Draw();

  TLine* lineZero = new TLine( xMin, 0., xMax, 0. );
  lineZero->Draw("same");

  TLine* lineOne = new TLine( xMin, -1., xMax, -1. );
  lineOne->SetLineStyle(2);
  lineOne->Draw("same");


  TCanvas* c1_E = new TCanvas( "c1_E", "", 800, 600 );
  c1_E->cd();





  TH2D* h2_axes_E = new TH2D( "axes_E", "", 10, xMin, xMax_E, 10, -3., 0.6 );
  h2_axes_E->SetXTitle( "E [V/mm]" );
  h2_axes_E->SetYTitle( "I(anode) [pA]" );
  h2_axes_E->Draw();

  TLine* lineZero_E = new TLine( xMin, 0., xMax_E, 0. );
  lineZero_E->Draw("same");

  TLine* lineOne_E = new TLine( xMin, -1., xMax_E, -1. );
  lineOne_E->SetLineStyle(2);
  lineOne_E->Draw("same");



  TLegend* legend = new TLegend( 0.17, 0.37-0.05*scans.size(), 0.35, 0.42, sampleName.c_str() );
  legend->SetFillColor(0);
  legend->SetTextSize(0.035);
  
  TLegend* legend_E = new TLegend( 0.17, 0.37-0.05*scans.size(), 0.35, 0.42, sampleName.c_str() );
  legend_E->SetFillColor(0);
  legend_E->SetTextSize(0.035);
  

  TGraphErrors* gr_crossing_1p  = new TGraphErrors(0);
  TGraphErrors* gr_crossing_02p = new TGraphErrors(0);

  gr_crossing_1p ->SetName( "gr_crossing_1p"  );
  gr_crossing_02p->SetName( "gr_crossing_02p" );


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

    addCrossingPoint( gr_crossing_1p , ivs->d(), graph, -1.0 );
    addCrossingPoint( gr_crossing_02p, ivs->d(), graph, -0.2 );


    c1_E->cd();

    TGraphErrors* graph_vsE = ivs->graph_vsE();

    graph_vsE->SetMarkerStyle(20);
    graph_vsE->SetMarkerSize(1.5);
    graph_vsE->SetMarkerColor(colors[i]);
    graph_vsE->SetLineColor(colors[i]);
    graph_vsE->Draw("P same");

    legend_E->AddEntry( graph_vsE, Form("d = %.1f mm", ivs->d()) );


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

  c1->SaveAs( Form("plots/fe_forTES_%s.pdf", batchName.c_str()) );


  c1_E->cd();

  legend_E->Draw("same");

  gPad->RedrawAxis();

  c1_E->SaveAs( Form("plots/fe_vsE_forTES_%s.pdf", batchName.c_str()) );


  AndCommon::setStyle();

  TCanvas* c1_cross = new TCanvas( "c1_cross", "", 600, 600 );
  c1_cross->cd();

  float xMax_cross = 6.;

  TH2D* h2_axes_crossing = new TH2D( "axes_crossing", "", 10, 0., xMax_cross, 10, 0., yMax_cross );
  h2_axes_crossing->SetXTitle( "d(anode-cathode) [mm]" );
  h2_axes_crossing->SetYTitle( "#DeltaV [V]" );
  h2_axes_crossing->Draw();

  gr_crossing_1p->SetMarkerStyle( 20 );
  gr_crossing_1p->SetMarkerSize( 1.7 );
  gr_crossing_1p->SetMarkerColor( 46 );
  gr_crossing_1p->SetLineColor  ( 46 );

  gr_crossing_02p->SetMarkerStyle( 24 );
  gr_crossing_02p->SetMarkerSize( 1.7 );
  gr_crossing_02p->SetMarkerColor( 46 );
  gr_crossing_02p->SetLineColor  ( 46 );

  TF1* f1_crossFit = new TF1( "crossFit", "[0] + [1]*x", 0., 6. );
  f1_crossFit->SetLineColor(46);
  f1_crossFit->SetLineWidth(1);
  gr_crossing_1p->Fit( f1_crossFit, "R" );

  TLine* line_100V = new TLine( 0., 100., xMax_cross, 100. );
  line_100V->Draw("same");

  std::cout << "Crossing fit: f(2 mm) = " << f1_crossFit->Eval(2.) << " V" << std::endl;
  std::cout << "Crossing fit: f(1 mm) = " << f1_crossFit->Eval(1.) << " V" << std::endl;
  std::cout << "Crossing fit: 100V @ d = " << f1_crossFit->GetX(100.) << " mm " << std::endl;


  gr_crossing_1p ->Draw("Psame");
  //gr_crossing_02p->Draw("Psame");

  TLegend* legend_cross = new TLegend( 0.2, 0.7, 0.5, 0.9 );
  legend_cross->SetFillColor(0);
  legend_cross->SetTextSize(0.035);
  legend_cross->AddEntry( gr_crossing_1p , "I = 1 pA", "PL" );
  legend_cross->AddEntry( gr_crossing_02p, "I = 0.2 pA", "P" );
  //legend_cross->Draw();


  gPad->RedrawAxis();

  c1_cross->SaveAs( Form("plots/fe_crossing_vs_d_%s.pdf", batchName.c_str()) );


  delete c1;
  delete legend;

  return 0;

}



void addCrossingPoint( TGraphErrors* gr_crossing, float graph_x, TGraphErrors* graph, float crossingPoint ) {

  double old_x(0.), old_y(0.);

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    double x, y;
    graph->GetPoint( iPoint, x, y );

    if( old_y>crossingPoint && y<crossingPoint ) {

      int crossN = gr_crossing->GetN();
      gr_crossing->SetPoint     ( crossN, graph_x, 0.5*(old_x+x) );
      gr_crossing->SetPointError( crossN,      0., fabs(old_x-x) );

      break;

    } else {

      old_x = x;
      old_y = y;

    }

  } // for points

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
