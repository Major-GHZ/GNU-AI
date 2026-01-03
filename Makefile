CC = gcc
NVCC = nvcc
CFLAGS = -Iinclude -Wall
CUDAFLAGS = -Iinclude
LDFLAGS = -L/usr/local/cuda/lib64 -lcudart -lcuda

all: mon_llm

mon_llm: src/main.o src/agent.o src/web.o src/cuda_utils.o
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) -lcurl

src/main.o: src/main.c include/agent.h include/web.h
	$(CC) $(CFLAGS) -c $< -o $@

src/agent.o: src/agent.c include/agent.h
	$(CC) $(CFLAGS) -c $< -o $@

src/web.o: src/web.c include/web.h
	$(CC) $(CFLAGS) -c $< -o $@

src/cuda_utils.o: src/cuda_utils.cu include/cuda_utils.h
	$(NVCC) $(CUDAFLAGS) -c $< -o $@

clean:
	rm -f mon_llm src/*.o
