DEL           = $(shell echo $(DELPHES))
#CXX           = ~/.vim/bundle/ClangComplete/bin/cc_args.py g++


CXX           = g++ -std=c++0x 
#CXXFLAGS     += -Wall -Wextra -Weffc++
## Include ROOT
CXXFLAGS     += $(shell root-config --cflags) 
## Include Delphes
CXXFLAGS     += -I../$(DEL)/external/ -I../$(DEL) -std=c++0x 

LD            = g++
LDFLAGS       = 

LIBS = $(shell root-config --glibs)
LIBS += -L../$(DEL) -lDelphes

#============================================================================#
#--------------------------   Include OxbridgeMT2   -------------------------#
#============================================================================#
ifeq ($(shell echo $(OxbridgeMT2)), True)
  CXXFLAGS     += -I../OxbridgeMT2/include/oxbridgekinetics-1.0/
  LIBS         += -L../OxbridgeMT2/lib -loxbridgekinetics-1.0
endif


#============================================================================#
#-----------------------------   Include Boost   ----------------------------#
#============================================================================#
ifneq (, $(findstring cmslpc, $(shell hostname)))
  CPPFLAGS += -I/uscmst1/prod/sw/cms/slc5_amd64_gcc462/external/boost/1.50.0-cms2/include
endif


OBJS          = DelMain.o DelLoop.o DelProcess.o DelAna.o HistTool.o
OBJS         += DelEvent.o DelEventWino.o DelEventTT.o DelEventZJJ.o DelEventHTB.o DelEventLL.o 
OBJS         += DelCut.o
#OBJS         += DelCut.o DelCutDM.o
PROGRAM       = DelFill


all:            $(PROGRAM)


%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%  : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(PROGRAM):     $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	@$(LD) $(OBJS) $(LIBS) -o $(PROGRAM)
	@echo "done"

clean:;         @rm -f $(OBJS) $(PROGRAM) core 

