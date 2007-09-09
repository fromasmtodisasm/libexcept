
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

