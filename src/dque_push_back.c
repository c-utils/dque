
/**
 *
 *  \file	dque_push_back.c
 *  \name	dque_push_back
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Insert new qnode with the specified user's data to the back of the queue.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_push_back(
 *      dque_qhead             *queue,         queue with element data
 *      void                   *data )         data to insert for new qnode
 *
 * 	This function checks the free list, and if empty, calls as dque_myalloc() to allocate
 * 	more qnodes. The function then removes a qnode from the free list, sets the data
 * 	pointer to the user's data, and then inserts the qnode at the end (back) of the queue.
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
dque_push_back(                             /* insert node onto back of queue*/
dque_qhead     *queue,                      /* queue to have node inserted  */
void           *data )                      /* data to insert into queue    */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *node;                   /* pointer to inserted node     */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (data == (void *)NULL) {      /* invalid data pointer?        */
        errcode = DQUEERR_NODATA;
    } else if (getfree( queue ) == NULL_NODE && ((errcode = dque_myalloc( queue )) != DQUEERR_NOERR)) {
        ;                   /* no free nodes? try to create more free nodes */
    } else if ((errcode = dque_mydelete( &getfree( queue ), &node, NOROTATE, &getfcnt( queue ) )) == DQUEERR_NOERR) {
        setdata( node, data );              /* got node, set the data       */
    }

    if (errcode == DQUEERR_NOERR) {         /* no error? insert node        */
        errcode = dque_myinsert( &gethead( queue ), node, NOROTATE, &gethcnt( queue ) );
    }                                       /* NOROTATE to insert at back   */

    return (errcode);
}

