#include <iostream>

#include "AndCommon.h"

#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2D.h"
#include "TString.h"




TGraphErrors* getGraph( const std::string& name, float scale=1. );
float getScale( const std::string& name );



int main( int argc, char* argv[] ) {

  AndCommon::setStyle();

  std::string saveName;
  std::string scan1(""), scan2("");
  float scale1(1.);
  float scale2(1.);
  std::string legend1("Scan 1"), legend2("Scan 2");
  float xmax = 1100.;
  float ymin = -1.;
  float ymax = 9.;
  float textSize = 0.035;
  std::string legendTitle = "";
  
  if( argc == 3 ) {

    scan1 = std::string(argv[1]);
    scan2 = std::string(argv[2]);
    scale1 = getScale(scan1);
    scale2 = getScale(scan2);
    legend1 = scan1;
    legend2 = scan2;
    saveName = "";
    textSize = 0.025;

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
      legend1 = "CNT HV < 0";
      legend2 = "CNT HV > 0";
      ymax = 1E3;

    } else if( saveName=="ionOFF" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan2 = "CNTetchedOLD_N6bis_whopper_d1_20230209_posDeltaV_ionOFF";
      scale1 = -1.;
      scale2 = -1.;
      legend1 = "Ion gauge on";
      legend2 = "Ion gauge off";

    } else if( saveName=="doubleTeflon" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan2 = "CNTetchedOLD_N6bis_whopper_d2_20230209_posDeltaV";
      scale1 = -1.;
      scale2 = -1.;
      legend1 = "d = 1 mm";
      legend2 = "d = 2 mm";

    } else if( saveName=="tripleTeflon" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan2 = "CNTetchedOLD_N6bis_whopper_d3_20230209_posDeltaV";
      scale1 = -1.;
      scale2 = -1.;
      legend1 = "d = 1 mm";
      legend2 = "d = 3 mm";

    } else if( saveName=="posNegAfterClean" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_d1p5_20230213";
      scan2 = "CNTetchedOLD_N6bis_whopper_d1p5_20230213_posDeltaV";
      scale1 = +1.;
      scale2 = -1.;
      legend1 = "HV < 0";
      legend2 = "HV > 0";

    } else if( saveName=="LNGS_T1" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain";
      scan2 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228";
      scale1 = -1.;
      scale2 = +1.;
      legend1 = "HV < 0";
      legend2 = "HV > 0";
      ymin = 100.;
      ymax = 50E6;
      xmax = 1300.;
      legendTitle = "IETI (T = 1 K)";

    } else if( saveName=="LNGS_T1_vs_Hyperion" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain";
      scan2 = "CNTetchedOLD_N6bis_whopper_d1_20230208";
      scale1 = -1.;
      scale2 = +1.;
      legend1 = "IETI (T = 1 K)";
      legend2 = "Hyperion (RT)";
      ymax = 50E6;
      xmax = 1300.;

    } else if( saveName=="LNGS_T1_vs_T0p02" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain";
      scan2 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t0p02_20230301_drain";
      scale1 = -1.;
      scale2 = -1.;
      legend1 = "IETI (T = 1 K)";
      legend2 = "IETI (T = 0.02 K)";
      ymin = 100;
      ymax = 50E6;
      xmax = 1300.;

    } else if( saveName=="Ohm_LNGS_vs_Hyperion" ) {

      scan1 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228";
      scan2 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scale1 = 1.;
      scale2 = -1.;
      legendTitle = "CNT HV > 0";
      legend1 = "IETI (T = 1 K)";
      legend2 = "Hyperion";
      ymin = 0.1;
      ymax = 50E5;
      xmax = 1300.;

    } // if saveName

  } else {

    std::cout << "you need to input either two scan names, or a comparison name" << std::endl;
    exit(1);

  } // if argc


  std::cout << std::endl;
  std::cout << "-> Will compare scans:" << std::endl;
  std::cout << "   " << scan1 << "   with scale: " << scale1 << std::endl;
  std::cout << "   " << scan2 << "   with scale: " << scale2 << std::endl;
  std::cout << std::endl;

  
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

  TH2D* h2_axes = new TH2D( "axes", "", 10, 0., xmax, 10, ymin, ymax );
  h2_axes->SetXTitle( "|#DeltaV| (V)" );
  h2_axes->SetYTitle( "I (pA)" );
  h2_axes->Draw();

  
  TCanvas* c1_log = new TCanvas( "c1_log", "c1_log", 600, 600 );
  c1_log->cd();
  c1_log->SetLogy();

  float ymin_log = (ymin < 0.1 ) ? 0.1 : ymin;
  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, 0., xmax, 10, ymin_log, 2*ymax );
  h2_axes_log->SetXTitle( "|#DeltaV| (V)" );
  h2_axes_log->SetYTitle( "I (pA)" );
  h2_axes_log->Draw();

  float ymin_legend = 0.72;
  if( legendTitle!="" ) ymin_legend -= 0.05;
  TLegend* legend = new TLegend( 0.22, ymin_legend, 0.55, 0.87 );
  if( legendTitle!="" ) legend->SetHeader( legendTitle.c_str() );
  legend->SetTextSize( textSize );
  legend->SetFillColor( 0 );
  legend->AddEntry( gr0, legend1.c_str(), "P" );
  legend->AddEntry( gr1, legend2.c_str(), "P" );

  c1->cd();
  legend->Draw("same");
  gr0->Draw("Psame");
  gr1->Draw("Psame");

  gPad->RedrawAxis();

  if( saveName=="" ) 
    c1->SaveAs( Form("plots/compareScansIV_%s_vs_%s.pdf", scan1.c_str(), scan2.c_str()) );
  else 
    c1->SaveAs( Form("plots/compareScansIV_%s.pdf", saveName.c_str()) );
   

  c1_log->cd();
  legend->Draw("same");
  gr0->Draw("Psame");
  gr1->Draw("Psame");

  gPad->RedrawAxis();

  if( saveName=="" ) 
    c1_log->SaveAs( Form("plots/compareScansIV_%s_vs_%s_log.pdf", scan1.c_str(), scan2.c_str()) );
  else 
    c1_log->SaveAs( Form("plots/compareScansIV_%s_log.pdf", saveName.c_str()) );
   

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



float getScale( const std::string& name ) {

  TString tstr(name);
  
  float scale = (tstr.Contains("posDeltaV") || tstr.Contains("drain")) ? -1. : 1.;

  return scale;

}
