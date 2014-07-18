#!/usr/bin/python

from random import random, seed

with open('arreglos.h', 'w') as f:
  seed(0)
  f.write('double b[SIZE]={')
  for i in range(100000):
    f.write('{0}'.format(random()))
    if i < 99999:
      f.write(',')
    if (i + 1) % 1000 == 0:
      print >> f
  f.write('};\n')
  f.write('double c[SIZE]={')
  for i in range(100000):
    f.write('{0}'.format(random()))
    if i < 99999:
      f.write(',')
    if (i + 1) % 1000 == 0:
      print >> f
  f.write('};\n')

