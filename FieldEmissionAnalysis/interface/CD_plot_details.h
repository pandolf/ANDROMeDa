//funzioni con dettagli per plottare grafici finali
//divisione tra CD per non avere funzioni chilometriche
//classi Histo e Fit definite in graphics.h

//variabili per riprodurre i plot finali
Histo SetAllVariables_CD188(int V, char* choice){
  Histo h;
  int   nbins = 0;
  float range_min = 0; float range_max = 0;
  float fit_min   = 0; float fit_max   = 0;
  float binsize;
  std::string x_name;
  std::string y_name;

  //AMPIEZZA
  if(strcmp(choice,"amp")==0){
    x_name = "Amplitude (V)";
    //graphic and fit conditions
    if     (V == 95 ){ nbins=220; range_min=0.7 ; range_max=0.9 ; fit_min=0.795; fit_max=0.86; }
    else if(V == 96 ){ nbins=170; range_min=0.7 ; range_max=0.9 ; fit_min=0.79 ; fit_max=0.85; }
    else if(V == 97 ){ nbins=209; range_min=0.7 ; range_max=0.9 ; fit_min=0.785; fit_max=0.85; }
    else if(V == 98 ){ nbins=188; range_min=0.72; range_max=0.91; fit_min=0.79 ; fit_max=0.86; }
    else if(V == 99 ){ nbins=213; range_min=0.72; range_max=0.9 ; fit_min=0.79 ; fit_max=0.86; }
    else if(V == 100){ nbins=210; range_min=0.73; range_max=0.88; fit_min=0.79 ; fit_max=0.84; }
    else if(V == 101){ nbins=213; range_min=0.73; range_max=0.9 ; fit_min=0.79 ; fit_max=0.86; }
    else if(V == 102){ nbins=204; range_min=0.73; range_max=0.92; fit_min=0.80 ; fit_max=0.88; }
    else if(V == 103){ nbins=225; range_min=0.73; range_max=0.9 ; fit_min=0.80 ; fit_max=0.88; }
    else if(V == 105){ nbins=215; range_min=0.76; range_max=0.95; fit_min=0.84 ; fit_max=0.91; }
    else if(V == 104){ ; } //fit davvero brutto perchè i dati sono piatteggianti
    else { std::cout << "voltaggio non presente!" << std::endl; exit(1); }

    binsize = Settings::Binsize((range_max-range_min)/nbins);
    y_name  = "Counts / "+ to_str(binsize) + " V";
  }
  
  
  //CARICA
  else if(strcmp(choice,"charge")==0){
    x_name = "Charge (uC)";
    //graphic and fit conditions
    if     (V == 95 ){ nbins=450 ; range_min=8.5 ; range_max=12.5; fit_min=10.2; fit_max=11  ; }
    else if(V == 96 ){ nbins=262 ; range_min=8.5 ; range_max=12.5; fit_min=10.2; fit_max=11.5; }
    else if(V == 97 ){ nbins=325 ; range_min=7.5 ; range_max=13  ; fit_min=10  ; fit_max=11.5; }
    else if(V == 98 ){ nbins=204 ; range_min=6   ; range_max=14  ; fit_min=10.1; fit_max=11.5; }
    else if(V == 99 ){ nbins=315 ; range_min=8.5 ; range_max=13  ; fit_min=10.2; fit_max=11.5; }
    else if(V == 100){ nbins=460 ; range_min=8.5 ; range_max=12.5; fit_min=10.3; fit_max=11.3; }
    else if(V == 101){ nbins=325 ; range_min=9   ; range_max=13  ; fit_min=10.9; fit_max=12  ; }
    else if(V == 102){ nbins=1000; range_min=9.5 ; range_max=14  ; fit_min=11.5; fit_max=13  ; }
    else if(V == 103){ nbins=350 ; range_min=10  ; range_max=14  ; fit_min=11.8; fit_max=13  ; }
    else if(V == 104){ nbins=930 ; range_min=11  ; range_max=14.5; fit_min=12.7; fit_max=13.6; }
    else if(V == 105){ nbins=850 ; range_min=13.5; range_max=18.5; fit_min=15.6; fit_max=17.5; }
    else { std::cout << "voltaggio non presente!" << std::endl; }

    binsize = Settings::Binsize((range_max-range_min)/nbins);
    y_name  = "Counts / "+ to_str(binsize) + " uC";
  }

  //assegnazione variabili della classe
  h.set_nbins(nbins);
  h.set_range_min(range_min);
  h.set_range_max(range_max);
  h.set_fit_min(fit_min);
  h.set_fit_max(fit_max);
  h.set_x_name(x_name);
  h.set_y_name(y_name);

  return h;
}


//variabili per riprodurre i plot finali
Histo SetAllVariables_CD190(int V, char* choice, int picco){
  Histo h;
  int   nbins = 0;
  float range_min = 0; float range_max = 0;
  float fit_min   = 0; float fit_max   = 0;
  float binsize;
  std::string x_name;
  std::string y_name;

 
  //CARICA
  if(strcmp(choice,"charge")==0){
     x_name = "Charge (uC)";

     if(picco == 1){
      //graphic and fit conditions
      if     (V == 130){ nbins=700 ; range_min=9   ; range_max=13  ; fit_min=10.8; fit_max=12  ; }
      else if(V == 135){ nbins=560 ; range_min=10  ; range_max=13  ; fit_min=11.2; fit_max=12.6; }
      else if(V == 140){ nbins=780 ; range_min=10  ; range_max=14  ; fit_min=11.4; fit_max=12.6; }
      else if(V == 142){ nbins=730 ; range_min=10  ; range_max=14.5; fit_min=11.8; fit_max=13  ; }
      else if(V == 143){ nbins=660 ; range_min=10.5; range_max=14.5; fit_min=12  ; fit_max=13.2; }
      else if(V == 144){ nbins=815 ; range_min=10.5; range_max=14.5; fit_min=12  ; fit_max=13.2; }
      else if(V == 145){ nbins=770 ; range_min=10  ; range_max=15  ; fit_min=12.2; fit_max=13.6; }
      else if(V == 146){ nbins=970 ; range_min=12.5; range_max=16.5; fit_min=14  ; fit_max=15.5; }
      else if(V == 147){ nbins=1760; range_min=10  ; range_max=15.5; fit_min=12.2; fit_max=13.5; }
      else if(V == 148){ nbins=1350; range_min=10  ; range_max=15.5; fit_min=12.4; fit_max=13.4; }
      else if(V == 149){ nbins=1180; range_min=10.5; range_max=16  ; fit_min=12.8; fit_max=14  ; }
      else if(V == 150){ nbins=1120; range_min=10.5; range_max=16  ; fit_min=12.7; fit_max=14.2; }
      else if(V == 141){ ; } //dati presi in condizioni diverse
      else { std::cout << "voltaggio non presente!" << std::endl; exit(1); }
    }

      if(picco == 2){
      //graphic and fit conditions
      if     (V == 140){ nbins=60  ; range_min=15 ; range_max=27 ; fit_min=16.5; fit_max=23 ; }
      else if(V == 142){ nbins=65  ; range_min=14 ; range_max=28 ; fit_min=16.5; fit_max=23 ; }
      else if(V == 143){ nbins=85  ; range_min=14 ; range_max=27 ; fit_min=16  ; fit_max=27 ; }
      else if(V == 144){ nbins=67  ; range_min=14 ; range_max=30 ; fit_min=17  ; fit_max=27 ; }
      else if(V == 145){ nbins=146 ; range_min=15 ; range_max=29 ; fit_min=19  ; fit_max=25 ; }
      else if(V == 146){ nbins=119 ; range_min=16 ; range_max=32 ; fit_min=18  ; fit_max=29 ; }
      else if(V == 147){ nbins=366 ; range_min=15 ; range_max=29 ; fit_min=19  ; fit_max=25 ; }
      else if(V == 148){ nbins=158 ; range_min=15 ; range_max=29 ; fit_min=16  ; fit_max=25 ; }
      else if(V == 149){ nbins=200 ; range_min=15 ; range_max=31 ; fit_min=18.5; fit_max=28 ; }
      else if(V == 150){ nbins=237 ; range_min=16 ; range_max=33 ; fit_min=18.5; fit_max=30 ; }
      else if(V == 130 || V == 135 || V == 141){ ; }  //voltaggi bassi non hanno picco delle doppie 
      else { std::cout << "voltaggio non presente!" << std::endl; exit(1); }
      }
      
      binsize = Settings::Binsize((range_max-range_min)/nbins);
      y_name  = "Counts / "+ to_str(binsize) + " uC";
  }

  else if (strcmp(choice,"amp")==0){ ; }
  //studio sull'ampiezza ancora non eseguito
  
   /*
  //AMPIEZZA
  if(strcmp(choice,"amp")==0){
    x_name = "Amplitude (V)";
    //graphic and fit conditions
    if     (V == 130){ nbins=220; range_min=0.7 ; range_max=0.9 ; fit_min=0.795; fit_max=0.86; y_name="Counts / 0.004 V"; }
    else if(V == 135){ nbins=170; range_min=0.7 ; range_max=0.9 ; fit_min=0.79 ; fit_max=0.85; y_name="Counts / 0.004 V"; }
    else if(V == 140){ nbins=209; range_min=0.7 ; range_max=0.9 ; fit_min=0.785; fit_max=0.85; y_name="Counts / 0.004 V"; }
    else if(V == 142){ nbins=188; range_min=0.72; range_max=0.91; fit_min=0.79 ; fit_max=0.86; y_name="Counts / 0.003 V"; }
    else if(V == 143){ nbins=213; range_min=0.72; range_max=0.9 ; fit_min=0.79 ; fit_max=0.86; y_name="Counts / 0.003 V"; }
    else if(V == 144){ nbins=210; range_min=0.73; range_max=0.88; fit_min=0.79 ; fit_max=0.84; y_name="Counts / 0.004 V"; }
    else if(V == 145){ nbins=213; range_min=0.73; range_max=0.9 ; fit_min=0.79 ; fit_max=0.86; y_name="Counts / 0.004 V"; }
    else if(V == 146){ nbins=204; range_min=0.73; range_max=0.92; fit_min=0.80 ; fit_max=0.88; y_name="Counts / 0.005 V"; }
    else if(V == 147){ nbins=225; range_min=0.73; range_max=0.9 ; fit_min=0.80 ; fit_max=0.88; y_name="Counts / 0.004 V"; }
    else if(V == 148){ nbins=215; range_min=0.76; range_max=0.95; fit_min=0.84 ; fit_max=0.91; y_name="Counts / 0.004 V"; }
    else if(V == 149){ nbins=215; range_min=0.76; range_max=0.95; fit_min=0.84 ; fit_max=0.91; y_name="Counts / 0.004 V"; }
    else if(V == 150){ nbins=215; range_min=0.76; range_max=0.95; fit_min=0.84 ; fit_max=0.91; y_name="Counts / 0.004 V"; }
    else if(V == 141){ continue; } //dati presi in condizioni diverse...
    else { std::cout << "voltaggio non presente!" << std::endl; exit(1); }
  }
  */
  
  //assegnazione variabili della classe
  h.set_nbins(nbins);
  h.set_range_min(range_min);
  h.set_range_max(range_max);
  h.set_fit_min(fit_min);
  h.set_fit_max(fit_max);
  h.set_x_name(x_name);
  h.set_y_name(y_name);

  return h;
}


//variabili per riprodurre i fit
Fit SetFitVariables_CD188(char* choice, char* fit_fcn){
  Fit f;

  float    mu_min = 0; float    mu_max = 0; float    mu_ = 0;
  float     k_min = 0; float     k_max = 0; float     k_ = 0;
  float sigma_min = 0; float sigma_max = 0; float sigma_ = 0;
  float  alfa_min = 0; float  alfa_max = 0; float  alfa_ = 0;
  float     A_min = 0; float     A_max = 0; float     A_ = 0;

  //inizializzazione generale k e alfa per la cruijff
  if(strcmp(fit_fcn,"cruijff")==0){
    alfa_min = 0   ; alfa_max = 10  ; alfa_ = 0.1 ;
       k_min = 0.57;    k_max = 1.13;    k_ = 0.84;
  }
  
  //PICCO UNICO
  //AMPIEZZA
  if(strcmp(choice,"amp")==0){
        A_min = 1    ;     A_max = 1000;     A_ = 10 ;
       mu_min = 0.7  ;    mu_max = 0.9 ;    mu_ = 0.8;
    sigma_min = 0.001; sigma_max = 1   ; sigma_ = 0.1;   
    }

  //CARICA
  if(strcmp(choice,"charge")==0){
        A_min = 1  ;      A_max = 3000;      A_  = 1 ;
       mu_min = 9  ;     mu_max = 18  ;     mu_  = 11;
    sigma_min = 0.1;  sigma_max = 10  ;  sigma_  = 1 ;
  }

  f.set_mu_min(mu_min)      ; f.set_mu_max(mu_max)      ; f.set_mu(mu_)      ;
  f.set_k_min(k_min)        ; f.set_k_max(k_max)        ; f.set_k(k_)        ;
  f.set_sigma_min(sigma_min); f.set_sigma_max(sigma_max); f.set_sigma(sigma_);
  f.set_alfa_min(alfa_min)  ; f.set_alfa_max(alfa_max)  ; f.set_alfa(alfa_)  ;
  f.set_A_min(A_min)        ; f.set_A_max(A_max)        ; f.set_A(A_)        ;

  return(f);
}


//variabili per riprodurre i fit
Fit SetFitVariables_CD190(char* choice, char* fit_fcn, int picco){
  Fit f;

  float    mu_min = 0; float    mu_max = 0; float    mu_ = 0;
  float     k_min = 0; float     k_max = 0; float     k_ = 0;
  float sigma_min = 0; float sigma_max = 0; float sigma_ = 0;
  float  alfa_min = 0; float  alfa_max = 0; float  alfa_ = 0;
  float     A_min = 0; float     A_max = 0; float     A_ = 0;

  //inizializzazione generale k e alfa per la cruijff
  if(strcmp(fit_fcn,"cruijff")==0){
    alfa_min = 0   ; alfa_max = 1   ; alfa_ = 0   ;
       k_min = 0.57;    k_max = 1.13;    k_ = 0.84;
  }
   
  //CARICA
  if(strcmp(choice,"charge")==0){
    if(picco == 1){
          A_min = 1  ;      A_max = 3000;      A_  = 1 ;
         mu_min = 9  ;     mu_max = 18  ;     mu_  = 11;
      sigma_min = 0.1;  sigma_max = 10  ;  sigma_  = 1 ;
    }

    if(picco == 2){
          A_min = 1  ;      A_max = 3000;      A_  = 1 ;
         mu_min = 10 ;     mu_max = 25  ;     mu_  = 11;
      sigma_min = 0.1;  sigma_max = 10  ;  sigma_  = 1 ;
    }
  }
  
  f.set_mu_min(mu_min)      ; f.set_mu_max(mu_max)      ; f.set_mu(mu_)      ;
  f.set_k_min(k_min)        ; f.set_k_max(k_max)        ; f.set_k(k_)        ;
  f.set_sigma_min(sigma_min); f.set_sigma_max(sigma_max); f.set_sigma(sigma_);
  f.set_alfa_min(alfa_min)  ; f.set_alfa_max(alfa_max)  ; f.set_alfa(alfa_)  ;
  f.set_A_min(A_min)        ; f.set_A_max(A_max)        ; f.set_A(A_)        ;

  return(f);
}
  

 
