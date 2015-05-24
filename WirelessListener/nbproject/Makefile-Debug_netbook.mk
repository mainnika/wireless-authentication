#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=clang
CCC=clang++
CXX=clang++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=CLang-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Debug_netbook
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/Core/airpcap.o \
	${OBJECTDIR}/Core/common.o \
	${OBJECTDIR}/Core/core.o \
	${OBJECTDIR}/Core/file.o \
	${OBJECTDIR}/Core/linux.o \
	${OBJECTDIR}/Core/linux_tap.o \
	${OBJECTDIR}/Core/network.o \
	${OBJECTDIR}/Core/radiotap/radiotap.o \
	${OBJECTDIR}/Server/Interface.o \
	${OBJECTDIR}/Server/Interfaces.o \
	${OBJECTDIR}/Server/Server.o \
	${OBJECTDIR}/Server/Thread.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=-fsanitize=address -fno-omit-frame-pointer

# CC Compiler Flags
CCFLAGS=-std=c++1y -fsanitize=address -fno-omit-frame-pointer
CXXFLAGS=-std=c++1y -fsanitize=address -fno-omit-frame-pointer

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L${HOME}/.lib/libev/lib -lpcap -lev -lglog

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener ${OBJECTFILES} ${LDLIBSOPTIONS} -fsanitize=address

${OBJECTDIR}/Core/airpcap.o: Core/airpcap.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/airpcap.o Core/airpcap.c

${OBJECTDIR}/Core/common.o: Core/common.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/common.o Core/common.c

${OBJECTDIR}/Core/core.o: Core/core.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/core.o Core/core.c

${OBJECTDIR}/Core/file.o: Core/file.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/file.o Core/file.c

${OBJECTDIR}/Core/linux.o: Core/linux.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/linux.o Core/linux.c

${OBJECTDIR}/Core/linux_tap.o: Core/linux_tap.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/linux_tap.o Core/linux_tap.c

${OBJECTDIR}/Core/network.o: Core/network.c 
	${MKDIR} -p ${OBJECTDIR}/Core
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/network.o Core/network.c

${OBJECTDIR}/Core/radiotap/radiotap.o: Core/radiotap/radiotap.c 
	${MKDIR} -p ${OBJECTDIR}/Core/radiotap
	${RM} "$@.d"
	$(COMPILE.c) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Core/radiotap/radiotap.o Core/radiotap/radiotap.c

${OBJECTDIR}/Server/Interface.o: Server/Interface.cpp 
	${MKDIR} -p ${OBJECTDIR}/Server
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server/Interface.o Server/Interface.cpp

${OBJECTDIR}/Server/Interfaces.o: Server/Interfaces.cpp 
	${MKDIR} -p ${OBJECTDIR}/Server
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server/Interfaces.o Server/Interfaces.cpp

${OBJECTDIR}/Server/Server.o: Server/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/Server
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server/Server.o Server/Server.cpp

${OBJECTDIR}/Server/Thread.o: Server/Thread.cpp 
	${MKDIR} -p ${OBJECTDIR}/Server
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/Server/Thread.o Server/Thread.cpp

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Werror -I. -ICore -IServer -ITools -I${HOME}/.lib/libev/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc