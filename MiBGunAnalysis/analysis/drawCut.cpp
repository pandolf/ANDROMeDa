#include <iostream>
#include <stdlib.h>

#include "interface/AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"





int main() {

  AndCommon::setStyle();

  TFile* file = TFile::Open( "lite_Run_APDWL_HV380_egun15keV_th7_Data_1_19_2023_Ascii.root" );
  TTree* tree = (TTree*)file->Get("treeLite");

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH1D* h1_beforeCut = new TH1D( "beforeCut", "", 100, 0., 20. );
  TH1D* h1_afterCut  = new TH1D( "afterCut", "", 100, 0., 20. );

  tree->Project( h1_beforeCut->GetName(), "ampRA", "" );
  tree->Project( h1_afterCut ->GetName(), "ampRA", "ampNegRA>-2.8" );

  h1_beforeCut->SetXTitle( "Max Amplitude (RA) (mV)" );
  h1_beforeCut->SetYTitle( "Entries" );

  h1_beforeCut->SetLineColor(38);
  h1_beforeCut->SetLineWidth(3);

  h1_afterCut->SetLineColor(46);
  h1_afterCut->SetFillColor(46);
  h1_afterCut->SetFillStyle(3004);
  h1_afterCut->SetLineWidth(3);

  TLegend* legend = new TLegend( 0.55, 0.6, 0.9, 0.9, "15 keV e-gun data" );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);
  legend->AddEntry( h1_beforeCut, "Full distribution", "L" );
  legend->AddEntry( h1_afterCut , "ampNeg > -2.8 mV", "F" );

  h1_beforeCut->Draw();
  h1_afterCut->Draw("same");

  legend->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("provaCut.pdf");

  return 0;

}
