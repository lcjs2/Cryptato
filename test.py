#!/usr/bin/python
import os
for i in range(100):
	print "Testing 2**",i
	print 2**i
	os.system('./cryptato '+str(i))
