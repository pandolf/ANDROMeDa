#ifndef IVScan_h
#define IVScan_h


#include "IScan.h"

#include <string>

#include "TGraphErrors.h"



class IVScan : public IScan {

 public:

  IVScan( const std::string& name, float scale=1., float xMin=-99999., float xMax=99999. ); //xMin/xMax: only points with xMin<x<xMax will be selected

  virtual ~IVScan();

  TGraphErrors* graph_vsE() const; // I vs deltaV/d = E (V/mm)

 private:

};


#endif
