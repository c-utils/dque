
# DQUE C Library

  Double-ended queue library with API similar to C++ deque modified for C

* Can be used for LIFO, FIFO, and Priority queues using any "key"
* Can push/pop a node on/off either front or back of the queue with execution O(1)
* Can search, insert, or delete a node with a specified "key" with execution O(n)
* All structures, memory allocations, pointer manipulations hidden from user
* All function calls similar to standard C++ deque/list calls modified for use in C
* Uses iterators instead of direct pointer manipulation
* A single pointer to data can be inserted on multiple queues
* A single queue can store multiple types of data (user to track different types)
* Short name may be used in most situations means "dque_" can be left off calls
* All functions return error codes similar to C11 functions with error checking
* DQUE functions only store a pointer to user's data, so data can be anything

  To build the library

 * DQUE library was developed using gcc compiler (built by MYSYS2 project) 9.2.0
 * works using -std=c17, c11, c99, c90, and -ansi -Wpadantic
 * clone DQUE project
 * change directory to dque/src
 * type 'make' to build the library
 * type 'make test' to build the test program and the library

  For more information

 * See dque_user_manual.pdf for more information on API and usage
 * See dque_internals.pdf  for more information on how DQUE works

## API

 Below is the public api currently provided by DQUE.

## dque_create( qhead **queue, int bufsize, (char *)buf )

 Allocate and initialize a `qhead`.
 
    dque_qhead *queue;                 /* only place user should use "dque_" prefix */
	create( &queue, 0, (char *)NULL ); /* bufsize = 0 means use dynamic allocations */

## dque_destroy( qhead **queue )

 Free qhead and all associated qnodes.

	destroy( &queue );

## dque_error( dque_err errcode, char *buf, unsigned int bufsize )

 Return error message equivalent of errcode in buf.
 
	char buf[128];
	int errcode;
	userdata data;
	if ((errcode = push_back( qhead, &data )) != 0) {
		error( buf, sizeof(buf), errcode );
		printf( "push_back failed: %s\n", buf );
	}

## dque_error_len( dque_err errcode, unsigned int *len )

  Return the length of the specified error errcode.

	unsigned int val;
	int errcode;
	error_len( errcode, &val );  /* error checking omitted for clarity */

##  dque_options( qhead *queue, int option, ... )

 Control queue behavior with options and various arguments. See dque.h for values.

## dque_empty( qhead *queue, unsigned int *val )

 Return non-zero value in val if queue is empty, otherwise zero.

	int val;
	empty( queue, &val );

## dque_size( qhead *queue, unsigned int *val )

  Return the number of nodes in the current queue in val. 

	unsigned int val;
	size( queue, &val );

## dque_max_size( qhead *queue, unsigned int *val )

  Return the number of nodes in the queue and free list in val.
  Basically, the total number of node created for the queue. 

	unsigned int val;
	max_size( queue, &val );

## dque_back( qhead *queue, (void **)data )

  Return a pointer to the user's data from the last node in the queue.
  This does not remove the node from the queue.

	userdata *data;
	back( queue, &data );

## dque_front( qhead *queue, (void **)data )

  Return a pointer to the user's data from the first node in the queue.
  This does not remove the node from the queue.

	userdata *data;
	front( queue, &data );

## dque_key_find( qhead *queue, void *key, COMPFUNC comp, void **data )

  Return a pointer to the user's data from the first node in the queue
  that equals the specified key as reported by the user's comparison
  function comp. This does not remove the node from the queue.

	userdata *data;
	userkey  *key;
    COMPFUNC  comp;	/* user function compares key with node data similar to qsort */
	key_find( queue, key, comp, &data );
	/* can now use the data */

## dque_get_data( dque_qiter *iter, (void **)data )

  Return a pointer to the user's data in the node specified by the iterator iter.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;		/* iter is set by iterator function described below */
	userdata *data;
	get_data( iter, &data );

## dque_set_data( dque_qiter *iter, (void *)data )

  Set the user's data in the node specified by the iterator iter.

	qiter    *iter;
	userdata *data;
	get_data( iter, data );

## dque_begin( qhead *queue, dque_qiter **iter )

  Return forward iterator, iter, to the front of the queue.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;
	begin( queue, &iter );

## dque_rbegin( qhead *queue, dque_qiter **iter )

  Return reverse iterator, iter, to the back of the queue.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;
	rbegin( queue, &iter );

## dque_next( qhead *queue, dque_qiter **iter )

  Return iterator, iter, to the next node forward from current iterator in the queue.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;
	next( queue, &iter );

## dque_rnext( qhead *queue, dque_qiter **iter )

  Return iterator, iter, to the next node rearward from current iterator in the queue.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;
	rnext( queue, &iter );

## dque_end( qhead *queue, dque_qiter **iter )

  Return iterator, iter, to stop forward iteration of the queue.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;
	end( queue, &iter );

## dque_rend( qhead *queue, dque_qiter **iter )

  Return iterator, iter, to stop reverse iteration of the queue.
  The file wrapper.c contains a wrapper for this function that is easier to use.

	qiter    *iter;
	rend( queue, &iter );

## dque_delete( qhead *queue, (void **)data, dque_qiter *iter )

  Alias for erase() in C only.

## dque_erase( qhead *queue, (void **)data, dque_qiter *iter )

  Remove and free node specified by iterator, return data pointer to user's data.

	qiter    *iter;
	userdata *data;
	erase( queue, &data, iter );

## dque_insert( qhead *queue, (void *)data, dque_qiter *iter )

  Insert new node, with user's data, into queue just before node specified by iterator.

	qiter    *iter;
	userdata *data;
	erase( queue, &data, iter );

## dque_key_delete( qhead *queue, (void *)key, COMPFUNC comp, (void **)data )

  Alias for key_erase() in C only.

## dque_key_erase( qhead *queue, (void *)key, COMPFUNC comp, (void **)data )

  Return a pointer to the user's data from the first node in the queue
  that equals the specified key as reported by the user's comparison
  function comp. Remove and free the node that had the user's data.

	userdata *data;
	userkey  *key;
    COMPFUNC  comp;	/* user function compares key with node data similar to qsort */
	key_erase( queue, key, comp, &data );

## dque_key_insert( qhead *queue, (void *)key, COMPFUNC comp, (void *)data )

  Insert a new node with the user's data into the queue just in front of the 
  first node that is greater than the specified key as reported by the user's
  comparison function comp.

	userdata *data;
	userkey  *key;
    COMPFUNC  comp;	/* user function compares key with node data similar to qsort */
	key_erase( queue, key, comp, &data );

## dque_pop_back( qhead *queue, (void **)data )

  Remove and free node from back of queue, return data pointer to user's data.
  
	userdata *data;
	pop_back( queue, &data );

## dque_pop_front( qhead *queue, (void **)data )

  Remove and free node from front of queue, return data pointer to user's data.
  
	userdata *data;
	pop_front( queue, &data );

## dque_push_back( qhead *queue, (void *)data )

  Insert new node with user's data onto back of the queue.
  
	userdata *data;
	push_back( queue, data );

## dque_push_front( qhead *queue, (void *)data )

  Insert new node with user's data onto front of the queue.
  
	userdata *data;
	push_front( queue, data );

## Examples

  Fill queue with five strings from an array.

<pre><code>
char *array2[] = { "zero", "one", "two", "three", "four" };

for (int i = 0; i < (sizeof(array2)/sizeof(char *)); ++i) {
    if (push_back( queue, &array2[i] ) != DQUEERR_NOERR ) {
        /* handle error */
        break;
    }
}
</code></pre>

  Fill queue with ten integers from an integer array.

<pre><code>
int    array[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

for (int i = 0; i < (sizeof(array)/sizeof(int)); ++i) {
    if (push_back( queue, &array[i] ) != DQUEERR_NOERR ) {
        /* handle error */
        break;
    }
}
</code></pre>

  Forward iteration using wrapper code with ten integers as data:

<pre><code>
qiter *iter;
void  *ptr;

for (iter = my_begin( queue ); iter != my_end( queue ); my_next( queue, &iter )) {
    ptr = my_data( iter );
    (void) printf( " %i", *(int *)ptr );
}
(void) printf( "\n" );
</code></pre>

  stdout:

    0 1 2 3 4 5 6 7 8 9

  An example of sample student record is in the file test.c

## Testing

    $ make test

## License 

(The MIT License)

Copyright (c) 2019 Dale Anderson &lt;daleanderson488@gmail.com&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
