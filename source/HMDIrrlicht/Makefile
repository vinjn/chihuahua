CC=clang++
CFLAGS=-I/usr/include/irrlicht
LDFLAGS=-lIrrlicht

all: HMDTest

HMDTest: HMDTest.o HMDStereoRender.o
	$(CC) $(LDFLAGS) HMDStereoRender.o HMDTest.o -o HMDTest

HMDTest.o: HMDTest.cpp
	$(CC) -c $(CFLAGS) HMDTest.cpp

HMDStereoRender.o: HMDStereoRender.cpp
	$(CC) -c $(CFLAGS) HMDStereoRender.cpp

clean:
	rm -rf *o HMDTest
