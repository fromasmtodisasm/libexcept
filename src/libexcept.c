
/*

Copyright (C) 2006, Simon Howard 

Permission is hereby granted, free of charge, to any person obtaining a 
copy of this software and associated documentation files (the "Software"), 
to deal in the Software without restriction, including without limitation 
the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the 
Software is furnished to do so, subject to the following conditions: 

The above copyright notice and this permission notice shall be included in 
all copies or substantial portions of the Software. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL 
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
DEALINGS IN THE SOFTWARE. 

*/


#include <stdio.h>
#include <stdlib.h>

#include "libexcept.h"

/* Work out how to get thread-local storage. */

#if defined(__GNUC__)

#define TLS_PREFIX __thread

#elif defined(_MSC_VER)

#define TLS_PREFIX __declspec(thread)

#else

#warning Unknown platform - dont know how to use thread-local storage.  \
         exceptions in multithreaded programs may break.
#define TLS_PREFIX

#endif

typedef enum {
    STACK_UNKNOWN, 
    STACK_DOWNWARD,       /* Stack grows downward, ie. toward 0 */
    STACK_UPWARD,         /* Stack grows upward, ie. away from 0 */
} stack_direction_t;

struct _Exception {
    ExceptionType *type;
    void *user_data;
    char *file;
    int line;
};

/* Base exception type that all inherit from */

ExceptionType BaseException = { NULL };

/* Exception type thrown by except_assert() */

ExceptionType AssertException = EXCEPTION_TYPE(BaseException);

static TLS_PREFIX ExceptData *except_chain_head = NULL;
static TLS_PREFIX Exception *current_exception = NULL;

#ifdef EXCEPT_CHECK_STACK

static stack_direction_t stack_direction = STACK_UNKNOWN;

static stack_direction_t find_stack_direction_2(int *parent_var)
{
    int variable;

    variable = 5;

    if (&variable > parent_var)
    {
        return STACK_UPWARD;
    }
    else if (&variable < parent_var)
    {
        return STACK_DOWNWARD;
    }
    else
    {
        /* This should never happen, except maybe due to some 
         * strange compiler optimisations. */

        fprintf(stderr, "Unable to determine stack direction!\n");
        exit(-1);

        return STACK_UNKNOWN;
    }
}

static stack_direction_t find_stack_direction(void)
{
    int variable;

    variable = 1;

    return find_stack_direction_2(&variable);
}

/* Called when a bug is detected in the calling program - either because
 * break or continue was used, or because the programmer screwed up the
 * stack by using return. */

void __except_bug(char *file, int line)
{
    fprintf(stderr, "%s:%i: BUG detected: Check that you are not using "
                    "break, continue, return or goto from within an "
                    "exception block.\n", file, line);

    exit(-1);
}

#endif

void __except_add(ExceptData *data, char *file, int line)
{
#ifdef EXCEPT_CHECK_STACK

    /* Find the direction the stack grows, if we have not done
     * so already. */

    if (stack_direction == STACK_UNKNOWN)
    {
        stack_direction = find_stack_direction();
    }

    /* Check that objects are being added into the chain in stack order.
     * If not, there is probably a bug in the calling program. */

    if (except_chain_head != NULL)
    {
        if ((stack_direction == STACK_UPWARD 
             && data <= except_chain_head)
         || (stack_direction == STACK_DOWNWARD
             && data >= except_chain_head))
        {
            __except_bug(file, line);
        }
    }
#endif
    
    /* Add to the linked list. */

    data->next = except_chain_head;
    except_chain_head = data;
}

void __except_remove(ExceptData *data, char *file, int line)
{
    if (data != except_chain_head) {
        __except_bug(file, line);
    }

    except_chain_head = except_chain_head->next;
}

Exception *__exception_new(ExceptionType *type, void *data, 
                           char *file, int line)
{
    Exception *exception;

    exception = (Exception *) malloc(sizeof(Exception));
    exception->type = type;
    exception->user_data = data;
    exception->file = file;
    exception->line = line;

    return exception;
}

ExceptionType *exception_get_type(Exception *exception)
{
    return exception->type;
}

void *exception_get_data(Exception *exception)
{
    return exception->user_data;
}

char *exception_get_file(Exception *exception)
{
    return exception->file;
}

int exception_get_line(Exception *exception)
{
    return exception->line;
}

void exception_free(Exception *exception)
{
    free(exception);
}

Exception *__except_get_current(void)
{
    return current_exception;
}

int __exception_is_a(Exception *exception, ExceptionType *type)
{
    ExceptionType *test_type;

    for (test_type = exception->type; 
         test_type != NULL; 
         test_type = test_type->parent) {
        if (test_type == type) {
            return 1;
        }
    }

    return 0;
}

void except_throw(Exception *exception)
{
    int i;

    /* Jump to the first in the chain */

    if (except_chain_head == NULL) {
        /* Nothing is catching this exception.  Bomb out with an error. */

        fprintf(stderr, "ERROR: Uncaught exception at %s:%i\n",
                exception->file, exception->line);
        exit(-1);
    }

    current_exception = exception;

    /* Check all of the catch {} blocks in the head to see if one applies
     * to our exception. */

    for (i=0; i<except_chain_head->num_catch_blocks; ++i) {
         if (__exception_is_a(exception,
                              except_chain_head->catch_blocks[i].type)) {
             /* This catch block catches this kind of exception. */

             longjmp(except_chain_head->catch_blocks[i].jmp_location, 1);
         }
    }

    /* None of the catch {} blocks matched.  We must still jump to the
     * finally {} block to clean up.  The macros make sure that a 
     * finally {} block is always created. */

    longjmp(except_chain_head->finally_block, 1);
}

