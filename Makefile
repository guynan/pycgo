CC=gcc
GOC=go
GOLIB=libgoprime.so
GOSRC=goprime.go
CLIB=goprime.so

CFLAGS= -Wall -Wextra -pedantic --std=c99 -Werror

PY_DEV_PATH=/usr/include/python3.6m/
PYTHON_TEST_SRC=test.py

all: compile-lib

gohelper.o: gohelper.h
	gcc -shared -fPIC $(CFLAGS) -c gohelper.c -I $(PY_DEV_PATH)

wrapper.o: wrapper.h
	gcc -shared -fPIC $(CFLAGS) -c wrapper.c -I $(PY_DEV_PATH)

libgoprime.so:
		go build -buildmode=c-shared -o $(GOLIB) $(GOSRC)

build-go-source: libgoprime.so

build-c-wrapper: gohelper.o wrapper.o 

goprime.so: libgoprime.so gohelper.o wrapper.o
	gcc -shared -fPIC $(CFLAGS) -o $(CLIB) -I $(PY_DEV_PATH) \
				./$(GOLIB) *.o

compile-lib: goprime.so

c: build-c-wrapper

go: build-go-source

test:
	python3 $(PYTHON_TEST_SRC)

time:
	time python3 $(PYTHON_TEST_SRC)

clean:
	rm -f *.so *.o libgoprime.h
