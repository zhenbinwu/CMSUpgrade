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
#include <cmath>
#include <map>
#include <cstdio>
#include "HTSample.h"

using namespace std;

std::vector<std::string> order; // Order of the cut  
int CutTable(std::string dir, std::string sample, std::string pileup);
int ElogTable(std::string dir, std::string pileup, std::vector<std::string> sample);
int LatexTable(std::string dir, std::string pileup, std::vector<std::string> Sample);
std::vector<std::string> GetCutOrder(std::string dir);

std::string DEC = "Snowmass";
int main ( int argc, char *argv[] )
{

  std::vector<std::string> Dir;
  //DEC = argv[1];
  //Dir.push_back("MJJ__5_30/");
  //Dir.push_back("./Jul26Higgs_5_30/");
  //Dir.push_back("./system//");
  //Dir.push_back("./SUSY_5_30/");
  //Dir.push_back("./ECFA/");
  //Dir.push_back("./Preapproval/");
  //Dir.push_back("./Snowmass_5_30/");
  Dir.push_back("./SbottomMar_5_30/");
  //Dir.push_back("./LPC/");
  //Dir.push_back("./TOOL/");
  //Dir.push_back("./Snowmass/");
  //Dir.push_back("./SUSY_5_30_0812/SUSY_5_30/");
  //Dir.push_back("./Higgs_5_30/");
  //Dir.push_back("./HiggsECFA_5_30_BK/");
  //Dir.push_back("./Higgs8TEV_5_30/");
  //Dir.push_back("./METAsysHiggs_5_30/");
  //Dir.push_back("./METAsys021M_5_30/");
  //Dir.push_back("./CondorHiggs_5_30/");
  //Dir.push_back("Jul163M_5_30_temp/");
  //Dir.push_back("HT5M_5_30/");
  //Dir.push_back("LoopCut_5_30");
  //Dir.push_back("LoopCut_5_50");
  //Dir.push_back("LoopCut_4_30");
  //Dir.push_back("LoopCut_4_50");

  std::vector<std::string> PU;
  PU.push_back("NoPileUp");
  //PU.push_back("50PileUp");
  PU.push_back("140PileUp");
  

  std::vector<std::string> Sample;
  //Sample.push_back("Wino200_14TeV");
  //Sample.push_back("Wino500_14TeV");
  //Sample.push_back("TT_33TEV_HT");
  //Sample.push_back("TT_14TEV_HT");
  //Sample.push_back("B_14TEV_HT");
  //Sample.push_back("BJ_14TEV_HT");
  //Sample.push_back("BJJ_14TEV_HT");
  //
  Sample.push_back("Sbottom15_14TeV");
  //Sample.push_back("Sbottom50_14TeV");
  Sample.push_back("Sbottom100_14TeV");
  Sample.push_back("Sbottom150_14TeV");
  Sample.push_back("Sbottom200_14TeV");
  Sample.push_back("Sbottom300_14TeV");
  //Sample.push_back("Wino100_14TeV");
  //Sample.push_back("Wino200_14TeV");
  //Sample.push_back("Wlv*_14TEV_HT");
  //Sample.push_back("Zvv*_14TEV_HT");
  //Sample.push_back("Zvv_14TEV_HT");
  //Sample.push_back("ZvvJ_14TEV_HT");
  //Sample.push_back("ZvvJJ_14TEV_HT");
  //Sample.push_back("Wlv_14TEV_HT");
  //Sample.push_back("WlvJ_14TEV_HT");
  //Sample.push_back("WlvJJ_14TEV_HT");
  //Sample.push_back("SysZmm_14TEV_HT");
  //Sample.push_back("SysZmmJ_14TEV_HT");
  //Sample.push_back("SysZmmJJ_14TEV_HT");
  //Sample.push_back("SysZee_14TEV_HT");
  //Sample.push_back("SysZeeJ_14TEV_HT");
  //Sample.push_back("SysZeeJJ_14TEV_HT");
  //Sample.push_back("SysZmm*_14TEV_HT");
  //Sample.push_back("SysZee*_14TEV_HT");
  Sample.push_back("Zvv*_14TEV_HT");
  //Sample.push_back("Wlv*_14TEV_HT");
  Sample.push_back("Wev*_14TEV_HT");
  Sample.push_back("Wmv*_14TEV_HT");
  Sample.push_back("Wtv*_14TEV_HT");
  Sample.push_back("TT_14TEV_HT");
  //Sample.push_back("Wlv*_14TEV_HT");
  //Sample.push_back("H*_14TEV_HT");
  //Sample.push_back("Photon*_14TEV_HT");
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

  //Sample.push_back("W1Jet_8TEV_HT");
  //Sample.push_back("W2Jet_8TEV_HT");
  //Sample.push_back("W3Jet_8TEV_HT");
  //Sample.push_back("W4Jet_8TEV_HT");
  ////Sample.push_back("TT_8TEV");
  //Sample.push_back("W*Jet_8TEV_HT");
  //Sample.push_back("ZJETS_8TEV_HT");

  // The Cut flow

  order = GetCutOrder(Dir.front());

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
    std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
    std::cout << " dir " << dir <<" sample " << *Sit  << std::endl;
    HTSample temp(dir, *Sit, pileup, DEC);
    //HTSample temp(dir, *Sit, pileup, "PhaseII4");
    //HTSample temp(dir, *Sit, pileup, "PhaseI");
    //HTSample temp(dir, *Sit, pileup, "Snowmass");
    //HTSample temp(dir, *Sit, pileup, "SnowmassOpt");
    //temp.InitSample(19.6*1000);
    //temp.InitSample(500*1000);
    temp.InitSample(3000*1000);
    //TH1F* c1 = temp.GetTH1("CutFlow", true);
    //TH1F* c2 = temp.GetTH1("CutFlow", false);
    std::vector<double> itgl;
    std::vector<double> entry;

    //double orginal = c1->GetBinContent(1);

    //for (int i = 1; i <= c1->GetXaxis()->GetNbins(); ++i)
    //{
    ////std::cout << " Bin label "<< c1->GetXaxis()->GetBinLabel(i) << std::endl;
    ////if (i > (order.size()+1)) break;
    //char buf[100];
    //double error = c2->GetBinError(i); 
    //double frac = c1->GetBinContent(i)/c1->GetBinContent(i-1);

    //sprintf(buf, "%.f(%.2f)",c1->GetBinContent(i), frac);
    ////sprintf(buf, "%.f+-%.f(%.2f)",c1->GetBinContent(i) ,error, frac);
    ////sprintf(buf, "%.f(%.1e)",c1->GetBinContent(i) , c1->GetBinContent(i)/orginal  );
    //std::string temp = buf;
    //table[*Sit].push_back(temp);
    //}
    //

    for (int i = 0; i < order.size(); ++i)
    {
      std::stringstream ss;
      ss<< "MHT_" <<i;
      std::cout << "  " << ss.str();
      TH1F* c1 = temp.GetTH1("MHT", i); 
      //TH1F* c1 = temp.GetTH1(ss.str().c_str()); 
      itgl.push_back(c1->Integral());

      entry.push_back(c1->GetEntries());

      std::cout <<  "  "<< c1->Integral() << " " << c1->GetEntries() << std::endl;
    }

    for (int i = 0; i < order.size(); ++i)
    {
      char buf[100];
      double error = std::sqrt(entry.at(i));
      double frac = 0.0;
      if ( i > 0)
        frac = itgl.at(i)/itgl.at(i-1);

      //std::cout << "frac " << frac << std::endl;
      sprintf(buf, "%.f+-%.f(%.0f%%)", itgl.at(i), error, frac*100);
      //std::cout << " buff "  << buf << std::endl;
      table[*Sit].push_back(buf);


    }
  }



  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "================================================================= " << std::endl;
  std::cout <<  "[b]" <<"Detector: " << DEC << " Jet selection: " << "Pt > 30, |Eta| < 5" <<  " PileUp : " << pileup  << "[/b]"<< std::endl;
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


// ===  FUNCTION  ============================================================
//         Name:  GetCutOrder
//  Description:  return the cut order
// ===========================================================================
std::vector<std::string> GetCutOrder(std::string dir)
{
  glob_t glob_result;
  char pat[100];
  std::vector<std::string> order;
  TFile *f = NULL;


  sprintf(pat, "%s/*.root", dir.c_str());
  glob(pat, GLOB_TILDE, NULL, &glob_result);
  if (glob_result.gl_pathc > 0)
    f = new TFile(glob_result.gl_pathv[0], "R");
  else 
    return order;

  globfree(&glob_result);

  assert(f != NULL);

  TH1F* cut = (TH1F*) f->Get("CutFlow");

  for (int i = 0; i < cut->GetNbinsX(); ++i)
  {
    std::string d = cut->GetXaxis()->GetBinLabel(i+1);
    //std::cout <<i << " "<<  d << std::endl;
    order.push_back(d);
  }

  return order;

}       // -----  end of function GetCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  LatexTable
//  Description:  
// ===========================================================================
int LatexTable(std::string dir, std::string pileup, std::vector<std::string> Sample)
{
  std::map<string, std::vector<std::string> > table;

  for(std::vector<std::string>::iterator Sit=Sample.begin();
      Sit!=Sample.end(); Sit++)
  {
    std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
    std::cout << " dir " << dir <<" sample " << *Sit  << std::endl;
    HTSample temp(dir, *Sit, pileup, DEC);
    //HTSample temp(dir, *Sit, pileup, "PhaseII4");
    //HTSample temp(dir, *Sit, pileup, "PhaseI");
    //HTSample temp(dir, *Sit, pileup, "Snowmass");
    //HTSample temp(dir, *Sit, pileup, "SnowmassOpt");
    //temp.InitSample(19.6*1000);
    //temp.InitSample(500*1000);
    temp.InitSample(3000*1000);
    //TH1F* c1 = temp.GetTH1("CutFlow", true);
    //TH1F* c2 = temp.GetTH1("CutFlow", false);
    std::vector<double> itgl;
    std::vector<double> entry;

    //double orginal = c1->GetBinContent(1);

    //for (int i = 1; i <= c1->GetXaxis()->GetNbins(); ++i)
    //{
    ////std::cout << " Bin label "<< c1->GetXaxis()->GetBinLabel(i) << std::endl;
    ////if (i > (order.size()+1)) break;
    //char buf[100];
    //double error = c2->GetBinError(i); 
    //double frac = c1->GetBinContent(i)/c1->GetBinContent(i-1);

    //sprintf(buf, "%.f(%.2f)",c1->GetBinContent(i), frac);
    ////sprintf(buf, "%.f+-%.f(%.2f)",c1->GetBinContent(i) ,error, frac);
    ////sprintf(buf, "%.f(%.1e)",c1->GetBinContent(i) , c1->GetBinContent(i)/orginal  );
    //std::string temp = buf;
    //table[*Sit].push_back(temp);
    //}
    //

    for (int i = 0; i < order.size(); ++i)
    {
      std::stringstream ss;
      ss<< "MHT_" <<i;
      std::cout << "  " << ss.str();
      TH1F* c1 = temp.GetTH1("MHT", i); 
      //TH1F* c1 = temp.GetTH1(ss.str().c_str()); 
      itgl.push_back(c1->Integral());

      entry.push_back(c1->GetEntries());

      std::cout <<  "  "<< c1->Integral() << " " << c1->GetEntries() << std::endl;
    }

    for (int i = 0; i < order.size(); ++i)
    {
      char buf[100];
      double error = std::sqrt(entry.at(i));
      double frac = 0.0;
      if ( i > 0)
        frac = itgl.at(i)/itgl.at(i-1);

      //std::cout << "frac " << frac << std::endl;
      sprintf(buf, "%.f+-%.f(%.0f%%)", itgl.at(i), error, frac*100);
      //std::cout << " buff "  << buf << std::endl;
      table[*Sit].push_back(buf);


    }
  }



  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << "================================================================= " << std::endl;
  std::cout <<  "[b]" <<"Detector: " << DEC << " Jet selection: " << "Pt > 30, |Eta| < 5" <<  " PileUp : " << pileup  << "[/b]"<< std::endl;
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
}       // -----  end of function LatexTable  -----
