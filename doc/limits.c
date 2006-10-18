
/*

Copyright (c) 2006, Simon Howard 
All rights reserved. 

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions are met: 

 * Redistributions of source code must retain the above copyright notice, 
   this list of conditions and the following disclaimer. 
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in the 
   documentation and/or other materials provided with the distribution. 
 * Neither the name of the libexcept project nor the names of its 
   contributors may be used to endorse or promote products derived from 
   this software without specific prior written permission. 

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE. 

 */


/* This is the "limitations" page for the documentation. */

/**
 
@page limits Limitations

Although it is possible to get a fairly functional exceptions framework using standard ANSI C, there are limitations to the system. This page documents things that cannot be done with libexcept.

@section breakout Breaking out of an exception block

It is not possible to break out of an exception block using continue, break, return or goto. libexcept has built-in guards that detect when this is done. The following example demonstrates illegal usage:

@code
    void illegal(void)
    {
        int i;

        for (i=0; i<10; ++i) {
            except_try {
                /* Illegal: cannot use break to exit out of an exception block */

                break;
            
                /* Illegal: cannot use continue to exit out of an exception block */

                continue;

                /* Illegal: cannot return from within an exception block */

                return;

                /* Illegal: do not use goto to break out of an exception block */

                goto label;
            } except_end

        label:
        }
    }
@endcode

However, the following is legal:

@code
    void legal(void)
    {
        int i;

        except_try {
            for (i=0; i<10; ++i) {
                /* Legal: Can use break to exit a loop within an exception block */

                break;
            
                /* Legal: Can use continue within an exception block */

                continue;

                /* Legal: Can use goto to jump within an exception block */

                goto label;
            }
            label:
        } except_end
    }
@endcode

Note: the same limitations also apply within catch and finally blocks.

@section nested Nested exception blocks

It is not possible to have an exception block within another exception block. The following is illegal:

@code
    void illegal(void)
    {
        except_try {
            except_try {
                printf("hello, world\n");
            } except_catch (CircleException, e) {
                printf("Circle Exception caught!\n");
            }
        } except_catch (SquareException, e) {
            printf("Square Exception caught!\n");
        }
    }
@endcode

However, the following is legal:

@code
    void legal_2(void)
    {
        except_try {
            printf("hello, world\n");
        } except_catch (CircleException, e) {
            printf("Circle Exception caught!\n");
        }
    }

    void legal_1(void)
    {
        except_try {
            legal_2();
        } except_catch (SquareException, e) {
            printf("Square Exception caught!\n");
        }
    }
@endcode

 */

