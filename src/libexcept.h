
#ifndef LIBEXCEPT_H
#define LIBEXCEPT_H

#include <setjmp.h>

typedef struct exception_s exception_t;
typedef struct except_data_s except_data_t;

struct except_data_s
{
    jmp_buf jmp_location;
    except_data_t *next;
};

#define EXCEPT_TRY                                                      \
    {                                                                   \
        except_data_t __except_data;                                    \
        __except_add(&__except_data);                                   \
        int __except_throwing = setjmp(__except_data.jmp_location);     \
        int __except_uncaught = 1;                                      \
        if (!__except_throwing)                                         \
        {

#define EXCEPT_CATCH(var)                                               \
        }                                                               \
        __except_remove(&__except_data);                                \
        if (__except_throwing)                                          \
        {                                                               \
            var = __except_get_current();                               \
            __except_uncaught = 0;

#define EXCEPT_FINALLY                                                  \
        }                                                               \
        __except_remove(&__except_data);                                \
        /* always do this */                                            \
        {
            
#define EXCEPT_END                                                      \
        }                                                               \
        __except_remove(&__except_data);                                \
        if (__except_throwing && __except_uncaught)                     \
        {                                                               \
            /* rethrow the exception */                                 \
            except_throw(__except_get_current());                       \
        }                                                               \
    }

void __except_add(except_data_t *data);
void __except_remove(except_data_t *data);
exception_t *__except_get_current(void);

exception_t *__exception_new(void *data, char *file, char *line);
#define exception_new(data)   __exception_new((data), __FILE__, __LINE__)
void exception_free(exception_t *exception);
void except_throw(exception_t *exception);
#define except_raise(data)    except_throw(exception_new(data))

#endif /* #ifndef LIBEXCEPT_H */

