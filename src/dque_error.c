
/**
 *
 *  \file	dque_error.c
 *  \name	dque_error
 *  \author	Dale Anderson
 *  \date	07/11/2019
 *  \brief	Return error message equivalent to specified error.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.0		07/11/2019  	D.Anderson  Original
 *  1.1		07/30/2019  	D.Anderson  Return error status instead of buffer pointer (buf),
 *                                          removed header (hdr) argument, added size (siz) of buffer.
 *
 *      dque_error(
 *      int                     err,    error code
 *      char                   *buf,    buffer for equivalent error message
 *      unsigned int            siz )   size of buffer
 *
 *      error returns an error message equivalent to the specified error code.
 *      The error message is placed in the specified buffer upto siz - 1
 *      characters. If the error is negative, this function automatically
 *      negates the error code. If the error is unknown, this function will
 *      return the unknown error code message.
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
#include <string.h>                         /* need for sprintf, strcpy     */
#include "mydque.h"
#include "dque.h"

static const char  *emsg[]  = {
    "No error",
    "Unknown error code",
    "Could not dynamically allocate memory",
    "Invalid pointer to queue",
    "Invalid pointer to pointer to queue",
    "Invalid pointer to data",
    "Invalid pointer to pointer to data",
    "Invalid pointer to iterator",
    "Invalid pointer to pointer to iterator",
    "Specified buffer size is too small",
    "Buffer size resulted in too few nodes",
    "Empty queue",
    "Invalid pointer to key",
    "Invalid pointer to comparison function",
    "Dynamic allocation not allowed",
    "Node not found",
    "Unknown queue options option",
    "No duplicates allowed in priority queue",
    "invalid iterator to deleted node",
    "invalid pointer to buffer",
    "invalid header buffer"
};

static const int DQUEERR_MAXERR = sizeof(emsg)/sizeof(char *); /* maximum error code */

dque_err                                    /* returned error code          */
dque_error(                                 /* build error msg from err     */
dque_err        err,                        /* error code                   */
char           *buf,                        /* buffer to hold error message */
unsigned int    siz )                       /* size of buf                  */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;

    if (err < 0) {                          /* it is OK to be negative      */
        err     = -err;
    }

    if (err >= DQUEERR_MAXERR) {            /* invalid error code?          */
        err     = DQUEERR_UNKERR;
    }

    if (buf == (char *)NULL) {              /* invalid input?               */
        errcode = DQUEERR_NOBUF;
    } else if (siz == 0) {
        errcode = DQUEERR_BUFSIZE;
    } else {
        (void) strncpy( buf, emsg[err], siz );
    }

    return (errcode);
}

/**
 *
 *  \name	dque_error_len
 *  \author	Dale Anderson
 *  \date	07/30/2019
 *  \brief	Return length of message for specified error.
 *  \version
 *  Version	Date        	Author      Comment
 *  1.1		07/30/2019  	D.Anderson  Original
 *
 *      dque_error_len(
 *      int                     err,    specified error code
 *      unsigned int           *len )   returned length of specified error
 *
 *      Returns error status and length of error message.
 *
 *      \return non-zero for failure, zero for success and length of error message.
 */

dque_err                                    /* returned error code          */
dque_error_len(                             /* return size of err message   */
dque_err        err,                        /* error code                   */
unsigned int   *len )                       /* returned length of message   */
{                                           /*------------------------------*/
    dque_err        errcode = DQUEERR_NOERR;

    if (err < 0) {                          /* it is OK to be negative      */
        err     = -err;
    }

    if (len == (unsigned int *)NULL) {      /* invalid pointer to data      */
        errcode = DQUEERR_NODATA;
    } else if (err >= DQUEERR_MAXERR) {     /* invalid error code?          */
        errcode = DQUEERR_UNKERR;
	*len    = 0;
    } else {
    	*len    = strlen( emsg[err] );
    }

    return (errcode);
}

