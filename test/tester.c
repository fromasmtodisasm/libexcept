
#include <stdio.h>
#include "libexcept.h"

ExceptionType OrangeException = EXCEPTION_TYPE(BaseException);
ExceptionType SatsumaException = EXCEPTION_TYPE(OrangeException);
ExceptionType AppleException = EXCEPTION_TYPE(BaseException);

void myfunc(void)
{
    except_try {
        except_raise(SatsumaException, "Error!");
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
    } except_catch(AppleException, exception) {
        printf("Caught a AppleException!\n");
    } except_catch(OrangeException, exception) {
        printf("Caught a OrangeException!\n");
    } except_finally {
        printf("main finally\n");
    } except_end

    except_try {
        except_assert(1 == 0);
    } except_catch(OrangeException, exception) {
        printf("Caught a OrangeException!\n");
    } except_catch(AssertException, exception) {
        printf("Caught an assert failure\n");
    } except_end

    return 0;
}

