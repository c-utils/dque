
/**
 *
 *  \file	dque_mysearch.c
 *  \name	dque_mysearch
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Search queue for specified qnode.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_mysearch(
 *      qnodep                 *nodep,         queue to search
 *      qnode                  *node )         qnode to search for
 *
 *      Search the specified queue for the specified node. This function is
 *      different from dque_myfind and dque_myscan in that it does not look at
 *      the user's data but looks only at pointers to qnodes.
 *
 *      \return zero for success, non-zero for a failure
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
dque_mysearch(                              /* search queue for qnode       */
dque_qnodep    *nodep,                      /* queue to search for qnode    */
dque_qnode     *node )                      /* qnode to search for          */
{                                           /*------------------------------*/
    dque_qnode     *headp   = *nodep;       /* first qnode in queue         */
    dque_qnode     *currp   = headp;        /* current qnode in search      */


    do  {
        if (currp == node) {                /* specified node first in list?*/
            return (DQUEERR_NOERR);
        }
        currp = getnext(currp);
    } while (currp != headp);

    return (DQUEERR_NOTFOUND);
}

