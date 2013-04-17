# adarq.org && github.com/adarqui

everything:
	make ht
	make all
	make install
	make t
	make clean

all:
	gcc -Wall -O3 sc2s.c -shared -fPIC -o libsc2s.so

ht:
	sh build.sh inc/
	cp inc/*.h.t /usr/share/sc2s/

install:
	make all
	cp libsc2s.so /lib/
	ldconfig
	mkdir -p /usr/share/sc2s/
	make ht

t:
	make clean
	make all
	gcc -Wall -O3 test.c libsc2s.so -o test ; LD_LIBRARY_PATH=./ ./test

clean:
	rm -f libsc2s.so test
