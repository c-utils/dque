
/**
 *
 *  \file	dque_myscan.c
 *  \name	dque_myscan
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Internal search the queue for an insertion point based on the specified key.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.1		07/30/2019  	D.Anderson  original
 *
 *      dque_myscan(
 *      qnodep                 *nodep,         pointer to queue head pointer (may not be the actual qhead node)
 *      void                   *key,           pointer to camparison key. May or may not be same as 'data' in queue
 *      COMPFUNC                comp,          user supplied comparison function. Key must be first argument
 *      qnode                 **node,          if key node found, returned pointer to the qnode
 *
 *      Scan the specified queue for an insertion point based on the specified key using the specified comparison
 *      function. There is always an insertion point, but if the insert point is the front or back of the queue, this
 *      function will return the headp pointer and the caller must differentiate between the two cases.
 *      This function assumes the queue is ordered by comp and will abort the search when the key is less than the key in
 *      the current qnode. If the qnode does not exist in the queue, then this is linear search of a list with a O(n) time.
 *
 *      \return non-zero for failure, zero for success and pointer to qnode just before insertion point
 *
 * Copyright (c) 2019 Dale Anderson <daleanderson488@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files
 * (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>                          /* need NULL definition         */
#include "mydque.h"
#include "dque.h"

dque_err                                    /* returned completion status   */
dque_myscan(                                /* scan queue for insert point  */
dque_qnodep    *nodep,                      /* queue to search for insert   */
void           *key,                        /* key to search queue for      */
COMPFUNC        comp,                       /* user comparison function     */
dque_qnode    **node )                      /* ptr to qnode for insertion   */
{                                           /*------------------------------*/
    dque_qnode     *headp   = *nodep;       /* ptr to first qnode in queue  */
    dque_qnode     *currp   = headp;        /* ptr to current search qnode  */

    if ((*comp)( key, getdata(currp) ) >= 0) {/* insert point not at headp? */
        for (currp = getnext(currp); currp != headp; currp = getnext(currp) ) {
            if ((*comp)( key, getdata(currp) ) < 0) {
                break;                      /* found insert point?          */
            }
        }
    }

    *node   = currp;

    return (DQUEERR_NOERR);                 /* return no error              */
}

