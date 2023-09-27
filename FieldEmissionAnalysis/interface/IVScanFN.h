#ifndef IVScanFN_h
#define IVScanFN_h

// IVScan dedicated to Fowler–Nordheim analysis


#include "../interface/IVScan.h"




class IVScanFN : public IVScan {

 public:

  IVScanFN( const std::string& name, float scale=1. );

  ~IVScanFN();

  TGraphErrors* getFNgraph() const;
  static TGraphErrors* getFNgraph( TGraphErrors* graph );

  TGraphErrors* graphFN() const;
  TF1* lineFN() const;

  static float get_gamma_and_err( float& gamma_err_uncorr, float& gamma_err_corr, float s, float s_err, float d, float d_err=-1. );

  static float phi();
  static float phi_err();
  static float b();
  static float d_err_uncorr();
  static float d_err_corr();

  float gamma() const;
  float gamma_err() const;

  virtual void set_graph( TGraphErrors* graph );

  virtual void setColor( int color );

  static float xMinFN();
  static float xMaxFN();
  static float yMinFN();
  static float yMaxFN();
  static std::string xTitleFN();
  static std::string yTitleFN();



 private:

  TGraphErrors* graphFN_; // F-N graph: Log(I/V^2) vs 1/V

  void set_graphFN();

  float gamma_; // field enhancement factor
  float gamma_err_;


};


#endif
