#include <iostream>
#include <stdlib.h>

#include "interface/AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TRandom3.h"





int main() {

  AndCommon::setStyle();


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH1D* h1_now10  = new TH1D( "now10", "", 100, 0., 12. );
  TH1D* h1_now20  = new TH1D( "now20", "", 100, 0., 12. );

  TH1D* h1_then10  = new TH1D( "then10", "", 100, 0., 12. );
  TH1D* h1_then20  = new TH1D( "then20", "", 100, 0., 12. );

  TRandom3* random = new TRandom3(1);

  for(unsigned i=0; i<10000; ++i ) {

    h1_now10 ->Fill( random->Gaus( 6., 1. ) );
    h1_now20 ->Fill( random->Gaus( 8., 1. ) );

    h1_then10->Fill( random->Gaus( 6., 1./sqrt(10.) ) );
    h1_then20->Fill( random->Gaus( 8., 1./sqrt(10.) ) );

  }

  c1->cd();

  h1_now20->SetXTitle( "Max Amplitude (RA) (mV)" );
  h1_now20->SetYTitle( "Entries" );

  h1_now10->SetLineColor(38);
  h1_now10->SetFillColor(38);
  h1_now10->SetLineWidth(3);
  h1_now10->SetFillStyle(3004);

  h1_now20->SetLineColor(46);
  h1_now20->SetFillColor(46);
  h1_now20->SetLineWidth(3);
  h1_now20->SetFillStyle(3004);

  TLegend* legend = new TLegend( 0.2, 0.7, 0.55, 0.9, "Simulation (present)" );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);
  legend->AddEntry( h1_now10, "10 keV", "F" );
  legend->AddEntry( h1_now20, "20 keV", "F" );

  h1_now20->Draw();
  legend->Draw("Same");
  h1_now10->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("provaSim1020_present.pdf");

  TCanvas* c2 = new TCanvas( "c2", "", 600, 600 );
  c2->cd();

  h1_then20->SetXTitle( "Max Amplitude (RA) (mV)" );
  h1_then20->SetYTitle( "Entries" );

  h1_then10->SetLineColor(38);
  h1_then10->SetFillColor(38);
  h1_then10->SetLineWidth(3);
  h1_then10->SetFillStyle(3004);

  h1_then20->SetLineColor(46);
  h1_then20->SetFillColor(46);
  h1_then20->SetLineWidth(3);
  h1_then20->SetFillStyle(3004);

  TLegend* legend2 = new TLegend( 0.2, 0.7, 0.55, 0.9, "Simulation (future)" );
  legend2->SetTextSize(0.035);
  legend2->SetFillColor(0);
  legend2->AddEntry( h1_then10, "10 keV", "F" );
  legend2->AddEntry( h1_then20, "20 keV", "F" );

  h1_then20->Draw();
  legend2->Draw("Same");
  h1_then10->Draw("same");

  gPad->RedrawAxis();

  c2->SaveAs("provaSim1020_future.pdf");

  return 0;

}
