# This file implements FD representation 
# and routines.

# A routine for printing purpose
def set_to_str(s):
    sorted_set = sorted(list(s))
    
    return ' '.join(sorted_set)

# A FD consists in two sets of attributes
# 'left -> right'
class FD:
    def __init__(self, left_p = [], right_p = []):
        # For manipulation purpose we stock them as str
        # (to deal with 0 -> 1 in the same way as A -> B)
        self.left = set(map(str,left_p))
        self.right = set(map(str,right_p))
    
    def __repr__(self):
        return set_to_str(self.left)+' -> '+set_to_str(self.right)