NAME        := lab_10_11
BIN_DIR     := ./bin

.PHONY: build

build:
	mkdir -p ${BIN_DIR}
	mpic++ -std=c++17 main.cpp -o ${BIN_DIR}/${NAME}

run: build
	mpirun -n 4 ${BIN_DIR}/${NAME}