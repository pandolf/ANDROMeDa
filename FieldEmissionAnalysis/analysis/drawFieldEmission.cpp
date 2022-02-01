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






int main( int argc, char* argv[] ) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./drawFieldEmission [dataName]" << std::endl;
    exit(1);
  }

  std::string name( argv[1] );
  std::string dataName = AndCommon::removePathAndSuffix(name);

  AndCommon::setStyle();

  IVScan ivs( dataName );

  ivs.readFile();

  TGraphErrors* graph = ivs.graph();


  std::string outdir(Form("plots/%s/", dataName.c_str()));
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


  TGraphErrors* gr_fn = ivs.getFNgraph();

  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->Clear();

  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, 0.0002, 0.0007, 10, -20., 0. );
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

  std::cout << std::endl;
  std::cout << "Chi^2 / NDF: " << f1_line->GetChisquare() << " / " << f1_line->GetNDF() <<std::endl;
  std::cout << std::endl;

  float phi = 4.7; // in eV
  float d = ivs.d(); // in mm
  float d_err = 0.1; // see logbook_ANDROMeDa entry 24/01/22 for details on why 0.1 mm
  float s = f1_line->GetParameter(1);
  float s_err = f1_line->GetParError(1);
  float gamma = -6.83E6*phi*sqrt(phi)*d/s;
  float gamma_err = sqrt( gamma*gamma/(s*s)*s_err*s_err + gamma*gamma/(d*d)*d_err*d_err );

  TPaveText* gamma_text = new TPaveText( 0.65, 0.8, 0.9, 0.9, "brNDC" );
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


  return 0;

}


