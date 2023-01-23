#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TF1.h"




TH1D* addPoint( TGraphErrors* gr_mean, TGraphErrors* gr_rms, const std::string& fileName, float energy );
void addSinglePoint( TGraphErrors* gr, float x, float y, float y_err );

int main() {

  AndCommon::setStyle();

  std::vector<float> energy;
  energy.push_back(8.);
  energy.push_back(10.);
  energy.push_back(12.);
  energy.push_back(15.);
  energy.push_back(17.);

  TGraphErrors* gr_mean = new TGraphErrors(0);
  TGraphErrors* gr_rms  = new TGraphErrors(0);

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., 12., 10, 0., 0.16 );
  h2_axes->SetXTitle( "Max Amplitude (RA) (mV)");
  h2_axes->SetYTitle( "Normalized to Unity" );
  h2_axes->Draw();

  std::vector<int> colors = AndCommon::colors();

  TLegend* legend = new TLegend( 0.2, 0.6, 0.5, 0.9 );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);

  for( unsigned i=0; i<energy.size(); ++i ) {

    TH1D* h1 = addPoint( gr_mean, gr_rms, Form("lite_Run_APDWL_HV380_egun%.0fkeV_th7_Data_1_19_2023_Ascii.root", energy[i]), energy[i]  );

    h1->SetLineColor( colors[i] );
    h1->SetLineWidth( 2 );
    h1->DrawNormalized("same");

    legend->AddEntry( h1, Form("%.0f keV", energy[i]), "L" );

  }

  legend->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("provaLin.pdf");

  TF1* line = new TF1( "line", "[0] + [1]*x" );
  line->SetLineColor(46);
  line->SetLineWidth(2);

  gr_mean->Fit(line);

  c1->Clear();

  TH2D* h2_axes_mean = new TH2D( "axes_mean", "", 10, 7., 18., 10, 5.5, 7.5 );
  h2_axes_mean->SetXTitle( "Electron energy (keV)");
  h2_axes_mean->SetYTitle( "Max Amplitude (RA) mean (mV)" );
  h2_axes_mean->Draw();

  gr_mean->SetMarkerStyle(20);
  gr_mean->SetMarkerSize(1.6);
  gr_mean->SetMarkerColor(kGray+3);
  gr_mean->SetLineColor(kGray+3);
  gr_mean->Draw("P same");

  TPaveText* fitResults = new TPaveText( 0.48, 0.2, 0.9, 0.4, "brNDC" );
  fitResults->SetTextAlign(13);
  fitResults->SetFillColor(0);
  fitResults->SetTextSize(0.035 );
  fitResults->SetTextColor(46);
  fitResults->AddText( "Linear fit: y = mx + q" );
  fitResults->AddText( Form("m = %.3f #pm %.3f keV^{-1} mV", line->GetParameter(1), line->GetParError(1)) );
  fitResults->AddText( Form("q = %.3f #pm %.3f mV"         , line->GetParameter(0), line->GetParError(0)) );
  fitResults->Draw("same");

  c1->SaveAs("mean_vs_energy.pdf");

  return 0;

}


TH1D* addPoint( TGraphErrors* gr_mean, TGraphErrors* gr_rms, const std::string& fileName, float energy ) {

  TFile* file = TFile::Open( fileName.c_str() );
  TTree* tree = (TTree*)file->Get("treeLite");

  std::string hname(Form("h1_%s", fileName.c_str()));

  TH1D* h1 = new TH1D(hname.c_str(), "", 100, 0., 25.);

  //tree->Project( hname.c_str(), "ampRA", "ampNegRA > -3." );
  tree->Project( hname.c_str(), "ampRA", "ampNegRA > -2.5" );

  float mean = h1->GetMean();
  float rms = h1->GetRMS();
  float mean_err = h1->GetMeanError();
  float rms_err = h1->GetRMSError();

  addSinglePoint( gr_mean, energy, mean, mean_err );
  addSinglePoint( gr_rms , energy, rms , rms_err );

  std::cout << "Energy: " << energy << " keV    Mean: " << mean*1000. << " mV    RMS: " << rms*1000 << " mV    RMS/mean: " << rms/mean << std::endl;

  return h1;

}

  
void addSinglePoint( TGraphErrors* gr, float x, float y, float y_err ) {

  int thisPoint = gr->GetN();
  gr->SetPoint( thisPoint, x, y);
  gr->SetPointError( thisPoint, 0., y_err);

}
