
/**
 *
 *  \file	dque_destroy.c
 *  \name	dque_destroy
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Destroy a circular doubly-linked list.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_destroy(
 *      dque_qheadp            *queuep )       pointer to pointer to queue head
 *
 *      Destroy a queue. If we dynamically allocated memory, then we must
 *      free the allocated memory. The pointer to each node on the blks
 *      list is the same pointer to the allocated memory block that we
 *      can use to free the memory. This function frees the memory in the
 *      opposite order of their allocation. The function then frees the
 *      qhead and sets the user's qhead pointer to NULL.
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
#include <stdlib.h>                         /* need for malloc and free     */
#include "mydque.h"
#include "dque.h"

dque_err                                    /* returned completion status   */
dque_destroy(                               /* destroy a queue              */
dque_qheadp    *queuep )                    /* ptr to ptr to a queue        */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qhead     *queue;
    dque_qnode     *node;

    if (queuep == NULL_QUEUEP) {            /* no pointer to queue head?    */
        errcode = DQUEERR_NOQUEUEP;
    } else if ((queue = *queuep) == NULL_QUEUE) {   /* no pointer to queue? */
        errcode = DQUEERR_NOQUEUE;
    } else if (getflag(queue, DQUE_NOALLOC) != 0) { /* no dynamic allocation? */
        ;                                           /* yes, so nothing to free*/
    } else {                                /* no, free the allocated memory  */
        while (getblks(queue) != NULL_NODE && errcode == DQUEERR_NOERR) {
            /* while block list is not empty, remove last memory block and free it  */
            if ((errcode = dque_mydelete( &getblks(queue), &node, ROTATE, &getbcnt(queue) )) == DQUEERR_NOERR) {
                free( node );
            }
        }

        if (errcode == DQUEERR_NOERR) {
            free( queue );
            *queuep = NULL_QUEUE;
        }
    }

    return (errcode);
}

