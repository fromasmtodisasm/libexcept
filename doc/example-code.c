
/* This is a dummy source file containing the "example code" page */

/**
 
@page example Example code

The following is some example code demonstrating how libexcept is
used:

@code
    #include <stdio.h>
    #include <stdlib.h>
    #include <libexcept.h>

    /* Exception hierarchy: 
         RectangleException, CircleException extend ShapeException
         SquareException extends RectangleException 
     */

    ExceptionType ShapeException = EXCEPTION_TYPE(BaseException);
    ExceptionType RectangleException = EXCEPTION_TYPE(ShapeException);
    ExceptionType SquareException = EXCEPTION_TYPE(RectangleException);
    ExceptionType CircleException = EXCEPTION_TYPE(ShapeException);

    void test(void)
    {
        except_try {
            except_raise(SquareException, NULL);
            except_assert(1 == 2);
        } except_finally {
            printf("Cleaning up at end of test function!\n");
        } except_end
    }

    int main(int argc, char *argv[])
    {
        except_try {
            test();
        } except_catch(RectangleException, e) {
            printf("Caught a RectangleException!\n");
        } except_end
    }
@endcode

  */

