CC = g++
CFLAGS = -Wall -c -g -fpermissive 

#ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
#ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include  -I../../CommonTools/ -I../../CommonTools/interface/ -I../interface/  #-I$(ROOFIT_INCLUDE)/ C-I$(CMSSW_BASE)/src 

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -lHtml -lMathCore -lGenVector -lMinuit -lTreePlayer -lRooFitCore -lRooFit #-L$(ROOFIT_LIBDIR)/ 


asciiToTree: asciiToTree.cpp AndCommon.o
	$(CC) -Wall $(INCLUDES) -o asciiToTree asciiToTree.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

makeTreeLite: makeTreeLite.cpp AndCommon.o
	$(CC) -Wall $(INCLUDES) -o makeTreeLite makeTreeLite.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

checkEnergyLinearity: checkEnergyLinearity.cpp AndCommon.o
	$(CC) -Wall $(INCLUDES) -o checkEnergyLinearity checkEnergyLinearity.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawRollingAverage: drawRollingAverage.cpp AndCommon.o  
	$(CC) -Wall $(INCLUDES) -o drawRollingAverage drawRollingAverage.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)

drawPulseShape: drawPulseShape.cpp AndCommon.o  
	$(CC) -Wall $(INCLUDES) -o drawPulseShape drawPulseShape.cpp AndCommon.o $(ROOTFLAG) $(EXTRALIBS)






AndCommon.o: ../../CommonTools/src/AndCommon.cc 
	$(CC) -Wall $(INCLUDES) -c ../../CommonTools/src/AndCommon.cc $(ROOTFLAG) $(EXTRALIBS)



clean:
	rm *.o 
