#ifndef UncCorr_h
#define UncCorr_h

#include "TMatrixD.h"
#include <vector>



class DataPoint {

 public:

  DataPoint( float m, float e_u, float e_c ) {
    mean = m;
    err_uncorr = e_u;
    err_corr = e_c;
  }

  float mean;
  float err_uncorr;
  float err_corr;

 private:

};



class UncCorr {

 public:

  UncCorr();

  ~UncCorr();

  void addDataPoint( float mean, float err_uncorr, float err_corr );
  void addDataPoint( const DataPoint& d );

  TMatrixD* getCorrelationMatrix( bool correlations=true ) const;
  std::vector<float> getWeights( TMatrixD* corrV_inv );

  void combine( float& mean, float& err, bool correlations=true );


 private:

  std::vector< DataPoint > datapoints_;

};









#endif
