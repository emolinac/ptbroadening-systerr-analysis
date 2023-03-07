BIN := ./bin
INC := ./include
SRC := ./src

CXX := g++ -std=c++17
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/calculate-meanpt2-systerror ${BIN}/calculate-broadening-systerror ${BIN}/print-plot-deviations-broadening ${BIN}/print-plot-deviations-meanpt2

${BIN}/calculate-meanpt2-systerror: ${SRC}/calculate-meanpt2-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-meanpt2-systerror.cpp -o ${BIN}/calculate-meanpt2-systerror ${ROOTLIBS}

${BIN}/calculate-broadening-systerror: ${SRC}/calculate-broadening-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-broadening-systerror.cpp -o ${BIN}/calculate-broadening-systerror ${ROOTLIBS}

${BIN}/print-plot-deviations-broadening: ${SRC}/print-plot-deviations-broadening.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-deviations-broadening.cpp -o ${BIN}/print-plot-deviations-broadening ${ROOTLIBS}

${BIN}/print-plot-deviations-meanpt2: ${SRC}/print-plot-deviations-meanpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-deviations-meanpt2.cpp -o ${BIN}/print-plot-deviations-meanpt2 ${ROOTLIBS}

clean:
	rm ${BIN}/*
