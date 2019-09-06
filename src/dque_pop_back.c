
/**
 *
 *  \file	dque_pop_back.c
 *  \name	dque_pop_back
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Remove the last qnode of a queue, and return pointer to user's data.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_pop_back(
 *      dque_qhead             *queue,         queue with element data
 *      void                  **data )         pointer to pointer to returned data
 *
 *	If he queue is empty, this function will set the data pointer to (void *)NULL.
 *	If the queue is not empty, this function removes the last qnode of the queue,
 *	puts it onto the free list, and then returns the pointer to the data from 
 *	the removed qnode. This function also sets the data pointer of the removed
 *	node to (void *)NULL to indicate the node is now invalid to any iterators.
 *
 *      \return non-zero for failure, zero for success and pointer to user's data
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
dque_pop_back(                              /* remove last qnode from queue */
dque_qhead     *queue,                      /* queue to have node deleted   */
void          **data )                      /* returned data pointer        */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *node;                   /* pointer to deleted node      */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (void **)NULL) {     /* invalid data pointer?        */
        errcode = DQUEERR_NODATAP;
    } else if (gethead(queue) == NULL_NODE){/* empty queue?                 */
        *data    = (void *)NULL;            /* technically OK, but no data  */
    } else if ((errcode = dque_mydelete( &gethead( queue ), &node, ROTATE, &gethcnt( queue ) )) != DQUEERR_NOERR) {
        ;                                   /* ROTATE to delete from back   */
    } else if ((errcode = dque_myinsert( &getfree( queue ), node, NOROTATE, &getfcnt( queue ) )) == DQUEERR_NOERR) {
        *data   = getdata( node );
	setdata( node, (void *)NULL );      /* set to null for safety       */
    }

    return (errcode);
}

