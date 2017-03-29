# Implements both naïve and improved
# closure algorithms

from debug import *
from FD import *

# Naive closure algorithm
# With sigma a list of FDs and X a set of attributes
def closure_naive(sigma, X):
    closure = set(X)
    done = False
    while not done:
        done = True
        for fd in sigma:
            W,Z = fd.left,fd.right
            # <= overloads the 'issubset' operation
            if W <= closure and not (Z <= closure):
                # | overloads the 'union' operation
                closure = closure | Z
                done = False
    return closure


# Auxiliary function for the improved
# closure algorithm
# With sigma a list of FDs
# count and fd_list mutable structure to compute
def closure_improved_aux(sigma, count, fd_list): 
    for (i,fd) in enumerate(sigma):
        count[i] = len(fd.left)
        for att in fd.left:
            if not att in fd_list:
                fd_list[att] = []
            fd_list[att].append(i)
            
    if DEBUG:
        for A in fd_list:
            print('List['+A+']')
            for i in fd_list[A]:
                print('\t'+str(sigma[i]))
        print()
        for i in range(len(sigma)):
            print('count['+str(sigma[i])+'] = '+str(count[i]))
        

# Improved closure algorithm
# With sigma a list of FDs and X a set of attributes
def closure_improved(sigma, X):
    
    # Count is an array which asssociates
    # FD's ID to their left member size
    count = [0]*len(sigma)
    
    # fd_list in a dictionary that associates
    # to an attribute the list of FD's ID
    # where it appears on the left
    fd_list = {}
    
    # Initialize both count and fd_list
    closure_improved_aux(sigma, count, fd_list)
    
    closure = set(X)
    update = set(X)
    
    # To solve 4.3 bug
    for fd in sigma:
        if len(fd.left) == 0:
            closure = closure | fd.right
            update = update | fd.right
    
    while len(update) != 0:
        A = update.pop()
        if DEBUG:
            print('Current attribute:', A)
            
        if not A in fd_list:
            continue
        
        for i in fd_list[A]:
            count[i] = count[i]-1
            if count[i] == 0:
                if DEBUG:
                    print('\t',sigma[i],'becomes empty')
                Z = sigma[i].right
                # - overloads difference operation
                update = update | (Z-closure)
                closure = closure | Z
    return closure
