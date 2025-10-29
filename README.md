# CSD5504 — Giorgos Sigioltzakhs
## Implementation of a "realistic" Library using DSA and focused on lists.

## main.c logic:
In our main, I need to find a way to take the input from the test files (which I am going to -r (read)). 
So in order to achieve that, I could use the case/switch statements, but due to strings attached to the events 
and not characters (eg: S != PM), I went to an alternative solution: using if/else with scanf and string compare 
functions. Also, I need to make a function that doesn’t end in a string with 2 words but takes both those words as 
input later. For example: G 10 "Computer Science" — I need both computer and science, therefore I created the trim 
function in main.c.

## library.c logic:
Here we implement our events (functions). Each function is analyzed in the .c file with comments. 
"//" comments are general and small and "/**/" are more explanatory. 
NOTE: before I started implementing the functions, I had to create a library initialization function.

## library.h logic:
In our header file, I announce our structs, extern global variables for better linking, 
and set our functions for our main to see. 
One note: in Struct genre_t I added 2 new extra fields for an easier approach in an event.

## Makefile logic:
Instead of compiling by hand and writing a whole gcc command with a lot of flags and files, 
I simply created a Makefile for that. Using "make" it creates a "library" executable and with 
"make clean" we clear our new object and executable files.

## Test files results:

## test1
```
<S>     Done
<G>     DONE
<G>     DONE
<BK>    DONE
<BK>    DONE
<BK>    DONE
<M>     DONE
<M>     DONE
<L>     DONE
<L>     DONE
<R>     DONE
<R>     DONE
Genre <10>: <Computer Science>
      <101>, <8>
      <102>, <0>
Genre <20>: <History>
      <201>, <9>
<D>     DONE
Display:
    <10>:
    <101>, <8>
    <102>, <0>
    <20>:
    <201>, <9>
Loans:
Loans:
```
## test2
```
<S>     Done
<G>     DONE
<BK>    DONE
<M>     DONE
<L>     DONE
IGNORED
<R>     DONE
IGNORED
<L>     DONE
<R>     DONE
<R>     DONE
Genre <10>: <CS>
      <101>, <0>
<D>     DONE
Display:
    (empty)
IGNORED
IGNORED
IGNORED
IGNORED
IGNORED
Loans:
```
## test3_1 (test3 was deleted)
```
<S>     Done
<G>     DONE
<G>     DONE
<BK>    DONE
<BK>    DONE
<BK>    DONE
<M>     DONE
<M>     DONE
<L>     DONE
<R>     DONE
<L>     DONE
<R>     DONE
<L>     DONE
Genre <10>: <HEROES>
      <102>, <9>
      <101>, <8>
Genre <20>: <comedy>
      <201>, <0>
Loans:
Loans:
    <201>
<D>     DONE
Display:
    <10>:
    <102>, <9>
    <101>, <8>
```