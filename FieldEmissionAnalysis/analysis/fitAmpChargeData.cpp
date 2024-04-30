#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <stdlib.h>

#include "TFile.h"
#include "TF1.h"
#include "TTree.h"
#include "TString.h"
#include "TH2D.h"
#include "TMath.h"
#include "TAxis.h"
#include "pshape_functions.h"
#include "graphics.h"

//variabili da modificare di volta in volta: range_min, range_max, fit_min, fit_max, nbins

void Input_Param_Ctrls(int argc, char* argv[]);
Double_t Cruijff(Double_t *x, Double_t *par);

int main(int argc, char* argv[]){

  //controlli iniziali su variabili da input
  Input_Param_Ctrls(argc,argv);

  //parametri da input
  int   CD_number = (atoi(argv[1])); //cooldown
  int   voltage   = (atoi(argv[2]));
  char* choice    =       argv[3]  ;
  int   peak      = (atoi(argv[4]));
  char* fit_fcn   =       argv[5]  ;
  
  //apertura file con il tree
  TFile run(Form("tree/CD%d_%dV_tree.root",CD_number,voltage));
  TTree *tree = (TTree*)run.Get("tree");
  Long64_t nentries = tree->GetEntries();
 
  //preparazione per la lettura del tree
  float variable;
  tree->SetBranchAddress(choice, &variable);

  //impostazioni grafiche
  float magnifying = 1; //per avere numeri in carica più comodi (sono in E-6 sennò)
  float offset     = 0; //per allungare il range di disegno permesso wrt var_min/var_max
  if      (strcmp(choice,"amp"   )==0){ magnifying = 1e+1; offset = 0.1;}
  else if (strcmp(choice,"charge")==0){ magnifying = 1e+6; offset = 5  ;}

  //variabili per l'istogramma
  int   nbins     = 780;
  float var_min   = tree->GetMinimum(choice)*magnifying;
  float var_max   = tree->GetMaximum(choice)*magnifying;
  float range_min = 0;
  float range_max = 20;

  Settings settings;
  //funzione che tiene solo una cifrq significativa per passare a stringa
  float binsize = settings.Binsize((var_max-var_min)/nbins); 

  const char* x_name; //x axis label
  std::string y_name; //y axis label 
  if      (strcmp(choice,"amp"   )==0){
    x_name = "Amplitude [V]";
    y_name = "Counts / " + to_str(binsize) + " V" ;
  }
  else if (strcmp(choice,"charge")==0){
    x_name = "Charge [uC]"  ;
    y_name = "Counts / " + to_str(binsize)  + " uC";
  }
  
  //creazione istogramma e lettura del tree
  TH1F* histo = new TH1F("", choice, nbins, var_min-offset, var_max+offset);
  
  for(int iEntry=0; iEntry<nentries; iEntry++){
    tree->GetEntry(iEntry);
    histo->Fill(variable*magnifying);
  }
  
  //fitting options
  double fit_min = 0; double fit_max = 0;

  float    mu_min = 0; float    mu_max = 0; float    mu_ = 0;
  float     k_min = 0; float     k_max = 0; float     k_ = 0;
  float sigma_min = 0; float sigma_max = 0; float sigma_ = 0;
  float  alfa_min = 0; float  alfa_max = 0; float  alfa_ = 0;
  float     A_min = 0; float     A_max = 0; float     A_ = 0;

  //inizializzazione generale k e alfa per la cruijff
  if(strcmp(fit_fcn,"cruijff")==0){
       k_min = 0.57;    k_max = 1.13;    k_ = 0.84;
    alfa_min = 0   ; alfa_max = 1   ; alfa_ = 0   ;
  }

  //inizializzazione parametri di fit
  //CARICA
  if(strcmp(choice,"charge")==0){
    //PICCO 1
    if(peak == 1){
        fit_min = 11  ;   fit_max = 15  ;
          A_min = 1   ;     A_max = 3000;     A_ = 1 ;
         mu_min = 9   ;    mu_max = 18  ;    mu_ = 12;
      sigma_min = 0.10; sigma_max = 10  ; sigma_ = 1 ;   
    }
    if(peak == 2){
      //PICCO 2
        fit_min = 16.5;   fit_max = 23  ;
          A_min = 1   ;     A_max = 3000;     A_ = 22;
         mu_min = 10  ;    mu_max = 25  ;    mu_ = 11;
      sigma_min = 0.10; sigma_max = 10  ; sigma_ = 1 ;
    }
  }

  //AMPIEZZA
  if(strcmp(choice,"amp")==0){
    //PICCO UNICO
        fit_min = 0.75 ;   fit_max = 0.85;
          A_min = 1    ;     A_max = 3000;     A_ = 1  ;
         mu_min = 0.75 ;    mu_max = 0.85;    mu_ = 11 ;
      sigma_min = 0.001; sigma_max = 1   ; sigma_ = 0.1;   
  }

  //funzione di fit (se è gaussiana, fixerà k e alfa)
  //definita in fondo al programma
  TF1 *cruijff  = new TF1(fit_fcn, Cruijff, fit_min, fit_max, 5);

  //parametri del fit
  cruijff->SetParNames("#mu","k", "#sigma_{R}", "#alpha", "A");
  cruijff->SetParLimits(0,   mu_min,   mu_max);
  cruijff->SetParLimits(1,    k_min,    k_max); 
  cruijff->SetParLimits(2,sigma_min,sigma_max);
  cruijff->SetParLimits(3, alfa_min, alfa_max);
  cruijff->SetParLimits(4,    A_min,    A_max);

  cruijff->SetParameters(mu_, k_, sigma_, alfa_, A_); //parametri iniziali
  
  if(strcmp(fit_fcn,"gauss")==0){ //cruijff -> gaussiana
    cruijff->FixParameter(1,1);  //k value
    cruijff->FixParameter(3,0); //alfa value
  }
  
  cruijff->SetLineColor(kRed);
  cruijff->SetLineWidth(4);

  //tela
  TCanvas* c = new TCanvas("c","",1400,1200);
  settings.Margin(c);
  c->cd();

  histo->Fit(fit_fcn,"r"); //in range
   
  //disegno dell'histo con funzione definita in graphics.h
  settings.graphic_histo(histo,range_min,range_max,x_name,y_name.c_str(),voltage);
  settings.general_style();
  gStyle->SetOptFit(1111);

  histo->SetMarkerSize(2);
  histo->SetLineWidth(2);
  histo->Draw("pe");
  cruijff->Draw("same");
  
  c->SaveAs(Form("plots/CD %d/%d V/%s_picco%d_%s.png",CD_number,voltage,choice,peak,fit_fcn));
  std::cout << "cruijff P: " << cruijff->GetProb() << std::endl;

  delete(cruijff);
  delete(histo);
  delete(c);
  
  return(0);
}


//funzione di controllo sui parametri iniziali
void Input_Param_Ctrls(int argc, char* argv[]){
  //ctrl sull'input inserito
  if (argc!=6){
    std::cout << "USAGE: ./fitAmpChargeData [CD_number] [voltage] [amp/charge] [peak] [cruijff/gauss]" << std::endl;
    std::cout << "[amp/charge]: variable studied"           << std::endl;
    std::cout << "[peak]: if there's only one peak, type 1" << std::endl;
    std::cout << "[cruijff/gauss]: fit function used"       << std::endl;
    exit(1);
  }

  //ctrl su scelta amp/charge
  if(strcmp(argv[3],"amp"   )!=0 &&
     strcmp(argv[3],"charge")!=0 ){ 
    std::cout << "Wrong choice! Choose between 'amp' and 'charge'" << std::endl;
    exit(1);
  }

  //ctrl su amp con picco!=1
  if(strcmp(argv[3],"amp")==0 && atoi(argv[4])!=1){
    std::cout << "Amplitude analysis concerns just one peak" << std::endl;
    exit(1);
  }

  //ctrl su scelta cruijff/gauss
  if(strcmp(argv[5],"cruijff")!=0 &&
     strcmp(argv[5],"gauss"  )!=0 ){
    std::cout << "Wrong choice! Choose between 'cruijff' and 'gauss'" << std::endl;
    exit(1);
  }
}


//cruiff (che può diventare una gaussiana) da fittare
Double_t Cruijff(Double_t *x, Double_t *par){
  //par[0]=mean      par[1]=costante 0<k<1    par[2]=sigma_R
  //par[3]=alpha_L   par[4]=A
  Double_t fitval = 0;
  Double_t arg_L  = 0;
  Double_t arg_R  = 0;
  Double_t num    = pow((x[0]-par[0]),2);
  Double_t denL   = 2*par[2]*par[2]*par[1]*par[1];
  Double_t denR   = 2*par[2]*par[2];
  Double_t asym   = par[3]*num;
  if ( denL + asym != 0 ){ arg_L = -num/(denL+asym); }
  if ( par[2]      != 0 ){ arg_R = -num/ denR      ; }

  if       ( x[0] < par[0] ){ fitval = par[4] * exp(arg_L); } //LEFT
  else if  ( x[0] > par[0] ){ fitval = par[4] * exp(arg_R); } //RIGHT

  return(fitval);
}


