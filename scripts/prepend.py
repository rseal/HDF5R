#!/usr/bin/python

import os

newList = list();

#find files without copyright statement
fileList = os.popen('grep -RL "Copyright" ../include ../src')

#filter only source and header files
for i in fileList:
    if(i.find(".cpp") != -1 or i.find(".hpp") != -1 ):
        newList.append(i.strip())

f_license = open('preamble.txt')
license_str = f_license.read();
f_license.close()

for file in newList:
    f_in = open(file,'r')
    source_str = f_in.read()
    f_in.close()
    f_out = open(file,'w')
    f_out.write(license_str)
    f_out.write(source_str)
    f_out.close()


