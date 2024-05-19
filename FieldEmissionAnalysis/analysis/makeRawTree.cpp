#include <iostream>

#include "lecroyparser.h"
#include "AndCommon.h"

#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"



int main() {

  int cd = 188;
  int v = 104;

  std::string dir_trc(Form("data/trc/CD%d/%d V", cd, v));
  std::string fileName_trc("C1--PTL4eD_CNT_StrongEtching_mini_40_mK_51p841uA_m166mV_trg_104V--00057.trc");

  std::string fileName = AndCommon::splitString( fileName_trc, "." )[0];
  //std::string fileName_root = fileName + ".root";

  TFile* file = TFile::Open( "test.root", "recreate" );
  //TFile* file = TFile::Open( Form("%s/%s.root", dir.c_str(), fileName.c_str()), "recreate" );

  TGraphErrors* gr = new TGraphErrors(0);

  TTree* tree = new TTree("treeraw", "");

  tree->Branch( "CD_number", &cd, "CD_number/I" );

  int nEvent = 0;
  tree->Branch( "event", &nEvent, "nEvent/I" );

  float pshape[2502];
  tree->Branch( "pshape", pshape, "pshape[2502]/F" );

  lecroyparser::lecroy_wavedesc_2_3 header;
  int16_t* waveform = nullptr;

  bool res = lecroyparser::read(Form("%s/%s", dir_trc.c_str(), fileName_trc.c_str()), header, waveform);

  if(res && waveform) {

    int nsamples = ((header.wave_array_count % 2502)==0 ) ? 2502 : 2501;

    int nEvents_thisFile = header.wave_array_count/nsamples;

    for( unsigned iEv=0; iEv<nEvents_thisFile; ++iEv ) {

      for(uint16_t i = 0; i < nsamples; ++i ) {

        pshape[i] = waveform[i + iEv*nsamples]*header.vertical_gain-header.vertical_offset;
        //gr->SetPoint(i, i, waveform[i]*header.vertical_gain-header.vertical_offset);
        //std::cout << i << "\t" << waveform[i] << "\t" << i*header.horiz_offset+header.horiz_interval << " " << waveform[i]*header.vertical_gain-header.vertical_offset << " " << std::endl;
      }

      tree->Fill();
      nEvent++;

      //std::cout << std::endl;

    }

    delete[] waveform;

  } else  {

    return 1;

  }

  file->cd();
  tree->Write();
  file->Close();

  return 0;

}

