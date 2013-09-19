ifeq ($(debug),1)
ROOTBUILD=debug
CPPFLAGS=-g
endif

LD            = g++
LDFLAGS       = -g -O
OutPutOpt = -o
CPPFLAGS= -g -O2 -pipe -Wall -W -Woverloaded-virtual -fPIC \
        -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO \
        -DG__OSFDLL -D__NO_CDFSOFT__ $(shell root-config --cflags)

LIBS = $(shell root-config --glibs)

HEADERS = $(shell ls *.h *.hh) Makefile
HEADERS = Makefile


#all: test
all: HTStack
#all: HistStack

%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<


CutTable : CutTable.o HTSample.o MCHis.o HTHis.o 
	$(LD) $(LDFLAGS) $^ $(LIBS) -lMinuit $(OutPutOpt)$@
	@echo "$@ done"

HistStack: HistStack.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

HTStack: HTStack.o MCHis.o HTHis.o HTSample.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

InterCom: InterCom.o MCHis.o HTHis.o HTSample.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

PrepLimit: PrepLimit.o MCHis.o HTHis.o HTSample.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

HTCom: HTCom.o MCHis.o HTHis.o HTSample.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

PlotEff: PlotEff.o MCHis.o HTHis.o HTSample.o StopStyle.o HiggsPaperStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

PlotLimit: PlotLimit.o MCHis.o HTHis.o HTSample.o StopStyle.o HiggsPaperStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

clean :
	rm -f *.o cyclic tcbayes tccls tcclsobs tcclsexp tcclspx nk_bayes cdf.root

