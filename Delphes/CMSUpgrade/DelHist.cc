// ===========================================================================
// 
//       Filename:  DelHist.cc
// 
//    Description:  The main file to be called 
// 
//        Version:  1.0
//        Created:  05/19/2013 02:53:52 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include <cstdlib>
#include "DelFill.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

const std::string Path = "dcache:/pnfs/cms/WAX/11/store/user/snowmass/Delphes-3.0.7/";

int main ( int argc, char *argv[] )
{

  if (argc < 3)
  {
    std::cout << "Please enter the pileup and process name, Jet Eta and PT for PU correction to be run on ! " <<  std::endl;
    return EXIT_FAILURE;
  }


//----------------------------------------------------------------------------
//  Define all the variables here
//----------------------------------------------------------------------------
  //std::string Pileup  = "TEST";
  //std::string Process = "TTBAR";
  //std::string Process = "ZJETS";

  std::string Pileup    = argv[1];
  std::string Process   = argv[2];


  // PU corrected Met
  //const double PUCorJetEta = 5;
  //const double PUCorJetPt  = 30;
  const double PUCorJetEta   = atof(argv[3]);
  const double PUCorJetPt    = atof(argv[4]);

  char buf[100];
  sprintf(buf, "%s_%.0f_%.0f", "Test", PUCorJetEta, PUCorJetPt );
  const std::string Outdir  = buf;
  //const std::string Outdir  = "TEST";

//----------------------------------------------------------------------------
//  Done with input variables
//----------------------------------------------------------------------------

  std::cout<<"Running Process : \033[0;31m"<< Process<<"\033[0m with pileup : \033[1;36m"<< Pileup << "\033[0m"<< std::endl; 

  //TString TreeList = Path + Pileup +"/"+Process+"/"+Process+"_"+Pileup+"_*.root";
  TString TreeList = "FileList/"+Process+"_"+Pileup+".list";
  std::cout << "Files to be run on : " << TreeList  << std::endl;
  TChain chain("Delphes");

  if(TreeList.Contains("FileList"))
  {
    std::fstream input(TreeList.Data());
    for(std::string line; getline(input, line);)
    {
      std::cout << "Add File: " << line << std::endl;
      chain.Add(line.c_str());
    }
  }
  else
    chain.Add(TreeList);

  //chain.Add("test/TTBARW_13TEV_50PileUp_6351.root");
  //chain.Add("test/ZJETS_13TEV_NoPileUp_9850.root");
  //chain.Add("./ZJETS_13TEV_NoPileUp_62128.root");

  DPhes DP(&chain);
  DP.SetPUCorMet(PUCorJetPt, PUCorJetEta);
  DP.InitDelPhes(Process, Pileup, Outdir);
  DP.ReadDelPhes();
  DP.PreLooping();
  DP.Looping();
  DP.PostLooping();

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


