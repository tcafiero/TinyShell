/*
 * ===============================================================
 *  Natural Tiny Shell (NT-Shell) Application example.
 *  Version 0.0.6
 * ===============================================================
 * Copyright (c) 2010-2011 Shinichiro Nakamura
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * ===============================================================
 */

#include "cmd.h"
#include <stdio.h>

int sum(int add1,int add2)
{
	return add1+add2;
}

void cmd_foo(int argc, char **argv)
{
    printf("This is %s\n", __func__);
}

void cmd_bar(int argc, char **argv)
{
    printf("This is %s\n", __func__);
}

void cmd_baz(int argc, char **argv)
{
    printf("This is %s\n", __func__);
}

void cmd_sum(int argc, char **argv)
{
	int add1,add2,result;
	sscanf(argv[1],"(%d,%d)",&add1,&add2);
	result=sum(add1,add2);
	printf("sum(%d,%d)=%d\n", add1,add2,result);
}

