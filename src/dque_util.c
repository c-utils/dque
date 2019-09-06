/**
 *
 *  \file	dque_util.c
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

#include <stdio.h>                          /* need NULL definition         */
#include <stdlib.h>                         /* need for malloc              */
#include "dque.h"
#include "dque_util.h"

dque_qiter *
dque_my_begin( dque_qhead *queue ) {
    dque_qiter     *iter    = NULL_QITER;

    if ((errcode = dque_begin( queue, &iter )) != DQUEERR_NOERR) {
        if ((errcode = dque_end( queue, &iter )) != DQUEERR_NOERR) {
            iter = NULL_QITER;
        }
    }

    return (iter);
}

dque_qiter *
dque_my_next( dque_qhead *queue, dque_qiter **iterp ) {

    if ((errcode = dque_next( queue, iterp )) != DQUEERR_NOERR) {
        if ((errcode = dque_end( queue, iterp )) != DQUEERR_NOERR) {
            *iterp = (dque_qiter *)NULL;
        }
    }

    return (*iterp);
}

dque_qiter *
dque_my_end( dque_qhead *queue ) {
    dque_qiter     *iter    = NULL_QITER;

    if ((errcode = dque_end( queue, &iter )) != DQUEERR_NOERR) {
        iter = NULL_QITER;
    }

    return (iter);
}

dque_qiter *
dque_my_rbegin( dque_qhead *queue ) {
    dque_qiter     *iter    = NULL_QITER;

    if ((errcode = dque_rbegin( queue, &iter )) != DQUEERR_NOERR) {
        if ((errcode = dque_rend( queue, &iter )) != DQUEERR_NOERR) {
            iter = NULL_QITER;
        }
    }

    return (iter);
}

dque_qiter *
dque_my_rnext( dque_qhead *queue, dque_qiter **iterp ) {

    if ((errcode = dque_rnext( queue, iterp )) != DQUEERR_NOERR) {
        if ((errcode = dque_rend( queue, iterp )) != DQUEERR_NOERR) {
            *iterp = (dque_qiter *)NULL;
        }
    }

    return (*iterp);
}

dque_qiter *
dque_my_rend( dque_qhead *queue ) {
    dque_qiter     *iter    = NULL_QITER;

    if ((errcode = rend( queue, &iter )) != DQUEERR_NOERR) {
        iter = NULL_QITER;
    }

    return (iter);
}

void *
dque_my_data( dque_qiter *iter ) {
    void   *data    = (void *)NULL;

    if ((errcode = dque_get_data( iter, &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}


void *
dque_stk_pop( dque_qhead *queue ) {
    void      *data;

    if ((errcode = dque_pop_front( queue, &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

void *
dque_stk_top( dque_qhead *queue ) {
    void      *data;

    if ((errcode = dque_front( queue, &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

int
dque_stk_empty( dque_qhead *queue ) {
    unsigned int val;

    if ((errcode = dque_empty( queue, &val )) != DQUEERR_NOERR) {
        val     = -1;
    }

    return (val);
}

int
dque_stk_size( dque_qhead *queue ) {
    unsigned int val;

    if ((errcode = dque_size( queue, &val )) != DQUEERR_NOERR) {
        val     = -1;
    }

    return (val);
}

void *
dque_que_pop( dque_qhead *queue ) {
    void      *data;

    if ((errcode = dque_pop_front( queue, &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

void *
dque_que_back( dque_qhead *queue ) {
    void      *data;

    if ((errcode = dque_back( queue, &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

void *
dque_que_front( dque_qhead *queue ) {
    void      *data;

    if ((errcode = dque_front( queue, &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

int
dque_que_empty( dque_qhead *queue ) {
    unsigned int val;

    if ((errcode = dque_empty( queue, &val )) != DQUEERR_NOERR) {
        val     = -1;
    }

    return (val);
}

int
dque_que_size( dque_qhead *queue ) {
    unsigned int val;

    if ((errcode = dque_size( queue, &val )) != DQUEERR_NOERR) {
        val     = -1;
    }

    return (val);
}

dque_err
dque_pri_create( dque_pri_qheadp *pri_qheadp, COMPFUNC comp ) {
    dque_pri_qhead    *pri;
    dque_qhead        *queue;

    if (pri_qheadp == (dque_pri_qheadp *)NULL) {
        errcode = DQUEERR_NOQUEUEP;                     /* no new error codes for now   */
    } else if (comp == (COMPFUNC)NULL) {
        errcode = DQUEERR_NOCOMP;
    } else if ((errcode = dque_create( &queue, 0, (char *)NULL )) != DQUEERR_NOERR) { /* no static memory allocations */
        ;
    } else if ((pri = (dque_pri_qhead *)malloc( sizeof(dque_pri_qhead) )) == (dque_pri_qhead *)NULL) {
        (void) dque_destroy( &queue );                  /* attempt to clean up the mess */
        errcode = DQUEERR_NOALLOC;
    } else {
        setqueue( pri, queue );
        setcomp(  pri, comp  );
       *pri_qheadp = pri;
        errcode    = DQUEERR_NOERR;
    }

    return (errcode);
}

dque_err
dque_pri_destroy( dque_pri_qheadp *pri_qheadp ) {
    dque_pri_qhead    *pri;

    if (pri_qheadp == (dque_pri_qheadp *)NULL) {
        errcode = DQUEERR_NOQUEUEP;                     /* no new error codes for now   */
    } else {
        pri = *pri_qheadp;
	errcode = dque_destroy( &(getqueue(pri)) );
        free( pri );                                    /* succeed or fail, we do the same */
        *pri_qheadp = (dque_pri_qhead *)NULL;
    }

    return (errcode);
}

void *
dque_pri_pop( dque_pri_qhead *pri ) {
    void      *data;

    if ((errcode = dque_pop_front( getqueue(pri), &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

void *
dque_pri_top( dque_pri_qhead *pri ) {
    void      *data;

    if ((errcode = dque_front( getqueue(pri), &data )) != DQUEERR_NOERR) {
        data    = (void *)NULL;
    }

    return (data);
}

int
dque_pri_empty( dque_pri_qhead *pri ) {
    unsigned int val;

    if ((errcode = dque_empty( getqueue(pri), &val )) != DQUEERR_NOERR) {
        val     = -1;
    }

    return (val);
}

int
dque_pri_size( dque_pri_qhead *pri ) {
    unsigned int val;

    if ((errcode = dque_size( getqueue(pri), &val )) != DQUEERR_NOERR) {
        val     = -1;
    }

    return (val);
}


