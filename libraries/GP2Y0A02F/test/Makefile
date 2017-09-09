
CXX = clang++
CXXFLAGS += -I.. -DGP2Y0A02F_DOUBLE
VPATH =..
all: GP2Y0A02F_test

%.o : %cpp
	$(CXX) $(CXXFLAGS) $< -o $@
	
GP2Y0A02F_test: GP2Y0A02F.o GP2Y0A02F_test.o
	$(CXX) GP2Y0A02F.o GP2Y0A02F_test.o -o $@