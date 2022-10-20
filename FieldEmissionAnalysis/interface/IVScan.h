#ifndef IVScan_h
#define IVScan_h


#include <string>

#include "TGraphErrors.h"



class IVScan {

 public:

  IVScan( const std::string& name );

  virtual ~IVScan();


  void readFile( const std::string& name="" );

  std::string name() const;

  TGraphErrors* graph() const;  // I vs deltaV
  TGraphErrors* graph_vsE() const; // I vs deltaV/d = E (V/mm)

  float pressure() const;
  float p() const;
  float d() const;

  void set_name( const std::string& name );
  void set_p( float p );
  void set_d( float d );

  virtual void set_graph( TGraphErrors* graph );

  virtual void setColor( int color );

  void addPointToGraph( float hv, std::vector<float> i_meas );


 private:

  std::string name_;

  TGraphErrors* graph_; // graph of I(nA) vs V(V)

  float p_; // pressure in mbar
  float d_; // distance between anode and cathode in mm

  std::string getDataFileName( const std::string& dataName );
  void getMeanRMS( std::vector<float> v, float& mean, float& rms );

};


#endif
