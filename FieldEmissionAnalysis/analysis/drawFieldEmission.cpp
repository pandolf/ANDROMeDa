#include "AndCommon.h"
#include "IVScanFN.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TFile.h"
#include "TGaxis.h"
#include "TLegend.h"




IVScanFN analyzeFN( const std::string& name );



int main( int argc, char* argv[] ) {

  if( argc < 2 ) {
    std::cout << "USAGE: ./drawFieldEmission [dataName]" << std::endl;
    exit(1);
  }

  std::string name( AndCommon::removePathAndSuffix(argv[1]) );

  TStyle* style = AndCommon::setStyle();
  style->SetPadLeftMargin(0.12);
  style->SetPadRightMargin(0.11);
  style->SetTitleYOffset(1.15);
  style->cd();

  std::vector<int> colors = AndCommon::colors();

  std::vector< IVScanFN > scans;

  if( name == "CNTArO2Etching_N1" ) {

    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d5_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d4_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d3_20221130" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1_d2_20221130" ) );

  } else if( name == "CNTArO2Etching_N1new" ) {

    scans.push_back( analyzeFN( "CNTArO2Etching_N1new_d5_20230517_drain" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1new_d4_20230517_drain" ) );

  } else if( name == "paper" ) {

    scans.push_back( analyzeFN( "CNTArO2Etching_AsGrown_INRiM_MICa_3Kplate_d0p5_IvsV_anode_90to150V_20231213_2_sweepA" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_N1new_INRiM_MICb_3Kplate_d0p5_IvsV_drain_0to88V_20231130_sweepR" ) );
    scans.push_back( analyzeFN( "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR" ) );
    scans.push_back( analyzeFN( "PECVD_FE_INRIM_001_MICa_3Kplate_d0p5_IvsV_anode_from205Vto265V_10032025_post_conditioning_sweepR" ) );

  } else {

    scans.push_back( analyzeFN(name) );

  }


  if( scans.size()>1 ) { // plot them together (hardwired numbers just for paper for the moment)

    TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
    c1->cd();

    TH2D* h2_axes = new TH2D( "axes_all", "", 10, 0., 200., 10, -1., 3. );
    h2_axes->SetXTitle( "V_{cnt} (V)" );
    h2_axes->SetYTitle( "I_{cnt} (pA)" );
    h2_axes->Draw("");

    TLegend* legend = new TLegend( 0.2, 0.55, 0.6, 0.9 );
    legend->SetFillColor(0);
    legend->SetTextSize(0.04);
    legend->Draw("same");

    for( unsigned i=0; i<scans.size(); ++i ) {
      scans[i].set_color(colors[i]);
      scans[i].graph()->SetLineColor(kBlack);
      scans[i].graph()->Draw("P same");
      //scans[i].reducedgraph()->Draw("P same");
      legend->AddEntry( scans[i].graph(), AndCommon::cuteSampleName(scans[i].sampleName()).c_str(), "PL" );
    }


    gPad->RedrawAxis();

    c1->SaveAs( Form("iv_%s.pdf", name.c_str()) );

  }

    

  return 0;

}



IVScanFN analyzeFN( const std::string& name ) {


  float scale = -1.;
  float iMin = 0.5; // default: compute FN only between 0.5 pA
  float iMax = 7.;
  //float iMax = 1.5; //          and 1.5 pA

  TString name_tstr(name);
  if( name_tstr.Contains("drain") ) scale = +1.;

  if ( name_tstr.BeginsWith( "CNTArO2Etching_AsGrown_INRiM_MICa_3Kplate_d1p5_IvsV_anode" ) ) {
    iMin = 0.5;
    iMax = 1.0;
  } else if ( name_tstr.BeginsWith( "PECVD_FE_INRIM_001_MICa_3Kplate_d0p5_IvsV_anode_from235Vto275V_18022024_post_conditioning" ) ) {
    iMin = 2.;
    iMax = 7.;
  } else if ( name_tstr.BeginsWith( "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepA" ) ) {
    iMin = 2.5;
    iMax = 7.;
  } else if ( name_tstr.BeginsWith( "CNTArO2Etching_Strongnew_INRiM_MICb_3Kplate_d0p5_IvsV_anode_20to95V_20231207_sweepR" ) ) {
    iMin = 0.5;
    iMax = 1.5;
  } else if ( name_tstr.BeginsWith( "PECVD_FE_INRIM_001_MICa_3Kplate_d0p5_IvsV_anode_from205Vto265V_10032025_post_conditioning" ) ) {
    iMin = 2.;
    iMax = 7.;
  }


  IVScanFN ivs( name, scale, iMin, iMax );

//if( name_tstr.BeginsWith( "PECVD_FE_INRIM_001_MICa_3Kplate_d0p5_IvsV_anode_from235Vto275V_18022024_post_conditioning" ) )
//  ivs.set_n(0);



  // ***** FIRST STEP: draw I vs V

  TGraphErrors* graph = ivs.graph();

  if( graph->GetN()==0 ) {
    std::cout << "-> ERROR! No data points in graph. Check the data file or change hvMin and hvMax to select more points." << std::endl;
    exit(1);
  }


  std::string outdir(Form("plots/%s/", ivs.name().c_str()));
  system( Form("mkdir -p %s", outdir.c_str()) );


  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  //float xMin = 1300.;
  //float xMax = 2999.;
  //float xMin = 0.9*hvMin;
  //float xMax = 1.1*hvMax;


  float xMin_iv, xMax_iv, yMin_iv, yMax_iv;
  AndCommon::findGraphRanges( graph, xMin_iv, xMax_iv, yMin_iv, yMax_iv );

  xMin_iv = 0.9*xMin_iv;
  xMax_iv = 1.1*xMax_iv;
  yMin_iv = 0.9*yMin_iv;
  yMax_iv = 1.1*yMax_iv;

  if( ivs.lab() == "INRiM" ) {

    if( ivs.sampleName()=="CNTArO2Etching_N1new" ) {
      if( ivs.d() < 0.9 ) {
        xMin_iv = 0.;
        xMax_iv = 120.;
        //xMax_iv = 80.*(ivs.d()/0.5);
      } else {
        xMin_iv = 170.;
        xMax_iv = 220.;
      }
   // } else if( ivs.sampleName()=="CNTArO2Etching_N1new_B" ) {
   //   xMin_iv = 0.;
   //   xMax_iv = 150.;
    }

    yMin_iv = -0.9999;
    yMax_iv = 10.;

  } else {

    xMin_iv = 0.;
    yMin_iv = 0.;

  }


  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin_iv, xMax_iv, 10, yMin_iv, yMax_iv );
  h2_axes->SetXTitle( "-#DeltaV (V)" );
  h2_axes->SetYTitle( "I (pA)" );
  h2_axes->Draw();

  TLine* line_zero = new TLine( xMin_iv, 0., xMax_iv, 0. );
  line_zero->Draw("P same");

  TLine* line_one  = new TLine( xMin_iv, 1., xMax_iv, 1. );
  line_one->SetLineStyle(2);
  if( ivs.lab() == "INRiM" ) line_one->Draw("P same");

  TPaveText* pd_text = ivs.setupLabel();
  pd_text->Draw("Same");

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(1.5);
  graph->SetMarkerColor(46);
  graph->SetLineColor  (46);

  graph->Draw("P same");

  gPad->RedrawAxis();

  c1->SaveAs( Form("%s/iv.pdf", outdir.c_str()) );

  delete h2_axes;

  c1->Clear();
  c1->SetLogy();

  TH2D* h2_axes_log = new TH2D( "axes_log", "", 10, 60, 150, 10, 0.1, 600. );
  h2_axes_log->SetXTitle( "-#DeltaV (V)" );
  h2_axes_log->SetYTitle( "I (pA)" );
  h2_axes_log->Draw();

  pd_text->Draw("Same");
  graph->Draw("P same");

  c1->SaveAs( Form("%s/iv_log.pdf", outdir.c_str()) );

  delete h2_axes_log;



  // ***** SECOND STEP: draw FN plots

  TGraphErrors* gr_fn = ivs.graphFN();
  TF1* lineFN = gr_fn->GetFunction(Form("lineFN_%s", gr_fn->GetName()));


  TCanvas* c1_fn = new TCanvas( "c1_fn", "", 600, 600 );
  c1_fn->Clear();

  //TGaxis::SetMaxDigits(2);


  float xMinFN, xMaxFN, yMinFN, yMaxFN;
  AndCommon::findGraphRanges( gr_fn, xMinFN, xMaxFN, yMinFN, yMaxFN );

  xMinFN = 0.95*xMinFN;
  xMaxFN = 1.05*xMaxFN;

  yMinFN = (yMinFN<0.) ? 1.1*yMinFN : 0.9*yMinFN;
  yMaxFN = (yMaxFN<0.) ? 0.9*yMaxFN : 1.1*yMaxFN;

  //yMinFN = -10. - 2.*(ivs.d()-0.5)/0.5;
  //yMaxFN = -3.  - 2.*(ivs.d()-0.5)/0.5;

  TH2D* h2_axes_fn = new TH2D( "axes_fn", "", 10, xMinFN, xMaxFN, 10, yMinFN, yMaxFN );
  h2_axes_fn->SetXTitle( IVScanFN::xTitleFN().c_str() );
  h2_axes_fn->SetYTitle( IVScanFN::yTitleFN().c_str() );
  h2_axes_fn->GetXaxis()->SetMaxDigits(2);
  h2_axes_fn->GetYaxis()->SetMaxDigits(2);
  h2_axes_fn->Draw();

  float gamma_err;
std::cout << "before" << std::endl;
  float gamma = ivs.get_gamma_and_err( gamma_err );
std::cout << "after" << std::endl;
  //float gamma_err = sqrt( gamma_err_corr*gamma_err_corr + gamma_err_ucorr*gamma_err_ucorr );

  TPaveText* gamma_text = new TPaveText( 0.6, 0.8, 0.85, 0.9, "brNDC" );
  gamma_text->SetFillColor(0);
  gamma_text->SetTextSize(0.038);
  gamma_text->SetTextColor( 46 );
  //gamma_text->SetTextColor( kGray+3 );
  gamma_text->AddText( Form("#gamma = %.0f #pm %.0f", gamma, gamma_err) );
  gamma_text->Draw("same");

  pd_text->SetX1( 0.18  );
  pd_text->SetY1( 0.2  );
  pd_text->SetX2( 0.48  );
  pd_text->SetY2( 0.35 );
  pd_text->Draw("same");


  // test per git
  gr_fn->SetMarkerStyle(20);
  gr_fn->SetMarkerSize(1.5);
  gr_fn->SetMarkerColor(kGray+3);
  gr_fn->SetLineColor  (kGray+3);

  gr_fn->Draw("P same");

  std::cout << std::endl;
  std::cout << "Chi^2 / NDF: " << lineFN->GetChisquare() << " / " << lineFN->GetNDF() <<std::endl;
  std::cout << std::endl;


//// check outliers:
//for( unsigned iPoint=0; iPoint<gr_fn->GetN(); ++iPoint ) {

//  double x, y;
//  gr_fn->GetPoint( iPoint, x, y );

//  double yerr = gr_fn->GetErrorY( iPoint );

//  float delta = fabs( lineFN->Eval(x)-y )/yerr;

//  if( delta > 3. ) {

//    std::cout << std::endl;
//    std::cout << "   WARNING!! POINT #" << iPoint << " is an OUTLIER!" << std::endl;
//    std::cout << "     hv = " << 1/x << " Volts" << std::endl;
//    std::cout << "     y = " << y << " +/- " << yerr << std::endl;
//    std::cout << "     f(x) = " << lineFN->Eval(x) << std::endl;
//    std::cout << "     Delta = " << delta << " sigma" << std::endl;
//    std::cout << std::endl;

//  } // if outlier

//} // for points

 

  c1_fn->SaveAs( Form("%s/fn.pdf", outdir.c_str()) );

  delete h2_axes_fn;

  std::cout << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << " gamma: " << gamma << " +/- " << gamma_err  << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << std::endl << std::endl;


  std::string graphsFileName(Form("%s/graphs.root", outdir.c_str()));

  TFile* outfile = TFile::Open( graphsFileName.c_str(), "RECREATE" );
  outfile->cd();

  graph->Write();
  gr_fn->Write();

  outfile->Write();
  outfile->Close();

  std::cout << "-> Stored graphs in: " << graphsFileName << std::endl;

  std::ofstream ofs(Form("%s/gamma.dat", outdir.c_str()));

  ofs << gamma << " " << gamma_err << std::endl;

  ofs.close();

  std::cout << "-> Stored gamma in: " << Form("%s/gamma.dat", outdir.c_str()) << std::endl;

  delete c1;
  delete c1_fn;

  return ivs;

}


