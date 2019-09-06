/**
 *
 *  \file	mydque.h
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Internal implementation header for dequeue functions.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
 *
 *  	This header file is for internal use only and should not be used by the user.
 *  	The user should only use the dque.h file which is all they need to use the
 *  	functions of this library. 
 *
 * 	This header file contains the definitions of the actual qhead and qnode structures,
 * 	macros to access those structures, DQUE bit flags, internal function declarations,
 * 	and all error code definitions. All internal functions should include this file
 * 	before the dque.h to prevent the qhead and qnode definitions from activating.
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

#ifndef MYDQUE_H
#define MYDQUE_H

#include <limits.h>                         /* need for UINT_MAX            */

/* global defines, some may be changed through dque_options()               */
#define DQUE_VERSION   0x00010001           /* version 1.1                  */
#define DQUE_NODECNT   25                   /* default allocation node count*/
#define DQUE_MINCNT    2                    /* minimum node allocation count*/

/* this is THE real DQUE structions, try to contain yourself                */
typedef struct dque_qnode
    {
    struct dque_qnode  *next;
    struct dque_qnode  *prev;
    void               *data;
    } dque_qnode, *dque_qnodep;
#define DQUE_QNODE_DEF                      /* cause dque.h to use this qnode */

#define dque_qiter  dque_qnode
#define dque_qiterp dque_qnodep
#define DQUE_QITER_DEF                      /* cause dque.h to use this qiter */

typedef struct dque_qhead
    {
    unsigned int        vers;               /* version # for later expansion*/
    unsigned int        flgs;               /* bit flags for future use     */
    dque_qnode         *head;               /* ptr to queue's first node    */
    dque_qnode         *free;               /* ptr to list of free nodes    */
    dque_qnode         *blks;               /* ptr to list of allocated blks*/   
    unsigned int        hcnt;               /* # of nodes in head queue     */ 
    unsigned int        fcnt;               /* # of nodes in free list      */ 
    unsigned int        bcnt;               /* # of nodes in blks list      */ 
    unsigned int        acnt;               /* # of nodes to allocate       */
    } dque_qhead, *dque_qheadp;
#define DQUE_QHEAD_DEF                      /* cause dque.h to use this qhead */

/* this definition is same as dque.h, define allows for future changes      */
typedef int (* COMPFUNC)( void *, void * );
#define COMFUNC_DEF                         /* cause dque.h to use this definition */

#define NULL_NODE       (dque_qnode  *)NULL /* NULL qnode pointer           */
#define NULL_NODEP      (dque_qnodep *)NULL /* NULL qnode pointer pointer   */
#define NULL_QUEUE      (dque_qhead  *)NULL /* NULL queue pointer           */
#define NULL_QUEUEP     (dque_qheadp *)NULL /* NULL queue pointer pointer   */
#define NULL_QITER      (dque_qiter  *)NULL /* NULL iterator pointer        */
#define NULL_QITERP     (dque_qiterp *)NULL /* NULL iterator pointer pointer*/

/* flags */
#define getflag(q,f)    (getflgs(q)&(f))
#define setflag(q,f)    (getflgs(q)|=(f))
#define resetflag(q,f)  (getflgs(q)&=(~(f)))
#define DQUE_NOFLAGS    0x00000000
#define DQUE_NOALLOC    0x00000001          /* no dynamic allocation        */
#define DQUE_NODUPE     0x00000002          /* no duplication in priority queue */
#define DQUE_NOSCAN     0x00000004          /* no scan on insert, remove    */

/* use these macros to increment/decrement counters to prevent wrap around  */
#define dec_uns(i)      ((i)-=((i)>0?1:0))
#define inc_uns(i)      ((i)+=((i)<UINT_MAX?1:0))

/* internal defines for rotation                                            */
#define NOROTATE        0                   /* do not rotate node           */
#define ROTATE          1                   /* rotate node after insertion  */
                                            /*  or before deletion          */

/* macros to access structures because rule #1 is never access them directly*/
#define getnext(n)      (n)->next
#define setnext(n,p)    getnext(n)=(p)

#define getprev(n)      (n)->prev
#define setprev(n,p)    getprev(n)=(p)

#define getdata(n)      (n)->data
#define setdata(n,x)    getdata(n)=(x)

#define getvers(a)      ((a)->vers)
#define setvers(a,p)    getvers(a)=(p)

#define getflgs(a)      ((a)->flgs)
#define setflgs(a,p)    getflgs(a)=(p)

#define gethcnt(a)      ((a)->hcnt)
#define sethcnt(a,p)    gethcnt(a)=(p)

#define getfcnt(a)      ((a)->fcnt)
#define setfcnt(a,p)    getfcnt(a)=(p)

#define getbcnt(a)      ((a)->bcnt)
#define setbcnt(a,p)    getbcnt(a)=(p)

#define getacnt(a)      ((a)->acnt)
#define setacnt(a,p)    getacnt(a)=(p)

#define gethead(a)      ((a)->head)
#define sethead(a,p)    gethead(a)=(p)

#define getfree(a)      ((a)->free)
#define setfree(a,p)    getfree(a)=(p)

#define getblks(a)      ((a)->blks)
#define setblks(a,p)    getblks(a)=(p)

/* list error codes for those function that return error codes              */
typedef enum dque_err {
        DQUEERR_NOERR       =  0,   /* no error                             */
        DQUEERR_UNKERR      =  1,   /* unknown error code                   */
        DQUEERR_NOALLOC     =  2,   /* could not allocate memory            */
        DQUEERR_NOQUEUE     =  3,   /* no pointer to queue                  */
        DQUEERR_NOQUEUEP    =  4,   /* no pointer to pointer to queue       */
        DQUEERR_NODATA      =  5,   /* no pointer to data                   */
        DQUEERR_NODATAP     =  6,   /* no pointer to pointer to data        */
        DQUEERR_NOITER      =  7,   /* no pointer to iterator               */
        DQUEERR_NOITERP     =  8,   /* no pointer to pointer to iterator    */
        DQUEERR_BUFSIZE     =  9,   /* no pointer to buffer                 */
        DQUEERR_MINCNT      = 10,   /* allocation count too small           */
        DQUEERR_EMPTY       = 11,   /* empty queue                          */
        DQUEERR_NOKEY       = 12,   /* no search key                        */
        DQUEERR_NOCOMP      = 13,   /* no comparison function               */
        DQUEERR_ALLOCNA     = 14,   /* dynamic allocation not allowed       */
        DQUEERR_NOTFOUND    = 15,   /* node not found in queue              */
        DQUEERR_UNKOPT      = 16,   /* unknown options option               */
        DQUEERR_NODUPE      = 17,   /* no duplicates in priority queue      */
        DQUEERR_INVITER     = 18,   /* invalid iterator to deleted node     */
        DQUEERR_NOBUF       = 19,   /* no pointer to buffer                 */
        DQUEERR_INVHDR      = 20    /* invalid header                       */
	} dque_err;                 /* other error codes go here            */
#define DQUE_ERR_DEF                /* cause dque.h to use this enum        */

/* internal function definitions                                            */
extern dque_err dque_myalloc(    dque_qhead  *                                     );
extern dque_err dque_myinsert(   dque_qnodep *, dque_qnode *,  int, unsigned int * );
extern dque_err dque_mydelete(   dque_qnodep *, dque_qnode **, int, unsigned int * );
extern dque_err dque_mypopulate( dque_qhead  *, unsigned int,  dque_qnode *        );
extern dque_err dque_myfind(     dque_qnodep *, void *, COMPFUNC, dque_qnode **    );
extern dque_err dque_myscan(     dque_qnodep *, void *, COMPFUNC, dque_qnode **    );
extern dque_err dque_mysearch(   dque_qnodep *, dque_qiter *                       );

#endif /* MYLIST_H */
