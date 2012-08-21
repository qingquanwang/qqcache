uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OPT = -O2
ROOTDIR = ..
DEP =  $(ROOTDIR)/lib/libqqcache.a $(ROOTDIR)/dep/qqhash/lib/libhash.a
INCLUDEPATH = -I$(ROOTDIR)/dep/qqhash/include -I$(ROOTDIR)

AR = ar

TARGET =  sdslru_test

all: $(TARGET)

%.o:%.c
	$(CC) $(CFLAGS) $(OPT) $(PROF) $(INCLUDEPATH)  -c $< 

$(TARGET): sdscache_test.o
	$(CC) $(LDFLAGS) $(OPT) $(PROF) $(INCLUDEPATH) -o $(TARGET) sdscache_test.o $(DEP)

	
.PHONY: all clean 

clean:
	rm -rf $(TARGET) *.a *.o *.gcda *.gcno *.gcov


gprof:
	$(MAKE) PROF="-pg" OPT=" -O0"

gcov:
	$(MAKE) PROF="-fprofile-arcs -ftest-coverage" OPT="-O0"


