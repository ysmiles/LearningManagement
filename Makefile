# FILES = Foo.cpp Bar.cpp
# SOURCES = $(FILES:%.cpp=$(SRC_PATH)/%.cpp)
FCOBJ = freceiver.o file_clnt.o file_cif.o
FSOBJ = fsender.o file_sif.o file_srp.o
FAOBJ = ${FCOBJ} ${FSOBJ} file_xdr.o
FPATH = filemanage
PATHFAOBJ = $(FAOBJ:%.o=$(FPATH)/%.o)


INCLUDE =

CLNTS = StudentClient InstructorClient
SERVS = Server

DEFS = -D_BSD_SOURCE
CFLAGS = -W -pedantic -g ${DEFS} ${INCLUDE}
CXXFLAGS = -std=c++11 -W ${DEFS} ${INCLUDE}
#CXXFLAGS = -std=c++11 -W -pedantic -g ${DEFS} ${INCLUDE}

HDR =

CXSRC = connectsock.c connectTCP.c
COBJ = InstructorClient.o StudentClient.o
CXOBJ = connectsock.o connectTCP.o

GENOBJ = errexit.o
#CPP version .o file
# GENOBJCXX = errexitcpp.o

SSRC = Server.cpp
SXSRC = handle.cpp student.cpp passivesock.cpp passiveTCP.cpp
SXOBJ = handle.o student.o passivesock.o passiveTCP.o

PROGS = ${CLNTS} ${SERVS} ${OTHER}

all: ${PROGS}

${CLNTS}: ${COBJ} ${CXOBJ} ${GENOBJ} ${PATHFAOBJ} secure.o
	${CXX} -o $@ ${CXXFLAGS} $@.o ${CXOBJ} ${GENOBJ} ${PATHFAOBJ} secure.o -lcrypto

dbconnector.o: dbconnector.cpp
	${CXX} -o $@ ${CXXFLAGS} -c dbconnector.cpp -lmysqlcppconn

${SERVS}: Server.o ${SXOBJ} ${GENOBJ} dbconnector.o ${PATHFAOBJ} secure.o
	${CXX} -o $@ ${CXXFLAGS} $@.o ${SXOBJ} ${GENOBJ} dbconnector.o ${PATHFAOBJ} secure.o -lcrypto -lmysqlcppconn

clients: ${CLNTS}
servers: ${SERVS}

clean:
	rm -f Makefile.bak a.out core errs lint.errs ${PROGS} *.o

# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
