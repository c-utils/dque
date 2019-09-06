
/**
 *
 *  \file	dque_key_erase.c
 *  \name	dque_key_erase
 *  \author	Dale Anderson
 *  \date	07/30/2019
 *  \brief	remove qnode specified by the keyand return pointer to the user's data.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.1		07/30/2019  	D.Anderson  original
 *
 *      dque_key_erase(
 *      dque_qhead             *queue,         queue to scan for insertion
 *      void                   *key,           key to use for insertion point
 *      COMPFUNC                comp,          user defined comparison function
 *      void                  **data )         pointer to data from removed qnode
 *
 *      Search the queue using specified key for the node to delete. It is
 *      technically OK (by definition) if the queue is empty or if the qnode
 *      specified by the key is not in the queue. In either case, no error
 *      is returned and the the pointer to data is set to NULL.
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
 *      \return non-zero for failure, zero for success and pointer to the data from the deleted qnode.
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
dque_key_erase(                             /* remove node w/specified key  */
dque_qhead     *queue,                      /* queue to have node removed   */
void           *key,                        /* key to use to find node      */
COMPFUNC        comp,                       /* user comparison function     */
void          **data )                      /* data from deleted found node */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *headp;                  /* pointer to first qnode in queue */
    dque_qnode     *node;                   /* removed node                 */
    dque_qnode     *temp;                   /* node for temporary head      */

    if (queue == NULL_QUEUE) {              /* invalid queue?               */
        errcode = DQUEERR_NOQUEUE;
    } else if (key == (void *)NULL) {       /* no key?                      */
        errcode = DQUEERR_NOKEY;
    } else if (comp == (COMPFUNC)NULL) {    /* no comparison function?      */
        errcode = DQUEERR_NOCOMP;
    } else if (data == (void **)NULL) {     /* invalid data pointer?        */
        errcode = DQUEERR_NODATAP;
    } else if ((headp = gethead(queue)) == NULL_NODE) {     /* empty queue? */
        *data   = (void *)NULL;
	return (DQUEERR_NOERR);             /* OK, qnode is not in the queue*/
    } else if ((errcode = dque_myfind( &headp, key, comp, &temp )) != DQUEERR_NOERR) {
        *data   = (void *)NULL;
	return (DQUEERR_NOERR);             /* OK, qnode is not in the queue*/
    } else if (temp == headp) {             /* delete the first node?       */
        errcode = dque_mydelete( &gethead(queue), &node, NOROTATE, &gethcnt(queue) );
    } else {                                /* no, delete qnode in the queue*/
        errcode = dque_mydelete( &temp,           &node, NOROTATE, &gethcnt(queue) );
    }

    if (errcode != DQUEERR_NOERR) {         /* error? do not free node      */
        ;
    } else if ((errcode = dque_myinsert( &getfree( queue ), node, NOROTATE, &getfcnt( queue ) )) == DQUEERR_NOERR) {
        *data   = getdata( node );
        setdata( node, (void *)NULL );      /* set to null to show invalid  */
    }

    return (errcode);
}

