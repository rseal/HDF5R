#include<shs/Shs.h>
#include <HDF5/HDF5.hpp>
#include <HDF5/Complex.hpp>
#include<iostream>
#include<clp/CommandLineParser.hpp>
#include<boost/cstdint.hpp>

using namespace std;
using namespace H5;

//Primary keywords
struct Primary{
   string instrument;
   string startTime;
   double sampleRate;
   double bandwidth;
   int decimation;
   int channels;
   double outputRate;
   double rf;
   double ipp;
   double ddc;
};

struct Data{
   //Data keywords
   int tableSize;
   int dim0;
   int dim1;
   int dWinStart;
   int dWinSize;
   string time;
};

int main(int argc, char** argv){

   typedef SimpleHeader<int16_t,2> Header;
   Primary pHeader;
   Data dHeader;

   //class to handle command line options/parsing
   CommandLineParser clp(argc,argv);
   Arg arg1("f", "file to view", 1, true);
   clp.AddArg(arg1);
   clp.Parse();

   //get filename from command-line argument
   string fileName = clp.GetArgValue<string>("f");

   //open shs file
   Header header(fileName, File::READ, File::BINARY);
   header.ReadPrimary();
   header.ReadData(0);
   int numTables = header.NumTables();

   //read SHS primary keywords
   pHeader.instrument = header.primaryValue<string>("Instrument");
   pHeader.startTime = header.primaryValue<string>("Time");
   pHeader.sampleRate = header.primaryValue<double>("Sample Rate");
   pHeader.bandwidth = header.primaryValue<double>("Bandwidth");
   pHeader.decimation = header.primaryValue<int>("Decimation");
   pHeader.channels = header.primaryValue<int>("Channels");
   pHeader.outputRate = header.primaryValue<double>("Output Rate");
   pHeader.rf = header.primaryValue<double>("RF");
   pHeader.ipp = header.primaryValue<double>("IPP");
   pHeader.ddc = header.primaryValue<double>("DDC0");

   //open hdf5 in write mode
   HDF5 hdfFile(fileName+"_",hdf5::WRITE);

   //create an instance of the custom HDF5 complex DataType
   ComplexHDF5 cpx;

   //vector used to create HDF5 DataSpace information
   vector<hsize_t> dim(2);
   dim[0] = header.data.Dim(0);
   dim[1] = header.data.Dim(1)/2;

   //complex vector to create a data table
   vector<complex_t> data(dim[0]*dim[1]);


   cout << "Found " << numTables << " tables " << endl;
   cout << "DIM0  " << dim[0] << endl;
   cout << "DIM1  " << dim[1] << endl;

   hdfFile.Description("File conversion SHS --> HDF5");
   hdfFile.WriteStrAttrib("START_TIME", pHeader.startTime); 
   hdfFile.WriteAttrib<double>("SAMPLE_RATE", pHeader.sampleRate, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());
   hdfFile.WriteAttrib<double>("BANDWIDTH", pHeader.bandwidth, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());
   hdfFile.WriteAttrib<int>("DECIMATION", pHeader.decimation, H5::PredType::NATIVE_INT, H5::DataSpace());
   hdfFile.WriteAttrib<int>("CHANNELS", pHeader.channels, H5::PredType::NATIVE_INT, H5::DataSpace());
   hdfFile.WriteAttrib<double>("OUTPUT_RATE", pHeader.outputRate, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());
   hdfFile.WriteAttrib<double>("RF", pHeader.rf, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());
   hdfFile.WriteAttrib<double>("IPP", pHeader.ipp, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());
   hdfFile.WriteAttrib<double>("DDC", pHeader.ddc, H5::PredType::NATIVE_DOUBLE, H5::DataSpace());

   int fileNum=0; 
   int i=0, j=0;
   //create an HDF5 DataSpace instance
   DataSpace space(2,&dim[0]);
   //complex_t* dPtr = &data[0];
   int idx=0;
   for(i=0; i<numTables; ++i){

      idx = i + j;
      //dPtr = reinterpret_cast<complex_t*>(header.ReadData(i));

      hdfFile.CreateTable(cpx.GetRef(),space);
      hdfFile.WriteTStrAttrib("TABLENUM", boost::lexical_cast<string>(idx));
      hdfFile.WriteTStrAttrib("TIME", header.dataValue<string>("TIME"));
      hdfFile.WriteTAttrib<int>("DATA_WIN_START", 466, H5::PredType::NATIVE_INT, DataSpace());
      hdfFile.WriteTAttrib<int>("DATA_WIN_SIZE", header.dataValue<int>("Data"), H5::PredType::NATIVE_INT, DataSpace());
      hdfFile.WriteTable(reinterpret_cast<void*>(header.ReadData(i)));

      if((i%100)==0) cout << "table[" << idx << "]" << endl;

      if(!i){
         cout << "FileNumber = " << fileNum << endl;
         cout << "numTables  " << numTables << endl;
      }

      header.ReadData(i);

      if(i == numTables - 1){
         j+=numTables;
         header.NextFile();
         header.ReadPrimary();
         header.ReadData(0);
         numTables = header.NumTables();
         fileNum = header.FileNumber();
         i=-1;
      }
   }
   hdfFile.Close();

   return 0;
}
