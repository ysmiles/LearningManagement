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

SSRC = Server.cpp
SXSRC = handle.cpp student.cpp

SXOBJ = handle.o student.o

PROGS = ${CLNTS} ${SERVS} ${OTHER}

all: ${PROGS}

#${CLNTS}: ${CXOBJ}
#	${CC} -o $@ ${CFLAGS} $@.o ${CXOBJ}


${SERVS}: ${SXOBJ} Server.o
	${CXX} -o $@ ${CXXFLAGS} $@.o ${SXOBJ}

clients: ${CLNTS}
servers: ${SERVS}

clean:
	rm -f Makefile.bak a.out core errs lint.errs ${PROGS} *.o

# DO NOT DELETE THIS LINE - maketd DEPENDS ON IT
