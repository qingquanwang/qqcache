qqcache
=======

Cache,  very Agile by leveraging the redis dict, easily replace both key/value data types or Cache Replacement Policy



Advantages: (highly configurable)
	1) support the key/value with different or high level data types, by leveraging the qqhash
    2) define the interfaces of cache replacement policy,so you can just define another replacement policy easily, or 
       switch between these policies.
       cache will control the workflow, you just need to provide 3 operations put/dieout/access.

Benchmark:
	NEED TO DO
