// generic class for current scans (eg vs voltage or vs time)

#ifndef IScan_h
#define IScan_h


#include <string>

#include "TGraphErrors.h"



class IScan {

 public:

  IScan( const std::string& name, float scale=1.);

  virtual ~IScan();

  virtual void readFile( const std::string& name="" );
  virtual void readCommentLine( const std::vector< std::string >& words );
  virtual void readDataLine( const std::vector< std::string >& words, bool& addToGraph );

  void scaleDataPoints( float scale = 1. );

  std::string name() const;
  std::string sampleName() const;
  std::string lab() const;

  TGraphErrors* graph() const;  // I vs something

  float pressure() const;
  float p() const;
  float d() const;
  float t() const;
  std::string hv() const;
  float dz() const;
  float verr() const;
  int n() const;
  int color() const;

  void set_name( const std::string& name );
  void set_sampleName( const std::string& sampleName );
  void set_lab( const std::string& lab );
  void set_p ( float p );
  void set_d ( float d );
  void set_t ( float t );
  void set_hv( const std::string& hv );
  void set_dz( float dz );
  void set_verr( float verr );
  void set_n ( int n );
  virtual void set_color ( int color );

  float vMin() const;
  float vMax() const;

  void set_vMin( float vMin );
  void set_vMax( float vMax );


  virtual void set_graph( TGraphErrors* graph );



 private:

  std::string name_; // scan name

  std::string sampleName_; // CNT sample name

  std::string lab_; 

  TGraphErrors* graph_; // graph of I(nA) vs V(V)

  float p_;    // pressure in mbar
  float d_;    // distance between anode and cathode in mm
  float t_;    // temperature in K
  std::string hv_; // power supply model
  float dz_;   // uncertainty on delta(d) in mm
  float verr_; // voltage uncertainty
  int n_;      // number of points used in current measurement
  int color_;  // color of graphs

  std::string getDataFileName( const std::string& dataName );

};


#endif
