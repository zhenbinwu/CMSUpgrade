ifeq ($(debug),1)
ROOTBUILD=debug
CPPFLAGS=-g
endif

LD            = g++
LDFLAGS       = -g -O -m32
OutPutOpt = -o
CPPFLAGS= -g -O2 -pipe -Wall -W -Woverloaded-virtual -fPIC \
        -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO \
        -DG__OSFDLL -D__NO_CDFSOFT__ $(shell root-config --cflags)

LIBS = $(shell root-config --glibs)

HEADERS = $(shell ls *.h *.hh) Makefile
HEADERS = Makefile


all: HistStack

%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<


PlotMCLimit : PlotMCLimit.o
	$(LD) $(LDFLAGS) $^ $(LIBS) -lMinuit $(OutPutOpt)$@
	@echo "$@ done"

XSDraw: XSDraw.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@

XTDraw: XTDraw.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@

HistStack: HistStack.o StopStyle.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
fit2d: fit2d.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"


try: try.o
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@

clean :
	rm -f *.o cyclic tcbayes tccls tcclsobs tcclsexp tcclspx nk_bayes cdf.root

