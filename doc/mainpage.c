
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

/* This is a dummy source file used to generate the front page documentation
 * in Doxygen. */


/**
 
@mainpage libexcept

@section Introduction


libexcept is an exceptions framework for the C programming language.

C suffers from a lack of exception handling as part of the language. The 
most common way to deal with errors, for example, is for functions to 
return a status code indicating whether the function succeeded. In large 
projects, as much as 30-50% of code may be dedicated to checking status 
code values in order to deal with errors.

libexcept implements exceptions in C, providing the same kind of 
functionality that Java and other modern high-level languages provide. 
It is licensed under the MIT license, which means that it may 
be freely used in Free Software and proprietary software without
restriction.

@subsection Features

@li 100% ANSI standard C, using the standard "setjmp" and "longjmp" 
    library calls.
@li Thread safe.
@li Exception hierarchies (subtypes).
@li Supports the finally {} keyword.
@li Assertations ala assert().
@li Self-contained, so that it may be easily imported into any project. 

@subsection Documentation

Click @link libexcept here @endlink for API documentation to libexcept.

@subsection Example

Click @link example here @endlink for a brief example of how to use
libexcept.

@subsection Limitations

There are certain @link limits limitations @endlink to what may be 
done inside an exceptions block. 

@if Sourceforge
@subsection Download
@htmlonly

libexcept can be downloaded from
<a href="http://sourceforge.net/projects/libexcept">the sourceforge
project page.</a>

@endhtmlonly
@endif

  */

