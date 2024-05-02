//libreria di funzioni che ricavano info leggendo le pshapes

//BASELINE
float GetBaseline(float pshape[2500]){
  float baseline = 0;
  int   i_min    = 0;
  int   i_max    = 450; 		     

  for(int i=i_min; i<i_max; i++){
    baseline += pshape[i];
  }
  baseline /= (i_max-i_min);
  return baseline;
}


//STD_DEV BASELINE
float GetBaselineError(float pshape[2500], float baseline){
  float baseline_error = 0;
  int   i_min          = 0;
  int   i_max          = 450;	

  for(int i=i_min; i<i_max; i++){
    baseline_error += pow((pshape[i]-baseline),2);
  }
  baseline_error /= (i_max-i_min-1);
  return sqrt(baseline_error);
}


//CHARGE
float GetCharge(float pshape[2500], float dt, float baseline){
  float charge = 0;
  
  for(int i=0; i<2500; i++){
    charge += (pshape[i]-baseline);
  }
  
  charge *= dt;
  return -charge; //lavoro con carica>0
}


//AMPLITUDE
float GetAmp(float pshape[2500], float baseline){
  float amp = 0;
  float min = 0;
  
  for(int i=0; i<2500; i++){
    if (pshape[i]<min){
      min = pshape[i];
    }
  }
  amp = baseline - min; //ampiezza>0
  return amp; 
}



//CLASS OF CONTROLS OVER PSHAPE
class Controls{
 public:
  //constructors and destructos
  Controls(){}
  ~Controls(){}

  //getters
  int get_ctrl_double_low  (void) const { return ctrl_double_low;  }
  int get_ctrl_double_high (void) const { return ctrl_double_high; }
  int get_ctrl_trigger     (void) const { return ctrl_trigger;     }
  int get_ctrl_saturation  (void) const { return ctrl_saturation;  }
  int get_ctrl_width       (void) const { return ctrl_width;       }

  //setters
  void set_ctrl_double_low  (int a) { ctrl_double_low  = a; }
  void set_ctrl_double_high (int b) { ctrl_double_high = b; }
  void set_ctrl_trigger     (int c) { ctrl_trigger     = c; }
  void set_ctrl_saturation  (int d) { ctrl_saturation  = d; }
  void set_ctrl_width       (int e) { ctrl_width       = e; }

 private:
  int ctrl_double_low;  //ctrl sul superamento soglia a 30% ampiezza
  int ctrl_double_high; //ctrl sul superamento soglia a 80% ampiezza
  int ctrl_trigger;     //ctrl sull'indice della pshape in cui supera trigger
  int ctrl_saturation;  //numero di punti == ampiezza (ctrl saturazione)
  int ctrl_width;       //ctrl su larghezza onda a metà ampiezza

};


//CONTROL OVER PSHAPE 
Controls Ctrl_pshape(float pshape[2500], float amp, float trigger, float baseline){
  Controls ctrl;
  int ctrl_double_low  = 0;
  int ctrl_double_high = 0;
  int ctrl_trigger     = 0;
  int ctrl_saturation  = 0;
  int ctrl_width       = 0;

  //multiple event & width & trigger thresholds
  float threshold_low      = amp * 0.3; //30% -> ctrl doppie 
  float threshold_mid      = amp * 0.5; //50% -> width
  float threshold_high     = amp * 0.8; //80% -> ctrl doppie
  float trigger_threshold  = abs(trigger); //pshape studiata in positivo
 
  //width indexes
  int index_1 = 0;
  int index_2 = 0;
 
  //variabili per il ctrl doppie
  int   j_max = 20; //quanti punti medio dinamicamente
  float dyn_mean[2];
  int   counter [2500];
  int   index   [2500] = {0};

  //preparazione array media dinamica
  //se qualcuno non si fida, provi l'algoritmo a parte
  //ci metto troppo a spiegarlo :')
  //grazie per la fiducia
  for(int l=0; l<2500; l++){
    counter[l] = int(j_max/2);

    if(l < (     int(j_max/2)+1)){
      counter[l] -= (int(j_max/2)-l);
      index[l]   += (int(j_max/2)-l);
    }
    if(l > (2500-int(j_max/2)-1)){
      counter[l] -= (int(j_max/2)+l-2499);
    }
  }

  int i = 0;

  //while continua finchè non finisco pshape oppure ho un ctrl ancora non verificato
  while ((ctrl_double_low  == 0 ||
	  ctrl_double_high == 0 ||
	  ctrl_trigger     == 0 ||
	  ctrl_saturation  == 0 ||
 	  ctrl_width       == 0  ) &&
	  i < (2499)){ //leggo la pshape i+1

    //lavoro con l'onda in positivo
    pshape[i+1] = abs(pshape[i+1]);
    pshape[i]   = abs(pshape[i]  );

   
    //ctrl saturazione
    if( pshape[i] == amp-baseline && amp > 0.9 ){
      ctrl_saturation++;
    }

    //controllo del trigger
    if( pshape[i]   < trigger_threshold &&
	pshape[i+1] > trigger_threshold   ){
      if      (ctrl_trigger == 0) { ctrl_trigger = i+1; }
      else if (ctrl_trigger != 0) { ; }
    }

    //controllo sulla width 
    //superamento soglia in discesa
    if( pshape[i]   < threshold_mid &&
	pshape[i+1] > threshold_mid   ){
      if ( index_1 == 0 && index_2 == 0 ) { index_1 = i+1; }
      else                                { ; }
    }
    //superamento soglia in salita
    if( pshape[i]   > threshold_mid &&
	pshape[i+1] < threshold_mid   ){
      if ( index_1 != 0 && index_2 == 0 ) { index_2 = i+1; }
      else                                { ; }
    }
    

    
    //controllo sulle doppie con media dinamica
    //calcolo media dinamica
    dyn_mean[0] = 0;
    dyn_mean[1] = 0;
    for(int p=i; p<(i+2); p++){ //ciclo per riempire i due elementi di dyn_mean
      for(int j=0; j<(int(j_max/2)+1+counter[p]); j++){ 
	dyn_mean[p-i] += abs(pshape[p+j-int(j_max/2)+index[p]]);
      }
      //divisione che tiene conto delle condizioni al contorno (#punti usati)
      dyn_mean[p-i]/=(int(j_max/2)+1+counter[p]); 
    }
    
    
    //ctrl doppie con soglia bassa
    if( dyn_mean[0] < threshold_low &&
	dyn_mean[1] > threshold_low   ){
      ctrl_double_low++; }
    
    //ctrl doppie con soglia alta
    if( dyn_mean[0] < threshold_high &&
	dyn_mean[1] > threshold_high   ){
      ctrl_double_high++; }
    
    i++; //scorrimento sull'indice della pshape
  
  }//while sulla lettura pshape
   
  //calcolo della larghezza a metà ampiezza
  if (index_1 != 0 && index_2 != 0 ){
    ctrl_width = index_2 - index_1;
  }
  
  ctrl.set_ctrl_width(ctrl_width);
  ctrl.set_ctrl_trigger(ctrl_trigger);
  ctrl.set_ctrl_saturation(ctrl_saturation);
  ctrl.set_ctrl_double_low(ctrl_double_low);
  ctrl.set_ctrl_double_high(ctrl_double_high);
  
  return(ctrl);
}





