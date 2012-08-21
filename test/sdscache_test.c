#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "sds.h"
#include "dict.h"
#include "zmalloc.h"
#include "Replace.h"
#include "cache.h"
#define N 10
int
main(int argc, char** argv){
    char buf[100];
    int i;
    sds s,v;
    qqCache* cache = createCache(&sdsLRUCache,4096);
    for(i=0;i<N;++i){
        snprintf(buf,100,"%d",i);
        s=sdsnew(buf);
        snprintf(buf,100,"%d",100-i);
        v=sdsnew(buf);
        cache_set(cache,s,v);
    }
    //check
    for(i=0;i<N;++i){
        snprintf(buf,100,"%d",i);
        s=sdsnew(buf);
        printf("Key:%s Value:%s\n",s,(sds)cache_get(cache,s));
        sdsfree(s);
    }
    destroyCache(cache);
    return 0;
}
