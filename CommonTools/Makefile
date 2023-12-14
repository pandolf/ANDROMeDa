CC = g++
CFLAGS = -Wall -c -g -fpermissive 

#ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
#ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I.. -I$(ROOTSYS)/include   -I./interface/  #-I$(ROOFIT_INCLUDE)/ C-I$(CMSSW_BASE)/src 

ROOTSYS ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs`

EXTRALIBS := -L$(ROOTSYS)/lib -lHtml -lMathCore -lGenVector -lMinuit -lTreePlayer -lRooFitCore -lRooFit #-L$(ROOFIT_LIBDIR)/ 


testUncCorr: testUncCorr.cpp AndCommon.o UncCorr.o
	$(CC) -Wall $(INCLUDES) -o testUncCorr testUncCorr.cpp AndCommon.o UncCorr.o $(ROOTFLAG) $(EXTRALIBS)






AndCommon.o: ./src/AndCommon.cc 
	$(CC) -Wall $(INCLUDES) -c ./src/AndCommon.cc $(ROOTFLAG) $(EXTRALIBS)

UncCorr.o: ./src/UncCorr.cc 
	$(CC) -Wall $(INCLUDES) -c ./src/UncCorr.cc $(ROOTFLAG) $(EXTRALIBS)



clean:
	rm *.o 
