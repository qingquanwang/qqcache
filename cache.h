#ifndef __CACHE_H
#define __CACHE_H
#define LOWEST_SIZE 4096
#define CACHE_OK 0
/*
 * The workflow is kept by cache.c
 * But every detail to handle the key or value, or replacement policy 
 * depends on the callback functions below
 */
typedef struct CacheType{
/*
 * Dict Type, regarding to hash and storage
 * values are reserved by Cache
 */
    unsigned int (*hashFunction)(const void *key);
    void *(*keyDup)(void *privdata, const void *key);
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    void (*keyDestructor)(void *privdata, void *key);
/*
 * Cache Related
 */
    void (*valDestructor)(void *obj);
    void *(*valDup)(const void *obj);
    size_t (*valueSize)(void *value);
    size_t (*keySize)(void *key);
/*
 * Replace Type
 */
    void* (*wrapValue)(void* key,void* rawVal,size_t size);
    void* (*dewrapValue)(void* val);
    void* (*dieout)(void (*destructor)(void*),size_t* usedsize);
    void (*put)(void *wrappedValue);
    void (*access)(void* oldWrappedValue,void *wrappedValue, size_t* usedsize, \
         void (*valDestructor)(void*));
    void (*init)();
    void (*destroy)(void(*valDestructor)(void*),size_t* usedsize);
}CacheType;

/*
 * No stat info until now
 */
typedef struct qqCache{
    CacheType* type;
    size_t maxsize;
    size_t usedsize;
    void* stat;
    void* dict;
    dictType* dtype;
}qqCache;

qqCache* createCache(CacheType* cachetype, size_t max_size);
void destroyCache(qqCache* cache);
int  cache_set(qqCache* cache, void* key, void* value);
void* cache_get(qqCache* cache, const void* key);

/*
 * defined in dnscache & sdscache, they are for general purpose, for the convenience
 * Agile config needs too many callbacks which is somehow a burden to users
 * But the below templates can give you a simple way
 */
extern CacheType dnsLRU2Cache;
extern CacheType sdsLRU2Cache;
extern CacheType sdsLRUCache;
#endif
