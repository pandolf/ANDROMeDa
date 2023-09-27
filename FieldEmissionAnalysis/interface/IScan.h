#ifndef IScan_h
#define IScan_h


#include <string>

#include "TGraphErrors.h"



class IScan {

 public:

  IScan( const std::string& name, float scale=1. );

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

  void set_name( const std::string& name );
  void set_p( float p );
  void set_d( float d );

  virtual void set_graph( TGraphErrors* graph );

  virtual void setColor( int color );



 private:

  std::string name_;

  TGraphErrors* graph_; // graph of I(nA) vs V(V)

  float p_; // pressure in mbar
  float d_; // distance between anode and cathode in mm

  std::string getDataFileName( const std::string& dataName );

};


#endif
