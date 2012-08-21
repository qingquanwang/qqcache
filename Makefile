uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OPT = -O2
DEP = dep/qqhash/lib/libhash.a
DEPDIR = dep/qqhash
INCLUDEPATH = -Idep/qqhash/include -I.

AR = ar

TARGET = lib/libqqcache.a

all: $(TARGET) $(DEP)

%.o:%.c
	$(CC) $(CFLAGS) $(OPT) $(PROF) $(INCLUDEPATH)  -c $< 

$(TARGET): cache.o sdscache.o LRU.o
	$(AR) r $(TARGET) cache.o sdscache.o LRU.o

$(DEP):
	cd $(DEPDIR)
	$(MAKE)
	cd ../..
	
.PHONY: all clean 

clean:
	rm -rf $(TARGET) *.a *.o *.gcda *.gcno *.gcov


gprof:
	$(MAKE) PROF="-pg" OPT=" -O0"

gcov:
	$(MAKE) PROF="-fprofile-arcs -ftest-coverage" OPT="-O0"


