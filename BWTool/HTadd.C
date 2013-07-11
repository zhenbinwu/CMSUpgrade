// ===========================================================================
// 
//       Filename:  HTadd.C
// 
//    Description:  This code will work for merging the HTBin splited samples
//    into one files
// 
//        Version:  1.0
//        Created:  07/11/2013 11:13:44 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================


#include <cstdlib>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>
#include <sstream>
#include "glob.h"
#include <cstdio>
#include <vector>
#include <sys/stat.h>
#include <boost/foreach.hpp>

#include "TFile.h"
#include "TH1F.h"
//#include "StopStyle.hh" //Need this for TFile?? don't understand 

std::string exec(std::string cmd);
bool HistNoMerg(std::string target, std::string firstfile, std::string hname);
int MoveMerged(std::string outdir, std::string file);

int main ( int argc, char *argv[] )
{

  if (argc < 3)
  {
    std::cout << "Please enter the target file and input file names ! " <<  std::endl;
    return EXIT_FAILURE;
  }

  std::string target = argv[1]; // Output Target
  std::vector<std::string> vFiles;
  //TFile te(argv[2], "R");
  //return 1;


   for ( int i = 2; i < argc; i++ ) {
     vFiles.push_back(argv[i]);
   }

//----------------------------------------------------------------------------
//  Define which histogram should not be overwritten
//----------------------------------------------------------------------------
  std::vector<std::string> vHist;
  vHist.push_back("CrossSection");
  vHist.push_back("NEVT");

//----------------------------------------------------------------------------
//  Merging the files
//----------------------------------------------------------------------------
  std::stringstream ss;
  ss<< "hadd -O -f " << target;
  BOOST_FOREACH(std::string input, vFiles)
  {
    ss<< " " << input;
  }
  std::string output = exec(ss.str());
  std::cout <<  output<< std::endl;

//----------------------------------------------------------------------------
// Unmerge some histograms defined in vHist
//----------------------------------------------------------------------------o  
  std::for_each(vHist.begin(), vHist.end(), 
      std::bind(HistNoMerg, target, vFiles.front(), std::placeholders::_1));


//----------------------------------------------------------------------------
//  Now moving the merged file to a temp directory 
//----------------------------------------------------------------------------
  std::string outdir = "splited";
  struct stat st;
  if (stat(outdir.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
    ;
  else
    mkdir(outdir.c_str(), S_IRWXU);

  std::for_each(vFiles.begin(), vFiles.end(), 
      std::bind(MoveMerged, outdir, std::placeholders::_1));



  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

std::string exec(std::string cmd) {
    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
      if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    pclose(pipe);
   return result;
}

// ===  FUNCTION  ============================================================
//         Name:  HistNoMerg
//  Description:  For those histogram should not be merge, rewrite it from the
//  first file in the list
// ===========================================================================
bool HistNoMerg(std::string target, std::string firstfile, std::string hname)
{
  //TFile targF(target.c_str(), "UPDATE");
  //TFile inpF(firstfile.c_str(), "R");
  TFile* targF = new TFile(target.c_str(), "UPDATE");
  TFile* inpF = new TFile(firstfile.c_str(), "R");

  std::cout << "Merging " << hname << std::endl;
  TH1F* inpH = (TH1F*)inpF->Get(hname.c_str());

  // Dummy for avoiding Warning for TClass for no dictionary? 
  inpH->GetXaxis()->GetNbins();

  targF->cd();
  inpH->Write("", TObject::kOverwrite);
  targF->Close();
  inpF->Close();

  delete targF;
  delete inpF;

  return true;
}       // -----  end of function HistNoMerg  -----

// ===  FUNCTION  ============================================================
//         Name:  MoveMerged
//  Description:  Move the merged files to the outdir
// ===========================================================================
int MoveMerged(std::string outdir, std::string file)
{

  std::stringstream ss;
  ss << outdir << "/" << file;
  int result = rename(file.c_str(), ss.str().c_str());
  std::cout << " moving " << file << " to " << ss.str().c_str() << std::endl;

  return result;
}       // -----  end of function MoveMerged  -----
