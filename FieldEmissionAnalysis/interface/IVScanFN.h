#ifndef IVScanFN_h
#define IVScanFN_h

// IVScan dedicated to Fowler–Nordheim analysis


#include "../interface/IVScan.h"




class IVScanFN : public IVScan {

 public:

  IVScanFN( const std::string& name, float scale=1., float xMin=-99999., float xMax=99999. );

  ~IVScanFN();


  TGraphErrors* graphFN() const;
  TF1* lineFN() const;

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
