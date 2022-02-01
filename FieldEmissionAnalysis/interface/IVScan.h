#ifndef IVScan_h
#define IVScan_h


#include <string>

#include "TGraphErrors.h"



class IVScan {

 public:

  IVScan( const std::string& name );

  ~IVScan();


  void readFile( const std::string& name="" );

  std::string name() const;

  TGraphErrors* graph() const;
  TGraphErrors* graphFN() const;
  TF1* lineFN() const;

  float pressure() const;
  float p() const;
  float d() const;
  float gamma() const;
  float gamma_err() const;

  void set_name( const std::string& name );
  void set_graph( TGraphErrors* graph );
  void set_p( float p );
  void set_d( float d );

  void setColor( int color );

  void addPointToGraph( float hv, std::vector<float> i_meas );

  static float xMinFN();
  static float xMaxFN();
  static float yMinFN();
  static float yMaxFN();
  static std::string xTitleFN();
  static std::string yTitleFN();



 private:

  std::string name_;

  TGraphErrors* graph_; // graph of I(nA) vs V(V)
  TGraphErrors* graphFN_; // F-N graph: Log(I/V^2) vs 1/V

  void set_graphFN();

  float p_; // pressure in mbar
  float d_; // distance between anode and cathode in mm

  float gamma_;
  float gamma_err_;

  std::string getDataFileName( const std::string& dataName );
  void getMeanRMS( std::vector<float> v, float& mean, float& rms );

};


#endif
