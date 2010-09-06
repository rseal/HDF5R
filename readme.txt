HDF5R Project 
Date: September o6, 2010
Version: 1.0.0-SEP-2010
Author: Ryan Seal

Introduction:

This project contains a small, very specific wrapper around the c++
implementation of the HDF5 library for radar data collection. A custom data
type is designed to collect raw IQ samples. Additionally, a number of
functions exist to greatly simply attribute creation. This implementation is
capable of reading and writing family files only ( at the moment ). 

Installation:

1. Use "scons install-headers" as root to install headers in
/usr/local/include/HDF5.

Dependencies:

1. CommandLineParser library required to build examples. 
   ( http://github.com/rseal/CommandLineParser ).
2. Latest scons ( http://www.scons.org ).

