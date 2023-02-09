#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2D.h"




TGraphErrors* getGraph( const std::string& name, float scale=1. );




int main( int argc, char* argv[] ) {

  AndCommon::setStyle();

  std::string saveName;
  std::string scan1(""), scan2("");
  float scale1(1.), scale2(1.);
  std::string legend1("Scan 1"), legend2("Scan 2");
  float ymax = 20.;
  
  if( argc == 3 ) {

    scan1 = std::string(argv[1]);
    scan2 = std::string(argv[2]);
    scale1 = 1.;
    scale2 = 1.;
    legend1 = "Scan 1";
    legend2 = "Scan 2";
    saveName = "";

  } else if( argc == 2 ) {

    saveName = std::string(argv[1]);

    if( saveName=="pressure" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan2 = "CNTetchedOLD_N6bis_whopper_d1_20230209_posDeltaV_highP";
      scale1 = -1.;
      scale2 = -1.;
      legend1 = "p = 3E-6 mbar";
      legend2 = "2 < p < 5 E-5 mbar";

    } else if( saveName=="posneg" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208";
      scan2 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scale1 = +1.;
      scale2 = -1.;
      legend1 = "Negative";
      legend2 = "Positive";
      ymax = 2000.;

    } // if saveName

  } else {

    std::cout << "you need to input either two scan names, or a comparison name" << std::endl;
    exit(1);

  } // if argc


  
  TGraphErrors* gr0 = getGraph( scan1, scale1 );
  TGraphErrors* gr1 = getGraph( scan2, scale2 );

  gr0->SetMarkerSize ( 2  );
  gr0->SetMarkerColor( 46 );
  gr0->SetLineColor  ( 46 );
  gr0->SetMarkerStyle( 20 );

  gr1->SetMarkerSize ( 2  );
  gr1->SetMarkerColor( 46 );
  gr1->SetLineColor  ( 46 );
  gr1->SetMarkerStyle( 24 );

  TCanvas* c1 = new TCanvas( "c1", "c1", 600, 600 );
  c1->cd();
  c1->SetLogy();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., 1100., 10, 0.1, ymax );
  h2_axes->SetXTitle( "|#DeltaV| (V)" );
  h2_axes->SetYTitle( "I (pA)" );
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.2, 0.72, 0.55, 0.87 );
  legend->SetTextSize( 0.035 );
  legend->SetFillColor( 0 );
  legend->AddEntry( gr0, legend1.c_str(), "P" );
  legend->AddEntry( gr1, legend2.c_str(), "P" );
  legend->Draw("same");

  gr0->Draw("Psame");
  gr1->Draw("Psame");

  gPad->RedrawAxis();

  if( saveName=="" ) 
    c1->SaveAs( "compareScansIV.pdf");
  else 
    c1->SaveAs( Form("compareScansIV_%s.pdf", saveName.c_str()) );
   

  return 0;

}



TGraphErrors* getGraph( const std::string& name, float scale ) {

  TFile* file = TFile::Open( Form("plots/%s/graphs.root", name.c_str()) );

  TGraphErrors* graph = (TGraphErrors*)file->Get( Form( "gr_%s", name.c_str()) );

  TGraphErrors* returnGraph = new TGraphErrors(0);
  returnGraph->SetName( Form( "scaled_gr_%s", name.c_str()) );

  for( unsigned iPoint = 0; iPoint < graph->GetN(); ++iPoint ) {

    double x, y;
    graph->GetPoint( iPoint, x, y );

    double errx = graph->GetErrorX(iPoint);
    double erry = graph->GetErrorY(iPoint);

    returnGraph->SetPoint( iPoint, x, scale*y );
    returnGraph->SetPointError( iPoint, errx, erry );

  } // for points

  return returnGraph;

}
