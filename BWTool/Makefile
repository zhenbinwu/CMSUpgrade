ifeq ($(debug),1)
ROOTBUILD=debug
CPPFLAGS=-g
endif

LD        = g++
LDFLAGS   = -g -O
OutPutOpt = -o
CPPFLAGS  = -g -O2 -pipe -Wall -W -Woverloaded-virtual -fPIC -std=c++0x \
		  -D__REGEXP -DG__UNIX -DG__SHAREDLIB -DG__ROOT -DG__REDIRECTIO \
		  -DG__OSFDLL -D__NO_CDFSOFT__ $(shell root-config --cflags)

LIBS      = $(shell root-config --glibs)

HEADERS   = $(shell ls *.h *.hh) Makefile
HEADERS   = Makefile

#============================================================================#
#-----------------------------   Include Boost   ----------------------------#
#============================================================================#
ifneq ($(shell echo $(BOOSTROOT)), '')
  CPPFLAGS  += -I$(shell echo $(BOOSTROOT))
endif


all: HTadd

%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

HTadd : HTadd.o 
	$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt) $@
	@echo "$@ done"

clean :
	rm -f *.o HTadd

