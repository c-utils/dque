
/**
 *
 *  \file	dque_key_find.c
 *  \name	dque_key_find
 *  \author	Dale Anderson
 *  \date	07/30/2019
 *  \brief	Find qnode with the specified key, and return iterator to the qnode.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.1		07/30/2019  	D.Anderson  original
 *
 *      dque_key_find(
 *      dque_qhead             *queue,         queue to search for node w/key
 *      void                   *key,           key to search the queue with
 *      COMPFUNC                comp,          user defined comparison function
 *      dque_qiter            **iter )         pointer to a pointer to an iterator w/key node
 *
 *      Search the queue for a node with the specified key. The comparison
 *      function will return zero if the current node in queue matches the key.
 *      The comparison function takes two arguments and the first one MUST
 *      be the specified key arument and the data pointer stored in each
 *      node as the second argument. The user supplied comparison function
 *      should return an integer follows:
 *
 *                  < 0 if key < data->key
 *                    0 if key = data->key
 *                  > 0 if key > data->key
 *
 *      \return non-zero for failure, zero for success and iterator to found qnode.
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
dque_key_find(                              /* find node with specified key */
dque_qhead     *queue,                      /* queue to have node inserted  */
void           *key,                        /* key to search queue for      */
COMPFUNC        comp,                       /* comparison function          */
dque_qiter    **iter )                      /* iterator to specified node   */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *headp;                  /* pointer to 1st qnode in queue*/
    dque_qnode     *node;                   /* returned node as iterator    */

    if (queue == NULL_QUEUE) {              /* invalid queue pointer?       */
        errcode = DQUEERR_NOQUEUE;
    } else if (key == (void *)NULL) {       /* no key?                      */
        errcode = DQUEERR_NOKEY;
    } else if (comp == (COMPFUNC)NULL) {    /* no caomparison function?     */
        errcode = DQUEERR_NOCOMP;
    } else if (iter == NULL_QITERP) {       /* invalid iterator pointer?    */
        errcode = DQUEERR_NOITERP;
    } else if ((headp = gethead(queue)) == NULL_NODE) {     /* empty queue? */
        errcode = DQUEERR_NOTFOUND;
    } else if ((errcode = dque_myfind( &headp, key, comp, &node )) == DQUEERR_NOERR) {
        *iter   = (dque_qiter *)node;       /* set the data                 */
    }

    if (errcode != DQUEERR_NOERR && iter != NULL_QITERP) {
        *iter   = NULL_QITER;               /* just to make sure            */
    }

    return (errcode);
}

