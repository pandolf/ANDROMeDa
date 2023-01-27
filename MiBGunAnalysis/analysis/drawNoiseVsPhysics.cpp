#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TLegend.h"




void drawVar( const std::string& outdir, const std::string& suffix, TTree* tree_phys, TTree* tree_noise, const std::string& var, const std::string& selection, const std::string& legendTitle, const std::string& axisName, int nBins, float xMin, float xMax );


int main( int argc, char* argv[] ) {

  AndCommon::setStyle();

  std::string run(argv[1]);

  std::string fileName_phys ;
  std::string fileName_noise;
  std::string legendTitle;

  if( run == "Fe55" ) {

    fileName_phys  = "lite_Run_APDWL_HV380_Fe55_th4_Data_1_19_2023_Ascii.root";
    fileName_noise = "lite_Run_APDWL_HV380_noiseFe_th4_Data_1_19_2023_Ascii.root";

    legendTitle = "^{55}Fe data";

  } else if( run == "egun15keV" ) {

    fileName_phys  = "lite_Run_APDWL_HV380_egun15keV_th7_Data_1_19_2023_Ascii.root";
    fileName_noise = "lite_Run_APDWL_HV380_noise_HV15on_th7_Data_1_19_2023_Ascii.root";
   
    legendTitle = "15 keV e-gun data";

  }


  TFile* file_phys  = TFile::Open( fileName_phys .c_str() );
  TFile* file_noise = TFile::Open( fileName_noise.c_str() );

  std::cout << "Physics file: " << fileName_phys << std::endl;
  std::cout << "Noise   file: " << fileName_phys << std::endl;
  std::cout << std::endl;

  TTree* tree_phys  = (TTree*) file_phys ->Get("treeLite");
  TTree* tree_noise = (TTree*) file_noise->Get("treeLite");

  std::string outdir = "plots/" + run + "/";

  system( Form("mkdir -p %s", outdir.c_str()) );

  drawVar( outdir, ""            , tree_phys, tree_noise, "ampNegRA", ""             , legendTitle, "Negative Max Amplitude (RA) (mV)", 50, -10.,  0. );
  drawVar( outdir, "_ampRA5"     , tree_phys, tree_noise, "ampNegRA", "ampRA>5."     , legendTitle, "Negative Max Amplitude (RA) (mV)", 50, -10.,  0. );

  drawVar( outdir, ""            , tree_phys, tree_noise, "amp"     , ""             , legendTitle, "Max Amplitude (mV)"              , 50,   0., 30. );
  drawVar( outdir, ""            , tree_phys, tree_noise, "ampRA"   , ""             , legendTitle, "Max Amplitude (RA) (mV)"         , 50,   0., 20. );
  drawVar( outdir, "_ampNegRA2p5", tree_phys, tree_noise, "ampRA"   , "ampNegRA>-2.5", legendTitle, "Max Amplitude (RA) (mV)"         , 50,   0., 20. );

  return 0;

}


void drawVar( const std::string& outdir, const std::string& suffix, TTree* tree_phys, TTree* tree_noise, const std::string& var, const std::string& selection, const std::string& legendTitle, const std::string& axisName, int nBins, float xMin, float xMax ) {

  TH1D* h1_phys  = new TH1D( Form("%s_%s_phys" , var.c_str(), suffix.c_str()), "", nBins, xMin, xMax );
  TH1D* h1_noise = new TH1D( Form("%s_%s_noise", var.c_str(), suffix.c_str()), "", nBins, xMin, xMax );

  tree_phys ->Project( h1_phys ->GetName(), var.c_str(), selection.c_str() );
  tree_noise->Project( h1_noise->GetName(), var.c_str(), selection.c_str() );

  float yMax_phys  = 1.3*h1_phys->GetMaximum() /h1_phys ->Integral();
  float yMax_noise = 1.3*h1_noise->GetMaximum()/h1_noise->Integral();
  float yMax_axes = (yMax_phys>yMax_noise) ? yMax_phys : yMax_noise;

  TCanvas* c1 = new TCanvas( Form("c1_%s_%s", var.c_str(), suffix.c_str()), "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( Form("axes_%s_%s", var.c_str(), suffix.c_str()), "", 10, xMin, xMax, 10, 0., yMax_axes );
  h2_axes->SetXTitle( axisName.c_str() );
  h2_axes->SetYTitle( "Normalized to Unity" );
  h2_axes->Draw();

  h1_phys ->SetLineWidth(3);
  h1_phys ->SetLineColor(46);
  h1_phys ->SetFillColor(46);
  h1_phys ->SetFillStyle(3004);

  h1_noise->SetLineWidth(3);
  h1_noise->SetLineColor(38);
  h1_noise->SetFillColor(38);
  h1_noise->SetFillStyle(3005);

  TLegend* legend = new TLegend( 0.2, 0.7, 0.5, 0.9, legendTitle.c_str() );
  //TLegend* legend = new TLegend( 0.4, 0.7, 0.7, 0.9, legendTitle.c_str() );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);
  legend->AddEntry( h1_noise, "Noise run", "L" );
  legend->AddEntry( h1_phys , "Physics run", "L" );
  legend->Draw("same");

  h1_phys ->DrawNormalized("same");
  h1_noise->DrawNormalized("same");

  gPad->RedrawAxis();

  c1->SaveAs( Form("%s/%s%s.pdf", outdir.c_str(), var.c_str(), suffix.c_str()) );

  TCanvas* c1_log = new TCanvas( Form("c1_%s_%s_log", var.c_str(), suffix.c_str()), "", 600, 600 );
  c1_log->SetLogy();

  TH2D* h2_axes_log = new TH2D( Form("axes_%s_%s_log", var.c_str(), suffix.c_str()), "", 10, xMin, xMax, 10, 0.001, 10.*yMax_axes );
  h2_axes_log->SetXTitle( axisName.c_str() );
  h2_axes_log->SetYTitle( "Normalized to Unity" );
  h2_axes_log->Draw();

  legend->Draw("same");
  
  h1_phys ->DrawNormalized("same");
  h1_noise->DrawNormalized("same");

  gPad->RedrawAxis();

  c1_log->SaveAs( Form("%s/%s%s_log.pdf", outdir.c_str(), var.c_str(), suffix.c_str()) );

  delete c1;
  delete c1_log;
  delete h2_axes;
  delete h2_axes_log;
  delete h1_phys;
  delete h1_noise;

}
