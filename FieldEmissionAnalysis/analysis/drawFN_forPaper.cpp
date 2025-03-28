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
#include "TPaveText.h"




TGraphErrors* correctZeroAndErrors( TGraphErrors* graph );
void setFunctionRange( TGraphErrors* graph, TF1* line );




int main( ) {


  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.16);
  style->SetPadRightMargin(0.1);
  style->SetPadTopMargin(0.06);
  style->SetPadBottomMargin(0.17);
  style->SetTitleYOffset(1.15);
  style->cd();

  std::vector<int> colors;
  //colors.push_back( kBlack );
  colors.push_back( 46 );
  colors.push_back( 92 );
  colors.push_back( 38 );

  std::vector<std::string> scanNames;
  scanNames.push_back( "CNTArO2Etching_AsGrown_INRiM_MICa_3Kplate_d0p5_IvsV_anode_90to150V_20231213_2_sweepA" );
  scanNames.push_back( "CNTArO2Etching_N1new_INRiM_MICb_3Kplate_d0p5_IvsV_drain_0to88V_20231130_sweepR" );
  scanNames.push_back( "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR" );

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  float xMin = 0.;
  float xMax = 135.;

  //TH2D* h2_axes = new TH2D( "axes_all", "", 10, xMin, xMax, 10, -0.5, 7.);
  TH2D* h2_axes = new TH2D( "axes_all", "", 10, xMin, xMax, 10, -0.2, 1.6 );
  h2_axes->GetYaxis()->SetTitleOffset( 1.5 );
  //h2_axes->SetXTitle( "V_{cnt} (V)" );
  //h2_axes->SetYTitle( "I_{cnt} (pA)" );
  h2_axes->SetXTitle( "V (V)" );
  h2_axes->SetYTitle( "I (pA)" );
  h2_axes->Draw("");

  TLine* line_zero = new TLine( xMin, 0., xMax, 0. );
  line_zero->Draw("same");

  TPaveText* label = new TPaveText( 0.19, 0.85, 0.6, 0.9, "brNDC" );
  label->SetFillColor(0);
  label->SetTextAlign(12);
  label->SetTextSize(0.038);
  label->AddText( "T = 2.8 K" );
  //label->AddText( "0.56 #leq d #leq 0.62 mm" );
  label->Draw("same");


  TLegend* legend = new TLegend( 0.2, 0.7, 0.6, 0.84 );
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.035);
  legend->SetTextFont(42);
  legend->Draw("same");

  

  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->cd();

  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, 0.004, 0.017, 10, -11., -5.51 );
  h2_axes_fn->GetYaxis()->SetTitleOffset( 1.5 );
  //h2_axes_fn->SetXTitle( "1/V_{cnt} (V^{-1})" );
  //h2_axes_fn->SetYTitle( "Log(I_{cnt}/V_{cnt}^{2}) (a.u.)" );
  h2_axes_fn->SetXTitle( "1/V (V^{-1})" );
  h2_axes_fn->SetYTitle( "Log(I/V^{2}) (a.u.)" );
  h2_axes_fn->GetXaxis()->SetMaxDigits(2);
  h2_axes_fn->GetYaxis()->SetMaxDigits(2);
  h2_axes_fn->Draw("");

  label->Draw("same");
  legend->Draw("same");

  for( unsigned i=0; i<scanNames.size(); ++i ) {

    std::string ofsName(Form("plots/%s/gamma.dat", scanNames[i].c_str()));
    std::ofstream ofs(ofsName.c_str());

    float scale = -1.;
    float iMin = 0.5; //(scanNames[i]=="CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR") ? 0.3 : 0.5; // default: compute FN only between 0.5 pA
    float iMax = 7.;
    if( scanNames[i] == "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR" )
      iMax = 1.5;
    else if( scanNames[i] == "CNTArO2Etching_N1new_INRiM_MICb_3Kplate_d0p5_IvsV_drain_0to88V_20231130_sweepR" )
      iMax = 2.;

    TString name_tstr(scanNames[i]);
    if( name_tstr.Contains("drain") ) scale = +1.;

    IVScanFN* thisScan = new IVScanFN( scanNames[i], scale, iMin, iMax );

    thisScan->set_color(colors[i]);

    //TGraphErrors* thisGraph = thisScan->graph();
    TGraphErrors* thisGraph = correctZeroAndErrors( thisScan->graph() );

    c1->cd();

    thisGraph->SetLineColor( colors[i] );
    //thisGraph->SetMarkerColor( kPink );
    thisGraph->SetMarkerColor( colors[i] );
    thisGraph->SetMarkerStyle( 20+i );
    thisGraph->SetMarkerSize( 0.8 );
    //thisGraph->SetMarkerSize( 1.6 );
    thisGraph->Draw("P same" );

    legend->AddEntry( thisGraph, AndCommon::cuteSampleName(thisScan->sampleName()).c_str(), "PL" );

    c1_fn->cd();

    TGraphErrors* gr_fn = thisScan->graphFN();
    TF1* lineFN = gr_fn->GetFunction(Form("lineFN_%s", gr_fn->GetName()));
    setFunctionRange( gr_fn, lineFN );

    gr_fn->SetLineColor( colors[i] );
    gr_fn->SetMarkerColor( colors[i] );
    gr_fn->SetMarkerStyle( 20+i );
    gr_fn->SetMarkerSize( 0.8 );
    gr_fn->Draw("psame");
    lineFN->SetLineWidth(1);
    lineFN->Draw("same");

    float gamma_err = -1.;
    float gamma = -1.;
    if( scanNames[i] == "CNTArO2Etching_AsGrown_INRiM_MICa_3Kplate_d0p5_IvsV_anode_90to150V_20231213_2_sweepA" )
      gamma = thisScan->get_gamma_and_err( gamma_err, lineFN->GetParameter(1), lineFN->GetParError(1) ); // , 0.612, 0.032 );
    else if( scanNames[i] == "CNTArO2Etching_N1new_INRiM_MICb_3Kplate_d0p5_IvsV_drain_0to88V_20231130_sweepR" )
      gamma = thisScan->get_gamma_and_err( gamma_err, lineFN->GetParameter(1), lineFN->GetParError(1) ); // , 0.562, 0.032 );
    else if( scanNames[i] == "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR" )
      gamma = thisScan->get_gamma_and_err( gamma_err, lineFN->GetParameter(1), lineFN->GetParError(1) ); // , 0.617, 0.046 );

    std::cout << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << " " << thisScan->sampleName() << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
    std::cout << "---------------------------------" << std::endl;
    std::cout << std::endl << std::endl;
 
    ofs << gamma << " " << gamma_err << std::endl;

    std::cout << " -> Saved gamma in: " << ofsName << std::endl;
    ofs.close();

  }


  c1->cd();
  gPad->RedrawAxis();
  c1->SaveAs( Form("iv_paper.pdf") );


  c1_fn->cd();
  gPad->RedrawAxis();
  c1_fn->SaveAs( Form("fn_paper.pdf") );

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



void setFunctionRange( TGraphErrors* graph, TF1* line ) {

  double xMin=9999., xMax=-9999.;

  for( unsigned iPoint=0; iPoint<graph->GetN(); ++iPoint ) {

    double x,y;
    graph->GetPoint( iPoint, x, y );
    if( x<xMin ) xMin=x;
    if( x>xMax ) xMax=x;

  }

  line->SetRange( 0.985*xMin, 1.02*xMax );

}
