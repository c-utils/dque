
/**
 *
 *  \file	dque_empty.c
 *  \name	dque_empty
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Return non-zero if empty, else zero.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_empty(
 *      dque_qhead             *queue,         queue to check for nodes
 *      int                    *data )         pointer to returned data
 *
 *      Return error status and non-zero value if the queue is empty. The
 *      queue is empty if the head pointer of the queue head is NULL.
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
dque_empty(                                 /* return non-zero if empty     */
dque_qhead     *queue,                      /* queue to check for nodes     */
unsigned int   *data )                      /* pointer to returned data     */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (void *)NULL) {      /* invalid data pointer?        */
        errcode = DQUEERR_NODATA;
    } else {                                /* return non-zero if empty     */
	*data   = (unsigned int)(gethead(queue) == NULL_NODE ? 1 : 0);
    }

    return (errcode);
}

