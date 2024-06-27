CC = g++
CFLAGS = -Wall -c -g -fpermissive 

#ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
#ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include  -I../../CommonTools/ -I../../CommonTools/interface/ -I../interface/  #-I$(ROOFIT_INCLUDE)/ C-I$(CMSSW_BASE)/src 

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -lHtml -lGenVector -lMinuit -lRooFitCore -lRooFit #-L$(ROOFIT_LIBDIR)/ 


drawFieldEmission: drawFieldEmission.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o drawFieldEmission drawFieldEmission.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

drawFieldEmission_forTES: drawFieldEmission_forTES.cpp AndCommon.o IScan.o IVScan.o
	$(CC) -Wall $(INCLUDES) -o drawFieldEmission_forTES drawFieldEmission_forTES.cpp AndCommon.o IScan.o IVScan.o $(ROOTFLAG) $(EXTRALIBS)

drawCompareAnodeAndDrain: drawCompareAnodeAndDrain.cpp AndCommon.o IScan.o IVScan.o
	$(CC) -Wall $(INCLUDES) -o drawCompareAnodeAndDrain drawCompareAnodeAndDrain.cpp AndCommon.o IScan.o IVScan.o $(ROOTFLAG) $(EXTRALIBS)

drawFN_forPaper: drawFN_forPaper.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o drawFN_forPaper drawFN_forPaper.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

drawGamma_vs_r: drawGamma_vs_r.cpp AndCommon.o 
	$(CC) -Wall $(INCLUDES) -o drawGamma_vs_r drawGamma_vs_r.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawRadiusHisto: drawRadiusHisto.cpp AndCommon.o 
	$(CC) -Wall $(INCLUDES) -o drawRadiusHisto drawRadiusHisto.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

compareFN: compareFN.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o compareFN compareFN.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

compareScans: compareScans.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o compareScans compareScans.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

makeRawTree: makeRawTree.cpp AndCommon.o
	$(CC) -Wall $(INCLUDES) -o makeRawTree makeRawTree.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

makeTreeLite: makeTreeLite.cpp AndCommon.o
	$(CC) -Wall $(INCLUDES) -o makeTreeLite makeTreeLite.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

checkPulseShape: checkPulseShape.cpp 
	$(CC) -Wall $(INCLUDES) -o checkPulseShape checkPulseShape.cpp $(ROOTFLAG) $(EXTRALIBS)

checkPulseShape_old: checkPulseShape_old.cpp 
	$(CC) -Wall $(INCLUDES) -o checkPulseShape_old checkPulseShape_old.cpp $(ROOTFLAG) $(EXTRALIBS)

computeRate: computeRate.cpp 
	$(CC) -Wall $(INCLUDES) -o computeRate computeRate.cpp $(ROOTFLAG) $(EXTRALIBS)



drawTimeStability: drawTimeStability.cpp AndCommon.o IScan.o
	$(CC) -Wall $(INCLUDES) -o drawTimeStability drawTimeStability.cpp AndCommon.o IScan.o $(ROOTFLAG) $(EXTRALIBS)

drawTempStability: drawTempStability.cpp AndCommon.o IScan.o
	$(CC) -Wall $(INCLUDES) -o drawTempStability drawTempStability.cpp AndCommon.o IScan.o $(ROOTFLAG) $(EXTRALIBS)

computeGammaFromScans: computeGammaFromScans.cpp AndCommon.o UncCorr.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o computeGammaFromScans computeGammaFromScans.cpp AndCommon.o UncCorr.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

computeGammaFromScans_deltaV: computeGammaFromScans_deltaV.cpp AndCommon.o UncCorr.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o computeGammaFromScans_deltaV computeGammaFromScans_deltaV.cpp AndCommon.o UncCorr.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

computeGammaFromScans_R: computeGammaFromScans_R.cpp AndCommon.o UncCorr.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o computeGammaFromScans_R computeGammaFromScans_R.cpp AndCommon.o UncCorr.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

testUncCorr: testUncCorr.cpp AndCommon.o UncCorr.o
	$(CC) -Wall $(INCLUDES) -o testUncCorr testUncCorr.cpp AndCommon.o UncCorr.o $(ROOTFLAG) $(EXTRALIBS)



makeTree: makeTree.cpp pshape_functions.h
	$(CC) -Wall $(INCLUDES) -o makeTree makeTree.cpp $(ROOTFLAG) $(EXTRALIBS) 

fitAmpChargeData: fitAmpChargeData.cpp pshape_functions.h graphics.h
	$(CC) -Wall $(INCLUDES) -o fitAmpChargeData fitAmpChargeData.cpp $(ROOTFLAG) $(EXTRALIBS) 

drawAmpChargeFinalFits_all: drawAmpChargeFinalFits_all.cpp pshape_functions.h CD_plot_details.h
	$(CC) -Wall $(INCLUDES) -o drawAmpChargeFinalFits_all drawAmpChargeFinalFits_all.cpp $(ROOTFLAG) $(EXTRALIBS)

drawAmpChargeFinalFits_single: drawAmpChargeFinalFits_single.cpp pshape_functions.h CD_plot_details.h
	$(CC) -Wall $(INCLUDES) -o drawAmpChargeFinalFits_single drawAmpChargeFinalFits_single.cpp $(ROOTFLAG) $(EXTRALIBS) 


graphics.h: ../interface/graphics.h
CD_plot_details.h: ../interface/graphics.h
pshape_functions.h: ../interface/graphics.h

AndCommon.o: ../../CommonTools/src/AndCommon.cc 
	$(CC) -Wall $(INCLUDES) -c ../../CommonTools/src/AndCommon.cc $(ROOTFLAG) $(EXTRALIBS)

UncCorr.o: ../../CommonTools/src/UncCorr.cc 
	$(CC) -Wall $(INCLUDES) -c ../../CommonTools/src/UncCorr.cc $(ROOTFLAG) $(EXTRALIBS)


IScan.o: ../src/IScan.cc AndCommon.o
	$(CC) -Wall $(INCLUDES) -c ../src/IScan.cc AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

IVScan.o: ../src/IVScan.cc IScan.o AndCommon.o
	$(CC) -Wall $(INCLUDES) -c ../src/IVScan.cc IScan.o AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

IVScanFN.o: ../src/IVScanFN.cc IScan.o IVScan.o AndCommon.o
	$(CC) -Wall $(INCLUDES) -c ../src/IVScanFN.cc IScan.o IVScan.o AndCommon.o $(ROOTFLAG) $(EXTRALIBS)



clean:
	rm *.o 
