
/**
 *
 *  \file	dque_set_data.c
 *  \name	dque_set_data
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Set the data in the qnode specified by the iterator.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_set_data(
 *      dque_qiter             *iter;          pointer to iterator
 *      void                   *data;          pointer to data
 *
 *      Set the data in the qnode specified by the iterator. Note that if
 *      this is a priority queue, changing the data may violate the order
 *      of the queue. To avoid this, it is recommended that the user remove
 *      the qnode from the queue, change the data, then re-insert the qnode
 *      to insure that the qnode is in the proper order.
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
dque_set_data(                              /* set iterator's node data     */
dque_qiter     *iter,                       /* pointer to iterator          */
void           *data )                      /* pointer to data              */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */

    if (iter == NULL_QITER) {               /* invalid iterator pointer?    */
        errcode = DQUEERR_NOITER;
    } else if (data == (void *)NULL) {      /* invalid data pointer?        */
        errcode = DQUEERR_NODATA;
    } else {                                /* set the user's data          */
        setdata( iter, data );
    }

    return (errcode);
}

