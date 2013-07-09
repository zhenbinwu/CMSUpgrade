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
#include "CMSAUX.h"
#include <cmath>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdio>

using namespace std;

const std::string Path = "dcache:/pnfs/cms/WAX/11/store/user/snowmass/Delphes-3.0.7/";

int main ( int argc, char *argv[] )
{

  /*
   *if (argc < 3)
   *{
   *  std::cout << "Please enter the pileup and process name, Jet Eta and PT for PU correction to be run on ! " <<  std::endl;
   *  return EXIT_FAILURE;
   *}
   */


//----------------------------------------------------------------------------
//  Define all the variables here
//----------------------------------------------------------------------------
  //std::string Pileup  = "50PileUp25ns";
  //std::string Process = "TTBAR";
  //std::string Process = "ZJETS";

  std::string Pileup    = argv[1];
  std::string Process   = argv[2];
  std::string File      = argv[3];


  // PU corrected Met
  const double PUCorJetEta = 5;
  const double PUCorJetPt  = 30;
  //const double PUCorJetEta   = atof(argv[3]);
  //const double PUCorJetPt    = atof(argv[4]);

  char buf[100];
  sprintf(buf, "%s_%.0f_%.0f", "MuonIso05", PUCorJetEta, PUCorJetPt );
  const std::string Outdir  = buf;
  //const std::string Outdir  = "TEST";

//----------------------------------------------------------------------------
//  Done with input variables
//----------------------------------------------------------------------------

  std::cout<<"Running Process : \033[0;31m"<< Process<<"\033[0m with pileup : \033[1;36m"<< Pileup << "\033[0m"<< std::endl; 

  //TString TreeList = Path + Pileup +"/"+Process+"/"+Process+"_"+Pileup+"_*.root";
  //TString TreeList = "FileList/"+Process+"_"+Pileup+".list";
  std::cout << "Files to be run on : " << File  << std::endl;
  TChain chain("histAndTree_forVBFSusy/AUX","");

  //if(TreeList.Contains("FileList"))
  //{
    //std::fstream input(TreeList.Data());
    //for(std::string line; getline(input, line);)
    //{
      //std::cout << "Add File: " << line << std::endl;
      //chain.Add(line.c_str());
    //}
  //}
  //else
    //chain.Add(TreeList);

  //chain.Add("test/TTBARW_13TEV_50PileUp_6351.root");
  chain.Add(File.c_str());
  //chain.Add("./CMS/res/*root");
  //chain.Add("./ZJETS_13TEV_NoPileUp_62128.root");

  CMSAUX AUX(&chain);
  AUX.SetPUCorrection(PUCorJetPt, PUCorJetEta);
  AUX.SetPreName(Process, Pileup, Outdir);

  AUX.PreLooping();
  AUX.Loop();
  AUX.PostLooping();

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


