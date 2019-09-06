
/**
 *
 *  \file	dque_back.c
 *  \name	dque_back
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Return pointer to user's data from the last qnode of a queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_back(
 *              dque_qhead     *queue,      queue with element data
 *              dque_qiter    **data )      pointer to a pointer to the returned data
 *
 *      Return error status and pointer to element data. The macro
 *      gethead(queue) returns a pointer to the first node in the queue.
 *      Since this is a circular doubly linked list, the previous pointer
 *      in the first node points to the last node in the list. So the macro
 *      getprev(gethead(queue)) returns a pointer to the last node and
 *      the macro getdata(node) returns a pointer to the data that the
 *      user stored in this qnode element.
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
dque_back(                                  /* return last element data     */
dque_qhead     *queue,                      /* queue with element data      */
void          **data )                      /* ptr to ptr to returned data  */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *node;

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (void **)NULL) {     /* invalid data pointer?        */
        errcode = DQUEERR_NODATAP;
    } else if (gethead(queue) == NULL_NODE){/* empty queue?                 */
        *data    = (void *)NULL;            /* technically OK, but no data  */
    } else {
        node    = getprev( gethead(queue) );/* ptr to last node in queue    */
	*data   = getdata( node );
    }

    return (errcode);
}

