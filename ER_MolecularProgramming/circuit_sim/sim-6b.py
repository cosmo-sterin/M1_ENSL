#!/usr/bin/python

import sys, getopt


def help():
   print('sim-6b.py -i <inputfile> -s <initial_state>')
   sys.exit()


def parse_6b_circuit(filename):
   raw_data = open(filename, "r").read()
   raw_data = list(filter(lambda x: not(x == '' or x[0] == '%'),raw_data.split("\n")))
   circuit = {1:{},2:{},3:{},4:{},5:{},6:{},7:{}}

   for (i,row) in enumerate(raw_data):
      if i == 0 or i == 6:
         circuit[i+1]['0'] = row[0]
         circuit[i+1]['1'] = row[1]
      else:
         circuit[i+1]['00'] = row[0]+row[1]
         circuit[i+1]['01'] = row[2]+row[3]
         circuit[i+1]['10'] = row[4]+row[5]
         circuit[i+1]['11'] = row[6]+row[7]
   print(circuit)
   return circuit

# f is the 6b function computed
# from the circuit
def f(circuit,x):
   input_2_1 = circuit[1][x[0]]
   input_2_2 = circuit[3][x[1]+x[2]][0]
   
   input_4_1 = circuit[3][x[1]+x[2]][1]
   input_4_2 = circuit[5][x[3]+x[4]][0]

   input_6_1 = circuit[5][x[3]+x[4]][1]
   input_6_2 = circuit[7][x[5]]
   #print(input_2_1)
   #print(input_2_2)
   #print(input_4_1)
   #print(input_4_2)
   #print(input_6_1)
   #print(input_6_2)
   return circuit[2][input_2_1+input_2_2]+circuit[4][input_4_1+input_4_2]+circuit[6][input_6_1+input_6_2]





def main(argv):
   if len(argv) < 5:
      help()

   if argv[1] != '-i':
      help()

   inputfile = argv[2]

   if argv[3] != '-s':
      help()

   init_state = argv[4]
   if len(init_state) != 6:
      print("Initial state should be coded on 6bits")
      sys.exit()

   print('Input file is "', inputfile)
   print('Initial state is "', init_state)
   circuit = parse_6b_circuit(inputfile)


   curr_state = init_state
   k = 0
   seen = {}
   while not curr_state in seen:
      print("s"+str(k)+" "+curr_state)
      seen[curr_state] = k
      curr_state = f(circuit,curr_state)
      k+=1
   print("s"+str(k)+" "+curr_state+" (cf s"+str(seen[curr_state])+")")

if __name__ == "__main__":
   main(sys.argv)