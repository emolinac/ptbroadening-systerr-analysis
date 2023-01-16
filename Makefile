BIN := ./bin
INC := ./include
SRC := ./src

CXX := g++ -std=c++17
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/calculate-meanpt2-systerror 

${BIN}/calculate-meanpt2-systerror: ${SRC}/calculate-meanpt2-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-meanpt2-systerror.cpp -o ${BIN}/calculate-meanpt2-systerror ${ROOTLIBS}

clean:
	rm ${BIN}/*
