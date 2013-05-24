// ===========================================================================
// 
//       Filename:  HistComp.cc
// 
//    Description:  This is the code to make pretty comparison among files.
// 
//        Version:  1.0
//        Created:  05/17/2013 10:07:32 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================


#include <cstdlib>
#include "StopStyle.hh"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TLegend.h"
#include "TKey.h"
#include "TRint.h"
#include "TLine.h"
#include "TStyle.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;
typedef std::map<std::string, std::string> Tmaps;
typedef std::map<std::string, std::string>::iterator Tmapit;
std::vector<string> GetAllHis (Tmaps Hmap );
std::map<std::string, double> ScalePUSample(std::map<std::string, TFile*> fmap, std::string nm);
int MakeComparison(std::map<std::string, Tmaps> Com, std::vector<std::string> order);
int MakeCutFlow(std::map<std::string, Tmaps> Com, std::vector<std::string> order);

int main ( int argc, char *argv[] )
{
  //TopStyle();
  StopStyle();
  TRint *theApp = new TRint("plot interactively", &argc, argv);

  //std::string study = "Cut"; // Study the cut flow on the sample
  //std::string study = "C1PU"; // Study the effect of PU on different cut level
  std::string study = "PU"; // Study the effect of PU on different cut level
  //std::string study = "SIM"; // Study the effect of PU on different cut level
  //----------------------------------------------------------------------------
  //  Define mapping that will make the comparison
  //----------------------------------------------------------------------------
  std::map<std::string, Tmaps> Com;

  Tmaps C1PU0_Sys;
  C1PU0_Sys["noCut"] = "HRoot/C1PU0_NoCut.root";
  C1PU0_Sys["CTVBF"] = "HRoot/C1PU0_CTVBF.root";
  C1PU0_Sys["CTLepV"] = "HRoot/C1PU0_CTLepV.root";
  C1PU0_Sys["CTMet50"] = "HRoot/C1PU0_CTMet50.root";
  C1PU0_Sys["CTMjj"] = "HRoot/C1PU0_CTMjj.root";
  C1PU0_Sys["CTMet200"] = "HRoot/C1PU0_CTMet200.root";
  C1PU0_Sys["AllCut"] = "HRoot/C1PU0_AllCut.root";

  Tmaps C1PU50_Sys;
  C1PU50_Sys["noCut"] = "HRoot/C1PU50_NoCut.root";
  C1PU50_Sys["CTVBF"] = "HRoot/C1PU50_CTVBF.root";
  C1PU50_Sys["CTLepV"] = "HRoot/C1PU50_CTLepV.root";
  C1PU50_Sys["CTMet50"] = "HRoot/C1PU50_CTMet50.root";
  C1PU50_Sys["CTMjj"] = "HRoot/C1PU50_CTMjj.root";
  C1PU50_Sys["CTMet200"] = "HRoot/C1PU50_CTMet200.root";
  C1PU50_Sys["AllCut"] = "HRoot/C1PU50_AllCut.root";

  Tmaps C1PU50x25_Sys;
  C1PU50x25_Sys["noCut"] = "HRoot/C1PU50x25_NoCut.root";
  C1PU50x25_Sys["CTVBF"] = "HRoot/C1PU50x25_CTVBF.root";
  C1PU50x25_Sys["CTLepV"] = "HRoot/C1PU50x25_CTLepV.root";
  C1PU50x25_Sys["CTMet50"] = "HRoot/C1PU50x25_CTMet50.root";
  C1PU50x25_Sys["CTMjj"] = "HRoot/C1PU50x25_CTMjj.root";
  C1PU50x25_Sys["CTMet200"] = "HRoot/C1PU50x25_CTMet200.root";
  C1PU50x25_Sys["AllCut"] = "HRoot/C1PU50x25_AllCut.root";

  Tmaps DYNN_Sys;
  DYNN_Sys["noCut"] = "HRoot/DYNN_NoCut.root";
  DYNN_Sys["CTVBF"] = "HRoot/DYNN_CTVBF.root";
  DYNN_Sys["CTLepV"] = "HRoot/DYNN_CTLepV.root";
  DYNN_Sys["CTMet50"] = "HRoot/DYNN_CTMet50.root";
  DYNN_Sys["CTMjj"] = "HRoot/DYNN_CTMjj.root";
  DYNN_Sys["CTMet200"] = "HRoot/DYNN_CTMet200.root";
  DYNN_Sys["AllCut"] = "HRoot/DYNN_AllCut.root";


  Tmaps TAll_Sys;
  TAll_Sys["noCut"] = "HRoot/TAll_NoCut.root";
  TAll_Sys["CTVBF"] = "HRoot/TAll_CTVBF.root";
  TAll_Sys["CTLepV"] = "HRoot/TAll_CTLepV.root";
  TAll_Sys["CTMet50"] = "HRoot/TAll_CTMet50.root";
  TAll_Sys["CTMjj"] = "HRoot/TAll_CTMjj.root";
  TAll_Sys["CTMet200"] = "HRoot/TAll_CTMet200.root";
  TAll_Sys["AllCut"] = "HRoot/TAll_AllCut.root";

  Tmaps Wino200PU0_Sys;
  Wino200PU0_Sys["noCut"] = "HRoot/Wino200PU0_NoCut.root";
  Wino200PU0_Sys["CTVBF"] = "HRoot/Wino200PU0_CTVBF.root";
  Wino200PU0_Sys["CTLepV"] = "HRoot/Wino200PU0_CTLepV.root";
  Wino200PU0_Sys["CTMet50"] = "HRoot/Wino200PU0_CTMet50.root";
  Wino200PU0_Sys["CTMjj"] = "HRoot/Wino200PU0_CTMjj.root";
  Wino200PU0_Sys["CTMet200"] = "HRoot/Wino200PU0_CTMet200.root";
  Wino200PU0_Sys["AllCut"] = "HRoot/Wino200PU0_AllCut.root";

  Tmaps WJETS_Sys;
  WJETS_Sys["noCut"] = "HRoot/WJETS_NoCut.root";
  WJETS_Sys["CTVBF"] = "HRoot/WJETS_CTVBF.root";
  WJETS_Sys["CTLepV"] = "HRoot/WJETS_CTLepV.root";
  WJETS_Sys["CTMet50"] = "HRoot/WJETS_CTMet50.root";
  WJETS_Sys["CTMjj"] = "HRoot/WJETS_CTMjj.root";
  WJETS_Sys["CTMet200"] = "HRoot/WJETS_CTMet200.root";
  WJETS_Sys["AllCut"] = "HRoot/WJETS_AllCut.root";

  Tmaps Del_W200PU0_Sys;
  Del_W200PU0_Sys["noCut"] = "Delphes/Wino200_14TeV_NoPileUp_NoCut.root";
  Del_W200PU0_Sys["CTVBF"] = "Delphes/Wino200_14TeV_NoPileUp_CTVBF.root";
  Del_W200PU0_Sys["CTLepV"] = "Delphes/Wino200_14TeV_NoPileUp_CTLepV.root";
  Del_W200PU0_Sys["CTMet50"] = "Delphes/Wino200_14TeV_NoPileUp_CTMet50.root";
  Del_W200PU0_Sys["CTMjj"] = "Delphes/Wino200_14TeV_NoPileUp_CTMjj.root";
  Del_W200PU0_Sys["CTMet200"] = "Delphes/Wino200_14TeV_NoPileUp_CTMet200.root";
  Del_W200PU0_Sys["AllCut"] = "Delphes/Wino200_14TeV_NoPileUp_AllCut.root";

  Tmaps Del_W200PU50_Sys;
  Del_W200PU50_Sys["noCut"] = "Delphes/Wino200_14TeV_50PileUp_NoCut.root";
  Del_W200PU50_Sys["CTVBF"] = "Delphes/Wino200_14TeV_50PileUp_CTVBF.root";
  Del_W200PU50_Sys["CTLepV"] = "Delphes/Wino200_14TeV_50PileUp_CTLepV.root";
  Del_W200PU50_Sys["CTMet50"] = "Delphes/Wino200_14TeV_50PileUp_CTMet50.root";
  Del_W200PU50_Sys["CTMjj"] = "Delphes/Wino200_14TeV_50PileUp_CTMjj.root";
  Del_W200PU50_Sys["CTMet200"] = "Delphes/Wino200_14TeV_50PileUp_CTMet200.root";
  Del_W200PU50_Sys["AllCut"] = "Delphes/Wino200_14TeV_50PileUp_AllCut.root";


  Tmaps Del_W200PU140_Sys;
  Del_W200PU140_Sys["noCut"] = "Delphes/Wino200_14TeV_140PileUp_NoCut.root";
  Del_W200PU140_Sys["CTVBF"] = "Delphes/Wino200_14TeV_140PileUp_CTVBF.root";
  Del_W200PU140_Sys["CTLepV"] = "Delphes/Wino200_14TeV_140PileUp_CTLepV.root";
  Del_W200PU140_Sys["CTMet50"] = "Delphes/Wino200_14TeV_140PileUp_CTMet50.root";
  Del_W200PU140_Sys["CTMjj"] = "Delphes/Wino200_14TeV_140PileUp_CTMjj.root";
  Del_W200PU140_Sys["CTMet200"] = "Delphes/Wino200_14TeV_140PileUp_CTMet200.root";
  Del_W200PU140_Sys["AllCut"] = "Delphes/Wino200_14TeV_140PileUp_AllCut.root";

  Tmaps Del_W500PU0_Sys;
  Del_W500PU0_Sys["noCut"] = "Delphes/Wino500_14TeV_NoPileUp_NoCut.root";
  Del_W500PU0_Sys["CTVBF"] = "Delphes/Wino500_14TeV_NoPileUp_CTVBF.root";
  Del_W500PU0_Sys["CTLepV"] = "Delphes/Wino500_14TeV_NoPileUp_CTLepV.root";
  Del_W500PU0_Sys["CTMet50"] = "Delphes/Wino500_14TeV_NoPileUp_CTMet50.root";
  Del_W500PU0_Sys["CTMjj"] = "Delphes/Wino500_14TeV_NoPileUp_CTMjj.root";
  Del_W500PU0_Sys["CTMet200"] = "Delphes/Wino500_14TeV_NoPileUp_CTMet200.root";
  Del_W500PU0_Sys["AllCut"] = "Delphes/Wino500_14TeV_NoPileUp_AllCut.root";

  Tmaps Del_W500PU50_Sys;
  Del_W500PU50_Sys["noCut"] = "Delphes/Wino500_14TeV_50PileUp_NoCut.root";
  Del_W500PU50_Sys["CTVBF"] = "Delphes/Wino500_14TeV_50PileUp_CTVBF.root";
  Del_W500PU50_Sys["CTLepV"] = "Delphes/Wino500_14TeV_50PileUp_CTLepV.root";
  Del_W500PU50_Sys["CTMet50"] = "Delphes/Wino500_14TeV_50PileUp_CTMet50.root";
  Del_W500PU50_Sys["CTMjj"] = "Delphes/Wino500_14TeV_50PileUp_CTMjj.root";
  Del_W500PU50_Sys["CTMet200"] = "Delphes/Wino500_14TeV_50PileUp_CTMet200.root";
  Del_W500PU50_Sys["AllCut"] = "Delphes/Wino500_14TeV_50PileUp_AllCut.root";


  Tmaps Del_W500PU140_Sys;
  Del_W500PU140_Sys["noCut"] = "Delphes/Wino500_14TeV_140PileUp_NoCut.root";
  Del_W500PU140_Sys["CTVBF"] = "Delphes/Wino500_14TeV_140PileUp_CTVBF.root";
  Del_W500PU140_Sys["CTLepV"] = "Delphes/Wino500_14TeV_140PileUp_CTLepV.root";
  Del_W500PU140_Sys["CTMet50"] = "Delphes/Wino500_14TeV_140PileUp_CTMet50.root";
  Del_W500PU140_Sys["CTMjj"] = "Delphes/Wino500_14TeV_140PileUp_CTMjj.root";
  Del_W500PU140_Sys["CTMet200"] = "Delphes/Wino500_14TeV_140PileUp_CTMet200.root";
  Del_W500PU140_Sys["AllCut"] = "Delphes/Wino500_14TeV_140PileUp_AllCut.root";



  Tmaps Del_TTBARPU0_Sys;
  Del_TTBARPU0_Sys["noCut"] = "Delphes/TTBAR_13TEV_NoPileUp_NoCut.root";
  Del_TTBARPU0_Sys["CTVBF"] = "Delphes/TTBAR_13TEV_NoPileUp_CTVBF.root";
  Del_TTBARPU0_Sys["CTLepV"] = "Delphes/TTBAR_13TEV_NoPileUp_CTLepV.root";
  Del_TTBARPU0_Sys["CTMet50"] = "Delphes/TTBAR_13TEV_NoPileUp_CTMet50.root";
  Del_TTBARPU0_Sys["CTMjj"] = "Delphes/TTBAR_13TEV_NoPileUp_CTMjj.root";
  Del_TTBARPU0_Sys["CTMet200"] = "Delphes/TTBAR_13TEV_NoPileUp_CTMet200.root";
  Del_TTBARPU0_Sys["AllCut"] = "Delphes/TTBAR_13TEV_NoPileUp_AllCut.root";

  Tmaps Del_TTBARPU50_Sys;
  Del_TTBARPU50_Sys["noCut"] = "Delphes/TTBAR_13TEV_50PileUp_NoCut.root";
  Del_TTBARPU50_Sys["CTVBF"] = "Delphes/TTBAR_13TEV_50PileUp_CTVBF.root";
  Del_TTBARPU50_Sys["CTLepV"] = "Delphes/TTBAR_13TEV_50PileUp_CTLepV.root";
  Del_TTBARPU50_Sys["CTMet50"] = "Delphes/TTBAR_13TEV_50PileUp_CTMet50.root";
  Del_TTBARPU50_Sys["CTMjj"] = "Delphes/TTBAR_13TEV_50PileUp_CTMjj.root";
  Del_TTBARPU50_Sys["CTMet200"] = "Delphes/TTBAR_13TEV_50PileUp_CTMet200.root";
  Del_TTBARPU50_Sys["AllCut"] = "Delphes/TTBAR_13TEV_50PileUp_AllCut.root";


  Tmaps Del_TTBARPU140_Sys;
  Del_TTBARPU140_Sys["noCut"] = "Delphes/TTBAR_13TEV_140PileUp_NoCut.root";
  Del_TTBARPU140_Sys["CTVBF"] = "Delphes/TTBAR_13TEV_140PileUp_CTVBF.root";
  Del_TTBARPU140_Sys["CTLepV"] = "Delphes/TTBAR_13TEV_140PileUp_CTLepV.root";
  Del_TTBARPU140_Sys["CTMet50"] = "Delphes/TTBAR_13TEV_140PileUp_CTMet50.root";
  Del_TTBARPU140_Sys["CTMjj"] = "Delphes/TTBAR_13TEV_140PileUp_CTMjj.root";
  Del_TTBARPU140_Sys["CTMet200"] = "Delphes/TTBAR_13TEV_140PileUp_CTMet200.root";
  Del_TTBARPU140_Sys["AllCut"] = "Delphes/TTBAR_13TEV_140PileUp_AllCut.root";

  Tmaps Del_WJETSPU0_Sys;
  Del_WJETSPU0_Sys["noCut"] = "Delphes/WJETS_13TEV_NoPileUp_NoCut.root";
  Del_WJETSPU0_Sys["CTVBF"] = "Delphes/WJETS_13TEV_NoPileUp_CTVBF.root";
  Del_WJETSPU0_Sys["CTLepV"] = "Delphes/WJETS_13TEV_NoPileUp_CTLepV.root";
  Del_WJETSPU0_Sys["CTMet50"] = "Delphes/WJETS_13TEV_NoPileUp_CTMet50.root";
  Del_WJETSPU0_Sys["CTMjj"] = "Delphes/WJETS_13TEV_NoPileUp_CTMjj.root";
  Del_WJETSPU0_Sys["CTMet200"] = "Delphes/WJETS_13TEV_NoPileUp_CTMet200.root";
  Del_WJETSPU0_Sys["AllCut"] = "Delphes/WJETS_13TEV_NoPileUp_AllCut.root";

  Tmaps Del_WJETSPU50_Sys;
  Del_WJETSPU50_Sys["noCut"] = "Delphes/WJETS_13TEV_50PileUp_NoCut.root";
  Del_WJETSPU50_Sys["CTVBF"] = "Delphes/WJETS_13TEV_50PileUp_CTVBF.root";
  Del_WJETSPU50_Sys["CTLepV"] = "Delphes/WJETS_13TEV_50PileUp_CTLepV.root";
  Del_WJETSPU50_Sys["CTMet50"] = "Delphes/WJETS_13TEV_50PileUp_CTMet50.root";
  Del_WJETSPU50_Sys["CTMjj"] = "Delphes/WJETS_13TEV_50PileUp_CTMjj.root";
  Del_WJETSPU50_Sys["CTMet200"] = "Delphes/WJETS_13TEV_50PileUp_CTMet200.root";
  Del_WJETSPU50_Sys["AllCut"] = "Delphes/WJETS_13TEV_50PileUp_AllCut.root";


  Tmaps Del_WJETSPU140_Sys;
  Del_WJETSPU140_Sys["noCut"] = "Delphes/WJETS_13TEV_140PileUp_NoCut.root";
  Del_WJETSPU140_Sys["CTVBF"] = "Delphes/WJETS_13TEV_140PileUp_CTVBF.root";
  Del_WJETSPU140_Sys["CTLepV"] = "Delphes/WJETS_13TEV_140PileUp_CTLepV.root";
  Del_WJETSPU140_Sys["CTMet50"] = "Delphes/WJETS_13TEV_140PileUp_CTMet50.root";
  Del_WJETSPU140_Sys["CTMjj"] = "Delphes/WJETS_13TEV_140PileUp_CTMjj.root";
  Del_WJETSPU140_Sys["CTMet200"] = "Delphes/WJETS_13TEV_140PileUp_CTMet200.root";
  Del_WJETSPU140_Sys["AllCut"] = "Delphes/WJETS_13TEV_140PileUp_AllCut.root";
























  // C1PU Study
  Tmaps C1PU_noCut;
  C1PU_noCut["PU0"] = "HRoot/C1PU0_NoCut.root";
  C1PU_noCut["PU50"] = "HRoot/C1PU50_NoCut.root";
  C1PU_noCut["PU50x25"] = "HRoot/C1PU50x25_NoCut.root";

  Tmaps C1PU_CTVBF;
  C1PU_CTVBF["PU0"] = "HRoot/C1PU0_CTVBF.root";
  C1PU_CTVBF["PU50"] = "HRoot/C1PU50_CTVBF.root";
  C1PU_CTVBF["PU50x25"] = "HRoot/C1PU50x25_CTVBF.root";
  
  Tmaps C1PU_CTLepV;
  C1PU_CTLepV["PU0"] = "HRoot/C1PU0_CTLepV.root";
  C1PU_CTLepV["PU50"] = "HRoot/C1PU50_CTLepV.root";
  C1PU_CTLepV["PU50x25"] = "HRoot/C1PU50x25_CTLepV.root";
  
  Tmaps C1PU_CTMet50;
  C1PU_CTMet50["PU0"] = "HRoot/C1PU0_CTMet50.root";
  C1PU_CTMet50["PU50"] = "HRoot/C1PU50_CTMet50.root";
  C1PU_CTMet50["PU50x25"] = "HRoot/C1PU50x25_CTMet50.root";

  Tmaps C1PU_CTMjj;
  C1PU_CTMjj["PU0"] = "HRoot/C1PU0_CTMjj.root";
  C1PU_CTMjj["PU50"] = "HRoot/C1PU50_CTMjj.root";
  C1PU_CTMjj["PU50x25"] = "HRoot/C1PU50x25_CTMjj.root";

  Tmaps C1PU_CTMet200;
  C1PU_CTMet200["PU0"] = "HRoot/C1PU0_CTMet200.root";
  C1PU_CTMet200["PU50"] = "HRoot/C1PU50_CTMet200.root";
  C1PU_CTMet200["PU50x25"] = "HRoot/C1PU50x25_CTMet200.root";

  Tmaps C1PU_AllCut;
  C1PU_AllCut["PU0"] = "HRoot/C1PU0_AllCut.root";
  C1PU_AllCut["PU50"] = "HRoot/C1PU50_AllCut.root";
  C1PU_AllCut["PU50x25"] = "HRoot/C1PU50x25_AllCut.root";
  
  Tmaps TTJJ_noCut;
  TTJJ_noCut["PU0"] = "Delphes/TTBAR_13TEV_NoPileUp_NoCut.root";
  TTJJ_noCut["PU50"] = "Delphes/TTBAR_13TEV_50PileUp_NoCut.root";
  TTJJ_noCut["PU140"] = "Delphes/TTBAR_13TEV_140PileUp_NoCut.root";

  Tmaps WJJ_noCut;
  WJJ_noCut["PU0"] = "Delphes/WJETS_13TEV_NoPileUp_NoCut.root";
  WJJ_noCut["PU50"] = "Delphes/WJETS_13TEV_50PileUp_NoCut.root";
  WJJ_noCut["PU140"] = "Delphes/WJETS_13TEV_140PileUp_NoCut.root";

  Tmaps TTJJ_AllCut;
  TTJJ_AllCut["PU0"] = "Delphes/TTBAR_13TEV_NoPileUp_AllCut.root";
  TTJJ_AllCut["PU50"] = "Delphes/TTBAR_13TEV_50PileUp_AllCut.root";
  TTJJ_AllCut["PU140"] = "Delphes/TTBAR_13TEV_140PileUp_AllCut.root";

  Tmaps WJJ_AllCut;
  WJJ_AllCut["PU0"] = "Delphes/WJETS_13TEV_NoPileUp_AllCut.root";
  WJJ_AllCut["PU50"] = "Delphes/WJETS_13TEV_50PileUp_AllCut.root";
  WJJ_AllCut["PU140"] = "Delphes/WJETS_13TEV_140PileUp_AllCut.root";

//----------------------------------------------------------------------------
//  
//----------------------------------------------------------------------------
  Tmaps TTBAR_CMET;
  TTBAR_CMET["PU0"] = "Delphes_0524/TTBAR_13TEV_NoPileUp_LepV.root";
  TTBAR_CMET["PU50"] = "Delphes_0524/TTBAR_13TEV_50PileUp_LepV.root";
  TTBAR_CMET["PU140"] = "Delphes_0524/TTBAR_13TEV_140PileUp_LepV.root";

  Tmaps ZJETS_CMET;
  ZJETS_CMET["PU0"] = "Delphes_0524/ZJETS_13TEV_NoPileUp_NoCut.root";
  ZJETS_CMET["PU50"] = "Delphes_0524/ZJETS_13TEV_50PileUp_NoCut.root";
  ZJETS_CMET["PU140"] = "Delphes_0524/ZJETS_13TEV_140PileUp_NoCut.root";

  Tmaps Wino200PU0_NoCut;
  Wino200PU0_NoCut["CMSSW"] = "CMSSW/Wino200PU0_NoCut.root";
  Wino200PU0_NoCut["Delphes"] = "Delphes/Wino200_14TeV_NoPileUp_NoCut.root";

  Tmaps Wino200PU0_AllCut;
  Wino200PU0_AllCut["CMSSW"] = "CMSSW/Wino200PU0_AllCut.root";
  Wino200PU0_AllCut["Delphes"] = "Delphes/Wino200_14TeV_NoPileUp_AllCut.root";
  // Cut Flow comparison
  if (study == "Cut")
  {
    // CMSSW
    //Com["DYNN_CutFlow : "] = DYNN_Sys;
    //Com["TAll_CutFlow : "] = TAll_Sys;
    //Com["C1PU0_CutFlow : "] = C1PU0_Sys;
    //Com["Wino200PU0_CutFlow : "] = Wino200PU0_Sys;
    //Com["WJETS_CutFlow : "] = WJETS_Sys;
    //Com["C1PU50_CutFlow : "] = C1PU50_Sys;
    //Com["C1PU50x25_CutFlow : "] = C1PU50x25_Sys;

    // Delphes
    Com["Del_WJETSPU140_CutFlow :"]  = Del_WJETSPU140_Sys;
    Com["Del_WJETSPU50_CutFlow :"]   = Del_WJETSPU50_Sys;
    Com["Del_WJETSPU0_CutFlow :"]    = Del_WJETSPU0_Sys;
    Com["Del_TTBARPU140_CutFlow :"]  = Del_TTBARPU140_Sys;
    Com["Del_TTBARPU50_CutFlow :"]   = Del_TTBARPU50_Sys;
    Com["Del_TTBARPU0_CutFlow :"]    = Del_TTBARPU0_Sys;
    Com["Del_W500PU140_CutFlow :"]   = Del_W500PU140_Sys;
    Com["Del_W500PU50_CutFlow :"]    = Del_W500PU50_Sys;
    Com["Del_W500PU0_CutFlow :"]     = Del_W500PU0_Sys;
    Com["Del_W200PU140_CutFlow :"]   = Del_W200PU140_Sys;
    Com["Del_W200PU50_CutFlow :"]    = Del_W200PU50_Sys;
    Com["Del_W200PU0_CutFlow :"]     = Del_W200PU0_Sys;


  }

  // C1PU Comparison
  if (study == "C1PU")
  {
    Com["C1PU_noCut : "] = C1PU_noCut;
    Com["C1PU_CTVBF : "] = C1PU_CTVBF;
    Com["C1PU_CTLepV : "] = C1PU_CTLepV;
    Com["C1PU_CTMet50 : "] = C1PU_CTMet50;
    Com["C1PU_CTMjj : "] = C1PU_CTMjj;
    Com["C1PU_CTMet200 : "] = C1PU_CTMet200;
    Com["C1PU_AllCut : "] = C1PU_AllCut;
  }

  // Delphes PU Comparison
  if (study == "PU")
  {
    //Com["TTBAR_noCut : "] = TTJJ_noCut;
    //Com["WJJ_noCut : "] = WJJ_noCut;
    //Com["TTBAR_AllCut : "] = TTJJ_AllCut;
    //Com["WJJ_AllCut : "] = WJJ_AllCut;
    Com["TTBAR_CMET : "] = TTBAR_CMET;
    Com["ZJETS_CMET : "] = ZJETS_CMET;
    //Com["TTBAR_LepCut : "] = TTBAR_LepCut;
  }

  // Compare CMSSW with Delphes
  if (study == "SIM")
  {
    Com["Wino200PU0_NoCut : "] = Wino200PU0_NoCut;
    Com["Wino200PU0_AllCut : "] = Wino200PU0_AllCut;
  }
  //I need to define the plotting order! Otherwise map will order
  //the key which make it harder to understand. Maybe a multimap?
  std::vector<std::string> order;
  // Cut Flow ordering 
  // Cut Flow ordering 
  if (study == "Cut")
  {
    order.push_back("noCut");
    order.push_back("CTVBF");
    order.push_back("CTLepV");
    order.push_back("CTMet50");
    order.push_back("CTMjj");
    order.push_back("CTMet200");
    order.push_back("AllCut");

  }

  // C1PU ordering
  if (study == "C1PU")
  {
    order.push_back("PU0");
    order.push_back("PU50");
    order.push_back("PU50x25");
  }

  // PU ordering
  if (study == "PU")
  {
    order.push_back("PU0");
    order.push_back("PU50");
    order.push_back("PU140");
  }

  //Simulator ordering
  if (study == "SIM")
  {
    order.push_back("CMSSW");
    order.push_back("Delphes");
  }

  if (study == "Cut")
    MakeCutFlow(Com, order);

    MakeComparison(Com, order);


  theApp->Run();
  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

// ===  FUNCTION  ============================================================
//         Name:  MakeCutFlow
//  Description:  The main functions that makes comparison
// ===========================================================================
int MakeCutFlow(std::map<std::string, Tmaps> Com, std::vector<std::string> order)
{
  //The canvas to draw plots
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);
  TFile out("CutFlow.root", "RECREATE");

  for(std::map<std::string, Tmaps>::iterator cit=Com.begin();
      cit!=Com.end(); cit++)
  {
    c1->cd();
    c1->Clear();

    //----------------------------------------------------------------------------
    //  First open all the files for comparison
    //----------------------------------------------------------------------------
    std::map<std::string, TFile*> Fmap;
    for(Tmapit it=cit->second.begin();
        it!=cit->second.end(); it++)
    {
      std::cout << "Getting file : " << it->second << std::endl;
      Fmap[it->first]=new TFile(it->second.c_str(), "R");
    }

    // Book the TH1 for the cut flow
    std::string Comname = cit->first;
    if (cit->first.find(" : ") != std::string::npos)
      Comname.replace(cit->first.find(" : "), 3, "");
    TH1F *CutFlow = new TH1F(Comname.c_str(), Comname.c_str(), 6, 0, 6);
    TAxis *xaxis = CutFlow->GetXaxis();
    int bin = 1;

      // Instead of looping the Tmap, I loop over a vector for the ordering
      for(std::vector<std::string>::iterator vit=order.begin();
          vit!=order.end(); vit++)
      {
        if (*vit == "AllCut") continue;
        Tmapit it=cit->second.find(*vit);
        TH1F* h1 = (TH1F*)Fmap[it->first]->Get("NEVT");
        double total = h1->GetBinContent(h1->FindBin(1));
        TH1F* h2 = (TH1F*)Fmap[it->first]->Get("NEVTS");
        double selected = h2->GetBinContent(h2->FindBin(1));
        std::cout << " Sample : " <<  cit->first << " Cut :  " << *vit <<" Event selected :"  
          << selected <<  " fraction :"   << selected/total << std::endl;
        CutFlow->SetBinContent(bin, selected/total);
        std::string tempname = *vit;
        if (vit->find("CT") != std::string::npos)
          tempname.replace(vit->find("CT"), 2, "");
        xaxis->SetBinLabel(bin, tempname.c_str());
        bin++;
      }
      out.cd();

      CutFlow->Draw();
      CutFlow->Write();
      TString plotname = Comname + ".png";
      c1->Print(plotname);
      delete CutFlow;
      xaxis = 0;

  }
  delete c1;

  return 1;
}

// ===  FUNCTION  ============================================================
//         Name:  MakeComparison
//  Description:  The main functions that makes comparison
// ===========================================================================
int MakeComparison(std::map<std::string, Tmaps> Com, std::vector<std::string> order)
{
  //Start with all the comparison
  for(std::map<std::string, Tmaps>::iterator cit=Com.begin();
      cit!=Com.end(); cit++)
  {
    //The canvas to draw plots
    TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);

    //----------------------------------------------------------------------------
    //  First open all the files for comparison
    //----------------------------------------------------------------------------
    std::map<std::string, TFile*> Fmap;
    for(Tmapit it=cit->second.begin();
        it!=cit->second.end(); it++)
    {
      std::cout << "Getting file : " << it->second << std::endl;
      Fmap[it->first]=new TFile(it->second.c_str(), "R");
    }

    //----------------------------------------------------------------------------
    //  Get the list of containing Hist
    //----------------------------------------------------------------------------
    std::vector<std::string> passive_var = GetAllHis(cit->second);

//----------------------------------------------------------------------------
//  Get the scale factor for this comparison
//----------------------------------------------------------------------------
    std::map<std::string, double> SFMap = ScalePUSample(Fmap, order.at(0));

    //----------------------------------------------------------------------------
    //  Looping over each hist
    //----------------------------------------------------------------------------
    for (std::vector<std::string>::iterator hit = passive_var.begin();
        hit != passive_var.end(); hit++ )
    {
      //Preparing for making this plot
      std::cout << "Ploting " << *hit << std::endl;
      c1->cd();
      c1->Clear();

      int lcolor=1;
      double maxy = 0;
      double miny = 0;
      TAxis* yaix = 0;
      // Add Legend?? Need a better handling of this
      TLegend f(0.6862416,0.5995763,0.9228188,0.8940678,NULL,"brNDC");
      f.SetBorderSize(0);
      f.SetFillStyle(0); //transparent hollow?
      f.SetTextFont(62); 
      f.SetTextSize(0.045);
      //
      // Instead of looping the Tmap, I loop over a vector for the ordering
      for(std::vector<std::string>::iterator vit=order.begin();
          vit!=order.end(); vit++)
      {
        Tmapit it=cit->second.find(*vit);
        TH1F* h = (TH1F*)Fmap[it->first]->Get(hit->c_str());
        //TH1F* h = (TH1F*)Fmap.begin()->second->Get("HT");
        //std::cout << "NA : " << h->GetTitle()<< std::endl;
        std::cout << "Sample :"   << it->first << "INtegral : " << h->Integral() << "  Scale factor: " << SFMap[it->first] << std::endl;
        h->Scale(SFMap[it->first]);
        h->SetLineWidth(3);
        // Set Line color
        if (*vit == "CTVBF") 
        {
          h->SetLineColor(9);
          lcolor++;
        }
        else if (*vit == "AllCut")
        {
          h->SetLineColor(2);
          lcolor++;
        }
        else h->SetLineColor(lcolor++);
        maxy = maxy > h->GetMaximum() ? maxy : h->GetMaximum();
        miny = miny < h->GetMinimum() ? miny : h->GetMinimum();
        if (vit == order.begin())
        {
          h->Draw();
          yaix = h->GetYaxis();
          TString nname = cit->first + h->GetTitle();
          std::cout << " name  : " << nname << std::endl;
          h->SetTitle(nname);
        }
        else
          h->Draw("same");
        f.AddEntry(h, it->first.c_str(), "l");
      }

      if (cit->first == "TAll_CutFlow : "
        || cit->first == "DYNN_CutFlow : ")
        c1->SetLogy();
      else
        yaix->SetRangeUser(miny, 1.2*maxy);
      f.Draw();

      //Print out the plots
      std::string tempname = cit->first;
      if (cit->first.find(" : ") != std::string::npos)
        tempname.replace(cit->first.find(" : "), 3, "_");
      TString outname =tempname+ *hit + ".png";
      c1->Print(outname);
      //outname = cit->first+ *hit + ".root";
      //c1->Print(outname);

      //break;
    }
    delete c1;
  }
  return 1;
}       // -----  end of function MakeComparison  -----

std::vector<string> GetAllHis (Tmaps Hmap )
{

  //Similar as LoadFile(), but just looking at the first file so far
  //So it is the user's job to make sure all the M2Info shared the same
  //histogram configuration. If the M2 couldn't find the corresponding
  //histogram, it would crash
  //For future improvement, would be nice to loop over the whole file list
  //to obtain the possible out put histograms
  //Loading of the root file list containig the Method2Info objects
  //Now loop over the samples in the list and read in the information
  for(Tmaps::iterator it=Hmap.begin();
      it!=Hmap.end(); it++)
  {

    TFile m2File(it->second.c_str(),"R");
    if(!m2File.IsOpen()){
      cout<<"Unable to locate first Input M2Info File: "<<it->second
        <<", so I cannot proceed."<<endl;
      exit(-1);
    }
  }
  TFile m2File(Hmap.begin()->second.c_str(),"R");
  TIter next (m2File.GetListOfKeys());
  std::map<std::string, int> variable;
  std::vector<std::string> varVec;
  TKey *key;
  //    std::string name;
  while ((key= (TKey*) next()))
  {
    std::string stdname=key->GetName();
    variable.insert(std::pair<std::string, int>(stdname, 1));
  }
  for (std::map<std::string, int>::iterator it = variable.begin(); 
      it != variable.end(); ++it)
  {
    varVec.push_back(it->first);
  }
  return varVec;
}		// -----  end of method MethodIICalc::GetAllHisFromM2Info  -----

// ===  FUNCTION  ============================================================
//         Name:  ScalePUSample
//  Description:  A function that can scale the samples in the comparison to
//  the same number of events.
// ===========================================================================
std::map<std::string, double> ScalePUSample(std::map<std::string, TFile*> fmap, std::string nm)
{
  std::map<std::string, TH1F*> THmap;
  std::map<std::string, double> SFmap;
  for(std::map<std::string, TFile*>::iterator fit=fmap.begin();
    fit!=fmap.end(); fit++)
  {
    THmap[fit->first] = (TH1F*)fit->second->Get("NEVT");
  }

  double normal = THmap[nm]->GetBinContent(THmap[nm]->FindBin(1));

  for(std::map<std::string, TFile*>::iterator fit=fmap.begin();
    fit!=fmap.end(); fit++)
  {
    SFmap[fit->first] = normal/THmap[fit->first]->GetBinContent(THmap[fit->first]->FindBin(1));
    //Need to check whether success
    //if (THmap[fit->first]->IsA()) {
    //}
  }
  return SFmap;

}       // -----  end of function ScalePUSample  -----
