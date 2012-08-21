#ifndef __REPLACE_H
#define __REPLACE_H
/*
 * Interface:
 *  1) init: create the data structures, initialize stat and internal variables
 *  2) destroy: release all the dynamic allocation space in list or node or value
 *  3) wrapValue: expand the raw value pointer to a node pointer containing
 *                rawVal,node info, This is used to be stored in the dict
 *  4) dewrapValue: return rawVal from wrapValue
 *  5) put: add a new element into Replace Structure
 *          update the element's key/timestamp/size
 *  6) dieout: the die out element, return the key
 *  7) access: 
 *      1) update the oldwrappedValue with newwrappedValue, changing usedsize, dropping the old one
 *      2) changing the Replace Data Structure
 *  8) dump:
 *      debug
 */
void init_lru();
void init_plru();
void destroy_lru(void (*destructor)(void*),size_t* usedsize);
void destroy_plru(void (*destructor)(void*),size_t* usedsize);
void* dieout_lru(void (*destructor)(void*),size_t* usedsize);
void* dieout_plru(void(*destructor)(void*),size_t* usedsize);
void put_lru(void* node);
void put_plru(void* node);
void access_lru(void* oldNode,void* newNode,size_t* usedsize, \
            void(*valDestructor)(void*));
void access_plru(void* oldNode,void* newNode,size_t* usedsize, \
            void(*valDestructor)(void*));
void* wrapValue_lru(void* key,void* rawval,size_t size);
void* wrapValue_plru(void* key,void* rawval,size_t size);
void* dewrapValue_lru(void* node);
void* dewrapValue_plur(void* node);
void dump_lru();
void dump_plru();
/*
 * Wrapper of operations on list
 */


#define INIT_LIST(l) \
    l->head->next = l->tail; \
    l->tail->before = l->head; \
    l->node_num=0
#define FREENODE(node) \
    zfree(node)
#define DUMMYNODE(node) \
    node->before = node->next = node->key = node->val=  NULL
#define NEXTNODE(node) node->next
#define BEFORENODE(node) node->before
#define LISTLEN(l) l->node_num
#define DELETE(node) \
    node->before->next = node->next; \
    node->next->before = node->before; \
    node->before=node->next=NULL 
#define POPHEAD(l,node) \
    node=l->head->next; \
    DELETE(node)
#define PUSHTAIL(l,node) \
    l->tail->before->next = node; \
    node->next = l->tail; \
    node->before = l->tail->before; \
    l->tail->before = node


#endif
