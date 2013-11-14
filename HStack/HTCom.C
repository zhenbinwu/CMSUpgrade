// ===========================================================================
// 
//       Filename:  Com.C
// 
//    Description:  a quick comparison 
// 
//        Version:  1.0
//        Created:  08/26/2013 01:33:59 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include <cstdlib>
#include "HTSample.h"
#include "TCanvas.h"
#include "StopStyle.hh"
#include "TLegend.h"
#include "TH1F.h"
#include "TLatex.h"
#include <map>

int DrawTitle();

int main ( int argc, char *argv[] )
{
  double InitLumifb = 3000;
  std::map<std::string, HTSample*> MCSample;
  std::vector<std::string> ProList; 
  StopStyle();
  const std::string& PU = "NoPileUp";
  //const std::string& PU = "140PileUp";
  //const std::string& Label = "Wino100";
  //const std::string& Label = "Wino200";
  //const std::string& Label = "Wlv";
  const std::string& Label = "Compare";
  //const std::string& Label = "CompareZvv";
  //const std::string& detector = "New";
  //const std::string& detector = "Compare";
  //const std::string& detector = "Old";
  //const std::string& detector = "ECFA";
  const std::string& detector = "PhaseI";
  //const std::string& detector = "PhaseII4";
  //const std::string& detector = "PhaseII3";
  //const std::string& detector = "PhaseII4v2";
  //const std::string& dir = "./TOOL/";
  ////const std::string& dir = "./ECFA_AN/";
  //const std::string& dir = "./ECFA_50/";
  //const std::string& dir = "./ECFA_Version/";
  //const std::string& dir = "./ECFA_200/";
  const std::string& dir = "./PhaseI_MT2/";
  //const std::string& dir = "./PhaseI_SUSY/";
  //const std::string& dir = "./JetEta/";

  if (Label == "Wino200")
  {
    //MCSample["Snowmass_0PU"]    = new HTSample("Snowmass/", "Wino200_14TeV", "NoPileUp", "Snowmass");
    //MCSample["Snowmass_140PU"]    = new HTSample("Snowmass/", "Wino200_14TeV", PU, "Snowmass");
    MCSample["PhaseI_0PU"]      = new HTSample(dir, "Wino200_14TeV", "NoPileUp", "PhaseI");
    MCSample["PhaseI_140PU"]      = new HTSample(dir, "Wino200_14TeV", PU, "PhaseI");
    MCSample["PhaseII3_140PU"]    = new HTSample(dir, "Wino200_14TeV", PU, "PhaseII3");
    MCSample["PhaseII4_140PU"]    = new HTSample(dir, "Wino200_14TeV", PU, "PhaseII4");
    MCSample["PhaseII4v2_140PU"]    = new HTSample(dir, "Wino200_14TeV", PU, "PhaseII4");
  }

  if (Label == "Wino100")
  {
    //MCSample["Snowmass_0PU"]    = new HTSample("Snowmass/", "Wino100_14TeV", "NoPileUp", "Snowmass");
    //MCSample["Snowmass_140PU"]    = new HTSample("Snowmass/", "Wino100_14TeV", PU, "Snowmass");
    MCSample["PhaseI_0PU"]      = new HTSample(dir, "Wino100_14TeV", "NoPileUp", "PhaseI");
    MCSample["PhaseI_140PU"]      = new HTSample(dir, "Wino100_14TeV", PU, "PhaseI");
    MCSample["PhaseII3_140PU"]    = new HTSample(dir, "Wino100_14TeV", PU, "PhaseII3");
    MCSample["PhaseII4_140PU"]    = new HTSample(dir, "Wino100_14TeV", PU, "PhaseII4");
    MCSample["PhaseII4v2_140PU"]    = new HTSample(dir, "Wino100_14TeV", PU, "PhaseII4");
  }


  if (Label == "Wlv")
  {
    //MCSample["Snowmass_0PU"]    = new HTSample("Snowmass/", "WlvJ*_14TEV_HT", "NoPileUp", "Snowmass");
    //MCSample["Snowmass_140PU"]    = new HTSample("Snowmass/", "WlvJ*_14TEV_HT", PU, "SnowmassOpt");
    //MCSample["SnowmassOpt"] = new HTSample("Snowmass/", "Wlv_14TEV_HT", PU, "SnowmassOpt");
    if (detector == "Old")
    {
      MCSample["PhaseI_0PU"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "NoPileUp", "PhaseI");
      MCSample["PhaseI_140PU"]      = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseI");
      MCSample["PhaseII3_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII3");
      MCSample["PhaseII4_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII4");
    }

    if (detector == "New")
    {
      MCSample["PhaseI_0PU"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "NoPileUp", "PhaseIv2");
      MCSample["PhaseI_140PU"]      = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseIv2");
      MCSample["PhaseII3_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII3v2");
      MCSample["PhaseII4_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII4v2");
    }
    if (detector == "Compare")
    {
      //MCSample["PhaseI_0PU_v1"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "NoPileUp", "PhaseI");
      //MCSample["PhaseI_0PU_v2"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "NoPileUp", "PhaseIv2");
      //MCSample["PhaseII3_140PU_v1"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "140PileUp", "PhaseII3");
      //MCSample["PhaseII3_140PU_v2"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "140PileUp", "PhaseII3v2");
      //MCSample["PhaseI_140PU_v1"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "140PileUp", "PhaseI");
      //MCSample["PhaseI_140PU_v2"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "140PileUp", "PhaseIv2");
      MCSample["PhaseI_0PU"]      = new HTSample(dir, "WlvJ*_14TEV_HT", "NoPileUp", "PhaseI");
      MCSample["PhaseI_140PU"]      = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseI");
      MCSample["PhaseII3_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII3");
      MCSample["PhaseII4_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII4v2");
    }
    //MCSample["PhaseII4_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII4v2");
    //MCSample["PhaseII4v2_140PU"]    = new HTSample(dir, "WlvJ*_14TEV_HT", PU, "PhaseII4v2");
    //MCSample["PhaseI_0PU"]      = new HTSample("BWTool", "Wlv*_14TEV_HT", "NoPileUp", "PhaseI");
    //MCSample["PhaseI_140PU"]      = new HTSample("BWTool", "Wlv*_14TEV_HT", PU, "PhaseI");
    //MCSample["PhaseII3_140PU"]    = new HTSample("BWTool", "Wlv*_14TEV_HT", PU, "PhaseII3");
    //MCSample["PhaseII4_140PU"]    = new HTSample("BWTool", "Wlv*_14TEV_HT", PU, "PhaseII4");
  }

  if (Label.find("Compare") != std::string::npos)
  {
    MCSample["Wino100"]      = new HTSample(dir, "Wino100_14TeV", PU, detector);
    MCSample["Wino200"]      = new HTSample(dir, "Wino200_14TeV", PU, detector);
    MCSample["Wino500"]      = new HTSample(dir, "Wino500_14TeV", PU, detector);
    MCSample["Wlv"]      = new HTSample(dir, "Wlv*_14TEV_HT", PU, detector);
    MCSample["SysWmv"]      = new HTSample(dir, "SysWmv*_14TEV_HT", PU, detector);
    MCSample["SysWev"]      = new HTSample(dir, "SysWev*_14TEV_HT", PU, detector);
    //MCSample["Wtv"]      = new HTSample(dir, "Wtv*_14TEV_HT", PU, detector);
    //MCSample["Wev"]      = new HTSample(dir, "Wev*_14TEV_HT", PU, detector);
    //MCSample["Wmv"]      = new HTSample(dir, "Wmv*_14TEV_HT", PU, detector);
    MCSample["Zvv"]       = new HTSample(dir, "Zvv*_14TEV_HT",      PU, detector);
    MCSample["SysZmm"]       = new HTSample(dir, "SysZmm*_14TEV_HT",      PU, detector);
    MCSample["SysZee"]       = new HTSample(dir, "SysZee*_14TEV_HT",      PU, detector);
    //MCSample["TTSL"]      = new HTSample(dir, "TTSLep*_14TEV_HT", PU, detector);
    //MCSample["TTFL"]      = new HTSample(dir, "TTFLep*_14TEV_HT", PU, detector);
    MCSample["TT"]      = new HTSample(dir, "TT_14TEV_HT", PU, detector);
    //MCSample["Wino100"]      = new HTSample("ECFA", "Wino100_14TeV", PU, "PhaseII4");
    //MCSample["Wino200"]      = new HTSample("ECFA", "Wino200_14TeV", PU, "PhaseII4");
    //MCSample["Wino500"]      = new HTSample("ECFA", "Wino500_14TeV", PU, "PhaseII4");
    //MCSample["Wlv"]      = new HTSample("ECFA", "Wlv*_14TEV_HT", PU, "PhaseII4");
    //MCSample["Zvv"]      = new HTSample("ECFA", "Zvv*_14TEV_HT", PU, "PhaseII4");
    //MCSample["TT"]      = new HTSample("ECFA", "TT*_14TEV_HT", PU, "PhaseII4");
    MCSample["Wino100"] -> SetOptions(true,  1);
    MCSample["Wino200"] -> SetOptions(true,  2);
    MCSample["Wino500"] -> SetOptions(true,  kOrange+9);
    MCSample["Wlv"]   -> SetOptions(true,  4);
    MCSample["SysWmv"]   -> SetOptions(true,  1);
    MCSample["SysWev"]   -> SetOptions(true,  2);
    //MCSample["Wtv"]   -> SetOptions(true,  41);
    //MCSample["Wev"]   -> SetOptions(true,  42);
    //MCSample["Wmv"]   -> SetOptions(true,  44);
    MCSample["Zvv"]  -> SetOptions(true,  6);
    MCSample["SysZmm"]  -> SetOptions(true,  1);
    MCSample["SysZee"]  -> SetOptions(true,  2);
    //MCSample["TTSL"]  -> SetOptions(true,  kGreen+2);
    MCSample["TT"]  -> SetOptions(true,  kGreen+2);
    //MCSample["TTFL"]  -> SetOptions(true,  kOrange+9);
    if (Label == "CompareWlv")
    {
      ProList.push_back("Wlv");
      ProList.push_back("SysWmv");
      ProList.push_back("SysWev");
    }
    ////ProList.push_back("Wtv");
    ////ProList.push_back("Wev");
    ////ProList.push_back("Wmv");
    if (Label == "CompareZvv")
    {

      ProList.push_back("Zvv");
      ProList.push_back("SysZmm");
      ProList.push_back("SysZee");
    }
    ////ProList.push_back("TTFL");
    ProList.push_back("TT");
    ProList.push_back("Wlv");
    ProList.push_back("Zvv");
    //ProList.push_back("Backgrounds");
    //ProList.push_back("TTFL");
    //ProList.push_back("TTSL");
    ProList.push_back("Wino100");
    ProList.push_back("Wino200");
    ProList.push_back("Wino500");
  }else{

    //MCSample["Wino100"]      = new HTSample("ECFA", "Wino100_14TeV", PU, "PhaseII4");
    //MCSample["Wino200"]      = new HTSample("ECFA", "Wino200_14TeV", PU, "PhaseII4");
    //MCSample["Wlv"]      = new HTSample("ECFA", "Wlv*_14TEV_HT", PU, "PhaseII4");
    //MCSample["Zvv"]      = new HTSample("ECFA", "Zvv*_14TEV_HT", PU, "PhaseII4");
    //MCSample["TT"]      = new HTSample("ECFA", "TT*_14TEV_HT", PU, "PhaseII4");

    //MCSample["PhaseI_0PU_v1"] -> SetOptions(true,  1);
    //MCSample["PhaseI_0PU_v2"] -> SetOptions(true,  2);
    //MCSample["PhaseII3_140PU_v1"] -> SetOptions(true,  4);
    //MCSample["PhaseII3_140PU_v2"] -> SetOptions(true,  6);
    //MCSample["PhaseI_140PU_v1"] -> SetOptions(true,  kGreen+2);
    //MCSample["PhaseI_140PU_v2"] -> SetOptions(true,  kOrange);
    MCSample["PhaseI_0PU"] -> SetOptions(true,  1);
    MCSample["PhaseI_140PU"] -> SetOptions(true,  4);
    MCSample["PhaseII3_140PU"] -> SetOptions(true,  kGreen+2);
    MCSample["PhaseII4_140PU"] -> SetOptions(true,  2);
    //MCSample["PhaseII4v2_140PU"] -> SetOptions(true,  2);
    //MCSample["Snowmass_0PU"] -> SetOptions(true,  2);
    //MCSample["Snowmass_140PU"] -> SetOptions(true,  kGreen);
    //MCSample["SnowmassOpt"] -> SetOptions(true,  6);

    //ProList.push_back("Snowmass_0PU");
    //ProList.push_back("Snowmass_140PU");
    ////ProList.push_back("SnowmassOpt");
    //ProList.push_back("PhaseI_0PU_v1");
    //ProList.push_back("PhaseI_0PU_v2");
    //ProList.push_back("PhaseI_140PU_v1");
    //ProList.push_back("PhaseI_140PU_v2");
    //ProList.push_back("PhaseII3_140PU_v1");
    //ProList.push_back("PhaseII3_140PU_v2");
    ProList.push_back("PhaseI_0PU");
    ProList.push_back("PhaseI_140PU");
    ProList.push_back("PhaseII3_140PU");
    ProList.push_back("PhaseII4_140PU");
    //ProList.push_back("PhaseII4v2_140PU");
  }


  for(std::map<std::string, HTSample*>::iterator it=MCSample.begin();
    it!=MCSample.end(); it++)
  {
    it->second->InitSample(InitLumifb*1000);
  } 
  //----------------------------------------------------------------------------
  // Setting the drawing order by a vector 
  //----------------------------------------------------------------------------
  std::vector<std::string> passive_var;
  //passive_var.push_back("GenMuonEta_0");
  //passive_var.push_back("GenMuonPt_0");
  //passive_var.push_back("GenEleEta_0");
  //passive_var.push_back("GenElePt_0");
  //passive_var.push_back("GenTauEta_0");
  //passive_var.push_back("GenTauPt_0");
  //passive_var.push_back("GenJetEta_0");
  //passive_var.push_back("GenJetPt_0");
  //passive_var.push_back("GenMuonEta_1");
  //passive_var.push_back("GenMuonPt_1");
  //passive_var.push_back("GenEleEta_1");
  //passive_var.push_back("GenElePt_1");
  //passive_var.push_back("GenTauEta_1");
  //passive_var.push_back("GenTauPt_1");
  //passive_var.push_back("GenJetEta_1");
  //passive_var.push_back("GenJetPt_1");
  //passive_var.push_back("GenMuonEta_2");
  //passive_var.push_back("GenMuonPt_2");
  //passive_var.push_back("GenEleEta_2");
  //passive_var.push_back("GenElePt_2");
  //passive_var.push_back("GenTauEta_2");
  //passive_var.push_back("GenTauPt_2");
  //passive_var.push_back("GenJetEta_2");
  //passive_var.push_back("GenJetPt_2");
  //passive_var.push_back("GenMuonEta_7");
  //passive_var.push_back("GenMuonPt_7");
  //passive_var.push_back("GenEleEta_7");
  //passive_var.push_back("GenElePt_7");
  //passive_var.push_back("GenTauEta_7");
  //passive_var.push_back("GenTauPt_7");
  //passive_var.push_back("GenJetEta_7");
  //passive_var.push_back("GenJetPt_7");
  //passive_var.push_back("GenMuonEta_3");
  //passive_var.push_back("GenMuonPt_3");
  //passive_var.push_back("GenEleEta_3");
  //passive_var.push_back("GenElePt_3");
  //passive_var.push_back("GenTauEta_3");
  //passive_var.push_back("GenTauPt_3");
  //passive_var.push_back("GenJetEta_3");
  //passive_var.push_back("GenJetPt_3");
  //passive_var.push_back("GenMuonEta_6");
  //passive_var.push_back("GenMuonPt_6");
  //passive_var.push_back("GenEleEta_6");
  //passive_var.push_back("GenElePt_6");
  //passive_var.push_back("GenTauEta_6");
  //passive_var.push_back("GenTauPt_6");
  //passive_var.push_back("GenJetEta_6");
  //passive_var.push_back("GenJetPt_6");
  //passive_var.push_back("GenMuonEta_5");
  //passive_var.push_back("GenMuonPt_5");
  //passive_var.push_back("GenEleEta_5");
  //passive_var.push_back("GenElePt_5");
  //passive_var.push_back("GenTauEta_5");
  //passive_var.push_back("GenTauPt_5");
  //passive_var.push_back("GenJetEta_5");
  //passive_var.push_back("GenJetPt_5");
  //passive_var.push_back("GenMet_0");
  //passive_var.push_back("MET_0");
  //passive_var.push_back("MET_6");
  //passive_var.push_back("GenMeT_7");
  //passive_var.push_back("ABSdPhiJJ_0");
  //passive_var.push_back("J1Eta_0");
  //passive_var.push_back("J1Phi_0");
  //passive_var.push_back("J1Pt_0");
  //passive_var.push_back("J2Eta_0");
  //passive_var.push_back("J2Phi_0");
  //passive_var.push_back("J2Pt_0");
  //passive_var.push_back("J3Eta_0");
  //passive_var.push_back("J3Phi_0");
  //passive_var.push_back("J3Pt_0");
  //passive_var.push_back("dEtaJJ_0");
  //passive_var.push_back("dPhiJJ_0");
  //passive_var.push_back("dPtJJ_0");
  //passive_var.push_back("dRJJ_0");
  //passive_var.push_back("MHTMET_0");
  //passive_var.push_back("MetAsys_0");
  //passive_var.push_back("MET_0");
  //passive_var.push_back("NJets_0");
  //passive_var.push_back("MJJ_7");
  //passive_var.push_back("MJJ_8");
  //passive_var.push_back("MJJ_9");
  //passive_var.push_back("dRJJ_0");
  //passive_var.push_back("dRJJ_7");
  //passive_var.push_back("dRJJ_8");
  //passive_var.push_back("dRJJ_9");
  //passive_var.push_back("JetEta_0");
  //passive_var.push_back("J1Pt_0");
  //passive_var.push_back("J1Pt_7");
  //passive_var.push_back("J1Pt_14");
  //passive_var.push_back("J1Pt_8");
  //passive_var.push_back("J1Pt_9");
  //passive_var.push_back("J2Pt_0");
  //passive_var.push_back("J2Pt_7");
  //passive_var.push_back("J1Pt_7");
  //passive_var.push_back("J2Pt_7");
  //passive_var.push_back("J2Pt_8");
  //passive_var.push_back("J2Pt_9");
  //passive_var.push_back("JetEta");
  //passive_var.push_back("JetEta20");
  //passive_var.push_back("JetEta30");
  //passive_var.push_back("JetEta40");
  //passive_var.push_back("JetEta50");
  //passive_var.push_back("JetEta70");
  //passive_var.push_back("JetEta100");
  //passive_var.push_back("JetEta120");
  //passive_var.push_back("JetEta150");
  //passive_var.push_back("JetEta");
  //passive_var.push_back("J1PU_0");
  //passive_var.push_back("J2PU_0");
  //passive_var.push_back("J3PU_0");
  //passive_var.push_back("J1PU_14");
  //passive_var.push_back("J2PU_14");
  //passive_var.push_back("J3PU_14");
  //passive_var.push_back("J1PU_7");
  //passive_var.push_back("J2PU_7");
  //passive_var.push_back("J3PU_7");
  //passive_var.push_back("CentralJetEta_3");
  //passive_var.push_back("CentralJetEta_4");
  //passive_var.push_back("CentralJetOrder_3");
  //passive_var.push_back("CentralJetOrder_4");
  //passive_var.push_back("CentralJetPt_3");
  //passive_var.push_back("CentralJetPt_4");
  //passive_var.push_back("CentralPUJetOrder_3");
  //passive_var.push_back("CentralPUJetOrder_4");
  //passive_var.push_back("CentralJetEta_10");
  //passive_var.push_back("CentralJetEta_11");
  //passive_var.push_back("CentralJetOrder_10");
  //passive_var.push_back("CentralJetOrder_11");
  //passive_var.push_back("CentralJetPt_10");
  //passive_var.push_back("CentralJetPt_11");
  //passive_var.push_back("CentralPUJetOrder_10");
  //passive_var.push_back("CentralPUJetOrder_11");
  //passive_var.push_back("NEle");
  //passive_var.push_back("NMuon");
  //passive_var.push_back("NPhoton");
  //passive_var.push_back("NJets_0");

  //passive_var.push_back("MET_0");
  //passive_var.push_back("MHT_0");
  //passive_var.push_back("MHT_1");
  //passive_var.push_back("MHT_2");
  //passive_var.push_back("MHT_6");
  ////passive_var.push_back("MHT_7");
  //passive_var.push_back("dRJJ_0");
  //passive_var.push_back("MJJ_0");
  //passive_var.push_back("MJJ_1");
  //passive_var.push_back("MJJ_2");
  //passive_var.push_back("MJJ_6");

  passive_var.push_back("AlphaT_0");
  passive_var.push_back("RazorMRT_0");
  passive_var.push_back("RazorMR_0");
  passive_var.push_back("RazorR_0");
  passive_var.push_back("MT2_Eta_0");
  passive_var.push_back("MT2_J1J2_0");
  passive_var.push_back("MT2_Razor_0");
  passive_var.push_back("MHTHT_0");
  passive_var.push_back("METHT_0");

  passive_var.push_back("AlphaT_7");
  passive_var.push_back("RazorMRT_7");
  passive_var.push_back("RazorMR_7");
  passive_var.push_back("RazorR_7");
  //passive_var.push_back("dEtaJJ_7");
  //passive_var.push_back("dEtaMHTJ1_7");
  //passive_var.push_back("dEtaMHTJ2_7");
  //passive_var.push_back("dEtaMHTJ3_7");
  //passive_var.push_back("dPtJJ_7");
  //passive_var.push_back("dPtMHTJ1_7");
  //passive_var.push_back("dPtMHTJ2_7");
  //passive_var.push_back("dPtMHTJ3_7");
  //passive_var.push_back("dRJJ_7");
  //passive_var.push_back("dRMHTJ1_7");
  //passive_var.push_back("dRMHTJ2_7");
  //passive_var.push_back("dRMHTJ3_7");
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);
  const std::string nostk = "noStack";

  //----------------------------------------------------------------------------
  //  Looping over each hist
  //----------------------------------------------------------------------------
  for (std::vector<std::string>::iterator hit = passive_var.begin();
      hit != passive_var.end(); hit++ )
  {

    std::cout << " Drawing His : " << *hit << std::endl;
    c1->cd();
    c1->Clear();

    TLegend *f = NULL;

    if (hit->find("JetEta") != std::string::npos)
      //f = new TLegend(0.4244966,0.1751055,0.6610738,0.4683544,NULL,"brNDC");
      //f = new TLegend(0.3691275,0.1666667,0.6057047,0.4599156, "W(l#nu) + jets","brNDC");
      f = new TLegend(0.3389262,0.1610169,0.5755034,0.4555085,"W(l#nu) + jets","brNDC");
    else if (hit->find("dRJJ") != std::string::npos)
      //f = new TLegend(0.4244966,0.1751055,0.6610738,0.4683544,NULL,"brNDC");
      //f = new TLegend(0.3691275,0.1666667,0.6057047,0.4599156, "W(l#nu) + jets","brNDC");
      f = new TLegend(0.1895973,0.1582278,0.4261745,0.4514768,"W(l#nu) + jets","brNDC");
    else
      //f = new TLegend(0.5889262,0.6383051,0.8255034,0.9327966,NULL,"brNDC");
      //f = new TLegend(0.4832215,0.5974576,0.7197987,0.8919492,NULL,"brNDC");
      //f = new TLegend(0.5654362,0.6144068,0.8020134,0.9088983,"W(l#nu) + jets","brNDC");
      f = new TLegend(0.4832215,0.5847458,0.7197987,0.8792373,"W(l#nu) + jets","brNDC");
      //f = new TLegend(0.4832215,0.5847458,0.7197987,0.8792373, detector.c_str(),"brNDC");
      //f = new TLegend(0.2516779,0.1751055,0.488255,0.4683544,detector.c_str(),"brNDC");

    f->SetBorderSize(0);
    f->SetFillStyle(0); //transparent hollow?
    f->SetTextFont(62); 
    f->SetTextSize(0.04);

    std::string Xlabel;
    std::string Ylabel;
    int count = 0;
    TAxis *yaxis = NULL;
    TAxis *xaxis = NULL;
    double ymax = 0;
    double ymin = 0;

    for(std::vector<std::string>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {
 
      std::cout << *it << std::endl;
      TH1F* h = NULL;
      if (*it == "Backgrounds")
      {
        TH1F* temp  = MCSample["Wlv"] -> GetTH1(*hit);
        h = (TH1F*)temp->Clone();
        std::cout << " original " << h->Integral() <<"  Nbins " << h->GetNbinsX() << std::endl;
        h->Add(MCSample["TT"] -> GetTH1(*hit));
        std::cout << " original " << h->Integral() <<"  Nbins " << h->GetNbinsX() << std::endl;
        h->Add(MCSample["Zvv"] -> GetTH1(*hit));
        std::cout << " original " << h->Integral() <<"  Nbins " << h->GetNbinsX() << std::endl;
      }
      else
        h = MCSample[*it]->GetTH1(*hit);
      //TH1F* h2 = MCSample[*it]->GetTH1("GenJetEta_1");
      //TH1F* h2 = MCSample[*it]->GetTH1("GenTauEta_0");
      //TH1F* h2 = MCSample[*it]->GetTH1("GenMuonEta_0");
      //h->Divide(h2);
      //h->Rebin(10);
      //h->Rebin(2);
      //h->Sumw2();
      //if (hit->find( "MJJ") != std::string::npos) h->Rebin(20);
      //if (*hit == "MHT") h->Rebin(4);
      std::cout << " original " << h->Integral() <<"  Nbins " << h->GetNbinsX() << std::endl;
      if (h->Integral() != 0)
      {
        h->Scale(1 / h->Integral());
      }
      
      std::cout << "NA : " << h->GetTitle() << " from : " << *it << "int " << h->Integral()<< std::endl;
      Xlabel = h->GetXaxis()->GetTitle();
      Ylabel = h->GetYaxis()->GetTitle();
      std::cout << " Xlabel " << Xlabel << std::endl;
      if (Xlabel == "")
      {
        Xlabel = h->GetTitle();
        if (Xlabel.find("(NoCut)") != std::string::npos)
        {
          Xlabel.replace(Xlabel.find("(NoCut)"), 7, "");
        }
        if (Xlabel.find("(CTMet200)") != std::string::npos)
        {
          Xlabel.replace(Xlabel.find("(CTMet200)"), 10, "");
        }
        if (Xlabel.find("(dEta42)") != std::string::npos)
        {
          Xlabel.replace(Xlabel.find("dEta42"), 6, "VBF selection");
        }
        if (Xlabel.find("(CTJ1)") != std::string::npos)
        {
          Xlabel.replace(Xlabel.find("CTJ1"), 4, "Jet1 P_{T}");
        }
        if (Xlabel.find("(CTJ2)") != std::string::npos)
        {
          Xlabel.replace(Xlabel.find("CTJ2"), 4, "Jet2 P_{T}");
        }
      }
      Xlabel = h->GetTitle();
      if (Xlabel.find("(VBFCut)") != std::string::npos)
      {
        std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
        Xlabel.replace(Xlabel.find("VBFCut"), 6, "VBF selection");
      }
      if (Xlabel.find("All Jets") != std::string::npos)
      {
        Xlabel.replace(Xlabel.find("All Jets"), 8, "Jets");
      }
      if (Xlabel.find("Gen ") != std::string::npos)
      {
        Xlabel.replace(Xlabel.find("Gen "), 4, "");
      }
      if (hit->find("J1Pt_7") != std::string::npos)
      {
        Xlabel = "p_{T}^{jet1} [GeV]";
      }
      if (hit->find("J2Pt_7") != std::string::npos)
      {
        Xlabel = "p_{T}^{jet2} [GeV]";
      }
      if (hit->find("MJJ_") != std::string::npos)
      {
        Xlabel = "M_{jj} (GeV)";
      }
      if (hit->find("MHT_") != std::string::npos)
      {
        Xlabel = "#slash{H}_{T} (GeV)";
      }
      if (*hit == "JetEta")
      {
        Xlabel = "#eta_{jets}";
      }


      //h->SetTitle(Label.Data());
      h->SetTitle("");

      h->SetLineWidth(3);
      if (*it == "Backgrounds")
      h->SetLineColor(4);
      else
      h->SetLineColor(MCSample[*it]->Color);
      //h->SetLineColor(MCSample[*it]->Color);
      //if (Label != "Compare")
      //{
      //f->AddEntry(h, it->c_str(), "fl");
      //}

      if (count == 0)
      {
        yaxis = h->GetYaxis();
        xaxis = h->GetXaxis();
        count++;
      }

      //std::size_t found = Ylabel.find("log_");
      //if (found != std::string::npos)
      if (true)
      {
        //Ylabel.replace(found, 4, "");
        c1->SetLogy();
        //h->GetYaxis()->SetTitle("Events (scaled to one)");
        //h->GetYaxis()->SetTitle("Events / 20 GeV");
        //if (hit->find("JetEta") != std::string::npos)
        //{
          //h->GetYaxis()->SetTitle("Number of jets");
        //}
        //else
          h->GetYaxis()->SetTitle("Events");

        //h->GetYaxis()->SetTitle(Ylabel.c_str());
      }
      h->GetXaxis()->SetTitle(Xlabel.c_str());
      //h->GetYaxis()->SetTitle("Normalized to Unit Area");
      //c1->SetLogy();



      if (*it == "Wino100")
      {
        //h->Scale(10);
        //f->AddEntry(h, "Wino 112 GeV (x10)", "fl");
        //h->Scale(100);
        //f->AddEntry(h, "Wino 112 GeV (x100)", "fl");
        f->AddEntry(h, "Wino 112 GeV", "fl");
        //f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 112 GeV", "fl");
      }else if (*it == "Wino200")
      {
        //h->Scale(20);
        //f->AddEntry(h, "Wino 200 GeV (x20)", "fl");
        //h->Scale(200);
        //f->AddEntry(h, "Wino 200 GeV (x200)", "fl");
        f->AddEntry(h, "Wino 200 GeV", "fl");
      }else if (*it == "Wino500")
      {
        //h->Scale(200);
        //f->AddEntry(h, "Wino 500 GeV (x200)", "fl");
        //h->Scale(2000);
        //f->AddEntry(h, "Wino 500 GeV (x2000)", "fl");
        f->AddEntry(h, "Wino 500 GeV", "fl");
      }else if (*it == "TT") {
        f->AddEntry(h, "t#bar{t} + jets", "fl");
      }else if (*it == "Wlv")
      {
        f->AddEntry(h, "W #rightarrow l #nu", "fl");
        //f->AddEntry(h, "W + jets", "fl");
      }else if (*it == "Zvv")
      {
        f->AddEntry(h, "Z #rightarrow#nu#bar{#nu}", "fl");
        //f->AddEntry(h, "Z(#rightarrow#nu#bar{#nu}) + jets", "fl");
      }else if (*it == "TTFL") {
        f->AddEntry(h, "t#bar{t} #rightarrowllbb", "fl");
      }else if (*it == "TTSL") {
        f->AddEntry(h, "t#bar{t} #rightarrow lqqbb", "fl");
      } else if (it->find("PU") != std::string::npos)
      {
        std::cout << "=================================" <<*it << std::endl;
        if (*it == "PhaseI_0PU")     f->AddEntry(h, "Phase I, <PU>=0",   "fl");
        if (*it == "PhaseI_140PU")   f->AddEntry(h, "Phase I, <PU>=140", "fl");
        if (*it == "PhaseII3_140PU") f->AddEntry(h, "Phase II Conf3, <PU>=140", "fl");
        if (*it == "PhaseII4_140PU") f->AddEntry(h, "Phase II Conf4, <PU>=140", "fl");

        //TString leg = *it;
        //f->AddEntry(h, leg.ReplaceAll("_", " "), "fl");
      }
      else
        f->AddEntry(h, it->c_str(), "fl");

        ymin = ymin < h->GetMinimum() ? ymin : h->GetMinimum();
        ymax = ymax > h->GetMaximum() ? ymax : h->GetMaximum();

      if (it == ProList.begin())
      {

        h->Draw();
      }
      else
        h->Draw("same");
    }

    f->Draw();
    DrawTitle();

    yaxis->SetTitleOffset(1.0);
    xaxis->SetTitleOffset(0.9);
    xaxis->SetTitleSize(0.06);
    yaxis->SetTitleSize(0.06);
    xaxis->SetLabelSize(0.05);
    yaxis->SetLabelSize(0.05);

    ymin = ymin == 0 ? ymax/100 : ymin;
    yaxis->SetRangeUser(0.1*ymin, 1.5*ymax);

    //if (hit->find("MJJ") != std::string::npos)
    //{
      //xaxis->SetRangeUser(0, 3000);
    //}

    if (hit->find("MET_") != std::string::npos)
    {
      xaxis->SetRangeUser(0, 1000);
      //xaxis->SetRangeUser(0, 500);
    }

    if (hit->find("MJJ_") != std::string::npos)
    {
      xaxis->SetRangeUser(0, 4500);
    }

    if (hit->find("MHT_") != std::string::npos)
    {
      xaxis->SetRangeUser(0, 500);
    }
    if (hit->find("MET_") != std::string::npos)
    {
      xaxis->SetRangeUser(0, 500);
    }
    //if (hit->find("Pt_") != std::string::npos)
    //{
      //xaxis->SetRangeUser(0, 500);
    //}
    c1->RedrawAxis();
    //TString outname = detector + "_" + PU + "_" + Label+ "_"+*hit + ".pdf";
    //c1->Print(outname);
    TString outname = detector+ "_" + PU +"_" + Label+ "_"+*hit + ".png";
    c1->Print(outname);
    //outname = detector+ "_" + PU +"_" + Label+ "_"+*hit + ".C";
    //c1->Print(outname);
    //outname = detector+ "_" + PU +"_" + Label+ "_"+*hit + ".root";
    //c1->Print(outname);
  }


  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------



// ===  FUNCTION  ============================================================
//         Name:  DrawTitle
//  Description:  
// ===========================================================================
int DrawTitle()
{
  std::string title = " fd";
  
  TLatex TitleBox;
  TitleBox.SetNDC();
  TitleBox.SetTextSize(0.04);
  TitleBox.DrawLatex(0.13255,0.940928, "CMS Simulation, #sqrt{s} = 14 TeV, L = 3000 fb^{-1}");
  return 1;
}       // -----  end of function DrawTitle  -----
