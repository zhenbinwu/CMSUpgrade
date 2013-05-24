CXX           = g++
CXXFLAGS      = -Wno-deprecated $(shell root-config --cflags) -I../external/ -I../classes/  -I..
LDFLAGS       = 
LD            = g++

LIBS = $(shell root-config --glibs)
LIBS += -L../ -lDelphes


OBJS          = DelFill.o DelHist.o 
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

