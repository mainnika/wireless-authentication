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
CC=mips-buildroot-linux-uclibc-gcc
CCC=mips-buildroot-linux-uclibc-g++
CXX=mips-buildroot-linux-uclibc-g++
FC=gfortran
AS=mips-buildroot-linux-uclibc-as

# Macros
CND_PLATFORM=MIPSEL-Linux-x86
CND_DLIB_EXT=so
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/main.o \
	${OBJECTDIR}/osdep/airpcap.o \
	${OBJECTDIR}/osdep/common.o \
	${OBJECTDIR}/osdep/file.o \
	${OBJECTDIR}/osdep/linux.o \
	${OBJECTDIR}/osdep/linux_tap.o \
	${OBJECTDIR}/osdep/network.o \
	${OBJECTDIR}/osdep/osdep.o \
	${OBJECTDIR}/radiotap/radiotap.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L/home/mainnika/.lib_mips/pcap/lib -lpcap

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/wirelesslistener ${OBJECTFILES} ${LDLIBSOPTIONS} -static

${OBJECTDIR}/main.o: main.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

${OBJECTDIR}/osdep/airpcap.o: osdep/airpcap.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/airpcap.o osdep/airpcap.c

${OBJECTDIR}/osdep/common.o: osdep/common.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/common.o osdep/common.c

${OBJECTDIR}/osdep/file.o: osdep/file.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/file.o osdep/file.c

${OBJECTDIR}/osdep/linux.o: osdep/linux.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/linux.o osdep/linux.c

${OBJECTDIR}/osdep/linux_tap.o: osdep/linux_tap.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/linux_tap.o osdep/linux_tap.c

${OBJECTDIR}/osdep/network.o: osdep/network.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/network.o osdep/network.c

${OBJECTDIR}/osdep/osdep.o: osdep/osdep.c 
	${MKDIR} -p ${OBJECTDIR}/osdep
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/osdep/osdep.o osdep/osdep.c

${OBJECTDIR}/radiotap/radiotap.o: radiotap/radiotap.c 
	${MKDIR} -p ${OBJECTDIR}/radiotap
	${RM} "$@.d"
	$(COMPILE.c) -O2 -Werror -I/home/mainnika/.lib_mips/pcap/include -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/radiotap/radiotap.o radiotap/radiotap.c

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
