
#include <stdio.h>
#include <stdlib.h>

#include "libexcept.h"

struct exception_s
{
    void *user_data;
    char *file;
    char *line;
};

static except_data_t *except_chain_head = NULL;
static exception_t *current_exception = NULL;

void __except_add(except_data_t *data)
{
    data->next = except_chain_head;
    except_chain_head = data;
}

void __except_remove(except_data_t *data)
{
    if (data == except_chain_head)
    {
        except_chain_head = except_chain_head->next;
    }
}

exception_t *__exception_new(void *data, char *file, char *line)
{
    exception_t *exception;

    exception = (exception_t *) malloc(sizeof(exception_t));
    exception->user_data = data;
    exception->file = file;
    exception->line = line;

    return exception;
}

void exception_free(exception_t *exception)
{
    free(exception);
}

void except_throw(exception_t *exception)
{
    /* Jump to the first in the chain */

    if (except_chain_head == NULL)
    {
        /* Nothing is catching this exception.  Bomb out with an error. */

        fprintf(stderr, "ERROR: Uncaught exception at %s, %s\n",
                exception->file, exception->line);
        exit(-1);
    }

    current_exception = exception;

    /* Call the first in the chain */

    longjmp(except_chain_head->jmp_location, 1);
}

exception_t *__except_get_current(void)
{
        return current_exception;
}

