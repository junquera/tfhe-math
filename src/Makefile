#the prefix where you installed tfhe
TFHE_PREFIX=/usr/local
C_INCLUDE_PATH=$C_INCLUDE_PATH:$TFHE_PREFIX/include
CPLUS_INCLUDE_PATH=$CPLUS_INCLUDE_PATH:$TFHE_PREFIX/include
LIBRARY_PATH=$LIBRARY_PATH:$TFHE_PREFIX/lib
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TFHE_PREFIX/lib

INSTALL_PATH=/usr/local

PREFIX=../prefix

CC=g++
CFLAGS=-Wall -I$(TFHE_PREFIX)/include -L$(TFHE_PREFIX)/lib
TFHE_LIB=-ltfhe-fftw

all: $(PREFIX)/lib/libtfhe-math.a $(PREFIX)/include/*.h
	rm *.o

install-lib: $(PREFIX)/lib/libtfhe-math.a
	cp -r $(PREFIX)/lib/* $(INSTALL_PATH)/lib/

install-include: $(PREFIX)/include/*.h
	cp -r $(PREFIX)/include/* $(INSTALL_PATH)/include/

install: all install-lib install-include
	echo "Installed at "$(INSTALL_PATH)

# uninstall

$(PREFIX)/include:
	mkdir -p $(PREFIX)/include

$(PREFIX)/include/*.h: $(PREFIX)/include
	cp -r tfhe-math $(PREFIX)/include/ && find $(PREFIX)/include/ -type f -not -name '*.h' -exec rm {} \;

arithmetic.o:
	$(CC) $(CFLAGS) -c tfhe-math/arithmetic.cpp -o arithmetic.o

$(PREFIX)/lib:
	mkdir -p $(PREFIX)/lib

libtfhe-math.a: arithmetic.o
	ar crf libtfhe-math.a arithmetic.o

$(PREFIX)/lib/libtfhe-math.a: libtfhe-math.a $(PREFIX)/lib
	mv libtfhe-math.a $(PREFIX)/lib/

clean:
	rm -rf $(PREFIX)
