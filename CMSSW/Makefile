DEL           = $(shell echo $(DELPHES))
CXX           = g++
CXXFLAGS      = -Wno-deprecated $(shell root-config --cflags)  -std=c++0x 
CXXFLAGS     += -I../$(DEL)/external/ -I../$(DEL) -I../Delphes/
LDFLAGS       = 
LD            = g++

LIBS = $(shell root-config --glibs)
LIBS += -L../$(DEL) -lDelphes


OBJS          = CMSHist.o CMSAUX.o DelEvent.o DelAna.o DelCut.o HistTool.o DelZJJ.o
PROGRAM       = CMSFill

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

