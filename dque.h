/**
 *
 *  \file	dque.h
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	header for double ended queue functions.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *  1.1		07/30/2019  	D.Anderson  added dque_key_find, dque_key_insert, dque_key_remove, 
 *                                          dque_error_len, and changed dque_error signature
 *
 *  These functions are implemented using a cicular doubly-linked list. All functions have a runtime of O(1)
 *  except dque_insert, dque_remove, dque_key_find, dque_key_insert, and dque_key_remove which are O(n).
 *
 *  These routines can be used to implement several Abstract Data Types (ADT) including FIFO, LIFO (stack),
 *  and Priority queues in C code. These ADTs are usually included in the libraries higher level languages
 *  such as C++, Java, and Python, but not in C. While all of these ADTs can be implemneted in C, without
 *  polymophism, inheritance, and operator overloading, it is difficult to implement the ADTs in a data
 *  independent manner as a standard library routine.
 *
 *  \return All routines return zero for success or a non-zero number for a failure.
 *
 *  If the #define INSERT_DEF is not defined then a short form of all of the functions can be used.
 *  The short form eliminates the "dque_" prefix to all of the functions.  This is useful in substituting 
 *  abstract data types without rewriting the code. If the user includes tree.h, for example or any other
 *  headers that use INSERT_DEF #define, the first one included will use the short forms.  All others
 *  must use their long forms.
 * 
 * Copyright (c) 2019 Dale Anderson <daleanderson488@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  member functions
 *      dque_create(    qhead **, unsigned, char * );  - create an empty queue
 *      dque_destroy(   qhead **                   );  - destroy a queue, free all dynamic memory allocations
 *      dque_error(     dque_err, char *, unsigned  ); - return error message for specified error
 *      dque_error_len( dque_err,         unsigned *); - return length of error message for specified error
 *      dque_options(   qhead *, int option, ...   );  - control queue behavior with options and arguments as follows:
 *          DQUEOPT_VERSION, int *major, int *minor    - returns major and minor library version numbers
 *          DQUEOPT_NODECNT, unsigned  siz             - sets the # of nodes allocated each time nodes are needed
 *          DQUEOPT_HEADSIZ, unsigned *siz             - returns size of queue head structure
 *          DQUEOPT_NODESIZ, unsigned *siz             - returns size of queue node structure
 *          DQUEOPT_NOALLOC, unisgned  siz, char *buf  - no dynamic allocation, uses buf of size siz for all nodes
 *          DQUEOPT_NODUPE,  unsigned int              - !0 = no duplicates in priority queue, 0 = allow duplicates
 *          DQUEOPT_NOSCAN,  unsigned int              - !0 = no scan on insert, remove, 0 = scan on insert,remove
 *
 *  capacity
 *      dque_empty(     qhead *,  unsigned int *   );  - return non-zero value if queue is empty, otherwise zero
 *      dque_size(      qhead *,  unsigned int *   );  - return current number of nodes in the queue
 *      dque_max_size(  qhead *,  unsigned int *   );  - return current number of nodes in the queue and free list
 *
 *  element access
 *      dque_back(      qhead *,  void **          );  - return pointer to user's data in the last element in the queue
 *      dque_front(     qhead *,  void **          );  - return pointer to user's data in the front element in the queue
 *      dque_key_find(  qhead *,  void *, COMPFUNC, void ** ); - return a pointer to an iterator to the element specified by the key
 *      dque_get_data(  qiter *,  void **          );  - return a pointer to the user's data in the element pointed to by the iterator
 *      dque_set_data(  qiter *,  void *           );  - set the user's data in the element pointed to by the iterator
 *
 *  iterators
 *      dque_begin(     qhead *, qiter **          );  - return forward iterator to the front of the queue
 *      dque_rbegin(    qhead *, qiter **          );  - return reverse iterator to the back of the queue
 *      dque_next(      qhead *, qiter **          );  - return next forward iterator towards the back of the queue
 *      dque_rnext(     qhead *, qiter **          );  - return next reverse iterator towards the front of the queue
 *      dque_end(       qhead *, qiter **          );  - return forward iterator to stop iteration of the queue
 *      dque_rend(      qhead *, qiter **          );  - return reverse iterator to stop iteration of the queue
 *
 *  modifiers
 *      dque_delete(    qhead *,  void **, qiter * );  - alias for dque_erase() in C only
 *      dque_erase(     qhead *,  void **, qiter * );  - remove qnode specified by iterator, return pointer to user's data
 *      dque_insert(    qhead *,  void *,  qiter * );  - insert new qnode with user's data just before node specified by iterator
 *      dque_key_delete(qhead *,  void *, COMPFUNC, void ** ); - alias for dque_key_erase() in C only
 *      dque_key_erase( qhead *,  void *, COMPFUNC, void ** ); - remove first qnode with specified key and return pointer to the user's data
 *      dque_key_insert(qhead *,  void *, COMPFUNC, void *  ); - insert new qnode with the user's data into ordered queue
 *      dque_pop_back(  qhead *,  void **          );  - remove qnode from the back of the queue, return pointer to the user's data
 *      dque_pop_front( qhead *,  void **          );  - remove qnode from the front of the queue, return pointer to the user's data
 *      dque_push_back( qhead *,  void *           );  - insert new qnode with user's data onto the back of the queue
 *      dque_push_front(qhead *,  void *           );  - insert new qnode with user's data onto the front of the queue
 *
 */

#ifndef DQUE_H
#define DQUE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DQUE_QHEAD_DEF
typedef struct dque_qhead { int type; } dque_qhead, *dque_qheadp, **dque_qheadpp;
#endif

#ifndef NULL_QUEUE
#define NULL_QUEUE      (dque_qhead *)NULL  /* a NULL queue pointer         */
#define NULL_QUEUEP     (dque_qhead **)NULL
#endif

#ifndef DQUE_QITER_DEF
typedef struct dque_qiter { int type; } dque_qiter, *dque_qiterp, **dque_qiterpp;
#endif

#ifndef NULL_QITER
#define NULL_QITER      (dque_qiter *)NULL  /* a NULL qiter pointer         */
#define NULL_QITERP     (dque_qiter **)NULL
#endif

#ifndef COMFUNC_DEF
typedef int (* COMPFUNC)( void *, void * );
#endif

#ifndef DQUE_ERR_DEF
/* error numbers deliberately not defined here. */
typedef enum dque_err {
        DQUEERR_NOERR       =  0,
        DQUEERR_UNKERR      =  1,
        DQUEERR_NOALLOC     =  2,
        DQUEERR_NOQUEUE     =  3,
        DQUEERR_NOQUEUEP    =  4,
        DQUEERR_NODATA      =  5,
        DQUEERR_NODATAP     =  6,
        DQUEERR_NOITER      =  7,
        DQUEERR_NOITERP     =  8,
        DQUEERR_BUFSIZE     =  9,
        DQUEERR_MINCNT      = 10,
        DQUEERR_EMPTY       = 11,
        DQUEERR_NOKEY       = 12,
        DQUEERR_NOCOMP      = 13,
        DQUEERR_ALLOCNA     = 14,
        DQUEERR_NOTFOUND    = 15,
        DQUEERR_UNKOPT      = 16,
        DQUEERR_NODUPE      = 17,
        DQUEERR_INVITER     = 18,
        DQUEERR_NOBUF       = 19,
        DQUEERR_INVHDR      = 20
	} dque_err;
#endif

/* member functions */
extern dque_err dque_create(    dque_qhead **, unsigned int,     char *        );
extern dque_err dque_destroy(   dque_qhead **                                  );
extern dque_err dque_error(     dque_err,      char  *,          unsigned int  );
extern dque_err dque_error_len( dque_err,                        unsigned int *);
extern dque_err dque_options(   dque_qhead *,  int, ...                        );
/* capacity */
extern dque_err dque_empty(     dque_qhead *,  unsigned int *                  );
extern dque_err dque_size(      dque_qhead *,  unsigned int *                  );
extern dque_err dque_max_size(  dque_qhead *,  unsigned int *                  );
/* element access */
extern dque_err dque_back(      dque_qhead *,  void **                         );
extern dque_err dque_front(     dque_qhead *,  void **                         );
extern dque_err dque_key_find(  dque_qhead *,  void *, COMPFUNC, dque_qiter ** );
extern dque_err dque_get_data(  dque_qiter *,  void **                         );
extern dque_err dque_set_data(  dque_qiter *,  void *                          );
/* iterators */
extern dque_err dque_begin(     dque_qhead *,                    dque_qiter ** );
extern dque_err dque_rbegin(    dque_qhead *,                    dque_qiter ** );
extern dque_err dque_next(      dque_qhead *,                    dque_qiter ** );
extern dque_err dque_rnext(     dque_qhead *,                    dque_qiter ** );
extern dque_err dque_end(       dque_qhead *,                    dque_qiter ** );
extern dque_err dque_rend(      dque_qhead *,                    dque_qiter ** );
/* modifiers */
extern dque_err dque_erase(     dque_qhead *,  void **,          dque_qiter *  );
extern dque_err dque_insert(    dque_qhead *,  void *,           dque_qiter *  );
extern dque_err dque_key_erase( dque_qhead *,  void *, COMPFUNC, void **       );
extern dque_err dque_key_insert(dque_qhead *,  void *, COMPFUNC, void *        );
extern dque_err dque_pop_back(  dque_qhead *,  void **                         );
extern dque_err dque_pop_front( dque_qhead *,  void **                         );
extern dque_err dque_push_back( dque_qhead *,  void *                          );
extern dque_err dque_push_front(dque_qhead *,  void *                          );

/* dque options                                                             */
#define DQUEOPT_NOOPT       0
#define DQUEOPT_VERSION     1
#define DQUEOPT_NODECNT     2
#define DQUEOPT_HEADSIZ     3
#define DQUEOPT_NODESIZ     4
#define DQUEOPT_NOALLOC     5
#define DQUEOPT_NODUPE      6
#define DQUEOPT_NOSCAN      7

/* short forms of the function names if INSERT_DEF is not defined           */

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L))    /* conforms to C11 standard? */
    #define create(X,b,c)       _Generic ((X),                      \
				        dque_qheadpp: dque_create   \
				        ) (X,b,c)
    #define destroy(X)          _Generic ((X),                      \
				        dque_qheadpp: dque_destroy  \
				        ) (X)
    #define error(X,b,c)        _Generic ((X),                      \
				        dque_err: dque_error        \
				        ) (X,b,c)
    #define error_len(X,b)      _Generic ((X),                      \
				        dque_err: dque_error_len    \
				        ) (X,b)
    #define options(X,b, ...)   _Generic ((X),                      \
				        dque_qheadp: dque_options   \
				        ) (X,b, __VA_ARGS__)
    #define empty(X,b)          _Generic ((X),                      \
				        dque_qheadp: dque_empty     \
				        ) (X,b)
    #define size(X,b)           _Generic ((X),                      \
				        dque_qheadp: dque_size      \
				        ) (X,b)
    #define max_size(X,b)       _Generic ((X),                      \
				        dque_qheadp: dque_max_size  \
				        ) (X,b)
    #define back(X,b)           _Generic ((X),                      \
				        dque_qheadp: dque_back      \
				        ) (X,b)
    #define front(X,b)          _Generic ((X),                      \
				        dque_qheadp: dque_front     \
				        ) (X,b)
    #define key_find(X,b,c,d)   _Generic ((X),                      \
				        dque_qheadp: dque_key_find  \
				        ) (X,b,c,d)
    #define get_data(X,b)       _Generic ((X),                      \
				        dque_qiterp: dque_get_data  \
				        ) (X,b)
    #define set_data(X,b)       _Generic ((X),                      \
				        dque_qiterp: dque_set_data  \
				        ) (X,b)
    #define begin(X,b)          _Generic ((X),                      \
				        dque_qheadp: dque_begin     \
				        ) (X,b)
    #define rbegin(X,b)         _Generic ((X),                      \
				        dque_qheadp: dque_rbegin    \
				        ) (X,b)
    #define next(X,b)           _Generic ((X),                      \
				        dque_qheadp: dque_next      \
				        ) (X,b)
    #define rnext(X,b)          _Generic ((X),                      \
				        dque_qheadp: dque_rnext     \
				        ) (X,b)
    #define end(X,b)            _Generic ((X),                      \
			 	        dque_qheadp: dque_end       \
				        ) (X,b)
    #define rend(X,b)           _Generic ((X),                      \
				        dque_qheadp: dque_rend      \
				        ) (X,b)
    #define erase(X,b,c)        _Generic ((X),                      \
				        dque_qheadp: dque_erase     \
				        ) (X,b,c)
    #define insert(X,b,c)       _Generic ((X),                      \
				        dque_qheadp: dque_insert    \
				        ) (X,b,c)
    #define key_erase(X,b,c,d)  _Generic ((X),                      \
				        dque_qheadp: dque_key_erase \
				        ) (X,b,c,d)
    #define key_insert(X,b,c,d) _Generic ((X),                      \
				        dque_qheadp: dque_key_insert\
				        ) (X,b,c,d)
    #define pop_back(X,b)       _Generic ((X),                      \
				        dque_qheadp: dque_pop_back  \
				        ) (X,b)
    #define pop_front(X,b)      _Generic ((X),                      \
				        dque_qheadp: dque_pop_front \
				        ) (X,b)
    #define push_back(X,b)      _Generic ((X),                      \
				        dque_qheadp: dque_push_back \
				        ) (X,b)
    #define push_front(X,b)     _Generic ((X),                      \
				        dque_qheadp: dque_push_front\
				        ) (X,b)
    #ifndef __cplusplus
      #define delete(X,b,c)       _Generic ((X),                      \
	  			          dque_qheadp: dque_erase     \
				          ) (X,b,c)
      #define key_delete(X,b,c,d) _Generic ((X),                      \
				          dque_qheadp: dque_erase     \
				          ) (X,b,c,d)
    #endif

#else	/* compiler does not conform to C11 standard */

    #ifndef DQUE_LIB
    #define DQUE_LIB            1
    #endif

    #ifndef INSERT_DEF
    #define INSERT_DEF          DQUE_LIB
    #endif

    #if INSERT_DEF == DQUE_LIB
      /* member functions */
      #define create(a,b,c)       dque_create(a,b,c)
      #define destroy(a)          dque_destroy(a)
      #define error(a,b,c)        dque_error(a,b,c)
      #define error_len(a,b)      dque_error_len(a,b)
      #if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)) /* conforms to C99 standard? */
        #define options(a,b, ...)   dque_options(a,b, __VA_ARGS__)
      #else
        #define options           dque_options
      #endif
      /* capacity */
      #define empty(a,b)          dque_empty(a,b)
      #define size(a,b)           dque_size(a,b)
      #define max_size(a,b)       dque_max_size(a,b)
      /* element access */
      #define back(a,b)           dque_back(a,b)
      #define front(a,b)          dque_front(a,b)
      #define key_find(a,b,c,d)   dque_key_find(a,b,c,d)
      #define get_data(a,b)       dque_get_data(a,b)
      #define set_data(a,b)       dque_set_data(a,b)
      /* iterators */
      #define begin(a,b)          dque_begin(a,b)
      #define rbegin(a,b)         dque_rbegin(a,b)
      #define next(a,b)           dque_next(a,b)
      #define rnext(a,b)          dque_rnext(a,b)
      #define end(a,b)            dque_end(a,b)
      #define rend(a,b)           dque_rend(a,b)
      /* Modifiers */
      #define erase(a,b,c)        dque_erase(a,b,c)
      #define insert(a,b,c)       dque_insert(a,b,c)
      #define key_erase(a,b,c,d)  dque_key_erase(a,b,c,d)
      #define key_insert(a,b,c,d) dque_key_insert(a,b,c,d)
      #define pop_back(a,b)       dque_pop_back(a,b)
      #define pop_front(a,b)      dque_pop_front(a,b)
      #define push_back(a,b)      dque_push_back(a,b)
      #define push_front(a,b)     dque_push_front(a,b)
      #ifndef __cplusplus
        #define delete(a,b,c)       dque_erase(a,b,c)
        #define key_delete(a,b,c,d) dque_key_erase(a,b,c,d)
      #endif                                /* __cplusplus                  */
    #endif                                  /* INSERT_DEF == DQUE_LIB       */
#endif                                      /* ifndef __STDC_VERSION__      */

#ifdef __cplusplus
}
#endif

#endif /* DQUE_H */
