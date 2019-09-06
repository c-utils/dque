
/**
 *
 *  \file	dque_create.c
 *  \name	dque_create
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Create an empty circular doubly-linked list.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      create(
 *      dque_qheadp            *queuep,        pointer to pointer to queue head
 *      unsigned                siz,           size of the static memory buffer
 *      char                   *buf )          pointer to static memory to use for qhead and qnode allocations
 *
 *      Create and populate a qhead structure for a circular doubly-linked list. This function can be used to create
 *      the queue head structure in three different ways. The first, and most common, method is to invoke the
 *      function with a buffer size argument of 0 (zero) which creates an empty queue using dynamically allocated
 *      memory. The second method is to invoke the function with a buffer size equal to the size of qhead structure.
 *      This prevents the whole library from using any dynamically allocated memory and creates the qhead structure
 *      in the specified buffer. The user may create qnodes using the dque_options function with the DQUEOPT_NOALLOC
 *      option. The third, and final, method is provided as a convenience and is a slight variation of the second
 *      method in that the specified buffer is much larger than the qhead structure. The extra buffer space is then
 *      used to create as many qnodes as possible and placed on the free list. The user can dque_max_size function
 *      to query how many nodes were created.
 *
 *      The queue head structure contains pointers to three lists. The first, called head, is the actual user queue,
 *      the second, called free, is the free list holding the nodes that are available for the user, but not yet used.
 *      The third list, called blks, is used by the library to track dynamic memory allocations. The qnodes are
 *      created in groups as a single dynamic memory allocation. The pointer to the memory allocation is also a pointer
 *      to the first node, so the first node is inserted onto the third list, blks, to be used later to free the memory
 *      allocations. The remaining qnodes are put on the free list.
 *
 *      The queue head also contains counts for each of the three lists plus a variable of bit flags to be used to
 *      control the behavior of the library functions by getting and setting the bit flags. The last variable
 *      in the queue head is an integer to store the version number of the library.
 *
 *      \return non-zero for failure, zero for success and a pointer to the head of the queue
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
#include <stdlib.h>                         /* need for malloc and free     */
#include "mydque.h"
#include "dque.h"

/**
 *  \name 	dque_populate_head
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Initalize fields in qhead.
 */

dque_err static                             /* returned completion status   */
dque_populate_head(                         /* internal helper function     */
dque_qhead     *queue )                     /* pointer to queue             */
{
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */

    setvers( queue, DQUE_VERSION );         /* library version number       */
    setflgs( queue, DQUE_NOFLAGS );         /* library operation flags      */
    sethead( queue, NULL_NODE    );         /* NULL head queue              */
    setfree( queue, NULL_NODE    );         /* NULL free list               */
    setblks( queue, NULL_NODE    );         /* NULL allocation blocks list  */
    sethcnt( queue, 0            );         /* current # nodes on head queue*/
    setfcnt( queue, 0            );         /* current # nodes on free list */
    setbcnt( queue, 0            );         /* current # nodes on blks list */
    setacnt( queue, DQUE_NODECNT );         /* default node count allocation*/

    return (errcode);
}

dque_err                                    /* returned completion status   */
dque_create(                                /* create a queue               */
dque_qheadp    *queuep,                     /* ptr to ptr to queue          */
unsigned int    siz,                        /* size of the buffer           */   
char           *buf )                       /* ptr to buffer to use for head*/
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qhead     *queue   = NULL_QUEUE;
    unsigned        cnt;

    if (queuep == NULL_QUEUEP) {            /* invalid queue head pointer?  */
        errcode = DQUEERR_NOQUEUEP;
    } else if (siz == 0) {                  /* use dynamic memory allocation? */
        if ((queue = (dque_qhead *)malloc( sizeof(dque_qhead) )) == NULL_QUEUE) {
	    errcode = DQUEERR_NOALLOC;      /* queue head allocation fail?  */
        } else if ((errcode = dque_populate_head( queue )) == DQUEERR_NOERR) {
            *queuep = queue;
        }
    } else {                                /* do not use dynamic memory    */
        if (siz < sizeof(dque_qhead)) {     /* buffer size too small?       */
            errcode = DQUEERR_BUFSIZE;
        } else if (buf == (char *)NULL) {   /* invalid buffer pointer?      */
            errcode = DQUEERR_NOBUF;
        } else {
            queue   = (dque_qhead *)buf;    /* point queue to new queue head*/
            buf    += sizeof(dque_qhead);   /* move pointer past new qhead  */
            siz    -= sizeof(dque_qhead);     /* remove qhead size from buf size */
            cnt     = siz/sizeof(dque_qnode); /* convert buf size to qnode count */
            if ((errcode = dque_populate_head( queue )) == DQUEERR_NOERR) {
                setflag( queue, DQUE_NOALLOC );
            }
            *queuep = queue;                /* qhead valid at this point    */
            if (cnt != 0) {                 /* need to allocate nodes?      */  
                if (cnt < (unsigned int)DQUE_MINCNT) { /* invalid node cnt?*/
                    errcode = DQUEERR_MINCNT;
                } else {                    /* cnt valid, create nodes      */
                    errcode = dque_mypopulate( queue, cnt, (dque_qnode *)buf );
                }
            }
        }
    }

    return (errcode);
}

