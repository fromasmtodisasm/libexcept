
#include <stdio.h>
#include "libexcept.h"

ExceptionType ShoeException = EXCEPTION_TYPE(BaseException);
ExceptionType TimberlandException = EXCEPTION_TYPE(ShoeException);
ExceptionType MonkeyException = EXCEPTION_TYPE(BaseException);

void myfunc(void)
{
    except_try {
        except_raise(TimberlandException, "Error!");
    } except_finally {
        printf("In finally block\n");
    } except_end
}

int main(int argc, char *argv[])
{
    int i;

    except_try {
        for (i=0; i<10; ++i) {
            myfunc();
        }
    } except_catch(MonkeyException, exception) {
        printf("Caught a MonkeyException!\n");
    } except_catch(ShoeException, exception) {
        printf("Caught a ShoeException!\n");
    } except_finally {
        printf("main finally\n");
    } except_end

    except_try {
        except_assert(1 == 0);
    } except_catch(ShoeException, exception) {
        printf("Caught a ShoeException!\n");
    } except_catch(AssertException, exception) {
        printf("Caught an assert failure\n");
    } except_end

    return 0;
}

