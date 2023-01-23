#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TH1D.h"
#include "TH2D.h"




TH1D* addPoint( TGraphErrors* graph, const std::string& fileName, float energy );

int main() {

  AndCommon::setStyle();

  std::vector<float> energy;
  energy.push_back(8.);
  energy.push_back(10.);
  energy.push_back(12.);
  energy.push_back(15.);
  energy.push_back(17.);

  TGraphErrors* graph = new TGraphErrors(0);

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., 0.015, 10, 0., 0.13 );
  h2_axes->Draw();

  std::vector<int> colors = AndCommon::colors();

  for( unsigned i=0; i<energy.size(); ++i ) {

    TH1D* h1 = addPoint( graph, Form("lite_Run_APDWL_HV380_egun%.0fkeV_th7_Data_1_19_2023_Ascii.root", energy[i]), energy[i]  );

    h1->SetLineColor( colors[i] );
    h1->SetLineWidth( 2 );
    h1->DrawNormalized("same");

  }

  //TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  //c1->cd();

  //graph->SetMarkerStyle(20);
  //graph->SetMarkerColor(kGray+3);
  //graph->SetMarkerSize(1.5);

  //graph->Draw("APE");

  c1->SaveAs("provaLin.pdf");

  return 0;

}


TH1D* addPoint( TGraphErrors* graph, const std::string& fileName, float energy ) {

  TFile* file = TFile::Open( fileName.c_str() );
  TTree* tree = (TTree*)file->Get("treeLite");

  std::string hname(Form("h1_%s", fileName.c_str()));

  TH1D* h1 = new TH1D(hname.c_str(), "", 100, 0., 0.02);

  tree->Project( hname.c_str(), "ampRA", "ampNegRA>-0.003" );

  float mean = h1->GetMean();
  float rms = h1->GetRMS();
  int thisPoint = graph->GetN();
  graph->SetPoint( thisPoint, energy, mean);
  //graph->SetPointError( thisPoint, 0., rms);

  std::cout << "Energy: " << energy << " keV    Mean: " << mean*1000. << " mV    RMS: " << rms*1000 << " mV    RMS/mean: " << rms/mean << std::endl;

  return h1;

}

  
