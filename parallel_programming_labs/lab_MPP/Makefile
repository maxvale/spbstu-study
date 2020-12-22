NAME        := lab_12
BIN_DIR     := ./bin

.PHONY: build

lib:
	mkdir -p ${BIN_DIR}
	gcc -fpic -c -o ${BIN_DIR}/func_0 func_clear.c
	gcc -fpic -c -o ${BIN_DIR}/func_1 func_vec.c
	gcc -fpic -c -fopenmp -o ${BIN_DIR}/func_2 func_omp.c
	gcc -fpic -c -fopenmp -o ${BIN_DIR}/func_3 func_vec_omp.c
build:
	gcc -shared -fPIC -fopenmp -o func_c.so func_clear.c func_omp.c func_vec.c func_vec_omp.c