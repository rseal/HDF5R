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
