
/**
 *
 *  \file	dque_key_insert.c
 *  \name	dque_key_insert
 *  \author	Dale Anderson
 *  \date	07/30/2019
 *  \brief	Insert new qnode with user's data using specified key to find insertion point.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.1		07/30/2019  	D.Anderson  original
 *
 *      dque_key_insert(
 *      dque_qhead             *queue,         queue to scan for insertion
 *      void                   *key,           key to use for insertion point
 *      COMPFUNC                comp,          user defined comparison function
 *      void                   *data )         data to insert into queue
 *
 *      Search the queue for insertion point using specified key. Insertion
 *      point is defined to be just before the node with a key that is
 *      larger than specified key. This means new qnodes will be placed
 *      towards the back of the queue behind any other qnodes with the same
 *      key value. This forms a FIFO queue for qnodes of the same value.
 *
 *      The dque_myscan function will scan the entire queue and return the
 *      correct insertion point. However, it will return a pointer to
 *      the first node in the queue if the new node is to be inserted at
 *      the front or the back of the queue, that is, the node has the
 *      smallest or the largest value in the queue. This would require two
 *      checks on return, one to see if the insertion point matches the
 *      head of the queue, and one to see if the node should be the first
 *      or the last node. This function chooses to make these checks before
 *      calling dque_scan by checking to see if node to insert is to be
 *      inserted at the front or the back of the queue. These checks will
 *      will not save much execution time if the node is to be inserted at
 *      the front of the queue, but does save some time if the node is to
 *      inserted at the back of the queue since the scan of the queue is
 *      not required. This also saves time because the returned insertion
 *      point from dque_myscan is guaranteed to be within the queue and no
 *      further checks are required. The downside of this approach is that
 *      if dque_myscan is called, one, or both, of these checks are repeated.
 *      Please note that both of these checks must be made using the user
 *      specified comparison function.
 *
 *      The comparison function takes two arguments and the first one MUST
 *      be the specified key arument and the data pointer stored in each
 *      node as the second argument. The user supplied comparison function
 *      should return an integer follows:
 *
 *                  < 0 if key < data->key
 *                    0 if key = data->key
 *                  > 0 if key > data->key
 *
 *      Under usual circumstances, the key and the data to insert will be
 *      the same, so as a convenience, if the key is a null pointer, the
 *      pointer to the data will be used for the key pointer.
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
dque_key_insert(                            /* insert node at specified pnt */
dque_qhead     *queue,                      /* queue to have node inserted  */
void           *key,                        /* key to use 4 insertion point */
COMPFUNC        comp,                       /* user comparison function     */
void           *data )                      /* data to insert               */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *headp;                  /* pointer to first qnode in queue */
    dque_qnode     *node;                   /* node to insert               */
    dque_qnode     *temp;                   /* node for insertion point     */
    int             val;

    if (key == (void *)NULL) {              /* no key? use specified node   */
        key     = data;
    }

    if (queue == NULL_QUEUE) {              /* invalid queue?               */
        errcode = DQUEERR_NOQUEUE;
    } else if (comp == (COMPFUNC)NULL) {    /* no comparison function?      */
        errcode = DQUEERR_NOCOMP;
    } else if (data == (void *)NULL) {      /* invalid data pointer?        */
        errcode = DQUEERR_NODATA;
    } else if (getfree( queue ) == NULL_NODE && ((errcode = dque_myalloc( queue )) != DQUEERR_NOERR)) {
        ;                   /* no free nodes? try to create more free nodes */
    } else if ((errcode = dque_mydelete( &getfree( queue ), &node, NOROTATE, &getfcnt( queue ) )) == DQUEERR_NOERR) {
        setdata( node, data );              /* set the data                 */
    }

    if (errcode != DQUEERR_NOERR) {         /* error? do not insert node    */
        ;
    } else if ( ((headp = gethead(queue)) == NULL_NODE) || ((*comp)( key, getdata(headp)) < 0) ) {     /* node goes in front?  */
        errcode = dque_myinsert( &gethead(queue), node, ROTATE, &gethcnt(queue) );
    } else if ((val = (*comp)( key, getdata(getprev(headp)) )) >= 0) {              /* node goes in back?   */
        if (val == 0 && getflag(queue, DQUE_NODUPE)) {                      /* duplicate node not allowed?  */
            errcode = DQUEERR_NODUPE;
        } else {
            errcode = dque_myinsert( &gethead(queue), node, NOROTATE, &gethcnt(queue) );
        }
    } else if ((errcode = dque_myscan( &gethead(queue), key, comp, &temp )) == DQUEERR_NOERR) {	/* node goes in the middle */
        if (getflag(queue, DQUE_NODUPE) && !(*comp)( key, getdata(getprev(temp)) ) ) { /* dupe not allowed? */
            errcode = DQUEERR_NODUPE;
        } else {
            errcode = dque_myinsert( &temp, node, NOROTATE, &gethcnt(queue) );      /* node goes in middle  */
        }
    }

    return (errcode);
}

