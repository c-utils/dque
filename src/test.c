/**
 *
 *  \file	test.c
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	regression test for DQUE library functions.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  original
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
#include <string.h>                         /* need for strcmp              */
#include <time.h>
#include "dque.h"
#include "dque_util.h"

dque_err errcode = DQUEERR_NOERR;
unsigned status;

void     myprintf( dque_qhead *queue, int typ       );
void     myrprintf(dque_qhead *queue, int typ       );
dque_err prtest(   char *name,   dque_err expcode, dque_err errcode );
int      intcomp(  void *i1,     void *i2      );
int      intrcomp( void *i1,     void *i2      );

char    buf[128], bfr[128], bfr2[128], buff[128];
int    array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
int   array3[] = { 6, 2, 4, 1, 9, 3, 0, 8, 4, 0 };
char *array2[] = { "zero", "one", "two", "three", "four" };
char *name = "Sara West";

typedef struct {
    int  id;
    char name[20];
    int  year;
    int  classes[7];
} record;

void prrecord( record *rec );

typedef struct {
    int   idx;
    int   val;
    char *str;
} reckey;

record  array4[] = {
	{ 1076, "Sara West",  2021, { 123, 654, 789, 0, 0, 0, 0 }},
	{ 1145, "hey Goggle", 2022, { 123, 456, 707, 0, 0, 0, 0 }},
	{ 1103, "Siri Apple", 2021, { 123, 456, 987, 0, 0, 0, 0 }},
	{ 1000, "Fred Smith", 2020, { 123, 456, 789, 0, 0, 0, 0 }},
	{ 1172, "HAL IBM",    2023, { 123, 654, 999, 0, 0, 0, 0 }}
	};

/* This shows a more complicated "key" and that the key can be almost anything.  */
/* Note that reccomp returns zero if the key and qnode data match, and a value   */
/* greater than zero to insure that reccomp searches the entire queue. This is   */
/* required when searching a queue on a field other than the field on which it   */
/* is ordered.                                                                   */
int
reccomp( void *keyval, void *recval ) {
    reckey *key = (reckey *)keyval;
    record *rec = (record *)recval;
    int     retval = 1;                     /* return +val to search whole queue */

    switch (key->idx) {
        case 0: if (key->val == rec->id)              { retval = 0; } break;
	case 1: if (strcmp(key->str, rec->name) == 0) { retval = 0; } break;
        case 2: if (key->val == rec->year)            { retval = 0; } break;
    }

    return (retval);
}

void 
delay(int seconds) { 
    int millisecs = 1000 * seconds; 
    clock_t start = clock(); 
  
    while (clock() < (start + millisecs)) {
        ;
    }
} 

int
main(
int	argc,
char  **argv )
{
    dque_qhead *queue;
    dque_qhead *queue2 = NULL_QUEUE;
    dque_pri_qhead *pri;
    int   *val,  val2;
    int   major, minor;
    unsigned int headsize, nodesize, uns;
    dque_qiter *iter   = NULL_QITER;
    dque_qiter *iter2  = NULL_QITER;
    void  *data, *data2;
    reckey  key;
    record *rec;
    int i;

    status = 0;

#ifdef __STDC_VERSION__
    (void) printf( "\nversion = %ld\n", __STDC_VERSION__ );
#endif

    (void) printf( "\n------------------------------\ntesting options() with no queue\n" );
    if (prtest( "dque_options (DQUEOPT_HEADSIZ)",    DQUEERR_NOERR, options( NULL_QUEUE, DQUEOPT_HEADSIZ, &headsize ) )) {
        (void) printf( "qhead size: %u\n", headsize );
    }
    if (prtest( "dque_options (DQUEOPT_NODESIZ)",    DQUEERR_NOERR, options( NULL_QUEUE, DQUEOPT_NODESIZ, &nodesize ) )) {
        (void) printf( "qnode size: %u\n", nodesize );
    }
    (void) printf( "\n------------------------------\ntesting invalid create() and destroy() function calls\n" );
    prtest( "dque_create (invalid ptr to ptr to queue)",DQUEERR_NOQUEUEP, create(  NULL_QUEUEP,            0, (char *)NULL ) );
    prtest( "dque_create (invalid buffer size)",        DQUEERR_BUFSIZE,  create(  &queue,        headsize-1, (char *)NULL ) );
    prtest( "dque_create (invalid pointer to buffer)",  DQUEERR_NOBUF,    create(  &queue,               128, (char *)NULL ) );
    prtest( "dque_create (invalid node count)",         DQUEERR_MINCNT,   create(  &queue, headsize+nodesize, buf          ) );
    prtest( "dque_destroy(invalid ptr to ptr to queue)",DQUEERR_NOQUEUEP, destroy( NULL_QUEUEP                             ) );
    prtest( "dque_destroy(invalid pointer to queue)",   DQUEERR_NOQUEUE,  destroy( &queue2                                 ) );

    (void) printf( "\n------------------------------\ntesting create() with static memory, add static memory with options()\n" );
    prtest( "dque_create (using static memory)",     DQUEERR_NOERR,   create(   &queue, headsize+(2*nodesize), bfr      ) );
    prtest( "dque_push_back",                        DQUEERR_NOERR,   push_back( queue, &array[0]                       ) );
    prtest( "dque_push_back",                        DQUEERR_NOERR,   push_back( queue, &array[1]                       ) );
    prtest( "dque_push_back (no allocation allowed)",DQUEERR_ALLOCNA, push_back( queue, &array[2]                       ) );
    prtest( "dque_options (DQUEOPT_NOALLOC)",        DQUEERR_NOERR,   options(   queue, DQUEOPT_NOALLOC, 128, bfr2      ) );
    prtest( "dque_push_back",                        DQUEERR_NOERR,   push_back( queue, &array[2]                       ) );
    prtest( "dque_destroy (using static memory)",    DQUEERR_NOERR,   destroy(  &queue                                  ) );

    (void) printf( "\n------------------------------\ntesting dynamic create() function calls\n" );
    prtest( "dque_create (using dynamic memory)",  DQUEERR_NOERR,    create( &queue,                 0, (char *)NULL ) );
    if (prtest( "dque_options (DQUEOPT_VERSION)",  DQUEERR_NOERR,   options( queue, DQUEOPT_VERSION, &major, &minor ) )) {
        (void) printf( "version %d.%d\n", major, minor );
    }
    if (prtest( "dque_empty", DQUEERR_NOERR, empty( queue, &uns )) ) {
        (void) printf( "empty queue: %s\n", uns ? "True" : "False" );
    }
    (void) printf( "\n------------------------------\ntesting invalid calls to size() and max_size()\n" );
    prtest( "dque_size (invalid pointer to queue)",     DQUEERR_NOQUEUE,      size( NULL_QUEUE, &uns                 ) );
    prtest( "dque_size (invalid pointer to data)",      DQUEERR_NODATA,       size( queue,      (unsigned int *)NULL ) );
    prtest( "dque_max_size (invalid pointer to queue)", DQUEERR_NOQUEUE,  max_size( NULL_QUEUE, &uns                 ) );
    prtest( "dque_max_size (invalid pointer to data)",  DQUEERR_NODATA,   max_size( queue,      (unsigned int *)NULL ) );

    (void) printf( "\nsize and maxsize should both be 0\n" );
    if (prtest( "dque_size",      DQUEERR_NOERR,       size( queue, &uns ) )) { (void) printf( "    size: %u\n", uns ); }
    if (prtest( "dque_max_size",  DQUEERR_NOERR,   max_size( queue, &uns ) )) { (void) printf( "max size: %u\n", uns ); }

    (void) printf( "\n------------------------------\ntesting invalid calls to back() and front()\n" );
    prtest( "dque_back (invalid pointer to queue)",     DQUEERR_NOQUEUE,      back( NULL_QUEUE, &data         ) );
    prtest( "dque_back (invalid ptr to ptr to data)",   DQUEERR_NODATAP,      back( queue,      (void **)NULL ) );
    prtest( "dque_front (invalid pointer to queue)",    DQUEERR_NOQUEUE,     front( NULL_QUEUE, &data         ) );
    prtest( "dque_front (invalid ptr to ptr to data)",  DQUEERR_NODATAP,     front( queue,      (void **)NULL ) );

    (void) printf( "\n------------------------------\ntesting invalid calls to empty(), error(), and error_len()\n" );
    prtest( "dque_empty (invalid pointer to queue)",    DQUEERR_NOQUEUE,     empty( NULL_QUEUE, data          ) );
    prtest( "dque_empty (invalid pointer to data)",     DQUEERR_NODATA,      empty( queue,      (void *)NULL  ) );
    prtest( "dque_error (invalid pointer to buffer)",   DQUEERR_NOBUF,       error(     (dque_err)100, (void *)NULL, sizeof(buf) ) );
    prtest( "dque_error (invalid buffer size)",         DQUEERR_BUFSIZE,     error(     (dque_err)100, buf,          0           ) );
    prtest( "dque_error_len (invalid pointer to data)", DQUEERR_NODATA,      error_len( (dque_err)0,   (unsigned int *)NULL      ) );
    prtest( "dque_error_len (unknown error code)",      DQUEERR_UNKERR,      error_len( (dque_err)100, &uns                      ) );

    (void) printf( "\n------------------------------\ntesting invalid calls to insert(), and erase() (delete)\n" );
    prtest( "dque_insert (invalid pointer to queue)",   DQUEERR_NOQUEUE,    insert( NULL_QUEUE, &array[0],    iter       ) );
    prtest( "dque_insert (invalid pointer to data)",    DQUEERR_NODATA,     insert( queue,      (void *)NULL, iter       ) );
    prtest( "dque_erase (invalid pointer to queue)",    DQUEERR_NOQUEUE,    erase(  NULL_QUEUE, &data,        iter       ) );
    prtest( "dque_erase (invalid ptr to ptr to data)",  DQUEERR_NODATAP,    erase(  queue,      (void **)NULL,iter       ) );
    prtest( "dque_erase (invalid pointer to iter)",     DQUEERR_NOITER,     delete( queue,      &data,        NULL_QITER ) );

    (void) printf( "\n------------------------------\ntesting invalid calls to push_*() and pop_*()\n" );
    prtest( "dque_push_back (invalid pointer to queue)",DQUEERR_NOQUEUE, push_back( NULL_QUEUE, &array[0]     ) );
    prtest( "dque_push_back (invalid pointer to data)", DQUEERR_NODATA,  push_back( queue,      (void *)NULL  ) );
    prtest( "dque_push_front(invalid pointer to queue)",DQUEERR_NOQUEUE, push_front(NULL_QUEUE, &array[0]     ) );
    prtest( "dque_push_front(invalid pointer to data)", DQUEERR_NODATA,  push_front(queue,      (void *)NULL  ) );

    prtest( "dque_pop_back  (invalid pointer to queue)",DQUEERR_NOQUEUE, pop_back(  NULL_QUEUE, (void **)&val ) );
    prtest( "dque_pop_back  (invalid data pointer)",    DQUEERR_NODATAP, pop_back(  queue,      (void **)NULL ) );
    prtest( "dque_pop_front (invalid pointer to queue)",DQUEERR_NOQUEUE, pop_front( NULL_QUEUE, (void **)&val ) );
    prtest( "dque_pop_front (invalid data pointer)",    DQUEERR_NODATAP, pop_front( queue,      (void **)NULL ) );

    (void) printf( "\n------------------------------\ntesting invalid calls to iterator function\n" );
    prtest( "dque_begin (invalid pointer to queue)",    DQUEERR_NOQUEUE, begin(  NULL_QUEUE, &iter            ) );
    prtest( "dque_begin (invalid ptr to ptr to iter)",  DQUEERR_NOITERP, begin(  queue,      NULL_QITERP      ) );
    prtest( "dque_rbegin (invalid pointer to queue)",   DQUEERR_NOQUEUE, rbegin( NULL_QUEUE, &iter            ) );
    prtest( "dque_rbegin (invalid ptr to ptr to iter)", DQUEERR_NOITERP, rbegin( queue,      NULL_QITERP      ) );
    prtest( "dque_next (invalid pointer to queue)",     DQUEERR_NOQUEUE, next(   NULL_QUEUE, &iter            ) );
    prtest( "dque_next (invalid ptr to ptr to iter)",   DQUEERR_NOITERP, next(   queue,      NULL_QITERP      ) );
    prtest( "dque_next (invalid ptr to iterator)",      DQUEERR_NOITER,  next(   queue,      &iter            ) );
    prtest( "dque_rnext (invalid pointer to queue)",    DQUEERR_NOQUEUE, rnext(  NULL_QUEUE, &iter            ) );
    prtest( "dque_rnext (invalid ptr to ptr to iter)",  DQUEERR_NOITERP, rnext(  queue,      NULL_QITERP      ) );
    prtest( "dque_rnext (invalid ptr to iterator)",     DQUEERR_NOITER,  rnext(  queue,      &iter            ) );
    prtest( "dque_end (invalid pointer to queue)",      DQUEERR_NOQUEUE, end(    NULL_QUEUE, &iter            ) );
    prtest( "dque_end (invalid ptr to ptr to iter)",    DQUEERR_NOITERP, end(    queue,      NULL_QITERP      ) );
    prtest( "dque_rend (invalid pointer to queue)",     DQUEERR_NOQUEUE, rend(   NULL_QUEUE, &iter            ) );
    prtest( "dque_rend (invalid ptr to ptr to iter)",   DQUEERR_NOITERP, rend(   queue,      NULL_QITERP      ) );

    (void) printf( "\n------------------------------\ntesting invalid calls to key_*()\n" );
    prtest( "dque_key_find (invalid pointer to queue)",    DQUEERR_NOQUEUE, key_find(NULL_QUEUE,&array[5],    intcomp,        &iter         ) );
    prtest( "dque_key_find (invalid pointer to key)",      DQUEERR_NOKEY,   key_find(queue,     (void *)NULL, intcomp,        &iter         ) );
    prtest( "dque_key_find (invalid pointer to comp)",     DQUEERR_NOCOMP,  key_find(queue,     &array[5],    (COMPFUNC)NULL, &iter         ) );
    prtest( "dque_key_find (invalid ptr to ptr to iter)",  DQUEERR_NOITERP, key_find(queue,     &array[5],    intcomp,        NULL_QITERP   ) );
    prtest( "dque_key_find (empty queue)",                 DQUEERR_NOTFOUND,key_find(queue,     &array[5],    intcomp,        &iter         ) );
    (void) printf( "\n" );
    prtest( "dque_key_insert (invalid pointer to queue)",  DQUEERR_NOQUEUE, key_insert(NULL_QUEUE, (void *)NULL, intcomp,        data       ) );
    prtest( "dque_key_insert (invalid pointer to comp)",   DQUEERR_NOCOMP,  key_insert(queue,      (void *)NULL, (COMPFUNC)NULL, data       ) );
    (void) printf( "\n" );
    prtest( "dque_key_erase (invalid pointer to queue)",   DQUEERR_NOQUEUE, key_erase( NULL_QUEUE, &array[5],    intcomp,        &data      ) );
    prtest( "dque_key_erase (invalid pointer to key)",     DQUEERR_NOKEY,   key_erase( queue,      (void *)NULL, intcomp,        &data      ) );
    prtest( "dque_key_erase (invalid pointer to comp)",    DQUEERR_NOCOMP,  key_erase( queue,      &array[5],    (COMPFUNC)NULL, &data      ) );
    prtest( "dque_key_erase (invalid ptr to ptr to data)", DQUEERR_NODATAP, key_erase( queue,      &array[5],    intcomp,     (void **)NULL ) );

    (void) printf( "\n------------------------------\nuse options() to force two dynamic memory allocations\n" );
    prtest( "dque_options (set node count to 6)",       DQUEERR_NOERR,   options( queue, DQUEOPT_NODECNT, 6   ) );
    (void) printf( "fill queue with ten integer elements to force allocations\n" );
    for (i = 0; i < (sizeof(array)/sizeof(int)); ++i) {
        if (!prtest( "dque_push_back", DQUEERR_NOERR, push_back( queue, &array[i] )) ) {
	    break;
        }
    }
    (void) printf( "size and maxsize should both be 10, if so, two allocations of 6 were successful\n" );
    if (prtest( "dque_size",      DQUEERR_NOERR,       size( queue, &uns ) )) { (void) printf( "    size: %u\n", uns ); }
    if (prtest( "dque_max_size",  DQUEERR_NOERR,   max_size( queue, &uns ) )) { (void) printf( "max size: %u\n", uns ); }
    (void) printf( "print queue of integers using forward iterator then reverse iterator\n" );
    myprintf(  queue, 1 );
    myrprintf( queue, 1 );

    (void) printf( "\n------------------------------\ntesting invalid calls to get_data(), and set_data()\n" );
    (void) begin( queue, &iter ); /* have to have a valid iterator for these next tests */
    prtest( "dque_get_data (invalid pointer to queue)", DQUEERR_NOITER,   get_data( NULL_QITER, data          ) );
    prtest( "dque_get_data (invalid ptr to ptr to data)",DQUEERR_NODATAP, get_data( iter,       (void **)NULL ) );
    prtest( "dque_set_data (invalid pointer to queue)", DQUEERR_NOITER,   set_data( NULL_QITER, data          ) );
    prtest( "dque_set_data (invalid ptr to ptr to data)",DQUEERR_NODATA,  set_data( iter,       (void *)NULL  ) );

    (void) printf( "\n------------------------------\ntest key_*() functions\n" );
    val2    = 0;
    if (prtest( "dque_key_erase (remove zero node)", DQUEERR_NOERR, key_erase(queue, &val2, intcomp, &data ) )) {
        prtest( "dque_key_erase data == zero node",  DQUEERR_NOERR, (*(int *)data != array[val2]) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    val2    = 5;
    if (prtest( "dque_key_erase (remove fifth node)", DQUEERR_NOERR, key_erase(queue, &val2, intcomp, &data ) )) {
        prtest( "dque_key_erase data == fifth node",  DQUEERR_NOERR, (*(int *)data != array[val2]) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    val2    = 9;
    if (prtest( "dque_key_erase (remove ninth node)", DQUEERR_NOERR, key_erase(queue, &val2, intcomp, &data ) )) {
        prtest( "dque_key_erase data == ninth node",  DQUEERR_NOERR, (*(int *)data != array[val2]) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    val2    = 10;
    prtest( "dque_key_erase (attempt to remove non-existent node)", DQUEERR_NOERR, key_erase(queue, &val2, intcomp, &data ) );

    myprintf(  queue, 1 );
    myrprintf( queue, 1 );
    (void) printf( "\nremove all but one integer elements\n" );
    for (i = 4; i < (sizeof(array)/sizeof(int)); ++i) {
        if (!prtest( "dque_pop_front", DQUEERR_NOERR, pop_front( queue, (void *)&val )) ) {
	    break;
        }
    }
    (void) printf( "\nprint queue of integers using forward iterator then reverse iterator\n" );
    myprintf(  queue, 1 );
    myrprintf( queue, 1 );
    (void) printf( "\ncompare pointers returned by dque_back() and dque_front(), should be the same\n" );
    prtest( "dque_back",                                DQUEERR_NOERR,        back( queue,      &data         ) );
    prtest( "dque_front",                               DQUEERR_NOERR,       front( queue,      &data2        ) );
    prtest( "dque_back == dque_front",                  DQUEERR_NOERR, (data == data2) ? DQUEERR_NOERR : DQUEERR_UNKERR );
    (void) printf( "\ncompare iterators returned by dque_begin() and dque_rbegin(), should be the same\n" );
    prtest( "dque_begin",                               DQUEERR_NOERR,       begin( queue,      &iter         ) );
    prtest( "dque_rbegin",                              DQUEERR_NOERR,      rbegin( queue,      &iter2        ) );
    prtest( "dque_begin == dque_rbegin",                DQUEERR_NOERR, (iter == iter2) ? DQUEERR_NOERR : DQUEERR_UNKERR );
    (void) printf( "\ninsert and erase at the front, back, and middle of the queue\n" );
    prtest( "dque_insert",                              DQUEERR_NOERR,      insert( queue,   &array[0],  iter ) );
    prtest( "dque_insert",                              DQUEERR_NOERR,      insert( queue,   &array[5],  iter ) );
    prtest( "dque_insert",                              DQUEERR_NOERR,      insert( queue,   &array[9],  NULL_QITER    ) );
    myprintf(  queue, 1 );
    myrprintf( queue, 1 );
    prtest( "dque_rnext",                               DQUEERR_NOERR,      rnext(  queue,              &iter ) );
    prtest( "dque_delete",                              DQUEERR_NOERR,     delete(  queue,   &data,      iter ) );
    prtest( "dque_delete data == fifth node",           DQUEERR_NOERR, (*(int *)data == array[5]) ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_begin",                               DQUEERR_NOERR,      begin(  queue,              &iter ) );
    prtest( "dque_delete",                              DQUEERR_NOERR,     delete(  queue,   &data,      iter ) );
    prtest( "dque_delete data == zero node",            DQUEERR_NOERR, (*(int *)data == array[0]) ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_rbegin",                              DQUEERR_NOERR,     rbegin(  queue,              &iter ) );
    prtest( "dque_delete",                              DQUEERR_NOERR,     delete(  queue,   &data,      iter ) );
    prtest( "dque_delete data == ninth node",           DQUEERR_NOERR, (*(int *)data == array[9]) ? DQUEERR_NOERR : DQUEERR_UNKERR );
    myprintf(  queue, 1 );
    myrprintf( queue, 1 );

    (void) printf( "\nremove last integer elements\n" );
    prtest( "dque_pop_front", DQUEERR_NOERR, pop_front( queue, (void *)&val ));
    if (prtest( "dque_empty", DQUEERR_NOERR, empty( queue, &uns )) ) {
        (void) printf( "empty: %s\n", uns ? "True" : "False" );
    }
    prtest( "dque_destroy (invalid qhead pointer)",  DQUEERR_NOQUEUEP,destroy(  NULL_QUEUEP                             ) );
    prtest( "dque_destroy (invalid queue pointer)",  DQUEERR_NOQUEUE ,destroy(  &queue2                                 ) );
    prtest( "dque_destroy (using dynamic memory)",   DQUEERR_NOERR,   destroy(  &queue                                  ) );

    (void) printf( "\n------------------------------\nfill queue with ten random order integer elements using key_insert\n" );
    prtest( "dque_create (using dynamic memory)",    DQUEERR_NOERR,   create(   &queue,                 0, (char *)NULL ) );
    for (i = 0; i < (sizeof(array3)/sizeof(int)); ++i) {
        if (!prtest( "dque_key_insert", DQUEERR_NOERR, key_insert( queue, (void *)NULL, intcomp, &array3[i] )) ) {
	    break;
        }
    }
    (void) printf( "\nprint queue of integers using forward iterator\n" );
    myprintf( queue, 1 );
    (void) printf( "\nremove all integer elements\n" );
    for (i = 0; i < (sizeof(array3)/sizeof(int)); ++i) {
        if (!prtest( "dque_pop_front", DQUEERR_NOERR, pop_front( queue, (void *)&val )) ) {
	    break;
        }
    }

    (void) printf( "\n------------------------------\nturn off duplication, fill queue with eight random order integer elements\n" );
    prtest( "dque_options (DQUEOPT_NODUPE)",        DQUEERR_NOERR,  options( queue, DQUEOPT_NODUPE, 1 ) );
    for (i = 0; i < (sizeof(array3)/sizeof(int))-2; ++i) {
        if (!prtest( "dque_key_insert", DQUEERR_NOERR, key_insert( queue, (void *)NULL, intcomp, &array3[i] )) ) {
	    break;
        }
    }
    prtest( "dque_key_insert (with duplicate key)", DQUEERR_NODUPE, key_insert( queue, (void *)NULL, intcomp, &array3[9] ) );
    prtest( "dque_destroy (using dynamic memory)",  DQUEERR_NOERR,    destroy( &queue                                    ) );

    (void) printf( "\n------------------------------\nfill queue with five string elements\n" );
    prtest( "dque_create (using dynamic memory)",   DQUEERR_NOERR,    create(  &queue,                 0, (char *)NULL   ) );
    for (i = 0; i < (sizeof(array2)/sizeof(char *)); ++i) {
        if (!prtest( "dque_push_back",              DQUEERR_NOERR, push_back(   queue, array2[i] )                       ) ) {
	    break;
        }
    }
    (void) printf( "\nprint queue of strings using forward iterator then reverse iterator\n" );
    myprintf( queue, 2 );
    myrprintf( queue, 2 );

    (void) printf( "\n------------------------------\ncheck dque_back() and dque_front()\n" );
    if (prtest( "dque_back",                        DQUEERR_NOERR,      back( queue,        &data     )                  ) ) {
        prtest( "dque_back data == back of queue",  DQUEERR_NOERR, strcmp(array2[4], (char *)data) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    if (prtest( "dque_front",                       DQUEERR_NOERR,     front( queue,        &data     )                  ) ) {
        prtest( "dque_front data == front of queue",DQUEERR_NOERR, strcmp(array2[0], (char *)data) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    (void) printf( "\nremove the five string elements\n" );
    for (i = 0; i < (sizeof(array2)/sizeof(char *)); ++i) {
        if (!prtest( "dque_pop_front",              DQUEERR_NOERR, pop_front( queue, (void **)&data )                    ) ) {
	    break;
        }
    }
    if (prtest( "dque_empty", DQUEERR_NOERR, empty( queue, &uns )) ) {
        (void) printf( "empty: %s\n", uns ? "True" : "False" );
    }

    (void) printf( "\n------------------------------\nfill queue with five record elements using dque_key_insert with integer comparison\n" );
    (void) printf( "  this works because the first field in the record is an integer\n" );
    for (i = 0; i < (sizeof(array4)/sizeof(record)); ++i) {
        if (!prtest( "dque_key_insert", DQUEERR_NOERR, key_insert( queue, (void *)NULL, intcomp, &array4[i] )) ) {
	    break;
        }
    }
    (void) printf( "\nprint queue of records using forward iterator then reverse iterator\n" );
    myprintf(  queue, 1 );
    myrprintf( queue, 1 );
    if (prtest( "dque_back",                        DQUEERR_NOERR,      back( queue,          &data     )                ) ) {
        prtest( "dque_back data == back of queue",  DQUEERR_NOERR, intcomp((void *)&array4[4], data) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    if (prtest( "dque_front",                       DQUEERR_NOERR,     front( queue,          &data     )                ) ) {
        prtest( "dque_front data == front of queue",DQUEERR_NOERR, intcomp((void *)&array4[3], data) ? DQUEERR_UNKERR : DQUEERR_NOERR );
    }
    /* use different key/comparison function pair for searching on a field different from the one used to order the queue */
    key.idx = 1;	/* search on name */
    key.val = 2022;
    key.str = name;

    (void) printf( "\nlook for record with name of Sara West\n" );
    if (prtest( "dque_key_find",                    DQUEERR_NOERR,  key_find( queue, &key, reccomp, &iter )              ) ) {
        rec = (record *)my_data(iter);
        prtest( "rec data == Sara West",            DQUEERR_NOERR, strcmp( rec->name, name ) ? DQUEERR_UNKERR : DQUEERR_NOERR );
	prrecord( rec );
    }
    (void) printf( "\nlook for record with a year of 2022\n" );
    key.idx = 2;	/* search on year */
    if (prtest( "dque_key_find",                    DQUEERR_NOERR,  key_find( queue, &key, reccomp, &iter )              ) ) {
        rec = (record *)my_data(iter);
        prtest( "rec data == 2022",                 DQUEERR_NOERR, ( rec->year == 2022 ) ? DQUEERR_NOERR : DQUEERR_UNKERR );
	prrecord( rec );
    }
    (void) printf( "\nremove the five record elements\n" );
    for (i = 0; i < (sizeof(array4)/sizeof(record)); ++i) {
        if (!prtest( "dque_pop_front",              DQUEERR_NOERR, pop_front( queue,          &data )                    ) ) {
	    break;
        }
    }

    (void) printf( "\n" );
    prtest( "dque_destory (using dynamic memory)",  DQUEERR_NOERR,   destroy(  &queue                                    ) );

    (void) printf( "\n------------------------------\nfill queue with five order integer elements using stk_push\n" );
    prtest( "dque_create (using dynamic memory)",    DQUEERR_NOERR,   create(   &queue,                 0, (char *)NULL ) );
    for (i = 0; i < 5; ++i) {
        if (!prtest( "dque_stk_insert", DQUEERR_NOERR, stk_push( queue, &array[i] )) ) {
	    break;
        }
    }
    myprintf(  queue, 1 );
    prtest( "dque_stk_empty == 0",  DQUEERR_NOERR, stk_empty( queue )        == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_size  == 5",  DQUEERR_NOERR, stk_size( queue )         == 5 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_top   == 4",  DQUEERR_NOERR, *(int *)stk_top( queue )  == 4 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_pop   == 4",  DQUEERR_NOERR, *(int *)stk_pop( queue )  == 4 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_pop   == 3",  DQUEERR_NOERR, *(int *)stk_pop( queue )  == 3 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_pop   == 2",  DQUEERR_NOERR, *(int *)stk_pop( queue )  == 2 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_pop   == 1",  DQUEERR_NOERR, *(int *)stk_pop( queue )  == 1 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_pop   == 0",  DQUEERR_NOERR, *(int *)stk_pop( queue )  == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_stk_empty == 1",  DQUEERR_NOERR, stk_empty( queue )        == 1 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    
    (void) printf( "\n------------------------------\nfill queue with five order integer elements using que_push\n" );
    for (i = 0; i < 5; ++i) {
        if (!prtest( "dque_que_insert", DQUEERR_NOERR, que_push( queue, &array[i] )) ) {
	    break;
        }
    }
    myprintf(  queue, 1 );
    prtest( "dque_que_empty == 0",  DQUEERR_NOERR, que_empty( queue )        == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_size  == 5",  DQUEERR_NOERR, que_size( queue )         == 5 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_back  == 4",  DQUEERR_NOERR, *(int *)que_back( queue ) == 4 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_front == 0",  DQUEERR_NOERR, *(int *)que_front( queue )== 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_pop   == 0",  DQUEERR_NOERR, *(int *)que_pop( queue )  == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_pop   == 1",  DQUEERR_NOERR, *(int *)que_pop( queue )  == 1 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_pop   == 2",  DQUEERR_NOERR, *(int *)que_pop( queue )  == 2 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_pop   == 3",  DQUEERR_NOERR, *(int *)que_pop( queue )  == 3 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_pop   == 4",  DQUEERR_NOERR, *(int *)que_pop( queue )  == 4 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_que_empty == 1",  DQUEERR_NOERR, que_empty( queue )        == 1 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    
    (void) printf( "\n" );
    prtest( "dque_destory (using dynamic memory)",  DQUEERR_NOERR,   destroy(  &queue                                    ) );

    (void) printf( "\n------------------------------\nturn off duplication, fill queue with eight random order integer elements\n" );
    prtest( "dque_pri_create (dynamic memory)",   DQUEERR_NOERR, pri_create( &pri, intcomp )            );
    queue = getqueue( pri );
    prtest( "dque_options (DQUEOPT_NODUPE)",      DQUEERR_NOERR,    options( queue, DQUEOPT_NODUPE, 1 ) );
    for (i = 0; i < (sizeof(array3)/sizeof(int))-2; ++i) {
        if (!prtest( "dque_pri_push",             DQUEERR_NOERR, pri_push( pri, &array3[i] )) ) {
	    break;
        }
    }
    prtest( "dque_pri_push (with duplicate key)", DQUEERR_NODUPE, pri_push( pri, &array3[8] )            );
    prtest( "dque_pri_push (with duplicate key)", DQUEERR_NODUPE, pri_push( pri, &array3[9] )            );
    myprintf(  queue, 1 );

    prtest( "dque_pri_empty == 0",  DQUEERR_NOERR, pri_empty( pri )        == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_size  == 8",  DQUEERR_NOERR, pri_size( pri )         == 8 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_top   == 0",  DQUEERR_NOERR, *(int *)pri_top( pri )  == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 0",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 0 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 1",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 1 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 2",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 2 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 3",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 3 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 4",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 4 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 6",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 6 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 8",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 8 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_pop   == 9",  DQUEERR_NOERR, *(int *)pri_pop( pri )  == 9 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_empty == 1",  DQUEERR_NOERR, pri_empty( pri )        == 1 ? DQUEERR_NOERR : DQUEERR_UNKERR );
    prtest( "dque_pri_destroy (dynamic memory)",  DQUEERR_NOERR, pri_destroy( &pri )                     );

    (void) printf( "\nHello World!!!\n" );

    (void) printf( "\n%-76s%s\n", "Overall test status:", status ? "Fail" : "Pass" );

    return (errcode);
}

/**
 *      HELPER FUNCTIONS
 */

void
myprintf( dque_qhead *queue, int typ ) {
    dque_qiter *iter;
    void       *ptr;

    switch (typ ) {
        case 1: /* int */
            for (iter = my_begin( queue ); iter != my_end( queue ); my_next( queue, &iter )) {
                ptr = my_data( iter );
                (void) printf( " %i", *(int *)ptr );
            }
	    break;
        case 2: /* char * */
            for (iter = my_begin( queue ); iter != my_end( queue ); my_next( queue, &iter )) {
                ptr = my_data( iter );
                (void) printf( " %s", (char *)ptr );
            }
            break;
        default:
            break;
    }

    (void) printf( "\n" );
}

void
myrprintf( dque_qhead *queue, int typ ) {
    dque_qiter *iter;
    void       *ptr;

    switch (typ ) {
        case 1: /* int */
            for (iter = my_rbegin( queue ); iter != my_rend( queue ); my_rnext( queue, &iter )) {
                ptr = my_data( iter );
                (void) printf( " %i", *(int *)ptr );
            }
	    break;
        case 2: /* char * */
            for (iter = my_rbegin( queue ); iter != my_rend( queue ); my_rnext( queue, &iter )) {
                ptr = my_data( iter );
                (void) printf( " %s", (char *)ptr );
            }
            break;
        default:
            break;
    }

    (void) printf( "\n" );
}

dque_err
prtest( char *name, dque_err expcode, dque_err errcode ) {
    char buf[80];

    if (errcode == expcode) {               /* errcode as expected, no error*/
        (void) sprintf( buff, "%s:", name );
        errcode = DQUEERR_NOERR;
    } else if (errcode != DQUEERR_NOERR) {  /* returned real error, print it*/
        error( errcode, buf, sizeof(buf) );
        (void) sprintf( buff, "%s: %s", name, buf );
    } else {                                /* no error, but not as expected*/
        (void) sprintf( buff, "%s: returned code %d not as expected", name, errcode );
        errcode = DQUEERR_UNKERR;
    }

    status += (unsigned int)errcode;

    (void) printf( "%-76s%s\n", buff, errcode ? "Fail" : "Pass" );

    return (!errcode);                      /* return true if no error      */
}

void
prrecord( record *rec ) {
    int i;

    (void) printf( "key_find: %d, %d, \"%s\", classes:", rec->id, rec->year, rec->name );
    for (i = 0; i < sizeof(rec->classes)/sizeof(int); ++i) {
        (void) printf( " %d", rec->classes[i] );
    }
    (void) printf( "\n" );

    return;
}

int
intcomp( void *i1, void *i2 ) {
/*    (void) printf( "%i - %i = %i\n", *(int *)i1, *(int *)i2, (*(int *)i1 - *(int *)i2) ); */
    return (*(int *)i1 - *(int *)i2);
}

int
intrcomp( void *i1, void *i2 ) {
/*    (void) printf( "%i - %i = %i\n", *(int *)i1, *(int *)i2, (*(int *)i1 - *(int *)i2) ); */
    return (*(int *)i2 - *(int *)i1);
}


