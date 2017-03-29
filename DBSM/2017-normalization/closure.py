#!/usr/bin/python3.6
import sys
import debug
debug.init()
from read_input import *
from closure_algorithms import *
from generate import *

# Help routine
def print_help():
    print('Implementation of DBDM closure, normalization and decomposition algorithms')
    print('Usage: ')
    print('./closure -naive input atts')
    print('./closure -improved input atts')
    print('./closure -generate n')
    print('./closure -normalize input')
    print('Look at the subject for more info')

# Safety check
if len(sys.argv) < 2:
    print_help()
    exit(1)

# The list of FD we are to work on
sigma = []


'''
    In the following we juste act
    accordingly with the command passed in parameter.
'''

if sys.argv[1] == '-generate':
    if len(sys.argv) < 3:
        print_help()
        exit(1)
    try:
        n = int(sys.argv[2])
    except ValueError:
        print_help()
        exit(1)
        
    generate(n)
    sys.exit(0)


# Safety check
if len(sys.argv) < 3:
    print_help()
    exit(1)

file_name = sys.argv[2]
if file_name[0] == '-':
    file_name = ''

# Filling sigma
read_input(sigma, file_name)

# Fetching attributes if any
atts = set([])
for i in range(3,len(sys.argv)):
    atts.update(sys.argv[i])

if sys.argv[1] == '-naive':
    print(set_to_str(closure_naive(sigma,atts)))
    sys.exit(0)

if sys.argv[1] == '-improved':
    print(set_to_str(closure_improved(sigma,atts)))
    sys.exit(0)