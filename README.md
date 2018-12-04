# simple_aes_simulation
A simple simulation of AES encryption, including Substitution, Padding, Shifting Rows, Parity, and Mixing Columns.

Usage only requires an updated `g++`.

Build instructions:
* `cd` into directory
* `make all` (If you wish to build on your own, view `makefile`'s commands)

Instructions:
	1. Ensure that all provided files are in the same directory:
		a) file.h
		b) file.cpp
		c) cipherengine.h
		d) cipherengine.cpp
		e) main.cpp
		f) makefile
		
	2. Execute "make"
		NOTE: No special compiler flags are necessary, and C99 is used
		
		a) Alternatively, simply execute "g++ *.cpp"
		
	3. Provide the program:
		a) A plaintext file
		b) A key file
		c) An output filename
	
	4. Enjoy!
