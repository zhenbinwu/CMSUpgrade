DEL           = $(shell echo $(DELPHES))
#CXX           = ~/.vim/bundle/ClangComplete/bin/cc_args.py g++
CXX           = g++
CXXFLAGS      = -Wno-deprecated $(shell root-config --cflags) -I../$(DEL)/external/ -I../$(DEL) -std=c++0x 
CXXFLAGS     += -I../OxbridgeMT2/include/oxbridgekinetics-1.0/
LDFLAGS       = 
LD            = g++

LIBS = $(shell root-config --glibs)
LIBS += -L../$(DEL) -lDelphes
LIBS += -L../OxbridgeMT2/lib -loxbridgekinetics-1.0


OBJS          = DelMain.o DelLoop.o DelEvent.o DelWino.o DelTT.o DelZJJ.o DelAna.o DelCut.o HistTool.o
OBJS         += DelHTB.o DelLL.o
PROGRAM       = DelFill

ifneq (, $(findstring cmslpc, $(shell hostname)))
  CPPFLAGS += -I/uscmst1/prod/sw/cms/slc5_amd64_gcc462/external/boost/1.50.0-cms2/include
endif

all:            $(PROGRAM)


%.o : %.C $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

%.o : %.cc $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(PROGRAM):     $(OBJS)
	@echo "Linking $(PROGRAM) ..."
	@$(LD) $(OBJS) $(LIBS) -o $(PROGRAM)
	@echo "done"



clean:;         @rm -f $(OBJS) $(PROGRAM) core 

