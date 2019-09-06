
/**
 *
 *  \file	dque_options.c
 *  \name	dque_options
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	get/set various queue variables that change the behavior of the library.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *      dque_options( 
 *      dque_qhead             *queue;         queue to control
 *      int                     opt;           get/set option
 *                              __VA_ARGS__    various arguments based on option opt
 *
 *      Return error status and pointer to data based on option. Options:
 *
 *      DQUEOPT_NOOPT       - dque_options( queue, DQUEOPT_NOOPT )
 *                            no option option, hehe
 *      DQUEOPT_VERSION     - dque_options( queue, DQUEOPT_VERSION, int *major, int *minor )
 *                            returns major and minor version numbers
 *      DQUEOPT_NODECNT     - dque_options( queue, DQUEOPT_NODECNT, unsigned int siz )
 *                            sets the number of nodes to allocate with each memory allocation
 *      DQUEOPT_HEADSIZ     - dque_options( queue, DQUEOPT_HEADSIZ, unsigned int *siz )
 *                            returns the size of the queue head structure
 *      DQUEOPT_NODESIZ     - dque_options( queue, DQUEOPT_NODESIZ, unsigned int *siz )
 *                            returns the size of the queue node structure
 *      DQUEOPT_NOALLOC     - dque_options( queue, DQUEOPT_NOALLOC, unsigned int siz, char *buf )
 *                            if not using dynamic memory allocation, used the specified buffer to create qnodes
 *      DQUEOPT_NODUPE      - dque_options( queue, DQUEOPT_NODUPE, unsigned int siz )
 *                            non-zero means no duplicates in priority queue, zero means allow duplicates
 *      DQUEOPT_NOSCAN      - dque_options( queue, DQUEOPT_NODUPE, unsigned int siz )
 *                            non-zero means no scan on insert and remove, zero means use scan on insert and remove
 *      DQUEOPT_NOORDER     - dque_options( queue, DQUEOPT_NOORDER, unsigned int siz )
 *                            non-zero means find does not assume ordered queue, zero means find assumes ordered queue
 *
 *      \return non-zero for failure, zero for success, and if query, returned pointers to data
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
#include <stdarg.h>                         /* need for variadic arguments  */
#include "mydque.h"
#include "dque.h"

dque_err                                    /* returned status              */
dque_options(                               /* get/set queue option         */
dque_qhead     *queue,                      /* queue to count nodes         */
int             opt,                        /* get/set option               */
                ... )                       /* option arguments             */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;/* non-zero indicates failure   */
    int             *major, *minor;
    unsigned int    siz, *val;
    char           *buf;
    va_list         args;

    if (queue == NULL_QUEUE && !(opt == DQUEOPT_HEADSIZ || opt == DQUEOPT_NODESIZ) ) {
        errcode = DQUEERR_NOQUEUE;          /* if opt not head size, invalid queue? */
    } else {
        va_start( args, opt );
	switch (opt) {
            case DQUEOPT_NOOPT:             /* no operation, couldn't resist*/
                break;
	    case DQUEOPT_VERSION:
                major   = va_arg( args, int * );
                minor   = va_arg( args, int * );
               *major   = (getvers( queue ) >> 16) & 0x0000FFFF;
               *minor   =  getvers( queue )        & 0x0000FFFF;
                break;
            case DQUEOPT_NODECNT:           /* # nodes to allocate each time*/
		siz = va_arg( args, unsigned int );
                if (siz < (unsigned int)DQUE_MINCNT) {
                    errcode = DQUEERR_MINCNT;   /* siz not => minimum size? */
                } else {
                    setacnt( queue, siz );  /* valid node count size, set it*/
                }
                break;
            case DQUEOPT_HEADSIZ:           /* returns size of queue head   */
                val     = va_arg( args, unsigned int * );
                *val    = sizeof( dque_qhead );
                break;
            case DQUEOPT_NODESIZ:           /* returns size of queue node   */
                val     = va_arg( args, unsigned int * );
                *val    = sizeof( dque_qnode );
                break;
	    case DQUEOPT_NOALLOC:       /* siz is size of buf, need # nodes */
                siz = va_arg( args, unsigned int ) / sizeof(dque_qnode);
                buf = va_arg( args, char * );
                if (getflag( queue, DQUE_NOALLOC ) == 0) {
                    errcode = DQUEERR_ALLOCNA;
                } else if (siz < (unsigned int)DQUE_MINCNT) {
                    errcode = DQUEERR_MINCNT;   /* siz not => minimum size? */
                } else if ((errcode = dque_mypopulate( queue, siz, (dque_qnode *)buf )) == DQUEERR_NOERR) {
                    setflag( queue, DQUE_NOALLOC );
                }
                break;
            case DQUEOPT_NODUPE:        /* !0 = no duplicates in priority queue, 0 = allow duplicates  */
                siz     = va_arg( args, unsigned int ); /* siz == a boolean  */
                setflag( queue, ((siz != 0) ? DQUE_NODUPE : 0) );
                break;
            case DQUEOPT_NOSCAN:        /* !0 = no scan on insert, remove, 0 = scan before insert,remove  */
                siz     = va_arg( args, unsigned int ); /* siz == a boolean  */
                setflag( queue, ((siz != 0) ? DQUE_NOSCAN : 0) );
                break;
            default:
                errcode = DQUEERR_UNKOPT;
                break;
            }
        va_end( args );
        }

    return (errcode);
}

