
/**
 *
 *  \file	dque_max_size.c
 *  \name	dque_max_size
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Return maximum number of nodes available in the queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_max_size(
 *      dque_qhead             *queue,         queue to count nodes
 *      unsigned int           *data )         pointer to returned data
 *
 *      Return number of nodes available to use in the queue. This includes
 *      qnodes currently in the queue and those on the free list, but not on the
 *      allocated blocks list (blks) since they cannot be used by the user.
 *
 *      \return non-zero for failure, zero for success and the maximum qnode count.
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
dque_max_size(                              /* return available # of qnodes */
dque_qhead     *queue,                      /* queue to check               */
unsigned int   *data )                      /* pointer to returned data     */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (unsigned *)NULL) {  /* invalid data pointer?        */
        errcode = DQUEERR_NODATA;
    } else {                                /* return head and free counts  */
        *data   = gethcnt(queue) + getfcnt(queue);
    }

    return (errcode);
}

