/**
 *
 *  \file	dque_util.h
 *  \name	dque_util
 *  \author	Dale Anderson
 *  \date	07/30/2019
 *  \brief	Return pointer to user's data from the last qnode of a queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.1		07/30/2019  	D.Anderson  original
 *
 *      These routines form wrappers around some of the dque functions.
 *
 * Copyright (c) 2019 Dale Anderson <daleanderson488@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the 'Software'), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef DQUE_UTIL_H
#define DQUE_UTIL_H

#include "dque.h"

typedef struct dque_pri_qhead
    {
    dque_qhead         *queue;              /* pointer to real queue        */
    COMPFUNC            comp;               /* user's comparison function   */
    } dque_pri_qhead, *dque_pri_qheadp, **dque_pri_qheadpp;

#define getqueue(h)     (h)->queue
#define setqueue(h,q)   getqueue(h)=(q)

#define getcomp(h)      (h)->comp
#define setcomp(h,c)    getcomp(h)=(c)

extern dque_err errcode;

extern dque_qiter *dque_my_begin(  dque_qhead *queue                     );
extern dque_qiter *dque_my_end(    dque_qhead *queue                     );
extern dque_qiter *dque_my_next(   dque_qhead *queue, dque_qiter **iterp );
extern dque_qiter *dque_my_rbegin( dque_qhead *queue                     );
extern dque_qiter *dque_my_rend(   dque_qhead *queue                     );
extern dque_qiter *dque_my_rnext(  dque_qhead *queue, dque_qiter **iterp );
extern void       *dque_my_data(   dque_qiter *iter                      );

extern void       *dque_stk_pop(   dque_qhead *queue                     );
extern void       *dque_stk_top(   dque_qhead *queue                     );
extern int         dque_stk_empty( dque_qhead *queue                     );
extern int         dque_stk_size(  dque_qhead *queue                     );

extern void       *dque_que_pop(   dque_qhead *queue                     );
extern void       *dque_que_back(  dque_qhead *queue                     );
extern void       *dque_que_front( dque_qhead *queue                     );
extern int         dque_que_empty( dque_qhead *queue                     );
extern int         dque_que_size(  dque_qhead *queue                     );

extern dque_err    dque_pri_create( dque_pri_qheadp *pri, COMPFUNC comp  );
extern dque_err    dque_pri_destroy(dque_pri_qheadp *pri                 );
extern dque_err    dque_pri_push(   dque_pri_qhead  *pri, void *data     );
extern void       *dque_pri_pop(    dque_pri_qhead  *pri                 );
extern void       *dque_pri_top(    dque_pri_qhead  *pri                 );
extern int         dque_pri_empty(  dque_pri_qhead  *pri                 );
extern int         dque_pri_size(   dque_pri_qhead  *pri                 );

#if (defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L))    /* conforms to C11 standard? */
    #define my_begin(X)         _Generic ((X),                       \
				        dque_qheadp: dque_my_begin   \
				        ) (X)
    #define my_next(X,b)        _Generic ((X),                       \
				        dque_qheadp: dque_my_next    \
				        ) (X,b)
    #define my_end(X)           _Generic ((X),                       \
				        dque_qheadp: dque_my_end     \
				        ) (X)
    #define my_rbegin(X)        _Generic ((X),                       \
				        dque_qheadp: dque_my_rbegin  \
				        ) (X)
    #define my_rnext(X,b)       _Generic ((X),                       \
				        dque_qheadp: dque_my_rnext   \
				        ) (X,b)
    #define my_rend(X)          _Generic ((X),                       \
				        dque_qheadp: dque_my_rend    \
				        ) (X)
    #define my_data(X)          _Generic ((X),                       \
				        dque_qiterp: dque_my_data    \
				        ) (X)

    #define stk_push(X,b)       _Generic ((X),                       \
				        dque_qheadp: dque_push_front \
				        ) (X,b)
    #define stk_pop(X)          _Generic ((X),                       \
				        dque_qheadp: dque_stk_pop    \
				        ) (X)
    #define stk_top(X)          _Generic ((X),                       \
				        dque_qheadp: dque_stk_top    \
				        ) (X)
    #define stk_empty(X)        _Generic ((X),                       \
				        dque_qheadp: dque_stk_empty  \
				        ) (X)
    #define stk_size(X)         _Generic ((X),                       \
				        dque_qheadp: dque_stk_size   \
				        ) (X)

    #define que_push(X,b)       _Generic ((X),                       \
				        dque_qheadp: dque_push_back  \
				        ) (X,b)
    #define que_pop(X)          _Generic ((X),                       \
				        dque_qheadp: dque_que_pop    \
				        ) (X)
    #define que_back(X)         _Generic ((X),                       \
				        dque_qheadp: dque_que_back   \
				        ) (X)
    #define que_front(X)        _Generic ((X),                       \
				        dque_qheadp: dque_que_front  \
				        ) (X)
    #define que_empty(X)        _Generic ((X),                       \
				        dque_qheadp: dque_que_empty  \
				        ) (X)
    #define que_size(X)         _Generic ((X),                       \
				        dque_qheadp: dque_que_size   \
				        ) (X)

    #define pri_create(X,b)     _Generic ((X),                            \
				        dque_pri_qheadpp: dque_pri_create \
				        ) (X,b)
    #define pri_destroy(X)      _Generic ((X),                            \
				        dque_pri_qheadpp: dque_pri_destroy\
				        ) (X)
    #define pri_push(X,b)       _Generic ((X),                            \
				        dque_pri_qheadp: dque_key_insert  \
				        ) (getqueue(X),(void*)NULL,getcomp(X),b)
    #define pri_pop(X)          _Generic ((X),                            \
				        dque_pri_qheadp: dque_pri_pop     \
				        ) (X)
    #define pri_top(X)          _Generic ((X),                            \
				        dque_pri_qheadp: dque_pri_top     \
				        ) (X)
    #define pri_empty(X)        _Generic ((X),                            \
				        dque_pri_qheadp: dque_pri_empty   \
				        ) (X)
    #define pri_size(X)         _Generic ((X),                            \
				        dque_pri_qheadp: dque_pri_size    \
				        ) (X)

#else	/* compiler does not conform to C11 standard */

    #ifndef DQUE_LIB
    #define DQUE_LIB            1
    #endif

    #ifndef INSERT_DEF
    #define INSERT_DEF          DQUE_LIB
    #endif

    #if INSERT_DEF == DQUE_LIB
      #define my_begin(a)       dque_my_begin(a)
      #define my_next(a,b)      dque_my_next(a,b)
      #define my_end(a)         dque_my_end(a)
      #define my_rbegin(a)      dque_my_rbegin(a)
      #define my_rnext(a,b)     dque_my_rnext(a,b)
      #define my_rend(a)        dque_my_rend(a)
      #define my_data(a)        dque_my_data(a)

      #define stk_push(a,b)     dque_push_front(a,b)
      #define stk_pop(a)        dque_stk_pop(a)
      #define stk_top(a)        dque_stk_top(a)
      #define stk_empty(a)      dque_stk_empty(a)
      #define stk_size(a)       dque_stk_size(a)

      #define que_push(a,b)     dque_push_back(a,b)
      #define que_pop(a)        dque_que_pop(a)
      #define que_back(a)       dque_que_back(a)
      #define que_front(a)      dque_que_front(a)
      #define que_empty(a)      dque_que_empty(a)
      #define que_size(a)       dque_que_size(a)

      #define pri_create(a,b)   dque_pri_create(a,b)
      #define pri_destroy(a)    dque_pri_destroy(a)
      #define pri_push(a,b)     dque_key_insert(getqueue(a),(void*)NULL,getcomp(a),b)
      #define pri_pop(a)        dque_pri_pop(a)
      #define pri_top(a)        dque_pri_top(a)
      #define pri_empty(a)      dque_pri_empty(a)
      #define pri_size(a)       dque_pri_size(a)
    #endif                                  /* INSERT_DEF == DQUE_LIB       */
#endif                                      /* ifndef __STDC_VERSION__      */

#endif /* DQUE_UTIL_H */
