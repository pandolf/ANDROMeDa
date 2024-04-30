#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
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
#include "CD_plot_details.h"

void  Input_Param_Ctrls(int argc, char* argv[]);
void  Voltage_Initialization(int CD_number, int voltage);
Double_t Cruijff(Double_t *x, Double_t *par);
  
int main(int argc, char* argv[]){

  //controlli su parametri da input
  Input_Param_Ctrls(argc, argv);

  //parametri da input
  int   CD_number = (atoi(argv[1])); //cooldown
  int   voltage   = (atoi(argv[2]));
  char* choice    =       argv[3]  ;
  char* fit_fcn   =       argv[4]  ;
  int   peak      = (atoi(argv[5])); 

  //impostazioni grafiche
  float magnifying = 1; //per avere numeri in carica più comodi (sono in E-6 sennò)
  float offset     = 0; //per allungare il range di disegno permesso wrt var_min/var_max
  if      (strcmp(choice,"amp"   )==0){ magnifying = 1e+0; offset = 0.1;}
  else if (strcmp(choice,"charge")==0){ magnifying = 1e+6; offset = 5  ;}
  
  //classe di variabili per il fit definita in graphics.h
  Fit fit; 
  if(CD_number==188){ fit = SetFitVariables_CD188(choice, fit_fcn      ); }
  if(CD_number==190){ fit = SetFitVariables_CD190(choice, fit_fcn, peak); }

  //massimi, minimi e valori iniziali
  float    mu_min = fit.get_mu_min()   ;  float    mu_max = fit.get_mu_max()   ;  float    mu_ = fit.get_mu();
  float     k_min = fit.get_k_min()    ;  float     k_max = fit.get_k_max()    ;  float     k_ = fit.get_k();
  float sigma_min = fit.get_sigma_min();  float sigma_max = fit.get_sigma_max();  float sigma_ = fit.get_sigma();
  float  alfa_min = fit.get_alfa_min() ;  float  alfa_max = fit.get_alfa_max() ;  float  alfa_ = fit.get_alfa();
  float     A_min = fit.get_A_min()    ;  float     A_max = fit.get_A_max()    ;  float     A_ = fit.get_A();

 
  //apertura file con il tree
  TFile run(Form("tree/CD%d_%dV_tree.root",CD_number,voltage));
  TTree *tree = (TTree*)run.Get("tree");
  Long64_t nentries = tree->GetEntries();
  
  //preparazione per la lettura del tree
  float variable;
  tree->SetBranchAddress(choice, &variable);
  float var_min = tree->GetMinimum(choice)*magnifying;
  float var_max = tree->GetMaximum(choice)*magnifying;
  
  //impostazione variabili dell'histo definita in CD_plot_details.h
  Histo histo_var; //classe di variabili dell'histo definita in graphics.h
  //funzioni divise per non renderle chilometriche
  if(CD_number==188){ histo_var = SetAllVariables_CD188(voltage,choice); }
  if(CD_number==190){ histo_var = SetAllVariables_CD190(voltage,choice, peak); }
  
  int   nbins        = histo_var.get_nbins()    ;
  float range_min    = histo_var.get_range_min(); float range_max    = histo_var.get_range_max();
  float fit_min      = histo_var.get_fit_min()  ; float fit_max      = histo_var.get_fit_max()  ;
  std::string x_name = histo_var.get_x_name()   ; std::string y_name = histo_var.get_y_name();

  //tela
  Settings settings;
  TCanvas* c = new TCanvas("c","",1400,1200);
  settings.Margin(c);
  c->cd();  
  
  //istogramma e suo riempiento
  TH1F* histo = new TH1F("histo", "", nbins, var_min-offset, var_max+offset);
  for(int iEntry=0; iEntry<nentries; iEntry++){
    tree->GetEntry(iEntry);
    histo->Fill(variable*magnifying);
  }
  
  //funzione di fit (se è gaussiana, fixerà k e alfa)
  TF1 *cruijff  = new TF1(fit_fcn , Cruijff, fit_min, fit_max, 5);
  
  //parametri del fit 
  cruijff->SetParNames("#mu","k", "#sigma_{R}", "#alpha", "A");
  cruijff->SetParLimits(0,    mu_min,    mu_max);
  cruijff->SetParLimits(1,     k_min,     k_max); 
  cruijff->SetParLimits(2, sigma_min, sigma_max);
  cruijff->SetParLimits(3,  alfa_min,  alfa_max);
  cruijff->SetParLimits(4,     A_min,     A_max);
  
  cruijff->SetParameters(mu_, k_, sigma_, alfa_, A_); //parametri iniziali
  
  if(strcmp(fit_fcn,"gauss")==0){ //cruijff -> gaussiana
    cruijff->FixParameter(1,1); //k value
    cruijff->FixParameter(3,0); //alfa value
  }
  
  cruijff->SetLineColor(kRed);
  cruijff->SetLineWidth(4);
  
  histo->Fit(fit_fcn,"rq"); //in range

  if(voltage==104 || voltage==141){ //voltaggi che stiamo escludendo
    std::cout << "excluded voltages as data are not good" << std::endl;
    std::cout << "if you still want them update the functions with ranges and fit values" << std::endl;
  }

  //creazione grafico dell'histo
  settings.graphic_histo(histo,range_min,range_max,x_name.c_str(),y_name.c_str(),voltage);
  settings.general_style();
  
  histo->SetNdivisions(5+1*500,"X");
  histo->SetMarkerSize(3);
  histo->SetLineWidth(4);
  histo->Draw("pe1");
  cruijff->Draw("same");
  c->SaveAs(Form("plots/CD %d/final plots/%dV_%s_picco%d_%s.png",CD_number,voltage,choice,peak,fit_fcn));
  
  delete(histo);
  delete(cruijff);
  delete(c);
  
  return(0);
}


//controllo su parametri da input
void Input_Param_Ctrls(int argc, char* argv[]){
  //parametri da input
  if (argc!=6){
    std::cout << "USAGE: ./drawAmpChargeFinalFits_single [CD_number] [voltage] [amp/charge] [cruijff/gauss] [picco]" << std::endl;
    std::cout << "If there's only one peak, type 1" << std::endl;
    exit(1);
  }
  
  //ctrl su scelta amp/charge
  if(strcmp(argv[3],"amp"   )!=0 &&
     strcmp(argv[3],"charge")!=0 ){
    std::cout << "Wrong choice! Choose between 'amp' and 'charge'" << std::endl;
    exit(1);
  }

  //ctrl su scelta cruijff/gauss
  if(strcmp(argv[4],"cruijff")!=0 &&
     strcmp(argv[4],"gauss"  )!=0 ){
    std::cout << "Wrong choice! Choose between 'cruijff' and 'gauss'" << std::endl;
    exit(1);
  }
}

//cruiff da fittare (che all'occorrenza diventa gaussiana)
Double_t Cruijff(Double_t *x, Double_t *par){
  //par[0]=mean      par[1]=costante 0<k<1    par[2]=sigma_R
  //par[3]=alpha_L   par[4]=A
  Double_t fit   = 0;
  Double_t arg_L = 0;
  Double_t arg_R = 0;
  Double_t num   = pow((x[0]-par[0]),2);
  Double_t denL  = 2*par[2]*par[2]*par[1]*par[1];
  Double_t denR  = 2*par[2]*par[2];
  Double_t asym  = par[3]*num;
  if ( denL + asym != 0 ){ arg_L = -num/(denL+asym); }
  if ( par[2]      != 0 ){ arg_R = -num/ denR      ; }

  if       ( x[0] < par[0] ){ fit = par[4] * exp(arg_L); } //LEFT
  else if  ( x[0] > par[0] ){ fit = par[4] * exp(arg_R); } //RIGHT

  return(fit);
}

