
#ifndef LIBEXCEPT_H
#define LIBEXCEPT_H

#include <setjmp.h>
 
typedef struct _ExceptionType ExceptionType;
typedef struct _Exception Exception;
typedef struct _ExceptData ExceptData;

struct _ExceptData {
    jmp_buf jmp_location;
    ExceptData *next;
};

struct _ExceptionType {
    ExceptionType *parent;
};

#define EXCEPTION_TYPE(parent)  { &(parent) }

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

#define except_try                                                      \
    {                                                                   \
        EXCEPT_LOOP_GUARD_START                                         \
        ExceptData __except_data;                                       \
        int __except_throwing;                                          \
        int __except_caught = 0;                                        \
        Exception *__except_exception = 0;                              \
        __except_add(&__except_data, __FILE__, __LINE__);               \
        __except_throwing = setjmp(__except_data.jmp_location);         \
        if (__except_throwing) {                                        \
            __except_exception = __except_get_current();                \
        } else {

#define except_catch(type, var)                                         \
        }                                                               \
        __except_remove(&__except_data);                                \
        if (__except_throwing && !__except_caught                       \
         && __exception_is_a(__except_exception, &(type))) {            \
            Exception *(var);                                           \
            (var) = __except_exception;                                 \
            __except_caught = 1;

#define except_finally                                                  \
        }                                                               \
        __except_remove(&__except_data);                                \
        /* always do this */                                            \
        {
            
#define except_end                                                      \
        }                                                               \
        __except_remove(&__except_data);                                \
        if (__except_caught) {                                          \
            /* we caught the exception; now free it */                  \
            __exception_free(__except_exception);                       \
        } else if (__except_throwing) {                                 \
            /* exception was not caught; rethrow it */                  \
            except_throw(__except_exception);                           \
        }                                                               \
        EXCEPT_LOOP_GUARD_END                                           \
    } 


#define __except_assert(condition, file, line)                          \
    if (!(condition)) {                                                 \
        except_raise(AssertException,                                   \
                     "Assertation failed: " file #line);                \
    }

#define except_assert(condition)                                        \
    __except_assert((condition), __FILE__, __LINE__)

void __except_add(ExceptData *data, char *file, int line);
void __except_remove(ExceptData *data);
Exception *__except_get_current(void);
void __exception_free(Exception *exception);
int __exception_is_a(Exception *exception, ExceptionType *type);
void __except_bug(char *file, int line);

Exception *__exception_new(ExceptionType *type, void *data, 
                           char *file, int line);
#define exception_new(type, data)                                       \
      __exception_new(&(type), (data), __FILE__, __LINE__)
ExceptionType *exception_get_type(Exception *exception);
void *exception_get_data(Exception *exception);
char *exception_get_file(Exception *exception);
int exception_get_line(Exception *exception);

void except_throw(Exception *exception);
#define except_raise(type, data)                                        \
        except_throw(exception_new((type), (data)))

extern ExceptionType BaseException;
extern ExceptionType AssertException;

#endif /* #ifndef LIBEXCEPT_H */

