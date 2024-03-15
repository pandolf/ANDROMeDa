// class for I vs V scans

#ifndef IVScan_h
#define IVScan_h


#include "IScan.h"

#include <string>

#include "TGraphErrors.h"



class IVScan : public IScan {

 public:

  IVScan( const std::string& name, float scale=1. );

  IVScan( const IVScan& scan );

  virtual ~IVScan();

  void add_deltaV( float deltaV ); // change data points V -> V + deltaV

  TGraphErrors* graph_vsE() const; // I vs deltaV/d = E (V/mm)

 private:

};


#endif
