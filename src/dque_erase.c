
/**
 *
 *  \file	dque_erase.c
 *  \name	dque_erase
 *  \author	Dale Anderson
 *  \date	07/30/2019
 *  \brief	Remove qnode at specified iterator of a queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *  1.1		07/30/2019  	D.Anderson  Renamed from "remove" to "erase" to conform to C++ terminology
 *
 *      dque_erase(
 *      dque_qhead             *queue,         queue to have node deleted
 *      void                  **data,          data from deleted node
 *      dque_qiter             *iter )         iterator of specified node
 *
 *      Remove qnode from the specified queue at the specified iterator.
 *      A 'queue' is simply a pointer to a qnode in the circular
 *      doubly-linked list with the pointed at node considered the head of
 *      the queue. If the place to remove the node is not at the front of
 *      the queue, then use the iterator as a temporary queue head to
 *      remove the node at the front of the temporary 'queue'. Also, if the
 *      iterator is not pointing to the first node in the queue, and if the
 *      DQUE_NOSCAN flag is not set, then the queue is searched to insure
 *      that the iterator is pointing to a node in the specified queue.
 *
 *      \return non-zero for failure, zero for success and pointer to data from removed qnode
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
#include "mydque.h"
#include "dque.h"

dque_err                                    /* returned status              */
dque_erase(                                 /* remove node from queue       */
dque_qhead     *queue,                      /* queue to have node deleted   */
void          **data,                       /* returned data pointer        */
dque_qiter     *iter )                      /* iterator to specified node   */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *headp;                  /* first qnode in queue     */
    dque_qnode     *node;                   /* pointer to deleted node      */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (void **)NULL) {     /* invalid data pointer         */
        errcode = DQUEERR_NODATAP;
    } else if (iter == NULL_QITER) {        /* invalid iterator?            */
        errcode = DQUEERR_NOITER;
    } else if ((headp = gethead(queue)) == NULL_NODE) {     /* empty queue? */
        *data    = (void *)NULL;            /* technically OK, but no data  */
        return (DQUEERR_NOERR);
    } else if (headp == iter) {             /* specified node is first node */
        errcode = dque_mydelete( &gethead( queue ), &node, NOROTATE, &gethcnt( queue ) );
    } else if (getflag(queue,DQUE_NOSCAN) || (errcode = dque_mysearch( &gethead( queue ), iter )) == DQUEERR_NOERR) {
        errcode = dque_mydelete( &iter, &node, NOROTATE, &gethcnt(queue) ); /* node in queue? */
    } else if (errcode == DQUEERR_NOTFOUND) {   /* iter specified node not found? */
        *data    = (void *)NULL;            /* technically OK, but no data  */
        return (DQUEERR_NOERR);
    }

    if (errcode != DQUEERR_NOERR) {         /* error? do not free node      */
        ;
    } else if ((errcode = dque_myinsert( &getfree( queue ), node, NOROTATE, &getfcnt( queue ) )) == DQUEERR_NOERR) {
        *data   = getdata( node );
	setdata( node, (void *)NULL );      /* set to null for safety       */
    }

    return (errcode);
}

