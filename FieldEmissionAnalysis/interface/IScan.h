#ifndef IScan_h
#define IScan_h


#include <string>

#include "TGraphErrors.h"



class IScan {

 public:

  IScan( const std::string& name, float scale=1., float xMin=-99999., float xMax=99999. );

  virtual ~IScan();

  virtual void readFile( const std::string& name="" );
  virtual void readCommentLine( const std::vector< std::string >& words );
  virtual void readDataLine( const std::vector< std::string >& words, bool& addToGraph );

  void scaleDataPoints( float scale = 1. );

  //void addPointToGraph( float hv, std::vector<float> i_meas );

  //void getMeanRMS( std::vector<float> v, float& mean, float& rms );

  std::string name() const;

  TGraphErrors* graph() const;  // I vs something

  float pressure() const;
  float p() const;
  float d() const;
  float t() const;
  std::string hv() const;
  float dz() const;
  int n() const;

  void set_name( const std::string& name );
  void set_p ( float p );
  void set_d ( float d );
  void set_t ( float t );
  void set_hv( const std::string& hv );
  void set_dz( float dz );
  void set_n ( int n );

  float xMin() const;
  float xMax() const;

  void set_xMin( float xMin );
  void set_xMax( float xMax );


  virtual void set_graph( TGraphErrors* graph );

  virtual void setColor( int color );



 private:

  std::string name_;

  TGraphErrors* graph_; // graph of I(nA) vs V(V)

  float p_;  // pressure in mbar
  float d_;  // distance between anode and cathode in mm
  float t_;  // temperature in K
  std::string hv_; // power supply model
  float dz_; // uncertainty on delta(d) in mm
  int n_;    // number of points used in current measurement

  float xMin_;
  float xMax_;

  std::string getDataFileName( const std::string& dataName );

};


#endif
