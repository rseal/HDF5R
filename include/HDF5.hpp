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
#ifndef HDF5_HPP
#define HDF5_HPP

#include<H5Cpp.h>
#include<iostream>
#include<boost/shared_ptr.hpp>
#include<stdexcept>
#include<vector>

namespace hdf5{
   enum{ READ=0,WRITE=1};
   const static std::string FILE_EXT = "h5";
};

struct HDF5{

   typedef boost::shared_ptr<H5::H5File> H5FilePtr;
   H5FilePtr file_;
   H5::DataSet dSet_;
   H5::DataType dType_;
   H5::DataSpace dSpace_;
   const hsize_t fileSize_;
   int writeCount_;
   const std::string tStr_;
   int numTables_;
   int flags_;

   std::string h5FileName_;
   H5::FileAccPropList fapl_;

   const std::string Num2Table(const int& tNum){

      //ensure that requested table is available - throw if not.
      if(tNum > numTables_)
         throw std::runtime_error("Requested table number exceeds number "
               "of available tables\n");

      //c-style int-to-string conversion to access table number
      char num[6];
      snprintf(num, 6, "T%d", tNum);

      return std::string(num);
   }

   public:

   HDF5( const std::string& fileName, const unsigned int& flags): 
      fileSize_(1<<30), writeCount_(0), tStr_("Table"), numTables_(0), 
      flags_(flags){

      //strip file extension if present
      h5FileName_ = fileName;
      h5FileName_ = h5FileName_.substr(0,fileName.find("."));

      //add file indexing - required for family VFD
      h5FileName_ += "." + hdf5::FILE_EXT;

      //create property list and set family VFD
      fapl_ = H5::FileAccPropList::DEFAULT;

      //bad HDF5 design - must use shared_ptr to initialize H5File properly
      file_ = H5FilePtr(  
            new H5::H5File( 
               h5FileName_, 
               (flags == hdf5::READ ? H5F_ACC_RDONLY : H5F_ACC_EXCL),
               H5::FileCreatPropList::DEFAULT, 
               fapl_
               ));

      if(flags == hdf5::READ){
         //retrieve the number of tables available
         numTables_ = file_->getNumObjs();
      }
   }

   const int& NumTables() { return numTables_;}

   void Description(const std::string& comment){
      file_->setComment("/",comment);
   }

   const std::string Description() { return file_->getComment("/");}

   const std::vector<hsize_t> TableDims(){

      if(flags_ != hdf5::READ) 
         throw std::runtime_error("TableDims() is only valid in READ mode");

      H5::DataSet dSet = file_->openDataSet("T0");
      H5::DataSpace dSpace = dSet.getSpace();

      std::vector<hsize_t> dims(dSpace.getSimpleExtentNdims());
      dSpace.getSimpleExtentDims(&dims[0]);

      return dims;
   }

   template<typename T>
      void ReadTable(const int& tableNum, void* buf, 
            const H5::DataType& dType){

         std::string tNum = Num2Table(tableNum);
         dSet_ = file_->openDataSet(tNum);
         dSet_.read( buf, dType);
      }

   void CreateTable(const H5::DataType& dType, const H5::DataSpace& dSpace, 
         const H5::DSetCreatPropList pList = H5::DSetCreatPropList::DEFAULT){

      //store private copies of dataspace and datatype for other members
      dType_ = dType;
      dSpace_ = dSpace;

      //c-style int-to-string conversion to track table numbers
      char name[10];
      snprintf(name, 10, "T%d", writeCount_++);

      //create a new data set - call WriteTable() to put data in it.
      dSet_ = file_->createDataSet(name,dType_,dSpace_,pList);
   }

   void WriteTable(void* data){
      //write data and close data set
      dSet_.write(data, dType_, dSpace_);
      dSet_.close();
   }

   template<typename T>
      void WriteTAttrib(const std::string& name, const T& value, 
            const H5::DataType& dType, const H5::DataSpace& dSpace){
         //attributes are clunky in HDF5++ implementation - this is a workaround
         //template is required to pass the proper value type

         //create a new data set attribute
         H5::Attribute attrib = dSet_.createAttribute(name,dType,dSpace);

         //write the value to the attribute and close
         attrib.write(dType,&value);
         attrib.close();
      }

   void WriteTStrAttrib(const std::string& name, const std::string& value){
      //HDF5 has a mangled way of creating string attributes - 
      //this is a workaround

      //create a string array of length 1 containing the value to be written
      std::string strBuf[1] = {value};
      //define the data type as a string with the value's length
      H5::StrType strType(H5::PredType::C_S1,256);

      //open the data set and create a new attribute of type string
      H5::Attribute attrib = dSet_.createAttribute(name,
            strType,H5::DataSpace());

      //write value to the attribute and close
      attrib.write(strType,strBuf[0]);
      attrib.close();
   }

   template<typename T>
      void ReadTAttrib(const int& tableNum, const std::string& name, 
            T& value, const H5::DataType& dType){
         //attributes are clunky in HDF5++ implementation - this is a workaround
         //template is required to pass the proper value type
         
         std::string tNum = Num2Table(tableNum);

         //open data set and read attribute "name"
         dSet_ = file_->openDataSet(tNum);
         H5::Attribute attrib = dSet_.openAttribute(name);

         //write the value to the attribute and close
         attrib.read(dType,reinterpret_cast<void*>(&value));
         attrib.close();
      }

   const std::string ReadTStrAttrib(const int& tableNum, 
         const std::string& name){

      //attributes are clunky in HDF5++ implementation - this is a workaround
      //template is required to pass the proper value type

      std::string tNum = Num2Table(tableNum);

      std::string value;
      H5::StrType strType(H5::PredType::C_S1,256);

      dSet_ = file_->openDataSet(tNum);
      H5::Attribute attrib = dSet_.openAttribute(name);

      //write the value to the attribute and close
      attrib.read(strType,value);
      attrib.close();

      return value;
   }

   template<typename T>
      void WriteAttrib(const std::string& name, const T& value, 
            const H5::DataType& dType, const H5::DataSpace& dSpace){

         //attributes are clunky in HDF5++ implementation - this is a workaround
         //template is required to pass the proper value type

         //access the built-in root group and create a new attribute for it.
         H5::Group rootGroup = file_->openGroup("/");
         H5::Attribute attrib = rootGroup.createAttribute(name,dType,dSpace);

         //write the value to the attribute and close
         attrib.write(dType,&value);
         attrib.close();
      }

   void WriteStrAttrib(const std::string& name, const std::string& value){
      //HDF5 has a mangled way of creating string attributes - 
      //this is a workaround

      //create a string array of length 1 containing the value to be written
      std::string strBuf[1] = {value};
      //define the data type as a string with the value's length
      H5::StrType strType(H5::PredType::C_S1,256);

      //open the root group and create a new attribute of type string
      H5::Group rootGroup = file_->openGroup("/");
      H5::Attribute attrib = rootGroup.createAttribute(name,
            strType,H5::DataSpace());

      //write value to the attribute and close
      attrib.write(strType,strBuf[0]);
      attrib.close();
   }

   template<typename T>
      void ReadAttrib(const std::string& name, T& value, 
            const H5::DataType& dType){
         //attributes are clunky in HDF5++ implementation - this is a workaround
         //template is required to pass the proper value type

         //access the built-in root group and create a new attribute for it.
         H5::Group rootGroup = file_->openGroup("/");
         H5::Attribute attrib = rootGroup.openAttribute(name);

         //write the value to the attribute and close
         attrib.read(dType,reinterpret_cast<void*>(&value));
         attrib.close();
      }

   const std::string ReadStrAttrib(const std::string& name){
      //attributes are clunky in HDF5++ implementation - this is a workaround
      //template is required to pass the proper value type

      std::string value;
      H5::StrType strType(H5::PredType::C_S1,256);
      //access the built-in root group and create a new attribute for it.
      H5::Group rootGroup = file_->openGroup("/");
      H5::Attribute attrib = rootGroup.openAttribute(name);

      //write the value to the attribute and close
      attrib.read(strType,value);
      attrib.close();

      return value;
   }

   void Close(){ file_->close();}

   ~HDF5(){ file_->close();}
};




#endif
