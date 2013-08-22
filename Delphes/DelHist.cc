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
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include <cstdlib>
#include "DelFill.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include "TError.h"
#include <cstdio>

int main ( int argc, char *argv[] )
{

  if (argc < 4)
  {
    std::cout << "Please enter the pileup, process name,  Dir_Eta_PT and Detector to be run on ! " <<  std::endl;
    return EXIT_FAILURE;
  }


//----------------------------------------------------------------------------
//  Define all the variables here
//----------------------------------------------------------------------------
  //std::string Pileup  = "140PileUp";
  //std::string Process = "BJJ_14TEV_HT1_1";
  //std::string Process = "ZJETS";
  //const std::string Outdir = "test_5_30";


  const std::string Pileup   = argv[1];
  const std::string Process  = argv[2];
  const std::string Outdir   = argv[3];
  const std::string Detector = argv[4];


  double PUCorJetEta = -99;
  double PUCorJetPt = -99;
  char buf[10];
  sscanf(Outdir.c_str(), "%[^_]_%lf_%lf", buf, &PUCorJetEta, &PUCorJetPt );


//----------------------------------------------------------------------------
//  Done with input variables
//----------------------------------------------------------------------------

  std::cout<<"Running Process : \033[0;31m"<< Process<<"\033[0m with pileup : \033[1;36m"<< Pileup << "\033[0m"<< std::endl; 

  std::string env  = getenv("DELPHES");
  TString TreeList = "";
  TChain chain("Delphes");

  if (Detector == "Snowmass")
  {
    TreeList = "./FileList/Snowmass/"+Process+"_"+Pileup+".list";
  } else if (Detector == "PhaseI")
  {
    TreeList = "./FileList/PhaseI/"+Process+"_"+Pileup+".list";
  } else if (Detector == "PhaseII3")
  {
    TreeList = "./FileList/PhaseII3/"+Process+"_"+Pileup+".list";
  } else if (Detector == "PhaseII4")
  {
    TreeList = "./FileList/PhaseII4/"+Process+"_"+Pileup+".list";
  }
  std::cout << "Files to be run on : " << TreeList  << std::endl;



  if(TreeList.Contains("FileList"))
  {
    std::fstream input(TreeList.Data());
    for(std::string line; getline(input, line);)
    {
      if (line[0] == '#') continue;
      std::cout << "Add File: " << line << std::endl;
      chain.Add(line.c_str());
    }
  }
  else
    chain.Add(TreeList);  


  //chain.Add("test/tt-4p-1700-2500-v1510_14TEV_140PileUp_99660663.root");
  //chain.Add("test/TTBARW_13TEV_50PileUp_6351.root");
  //chain.Add("test/ZJETS_13TEV_NoPileUp_9850.root");
  //chain.Add("./Bjj-vbf-4p-700-1400-v1510_14TEV_140PileUp_36404679.root");
  //chain.Add("./Bj-4p-1100-1800-v1510_14TEV_140PileUp_95160070.root");
  //chain.Add("./Bjj-vbf-4p-2300-3400-v1510_14TEV_NoPileUp_96938081.root");
  //chain.Add("./Bj-4p-1800-2700-v1510_14TEV_NoPileUp_291444200.root");
  //chain.Add("./ZJETS_13TEV_50PileUp_46978.root");
  //chain.Add("~/nobackup/Bj-4p-1800-2700-v1510_14TEV_NoPileUp_291444200.root");
  //chain.Add("~/nobackup/DYNOPU_Phase1.root");

  if (chain.GetListOfFiles()->GetEntries() == 0)
  {
    std::cout << "No files attached! Exiting ...."  << std::endl;
    return EXIT_FAILURE;
  }

  // To remove the warning from TFile::Append
  // -->  Replacing existing TH1: CrossSection (Potential memory leak)
  // We need same histname for later on comparison
  gErrorIgnoreLevel = kError;

  DPhes DP(&chain);
  DP.SetPUCorMet(PUCorJetPt, PUCorJetEta);
  DP.InitDelPhes(Process, Pileup, "");
  DP.ReadDelPhes();
  DP.PreLooping();
  DP.Looping();
  DP.PostLooping();
  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


