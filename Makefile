BIN := ./bin
INC := ./include
SRC := ./src

CXX := g++ -std=c++17
CFLAGS := -Wall -g

ROOTCFLAGS  := $(shell root-config --cflags)
ROOTLDFLAGS := $(shell root-config --ldflags)
ROOTLIBS    := $(shell root-config --libs) -lEG

all: ${BIN}/calculate-meanpt2-zh-systerror ${BIN}/calculate-broadening-zh-systerror ${BIN}/print-plot-deviations-broadening ${BIN}/print-plot-deviations-meanpt2 \
	 ${BIN}/print-plot-broadening-q2nuzh ${BIN}/print-plot-broadening-q2nuzha13 ${BIN}/calculate-broadening-differential-systerror ${BIN}/calculate-meanpt2-differential-systerror \
	 ${BIN}/print-plot-broadening-zh ${BIN}/print-plot-broadening-a13 ${BIN}/calculate-broadening-a13-systerror

${BIN}/calculate-meanpt2-zh-systerror: ${SRC}/calculate-meanpt2-zh-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-meanpt2-zh-systerror.cpp -o ${BIN}/calculate-meanpt2-zh-systerror ${ROOTLIBS}

${BIN}/calculate-broadening-a13-systerror: ${SRC}/calculate-broadening-a13-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-broadening-a13-systerror.cpp -o ${BIN}/calculate-broadening-a13-systerror ${ROOTLIBS}

${BIN}/calculate-broadening-zh-systerror: ${SRC}/calculate-broadening-zh-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-broadening-zh-systerror.cpp -o ${BIN}/calculate-broadening-zh-systerror ${ROOTLIBS}

${BIN}/calculate-broadening-differential-systerror: ${SRC}/calculate-broadening-differential-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-broadening-differential-systerror.cpp -o ${BIN}/calculate-broadening-differential-systerror ${ROOTLIBS}

${BIN}/calculate-meanpt2-differential-systerror: ${SRC}/calculate-meanpt2-differential-systerror.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/calculate-meanpt2-differential-systerror.cpp -o ${BIN}/calculate-meanpt2-differential-systerror ${ROOTLIBS}

${BIN}/print-plot-deviations-broadening: ${SRC}/print-plot-deviations-broadening.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-deviations-broadening.cpp -o ${BIN}/print-plot-deviations-broadening ${ROOTLIBS}

${BIN}/print-plot-deviations-meanpt2: ${SRC}/print-plot-deviations-meanpt2.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-deviations-meanpt2.cpp -o ${BIN}/print-plot-deviations-meanpt2 ${ROOTLIBS}

${BIN}/print-plot-broadening-q2nuzh: ${SRC}/print-plot-broadening-q2nuzh.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-q2nuzh.cpp -o ${BIN}/print-plot-broadening-q2nuzh ${ROOTLIBS}

${BIN}/print-plot-broadening-q2nuzha13: ${SRC}/print-plot-broadening-q2nuzha13.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-q2nuzha13.cpp -o ${BIN}/print-plot-broadening-q2nuzha13 ${ROOTLIBS}

${BIN}/print-plot-broadening-zh: ${SRC}/print-plot-broadening-zh.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-zh.cpp -o ${BIN}/print-plot-broadening-zh ${ROOTLIBS}

${BIN}/print-plot-broadening-a13: ${SRC}/print-plot-broadening-a13.cpp
	${CXX} -I${INC} ${ROOTCFLAGS} ${SRC}/print-plot-broadening-a13.cpp -o ${BIN}/print-plot-broadening-a13 ${ROOTLIBS}

clean:
	rm ${BIN}/*
