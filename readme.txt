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

1. Use "waf install_headers" as root to install headers in /usr/local/include/hdf5r.
2. Run the following to get local deps:
      a. git submodule init
      b. git submodule update
      c. cd into deps/clp, login as root, and run "scons install-headers"
      d. go back to the root project and type "scons"


Dependencies:

1. HDF5 with c++ bindings enabled.
2. WAF build system.
3. CommandLineParser library to test examples.
