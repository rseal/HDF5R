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

   //Open HDF5 file in READ mode - note these files 
   //use the HDF5 family mode VFD
   HDF5 file("psu_test_", hdf5::READ);

   //Custom complex data type for radar data
   //format is 16-bit real, 16-bit imag
   ComplexHDF5 cpx;

   //read table dimensions into vector
   vector<hsize_t> dim = file.TableDims();

   //create an appropriate size buffer to read and store tables
   vector<complex_t> data(dim[0]*dim[1]);

   //read the file's description string
   cout << "-------------------- DESCRIPTION --------------------" << endl;
   cout << file.Description() << endl;
   file.ReadTable<complex_t>(0, data, cpx.GetRef());
   cout << "-----------------------------------------------------\n" << endl;

   //create variables to store attributes
   float sampleRate;
   string startTime; 

   //read root attributes
   file.ReadAttrib<float>("SAMPLE_RATE", sampleRate, H5::PredType::NATIVE_FLOAT);
   startTime = file.ReadStrAttrib("START_TIME");
   cout << "Sample Rate = " << sampleRate << endl;
   cout << "Start Time  = " << startTime << endl;

   //read table attributes
   int time;
   string tableNum;

   //format and display data table 0
   int idx=0;
   for(unsigned int i=0; i<dim[0]; ++i){
      cout << "d[" << i << "]= [\n";
      for(unsigned int j=0; j<dim[1]; ++j){
         if(!(j%10)) cout << "\n";
         idx = i*dim[0] + j;
         cout << data[idx].real << "," << data[idx].imag << " "; 
      }
      cout << "\n\n]\n" << endl;
   }

   //loop through a few tables, read attributes and display
   int numTables = file.NumTables();
   for(int i=0; i<numTables; ++i){
      cout << "table number = " << file.ReadTStrAttrib(i,"TABLENUM") << endl;
      file.ReadTAttrib(i,"TIME", time, H5::PredType::NATIVE_INT);
      cout << "      time   = " << time << endl;
   }

   return 0;
}
