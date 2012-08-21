qqhash
======

My hash library, extracted from dict in redis
How to use it:
     build a dictType, you can build many dictType templates to satisfy your different requisites.
     A dictType will specify: hash function, keyDup,keyDestructor,ValDup,ValDestructor,keyCompare

Advantages:
     1) This library can be widely used, because there is no limitation of key,value data type.
     2) you can choose whether the hashtable should maintain the key or value, 
        which means, the hashtable can help you release the dynamic memory allocation of key or values.
     3) Easily tune the hash function
     According to the above advantages, you can maintain some dictType template for duplicated use.
     For example, stringDictType, stringIPDictType,...
Implementation:
     1) leverage the callback function in dictType to implement a high available, easily configurable hashtable
     2) Internally, it uses 2 real hashtable to implement rehashing--dynamic increase
Performance:
     Stay tuned

