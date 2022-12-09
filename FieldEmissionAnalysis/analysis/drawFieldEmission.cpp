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




IVScanFN analyzeFN( const std::string& name );



int main( int argc, char* argv[] ) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./drawFieldEmission [dataName]" << std::endl;
    exit(1);
  }

  std::string name( AndCommon::removePathAndSuffix(argv[1]) );

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();


  std::vector< IVScanFN > scans;

  if( name == "CNTArO2Etching_N1" ) {

    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d5_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d4_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d3_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d2_20221130" ) );

  } else {

    scans.push_back( analyzeFN(name) );

  }
    

  return 0;

}



IVScanFN analyzeFN( const std::string& name ) {


  float hvMin = -1.;
  float hvMax = -1.;
  float scale = 1.;

  if( name=="CNTArO2Etching_N1_d5_20221130" ) {

    hvMin = 480.;
    hvMax = 560.;
    scale = -1.;

  } else if( name=="CNTArO2Etching_N1_d4_20221130" ) {

    hvMin = 385.;
    hvMax = 440.;
    scale = -1.;

  } else if( name=="CNTArO2Etching_N1_d3_20221130" ) {

    hvMin = 280.;
    hvMax = 350.;
    scale = -1.;

  } else if( name=="CNTArO2Etching_N1_d2_20221130" ) {

    hvMin = 175.;
    hvMax = 350.;
    scale = -1.;

  } else if( name=="CNTArO2Etching_AG_d5_new" ) {

    hvMin = 1930.;
    hvMax = 2100.;
    scale = -1.;

  } else if( name=="CNTArO2Etching_AG_d4_new" ) {

    hvMin = 1500.;
    hvMax = 1800.;
    scale = -1.;

  } else if( name=="CNTArO2Etching_AG_d3_new" ) {

    hvMin = 1110.;
    hvMax = 1230.;
    scale = -1.;

  }


  IVScanFN ivs( name, scale, hvMin, hvMax );

  

  TGraphErrors* graph = ivs.graph();

  if( graph->GetN()==0 ) {
    std::cout << "-> ERROR! No data points in graph. Check the data file or change hvMin and hvMax to select more points." << std::endl;
    exit(1);
  }


  std::string outdir(Form("plots/%s/", ivs.name().c_str()));
  system( Form("mkdir -p %s", outdir.c_str()) );


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  //float xMin = 1300.;
  //float xMax = 2999.;
  float xMin = 0.9*hvMin;
  float xMax = 1.1*hvMax;

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., 30. );
  h2_axes->SetXTitle( "-#DeltaV (V)" );
  h2_axes->SetYTitle( "I (pA)" );
  h2_axes->Draw();

  TPaveText* pd_text = new TPaveText( 0.2, 0.75, 0.5, 0.85, "brNDC" );
  pd_text->SetFillColor(0);
  pd_text->SetTextSize(0.038);
  pd_text->SetTextColor(kGray+3);
  pd_text->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
  pd_text->AddText( Form("d = %.1f mm"  , ivs.d()) );
  pd_text->Draw("Same");

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);
  graph->SetMarkerColor(46);
  graph->SetLineColor  (46);

  graph->Draw("P same");

  c1->SaveAs( Form("%s/iv.pdf", outdir.c_str()) );

  c1->Clear();
  c1->SetLogy();

  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, xMin, xMax, 10, 0.02, 20. );
  h2_axes_log->SetXTitle( "-#DeltaV (V)" );
  h2_axes_log->SetYTitle( "I (nA)" );
  h2_axes_log->Draw();

  pd_text->Draw("Same");
  graph->Draw("P same");

  c1->SaveAs( Form("%s/iv_log.pdf", outdir.c_str()) );


  TGraphErrors* gr_fn = ivs.graphFN();

  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->Clear();


  float xMinFN = 1./hvMax;
  float xMaxFN = 1./hvMin;

  //TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, 0.0005, 0.0012, 10, IVScanFN::yMinFN(), IVScanFN::yMaxFN() );
  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, xMinFN, xMaxFN, 10, IVScanFN::yMinFN(), IVScanFN::yMaxFN() );
  h2_axes_fn->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axes_fn->SetYTitle( IVScanFN::yTitleFN().c_str() );
  h2_axes_fn->Draw();

  gr_fn->SetMarkerStyle(20);
  gr_fn->SetMarkerSize(1.5);
  gr_fn->SetMarkerColor(kGray+3);
  gr_fn->SetLineColor  (kGray+3);

  gr_fn->Draw("P same");

  std::cout << std::endl;
  std::cout << "Chi^2 / NDF: " << ivs.lineFN()->GetChisquare() << " / " << ivs.lineFN()->GetNDF() <<std::endl;
  std::cout << std::endl;

  float gamma = ivs.gamma();
  float gamma_err = ivs.gamma_err();

  TPaveText* gamma_text = new TPaveText( 0.6, 0.8, 0.85, 0.9, "brNDC" );
  gamma_text->SetFillColor(0);
  gamma_text->SetTextSize(0.038);
  gamma_text->SetTextColor( 46 );
  //gamma_text->SetTextColor( kGray+3 );
  gamma_text->AddText( Form("#gamma = %.0f #pm %.0f", gamma, gamma_err) );
  gamma_text->Draw("same");

  TPaveText* pd_text_log = new TPaveText( 0.2, 0.2, 0.5, 0.3, "brNDC" );
  pd_text_log->SetFillColor(0);
  pd_text_log->SetTextSize(0.038);
  pd_text_log->SetTextColor(kGray+3);
  pd_text_log->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
  pd_text_log->AddText( Form("d = %.1f mm"  , ivs.d()) );
  pd_text_log->Draw("same");


  c1_fn->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );

  std::cout << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << std::endl << std::endl;


  std::string graphsFileName(Form("%s/graphs.root", outdir.c_str()));

  TFile* outfile = TFile::Open( graphsFileName.c_str(), "RECREATE" );
  outfile->cd();

  graph->Write();
  gr_fn->Write();

  outfile->Write();
  outfile->Close();

  std::cout << "-> Stored graphs in: " << graphsFileName << std::endl;


  return ivs;

}


