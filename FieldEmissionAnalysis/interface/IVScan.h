#ifndef IVScan_h
#define IVScan_h


#include "IScan.h"

#include <string>

#include "TGraphErrors.h"



class IVScan : public IScan {

 public:

  IVScan( const std::string& name );

  virtual ~IVScan();

  virtual void readDataLine( const std::vector< std::string >& words );

  TGraphErrors* graph_vsE() const; // I vs deltaV/d = E (V/mm)

  void addPointToGraph( float hv, std::vector<float> i_meas );



 private:

  void getMeanRMS( std::vector<float> v, float& mean, float& rms );

};


#endif
