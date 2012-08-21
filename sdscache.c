#include "sds.h"
#include "dict.h"
#include "zmalloc.h"
#include "Replace.h"
#include "cache.h"
#include <stddef.h>
void dictSdsDestructor(void *privdata, void *val)
{
    ((void)privdata);
    zfree(val);
}
/*
 * The method to compare two sds keys
 */
int dictSdsKeyCompare(void *privdata, const void *key1,
        const void *key2)
{
     int l1,l2;
     ((void)privdata);
     l1 = sdslen((sds)key1);
     l2 = sdslen((sds)key2);
     if (l1 != l2) return 0;
     return memcmp(key1, key2, l1) == 0;
}
/*
 * The hash function of the key
 */
unsigned int dictSdsHash(const void *key) {
     return dictGenHashFunction((unsigned char*)key, sdslen((char*)key));
}
void* sdsDup(void *privdata, const void *key)
{
    ((void)privdata);
    return sdsdup((sds)key);
}
void sdsDestructor(void *key){
    return sdsfree(key);
}

void sdsDictDestructor(void* priv,void *key){
    ((void)priv);
    return sdsfree(key);
}
size_t sdsSize(void *s){
    return sdslen((sds)s);
}
CacheType sdsLRUCache = {
/*
 * Dict Type, regarding to hash and storage
 * values are reserved by Cache
 */
    dictSdsHash,
    NULL,
    dictSdsKeyCompare,
    sdsDictDestructor,
/*
 * Cache Related
 */
    sdsDestructor,
    NULL,
    sdsSize,
    sdsSize,
/*
 * Replace Type
 */
    wrapValue_lru,
    dewrapValue_lru,
    dieout_lru,
    put_lru,
    access_lru,
    init_lru,
    destroy_lru,
};

