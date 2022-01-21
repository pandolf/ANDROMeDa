#ifndef AndCommon_h
#define AndCommon_h

#include "TCanvas.h"
#include "TPaveText.h"
#include "TGraphErrors.h"


#include <vector>


class AndCommon {

 public:

  AndCommon();
  ~AndCommon();

  static void setStyle();

  static std::vector<int> colors();

  static void addAndLabel( TCanvas* c1, int quadrant=4, const std::string& text="ANDROMeDa" );
  static void addAndLabel( TCanvas* c1, float xmin, float ymin, float xmax, float ymax, const std::string& text="ANDROMeDa" );

  static TPaveText* getAndLabel( int quadrant, const std::string& text="ANDROMeDa" );
  static TPaveText* getAndLabel( float xmin, float ymin, float xmax, float ymax, const std::string& text="ANDROMeDa" );

  static float integrateSignal( TGraph* graph, bool invertPolarity=false );
  static float ampMaxSignal( TGraph* graph, bool invertPolarity=false );

  static float getPedestal( TGraph* graph, int nPoints );
  static void removeTimeOffset( TGraph* graph );
  static void plotWaveformGraph( TGraph* graph, const std::string& saveName );
  static TGraph* getGraphFromFile( const std::string& fileName );

  static void findGraphRanges( TGraph* graph, float& xMin, float& xMax, float& yMin, float& yMax );
  static TGraphErrors* getGraphRatio( TGraphErrors* gr_num, TGraphErrors* gr_denom );


  static std::string scientific( float x, int decimals = 1 );

};

#endif
