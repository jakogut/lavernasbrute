# Indentation & Whitespace #

Each tab is the same length as four spaces.

The ANSI indent style is used in Laverna's Brute. Each brace, regardless of whether it is a starting or closing brace, is put on it's own line, and the braces for every single block of code are aligned for clarity.

Here is an example:

```
if(foo)
{
    do
    {
        bar();

    } while(baz);
}
```

Use spaces in between operators and operands to maintain clarity.

Incorrect:
```
a+=b*(c-d);
```

Correct:
```
a += b * (c - d);
```

If you have an especially long function header or flow control condition, wrap it around to the next line.

```
void someFunction(int data1, bool isTrue, string inputText, 
    unsigned long long iterations)
{
    //Do something
}
```

If you use `vim` for editing files you may find [this script](http://code.google.com/p/lavernasbrute/source/browse/trunk/tools/scripts/lavernasbrute.vim) useful.

# Comments #
Comment your code well, but don't overdue it. (Please don't use comments on parts of your code where the functionality is blatantly obvious, it's patronizing, and makes me want to eat you.) Make your comments legible and use proper grammar to the best of your ability. "Silly" comments and easter eggs are welcome, but don't overdue it, don't use them where they are completely irrelevant to the code, and don't use bad spelling and grammar.

Comments belonging on a single line should use the double slash, '//', with no space in between the comment declaration and the comment text.

If the comment is long enough to wrap around to another line, use the comment block declaration. Separate the declaration and comment text with a single space.

```
//This is a short comment.

/* This is a very long comment. It will wrap around most text editors, which means it
should be declared as a block, rather than a single line. */
```

# Object Naming #

Variable names aren't that complicated. Name variables in a way that explicitly and concisely reveals their purpose. Any single-part variable is all lowercase. Any variable consisting of more than one part starts with a lowercase character, and subsequent parts start with an uppercase letter. (camelCase)

For example, these are all proper variable declarations:

```
//Group variable declarations of the same type with a similar purpose on the same line
int hello, helloWorld;

bool isThisTrue;
string namingConventions;
```

Header/source files are named in the same way as variables, but they always start with a capital letter. Class/struct definitions are named similar to variables, with the sole exception that single-part class/struct names should begin with a capital. Class/struct names should have the same or similar name as the header containing them.

```
struct someData; //...

class Networking; //...
```

# Header Files #

## Include Guards ##
My standard for writing include guards is the name of the header file in all capital letters, followed by an underscore, the letter 'H', and another underscore.

For example, the header "MyHeader.h" would have guards like so:

```
#ifndef MYHEADER_H_
#define MYHEADER_H_

//Header code here

#endif
```

# Class Creation #

It is preferred that blocks of code with similar functionality be put into their own class or classes, and that each class be declared in a header, and defined in a separate source file. (Though this is not required. A single class may be declared and defined in a single header, if necessary.)

Here is an example:


MyClass.h
```
#ifndef MYCLASS_H_
#define MYCLASS_H_

class myClass
{
    myClass();
    ~myClass();

public:

    //Public method
    void foo();

    //Public accessor methods
    int getFoobar();
    void setFoobar(int write);

//Use the protected keyword unless private is absolutely necessary
protected:

    //Protected function
    bar();

    //Protected variables
    int foobar;
};

#endif
```

MyClass.cpp
```
#include MyClass.h

myClass::myClass()
{
    foobar = 0;
}

myClass::~myClass()
{
}

void myClass::foo()
{
    bar();
}

void myClass::bar()
{
    //Do something
}

void myClass::setFoobar(int write)
{
    foobar = write;
}

int myClass::getFoobar()
{
    return foobar;
}
```

# Optimization #

Speed is critical in Laverna's brute, therefore, optimization is a necessary evil. Optimize your code, but make sure it does what is needed, and works without flaw before doing so. Premature optimization will cause you endless grief if your code is not ready for it. Make sure your optimizations don't compromise the stability of the application, or the clarity of your code. Only inline functions which are fewer than five lines in length.