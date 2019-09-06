
/**
 *
 *  \file	dque_rbegin.c
 *  \name	dque_rbegin
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Return iterator to the back of the queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_rbegin(
 *              dque_qhead      *queue,      queue with element data
 *              dque_qiter     **iter )      pointer to a pointer to an iterator
 *
 *      Return a "reverse" iterator to the back of the queue. An iterator
 *      for the queue is just a pointer to any node in the queue. The macro
 *      gethead(queue) returns a pointer to the first node in the queue.
 *      The macro getprev(gethead(queue)) returns a pointer to the last node
 *      in the queue. Since this is what is needed, it is returned as the
 *      iterator.
 *
 *      \return non-zero for failure, zero for success and a iterator to first qnode of the queue.
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
dque_rbegin(                                /* return iterator into queue   */
dque_qhead     *queue,                      /* queue to have iterator       */
dque_qiter    **iter )                      /* pointer to returned iterator */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (iter == NULL_QITERP) {       /* invalid iterator pointer?    */
        errcode = DQUEERR_NOITERP;
    } else if (gethead(queue) == NULL_NODE){/* empty queue?                 */
        *iter   = NULL_QITER;               /* equal to dque_rend()         */
    } else {
        *iter   = getprev(gethead( queue ));/* pointer to last node in queue*/
    }

    return (errcode);
}

