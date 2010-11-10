#include "Complex.hpp"
#include "HDF5.hpp"
#include <vector>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace boost;
using namespace std;
using namespace H5;
using namespace std;

int main(){

   //open file in WRITE mode
   HDF5 file("psu_test_", hdf5::WRITE);
   
   //create an instance of the custom HDF5 complex DataType
   ComplexHDF5 cpx;
   
   //vector used to create HDF5 DataSpace information
   vector<hsize_t> dim(2);
   dim[0] = 10; dim[1] = 10;

   //complex vector to create a data table
   vector<complex_t> data(dim[0]*dim[1]);

   //write a brief (256 chars or less) description of data 
   file.Description("Test data to illustrate usefulness of the HDF5 format when \n"
         "used with fixed size tables.\n");

   //write attributes to the root group "/"
   file.WriteStrAttrib("START_TIME", "00:00:00"); 
   file.WriteAttrib<double>("SAMPLE_RATE", 64e6, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());

   //generate a data table 
   unsigned int i,j;
   for(i=0; i<dim[0]; ++i){
      for(j=0; j<dim[1]; ++j){
         int16_t idx = j + i*dim[1];
         data[idx].real = idx;
         data[idx].imag = 2*idx;
      }
   }

   //create an HDF5 DataSpace instance
   DataSpace space(2,&dim[0]);

   //create 9200 data tables with attributes
   for(int i=0; i<99999; ++i){
      file.CreateTable(cpx.GetRef(),space);
      cout << "table num = " << boost::lexical_cast<string>(i) << endl;
      //file.WriteTAttrib<int>("TIME", i,  H5::PredType::NATIVE_INT, DataSpace());
      //file.WriteTStrAttrib("TABLENUM", boost::lexical_cast<string>((long)i));
      file.WriteTable(reinterpret_cast<void*>(&data[0]));
   }

   //close out the file
   file.Close();

   return 0;
}
