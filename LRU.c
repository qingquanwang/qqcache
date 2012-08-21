/*
 * push from tail,pop from head
 */
#include <stddef.h>
#include "Replace.h"

#ifdef CACHE_ONLY_USEMALLOC
#define zmalloc malloc
#define zfree free
#include <stdlib.h>
#else
#include "zmalloc.h"
#endif

#include <time.h>
#include <stdio.h>

typedef struct LRU_node{
    void*     key;
    void*     val; 
    struct LRU_node* next;
    struct LRU_node* before;
    unsigned  timestamp;
    size_t    datasize;
} LRU_node;

typedef struct LRU_list{
    LRU_node *head;
    LRU_node *tail;
    int       node_num;
} LRU_list;

LRU_list *lru;

#define LRU_CLOCK_RESOLUTION 10
#define LRU_CLOCK_MAX ((1<<21)-1)


unsigned lruclock(){
    return (time(NULL)/LRU_CLOCK_RESOLUTION) & LRU_CLOCK_MAX;
}
void* dewrapValue_lru(void* node){
    return ((LRU_node*)node)->val;
}
void* wrapValue_lru(void* key,void* rawval,size_t size){
    LRU_node* n = zmalloc(sizeof(LRU_node));
    DUMMYNODE(n);
    n->key=key;
    n->val=rawval;
    n->datasize=size;
    n->timestamp=lruclock();
    return n;
}
void init_lru(){
    lru = zmalloc(sizeof(LRU_list));
    lru->head = zmalloc(sizeof(LRU_node));
    lru->tail = zmalloc(sizeof(LRU_node));
    DUMMYNODE(lru->head);
    DUMMYNODE(lru->tail);
    INIT_LIST(lru);
}

void destroy_lru( void(*destructor)(void*),size_t* usedsize){
    LRU_node *c,*n;
    size_t s=0;
    c=n=lru->head;
    while(c!=NULL){
        n=c->next;
        if(destructor)
            (*destructor)(c->val);
        s+=c->datasize;
        FREENODE(c);
        c=n;
    }
    *usedsize = (*usedsize) - s;
    zfree(lru);
}
/*
 * pop from head
 */
void* dieout_lru(void (*destructor)(void*),size_t* usedsize){
    void *key;
    LRU_node *n;
    if(LISTLEN(lru)==0)return;
    POPHEAD(lru,n);
    --LISTLEN(lru);
    key=n->key;
    if(destructor)
        (*destructor)(n->val);
    *usedsize -= n->datasize;
    FREENODE(n);
    return key;
}

/*
 * push from tail
 */
void put_lru(void *n){
    LRU_node* node=(LRU_node*)n;
    node->timestamp=lruclock();
    PUSHTAIL(lru,node);
    ++LISTLEN(lru);
}

/*
 * move to tail
 */
void access_lru(void* oldn, void* newn,size_t* usedsize, void(*destructor)(void*)){
    LRU_node *on,*nn;
    on = (LRU_node*)oldn;
    nn = (LRU_node*)newn;
    if(usedsize == NULL){
        //delete node from the list
        DELETE(on);
        PUSHTAIL(lru,on);
    }else{
        if(destructor)
            (*destructor)(on->val);
        on->val = nn->val;
        on->datasize = nn->datasize;
        *usedsize+=(nn->datasize-on->datasize);
        on->timestamp=lruclock();
        DELETE(on);
        PUSHTAIL(lru,on);
        
    }
}
/*
 * For unit test
 */
void dump_lru(){
    LRU_node* c;
    c=lru->head->next;
    while(c!=lru->tail){
        printf("%d",*(int*)(c->val));
        c=c->next;
    }
    printf("\n");
}
