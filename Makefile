CC=g++
CFLAGS=-std=c++11

INCD = $(IDIRL) $(IDIRGRB) $(IDIR)
IDIRL=include
IDIRGRB=${GUROBI_HOME}/include
IDIR=/usr/local/include
INCL = $(foreach d, $(INCD), -I $d)


LINCD = $(LDIRL) $(LDIRGRB) $(LDIR)
LDIRL=lib
LDIRGRB=${GUROBI_HOME}/lib/ 
LDIR=/usr/local/lib
LINCL = $(foreach d, $(LINCD), -L $d)

LIBS=-lgurobi_c++ -lgurobi75

DEBUG ?= 1
ifeq ($(DEBUG),1)
	CFLAGS+= -Wall -g
	ODIR=debug/obj
	EDIR=debug/bin
else
	CFLAGS+= -O3 -s
	ODIR=release/obj
	EDIR=release/bin
endif


BINNAME ?= main
BINARY = $(EDIR)/$(BINNAME)
SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp,$(ODIR)/%.o,$(SRC)) 
HEADERS = $(wildcard $(IDIRL)/*.h)

all : $(BINARY)
	
$(BINARY) : $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LINCL) $(LIBS) $(LDIRGRB) 

$(ODIR)/%.o : src/%.cpp $(HEADERS)
	$(CC) -c -o $@ $< $(CFLAGS) $(INCL)

clean :
	rm -f $(ODIR)/*.o $(EDIR)/* src/*~ *~ core
