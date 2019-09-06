
/**
 *
 *  \file	dque_insert.c
 *  \name	dque_insert
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Insert new qnode with user's data just before the specified qnode.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_insert(
 *      dque_qhead             *queue,         queue to insert node into
 *      void                   *data,          data to insert into queue
 *      dque_qiter             *iter )         iterator of specified node
 *
 *      Insert a new node with the specified data into the queue just
 *      before the node specified by the iterator. A 'queue' is simply
 *      a pointer to a node in the circular doubly-linked list with the
 *      pointed at node considered the head of the queue. If the iter
 *      is a NULL pointer, then it is assumed that the new node should
 *      be appended to the end of the queue. If the place to insert the
 *      node is not at the front or back of the queue, then use the
 *      iterator as a temporary queue to insert the node at the 'back' of the
 *      temporary 'queue' which is also just before the where the iterator
 *      is pointing. Also, if the iterator is not pointing to the first
 *      node in the queue, and if the DQUE_NOSCAN flag is not set, then
 *      the queue is searched to insure that the iterator is pointing to
 *      a node in the specified queue.
 *
 *      \return non-zero for failure, zero for success
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

dque_err                                    /* returned completion status   */
dque_insert(                                /* insert node in a queue       */
dque_qhead     *queue,                      /* queue to have node inserted  */
void           *data,                       /* data to insert with new node */
dque_qiter     *iter )                      /* iterator to specified node   */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *node;

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (void *)NULL) {      /* invalid data pointer         */
        errcode = DQUEERR_NODATA;
    } else if (getfree( queue ) == NULL_NODE && ((errcode = dque_myalloc( queue )) != DQUEERR_NOERR)) {
        ;                   /* no free nodes? try to create more free nodes */
    } else if ((errcode = dque_mydelete( &getfree( queue ), &node, NOROTATE, &getfcnt( queue ) )) == DQUEERR_NOERR) {
        setdata( node, data );              /* set the data                 */
    }

    if (errcode != DQUEERR_NOERR) {         /* error? do not insert node    */
        ;
    } else if (iter == NULL_QITER) {        /* insert at the back of the queue? */
        errcode = dque_myinsert( &gethead(queue), node, NOROTATE, &gethcnt( queue ) ); 
    } else if (gethead(queue) == iter) {    /* first node is specified node?*/
        errcode = dque_myinsert( &gethead(queue), node, ROTATE, &gethcnt( queue ) );  /* insert at head of queue  */
    } else if (getflag(queue,DQUE_NOSCAN) || (errcode = dque_mysearch( &gethead(queue), iter )) == DQUEERR_NOERR) {
        errcode = dque_myinsert( &iter, node, NOROTATE, &gethcnt( queue ) ); /* insert at iterator used as headp */
    }

    return (errcode);
}

