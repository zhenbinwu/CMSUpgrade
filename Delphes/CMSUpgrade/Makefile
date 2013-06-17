CXX           = g++
CXXFLAGS      = -Wno-deprecated $(shell root-config --cflags) -I../external/ -I../classes/  -I.. -std=c++0x 
LDFLAGS       = 
LD            = g++

LIBS = $(shell root-config --glibs)
LIBS += -L../ -lDelphes


OBJS          = DelFill.o DelHist.o DelEvent.o DelWino.o DelTT.o DelZJJ.o DelAna.o DelCut.o HistTool.o
PROGRAM       = DelFill

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

