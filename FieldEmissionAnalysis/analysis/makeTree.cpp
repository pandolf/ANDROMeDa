#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TMath.h"
#include "../interface/pshape_functions.h"

int main(int argc, char* argv[]){

  //parametri da input
  if (argc!=3){
    std::cout << "USAGE: ./makeTree [CD_number] [voltage]" << std::endl;
    exit(1);
  }

  int   CD_number = (atoi(argv[1])); //cooldown number
  int voltage     = (atoi(argv[2]));

  //apertura file con il tree
  TFile run(Form("tree/CD%d_%dV_treeraw.root",CD_number,voltage));
  TTree *tree_raw = (TTree*)run.Get(Form("%dV_treeraw",voltage));
  Long64_t nentries = tree_raw->GetEntries();

  //preparazione per la lettura del tree_raw
  int   event_raw;
  float dt_raw;
  float trigger_raw;
  float pshape[2500];

  tree_raw->SetBranchAddress("CD_number", &CD_number   );
  tree_raw->SetBranchAddress("voltage"  , &voltage     );
  tree_raw->SetBranchAddress("event"    , &event_raw   );
  tree_raw->SetBranchAddress("dt"       , &dt_raw      );
  tree_raw->SetBranchAddress("trigger"  , &trigger_raw );
  tree_raw->SetBranchAddress("pshape"   ,  pshape      );

  
  //creazione variabili del nuovo tree
  int      event;
  float    trigger;
  float    baseline;
  float    baseline_error;
  float    charge;
  float    amp;
  Controls ctrl; //classe definita in pshape_functions.h

  int ctrl_double_low;
  int ctrl_double_high;
  int ctrl_trigger;
  int ctrl_saturation; 
  int ctrl_width;
  
  
  //nuovo tree
  std::string outfile_name = Form("CD%d_%dV_tree.root",CD_number,voltage);
  TFile* outfile = TFile::Open(Form("tree/%s",outfile_name.c_str()),"recreate");
  TTree* tree = new TTree("tree","tree");

  tree->Branch("CD_number"        , &CD_number        ,         "CD_number/I");
  tree->Branch("voltage"          , &voltage          ,           "voltage/I"); 
  tree->Branch("event"            , &event            ,             "event/I");
  tree->Branch("trigger"          , &trigger          ,           "trigger/F");
  tree->Branch("baseline"         , &baseline         ,          "baseline/F");
  tree->Branch("baseline_error"   , &baseline_error   ,    "baseline_error/F");
  tree->Branch("charge"           , &charge           ,            "charge/F");
  tree->Branch("amp"              , &amp              ,               "amp/F");
  tree->Branch("ctrl_double_low"  , &ctrl_double_low  ,   "ctrl_double_low/I");
  tree->Branch("ctrl_double_high" , &ctrl_double_high ,  "ctrl_double_high/I");
  tree->Branch("ctrl_width"       , &ctrl_width       ,        "ctrl_width/I");
  tree->Branch("ctrl_saturation"  , &ctrl_saturation  ,   "ctrl_saturation/I");
  tree->Branch("ctrl_trigger"     , &ctrl_trigger     ,      "ctrl_trigger/I");

   
  //riempimento del tree
  for(int n=0; n<nentries; n++){
    tree_raw->GetEntry(n);
    
    //functions defined in utility.h
    event           = event_raw;
    trigger         = trigger_raw;
    baseline        = GetBaseline(pshape);
    baseline_error  = GetBaselineError(pshape, baseline);
    charge          = GetCharge(pshape, dt_raw, baseline);
    amp             = GetAmp(pshape, baseline);

    ctrl             = Ctrl_pshape(pshape, amp, trigger, baseline);
    ctrl_double_low  = ctrl.get_ctrl_double_low();
    ctrl_double_high = ctrl.get_ctrl_double_high();
    ctrl_trigger     = ctrl.get_ctrl_trigger();
    ctrl_saturation  = ctrl.get_ctrl_saturation();
    ctrl_width       = ctrl.get_ctrl_width();

    tree->Fill();
  } //for on tree entries

  outfile->cd();
  tree->Write();

  std::cout << "tree entries: " << tree->GetEntries() << std::endl;
  std::cout << "tree saved in " << outfile_name << std::endl;
  outfile->Close();

  return(0);
}
	       	       
  

  
