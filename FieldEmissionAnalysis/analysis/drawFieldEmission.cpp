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






int main( int argc, char* argv[] ) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./drawFieldEmission [dataName]" << std::endl;
    exit(1);
  }

  std::string name( argv[1] );

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();

  IVScan ivs( name );


  TGraphErrors* graph = ivs.graph();


  std::string outdir(Form("plots/%s/", ivs.name().c_str()));
  system( Form("mkdir -p %s", outdir.c_str()) );


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

  c1->SaveAs( Form("%s/iv.pdf", outdir.c_str()) );

  c1->Clear();
  c1->SetLogy();

  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, 700., 1999., 10, 0.1, 2000. );
  h2_axes_log->SetXTitle( "-#DeltaV (V)" );
  h2_axes_log->SetYTitle( "I (nA)" );
  h2_axes_log->Draw();

  graph->Draw("P same");

  c1->SaveAs( Form("%s/iv_log.pdf", outdir.c_str()) );


  TGraphErrors* gr_fn = ivs.graphFN();

  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->Clear();


  //TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, 0.0005, 0.0012, 10, IVScan::yMinFN(), IVScan::yMaxFN() );
  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, IVScan::xMinFN(), IVScan::xMaxFN(), 10, IVScan::yMinFN(), IVScan::yMaxFN() );
  h2_axes_fn->SetXTitle( IVScan::xTitleFN().c_str() );
  h2_axes_fn->SetYTitle( IVScan::yTitleFN().c_str() );
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

  TPaveText* pd_text = new TPaveText( 0.2, 0.2, 0.5, 0.3, "brNDC" );
  pd_text->SetFillColor(0);
  pd_text->SetTextSize(0.038);
  pd_text->SetTextColor(kGray+3);
  pd_text->AddText( Form("p = %s mbar", AndCommon::scientific(ivs.p(), 0).c_str()) );
  pd_text->AddText( Form("d = %.1f mm"  , ivs.d()) );
  pd_text->Draw("same");


  c1_fn->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );

  std::cout << std::endl;
  std::cout << "------------------" << std::endl;
  std::cout << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
  std::cout << "------------------" << std::endl;


  TFile* outfile = TFile::Open( Form("%s/graphs.root", outdir.c_str()), "RECREATE" );
  outfile->cd();

  graph->Write();
  gr_fn->Write();

  outfile->Write();
  outfile->Close();


  return 0;

}


