// IVScan dedicated to Fowler–Nordheim analysis

#ifndef IVScanFN_h
#define IVScanFN_h


#include "../interface/IVScan.h"




class IVScanFN : public IVScan {

 public:

  IVScanFN( const std::string& name, float scale=1., float iMin=0., float iMax=1E9 );   //, float vMin = -99999., float vMax = 99999. );

  ~IVScanFN();

  TGraphErrors* graphFN() const;
  static TGraphErrors* graphFN( const IScan* scan, float iMin = 0., float iMax = 1E9 );
  static TGraphErrors* graphFN( TGraphErrors* graph, float iMin = 0., float iMax = 1E9, int n = 1, float verr = 0.1 );

  //TGraphErrors* graphFN() const;
  //TF1* lineFN() const;

  float get_gamma_and_err( float& gamma_err_uncorr, float& gamma_err_corr, float d_errcorr=-1. );
  static float get_gamma_and_err( float& gamma_err_uncorr, float& gamma_err_corr, float s, float s_err, float d, float d_errcorr=-1. );

  static float phi();
  static float phi_err();
  static float b();
  static float d_err_uncorr();
  static float d_err_corr();

  float iMin() const;
  float iMax() const;

  //float gamma() const;
  //float gamma_err() const;

  void set_iMin( float iMin );
  void set_iMax( float iMax );
  void set_iMinMax( float iMin, float iMax );

  //virtual void setColor( int color );

  static std::string xTitleFN();
  static std::string yTitleFN();



 private:

  //TGraphErrors* graphFN_; // F-N graph: Log(I/V^2) vs 1/V

  //void set_graphFN();

  float iMin_;
  float iMax_;

  //float gamma_; // field enhancement factor
  //float gamma_err_;


};


#endif
