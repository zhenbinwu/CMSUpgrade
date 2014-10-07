// ===========================================================================
// 
//       Filename:  CombingBkg.C
// 
//    Description:  A code to combine all the backgrounds so that it would
//    be easier for significance
// 
//        Version:  1.0
//        Created:  10/03/2014 06:08:29 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#include <cstdlib>
#include "HTSample.h"
#include <functional>
#include "TKey.h"
#include <map>
#include <iostream>
#include <fstream>
#include <cmath>
#include <iterator>
#include <sstream>
#include <algorithm>
#include <functional>
#include <iomanip>
#include <memory>
#include <cstdlib>

const std::string& Dir = "./0PUSep24/";
const std::string& PU  = "NoPileUp";
const std::string& Dec = "Snowmass";
const std::string& Var = "MHT";
const int type         = 1;             // 0 bin by bin, 1, lower bound , -1, upper bound
const bool SaveOutput  = true;

struct Process
{
  std::string label;
  std::string filename;
  bool IsSignal;
  HTSample* HT;

  Process(std::string label_, std::string filename_, bool IsSignal_)
    :label(label_), filename(filename_), IsSignal(IsSignal_)
  {
    HT = new HTSample(Dir, filename, PU, Dec);
  }
};

std::vector<std::string> GetAnalysis(const std::string& Dir, const std::string& PU);
std::vector<double> GetValues(int type, std::vector<std::pair<double, double> > values);
int GetHisIdx(std::string cut, HTSample* HT);
std::vector<std::string> GetCutOrder(const std::string& dir, const std::string& ana);
int PrintBinSB(std::vector<Process> vPro, std::map<std::string, std::vector<std::pair<double, double> > > rate);
double SBWithSys(double sig, double background, double sys);
bool CalBinSB(std::vector<Process> vPro, std::map<std::string, std::vector<std::pair<double, double> > > rate);

// ===  FUNCTION  ============================================================
//         Name:  main
//  Description:  
// ===========================================================================
int main ( int argc, char *argv[] )
{
  std::vector<std::string> Ana = GetAnalysis(Dir, PU);
  const double Lumi = 3000;
  const int cutidx = 1; // Inverse order, 0 means the last cut
  const int Nbin = 10; // Inverse order, 0 means the last cut

  std::vector<Process> vPro;
  vPro.push_back(Process("Sbottom300", "Sbottom300_QCD4QED4_14TEV", true));
  //vPro.push_back(Process("Sbottom500", "Sbottom500_QCD4QED4_14TEV", true));
  vPro.push_back(Process("B", "B*_14TEV_HT", false));
  vPro.push_back(Process("BB", "BB_14TEV_HT", false));
  vPro.push_back(Process("BBB", "BBB_14TEV_HT", false));
  vPro.push_back(Process("H", "H_14TEV_HT", false));
  vPro.push_back(Process("TT", "TT_14TEV_HT", false));
  vPro.push_back(Process("TB", "TB_14TEV_HT", false));
  vPro.push_back(Process("TJ", "TJ_14TEV_HT", false));

  TFile *outfile = NULL;
  if (SaveOutput)
  {
    std::string outname = Dec +"_"+PU +".root";
    outfile = new TFile(outname.c_str(), "RECREATE");
  }

  BOOST_FOREACH(std::string ana, Ana)
  {
    std::cout << "Ana " << ana << std::endl;
    std::map<std::string, std::vector<std::pair<double, double> > > Prorate;
    std::vector<std::string> cutorder = GetCutOrder(Dir, ana);
    std::string cutname  = *(cutorder.rbegin() + cutidx);
    std::cout << "Cut Name " << cutname << std::endl;

    BOOST_FOREACH(Process Pro, vPro)
    {
      Pro.HT->InitSample(Lumi * 1000);
      Pro.HT->SetAnalysis(ana);
      TH1F* temp = Pro.HT->GetTH1(Var, GetHisIdx(cutname, Pro.HT));

      int rebin = temp->GetNbinsX() / Nbin;
      temp->Rebin(rebin);

      for (int i = 1; i <= temp->GetNbinsX()+1; ++i)
      {
        Prorate[Pro.label].push_back(std::make_pair(temp->GetBinLowEdge(i), temp->GetBinContent(i)));
      }
      //if (Pro.IsSignal ) nsg++;
      //else nbk++;
      if (outfile != NULL)
      {
        std::stringstream ss;
        ss << Pro.label <<"_" << ana <<"_" << Var <<"_" << cutidx;
        //ss << Dec  << "_" << PU << "_" << Pro.label <<"_" << ana <<"_" << Var;
        TH1F* savehis = (TH1F*)temp->Clone(ss.str().c_str());
        outfile->cd();
        savehis->Write();
      }
    }

    PrintBinSB(vPro, Prorate);
    CalBinSB(vPro, Prorate);
  }

  outfile->Close();

/*
 *  std::vector<std::string> Sample;
 *  Sample.push_back("B*_14TEV_HT");
 *  //Sample.push_back("BJ_14TEV_HT");
 *  //Sample.push_back("BJJ_14TEV_HT");
 *  Sample.push_back("BB_14TEV_HT");
 *  Sample.push_back("BBB_14TEV_HT");
 *  Sample.push_back("H_14TEV_HT");
 *  //Sample.push_back("LL_14TEV_HT");
 *  //Sample.push_back("LLB_14TEV_HT");
 *  Sample.push_back("TB_14TEV_HT");
 *  Sample.push_back("TJ_14TEV_HT");
 *  Sample.push_back("TT_14TEV_HT");
 *
 *  order = GetCutOrder(Dir.front(), Ana);
 *
 *  for(std::vector<std::string>::iterator Dit=Dir.begin();
 *      Dit!=Dir.end(); Dit++)
 *  {
 *    for(std::vector<std::string>::iterator Pit=PU.begin();
 *        Pit!=PU.end(); Pit++)
 *    {
 *      //ElogTable(*Dit, *Pit, Sample);
 *      LatexTable(*Dit, *Pit, Sample);
 *      LatexSB(*Dit, *Pit, Sample);
 *
 *      //for(std::vector<std::string>::iterator Sit=Sample.begin();
 *          //Sit!=Sample.end(); Sit++)
 *      //{
 *        ////CutTable(*Dit, *Sit, *Pit);
 *      //}
 *
 *    }
 *  }
 */
  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

// ===  FUNCTION  ============================================================
//         Name:  GetAnalysis
//  Description:  
// ===========================================================================
std::vector<std::string> GetAnalysis(const std::string& Dir, const std::string& PU)
{
  glob_t glob_result;
  char pat[100];
  std::vector<std::string> anas;
  TFile *f = NULL;


  sprintf(pat, "%s/*%s*.root", Dir.c_str(), PU.c_str());
  glob(pat, GLOB_TILDE, NULL, &glob_result);
  if (glob_result.gl_pathc > 0)
    f = new TFile(glob_result.gl_pathv[0], "R");
  else 
    return anas;

  globfree(&glob_result);

  assert(f != NULL);

   TIter nextkey(f->GetListOfKeys());
   TKey *key;
   while ((key = (TKey*)nextkey())) {
     if (strcmp(key->GetClassName(), "TDirectoryFile") == 0 ) 
       anas.push_back(key->GetName());
   }

   return anas;
}       // -----  end of function GetAnalysis  -----




//// ===  FUNCTION  ============================================================
////         Name:  GetHists
////  Description:  
//// ===========================================================================
//template <class T>
//std::map<std::string, *T> GetHists(std::string& Dir, std::string& PU)
//{
  

  //return true;
//}       // -----  end of function GetHists  -----
//

// ===  FUNCTION  ============================================================
//         Name:  GetHisIdx
//  Description:  /* cursor */
// ===========================================================================
int GetHisIdx(std::string cut, HTSample* HT) 
{

  int hisIdx = -1;
  TH1F* cuthist = (TH1F*)HT->GetTH1("CutFlow");
  for (int i = 0; i < cuthist->GetNbinsX(); ++i)
  {
    std::string d = cuthist->GetXaxis()->GetBinLabel(i+1);
    if (d == cut)
    {
      hisIdx = i;
      break;
    }
  }

  if (hisIdx == -1)
  {
    std::cout << " Can't find this Cut !! Exiting... " << std::endl;
    return 0;
  } else return hisIdx;

}       // -----  end of function GetHisName  -----

// ===  FUNCTION  ============================================================
//         Name:  GetCutOrder
//  Description:  return the cut order
// ===========================================================================
std::vector<std::string> GetCutOrder(const std::string& dir, const std::string& ana)
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

  TString cutflow = ana + "/CutFlow";
  TH1F* cut = (TH1F*) f->Get(cutflow);

  for (int i = 0; i < cut->GetNbinsX(); ++i)
  {
    std::string d = cut->GetXaxis()->GetBinLabel(i+1);
    order.push_back(d);
  }

  return order;

}       // -----  end of function GetCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  PrintBinSB
//  Description:  
// ===========================================================================
int PrintBinSB(std::vector<Process> vPro,std::map<std::string, std::vector<std::pair<double, double> > > rate)
{

  std::vector<std::string> vStream;
  std::stringstream obsbin;

  int ProSize = vPro.size();
  int outwid = 20;
  obsbin   <<" bin          ";

  for (int i = 0; i < ProSize; ++i)
    vStream.push_back("");

  for (int i = 0; i < ProSize; ++i)
  { 
    std::stringstream ss;
    ss << " " << vPro.at(i).label <<" &" ;
    vStream.at(i) += ss.str();
  }

  std::vector<double> vBKs;

  for (unsigned int i = 0; i < rate.begin()->second.size(); ++i)
  {
    obsbin << std::setw(outwid) << std::left << i+1;

    double obs = 0;
    for (unsigned int j = 0; j < vPro.size(); ++j)
    {
      std::stringstream ss;
      ss  << std::setw(outwid) << std::left << std::fixed << std::setprecision(1) << rate[vPro.at(j).label].at(i).second <<" & ";
      vStream.at(j) += ss.str();
      if (!vPro.at(j).IsSignal) obs += rate[vPro.at(j).label].at(i).second;
    }
    vBKs.push_back(obs);
  }

  for (unsigned int i = 0; i < vPro.size(); ++i)
  { 
    if (vPro.at(i).IsSignal)
    {

      std::stringstream ss;
      ss  << " " << vPro.at(i).label <<" &" ;
      vStream.at(i)+= ss.str();
    }
  }

  for (unsigned int i = 0; i < vStream.size(); ++i)
  {
    std::cout << vStream.at(i) << std::endl;
  }



  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  //----------------------------------------------------------------------------
  //  Calculate significant
  //----------------------------------------------------------------------------
  for (unsigned int i = 0; i < vPro.size(); ++i)
  { 
    if (vPro.at(i).IsSignal)
    {
      std::string SigName = vPro.at(i).label;
      std::cout << " size " << rate[SigName].size() << std::endl;
      std::cout << " signal " << SigName ;
      for (unsigned int j = 0; j < rate[SigName].size(); ++j)
      {
        std::cout <<  "  " << rate[SigName].at(j).second << "  bk " <<  vBKs.at(j) << std::endl;
      }
      std::cout << std::endl;
    }
  }


  return 1;
}       // -----  end of function PrintBinSB  -----

// ===  FUNCTION  ============================================================
//         Name:  CalBinSB
//  Description:  
// ===========================================================================
bool CalBinSB(std::vector<Process> vPro, std::map<std::string, std::vector<std::pair<double, double> > > rate)
{

  std::vector<double> vBKs;
  std::map<std::string, std::vector<double> > vSGs;

  for (unsigned j = 0; j < rate.begin()->second.size(); ++j)
  {
    vBKs.push_back(0.0);
  }


  for (unsigned int j = 0; j < vPro.size(); ++j)
  {
    if (vPro.at(j).IsSignal) vSGs[vPro.at(j).label] = GetValues(type, rate[vPro.at(j).label]);
    else
    {
      std::vector<double> temp=GetValues(type, rate[vPro.at(j).label]);
      assert(vBKs.size() == temp.size());
      std::transform(vBKs.begin(), vBKs.end(), temp.begin(), vBKs.begin(), std::plus<double>());
    }
  }

  //----------------------------------------------------------------------------
  //  Calculate significant
  //----------------------------------------------------------------------------
  //
  for(std::map<std::string, std::vector<double> >::const_iterator it=vSGs.begin();
    it!=vSGs.end(); ++it)
  {
    std::cout << std::endl;
    std::cout << std::endl;
    std::cout << "================================================================= " << std::endl;
    std::cout << " Signal: " << it->first <<"  on " << Var <<std::endl;
    std::cout <<  "\\begin{tabular}{|c|";
    for (int i = 0; i < 5; ++i)
    {
      std::cout <<  "c|";
    }
    std::cout << "}" << std::endl;

    std::cout << "\\hline" << std::endl;

    // The first row of name
    std::cout <<  "\\textbf{Edge} & \\textbf{Signal} & \\textbf{Background} & \\textbf{S/B} & \\textbf{Sig} & \\textbf{5\\\%} \\\\" << std::endl;
    std::cout << "\\hline" << std::endl;
    // Start to fill in the Cut flow
    for (unsigned int i = 0; i < rate[it->first].size(); ++i)
    {

      double s = vSGs[it->first].at(i);
      double b = vBKs.at(i);
      std::cout << " \\textit{" << rate[it->first].at(i).first<< "} & ";
      //std::cout << " " <<std::setw(3) << s << " & ";
      std::cout << " " << s << " & ";
      //std::cout << " " <<std::setprecision(3) << s << " & ";
      std::cout << " " << b << " & ";
      std::cout << " " << s/b << " & ";
      std::cout << " " << SBWithSys(s, b, 0) << " & ";
      std::cout << " " << SBWithSys(s, b, 0.05) << " ";
      std::cout << " \\\\ " << std::endl;
    }
    std::cout << "\\hline" << std::endl;
    std::cout <<  "\\end{tabular}" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;

  }

  //std::cout << "\\hline" << std::endl;
  //std::cout <<  "\\end{tabular}" << std::endl;
  //std::cout << std::endl;
  //std::cout << std::endl;
  ////
  ////
  //for (unsigned int i = 0; i < vPro.size(); ++i)
  //{ 
    //if (vPro.at(i).IsSignal)
    //{
      //std::string SigName = vPro.at(i).label;
      //std::cout << " size " << rate[SigName].size() << std::endl;
      //std::cout << " signal " << SigName ;
      //for (unsigned int j = 0; j < rate[SigName].size(); ++j)
      //{
        //std::cout <<  "  " << rate[SigName].at(j).second << "  bk " <<  vBKs.at(j) << std::endl;
      //}
      //std::cout << std::endl;
    //}
  //}

  return true;
}       // -----  end of function CalBinSB  -----

// ===  FUNCTION  ============================================================
//         Name:  GetValues
//  Description:  
// ===========================================================================
std::vector<double> GetValues(int type, std::vector<std::pair<double, double> > values)
{
  std::vector<double> rvalues;

  for (unsigned i = 0; i < values.size(); ++i)
  {

    if (type == 0) rvalues.push_back(values.at(i).second);
    if (type == 1)
    {
      double val = 0;
      for (unsigned j = i; j < values.size(); ++j)
        val += values.at(j).second;
      rvalues.push_back(val);
    }
    if (type == -1)
    {
      double val = 0;
      for (unsigned j = 0; j <= i; ++j)
        val += values.at(j).second;
      rvalues.push_back(val);
    }
  }

  return rvalues;
}       // -----  end of function GetValues  -----

// ===  FUNCTION  ============================================================
//         Name:  SBWithSys
//  Description:  
// ===========================================================================
double SBWithSys(double sig, double background, double sys)
{
  //double sys = 0; // 10% systematics
  //double sys = 0.01; // 10% systematics
  //double sys = 0.1; // 10% systematics

  double domi = background + pow(sys*background, 2);
  return sig/sqrt(sig+domi);
  
}       // -----  end of function SBWithSys  -----
