DBDM : Closure Algorithm for Functional Dependencies
====================================================

STERIN Tristan


Content
=======

readme.txt : this file
results.tsv : raw results
plot.png : figure depicting results

/* to be completed with the list of your source files, */

All of the following has been tested with python 3.6, you also need numpy (see below).

    closure.py : core of the programs, it handles the execution of the task commanded by the shell
    closure_algorithms.py :  implements the closure algorithms (naive, improved)
    generate.py : implements the 'generate' feature (need numpy for random shuffling, generate.py:l.10)
    debug.py : describe global variable to enable debug printing, it is switch off in this release
    FD.py : describe the way we implement FDs and useful related routines.
    read_input.py : I/O routines

You'll find unit tests on the folder:
custom_examples/
    closure_naive_test1.txt : dummy associative test for naive closure
    closure_naive_test2.txt : more evoluated example for naive closure
    closure_improved_test1.txt : example given in class for improved closure, if you set 
    DEBUG to True it will show the initialization phase and the loop as in the course
    closure_improved_testNonStandard.txt : to show the bug of 4.3 and the desired behavior with non standards FDs

/* feel free to add relevant supplementary material */

Dependencies
=============

We use python (3.6.0).

We use the numpy package (in generate.py for shuffling purpose), you can install it with:
sudo pip3 install numpy

Sets are coded with the 'set' module of python which is implemented over
hash tables:

http://stackoverflow.com/questions/3949310/how-is-set-implemented

Open questions
==============

4.1 Justifications of data structures
-------------------------------------

We use sets of strings to deal with n-1 -> n in the same way as
A -> B.

For 'count' we juste use an array (python list) that associates
to each FD's ID (in the list sigma) card of W.
For 'list' we use a dictionaray (hash table) that associates to
each attributes the list of FD's ID (in the list sigma) where the attribute
appears on the left.

Array are nice and simple to work with that's why we use IDs of FDs (in sigma)
to identificate them.
The dictionary structure is flexible so that we don't have to create int ID's
for attribute but directly use the str.


4.2 Strategy for Choose A
-------------------------

We use no specific strategy, the 'pop' function of sets
will give back an arbitrary attributes.
It is kinda random strategy.

4.3 Find the bug
-------------------------

If there are non standard FDs such as '-> A', the algorithm
doesn't take into account that we have A "for free".
And for instance on the example: custom_examples/closure_improved_testNonStandard.txt
Which is:
-> A
A B -> C

closure'(B) will only output B instead of A B C.
To resolve the bug it is sufficient, when we initialize closure and update
to add in them all the attributes which are part of the right member of a
non standard FD.
It is done in closure_algorithms.py:l.65


6.1 Interestingness of generate
-------------------------------

6.2 Setup and methodology
-------------------------

We do 10 mesure per n from n=100 to n=10.000.
We have plotted on results.png several things:
    - Mean curves with std deviation errorbar for each algorithm (naïve and improved closure)
    - 2nd order polynomial for both cures
    - linear fit for the improved algorithm

    The coefficient of determination are shown for each regression.
    The exact equation annotates each regression

6.3 Analysis
------------

As expected we see that asymptically (and from the very begining) the improved
algorithm has a better complexity.

We see that for high n, the measure is very instable with the naïve approach (large deviation),
indeed the naïve approach is very sensitive to the order of the FDs.

The square fit seems very accurate for the naïve algorithm (R=0.9989)

However on this large scale of 10 000 the linear fit doesn't seem so accurate
for the improved algorithm (R^2=0.92). Whereas a square fit gives R^2 = 0.997.

I think that it is due to the inner implementation of python's structures such
as set and dict that we use "tel quel".


Additional comments
===================

We use python (3.6.0) for sake of simplicity, indeed all the operations on
sets are transparents with the 'set' embedded module.
The main drawback is that it is 10 to 100 times slower than a C++ implementation.

You can allow debug printing by setting the DEBUG variable to True (debug.py:l.5)