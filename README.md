# Kaka
Time to learn , time to code interpreter 

I felt like doing something foolish this evening , and that resulted in the below code. It is a basic interpreter ... It uses Recursive Descent Parser. It has highly limited operators , however I intend to add more and dress it up properly. But for now it works and maybe you should add your own features.

It can do basic stuff like:

?=  78 op 100     The ops are multiplication, division, addition, and subtraction.
And it can also do assignment:
?= vfoo  = 78 + 89

And you can also have variables in your expression

?=  vfoo = 28 + vfoo

But these are not possible as for now,

?= vfoo 
or
?= vfoo + 90

That is variable shouldn't start an expression. But there is a work around .

The host language is C language, and compiled with GCC. However, I see no reason why it would not work on other compilers.  If you have questions, or comments, please don't hesitate to reach out to me.
