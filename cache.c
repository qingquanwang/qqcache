#include "sds.h"
#include "dict.h"
#include "zmalloc.h"
#include "Replace.h"
#include "cache.h"
#include <stddef.h>
/*
 * key is reserved by dict
 * dict controls whether dup or destroy the key object, which defined by users
 */
dictType* getDictType(CacheType* cachetype){
    dictType* dtype; 
    dtype=zmalloc(sizeof(dictType));
    dtype->hashFunction = cachetype->hashFunction;
    dtype->keyDup = cachetype->keyDup;
    dtype->valDup = NULL;
    dtype->keyCompare = cachetype->keyCompare;
    dtype->keyDestructor = cachetype->keyDestructor;
    dtype->valDestructor = NULL;
    return dtype;
}
qqCache* createCache(CacheType* cachetype,size_t max_size){
    qqCache *qc = zmalloc(sizeof(qqCache));
    dict* d;
    qc->dtype=getDictType(cachetype);
    qc->type=cachetype;
    qc->maxsize = (max_size>LOWEST_SIZE) ? max_size:LOWEST_SIZE;
    qc->usedsize = 0;
    // create dict
    d=dictCreate(qc->dtype,NULL);
    qc->dict = (void*)d;
    // init Replace Policy
    cachetype->init();
    // init stat
    qc->stat = NULL;
    return qc;
}
void destroyCache(qqCache* qc){
    dictRelease(qc->dict);
    qc->type->destroy(qc->type->valDestructor,&qc->usedsize);
    zfree(qc->dtype);
    zfree(qc);
}
void* STORE(void *(*valDup)(const void*),const void *val){
    if(valDup==NULL)return (void*)val;
    else
        return (*valDup)(val);
}
int cache_set(qqCache* qc,void* key, void* val){
    //first lookup
    void* lookup;
    void* die_key;
    void* wrappedVal;
    size_t datasize;
    lookup = dictFetchValue(qc->dict,key);
    if(lookup==NULL){   // not hit
        datasize = qc->type->valueSize(val)+qc->type->keySize(key);
        //reserve value
        wrappedVal=qc->type->wrapValue(key,STORE(qc->type->valDup,val),datasize);
        dictAdd(qc->dict,key,wrappedVal);
        qc->type->put(wrappedVal);
        qc->usedsize+=datasize;
        while(qc->usedsize > qc->maxsize){
            //dieout will release the rawval space first, 
            //then release the other space regarding Replace Algorithm
            //and update the usedsize
            die_key=qc->type->dieout(qc->type->valDestructor,&qc->usedsize);
            dictDelete(qc->dict,die_key); //key has been destructed
        }
    }else{ //hit
        datasize = qc->type->valueSize(val)+qc->type->keySize(key);
        //update dict
        wrappedVal=qc->type->wrapValue(key,STORE(qc->type->valDup,val),datasize);
        dictReplace(qc->dict,key,wrappedVal);
        //replace and adjust Replace Structure, release the old wrappedVal
        //lookup:oldnode wrappedVal:newnode 
        access(lookup,wrappedVal,&qc->usedsize,qc->type->valDestructor);
        while(qc->usedsize > qc->maxsize){
            //dieout will release the rawval space first, 
            //then release the other space regarding Replace Algorithm
            //and update the usedsize
            die_key=qc->type->dieout(qc->type->valDestructor,&qc->usedsize);
            dictDelete(qc->dict,die_key);
        }
    }
}
void* cache_get(qqCache* qc,const void* key){
    void* lookup;
    void* die_key;
    void* dewrappedVal;
    size_t valsize;
    lookup = dictFetchValue(qc->dict,key);
    if(lookup==NULL){   // not hit
        return NULL;
    }else{
        dewrappedVal=qc->type->dewrapValue(lookup);
        access(lookup,lookup,NULL,NULL);
        return dewrappedVal;
    }
}
