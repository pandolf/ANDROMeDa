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
  float pressure() const;
  float p() const;
  float d() const;

  void set_name( const std::string& name );
  void set_graph( TGraphErrors* graph );
  void set_p( float p );
  void set_d( float d );

  void addPointToGraph( float hv, std::vector<float> i_meas );

  TGraphErrors* getFNgraph() const;


 private:

  std::string name_;

  TGraphErrors* graph_; // graph of I(nA) vs V(V)
  float p_; // pressure in mbar
  float d_; // distance between anode and cathode in mm

  std::string getDataFileName( const std::string& dataName );
  void getMeanRMS( std::vector<float> v, float& mean, float& rms );

};


#endif