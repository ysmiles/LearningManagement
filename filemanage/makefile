CXX = g++
CFLAGS =
# -Wall

CLNT = fileC

SERV = fileS

COBJ = file.o file_clnt.o file_cif.o file_xdr.o

SOBJ = file_svc.o file_sif.o file_srp.o file_xdr.o

PRGS = ${CLNT} ${SERV}

all: ${PRGS}

${CLNT}: ${COBJ}
	${CXX} -o $@ ${CFLAGS} ${COBJ}

${SERV}: ${SOBJ} ${SXOBJ}
	${CXX} -o $@ ${CFLAGS} ${SOBJ}

clean:
	rm *.o $(PRGS)
