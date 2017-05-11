CXX = g++
CXXFLAGS = -std=c++11
# -Wall

COBJ = freceiver.o file_clnt.o file_cif.o

SOBJ = fsender.o file_sif.o file_srp.o

PRGS = test

all: ${PRGS}

${PRGS}: ${COBJ} ${SOBJ} test.o file_xdr.o
	${CXX} -o $@ ${CXXFLAGS} $@.o ${COBJ} ${SOBJ} file_xdr.o

clean:
	rm *.o $(PRGS)
