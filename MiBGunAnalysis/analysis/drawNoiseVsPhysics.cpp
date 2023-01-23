#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TLegend.h"




void drawVar( const std::string& saveName, TTree* tree_phys, TTree* tree_noise, const std::string& var, const std::string& axisName, int nBins, float xMin, float xMax );


int main() {

  std::string format = "pdf";

  TFile* file_phys  = TFile::Open( "lite_Run_APDWL_HV380_Fe55_th4_Data_1_19_2023_Ascii.root" );
  TFile* file_noise = TFile::Open( "lite_Run_APDWL_HV380_noiseFe_th4_Data_1_19_2023_Ascii.root" );

  TTree* tree_phys  = (TTree*) file_phys ->Get("treeLite");
  TTree* tree_noise = (TTree*) file_noise->Get("treeLite");

  std::string outdir = "plots/Fe55/";

  system( Form("mkdir -p %s", outdir.c_str()) );

  drawVar( Form( "%s/ampNegRA.%s", outdir.c_str(), format.c_str()), tree_phys, tree_noise, "ampNegRA", "Negative Max Amplitude (RA) (V)", 50, -0.1, 0. );

  return 0;

}


void drawVar( const std::string& saveName, TTree* tree_phys, TTree* tree_noise, const std::string& var, const std::string& axisName, int nBins, float xMin, float xMax ) {

  //TH1D* h1_phys  = new TH1D( Form("h1_%s_phys" , saveName.c_str()), "", nBins, xMin, xMax );
  //TH1D* h1_noise = new TH1D( Form("h1_%s_noise", saveName.c_str()), "", nBins, xMin, xMax );
  TH1D* h1_phys  = new TH1D( "h1_phys" , "", nBins, xMin, xMax );
  TH1D* h1_noise = new TH1D( "h1_noise", "", nBins, xMin, xMax );

  tree_phys ->Project( h1_phys ->GetName(), var.c_str() );
  tree_noise->Project( h1_noise->GetName(), var.c_str() );

  float yMax_axes = 1.3*h1_phys->GetMaximum()/h1_phys->Integral();

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., yMax_axes );
  //TH2D* h2_axes = new TH2D( Form("axes_%s", saveName.c_str()), "", 10, xMin, xMax, 10, 0., yMax_axes );
  h2_axes->SetXTitle( axisName.c_str() );
  h2_axes->SetYTitle( "Normalized to Unity" );
  h2_axes->Draw();

  h1_phys ->SetLineWidth(2);
  h1_phys ->SetLineColor(46);

  h1_noise->SetLineWidth(2);
  h1_noise->SetLineColor(38);

  h1_phys ->DrawNormalized("same");
  h1_noise->DrawNormalized("same");

  gPad->RedrawAxis();

  c1->SaveAs( saveName.c_str() );

  delete h1_phys;
  delete h1_noise;

}
