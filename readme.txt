HDF5R Project 
Date: March 23, 2014
Version: 1.0.0-SEP-2010
Author: Ryan Seal

Introduction:

This project contains a small, very specific wrapper around the c++
implementation of the HDF5 library for radar data collection. A custom data
type is designed to collect raw IQ samples. Additionally, a number of
functions exist to greatly simply attribute creation. This implementation is
capable of reading and writing family files only ( at the moment ). 

Installation:

1. The library is header-only, just include the headers in your project. 
2. To run the example test:
      a. waf configure build
      b. cd build
      c. ./write (writes out hdf5 file)
      d. ./read (reads in newly written file and prints info to console)


Dependencies:

1. HDF5 with c++ bindings enabled.
2. WAF build system.
