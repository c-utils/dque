
/**
 *
 *  \file	dque_mydelete.c
 *  \name	dque_mydelete
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Internal delete qnode from the front of a queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_mydelete(
 *      qnodep                 *nodep,         pointer to queue head pointer (may not be the actual qhead node)
 *      qnode                 **node,          pointer to pointer to deleted qnode
 *      int                     rotate,        rotate queue before deleting to delete last qnode?
 *      unsigned int           *count )        pointer to counter to decrement if deletion successful
 *
 *      Internal function, and only function in the this library, to delete first qnode from the specified queue by
 *      manipulating each qnode's pointers. A 'queue' is simply a pointer to a node in the circular doubly-linked
 *      list with the pointed at node considered the head of the queue. This definition allows this function to be
 *      used in all situations where a qnode needs to be deleted from the queue.
 *
 *      This function deletes the first node pointed at by the head pointer ('queue') and then updates the head
 *      pointer to point at the old second node in the list, making it the new first node in the list. If the
 *      boolean variable 'rotate' is true, it means to delete the last node of the list instead of the first node.
 *      Since this is a circular doubly-linked list, we can 'rotate' the head pointer to point to the last node
 *      in the list, pretend that it is the first node in the list, and delete as normal. After the deletion, when
 *      the head pointer is incremented to point to the second node in the list, it will actually point once again
 *      to the old first node, which is what we wanted. If the deletion is successful, the node counter, pointed at
 *      by the 'count' pointer, is decremented.
 *
 *      Because this is an internal function, there is no error checking of the function's arguments.
 *
 *      \return non-zero for failure, zero for success and deleted qnode
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
 */

#include <stdio.h>                          /* need NULL definition         */
#include "mydque.h"
#include "dque.h"

dque_err                                    /* returned completion status   */
dque_mydelete(                              /* delete first node from queue */
dque_qnodep    *nodep,                      /* queue to have node deleted   */
dque_qnode    **node,                       /* ptr to deleted node          */
int             rotate,                     /* rotate 2 delete end node?    */
unsigned int   *count )                     /* queue counter to decrement   */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *headp   = *nodep;       /* ptr to first qnode in queue  */

    if (rotate) {                           /* delete the last node instead?*/
        *nodep  = getprev( headp );         /* queue = headp->prev          */
        headp   = *nodep;                   /* get new first node in queue  */
    }

    if (headp == getnext( headp )) {        /* queue has only one qnode?    */
        *node   = headp;                    /* set returned node            */
        *nodep  = NULL_NODE;                /* queue now a NULL queue       */
    } else {                                /* else get qnode               */
        *node   = headp;                    /* set returned node            */
        *nodep  = getnext(headp);           /* queue = headp->next;         */
        setprev( getnext(headp), getprev(headp) );  /* headp->next->prev = headp->prev; */
        setnext( getprev(headp), getnext(headp) );  /* headp->prev->next = headp->next; */
    }

    if (count != (unsigned int *)NULL) {    /* have counter to decrement?   */
        dec_uns(*count);                    /* safe decrement               */
    }

    return (errcode);

}

