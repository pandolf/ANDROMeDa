#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>

#include "TLegend.h"
#include "TStyle.h"
#include "TColor.h"
#include "TCanvas.h"

//stampa float in stringa come 0.0tot e non 0.0tot0000
template < typename Type > std::string to_str (const Type & t)
{
  std::ostringstream os;
  os << t;
  return os.str ();
}               


class Settings {
 public:

  Settings(){}
  ~Settings(){}
  
  static std::vector<int> colour(void);
  static void     general_style (void);
  static void     graphic_histo(TH1F* histo, float range_min, float range_max, const char* x_name, const char* y_name, int volt);
  static TLegend* setLegend(int      quadrant);
  static void     Margin   (TCanvas* c       );
  static double   Maximum  (TH1F*    histo   );
  static double   Binsize  (float    binsize );
};


//array of colours to be used with multiple lines graphs
std::vector<int> Settings::colour(void) {
  std::vector<int> colour;
  colour.push_back(899); //kPink-1
  colour.push_back(808); //kOrange+8
  colour.push_back(800); //korange0
  colour.push_back(416); //kGreen0
  colour.push_back(834); //kTeal-6
  colour.push_back(868); //kAzure+8
  colour.push_back(593); //kBlue-7
  colour.push_back(879); //kViolet-1
  colour.push_back(907); //kPink+7
  colour.push_back(901); //kPink+1;
  colour.push_back(806); //kOrange+6
  colour.push_back(834); //kTeal-6
  colour.push_back(597); //kBlue-3
  colour.push_back(871); //kViolet-9
  colour.push_back(612); //kMagenta-4
  colour.push_back(625); //kRed-7
  return colour;
}

//posizione e grandezza legenda
TLegend* Settings::setLegend(int quadrant) {
  TLegend* legend;
  //x1,y1 (su sx); x2,y2 (giù dx)
  //quadranti come circonferenza goniometrica
  if(quadrant==1) legend = new TLegend(0.70, 0.92, 1.06, 0.67);
  if(quadrant==2) legend = new TLegend(0.15, 0.97, 0.47, 0.57);
  if(quadrant==3) legend = new TLegend(0.12, 0.22, 0.29, 0.12);
  if(quadrant==4) legend = new TLegend(0.70, 0.25, 0.99, 0.12);
  legend->SetBorderSize(0);
  legend->SetFillStyle(0);

  return legend;
}


double Settings::Maximum(TH1F* histo){
  return (histo->GetBinContent(histo->GetMaximumBin()))*1.1;
}


//margine della tela
void Settings::Margin(TCanvas* c){
  c->SetTopMargin(0.09);
  c->SetBottomMargin(0.13);//0.13);
  c->SetLeftMargin(0.14);//0.15);
  c->SetRightMargin(0.06);//0.02);
}


//nomi, font e punti
void Settings::graphic_histo(TH1F* histo, float range_min, float range_max, const char* x_name, const char* y_name, int volt){
  histo->GetXaxis()->SetRangeUser(range_min, range_max);
  histo->GetXaxis()->SetTitle(x_name);
  histo->GetYaxis()->SetTitle(y_name);
  gStyle->SetTitleFontSize(0.07);

  histo->GetXaxis()->SetLabelSize(0.05);
  histo->GetXaxis()->SetTitleSize(0.05);
  histo->GetYaxis()->SetLabelSize(0.05);
  histo->GetYaxis()->SetTitleSize(0.05);
  histo->SetTitle(Form("%d V",volt));
 
  histo->SetMarkerSize(1.5);
  histo->SetMarkerStyle(20);
  histo->SetLineColor(kBlack);
  histo->SetLineWidth(2);

}


void Settings::general_style(void){
  gStyle->SetFrameLineWidth(2);
  gStyle->SetOptStat(0);
}


double Settings::Binsize (float binsize){
  float size = 0;
  float factor = 1;
  if (binsize > 0.0001 && binsize < 0.000999){ factor = 1e+4;}
  if (binsize > 0.0010 && binsize < 0.009990){ factor = 1e+3;}
  if (binsize > 0.0100 && binsize < 0.099900){ factor = 1e+2;}
  if (binsize > 0.1000 && binsize < 0.999000){ factor = 1e+1;}
  size = round(binsize*factor);
  size/= factor;
  return(size);
}


//CLASS OF HISTO VARIABLES
class Histo{
 public:
  //constructors and destructos
  Histo(){}
  ~Histo(){}

  //getters
  int   get_nbins     (void) const { return nbins;     }
  float get_range_min (void) const { return range_min; }
  float get_range_max (void) const { return range_max; }
  float get_fit_min   (void) const { return fit_min;   }
  float get_fit_max   (void) const { return fit_max;   }
  std::string get_x_name (void) const { return x_name; }
  std::string get_y_name (void) const { return y_name; }

  //setters
  void set_nbins     (int   a) { nbins     = a; }
  void set_range_min (float b) { range_min = b; }
  void set_range_max (float c) { range_max = c; }
  void set_fit_min   (float d) { fit_min   = d; }
  void set_fit_max   (float e) { fit_max   = e; }
  void set_x_name (std::string f) { x_name = f; }
  void set_y_name (std::string g) { y_name = g; }

 private:
  int   nbins;
  float range_min;
  float range_max;
  float fit_min;
  float fit_max;
  std::string x_name;
  std::string y_name;
};



//CLASS OF FIT VARIABLES
class Fit{
 public:
  //constructors and destructos
  Fit(){}
  ~Fit(){}

  //getters
  float get_mu_min    (void) const { return mu_min;    }
  float get_mu_max    (void) const { return mu_max;    }
  float get_mu        (void) const { return mu;        }
  float get_k_min     (void) const { return k_min;     }
  float get_k_max     (void) const { return k_max;     }
  float get_k         (void) const { return k;         }
  float get_sigma_min (void) const { return sigma_min; }
  float get_sigma_max (void) const { return sigma_max; }
  float get_sigma     (void) const { return sigma;     }
  float get_alfa_min  (void) const { return alfa_min;  } 
  float get_alfa_max  (void) const { return alfa_max;  }
  float get_alfa      (void) const { return alfa;      }
  float get_A_min     (void) const { return A_min;     }
  float get_A_max     (void) const { return A_max;     }
  float get_A         (void) const { return A;         }

  //setters
  void set_mu_min    (float a) { mu_min    = a; }
  void set_mu_max    (float b) { mu_max    = b; }
  void set_mu        (float c) { mu        = c; }
  void set_k_min     (float d) { k_min     = d; }
  void set_k_max     (float e) { k_max     = e; }
  void set_k         (float f) { k         = f; }
  void set_sigma_min (float g) { sigma_min = g; }
  void set_sigma_max (float h) { sigma_max = h; }
  void set_sigma     (float i) { sigma     = i; }
  void set_alfa_min  (float j) { alfa_min  = j; }
  void set_alfa_max  (float k) { alfa_max  = k; }
  void set_alfa      (float l) { alfa      = l; }
  void set_A_min     (float m) { A_min     = m; }
  void set_A_max     (float n) { A_max     = n; }
  void set_A         (float o) { A         = o; }

 private:
  float    mu_min; float    mu_max; float mu   ;
  float     k_min; float     k_max; float k    ;
  float sigma_min; float sigma_max; float sigma;
  float  alfa_min; float  alfa_max; float alfa ;
  float     A_min; float     A_max; float A    ;
 
};

