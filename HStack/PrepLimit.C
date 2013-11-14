// ===========================================================================
// 
//       Filename:  PrepLimit.C
// 
//    Description:  Code to prepare the limit input data
// 
//  # Simple counting experiment, with one signal and a few background processes 
//  # Simplified version of the 35/pb H->WW analysis for mH = 160 GeV
//  imax 1  number of channels
//  jmax 3  number of backgrounds
//  kmax 4  number of nuisance parameters (sources of systematical uncertainties)
//  ------------
//  # we have just one channel, in which we observe 0 events
//  bin 1
//  observation 0
//  ------------
//  # now we list the expected events for signal and all backgrounds in that bin
//  # the second 'process' line must have a positive number for backgrounds, and 0 for signal
//  # then we list the independent sources of uncertainties, and give their effect (syst. error)
//  # on each process and bin
//  bin              1     1     1     1
//  process         ggH  qqWW  ggWW  others
//  process          0     1     2     3
//  rate           136.157  1635.79  1438.65  0.22
//  ------------
//  lumi    lnN    1.11    -   1.11    -    lumi affects both signal and gg->WW (mc-driven). lnN = lognormal
//  xs_ggH  lnN    1.16    -     -     -    gg->H cross section + signal efficiency + other minor ones.
//  xs_ggWW lnN      -     -   1.50    -    50% uncertainty on gg->WW cross section
//  bg_others lnN    -     -     -   1.30   30% uncertainty on the rest of the backgrounds
//
//        Version:  1.0
//        Created:  08/12/2013 10:35:38 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================


#include <cstdlib>
#include "HTSample.h"
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

//const std::string Dir = "./ECFA/";
//const std::string Dir = "./Snowmass/";
//const std::string Dir = "./Preapproval/";
//const std::string Dir = "./LPC_MHTCUT/";
const std::string Dir = "./ECFA_200/";
//const std::string Dir = "./TOOL/";
std::string PU  = "140PileUp";
//std::string PU  = "NoPileUp";
//std::string Dec = "PhaseI";
std::string Dec = "PhaseII4v2";
//std::string Dec = "Snowmass";
//const std::string Dec = "SnowmassOpt";

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

struct Systematic
{
  Process pro;
  std::string proname;

  Systematic(std::string label_, std::string pro_, std::string filename_, bool IsSignal_)
    :pro(label_, filename_, IsSignal_), proname(pro_)
  {
  };
};

struct RateSys
{
  std::string label;
  std::string proname;
  double rateuncern;
  RateSys(std::string label_, std::string proname_, double rateuncern_):
   label(label_),proname(proname_), rateuncern(rateuncern_){};
};



std::vector<std::string> GetCutOrder(std::string dir);
bool PrintRate(std::ofstream &outfile, int Nbin, std::vector<Process> vPro, 
    std::map<std::string, std::vector<double> > rate);
bool PrintSys(std::ofstream &outfile, int Nbin, std::vector<Process> vPro,  
    std::vector<Systematic> vSys, std::map<std::string, std::vector<double> > rate);
bool PrintRateSys(std::ofstream &outfile, int Nbin, std::vector<Process> vPro,  std::vector<RateSys> vRate);
int PrintBinSB(int Nbin, std::vector<Process> vPro, std::map<std::string, std::vector<double> > rate);
int GetHisIdx(std::string cut);
int GetHisIdx(std::string cut, HTSample* HT);

int main ( int argc, char *argv[] )
{
//----------------------------------------------------------------------------
//  All the input variables for controlling
//----------------------------------------------------------------------------
  //int Nbin = 1;
  int Nbin = 10;
  //const double Lumi = 900;
  //const double Lumi = 500;
  const double Lumi = 3000;
  const bool WithEle = false;
  const std::string hisname = "MHT";
  //const std::string cutname = "NoCut";
  //const std::string cutname = "CTLep";
  const std::string cutname = "CTMet200";
  //const std::string cutname = "AllCut";

  //const std::string Signal  = "Wino500";
  //const std::string Signal  = "Wino200";
  //const std::string Signal  = "Wino100";
  std::string Signal  = argv[1];
  PU = argv[2];
  Dec = argv[3];
  int PLATEU = atoi(argv[4]);

  std::stringstream temp;
  //temp << "3000fb_Met200" << "_"<< 1 << "_" << Signal <<"_" << PU<<".txt";
  //temp <<Lumi <<"fb_MetAll_5p_" << Signal <<"_" << PU <<"_"<< Dec <<"_" << cutname<<"_WithE.txt";
  temp <<Lumi <<"fb_NewDefault_"<< PLATEU << "p_" << Signal <<"_" << PU <<"_"<< Dec <<"_" << cutname<<"_Wlv.txt";
  //temp <<"test_"<< Lumi <<"fb_MetAll_1p_" << Signal <<"_" << PU <<"_"<< Dec <<"_" << cutname<<".txt";
  //temp << "3000fb_MetAll" << "_"<< 1 << "_" << Signal <<"_" << PU<<".txt";
  std::cout << "temp " << temp.str() << std::endl;
  std::ofstream outfile(temp.str().c_str(), std::ofstream::out);

//----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------
  std::vector<Process> vPro;
  std::vector<Systematic> vSys;
  std::vector<RateSys> vRate;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Process ~~~~~
  Process Wino100("Wino100", "Wino100_14TeV", true);
  Process Wino200("Wino200", "Wino200_14TeV", true);
  Process Wino500("Wino500", "Wino500_14TeV", true);
  Process TT("TT", "TT_14TEV_HT", false);
  Process Wev("Wev", "Wev*_14TEV_HT", false);
  Process Wmv("Wmv", "Wmv*_14TEV_HT", false);
  Process Wtv("Wtv", "Wtv*_14TEV_HT", false);
  Process Zvv("Zvv", "Zvv*_14TEV_HT", false);
  //Process LL("LL", "Zvv*_14TEV_HT", false);

  if (Signal == "Wino100")
  {
    vPro.push_back(Wino100);
  }

  std::cout << " signal " << Signal << std::endl;

  if (Signal == "Wino200")
  {
    vPro.push_back(Wino200);
  }
  if (Signal == "Wino500") 
  {
    vPro.push_back(Wino500);
  }

  vPro.push_back(TT);
  vPro.push_back(Wev);
  vPro.push_back(Wmv);
  vPro.push_back(Wtv);
  vPro.push_back(Zvv);
  //vPro.push_back(LL);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Systematic from control ~~~~~
  //Systematic SWev("Sys_Wev", "Wev", "SysWev*_14TEV_HT", false);
  Systematic SWmv("Sys_Wmv", "Wev|Wmv|Wtv|Zvv", "SysWmv*_14TEV_HT", false);
  //Systematic SWmv("Sys_Wmv", "Wev|Wmv|Wtv", "SysWmv*_14TEV_HT", false);
  //Systematic SWtv("Sys_Wtv", "Wtv", "Sys_Wmv*_14TEV_HT", false);
  //Systematic SZvv("Sys_Zvv", "Zvv", "SysZmm*_14TEV_HT", false);

  //vSys.push_back(SWev);
  vSys.push_back(SWmv);
  //vSys.push_back(SWtv);
  //vSys.push_back(SZvv);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Rate systematic ~~~~~
  RateSys RWino200("MC_Wino", "Wino200|Wino100|Wino500", 1.06);
  RateSys RTT("MC_TT", "TT", 1.06);
  double plateu = 1;
  if (PLATEU == 1) plateu = 1.01;
  if (PLATEU == 5) plateu = 1.05;
  if (PLATEU == 10) plateu = 1.10;
  //double plateu = 1.01;
  RateSys RWtv("LepID_tau", "Wtv", plateu);
  RateSys RWev("LepID_ele", "Wev", plateu);
  //RateSys RWmv("LepID_muon", "Wmv", plateu);
  RateSys RWmv("LepID_muon", "Wev|Wmv|Wtv", plateu);
  RateSys RZvv("LepID_muon2", "Zvv", plateu);

  vRate.push_back(RWino200);
  vRate.push_back(RWtv);
  vRate.push_back(RWev);
  vRate.push_back(RWmv);
  vRate.push_back(RZvv);


//----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------
  int nbk = 0;
  int nsg = 0;

  std::map<std::string, std::vector<double> > Prorate;
  std::map<std::string, std::vector<double> > Sysrate;

  BOOST_FOREACH(Process Pro, vPro)
  {
    Pro.HT->InitSample(Lumi * 1000);
    std::cout << " Pro" << Pro.label << std::endl;
    TH1F* temp = Pro.HT->GetTH1(hisname, GetHisIdx(cutname, Pro.HT));

 
    //double val = temp->Integral(temp->FindBin(MetCut), temp->GetNbinsX());
    //Prorate[Pro.label].push_back(val);

    int rebin = temp->GetNbinsX() / Nbin;
    temp->Rebin(rebin);

    //if (Pro.label == "TT" || Pro.label == "Wev" 
        //|| Pro.label == "Wmv" || Pro.label == "Wtv" )
    //{
      //temp -> Scale( 0.333);
    //}

    for (int i = 1; i <= Nbin; ++i)
    {
      Prorate[Pro.label].push_back(temp->GetBinContent(i));
    }

    if (Pro.IsSignal ) nsg++;
    else nbk++;

  }

  BOOST_FOREACH(Systematic Sys, vSys)
  {
    Sys.pro.HT->InitSample(Lumi * 1000);
    TH1F* temp = Sys.pro.HT->GetTH1(hisname, GetHisIdx(cutname, Sys.pro.HT));

//----------------------------------------------------------------------------
//  Combine with Electron sample
//---------------------------------------------------------------------------
    if (WithEle)
    {
      TString  EleSample = Sys.pro.filename;
      EleSample.ReplaceAll('m', 'e');

      HTSample HT(Dir, EleSample.Data(), PU, Dec);
      HT.InitSample(Lumi * 1000);
      TH1F* temp2 = HT.GetTH1(hisname, GetHisIdx(cutname, &HT));
      temp->Add(temp2);
    }


    //double val = temp->Integral(temp->FindBin(MetCut), temp->GetNbinsX());
    //Sysrate[Sys.pro.label].push_back(val);

    int rebin = temp->GetNbinsX() / Nbin;
    temp->Rebin(rebin);
    for (int i = 1; i <= Nbin; ++i)
    {
      Sysrate[Sys.pro.label].push_back(temp->GetBinContent(i));
    }
  }
//----------------------------------------------------------------------------
//  Remove empty bins, higgs tools can't handle empty bins
//----------------------------------------------------------------------------
  std::vector<int> emptyidx;
  for (int i = 0; i < Nbin; ++i)
  {
    bool allempty = true;
    BOOST_FOREACH(Process Pro, vPro)
    {
      if (Prorate[Pro.label].at(i) != 0) 
      {
        allempty = false;
        break;
      }
    }
    if (allempty)
    {
      emptyidx.push_back(i); 
    }
  }

  for (int i = emptyidx.size()-1; i >= 0; --i)
  {

    //std::cout << " i"  <<i << std::endl;
    BOOST_FOREACH(Process Pro, vPro)
    {
      Prorate[Pro.label].erase(Prorate[Pro.label].begin() + i);
    }
    BOOST_FOREACH(Systematic Sys, vSys )
    {
      Sysrate[Sys.pro.label].erase(Sysrate[Sys.pro.label].begin() + i);
    }
  }

  //std::cout <<  " orignal Nbin " << Nbin << std::endl;
  Nbin = Prorate["TT"].size();
  //Nbin = Prorate["Wino200"].size();
  //std::cout <<  "not orignal Nbin " << Nbin << std::endl;

  outfile <<" imax  " << Nbin << std::endl;
  outfile <<" jmax  " << nbk << std::endl;
  outfile <<" kmax  *" << std::endl;
  outfile <<" Shape  * * Fake" << std::endl;

  PrintRate(outfile, Nbin, vPro, Prorate);
  PrintSys(outfile, Nbin, vPro, vSys, Sysrate);
  PrintRateSys(outfile, Nbin, vPro, vRate);

  PrintBinSB(Nbin, vPro, Prorate);
  outfile.close();
  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

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
//         Name:  GetHisIdx
//  Description:  /* cursor */
// ===========================================================================
int GetHisIdx(std::string cut) 
{
  std::vector<std::string> order = GetCutOrder(Dir);
  int hisIdx = -1;

  std::vector<std::string>::iterator cit =std::find(order.begin(), order.end(), cut.c_str());
  if (cit == order.end())
  {
    std::cout << " Can't find this Cut !! Exiting... " << std::endl;
    return 0;
  } else hisIdx = static_cast<int>(std::distance(order.begin(), cit));

  return hisIdx;
}       // -----  end of function GetHisName  -----

// ===  FUNCTION  ============================================================
//         Name:  PrintRate
//  Description:  /* cursor */
// ===========================================================================
bool PrintRate(std::ofstream &outfile, int Nbin, std::vector<Process> vPro, std::map<std::string, std::vector<double> > rate)
{
  std::stringstream obsbin, obsrate, probin, prolabel, proidx, prorate;
  int outwid = 20;

  obsbin   <<" bin          ";
  obsrate  <<" observation  ";
  probin   <<" bin          ";
  prolabel <<" process      ";
  proidx   <<" process      ";
  prorate  <<" rate         ";

  for (int i = 0; i < Nbin; ++i)
  {
    obsbin << std::setw(outwid) << std::left << i+1;

    int sigidx = 0;
    int bkidx = 1;
    double obs = 0;
    for (int j = 0; j < vPro.size(); ++j)
    {
      probin << std::setw(outwid) << std::left << i+1;
      prolabel<< std::setw(outwid)  << std::left << vPro.at(j).label;
      if (vPro.at(j).IsSignal)
        proidx << std::setw(outwid) << std::left  << sigidx--;
      else
        proidx << std::setw(outwid) << std::left  << bkidx++;
      
      prorate << std::setw(outwid) << std::left << std::fixed << std::setprecision(1) << rate[vPro.at(j).label].at(i);
      //obs += rate[vPro.at(j).label].at(i);
      if (!vPro.at(j).IsSignal) obs += rate[vPro.at(j).label].at(i);
    }

    //obsrate << std::setw(outwid) << std::left << 0;
    obsrate << std::setw(outwid) << std::left << obs;
  }

  //std::cout <<  obsbin.str() << std::endl;
  //std::cout <<  obsrate.str() << std::endl;
  //std::cout <<  probin.str() << std::endl;
  //std::cout <<  prolabel.str() << std::endl;
  //std::cout <<  proidx.str() << std::endl;
  //std::cout <<  prorate.str() << std::endl;

  outfile <<  obsbin.str() << std::endl;
  outfile <<  obsrate.str() << std::endl;
  outfile <<  probin.str() << std::endl;
  outfile <<  prolabel.str() << std::endl;
  outfile <<  proidx.str() << std::endl;
  outfile <<  prorate.str() << std::endl;

  return true;
}       // -----  end of function PrintRate  -----

// ===  FUNCTION  ============================================================
//         Name:  PrintSys
//  Description:  
// ===========================================================================
bool PrintSys(std::ofstream &outfile, int Nbin, std::vector<Process> vPro,  std::vector<Systematic> vSys, std::map<std::string, std::vector<double> > rate)
{
  
  std::stringstream out;
  int outwid = 20;

//----------------------------------------------------------------------------
//  Bin-by-Bin uncorrelated
//----------------------------------------------------------------------------
  for (int k = 0; k < Nbin; ++k)
  {
    BOOST_FOREACH(Systematic Sys, vSys)
    {

      out.str("");
      out << " " << Sys.pro.label << "_" << k
      //out << " " << std::setw(outwid) << std::left << Sys.pro.label << "_" << k
        << std::setw(outwid) << std::left << " lnN";

      for (int i = 0; i < Nbin; ++i)
      {

        for (int j = 0; j < vPro.size(); ++j)
        {
          if ( Sys.proname.find(vPro.at(j).label) != std::string::npos 
            && i == k )
            //if (vPro.at(j).label == Sys.proname )
          {
            double rt = std::sqrt(rate[Sys.pro.label].at(i));
            //std::cout << Sys.pro.label << " i " << i << " rt " << rt  << std::endl;
            double frac = 0;
            if (rate[Sys.pro.label].at(i) != 0)
              frac = rt /rate[Sys.pro.label].at(i);
            out << std::setw(outwid) << std::left << 1+frac;
          }
          else
            out << std::setw(outwid) << std::left << "-";
        }
      }

      //std::cout << out.str() << std::endl;
      outfile << out.str() << std::endl;
    }

  }

//----------------------------------------------------------------------------
  //Bin-by-Bin correlated
//----------------------------------------------------------------------------
  //BOOST_FOREACH(Systematic Sys, vSys)
  //{

    //out.str("");
    //std::cout << "====================" << Sys.pro.label << std::endl;
    //out << " " << Sys.pro.label 
      ////out << " " << std::setw(outwid) << std::left << Sys.pro.label << "_" << k
      //<< std::setw(outwid) << std::left << " lnN";

    //for (int i = 0; i < Nbin; ++i)
    //{

      //for (int j = 0; j < vPro.size(); ++j)
      //{
        //if ( Sys.proname.find(vPro.at(j).label) != std::string::npos )
          ////if (vPro.at(j).label == Sys.proname )
        //{
          //double rt = std::sqrt(rate[Sys.pro.label].at(i));
          ////std::cout << Sys.pro.label << " i " << i << " rt " << rt  << std::endl;
          //double frac = 0;
          //if (rate[Sys.pro.label].at(i) != 0)
            //frac = rt /rate[Sys.pro.label].at(i);
          //out << std::setw(outwid) << std::left << 1+frac;
        //}
        //else
          //out << std::setw(outwid) << std::left << "-";
      //}
    //}
     //outfile << out.str() << std::endl;
  //}

  return true;
}       // -----  end of function PrintSys  -----

// ===  FUNCTION  ============================================================
//         Name:  PrintRateSys
//  Description:  
// ===========================================================================
bool PrintRateSys(std::ofstream &outfile, int Nbin, std::vector<Process> vPro,  std::vector<RateSys> vRate)
{
  
  std::stringstream out;
  int outwid = 20;


    BOOST_FOREACH(RateSys rate, vRate)
    {

      out.str("");
      out << " " << std::setw(outwid) << std::left << rate.label
      << std::setw(outwid) << std::left << " lnN";

      for (int i = 0; i < Nbin; ++i)
      {
        for (int j = 0; j < vPro.size(); ++j)
        {
          if ( rate.proname.find(vPro.at(j).label) != std::string::npos )
            //if (vPro.at(j).label == Sys.proname )
          {
            out << std::setw(outwid) << std::left << rate.rateuncern;
          }
          else
            out << std::setw(outwid) << std::left << "-";
        }
      }

       //std::cout << out.str() << std::endl;
       outfile << out.str() << std::endl;
  }
  return true;
}       // -----  end of function PrintRateSys  -----

// ===  FUNCTION  ============================================================
//         Name:  Get2DProject
//  Description:  
// ===========================================================================
//TH1F* Get2DProject(HTSample *HT, std::string hisname, std::string cutname)
//{

  //TH2D* th2d = HT->GetTH2D(hisname, GetHisIdx(cutname));
  //return th2d->ProjectionX()

//}       // -----  end of function Get2DProject  -----
//
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
    if (d == cut )
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
//


// ===  FUNCTION  ============================================================
//         Name:  PrintBinSB
//  Description:  
// ===========================================================================
int PrintBinSB(int Nbin, std::vector<Process> vPro, std::map<std::string, std::vector<double> > rate)
{

  std::vector<std::stringstream> vStream;
  std::stringstream obsbin;

  int ProSize = vPro.size();
  int outwid = 20;
  obsbin   <<" bin          ";

  for (int i = 0; i < vPro.size(); ++i)
  { 
    vStream.at(i) << " " << vPro.at(i).label <<" &" ;
  }

  std::vector<double> vBKs;

  for (int i = 0; i < Nbin; ++i)
  {
    obsbin << std::setw(outwid) << std::left << i+1;

    double obs = 0;
    for (int j = 0; j < vPro.size(); ++j)
    {
      
      vStream.at(i)  << std::setw(outwid) << std::left << std::fixed << std::setprecision(1) << rate[vPro.at(j).label].at(i) <<" & ";
      if (!vPro.at(j).IsSignal) obs += rate[vPro.at(j).label].at(i);
    }
    vBKs.push_back(obs);
  }

  for (int i = 0; i < vPro.size(); ++i)
  { 
    if (vPro.at(i).IsSignal)
    {
    vStream.at(i) << " " << vPro.at(i).label <<" &" ;
    }
  }

  for (int i = 0; i < vStream.size(); ++i)
  {
    
      std::cout << vStream.at(i).str() << std::endl;
  }
  
  return 1;
}       // -----  end of function PrintBinSB  -----

