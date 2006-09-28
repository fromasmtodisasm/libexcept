
#include <stdio.h>
#include "libexcept.h"

void myfunc(void)
{
        EXCEPT_TRY
        {
                except_raise("Error!");
        }
        EXCEPT_FINALLY
        {
                printf("In finally block\n");
        }
        EXCEPT_END
}

int main(int argc, char *argv[])
{
        EXCEPT_TRY
        {
                myfunc();
        }
/*        EXCEPT_CATCH(exception_t *exception)
        {
                printf("Caught an exception!\n");
                exception_free(exception);
        }*/
        EXCEPT_END

        return 0;
}

