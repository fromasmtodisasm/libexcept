
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

/** @defgroup libexcept libexcept 
 * @{ */

#ifndef LIBEXCEPT_H
#define LIBEXCEPT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <setjmp.h>

/* Enabling this checks the stack for corruption as a result of 
 * incorrect usage of the library; for example, use of break,
 * continue, goto or return to break out of the exception block.
 * However, doing so uses more CPU.  For extra speed, disable
 * this define.  It should probably be left on most of the time. */

#define EXCEPT_CHECK_STACK 1

/**
 * A type of exception.  New exception types should be defined by
 * assigning an ExceptionType variable using the EXCEPTION_TYPE
 * macro. For example:
 *
 * @code
 * ExceptionType AppleException = EXCEPTION_TYPE(BaseException);
 * ExceptionType OrangeException = EXCEPTION_TYPE(BaseException);
 * ExceptionType SatsumaException = EXCEPTION_TYPE(OrangeException);
 * @endcode
 *
 * @see BaseException
 */

typedef struct _ExceptionType ExceptionType;

/**
 * An exception.  Created using @ref exception_new.
 *
 * @see exception_new
 */

typedef struct _Exception Exception;

typedef struct _ExceptData ExceptData;
typedef struct _ExceptCatchBlock ExceptCatchBlock;

struct _ExceptionType {
    ExceptionType *parent;
};

#define EXCEPTION_TYPE(parent)  { &(parent) }

#ifdef EXCEPT_CHECK_STACK

/* Check the stack for corruption */

#define EXCEPT_LOOP_GUARD_START                                         \
    {                                                                   \
        int __except_break = 1;                                         \
        do {

#define EXCEPT_LOOP_GUARD_END                                           \
            __except_break = 0;                                         \
        } while (0);                                                    \
        if (__except_break) {                                           \
            /* break or continue was used to break out of the loop.     \
             * This is a BUG in the calling program.  Bomb out with     \
             * an error. */                                             \
            __except_bug(__FILE__, __LINE__);                           \
        }                                                               \
    }

#else

/* Stack checking disabled. */

#define EXCEPT_LOOP_GUARD_START
#define EXCEPT_LOOP_GUARD_END

#endif

/* Maximum number of catch blocks after a try {} block. */

#define EXCEPT_MAX_CATCH_BLOCKS 5

struct _ExceptCatchBlock {
    jmp_buf jmp_location;
    ExceptionType *type;
};

struct _ExceptData {
    jmp_buf try_block;
    ExceptCatchBlock catch_blocks[EXCEPT_MAX_CATCH_BLOCKS];
    jmp_buf finally_block;
    ExceptData *next;
    Exception *caught;
    unsigned int num_catch_blocks   :5;
    unsigned int rethrow            :1;
    unsigned int setup              :1;
    unsigned int have_finally       :1;
};


#define except_try                                                      \
    {                                                                   \
        EXCEPT_LOOP_GUARD_START                                         \
        ExceptData __except_data;                                       \
                                                                        \
        __except_data.caught = (void *) 0;                              \
        __except_data.rethrow = 0;                                      \
        __except_data.setup = 1;                                        \
        __except_data.have_finally = 0;                                 \
        __except_data.num_catch_blocks = 0;                             \
                                                                        \
        if (setjmp(__except_data.try_block)) {                          \
                {
            

#define except_catch(extype, var)                                       \
                }                                                       \
        }                                                               \
                                                                        \
        if (__except_data.setup) {                                      \
                ExceptCatchBlock *block;                                \
                block = &__except_data.catch_blocks[__except_data.num_catch_blocks]; \
                block->type = &(extype);                                \
                ++__except_data.num_catch_blocks;                       \
                if (setjmp(block->jmp_location)) {                      \
                        Exception *var = __except_get_current();        \
                        __except_data.caught = var;                     \
                        __except_data.num_catch_blocks = 0;

#define __except_cleanup                                                \
        __except_remove(&__except_data, __FILE__, __LINE__);            \
        /* if we caught an exception, now we have to free it.           \
         * BUT! if we caught an exception and then threw an exception   \
         * inside the catch {} block, things are complicated.           \
         * If we are rethrowing the same exception, don't free it. */   \
        if (__except_data.caught) {                                     \
                if (!__except_data.rethrow                              \
                 || (__except_data.caught != __except_get_current())) { \
                        exception_free(__except_data.caught);           \
                }                                                       \
        }                                                               \

#define except_finally                                                  \
                }                                                       \
        }                                                               \
                                                                        \
        __except_data.have_finally = 1;                                 \
                                                                        \
        if (!__except_data.setup                                        \
         || (__except_data.rethrow = setjmp(__except_data.finally_block))) { \
                {                                                       \
                        __except_cleanup
            
#define except_end                                                      \
                }                                                       \
        }                                                               \
                                                                        \
        /* if we have no finally {} block, create one to do basic       \
         * cleanup. */                                                  \
        if (!__except_data.have_finally) {                              \
                if (!__except_data.setup                                \
                 || (__except_data.rethrow = setjmp(__except_data.finally_block))) {\
                        __except_cleanup                                \
                }                                                       \
        }                                                               \
                                                                        \
        if (__except_data.setup) {                                      \
                __except_data.setup = 0;                                \
                __except_add(&__except_data, __FILE__, __LINE__);       \
                longjmp(__except_data.try_block, 1);                    \
        } else if (__except_data.rethrow) {                             \
                /* if we jumped to a finally block, then                \
                 * we must rethrow the current exception */             \
                except_throw(__except_get_current());                   \
        }                                                               \
        EXCEPT_LOOP_GUARD_END                                           \
    } 


void __except_add(ExceptData *data, char *file, int line);
void __except_remove(ExceptData *data, char *file, int line);
Exception *__except_get_current(void);
int __exception_is_a(Exception *exception, ExceptionType *type);
void __except_bug(char *file, int line);

Exception *__exception_new(ExceptionType *type, void *data, 
                           char *file, int line);

/**
 * Create a new exception.
 *
 * @param type   The type of exception to create.
 * @param data   A pointer to some extra data to include with the
 *               exception.
 * @return       A new exception object.
 */

#define exception_new(type, data)                                       \
      __exception_new(&(type), (data), __FILE__, __LINE__)

/**
 * Returns the type of an exception.  This is the type specified to 
 * @ref exception_new.
 *
 * @param exception   The exception.
 * @return            The type of the exception.
 *
 * @see exception_new
 * @see ExceptionType
 */

ExceptionType *exception_get_type(Exception *exception);

/**
 * Free an exception.  You should almost never need to call this,
 * as when an exception is caught in an except_catch block, the 
 * exception is freed automatically.
 *
 * @param exception   The exception to free.
 */

void exception_free(Exception *exception);

/**
 * Returns a pointer to extra user-specified data included with an 
 * exception.  This data is specified when the user creates the
 * exception (see @ref exception_new).
 *
 * @param exception    The exception.
 * @return             The user-specified data.
 *
 * @see exception_new
 */

void *exception_get_data(Exception *exception);

/**
 * Returns the name of the source file in which an exception was created.
 *
 * @param exception   The exception.
 * @return            A C string containing the name of the source file
 *                    in which the exception was created.
 *
 * @see exception_get_line
 */

char *exception_get_file(Exception *exception);

/**
 * Returns the line number within the source file where an exception was 
 * created.
 * 
 * @param exception   The exception.
 * @return            The line number of the line where the exception
 *                    occurred.
 *
 * @see exception_get_file
 */

int exception_get_line(Exception *exception);

/**
 * Throw an exception.  This function does not return.
 *
 * @param exception   The exception to throw.
 *
 * @see except_raise
 */

void except_throw(Exception *exception);

/**
 * Raise a new exception.  This is equivalent to:
 *
 * @code
 * except_throw(exception_new(type, data));
 * @endcode
 *
 * @param type        The type of exception to raise.
 * @param data        Extra data to include with the exception.
 */

#define except_raise(type, data)                                        \
        except_throw(exception_new((type), (data)))

#define __except_assert(condition, file, line)                          \
    if (!(condition)) {                                                 \
        except_raise(AssertException,                                   \
                     "Assertation failed: " file #line);                \
    }

/**
 * Checks a given condition, throwing an exception if it is not met.
 * The exception thrown is of type @ref AssertException.
 *
 * @param condition    The condition to test.  If this evaluates to zero
 *                     (false), an exception is thrown.
 */

#define except_assert(condition)                                        \
    __except_assert((condition), __FILE__, __LINE__)



/**
 * The root of all exception types.
 */

extern ExceptionType BaseException;

/**
 * Exception type thrown when using @ref except_assert.
 */

extern ExceptionType AssertException;

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* #ifndef LIBEXCEPT_H */

