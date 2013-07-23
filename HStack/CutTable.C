// ===========================================================================
// 
//       Filename:  CutTable.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  05/30/2013 06:14:41 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================


#include <cstdlib>
#include <iomanip>
#include <iostream>
#include "TFile.h"
#include "TH1F.h"
#include <vector>
#include <string>
#include <map>
#include <cstdio>
#include "HTSample.h"

using namespace std;

std::vector<std::string> order; // Order of the cut  
int CutTable(std::string dir, std::string sample, std::string pileup);
int ElogTable(std::string dir, std::string pileup, std::vector<std::string> sample);

int main ( int argc, char *argv[] )
{

  std::vector<std::string> Dir;
  //Dir.push_back("MJJ__5_30/");
  Dir.push_back("./CondorHiggs_5_30/");
  //Dir.push_back("Jul163M_5_30_temp/");
  //Dir.push_back("HT5M_5_30/");
  //Dir.push_back("LoopCut_5_30");
  //Dir.push_back("LoopCut_5_50");
  //Dir.push_back("LoopCut_4_30");
  //Dir.push_back("LoopCut_4_50");

  std::vector<std::string> PU;
  PU.push_back("NoPileUp");
  //PU.push_back("50PileUp");
  //PU.push_back("140PileUp");
  

  std::vector<std::string> Sample;
  Sample.push_back("Wino200_14TeV");
  Sample.push_back("Wino500_14TeV");
  Sample.push_back("TT_14TEV_HT");
  //Sample.push_back("B_14TEV_HT");
  //Sample.push_back("BJ_14TEV_HT");
  //Sample.push_back("BJJ_14TEV_HT");
  Sample.push_back("W*_14TEV_HT");
  Sample.push_back("Z*_14TEV_HT");
  Sample.push_back("H*_14TEV_HT");
  Sample.push_back("Photon*_14TEV_HT");
  //Sample.push_back("W_14TEV_HT");
  //Sample.push_back("WJ_14TEV_HT");
  //Sample.push_back("WJJ_14TEV_HT");
  //Sample.push_back("W_14TEV_HT");
  //Sample.push_back("WJ_14TEV_HT");
  //Sample.push_back("WJJ_14TEV_HT");
  //Sample.push_back("Z_14TEV_HT");
  //Sample.push_back("ZJ_14TEV_HT");
  //Sample.push_back("ZJJ_14TEV_HT");
  //Sample.push_back("H_14TEV_HT");
  //Sample.push_back("HJ_14TEV_HT");
  //Sample.push_back("HJJ_14TEV_HT");
  //Sample.push_back("Photon_14TEV_HT");
  //Sample.push_back("PhotonJ_14TEV_HT");
  //Sample.push_back("PhotonJJ_14TEV_HT");


  // The Cut flow

  order.push_back("NoCut");
  order.push_back("CTVBF");
  order.push_back("CTJ1");
  order.push_back("CTJ2");
  order.push_back("CTMjj");
  order.push_back("CTJ3");
  order.push_back("CTBJ");
  order.push_back("CTLep");
  order.push_back("CTMet200");
  order.push_back("AllCut");

  //// The Cut flow
  //order.push_back("NoCut");
  //order.push_back("CTVBF");
  //order.push_back("CTLepV");
  //order.push_back("CTMet50");
  //order.push_back("CTBJet");
  //order.push_back("CTJ3");
  //order.push_back("CTMjj");
  //order.push_back("CTMet200");
  //order.push_back("AllCut");
  for(std::vector<std::string>::iterator Dit=Dir.begin();
      Dit!=Dir.end(); Dit++)
  {
    for(std::vector<std::string>::iterator Pit=PU.begin();
        Pit!=PU.end(); Pit++)
    {
      ElogTable(*Dit, *Pit, Sample);
      //for(std::vector<std::string>::iterator Sit=Sample.begin();
          //Sit!=Sample.end(); Sit++)
      //{
        ////CutTable(*Dit, *Sit, *Pit);
      //}

    }
  }

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


// ===  FUNCTION  ============================================================
//         Name:  CutTable
//  Description:  /* cursor */
// ===========================================================================
int CutTable(std::string dir, std::string sample, std::string pileup)
{
  std::cout << std::endl;
  std::cout << "======================================================" << std::endl;

  std::cout << " Dir :" << setw(15) << dir 
    <<" Pileup: "  << setw(15) << pileup
    <<" Sample: "  << setw(15) << sample 
    << std::endl;

  std::cout << "======================================================" << std::endl;
  TString filename = dir+"/"+sample+"_"+pileup+".root";
  TFile *f = new TFile(filename.Data(), "R");
  if (!f->IsOpen()) return 0;
  //TFile f1("LoopCut_5_30/TTBAR_13TEV_NoPileUp.root");
  TH1F* c1 = (TH1F*)f->Get("CutFlow");

  double orginal = c1->GetBinContent(1);


/*
 *  for (int i = 1; i < c1->GetXaxis()->GetNbins(); ++i)
 *  {
 *    if (i > (order.size()+1)) break;
 *    //std::cout << " bin " << i << " NEvt: " <<  c1->GetBinContent(i) << "eff : " << c1->GetBinContent(i)/orginal << std::endl;
 *    std::cout << " Cut: " << std::left << setw(10)<< order.at(i-1)
 *      << " Selected Event : "<< std::left << setw(10) <<  c1->GetBinContent(i) 
 *      << " Selection Eff. : " << std::setprecision(2)<<
 *      std::scientific<< c1->GetBinContent(i)/orginal << std::endl;
 *    
 *  }
 *
 */
  for (int i = 1; i < c1->GetXaxis()->GetNbins(); ++i)
  {
    if (i > (order.size()+1)) break;
    //std::cout << " bin " << i << " NEvt: " <<  c1->GetBinContent(i) << "eff : " << c1->GetBinContent(i)/orginal << std::endl;
    std::cout << " Cut: " << std::left << setw(10)<< order.at(i-1)
      << " Selected Event : "<< std::setprecision(0)<<std::fixed<< std::left <<  c1->GetBinContent(i) 
      << std::setprecision(1)<<std::fixed<< std::left 
      //<< " Selected Event : "<< std::setprecision(1)<< std::left <<  c1->GetBinContent(i) 
      << "(" << std::scientific<< c1->GetBinContent(i)/orginal <<")" << std::endl;
    
  }
  delete f;
  return 1;

}       // -----  end of function CutTable  -----

// ===  FUNCTION  ============================================================
//         Name:  ElogTable
//  Description:  
// ===========================================================================
int ElogTable(std::string dir, std::string pileup, std::vector<std::string> Sample)
{
  std::map<string, std::vector<std::string> > table;

  for(std::vector<std::string>::iterator Sit=Sample.begin();
      Sit!=Sample.end(); Sit++)
  {
    HTSample temp(dir, *Sit, pileup);
    temp.InitSample(100);
    TH1F* c1 = temp.GetTH1("CutFlow", false);

    double orginal = c1->GetBinContent(1);

    for (int i = 1; i <= c1->GetXaxis()->GetNbins(); ++i)
    {
      //std::cout << " Bin label "<< c1->GetXaxis()->GetBinLabel(i) << std::endl;
      //if (i > (order.size()+1)) break;
      char buf[100];
      sprintf(buf, "%.f(%.2f)",c1->GetBinContent(i) , c1->GetBinContent(i)/c1->GetBinContent(i-1)  );
      //sprintf(buf, "%.f(%.1e)",c1->GetBinContent(i) , c1->GetBinContent(i)/orginal  );
      std::string temp = buf;
      table[*Sit].push_back(temp);
    }
  }

  //----------------------------------------------------------------------------
  //  Start to print out elog table
  //----------------------------------------------------------------------------
  //[table border="1"] 
  //[b][center]fd[/center][/b]  | |-
  //[/table]


  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "================================================================= " << std::endl;
  std::cout <<  "[b]" << "Jet selection: " << "Pt > 30, |Eta| < 5" <<  " PileUp : " << pileup  << "[/b]"<< std::endl;
  std::cout <<  "[table border=\"1\"] " << std::endl;

  // The first row of name
  std::cout <<  "[b][center]Cut[/center][/b]  |";
  for(std::vector<std::string>::iterator Sit=Sample.begin();
      Sit!=Sample.end(); Sit++)
  {
    std::cout <<  "[b][center]" << *Sit<< "[/center][/b]  |";
  }
  std::cout << "-" << std::endl;

  // Start to fill in the Cut flow
  for (int i = 0; i < order.size(); ++i)
  {


    std::cout << "[center]" << order.at(i)<< "[/center] |";
    for(std::vector<std::string>::iterator Sit=Sample.begin();
        Sit!=Sample.end(); Sit++)
    {
      std::cout << "[center]" << table[*Sit].at(i)<< "[/center] |";
    }
    std::cout << "-" << std::endl;

  }
  std::cout <<  "[/table]" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;

  return 1;
}       // -----  end of function ElogTable  -----

