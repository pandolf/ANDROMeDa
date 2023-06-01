#include "../interface/UncCorr.h"

#include <iostream>

#include "TArrayD.h"




UncCorr::UncCorr() {};

UncCorr::~UncCorr() {};




void UncCorr::addDataPoint( float mean, float err_uncorr, float err_corr ) {

  DataPoint d(mean, err_uncorr, err_corr );
  this->addDataPoint(d);

}




void UncCorr::addDataPoint( const DataPoint& d ) {

  datapoints_.push_back(d);

}



void UncCorr::combine( float& mean, float& err ) {


  TMatrixD* corrV = getCorrelationMatrix();

  TMatrixD* corrV_inv = new TMatrixD(corrV->Invert());

  std::vector<float> weights = getWeights( corrV_inv );

  for( unsigned i=0; i<weights.size(); ++i )
    std::cout << "w[" << i << "]: " << weights[i] << std::endl;

  mean = 0.;

  for( unsigned i=0; i<datapoints_.size(); ++i )
    mean += weights[i]*datapoints_[i].mean;


  double* corrV_elem = corrV->GetMatrixArray();

  float err2 = 0.;

  int dim = datapoints_.size();

  for( unsigned i=0; i<dim; ++i )
    for( unsigned j=0; j<dim; ++j )
      err2 += weights[i]*weights[j]*corrV_elem[i*dim+j];

  err = sqrt(err2);

}



TMatrixD* UncCorr::getCorrelationMatrix() const {

  int dim = datapoints_.size();

  TArrayD dataarray(dim*dim);

  for( unsigned i=0; i<dim; ++i ) {
    for( unsigned j=0; j<dim; ++j ) {

      int index = i*dim+j;

      if( i==j ) {

        float sigmaiuncorr2 = datapoints_[i].err_uncorr*datapoints_[i].err_uncorr;
        float sigmaicorr2   = datapoints_[i].err_corr  *datapoints_[i].err_corr  ;
        float sigmai2 = sigmaiuncorr2 + sigmaicorr2;

        dataarray[index] = sigmai2;

      } else {

        dataarray[index] = datapoints_[i].err_corr*datapoints_[j].err_corr;

      } // else

    } // j

  } // i


  TMatrixD* corrV = new TMatrixD(dim,dim);
  corrV->SetMatrixArray(dataarray.GetArray());

  return corrV;

}



std::vector<float> UncCorr::getWeights( TMatrixD* corrV_inv ) {

  std::vector<float> weights;

  double* elements = corrV_inv->GetMatrixArray();

  int dim = datapoints_.size();

  float denom = 0.;
  for( unsigned i=0; i<dim*dim; ++i )
    denom += elements[i];



  for( unsigned i=0; i<dim; ++i ) {

    float num = 0.;

    for( unsigned j=0; j<dim; ++j ) {

      int index = dim*i + j;
      num += elements[index];

    } // j

    weights.push_back( num/denom );

  } // i

  return weights;

}


