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
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
int Loop(TChain* chain, std::string Process, std::string PileUp );
int RunSys(TChain* chain, std::string Process, std::string PileUp, std::string sys,
    bool PUCorMet, double PUCorJetPt , double PUCorJetEta, 
    bool LeptonicTT = false, double TTBarMetThre = 0.0);

const std::string Path = "dcache:/pnfs/cms/WAX/11/store/user/snowmass/Delphes-3.0.7/";

int main ( int argc, char *argv[] )
{

  if (argc < 3)
  {
    std::cout << "Please enter the pileup and process name to be run on ! " <<  std::endl;
    return EXIT_FAILURE;
  }


//----------------------------------------------------------------------------
//  Define all the variables here
//----------------------------------------------------------------------------
  //std::string Pileup        = "TEST";
  //std::string Process       = "TTBAR";
  //std::string Process     = "ZJETS";

  std::string Pileup      = argv[1];
  std::string Process     = argv[2];
  const bool LoopCuts       = false;


  // PU corrected Met
  const bool PUCorMet       = true;
  const double PUCorJetEta  = 4;
  const double PUCorJetPt   = 50;

  // Intrisic Vs Leptonic Met
  const bool LeptonicTT     = true;
  const double TTBarMetThre = 100;

  const std::string Ourdir  = "Lep_4_50";

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
  //chain.Add("/uscms_data/d3/benwu/CMSSW_6_0_1_PostLS1v2_patch3/src/UserCode/spadhi/Snowmass/Delphes/Delphes-3.0.7/CMSUpgrade/test/TTBARW_13TEV_50PileUp_6351.root");
  

  if (chain.GetEntriesFast() == 0)
  {
    std::cout << "No files are attached! Exiting... " << std::endl;
    return EXIT_FAILURE;
  }

  if (LoopCuts)
    Loop(&chain, Process, Pileup);

  if (argc == 4)
  {
    int input = atoi( argv[3]);

    std::vector<std::string> order;
    order.push_back("noCut");
    order.push_back("CTVBF");
    order.push_back("CTLepV");
    order.push_back("CTMet50");
    order.push_back("CTMjj");
    order.push_back("CTMet200");
    order.push_back("AllCut");


    if (input >= order.size())
    {
      std::cout << " Out of size " << std::endl;
      return 1;
    }

    std::cout << " input " << input << " sys " << order.at(input) << std::endl;

    if (Process.find("TTBAR") != std::string::npos)
      RunSys(&chain, Process, Pileup, order.at(input), PUCorMet, 
          PUCorJetPt, PUCorJetEta, LeptonicTT, TTBarMetThre);
    else
      RunSys(&chain, Process, Pileup, order.at(input), PUCorMet, PUCorJetPt, PUCorJetEta);

  } else {
    DPhes DP(&chain);
    DP.InitDelPhes(Process, Pileup);
    DP.ReadDelPhes();
    DP.SetCutBit("0"); //No Cut applied 
    //DP.SetCutBit("-1"); //All Cuts applied
    if (Process.find("TTBAR") != std::string::npos)
      DP.SetTTBar(LeptonicTT, TTBarMetThre);
    DP.SetPUCorMet(PUCorMet, PUCorJetPt, PUCorJetEta);
    DP.BookHistogram();
    DP.Looping();
    DP.DrawHistogram(Ourdir);

  }

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

int Loop(TChain* chain, std::string Process, std::string PileUp )
{
  typedef  std::map<std::string, std::string>  maps;
  //typedef  std::map<std::string, std::string>::iterator  maps_it;
  maps CutMap;
  CutMap["0"] = "NoCut";
  CutMap["0000000001"] = "CTVBF";
  CutMap["0000000011"] = "CTLepV";
  CutMap["0000000111"] = "CTMet50";
  CutMap["0000001111"] = "CTMjj";
  CutMap["0000011111"] = "CTMet200";
  CutMap["-1"] = "AllCut";

  for(maps::iterator c_it=CutMap.begin();
      c_it!=CutMap.end(); c_it++)
  {
    DPhes *DP = new DPhes(chain);
    TString PUCut = PileUp+"_"+c_it->second;
    DP->InitDelPhes(Process, PUCut.Data());
    DP->ReadDelPhes();
    DP->SetCutBit(c_it->first); //No Cut applied 
    DP->BookHistogram();
    DP->Looping();
    DP->DrawHistogram();
    delete DP;
  }
  return 1;
}

int RunSys(TChain* chain, std::string Process, std::string PileUp, std::string sys,
    bool PUCorMet, double PUCorJetEta , double PUCorJetPt, bool LeptonicTT, double TTBarMetThre)
{
  typedef  std::map<std::string, std::string>  maps;
  maps CutMap;

  CutMap["NoCut"]    = "0";
  CutMap["CTVBF"]    = "0000000001";
  CutMap["CTLepV"]   = "0000000011";
  CutMap["CTMet50"]  = "0000000111";
  CutMap["CTMjj"]    = "0000001111";
  CutMap["CTMet200"] = "0000011111";
  CutMap["AllCut"]   = "-1";

  DPhes *DP = new DPhes(chain);
  TString PUCut = PileUp+"_"+sys;
  DP->InitDelPhes(Process, PUCut.Data());
  DP->ReadDelPhes();
  DP->SetCutBit(CutMap[sys]); //No Cut applied 
  DP->BookHistogram();
  DP->SetPUCorMet(PUCorMet, PUCorJetPt, PUCorJetEta);
  DP->SetTTBar(LeptonicTT, TTBarMetThre);
  DP->Looping();
  DP->DrawHistogram();
  delete DP;
  return 1;
}

