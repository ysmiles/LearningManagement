#
# Internetworking with TCP/IP, Volume III example code Makefile
#
#	David L Stevens, Internetworking Research Group at Purdue
#	Tue Sep 17 19:40:42 EST 1991
#

INCLUDE =

CLNTS = StudentClient InstructorClient
SERVS = Server

DEFS = -D_BSD_SOURCE
CFLAGS = -W -pedantic -g ${DEFS} ${INCLUDE}
CXXFLAGS = -std=c++11 -W ${DEFS} ${INCLUDE}
#CXXFLAGS = -std=c++11 -W -pedantic -g ${DEFS} ${INCLUDE}

HDR =

CSRC = InstructorClient.c StudentClient.c
CXSRC = connectsock.c connectTCP.c
COBJ = InstructorClient.o StudentClient.o
CXOBJ = connectsock.o connectTCP.o

GENSRC = errexit.c 
GENOBJ = errexit.o
#CPP version .o file
GENOBJCXX = errexitcpp.o

SSRC = Server.cpp
SXSRC = handle.cpp student.cpp passivesock.cpp passiveTCP.cpp
SXOBJ = handle.o student.o passivesock.o passiveTCP.o

PROGS = ${CLNTS} ${SERVS} ${OTHER}

all: ${PROGS}

${CLNTS}: ${COBJ} ${CXOBJ} ${GENOBJ}
	${CC} -o $@ ${CFLAGS} $@.o ${CXOBJ} ${GENOBJ}

${GENOBJCXX}: ${GENSRC} 
	${CXX} -o $@ ${CXXFLAGS} -c ${GENSRC}

${SERVS}: Server.o ${SXOBJ} ${GENOBJCXX}
	${CXX} -o $@ ${CXXFLAGS} $@.o ${SXOBJ} ${GENOBJCXX}

clients: ${CLNTS}
servers: ${SERVS}

clean:
	rm -f Makefile.bak a.out core errs lint.errs ${PROGS} *.o

# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
