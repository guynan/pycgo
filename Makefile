CC=gcc
GOC=go
GOLIB=libgoprime.so
CSRC=wrapper.c
GOSRC=goprime.go
CLIB=goprime.so

CFLAGS= -Wall -Wextra -pedantic --std=c99 -Werror

PY_DEV_PATH=/usr/include/python3.6m/
PYTHON_TEST_SRC=test.py

all: build-go-source build-c-wrapper

build-c-wrapper:
	gcc -shared -fPIC $(CFLAGS) -o $(CLIB) $(CSRC) -I $(PY_DEV_PATH) ./$(GOLIB)

build-go-source:
		go build -buildmode=c-shared -o $(GOLIB) $(GOSRC)

c: build-c-wrapper

test:
	python3 $(PYTHON_TEST_SRC)

time:
	time python3 $(PYTHON_TEST_SRC)

clean:
	rm *.so
