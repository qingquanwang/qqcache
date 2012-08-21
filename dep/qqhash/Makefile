uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

OPT = -O2



AR = ar

TARGET = lib/libhash.a

all: $(TARGET)

%.o:%.c
	$(CC) $(OPT) $(PROF)  -c $< 
$(TARGET): dict.o sds.o zmalloc.o 
	$(AR) r $(TARGET) dict.o sds.o zmalloc.o


	
.PHONY: all clean 

clean:
	rm -rf $(TARGET) *.a *.o *.gcda *.gcno *.gcov


gprof:
	$(MAKE) PROF="-pg" OPT=" -O0"

gcov:
	$(MAKE) PROF="-fprofile-arcs -ftest-coverage" OPT="-O0"


32bitgprof:
	$(MAKE) PROF="-pg" ARCH="-arch i386" OPT="-O0"

