include ../Makefile.common

OBJS = conf.o snode.o meta.o index.o index2.o memobj.o topology.o sysstat.o log.o sphere.o constant.o crypto.o transport.o datachn.o dhash.o routing.o ssltransport.o tcptransport.o threadpool.o

all: libcommon.so libcommon.a

%.o: %.cpp
	$(C++) -fPIC $(CCFLAGS) $< -c

libcommon.so: $(OBJS)
	$(C++) -shared -o $@ $^

libcommon.a: $(OBJS)
	ar -rcs $@ $^

clean:
	rm -f *.o *.so *.a

install:
	mv libcommon.so libcommon.a ../lib
