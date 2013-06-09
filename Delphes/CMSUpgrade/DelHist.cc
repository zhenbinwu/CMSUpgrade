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
  const bool PUCorMet        = true;
  //const double PUCorJetEta = 5;
  //const double PUCorJetPt  = 30;
  const double PUCorJetEta   = atof(argv[3]);
  const double PUCorJetPt    = atof(argv[4]);

  // Intrisic Vs Leptonic Met
  const bool LeptonicTT     = false;
  const double TTBarMetThre = 100;

  char buf[100];
  sprintf(buf, "%s_%.0f_%.0f", "MJJ_", PUCorJetEta, PUCorJetPt );
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
  //chain.Add("test/ZJETS_13TEV_NoPileUp_62128.root");
  //chain.Add("./ZJETS_13TEV_NoPileUp_62128.root");

  // Quick and dirty way
  {

    DPhes DP(&chain);
    if (Process.find("TTBAR") != std::string::npos)
    {
      TString ProSLep = Process + (LeptonicTT ? "_FLep " : "_SLep");
      DP.InitDelPhes(ProSLep.Data(), Pileup);
    }
    else
      DP.InitDelPhes(Process, Pileup);

    DP.ReadDelPhes();
    DP.SetCutBit("0"); //No Cut applied 
    //DP.SetCutBit("-1"); //All Cuts applied
    if (Process.find("TTBAR") != std::string::npos)
      DP.SetTTBar(LeptonicTT, TTBarMetThre);
    DP.SetPUCorMet(PUCorMet, PUCorJetPt, PUCorJetEta);
    DP.BookHistogram();
    DP.Looping();
    DP.DrawHistogram(Outdir);
  }

  if (Process.find("TTBAR") != std::string::npos)
  {
    DPhes DP2(&chain);
    TString ProSLep = Process + (LeptonicTT ? "_SLep " : "_FLep");
    DP2.InitDelPhes(ProSLep.Data(), Pileup);
    DP2.ReadDelPhes();
    DP2.SetCutBit("0"); //No Cut applied 
    //DP2.SetCutBit("-1"); //All Cuts applied
    if (Process.find("TTBAR") != std::string::npos)
      DP2.SetTTBar(!LeptonicTT, TTBarMetThre);
    DP2.SetPUCorMet(PUCorMet, PUCorJetPt, PUCorJetEta);
    DP2.BookHistogram();
    DP2.Looping();
    DP2.DrawHistogram(Outdir);

  }

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


