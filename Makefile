GOSRC=libadd.go
CC=gcc
GOC=go
GOLIB=libgoprime.so
CSRC=cmod.c
CLIB=goprime.so

all: build-go-source build-c-wrapper

build-c-wrapper:
	gcc -shared -o $(CLIB) $(CSRC) -I /usr/include/python3.6m/ -fPIC $(GOLIB)

build-go-source:
		go build -buildmode=c-shared -o $(GOLIB) $(GOSRC)

clean:
	rm *.so
