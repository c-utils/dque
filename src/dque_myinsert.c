
/**
 *
 *  \file	dque_myinsert.c
 *  \name	dque_myinsert
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Internal insert qnode onto the back of the queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_myinsert(
 *      qnodep                 *nodep,         pointer to queue head pointer (may not be the actual qhead node)
 *      qnode                  *node,          pointer to qnode to insert
 *      int                     rotate;        rotate queue before deleting to delete last qnode?
 *      unsigned int           *count;         pointer to counter to increment if insertion successful
 *
 *      Internal function, and only function in the this library, to insert qnode to the back of the specified
 *      queue by manipulating each qnode's pointers. A 'queue' is simply a pointer to a qnode in the circular
 *      doubly-linked list and that qnode is considered the head of the queue. This definition allows this
 *      function to be used in all situations where a qnode needs to be inserted into the queue.
 *
 *      This function inserts the qnode pointed to by the 'node' pointer onto the end of the list. If the  boolean
 *      variable 'rotate' is true, it means to insert the qnode at the front of the list instead of the back.
 *      Since this is a circular doubly-linked list, we can insert the qnode onto the back of the list as normal,
 *      then 'rotate' the head pointer to point to the last qnode in the list, making it the new first node.
 *      If the insertion is successful, the node counter, pointed at by the 'count' pointer, is incremented.
 *
 *      Because this is an internal function, there is no error checking of the function's arguments.
 *
 *      \return non-zero for failure, zero for success
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
dque_myinsert(                              /* internal node insert         */
dque_qnodep    *nodep,                      /* queue to have node inserted  */
dque_qnode     *node,                       /* qnode to insert into queue   */
int             rotate,                     /* rotate inserted node 2 front?*/
unsigned int   *count )                     /* queue counter to increment   */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *headp   = *nodep;       /* ptr to first qnode in queue  */

    if (headp == NULL_NODE) {               /* empty queue?                 */
	headp   = node;                     /* needed if we rotate          */
        setprev( node,  node );             /* node->prev  = node;          */
        setnext( node,  node );             /* node->next  = node;          */
        *nodep  = node;                     /* *nodep      = node;          */
    } else {                                /* queue not empty, insert node */
        setnext( node, headp );             /* node->next  = headp;         */
	setprev( node, getprev(headp) );    /* node->prev  = headp->prev;   */
	setnext( getprev(headp), node );    /* headp->prev->next   = node;  */
	setprev( headp, node          );    /* headp->prev = node;          */
    }

    if (rotate) {                           /* insert at front of queue?    */
        *nodep = getprev( headp );          /* *nodep = headp->prev         */
    }

    if (count != (unsigned int *)NULL) {    /* have counter to increment?   */
        inc_uns(*count);                    /* increment if all went well   */
    }

    return (errcode);
}

