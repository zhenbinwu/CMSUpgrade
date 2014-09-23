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
#include <map>

int main ( int argc, char *argv[] )
{

  const double InitLumifb     = 3000;
  const std::string& PU       = "NoPileUp";
  const std::string& analysis = "NoPileUp";
  const std::string& dir      = "./Sbottom/";
  const std::string& detector = "Snowmass";
  const std::string& Label    = "Sbottom";
  //const std::string& PU = "140PileUp";
  //const std::string& Label = "Zvv";
  //const std::string& Label = "ZvvJJ";
  //const std::string& Label = "ZvvJ";
  //const std::string& Label = "test";
  //const std::string& Label = "Zvv0";
  //const std::string& Label = "Zvv";
  //const std::string& Label = "WlvJJ";
  //const std::string& Label = "WlvJ";
  //const std::string& Label = "Wlv0";
  //const std::string& Label = "Wlv";
  //const std::string& Label = "Sbottom140PU";
  //const std::string& Label = "Wino200";
  //const std::string& Label = "Compare";
  //const std::string& dir = "./TOOL/";

  std::map<std::string, HTSample*> MCSample;
  std::vector<std::string> ProList; 
  StopStyle();

  if (Label == "Sbottom")
  {
    MCSample["0PU_Sbottom15"]    = new HTSample(dir, "Sbottom15_14TeV", PU, detector);
    MCSample["0PU_Sbottom50"]    = new HTSample(dir, "Sbottom50_14TeV", PU, detector);
    MCSample["0PU_Sbottom100"]    = new HTSample(dir, "Sbottom100_14TeV", PU, detector);
    MCSample["0PU_Sbottom150"]    = new HTSample(dir, "Sbottom150_14TeV", PU, detector);
    MCSample["0PU_Sbottom200"]    = new HTSample(dir, "Sbottom200_14TeV", PU, detector);
    MCSample["0PU_Sbottom300"]    = new HTSample(dir, "Sbottom300_14TeV", PU, detector);
    //MCSample["0PU_Sbottom400"]    = new HTSample(dir, "Sbottom400_14TeV", PU, detector);
    MCSample["Wlv"]      = new HTSample(dir, "Wlv*_14TEV_HT", PU, detector);
    //MCSample["Wtv"]      = new HTSample(dir, "Wtv*_14TEV_HT", PU, detector);
    //MCSample["Wev"]      = new HTSample(dir, "Wev*_14TEV_HT", PU, detector);
    //MCSample["Wmv"]      = new HTSample(dir, "Wmv*_14TEV_HT", PU, detector);
    MCSample["Zvv"]      = new HTSample(dir, "Zvv*_14TEV_HT", PU, detector);
    MCSample["TT"]      = new HTSample(dir, "TT_14TEV_HT", PU, detector);
    MCSample["TTHad"]      = new HTSample(dir, "TTHad_14TEV_HT", PU, detector);
    MCSample["TTFLep"]      = new HTSample(dir, "TTFLep_14TEV_HT", PU, detector);
    MCSample["TTSLep"]      = new HTSample(dir, "TTSLep_14TEV_HT", PU, detector);
    MCSample["0PU_Sbottom15"] -> SetOptions(true,  1);
    MCSample["0PU_Sbottom50"] -> SetOptions(true,  2);
    MCSample["0PU_Sbottom100"] -> SetOptions(true,  3);
    MCSample["0PU_Sbottom150"] -> SetOptions(true,  4);
    MCSample["0PU_Sbottom200"] -> SetOptions(true,  6);
    MCSample["0PU_Sbottom300"] -> SetOptions(true,  2);
    //MCSample["0PU_Sbottom400"] -> SetOptions(true,  8);
    MCSample["Wlv"]   -> SetOptions(true,  4);
    //MCSample["Wtv"]   -> SetOptions(true,  41);
    //MCSample["Wev"]   -> SetOptions(true,  42);
    //MCSample["Wmv"]   -> SetOptions(true,  44);
    MCSample["Zvv"]  -> SetOptions(true,  6);
    MCSample["TT"]  -> SetOptions(true,  kGreen+2);
    MCSample["TTHad"]  -> SetOptions(true,  1);
    MCSample["TTFLep"]  -> SetOptions(true,  2);
    MCSample["TTSLep"]  -> SetOptions(true,  3);
    //
    //ProList.push_back("0PU_Sbottom15");
    //ProList.push_back("0PU_Sbottom50");
    //ProList.push_back("0PU_Sbottom100");
    //ProList.push_back("0PU_Sbottom150");
    //ProList.push_back("0PU_Sbottom200");
    //ProList.push_back("0PU_Sbottom300");
    //ProList.push_back("0PU_Sbottom400");
    //ProList.push_back("Wlv");
    //ProList.push_back("Zvv");
    ProList.push_back("TT");
    ProList.push_back("TTHad");
    ProList.push_back("TTFLep");
    ProList.push_back("TTSLep");
    //ProList.push_back("Wtv");
    //ProList.push_back("Wev");
    //ProList.push_back("Wmv");
  }

  if (Label == "Sbottom140PU")
  {
    MCSample["140PU_Sbottom15"]    = new HTSample("Sbottom/", "Sbottom15_14TeV", "140PileUp", "Snowmass");
    MCSample["140PU_Sbottom50"]    = new HTSample("Sbottom/", "Sbottom50_14TeV", "140PileUp", "Snowmass");
    MCSample["140PU_Sbottom100"]    = new HTSample("Sbottom/", "Sbottom100_14TeV", "140PileUp", "Snowmass");
    MCSample["140PU_Sbottom150"]    = new HTSample("Sbottom/", "Sbottom150_14TeV", "140PileUp", "Snowmass");
    MCSample["140PU_Sbottom200"]    = new HTSample("Sbottom/", "Sbottom200_14TeV", "140PileUp", "Snowmass");
    MCSample["140PU_Sbottom15"] -> SetOptions(true,  1);
    MCSample["140PU_Sbottom50"] -> SetOptions(true,  2);
    MCSample["140PU_Sbottom100"] -> SetOptions(true,  3);
    MCSample["140PU_Sbottom150"] -> SetOptions(true,  4);
    MCSample["140PU_Sbottom200"] -> SetOptions(true,  6);
    ProList.push_back("140PU_Sbottom15");
    ProList.push_back("140PU_Sbottom50");
    ProList.push_back("140PU_Sbottom100");
    ProList.push_back("140PU_Sbottom150");
    ProList.push_back("140PU_Sbottom200");
  }
  if (Label == "Wino200")
  {
    MCSample["Snowmass_0PU"]    = new HTSample("Snowmass/", "Wino200_14TeV", "NoPileUp", "Snowmass");
    MCSample["Snowmass_140PU"]    = new HTSample("Snowmass/", "Wino200_14TeV", PU, "Snowmass");
    MCSample["PhaseI_0PU"]      = new HTSample("TOOL", "Wino200_14TeV", "NoPileUp", "PhaseI");
    MCSample["PhaseI_140PU"]      = new HTSample("TOOL", "Wino200_14TeV", PU, "PhaseI");
    MCSample["PhaseII3_140PU"]    = new HTSample("TOOL", "Wino200_14TeV", PU, "PhaseII3");
    MCSample["PhaseII4_140PU"]    = new HTSample("TOOL", "Wino200_14TeV", PU, "PhaseII4");
  }

  if (Label == "Wlv")
  {
    MCSample["Snowmass_0PU"]    = new HTSample("Snowmass/", "WlvJ*_14TEV_HT", "NoPileUp", "Snowmass");
    MCSample["Snowmass_140PU"]    = new HTSample("Snowmass/", "WlvJ*_14TEV_HT", PU, "SnowmassOpt");
    //MCSample["SnowmassOpt"] = new HTSample("Snowmass/", "Wlv_14TEV_HT", PU, "SnowmassOpt");
    MCSample["PhaseI_0PU"]      = new HTSample("TOOL", "WlvJ*_14TEV_HT", "NoPileUp", "PhaseI");
    MCSample["PhaseI_140PU"]      = new HTSample("TOOL", "WlvJ*_14TEV_HT", PU, "PhaseI");
    MCSample["PhaseII3_140PU"]    = new HTSample("TOOL", "WlvJ*_14TEV_HT", PU, "PhaseII3");
    MCSample["PhaseII4_140PU"]    = new HTSample("TOOL", "WlvJ*_14TEV_HT", PU, "PhaseII4");
    MCSample["PhaseII4v2_140PU"]    = new HTSample("PhaseII4V2/", "WlvJ*_14TEV_HT", PU, "PhaseII4v2");
    //MCSample["PhaseI_0PU"]      = new HTSample("BWTool", "Wlv*_14TEV_HT", "NoPileUp", "PhaseI");
    //MCSample["PhaseI_140PU"]      = new HTSample("BWTool", "Wlv*_14TEV_HT", PU, "PhaseI");
    //MCSample["PhaseII3_140PU"]    = new HTSample("BWTool", "Wlv*_14TEV_HT", PU, "PhaseII3");
    //MCSample["PhaseII4_140PU"]    = new HTSample("BWTool", "Wlv*_14TEV_HT", PU, "PhaseII4");
  }

  if (Label == "Compare")
  {
    MCSample["Wino100"]      = new HTSample(dir, "Wino100_14TeV", PU, detector);
    MCSample["Wino200"]      = new HTSample(dir, "Wino200_14TeV", PU, detector);
    //MCSample["Wino500"]      = new HTSample("TOOL", "Wino500_14TeV", PU, detector);
    MCSample["Wlv"]      = new HTSample(dir, "Wlv*_14TEV_HT", PU, detector);
    MCSample["Wtv"]      = new HTSample(dir, "Wtv*_14TEV_HT", PU, detector);
    MCSample["Wev"]      = new HTSample(dir, "Wev*_14TEV_HT", PU, detector);
    MCSample["Wmv"]      = new HTSample(dir, "Wmv*_14TEV_HT", PU, detector);
    MCSample["Zvv"]      = new HTSample(dir, "Zvv*_14TEV_HT", PU, detector);
    MCSample["TTSL"]      = new HTSample(dir, "TTSLep*_14TEV_HT", PU, detector);
    MCSample["TTFL"]      = new HTSample(dir, "TTFLep*_14TEV_HT", PU, detector);
    //MCSample["Wino100"]      = new HTSample("ECFA", "Wino100_14TeV", PU, "PhaseII4");
    //MCSample["Wino200"]      = new HTSample("ECFA", "Wino200_14TeV", PU, "PhaseII4");
    //MCSample["Wino500"]      = new HTSample("ECFA", "Wino500_14TeV", PU, "PhaseII4");
    //MCSample["Wlv"]      = new HTSample("ECFA", "Wlv*_14TEV_HT", PU, "PhaseII4");
    //MCSample["Zvv"]      = new HTSample("ECFA", "Zvv*_14TEV_HT", PU, "PhaseII4");
    //MCSample["TT"]      = new HTSample("ECFA", "TT*_14TEV_HT", PU, "PhaseII4");
    MCSample["Wino100"] -> SetOptions(true,  1);
    MCSample["Wino200"] -> SetOptions(true,  2);
    //MCSample["Wino500"] -> SetOptions(true,  6);
    MCSample["Wlv"]   -> SetOptions(true,  4);
    MCSample["Wtv"]   -> SetOptions(true,  41);
    MCSample["Wev"]   -> SetOptions(true,  42);
    MCSample["Wmv"]   -> SetOptions(true,  44);
    MCSample["Zvv"]  -> SetOptions(true,  6);
    MCSample["TTSL"]  -> SetOptions(true,  kGreen+2);
    MCSample["TTFL"]  -> SetOptions(true,  kOrange+9);
    ProList.push_back("Wlv");
    //ProList.push_back("Wtv");
    //ProList.push_back("Wev");
    //ProList.push_back("Wmv");
    //ProList.push_back("Zvv");
    ProList.push_back("TTFL");
    ProList.push_back("TTSL");
    ProList.push_back("Wino100");
    ProList.push_back("Wino200");
    //ProList.push_back("Wino500");
  }else{


    ////MCSample["Wino100"]      = new HTSample("ECFA", "Wino100_14TeV", PU, "PhaseII4");
    ////MCSample["Wino200"]      = new HTSample("ECFA", "Wino200_14TeV", PU, "PhaseII4");
    ////MCSample["Wlv"]      = new HTSample("ECFA", "Wlv*_14TEV_HT", PU, "PhaseII4");
    ////MCSample["Zvv"]      = new HTSample("ECFA", "Zvv*_14TEV_HT", PU, "PhaseII4");
    ////MCSample["TT"]      = new HTSample("ECFA", "TT*_14TEV_HT", PU, "PhaseII4");

    //MCSample["PhaseI_0PU"] -> SetOptions(true,  7);
    //MCSample["PhaseI_140PU"] -> SetOptions(true,  1);
    //MCSample["PhaseII3_140PU"] -> SetOptions(true,  6);
    //MCSample["PhaseII4_140PU"] -> SetOptions(true,  4);
    //MCSample["PhaseII4v2_140PU"] -> SetOptions(true,  2);
    //MCSample["Snowmass_0PU"] -> SetOptions(true,  2);
    //MCSample["Snowmass_140PU"] -> SetOptions(true,  kGreen);
    ////MCSample["SnowmassOpt"] -> SetOptions(true,  6);

    ////ProList.push_back("Snowmass_0PU");
    ////ProList.push_back("Snowmass_140PU");
    //////ProList.push_back("SnowmassOpt");
    //ProList.push_back("PhaseI_0PU");
    //ProList.push_back("PhaseI_140PU");
    //ProList.push_back("PhaseII3_140PU");
    //ProList.push_back("PhaseII4_140PU");
    //ProList.push_back("PhaseII4v2_140PU");
  }

  for(std::map<std::string, HTSample*>::iterator it=MCSample.begin();
      it!=MCSample.end(); it++)
  {
    it->second->InitSample(InitLumifb*1000);
    it->second->SetAnalysis(analysis);
  }


  //----------------------------------------------------------------------------
  // Setting the drawing order by a vector 
  //----------------------------------------------------------------------------
  ////ProList.push_back("Wino100");
  //ProList.push_back("Wino500");


  std::vector<std::string> passive_var;
  //passive_var.push_back("GenMuonEta_0");
  //passive_var.push_back("GenMuonPt_0");
  //passive_var.push_back("GenEleEta_0");
  //passive_var.push_back("GenElePt_0");
  //passive_var.push_back("GenTauEta_0");
  //passive_var.push_back("GenTauPt_0");
  passive_var.push_back("GenBJetEta_0");
  //passive_var.push_back("GenBJetPt_0");
  //passive_var.push_back("BJetEta_0");
  //passive_var.push_back("BJetPt_0");
  //passive_var.push_back("GenBJetEta_6");
  //passive_var.push_back("GenBJetPt_6");
  //passive_var.push_back("BJetEta_6");
  //passive_var.push_back("BJetPt_6");
  //passive_var.push_back("BJetPt_7");
  //passive_var.push_back("GenBJetEta_1");
  //passive_var.push_back("GenBJetPt_1");
  //passive_var.push_back("BJetEta_1");
  //passive_var.push_back("BJetPt_1");
  //passive_var.push_back("GenBJetEta_2");
  //passive_var.push_back("GenBJetPt_2");
  //passive_var.push_back("BJetEta_2");
  //passive_var.push_back("BJetPt_2");
  //passive_var.push_back("GenBJetEta_13");
  //passive_var.push_back("GenBJetPt_13");
  //passive_var.push_back("BJetEta_13");
  //passive_var.push_back("BJetPt_13");
  //passive_var.push_back("CentralJetOrder_3");
  //passive_var.push_back("CentralJetOrder_4");
  //passive_var.push_back("CentralBJetEta_13");
  //passive_var.push_back("CentralBJetPt_13");
  //passive_var.push_back("CentralBJetOrder_13");
  passive_var.push_back("NBJets_0");
  passive_var.push_back("NBJets_6");
  passive_var.push_back("NGenBJets_0");
  //passive_var.push_back("NCentralBJets_6");
  ////passive_var.push_back("GenMet_0");
  ////passive_var.push_back("MET_0");
  ////passive_var.push_back("MHT_0");
  //passive_var.push_back("ABSdPhiJJ_0");
  //passive_var.push_back("J1Eta_0");
  //passive_var.push_back("J1Phi_0");
  //passive_var.push_back("J1Pt_0");
  //passive_var.push_back("J1Pt_1");
  //passive_var.push_back("J1Pt_2");
  //passive_var.push_back("J2Eta_0");
  //passive_var.push_back("J2Phi_0");
  //passive_var.push_back("J2Pt_0");
  //passive_var.push_back("J2Pt_1");
  //passive_var.push_back("J2Pt_2");
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
  //passive_var.push_back("MET_6");
  //passive_var.push_back("MET_7");
  //passive_var.push_back("MHT_0");
  //passive_var.push_back("MHT_6");
  //passive_var.push_back("MHT_7");
  //passive_var.push_back("MJJ_0");
  //passive_var.push_back("MJJ_2");
  //passive_var.push_back("MJJ_3");
  //passive_var.push_back("MJJ_7");
  //passive_var.push_back("MJJ_8");
  //passive_var.push_back("MJJ_9");
  //passive_var.push_back("dRJJ_0");
  //passive_var.push_back("dRJJ_7");
  //passive_var.push_back("dRJJ_8");
  //passive_var.push_back("dRJJ_9");
  //passive_var.push_back("J1Pt_0");
  //passive_var.push_back("J1Pt_7");
  //passive_var.push_back("J1Pt_8");
  //passive_var.push_back("J1Pt_9");
  //passive_var.push_back("J2Pt_0");
  //passive_var.push_back("J2Pt_7");
  //passive_var.push_back("J2Pt_8");
  //passive_var.push_back("J2Pt_9");
  //passive_var.push_back("MHT_0");
  //passive_var.push_back("MetAsys_0");
  passive_var.push_back("JetEta");

  //passive_var.push_back("ABSdPhiJJ_14");
  //passive_var.push_back("J1Eta_14");
  //passive_var.push_back("MHT_0");
  //passive_var.push_back("MetAsys_0");
  //passive_var.push_back("CutFlow");

  //passive_var.push_back("ABSdPhiJJ_14");
  //passive_var.push_back("J1Eta_14");
  //passive_var.push_back("J1Phi_14");
  //passive_var.push_back("J1Pt_14");
  //passive_var.push_back("J2Eta_14");
  //passive_var.push_back("J2Phi_14");
  //passive_var.push_back("J2Pt_14");
  //passive_var.push_back("J3Eta_14");
  //passive_var.push_back("J3Phi_14");
  //passive_var.push_back("J3Pt_14");
  //passive_var.push_back("dEtaJJ_14");
  //passive_var.push_back("dPhiJJ_14");
  //passive_var.push_back("dPtJJ_14");
  //passive_var.push_back("dRJJ_14");
  //passive_var.push_back("MHTMET_14");
  //passive_var.push_back("MetAsys_14");
  //passive_var.push_back("MET_14");
  //passive_var.push_back("MJJ_9");
  //passive_var.push_back("MJJ_10");
  //passive_var.push_back("MHT_14");
  //passive_var.push_back("MHT_13");
  //passive_var.push_back("MetAsys_14");
  ////passive_var.push_back("MHT_8");
  //passive_var.push_back("NJets");
  //passive_var.push_back("NEle");
  //passive_var.push_back("NMuon");
  //passive_var.push_back("MuonEta_0");
  //passive_var.push_back("JetEta");
  //passive_var.push_back("EleEta_0");
  //passive_var.push_back("NPhoton");
  //passive_var.push_back("RawMet");
  ////The canvas to draw plots
  //passive_var.push_back("CrossSection");
  //passive_var.push_back("NEVT");
  //
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);
  const std::string nostk = "noStack";

  //----------------------------------------------------------------------------
  //  Looping over each hist
  //----------------------------------------------------------------------------
  for (std::vector<std::string>::iterator hit = passive_var.begin();
      hit != passive_var.end(); hit++ )
  {
    std::cout << " Drawing His : " << *hit << std::endl;
    //if (*hit != "CMet") continue;
    //Preparing for making this plot
    //std::cout << "Ploting " << *hit << std::endl;

    c1->cd();
    c1->Clear();

    //TLegend *f = new TLegend(0.6912752,0.6885593,0.9513423,0.9470339,NULL,"brNDC"); //DR 
    TLegend *f = new TLegend(0.6744966,0.6313559,0.9110738,0.9258475,NULL,"brNDC");
    //TLegend *f = new TLegend(0.4144295,0.1864407,0.6744966,0.4449153,NULL,"brNDC");
    //TLegend *f = new TLegend(0.5889262,0.6483051,0.8255034,0.9427966,NULL,"brNDC");
    //TLegend *f = new TLegend(0.1996644,0.190678,0.4362416,0.4851695,NULL,"brNDC"); //ABS DPhi 
    //TLegend *f = NULL;
    //if (hit->find("J1Pt") != std::string::npos)
    //{
    //f =  new TLegend(0.5855705,0.720339,0.8657718,0.9427966,NULL,"brNDC");
    //}
    //if (hit->find("J2Pt") != std::string::npos)
    //{
    //f = new TLegend(0.5805369,0.6398305,0.8171141,0.934322,NULL,"brNDC");
    ////}
    //TLegend *f = new TLegend( 0.1741611,0.6398305,0.4342282,0.8983051, NULL,"brNDC"); //Pt 
    //TLegend *f = new TLegend(0.4228188,0.6567797,0.659396,0.9512712,NULL,"brNDC"); //JetETa 
    f->SetBorderSize(0);
    f->SetFillStyle(0); //transparent hollow?
    f->SetTextFont(62); 
    f->SetTextSize(0.03);

    std::string Xlabel;
    std::string Ylabel;
    int count = 0;
    TAxis *yaxis = NULL;
    TAxis *xaxis = NULL;
    double ymax = 0;
    double ymin = 0;

    TString outrootname = detector+"_"+PU+"_" + Label+ "_"+*hit + ".root";
    TFile outroot(outrootname, "RECREATE");

    for(std::vector<std::string>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {

      std::cout << "process" << *it << std::endl;
      TH1F* h = MCSample[*it]->GetTH1(*hit);
      //outroot.cd();
      h->Clone(it->c_str())->Write();
      //TH1F* h2 = MCSample[*it]->GetTH1("GenJetEta_1");
      //TH1F* h2 = MCSample[*it]->GetTH1("GenTauEta_0");
      //TH1F* h2 = MCSample[*it]->GetTH1("GenMuonEta_0");
      //h->Divide(h2);
      //h->Rebin(2);
      //h->Sumw2();
      if (hit->find( "MJJ") != std::string::npos) h->Rebin(20);
      if (*hit == "MHT") h->Rebin(4);
      if (*hit == "CutFlow")
      {
        double bin1 = h->GetBinContent(1);
        std::cout << " bin1 " << bin1  << " " << 1/bin1<< std::endl;
        h->Scale((1/bin1));
        std::cout <<  h->GetBinContent(1) << std::endl;
      }
      else h->Scale(1/h->Integral());

      std::cout << "NA : " << h->GetTitle() << " from : " << *it << "int " << h->Integral()<< std::endl;
      Xlabel = h->GetXaxis()->GetTitle();
      Ylabel = h->GetYaxis()->GetTitle();
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
      //h->SetTitle(Label.Data());
      h->SetTitle("");

      h->SetLineWidth(3);
      h->SetLineColor(MCSample[*it]->Color);
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
      if (false)
      {
        //Ylabel.replace(found, 4, "");
        c1->SetLogy();
        //h->GetYaxis()->SetTitle("Events (scaled to one)");
        h->GetYaxis()->SetTitle("Events");
        //h->GetYaxis()->SetTitle(Ylabel.c_str());
      }
      h->GetXaxis()->SetTitle(Xlabel.c_str());
      //h->GetYaxis()->SetTitle("Normalized to Unit Area");
      //c1->SetLogy();


      ymin = ymin < h->GetMinimum() ? ymin : h->GetMinimum();
      ymax = ymax > h->GetMaximum() ? ymax : h->GetMaximum();

      if (*it == "Wino100")
      {
        f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 112 GeV", "fl");
      }else if (*it == "Wino200")
      {
        f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 200 GeV", "fl");
      }else if (*it == "Wino500")
      {
        f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 500 GeV", "fl");
      }else if (*it == "TT") {
        f->AddEntry(h, "t#bar{t} + jets", "fl");
      }else if (*it == "Wlv")
      {
        f->AddEntry(h, "W #rightarrow l#nu", "fl");
        //f->AddEntry(h, "W + jets", "fl");
      }else if (*it == "Zvv")
      {
        f->AddEntry(h, "Z #rightarrow#nu#bar{#nu}", "fl");
        //f->AddEntry(h, "Z(#rightarrow#nu#bar{#nu}) + jets", "fl");
      }else if (*it == "TTFL") {
        f->AddEntry(h, "t#bar{t} #rightarrowllbb", "fl");
      }else if (*it == "TTSL") {
        f->AddEntry(h, "t#bar{t} #rightarrow lqqbb", "fl");
      }
      else
        f->AddEntry(h, it->c_str(), "fl");


      if (it == ProList.begin())
      {

        h->Draw();
      }
      else
        h->Draw("same");
    }

    f->Draw();

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
      xaxis->SetRangeUser(0, 500);
    }

    if (hit->find("MHT_") != std::string::npos)
    {
      xaxis->SetRangeUser(0, 500);
    }

    if (hit->find("Pt_") != std::string::npos)
    {
      xaxis->SetRangeUser(0, 500);
    }
    TString outname = detector+"_"+PU+"_" + Label+ "_"+*hit + ".pdf";
    c1->Print(outname);
    outname = detector+"_"+PU+"_" + Label+ "_"+*hit + ".png";
    c1->Print(outname);

    outroot.Close();
  }

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

