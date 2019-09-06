
/**
 *
 *  \file	dque_rnext.c
 *  \name	dque_rnext
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Return the next iterator rearward (towards the front) in a queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_rnext(
 *              dque_qhead     *queue,      queue with element data
 *              dque_qiter    **iter )      pointer to pointer to iterator
 *
 *      Return the iterator to the next qnode rearward in the queue. An iterator
 *      for the queue is just a pointer to any qnode in the queue. The macro
 *      getprev(iter) returns a pointer to the previous node in the queue. The
 *      macro gethead(queue) returns a pointer to the first node in the
 *      queue. If the current iterator is equal to the first node, we have
 *      completed the search. Return a NULL pointer to stop the iteration.
 *
 *      \return non-zero for failure, zero for success and iterator to the previous node rearward in the queue.
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
dque_rnext(                                 /* return prev iterator in queue*/
dque_qhead     *queue,                      /* queue to have iterator       */
dque_qiter    **iterp )                     /* pointer to returned iterator */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qiter     *iter;                   /* iterator ptr to node in queue*/

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (iterp == NULL_QITERP) {      /* invalid iterator pointer?    */
        errcode = DQUEERR_NOITERP;
    } else if ((iter = *iterp) == NULL_QITER) {        /* invalid iterator? */
        errcode = DQUEERR_NOITER;
    } else {
        if (getdata(iter) == (void *)NULL) {/* invalid iterator?            */
            errcode = DQUEERR_INVITER;
            iter    = NULL_QITER;
	} else if (iter == gethead( queue )) {  /* end of queue?            */
            iter    = NULL_QITER;           /* pointer to stop iteration    */
        } else {
            iter    = getprev(iter);        /* else next iteration          */
        }
        *iterp  = iter;                     /* save new iterator            */
    }

    return (errcode);
}

