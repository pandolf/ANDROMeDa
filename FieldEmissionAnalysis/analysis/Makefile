CC = g++
CFLAGS = -Wall -c -g -fpermissive 

#ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
#ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include  -I../../CommonTools/ -I../../CommonTools/interface/ -I../interface/  #-I$(ROOFIT_INCLUDE)/ C-I$(CMSSW_BASE)/src 

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -lHtml -lMathCore -lGenVector -lMinuit -lTreePlayer -lRooFitCore -lRooFit #-L$(ROOFIT_LIBDIR)/ 


drawFieldEmission: drawFieldEmission.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o drawFieldEmission drawFieldEmission.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

drawFieldEmission_forTES: drawFieldEmission_forTES.cpp AndCommon.o IScan.o IVScan.o
	$(CC) -Wall $(INCLUDES) -o drawFieldEmission_forTES drawFieldEmission_forTES.cpp AndCommon.o IScan.o IVScan.o $(ROOTFLAG) $(EXTRALIBS)

drawCompareAnodeAndDrain: drawCompareAnodeAndDrain.cpp AndCommon.o IScan.o IVScan.o
	$(CC) -Wall $(INCLUDES) -o drawCompareAnodeAndDrain drawCompareAnodeAndDrain.cpp AndCommon.o IScan.o IVScan.o $(ROOTFLAG) $(EXTRALIBS)

compareFN: compareFN.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o
	$(CC) -Wall $(INCLUDES) -o compareFN compareFN.cpp AndCommon.o IScan.o IVScan.o IVScanFN.o $(ROOTFLAG) $(EXTRALIBS)

compareScans: compareScans.cpp AndCommon.o 
	$(CC) -Wall $(INCLUDES) -o compareScans compareScans.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawTimeStability: drawTimeStability.cpp AndCommon.o IScan.o 
	$(CC) -Wall $(INCLUDES) -o drawTimeStability drawTimeStability.cpp AndCommon.o IScan.o $(ROOTFLAG) $(EXTRALIBS)

computeGammaFromScans: computeGammaFromScans.cpp AndCommon.o IVScan.o 
	$(CC) -Wall $(INCLUDES) -o computeGammaFromScans computeGammaFromScans.cpp AndCommon.o IScan.o IVScan.o $(ROOTFLAG) $(EXTRALIBS)






AndCommon.o: ../../CommonTools/src/AndCommon.cc 
	$(CC) -Wall $(INCLUDES) -c ../../CommonTools/src/AndCommon.cc $(ROOTFLAG) $(EXTRALIBS)


IScan.o: ../src/IScan.cc AndCommon.o
	$(CC) -Wall $(INCLUDES) -c ../src/IScan.cc AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

IVScan.o: ../src/IVScan.cc IScan.o AndCommon.o
	$(CC) -Wall $(INCLUDES) -c ../src/IVScan.cc IScan.o AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

IVScanFN.o: ../src/IVScanFN.cc IScan.o IVScan.o AndCommon.o
	$(CC) -Wall $(INCLUDES) -c ../src/IVScanFN.cc IScan.o IVScan.o AndCommon.o $(ROOTFLAG) $(EXTRALIBS)



clean:
	rm *.o 
