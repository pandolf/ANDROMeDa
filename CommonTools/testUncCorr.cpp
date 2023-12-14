#include "UncCorr.h"

#include <iostream>




int main() {

//DataPoint dp1(1., 0.1, 0.1);
//DataPoint dp2(1., 0.1, 0.2);
//DataPoint dp3(1., 0.1, 0.2);

//DataPoint dp1(22005.3, 411.008, 702.61 );
//DataPoint dp2(26091.4, 4104.77, 832.875 );
//DataPoint dp3(19315.9, 386.534, 616.537 );

  DataPoint dp1(22005.3, 411.008, 2206.51);
  DataPoint dp2(26091.4, 4104.77, 1876.08 );
  DataPoint dp3(19315.9, 386.534, 1125.22 );

  //DataPoint dp1(1., 0.2, 0.);
  //DataPoint dp2(1., 0.2, 0.);

  UncCorr uc;

  uc.addDataPoint(dp1);
  uc.addDataPoint(dp2);
  uc.addDataPoint(dp3);

  float m_comb, err_comb;
  uc.combine( m_comb, err_comb );

  std::cout << "Combined measurement: " << m_comb << " +/- " << err_comb << std::endl;

  float m_nocorr, err_nocorr;
  uc.combine( m_nocorr, err_nocorr, false );

  std::cout << "Combined measurement (no correlations): " << m_nocorr << " +/- " << err_nocorr << std::endl;

  return 0;

}
