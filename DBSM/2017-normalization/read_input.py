import sys
from debug import *
from FD import *

# Check if an input row is a comment
def is_comment(row):
    return len(row.split('#')) > 1
    

# Input from a file or stdin (if file_name = '')
def read_input(sigma, file_name = ''):
    if file_name != '':
        f = open(file_name,"r")
    else:
        f = sys.stdin
        
    raw_data = f.read()
    if f is not sys.stdin:
        f.close()
    
    # Split by lines
    data = raw_data.split('\n')
    # Remove empty lines
    data = list(filter(lambda x: x != '', data))
    
    for row in data:
        if not is_comment(row):
            arrow_split = row.split('->')
            
            # At this point only data of the form
            # A B C -> D E
            assert(len(arrow_split) == 2)
            
            left_side = arrow_split[0]
            right_side = arrow_split[1]
            
            # Get nice lists
            # e.g: A B C -> D E becomes
            # ['A','B','C'] and ['D','E']
            left_split = list(filter(lambda x: x != '', left_side.split(' ')))
            right_split = list(filter(lambda x: x != '', right_side.split(' ')))
            
            # Constructing the associated FD
            FD_to_add = FD(left_split,right_split)
            
            if DEBUG:
                print('Adding the FD:',FD_to_add, 'to Sigma')
            
            # Add the FD to sigma
            sigma.append(FD(left_split,right_split))    