// Copyright (c) 2010 Ryan Seal <rlseal -at- gmail.com>
//
// This file is part of HDF5 for Radar (HDF5R) Software.
//
// HDF5R is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//  
// HDF5R is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with HDF5R.  If not, see <http://www.gnu.org/licenses/>.
#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <H5Cpp.h>
#include <boost/cstdint.hpp>

//create a 16-bit complex data type and typedef 
struct ComplexData{
   int16_t real;
   int16_t imag;
};
typedef ComplexData complex_t;

//create an HDF5 complex DataType based on the compound type CompType
struct ComplexHDF5{

   H5::CompType compType_;

   public:

   ComplexHDF5(): compType_(sizeof(complex_t)) {
      compType_.insertMember("real", offsetof(complex_t,real), H5::PredType::NATIVE_INT16);
      compType_.insertMember("imag", offsetof(complex_t,imag), H5::PredType::NATIVE_INT16);
   }

   //call this member when passing DataType information to the HDF5 system
   const H5::CompType& GetRef() { return compType_;}

};

#endif
