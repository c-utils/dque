
/**
 *
 *  \file	dque_myalloc.c
 *  \name	dque_myalloc
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Dynamically allocate memory for qnodes.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      Dynamically allocate memory space for qnodes and place them on the free list.
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
dque_myalloc(                               /* internal qnode allocation    */
dque_qhead    *queue )                      /* pointer to queue head        */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    dque_qnode     *array;

    if (getflag( queue, DQUE_NOALLOC ) != 0) {
        errcode = DQUEERR_ALLOCNA;          /* allowed to allocate memory?  */
    } else if ((array = (dque_qnode *)malloc( getacnt(queue) * sizeof(dque_qnode) )) == NULL_NODE) {
        errcode = DQUEERR_NOALLOC;          /* memory allocation fail?      */
    } else if ((errcode = dque_myinsert( &getblks( queue ), &array[0], NOROTATE, &getbcnt( queue ) )) == DQUEERR_NOERR) {
        errcode = dque_mypopulate( queue, getacnt(queue)-1, &array[1] );   /* save first node to free allocation later */
    }                                                                      /* put the remaining nodes on the free list */

    return (errcode);
}

/**
 *  \name	dque_mypopulate
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Dynamically allocate memory for qnodes.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      Put specified cnt qnodes from specified array onto free list.
 */
 
dque_err                                    /* returned completion status   */
dque_mypopulate(                            /* internal qnode population    */
dque_qhead    *queue,                       /* pointer to queue head        */
unsigned int   cnt,                         /* number of qnodes to populate */
dque_qnode    *array )                      /* array of qnodes              */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    int             i;

    for (i = 0; i < cnt; ++i) {             /* put remaining nodes on free list */
        setdata( &array[i], (void *)NULL ); /* set data to null to show invalid qnode */
        if ((errcode = dque_myinsert( &getfree( queue ), &array[i], NOROTATE, &getfcnt( queue ) )) != DQUEERR_NOERR) {
            break;
        }
    }

    return (errcode);
}

