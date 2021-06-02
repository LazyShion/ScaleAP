#!/bin/bash

CC = g++ -std=c++11
CFLAGS = -O3
EXEC = ScaleAP convert
OBJ1 = data.o scaleap.o

all: $(EXEC)

ScaleAP: $(OBJ1) main.o
	$(CC) -o $@ $^ $(CFLAGS)

convert: convert.cpp
	$(CC) $(CFLAGS) -o $@ $<
	
%.o: %.cpp %.hpp
	$(CC) -o $@ -c $< $(CFLAGS)
	
%.o: %.cpp
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm $(EXEC) *.o	
