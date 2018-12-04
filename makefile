
all: file cipherengine main
	g++ file cipherengine main

file: file.h
	g++ -c file.cpp -o file

cipherengine: cipherengine.h
	g++ -c cipherengine.cpp -o cipherengine

main:
	g++ -c main.cpp -o main

clean:
	rm -rf file cipherengine main a.out
