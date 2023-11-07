#include <iostream>

#include "AndCommon.h"
#include "IVScanFN.h"

#include "TFile.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH2D.h"
#include "TString.h"




float getScale( const std::string& name );



int main( int argc, char* argv[] ) {

  AndCommon::setStyle();

  std::string saveName;
  std::string scan0(""), scan1("");
  float scale0(-1.);
  float scale1(-1.);
  std::string legend0("Scan 1"), legend1("Scan 2");
  float xmax = 1100.;
  float ymin = -1.;
  float ymax = 9.;
  float textSize = 0.035;
  std::string legendTitle = "";

  
  if( argc == 3 ) {

    scan0 = std::string(argv[1]);
    scan1 = std::string(argv[2]);

    scale0 = getScale(scan0);
    scale1 = getScale(scan1);

    legend0 = scan0;
    legend1 = scan1;
    saveName = "";
    textSize = 0.025;

  } else if( argc == 2 ) {

    saveName = std::string(argv[1]);

    if( saveName=="pressure" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230209_posDeltaV_highP";
      scale0 = -1.;
      scale1 = -1.;
      legend0 = "p = 3E-6 mbar";
      legend1 = "2 < p < 5 E-5 mbar";

    } else if( saveName=="posneg" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_d1_20230208";
      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scale0 = +1.;
      scale1 = -1.;
      legend0 = "CNT HV < 0";
      legend1 = "CNT HV > 0";
      ymax = 1E3;

    } else if( saveName=="ionOFF" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230209_posDeltaV_ionOFF";
      scale0 = -1.;
      scale1 = -1.;
      legend0 = "Ion gauge on";
      legend1 = "Ion gauge off";

    } else if( saveName=="doubleTeflon" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan1 = "CNTetchedOLD_N6bis_whopper_d2_20230209_posDeltaV";
      scale0 = -1.;
      scale1 = -1.;
      legend0 = "d = 1 mm";
      legend1 = "d = 2 mm";

    } else if( saveName=="tripleTeflon" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scan1 = "CNTetchedOLD_N6bis_whopper_d3_20230209_posDeltaV";
      scale0 = -1.;
      scale1 = -1.;
      legend0 = "d = 1 mm";
      legend1 = "d = 3 mm";

    } else if( saveName=="posNegAfterClean" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_d1p5_20230213";
      scan1 = "CNTetchedOLD_N6bis_whopper_d1p5_20230213_posDeltaV";
      scale0 = +1.;
      scale1 = -1.;
      legend0 = "HV < 0";
      legend1 = "HV > 0";

    } else if( saveName=="LNGS_T1" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain";
      scan1 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228";
      scale0 = -1.;
      scale1 = +1.;
      legend0 = "HV < 0";
      legend1 = "HV > 0";
      ymin = 100.;
      ymax = 50E6;
      xmax = 1300.;
      legendTitle = "IETI (T = 1 K)";

    } else if( saveName=="LNGS_T1_vs_Hyperion" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain";
      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208";
      scale0 = -1.;
      scale1 = +1.;
      legend0 = "IETI (T = 1 K)";
      legend1 = "Hyperion (RT)";
      ymax = 50E6;
      xmax = 1300.;

    } else if( saveName=="LNGS_T1_vs_T0p02" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228_drain";
      scan1 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t0p02_20230301_drain";
      scale0 = -1.;
      scale1 = -1.;
      legend0 = "IETI (T = 1 K)";
      legend1 = "IETI (T = 0.02 K)";
      ymin = 100;
      ymax = 50E6;
      xmax = 1300.;

    } else if( saveName=="Ohm_LNGS_vs_Hyperion" ) {

      scan0 = "CNTetchedOLD_N6bis_whopper_LNGS_d1_t1_20230228";
      scan1 = "CNTetchedOLD_N6bis_whopper_d1_20230208_posDeltaV";
      scale0 = 1.;
      scale1 = -1.;
      legendTitle = "CNT HV > 0";
      legend0 = "IETI (T = 1 K)";
      legend1 = "Hyperion";
      ymin = 0.1;
      ymax = 50E5;
      xmax = 1300.;

    } else if( saveName=="LNGS_HdM_300K" ) {

      scan0 = "CNTetchedOLD_N6bis_LNGS_HdM_d1p5_300K_20230315_drain";
      scan1 = "CNTetchedOLD_N6bis_LNGS_HdM_d1p5_300K_20230315";
      scale0 = -1.;
      scale1 = 1.;
      legendTitle = "LNGS HdM T = 300 K";
      legend0 = "V(cnt) < V(anode)";
      legend1 = "V(cnt) > V(anode)";
      ymin = 0.1;
      ymax = 50E5;
      xmax = 1300.;

    } else if( saveName=="LNGS_HdM_300K_vs_30K" ) {

      scan0 = "CNTetchedOLD_N6bis_LNGS_HdM_d1p5_300K_20230315_drain";
      scan1 = "CNTetchedOLD_N6bis_LNGS_HdM_d1p5_30K_20230315_drain";
      scale0 = -1.;
      scale1 = -1.;
      legendTitle = "LNGS HdM";
      legend0 = "T = 300 K";
      legend1 = "T = 30 K";
      ymin = 0.1;
      ymax = 50E5;
      xmax = 1300.;

    } else if( saveName=="CNTetchedOLD_N1new" ) {

      scan0 = "CNTetchedOLD_N1new_d4_20230517_drain";
      scan1 = "CNTetchedOLD_N1new_d5_20230517_drain";
      scale0 = -1.;
      scale1 = -1.;
      //legendTitle = "LNGS HdM";
      legend0 = "d = 4 mm";
      legend1 = "d = 5 mm";
      ymin = 0.1;
      ymax = 50E5;
      xmax = 1300.;

    } // if saveName

  } else {

    std::cout << "you need to input either two scan names, or a comparison name" << std::endl;
    exit(1);

  } // if argc

  TString scan0_tstr(scan0);
  if( scan0_tstr.Contains("drain") ) scale0 = +1.;
  TString scan1_tstr(scan1);
  if( scan1_tstr.Contains("drain") ) scale1 = +1.;


  IVScanFN ivs0( scan0, scale0 );
  IVScanFN ivs1( scan1, scale1 );

  std::cout << std::endl;
  std::cout << "-> Will compare scans:" << std::endl;
  std::cout << "   " << scan0 << "   with scale: " << scale0 << std::endl;
  std::cout << "   " << scan1 << "   with scale: " << scale1 << std::endl;
  std::cout << std::endl;

  
  TGraphErrors* gr0 = ivs0.graph();
  TGraphErrors* gr1 = ivs1.graph();

  float xMin0, xMax0, yMin0, yMax0;
  AndCommon::findGraphRanges( gr0, xMin0, xMax0, yMin0, yMax0 );

  float xMin1, xMax1, yMin1, yMax1;
  AndCommon::findGraphRanges( gr1, xMin1, xMax1, yMin1, yMax1 );

  xmax = (xMax0 > xMax1) ? 1.3*xMax0 : 1.3*xMax1;
  ymax = (yMax0 > yMax1) ? 1.3*yMax0 : 1.3*yMax1;


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
  legend->AddEntry( gr0, legend0.c_str(), "P" );
  legend->AddEntry( gr1, legend1.c_str(), "P" );

  c1->cd();
  legend->Draw("same");
  gr0->Draw("Psame");
  gr1->Draw("Psame");

  gPad->RedrawAxis();

  if( saveName=="" ) 
    c1->SaveAs( Form("plots/compareScansIV_%s_vs_%s.pdf", scan0.c_str(), scan1.c_str()) );
  else 
    c1->SaveAs( Form("plots/compareScansIV_%s.pdf", saveName.c_str()) );
   

  c1_log->cd();
  legend->Draw("same");
  gr0->Draw("Psame");
  gr1->Draw("Psame");

  gPad->RedrawAxis();

  if( saveName=="" ) 
    c1_log->SaveAs( Form("plots/compareScansIV_%s_vs_%s_log.pdf", scan0.c_str(), scan1.c_str()) );
  else 
    c1_log->SaveAs( Form("plots/compareScansIV_%s_log.pdf", saveName.c_str()) );
   

  return 0;

}



float getScale( const std::string& name ) {

  TString tstr(name);
  
  float scale = (tstr.Contains("posDeltaV") || tstr.Contains("drain")) ? +1. : -1.;

  return scale;

}
