### csd5504 Giorgos Sigioltzakhs "Implementation of a "realistic" Librabry using DSA and focused on lists.

## main.c logic:
# In our main , i need to find a way to take the input from the test files (wich i are going to -r (read)). So in order to achive that i could use the case/switch statemets, but due to strings attached to the events and not characters (eg: S != PM), i went to an alternative solution: using if/else with scanf and string compare functions. Also i need to make a function that doesnt end in a string with 2 words but taking both those words as an input later, for example: G 10 "Computer Science" i need both computer and science, there fore i created the trim function at main.c

## library.c logic:
# Here we implement our events(functions), each function is analised in the .c file with comments. "//" comments are general and small and "/**/" are more explanatory. NOTE: before i started implemented the function i had to create a library initialisation function. 

## library.h logic:
# In our header file, i announce our structs, extern global variables for better linking and set our functions for our main to see. One note: in Struct genre_t i added 2 new extra fields for a more easier appronch in an event.

## Makefile logic:
# Instead of compiling by hand and write a whole gcc with a lot of flags and files, i simply created a makefile for that. Using "make" it creates a "library" executable and with "make clean" we clear our new obeject and executable files.

### Test files results:

## test1
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

## test2
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

## test3_1 (test3 was deleted)
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