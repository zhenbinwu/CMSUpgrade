// ===========================================================================
// 
//       Filename:  DelCut.cc
// 
//    Description:  Class for Cut flow 
//           TODO:  1. How to avoid the warning of replacing existing TH1F
// 
//        Version:  1.0
//        Created:  06/10/2013 08:18:19 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================


#include "DelCut.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCut
//      Method:  DelCut
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelCut::DelCut (DelAna *ana, std::string name )
{
  Ana     = ana;
  ProName = name;
  His     = new HistTool(name);
}  // ~~~~~  end of method DelCut::DelCut  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCut
//      Method:  DelCut
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelCut::DelCut ( const DelCut &other )
{
}  // ~~~~~  end of method DelCut::DelCut  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCut
//      Method:  ~DelCut
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelCut::~DelCut ()
{
  delete His;
}  // ~~~~~  end of method DelCut::~DelCut  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCut
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelCut&
DelCut::operator = ( const DelCut &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelCut::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DelCut::BookHistogram
//  Description:  
// ===========================================================================
bool DelCut::BookHistogram()
{
  
  BookLeptonEff();
  BookJetEff();
//----------------------------------------------------------------------------
//  Booking global histogram
//----------------------------------------------------------------------------
  His->AddTH1("NEVT", "Num. of Events", 2, 0, 2 );
  His->AddTH1("Weight", "Weight", 100, 0, 10 );

  // Initial the cutflow
  TString title = AnaCut == "DM" ? "SUSY VBF DM" : AnaCut;
  TH1F* temp = His->AddTH1("CutFlow", title.Data(), CutOrder.size(), 0 , CutOrder.size());
  for (int i = 0; i < CutOrder.size(); ++i)
    temp->GetXaxis()->SetBinLabel(i+1, CutOrder.at(i).c_str());

  His->AddTH1("NJets", "Num. of Jets", "Number of Jets", "Events", 40, 0, 40 );
  His->AddTH1("NEle", "Num. of Electrons", 10, 0, 10 );
  His->AddTH1("NMuon", "Num. of Muons", 10, 0, 10 );
  His->AddTH1("NPhoton", "Num. of Photons", 10, 0, 10 );

  His->AddTH1("ZMass", "Zmass", "M_{Z} [GeV]", "Events / 8 GeV",  50, 0, 400, 0, 1);
  His->AddTH1("RawMet", "RawMet", "Raw #slash{E}_{T} [GeV]", "Events / 5 GeV",  280, 0, 1400, 0, 1);
  His->AddTH1("GenMet", "GenMet", "Gen #slash{E}_{T} [GeV]", "Events / 5 GeV",  280, 0, 1400, 0, 1);

  His->AddTH1("JetEta", "#eta_{Jets}", 280, -7, 7 );
//----------------------------------------------------------------------------i
//  Booking histogram for MET performance study
//----------------------------------------------------------------------------
   if (ProName.find("MetDiEle") != std::string::npos
       || ProName.find("MetDiMuon") != std::string::npos)
    BookMetPerf();

  His->AddTH1("MetRelSys", "MetAsys", 
      "|#slash{H}_{T} - #slash{E}_{T}|/(#slash{H}_{T} + #slash{E}_{T})|[GeV]", 
      "Events",  400, 0, 1, 0, 1);

  TProfile* pro1 = new TProfile("MHTAsys", 
      "MHTAsys;#slash{H}_{T} [GeV];|#slash{H}_{T} - #slash{E}_{T}|/(#slash{H}_{T} + #slash{E}_{T}) [GeV]",  400, 0, 2000, "S");
  His->AddTPro(pro1);
  TProfile* pro2 = new TProfile("METAsys", 
      "METAsys;#slash{E}_{T} [GeV];|#slash{H}_{T} - #slash{E}_{T}|/(#slash{H}_{T} + #slash{E}_{T}) [GeV]",  400, 0, 2000, "S");
  His->AddTPro(pro2);
//----------------------------------------------------------------------------
//  Booking histogram for each cut
//----------------------------------------------------------------------------
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Met ~~~~~
  His->AddTH1C("MHT", "CMet", "#slash{H}_{T} [GeV]", 
      "Events/ 5 GeV", 200, 0, 1000, 0, 1);
  His->AddTH1C("MHTMET", "MHT - MET", "#slash{H}_{T} - #slash{E}_{T} [GeV]", 
      "Events/ 5 GeV", 600, -1000, 2000, 0, 1);
  His->AddTH1C("MET", "CRawMet", "#slash{E}_{T} [GeV]", 
      "Events/ 5 GeV", 200, 0, 1000, 0, 1);
  His->AddTH1C("MetAsys", "MetAsys", 
      "|#slash{H}_{T} - #slash{E}_{T}|/(#slash{H}_{T} + #slash{E}_{T})|[GeV]", 
      "Events",  400, 0, 1, 0, 1);
  //His->AddTH2C("MetMHT", "MetMHT", 
      //"#slash{H}_{T} [GeV]", "#slash{E}_{T} [GeV]", 
      //400, 0, 2000, 400, 0, 2000);

  His->AddTH2C("MJJMHT", "MJJMHT", 
      "M_{J1, J2} [GeV]", "#slash{H}_{T} [GeV]", 
      800, 0, 8000, 400, 0, 2000);
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jets ~~~~~
  His->AddTH1C("J1Pt", "Pt_{J1}", 40, 0, 1200.0 );
  His->AddTH1C("J1Eta", "#eta_{J1}", 14, -7, 7 );
  His->AddTH1C("J1Phi", "#phi_{J1}", 14, -7, 7);
  His->AddTH1C("J2Pt", "Pt_{J2}", 40, 0, 800.0 );
  His->AddTH1C("J2Eta", "#eta_{J2}", 14, -7, 7 );
  His->AddTH1C("J2Phi", "#phi_{J2}", 14, -7, 7);
  His->AddTH1C("J3Pt", "Pt_{J3}", 40, 0, 800.0 );
  His->AddTH1C("J3Eta", "#eta_{J3}", 14, -7, 7 );
  His->AddTH1C("J3Phi", "#phi_{J3}", 14, -7, 7);
  His->AddTH1C("JCPt", "Pt_{central jets}", 40, 0, 1200.0 );
  His->AddTH1C("JFPt", "Pt_{forward jets}", 40, 0, 1200.0 );

  His->AddTH1C("MJJ", "M_{J1, J2}", 4000, 0, 8000.0 );
  His->AddTH1C("dPtJJ", "#Delta Pt_{J1, J2}", 40, 0, 1200 );
  His->AddTH1C("dPhiJJ", "#Delta #phi_{J1, J2}", 14, -7, 7 );
  His->AddTH1C("ABSdPhiJJ", "|#Delta #phi_{J1, J2}|", 32, 0, 3.2 );
  His->AddTH1C("dEtaJJ", "#Delta #eta_{J1, J2}", 14, -7, 7 );
  His->AddTH1C("dRJJ", "#Delta R_{J1, J2}", 20, 0, 10.0 );

  His->AddTH1C("dPtMHTJ1", "#Delta Pt_{#slash{H}_{T}, J1}", 40, 0, 1200 );
  His->AddTH1C("dPhiMHTJ1", "#Delta #phi_{#slash{H}_{T}, J1}", 14, -7, 7 );
  His->AddTH1C("dEtaMHTJ1", "#Delta #eta_{#slash{H}_{T}, J1}", 14, -7, 7 );
  His->AddTH1C("dRMHTJ1", "#Delta R_{#slash{H}_{T}, J1}", 20, 0, 10.0 );

  His->AddTH1C("dPtMHTJ2", "#Delta Pt_{#slash{H}_{T}, J2}", 40, 0, 1200 );
  His->AddTH1C("dPhiMHTJ2", "#Delta #phi_{#slash{H}_{T}, J2}", 14, -7, 7 );
  His->AddTH1C("dEtaMHTJ2", "#Delta #eta_{#slash{H}_{T}, J2}", 14, -7, 7 );
  His->AddTH1C("dRMHTJ2", "#Delta R_{#slash{H}_{T}, J2}", 20, 0, 10.0 );

  His->AddTH1C("dPtMHTJ3", "#Delta Pt_{#slash{H}_{T}, J3}", 40, 0, 1200 );
  His->AddTH1C("dPhiMHTJ3", "#Delta #phi_{#slash{H}_{T}, J3}", 14, -7, 7 );
  His->AddTH1C("dEtaMHTJ3", "#Delta #eta_{#slash{H}_{T}, J3}", 14, -7, 7 );
  His->AddTH1C("dRMHTJ3", "#Delta R_{#slash{H}_{T}, J3}", 20, 0, 10.0 );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2D ~~~~~
  //His->AddTH1("J12D", "J1  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  //His->AddTH1("J22D", "J2  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  //His->AddTH1("J32D", "J3  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  if (ProName.find("Photon") != std::string::npos)
      His->AddTH2("MetVsPhoton", "Met Vs Photon", 10, 0 , 10, 100, 0, 1000);
}       // -----  end of function DelCut::BookHistogram  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCut::InitCutOrder
//  Description:  
// ===========================================================================
bool DelCut::InitCutOrder(std::string ana)
{
  AnaCut = ana;
   // The Cut flow for DM
   if (AnaCut == "DM")
   {
     CutOrder.clear();
     CutMap.clear();
     CutOrder.push_back("NoCut");
     CutOrder.push_back("MetFilter");
     CutOrder.push_back("NJet2");
     CutOrder.push_back("Met50");
     CutOrder.push_back("VBFJ1");
     CutOrder.push_back("VBFJ2");
     CutOrder.push_back("Eta2");
     CutOrder.push_back("dEta42");
     CutOrder.push_back("CTJ1");
     CutOrder.push_back("CTJ2");
     CutOrder.push_back("CTMjj");
     CutOrder.push_back("CTJ3");
     CutOrder.push_back("CTBJ");
     CutOrder.push_back("CTLep");
     CutOrder.push_back("CTMet200");
     CutOrder.push_back("CTdPhi");
     CutOrder.push_back("AllCut");

     CutMap["NoCut"]     = "00000000000000000";
     CutMap["MetFilter"] = "00000000000000001";
     CutMap["NJet2"]     = "00000000000000011";
     CutMap["Met50"]     = "00000000000000111";
     CutMap["VBFJ1"]     = "00000000000001111";
     CutMap["VBFJ2"]     = "00000000000011111";
     CutMap["Eta2"]      = "00000000000111111";
     CutMap["dEta42"]    = "00000000001111111";
     CutMap["CTJ1"]      = "00000000011111111";
     CutMap["CTJ2"]      = "00000000111111111";
     CutMap["CTMjj"]     = "00000001111111111";
     CutMap["CTJ3"]      = "00000011111111111";
     CutMap["CTBJ"]      = "00000111111111111";
     CutMap["CTLep"]     = "00001111111111111";
     CutMap["CTMet200"]  = "00011111111111111";
     CutMap["CTdPhi"]    = "00111111111111111";
     CutMap["AllCut"]    = "01111111111111111";


   }

   // The Cut flow for Higss
   if (AnaCut == "Higgs")
   {
     CutOrder.clear();
     CutMap.clear();

     CutOrder.push_back("NoCut");
     CutOrder.push_back("Trigger");
     CutOrder.push_back("METfilter");
     CutOrder.push_back("Eveto");
     CutOrder.push_back("Mveto");
     CutOrder.push_back("JetPt");
     CutOrder.push_back("JetEta");
     CutOrder.push_back("dEta");
     CutOrder.push_back("MET130");
     CutOrder.push_back("MJJ");
     CutOrder.push_back("CJV");
     CutOrder.push_back("dPhi");
     CutOrder.push_back("Tveto");
     CutOrder.push_back("Bveto");
     CutOrder.push_back("AllCut");


     CutMap["NoCut"]     = "000000000000000";
     CutMap["Trigger"]   = "000000000000001";
     CutMap["METfilter"] = "000000000000011";
     CutMap["Eveto"]     = "000000000000111";
     CutMap["Mveto"]     = "000000000001111";
     CutMap["JetPt"]     = "000000000011111";
     CutMap["JetEta"]    = "000000000111111";
     CutMap["dEta"]      = "000000001111111";
     CutMap["MET130"]    = "000000011111111";
     CutMap["MJJ"]       = "000000111111111";
     CutMap["CJV"]       = "000001111111111";
     CutMap["dPhi"]      = "000011111111111";
     CutMap["Tveto"]     = "000111111111111";
     CutMap["Bveto"]     = "001111111111111";
     CutMap["AllCut"]    = "011111111111111";
   }

   assert(CutOrder.size() == CutMap.size());
   His->Cutorder(CutOrder);
  
}       // -----  end of function DelCut::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CutFlow
//  Description:  
// ===========================================================================
bool DelCut::CutFlow(std::bitset<20> cutbit)
{

  return 1;
}       // -----  end of function DelCut::CutFlow  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillCut
//  Description:  Main function to be called in DelFill
// ===========================================================================
int DelCut::FillCut()
{
//----------------------------------------------------------------------------
//  Set up the DelAna
//----------------------------------------------------------------------------
  //Set Weight for this event, auto fill each his by HistTool
  //You can over write the weight by adding the weight in Filling
  His->SetWeight(Ana->Weight); 
  His->FillTH1("Weight", Ana->Weight);
  His->FillTH1("NJets", (int)Ana->vJet->size());
  His->FillTH1("NEle", (int)Ana->vElectron->size());
  His->FillTH1("NMuon", (int)Ana->vMuon->size());
  His->FillTH1("NPhoton", (int)Ana->vPhoton->size());
  His->FillTH1("RawMet", Ana->RawMet.Mod());
  His->FillTH1("Met", Ana->Met);
  His->FillTH1("MetRelSys", Ana->METAsys);
  His->FillTPro("MHTAsys", Ana->Met, Ana->METAsys);
  His->FillTPro("METAsys", Ana->RawMet.Mod(), Ana->METAsys);

  if (ProName.find("Photon") != std::string::npos)
    His->FillTH2("MetVsPhoton", (double)Ana->vPhoton->size(), Ana->Met);
//----------------------------------------------------------------------------
// Filling jets Globally
//----------------------------------------------------------------------------
   FillJets();
   His->FillTH1("ZMass", Ana->GenZvv.M());
   His->FillTH1("GenMet", Ana->GenZvv.Pt());

//----------------------------------------------------------------------------
//  Filling histogram for MET performance study
//----------------------------------------------------------------------------
   if (ProName.find("MetDiEle") != std::string::npos
       || ProName.find("MetDiMuon") != std::string::npos)
    FillMetPerf();

   //std::cout << " name " << ProName << " Met " << Ana->Met <<" PUCorMEt " << Ana->PUCorMet->Mod()<< " sysMet " << Ana->SysMet.Mod() << std::endl;
  for (int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<20> locbit(CutMap[CutOrder.at(i)]);
    if (CheckCut(locbit) == false) break;
    //if (CheckCut(locbit) == false) continue;
    // For HTBin sample, the cutflow should fill with event weight
    His->FillTH1("CutFlow", i); 
    // Filling by functions
    FillJets(i);
    FillMet(i);
    FillLepton(i);
  }

  return 1;
}       // -----  end of function DelCut::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckCut
//  Description:  
// ===========================================================================
bool DelCut::CheckCut(std::bitset<20> cutflag)
{
  if (AnaCut == "DM") return CheckDMCut(cutflag);
  if (AnaCut == "Higgs") return CheckHiggsCut(cutflag);

  //return CheckPhenoCut(cutflag);
  return true;
}       // -----  end of function DelCut::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckHiggsCut
//  Description:  HIG-13-013
// ===========================================================================
bool DelCut::CheckHiggsCut(std::bitset<20> cutflag)
{

//----------------------------------------------------------------------------
//  Trigger, just 2 jet trigger?
//----------------------------------------------------------------------------
  if (cutflag.test(0)) 
  {
    if (Ana->vJet->size() < 2) return false;
  }

//----------------------------------------------------------------------------
//  MET Filter
//----------------------------------------------------------------------------
  if (cutflag.test(1)) 
  {
    if (!Ana->METMHTAsys())  return false;
  }

//----------------------------------------------------------------------------
//  Electron Veto
//----------------------------------------------------------------------------
  if (cutflag.test(2)) 
  {
    if (Ana->vElectron->size() > 0) return false;
  }

//----------------------------------------------------------------------------
//  Muon Veto
//----------------------------------------------------------------------------
  if (cutflag.test(3)) 
  {
    if (Ana->vMuon->size() > 0) return false;
  }

//----------------------------------------------------------------------------
//  J1 J2 Pt 
//----------------------------------------------------------------------------
  if (cutflag.test(4)) 
  {
    if ( Ana->J1 == 0 || Ana->J1->PT < 50 || std::fabs(Ana->J1->Eta) > 4.7) return false;
    if ( Ana->J2 == 0 || Ana->J2->PT < 50 || std::fabs(Ana->J2->Eta) > 4.7) return false;
  }

//----------------------------------------------------------------------------
//  J1J2 Eta
//----------------------------------------------------------------------------
  if (cutflag.test(5)) 
  {
    if (Ana->J1->Eta * Ana->J2->Eta > 0) return false;      
  }

//----------------------------------------------------------------------------
//  J1 J2 Delta Eta
//----------------------------------------------------------------------------
  if (cutflag.test(6)) 
  {
    if ( std::fabs(Ana->J1->Eta - Ana->J2->Eta ) < 4.2) return false;      
  }

//----------------------------------------------------------------------------
//  MET 130
//----------------------------------------------------------------------------
  if (cutflag.test(7)) 
  {
    if (Ana->Met < 130) return false;
  }

//----------------------------------------------------------------------------
//  MJJ 
//----------------------------------------------------------------------------

  if (cutflag.test(8)) 
  {
    if ( Ana->Mjj<1100. ) return false;
  }


//----------------------------------------------------------------------------
//  Central Jet Veto
//----------------------------------------------------------------------------

  if (cutflag.test(9)) 
  {

    if (Ana->J3 != 0)
    {
      
      // Jet3 is within jet1 and jet2 
      if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
         (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
      {
        // Only reject hard jets 
        if (Ana->J3->PT > 30) return false;
      }

    }
  }

//----------------------------------------------------------------------------
//  Delta Phi 
//----------------------------------------------------------------------------
  if (cutflag.test(10)) 
  {
    double deltaphi = Ana->J1->P4().DeltaPhi(Ana->J2->P4());
    if (  std::fabs(deltaphi) > 1.0  ) return false;
  }


//---------------------------------------------------------------------------
//  Tau Veto
//----------------------------------------------------------------------------
  if (cutflag.test(11)) 
  {
    // tau veto
    for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); it++)
    {
      if (it->TauTag) return false;
    }
  }

//---------------------------------------------------------------------------
//  Tau Veto
//----------------------------------------------------------------------------
  if (cutflag.test(12)) 
  {
    // tau veto
    for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); it++)
    {
      if (it->BTag) return false;
    }
  }

  return true;
}       // -----  end of function DelCut::CheckHiggsCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckDMCut
//  Description:  Cut for Susy VBF DM
// ===========================================================================
bool DelCut::CheckDMCut(std::bitset<20> cutflag)
{
 
//----------------------------------------------------------------------------
//  VBF Cut
//----------------------------------------------------------------------------
  if (cutflag.test(0)) 
  {
    if (!Ana->METMHTAsys())  return false;
  }

  if (cutflag.test(1)) 
  {
    if (Ana->vJet->size() < 2) return false;
  }


  if (cutflag.test(2)) 
  {
    if (Ana->Met < 50) return false;
  }

  if (cutflag.test(3)) 
    if (Ana->J1->PT < 30 || std::fabs(Ana->J1->Eta) > 5) return false;

  if (cutflag.test(4)) 
    if (Ana->J2->PT < 30 || std::fabs(Ana->J2->Eta) > 5) return false;

    //
    // Opposive eta and eta separation
    //
  if (cutflag.test(5)) 
    if (Ana->J1->Eta * Ana->J2->Eta > 0) return false;      

  if (cutflag.test(6)) 
    if ( fabs(Ana->J1->Eta - Ana->J2->Eta ) < 4.2) return false;      

//----------------------------------------------------------------------------
//  Leading jet cut
//----------------------------------------------------------------------------
  if (cutflag.test(7)) 
  {
    if (Ana->PileUp == "140PileUp")
    {
      if ( Ana->J1 == 0 || Ana->J1->PT < 200) return false; //For 140PU 
    }
    else
    {
      if ( Ana->J1 == 0 || Ana->J1->PT < 50) return false;
    }
  }

  if (cutflag.test(8)) 
  {
    if (Ana->PileUp == "140PileUp")
    {
      if (Ana->J2 == 0 || Ana->J2->PT < 100) return false; //For 140PU 
    }
    else
    {
      if (Ana->J2 == 0 || Ana->J2->PT < 50) return false;
    }
  }

  if (cutflag.test(9)) 
  {
    //if ( Ana->Mjj<2000. ) return false; //For harder cuts 
    if ( Ana->Mjj<1500. ) return false;
  }

  if (cutflag.test(10)) 
  {
    if (Ana->J3 != 0)
    {
      // Jet3 is within jet1 and jet2 
      if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
         (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
      {
        // Only reject hard jets 
        if (Ana->J3->PT > 30) return false;
      }

    }
  }



  if (cutflag.test(11)) 
  {
    // b veto
    for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); it++)
    {
      if (it->BTag) return false;
    }
  }


  if (cutflag.test(12)) 
  {
    // b veto
    for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); it++)
    {
      if (it->TauTag) return false;
    }

    if (ProName.find("Sys") == std::string::npos)
    {
      if (Ana->vElectron->size() > 0) return false;
      if (Ana->vMuon->size() > 0) return false;
    }
    else 
        if (CheckSysLep() == false) return false;
  }

  if (cutflag.test(13)) 
  {
    //if (Ana->Met < 300) return false; // for harder MHT
    if (Ana->Met < 200) return false;
  }

//----------------------------------------------------------------------------
//  Delta Phi 
//----------------------------------------------------------------------------
  if (cutflag.test(14)) 
  {
    double deltaphi = Ana->J1->P4().DeltaPhi(Ana->J2->P4());
    if (  std::fabs(deltaphi) > 1.8  ) return false;
  }

  return true;
}       // -----  end of function DelCut::CheckDMCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckPhenoCut
//  Description:  
// ===========================================================================
bool DelCut::CheckPhenoCut(std::bitset<20> cutflag)
{

//----------------------------------------------------------------------------
//  Leading jet cut
//----------------------------------------------------------------------------
  if (cutflag.test(0)) 
  {
    if ( Ana->J1 == 0 || Ana->J1->PT < 50) return false;
  }

  if (cutflag.test(1)) 
  {
    if (Ana->J2 == 0 || Ana->J2->PT < 50) return false;
  }

  if (cutflag.test(2)) 
  {
    if ( Ana->Mjj<1500. ) return false;
  }

  if (cutflag.test(3)) 
  {

    if (Ana->J3 != 0)
    {
      
      // Jet3 is within jet1 and jet2 
      if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
         (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
      {
        // Only reject hard jets 
        if (Ana->J3->PT > 30) return false;
      }

    }

    // b veto
    for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); it++)
    {
      if (it->BTag) return false;
      if (it->TauTag) return false;
    }

    if (Ana->vElectron->size() > 0) return false;
    if (Ana->vMuon->size() > 0) return false;

  }


  if (cutflag.test(4)) 
  {
    if (Ana->Met < 200) return false;
  }
  return true;

}       // -----  end of function DelCut::CheckPhenoCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::DrawHistogram
//  Description:  
// ===========================================================================
int DelCut::DrawHistogram()
{
  His->DrawTH1();
  His->DrawTPro();
  His->DrawTH2();
  return 1;
}       // -----  end of function DelCut::DrawHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::WriteHistogram
//  Description:  
// ===========================================================================
int DelCut::WriteHistogram()
{
  His->WriteTH1();
  His->WriteTPro();
  His->WriteTH2();
  return 1;
}       // -----  end of function DelCut::WriteHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillJets
//  Description:  Filling jets relative hist
// ===========================================================================
int DelCut::FillJets() const
{

  for (int i = 0; i < Ana->vJet->size(); ++i)
  {
    His->FillTH1("JetEta", Ana->vJet->at(i).Eta);
  }
  return 1;
}       // -----  end of function DelCut::FillJets  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillJets
//  Description:  
// ===========================================================================
int DelCut::FillJets(int NCut)
{
  int jentries = Ana->vJet->size();
  His->FillTH1(NCut, "NJet", jentries);
  if(jentries <= 0) return 0;

  for (int i = 0; i < Ana->vJet->size(); ++i)
  {
    if (std::fabs(Ana->vJet->at(i).Eta) <= 2)
    {
      His->FillTH1(NCut, "JCPt", Ana->vJet->at(i).PT);
    } else His->FillTH1(NCut, "JFPt", Ana->vJet->at(i).PT);
  }

  His->FillTH1(NCut, "J1Pt", Ana->J1->PT);
  His->FillTH1(NCut, "J1Eta", Ana->J1->Eta);
  His->FillTH1(NCut, "J1Phi", Ana->J1->Phi);
  
  His->FillTH1(NCut, "dPtMHTJ1", Ana->MHT->Pt() - Ana->J1->PT);
  His->FillTH1(NCut, "dPhiMHTJ1", Ana->J1->P4().DeltaPhi(*Ana->MHT));
  His->FillTH1(NCut, "dEtaMHTJ1", Ana->J1->Eta - Ana->MHT->Eta());
  His->FillTH1(NCut, "dRMHTJ1", Ana->J1->P4().DeltaR(*Ana->MHT));
  if (Ana->J2 != 0)
  {
    His->FillTH1(NCut, "J2Pt", Ana->J2->PT);
    His->FillTH1(NCut, "J2Eta", Ana->J2->Eta);
    His->FillTH1(NCut, "J2Phi", Ana->J2->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

    His->FillTH1(NCut, "MJJ", Ana->Mjj);
    His->FillTH1(NCut, "dPtJJ", Ana->J1->PT-Ana->J2->PT);
    His->FillTH1(NCut, "dPhiJJ", Ana->J1->P4().DeltaPhi(Ana->J2->P4()));
    His->FillTH1(NCut, "ABSdPhiJJ", std::fabs(Ana->J1->P4().DeltaPhi(Ana->J2->P4())));
    His->FillTH1(NCut, "dEtaJJ", Ana->J1->Eta - Ana->J2->Eta);
    His->FillTH1(NCut, "dRJJ", Ana->J1->P4().DeltaR(Ana->J2->P4()));
    His->FillTH2(NCut, "MJJMHT", Ana->Mjj, Ana->Met);

    His->FillTH1(NCut, "dPtMHTJ2", Ana->MHT->Pt() - Ana->J2->PT);
    His->FillTH1(NCut, "dPhiMHTJ2", Ana->J2->P4().DeltaPhi(*Ana->MHT));
    His->FillTH1(NCut, "dEtaMHTJ2", Ana->J2->Eta - Ana->MHT->Eta());
    His->FillTH1(NCut, "dRMHTJ2", Ana->J2->P4().DeltaR(*Ana->MHT));
  }

  
  if (Ana->J3 != 0)
  {
    His->FillTH1(NCut, "J3Pt", Ana->J3->PT);
    His->FillTH1(NCut, "J3Eta", Ana->J3->Eta);
    His->FillTH1(NCut, "J3Phi", Ana->J3->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~
    His->FillTH1(NCut, "dPtMHTJ3", Ana->MHT->Pt() - Ana->J3->PT);
    His->FillTH1(NCut, "dPhiMHTJ3", Ana->J3->P4().DeltaPhi(*Ana->MHT));
    His->FillTH1(NCut, "dEtaMHTJ3", Ana->J3->Eta - Ana->MHT->Eta());
    His->FillTH1(NCut, "dRMHTJ3", Ana->J3->P4().DeltaR(*Ana->MHT));
  }

//----------------------------------------------------------------------------
//  Fill in the jet eff. 
//----------------------------------------------------------------------------
   for (int i = 0; i < Ana->DEV->vGenJet.size(); ++i)
   {
     His->FillTH1(NCut, "GenJetPt", Ana->DEV->vGenJet.at(i).PT);
     His->FillTH1(NCut, "GenJetEta", Ana->DEV->vGenJet.at(i).Eta);
     His->FillTH2(NCut, "GenJet", Ana->DEV->vGenJet.at(i).Eta, Ana->DEV->vGenJet.at(i).PT);
   }

   for (int i = 0; i < Ana->MatchedJet.size(); ++i)
   {
     His->FillTH1(NCut, "JetPt", Ana->MatchedJet.at(i).Pt());
     His->FillTH1(NCut, "JetEta", Ana->MatchedJet.at(i).Eta());
     His->FillTH2(NCut, "RecoJet", Ana->MatchedJet.at(i).Eta(), Ana->MatchedJet.at(i).Pt());
   }

   for (int i = 0; i < Ana->JetPtScale.size(); ++i)
   {
     His->FillTH1(NCut, "JetPtScale", Ana->JetPtScale.at(i));
   }

   return 1;
    
}       // -----  end of function DelCut::FillJets  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillEle
//  Description:  
// ===========================================================================
int DelCut::FillEle(int NCut)
{


  return 1;
}       // -----  end of function DelCut::FillEle  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillMet
//  Description:  
// ===========================================================================
int DelCut::FillMet(int NCut)
{
  His->FillTH1(NCut, "MHT", Ana->Met);
  His->FillTH1(NCut, "MET", Ana->RawMet.Mod());
  His->FillTH1(NCut, "MHTMET", Ana->Met - Ana->RawMet.Mod());
  His->FillTH1(NCut, "MetAsys", Ana->METAsys);
  //His->FillTH2(NCut, "MetMHT", Ana->Met, Ana->RawMet.Mod());
  return 1;
}       // -----  end of function DelCut::FillEle  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillSampleXS
//  Description:  Save the cross section in bin 1 in histogram XS
// ===========================================================================
bool DelCut::FillSampleXS(double xs, double error)
{
  His->AddTH1("CrossSection", "Cross Section", 2, 0, 2);
  His->FillTH1("CrossSection", 1, xs);
  His->FillTH1("CrossSection", 0, error);
}       // -----  end of function DelCut::FillSampleXS  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCut::BookMetPerf
//  Description:  Booking all the histogram for the MET performance study
// ===========================================================================
int DelCut::BookMetPerf() const
{
  // Met study
  His->AddTH1("MLL", "M_{ll}", "M_{ll} [GeV]", "Events / 1 GeV", 60, 60, 120, 0, 1);
  His->AddTH1("QT", "Qt", "q_{T} [GeV]", "Events / 8 GeV",  50, 0, 400, 0, 1);
  His->AddTH1("UT", "Ut", "u_{T} [GeV]", "Events / 8 GeV",  50, 0, 400, 0, 1);
  His->AddTH1("UPQT", "UPQT", "u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal", "UParal", "u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp", "UPerp", "u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);


  //Filling for separating Z transverse momentum impulsion
  His->AddTH1("UPQT_0", "UPQT_0", "(0 < q_{T} < 40 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_0", "UParal_0", "(0 < q_{T} < 40 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_0", "UPerp_0", "(0 < q_{T} < 40 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_1", "UPQT_1", "(40 < q_{T} < 60 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_1", "UParal_1", "(40 < q_{T} < 60 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_1", "UPerp_1", "(40 < q_{T} < 60 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_2", "UPQT_2", "(60 < q_{T} < 80 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_2", "UParal_2", "(60 < q_{T} < 80 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_2", "UPerp_2", "(60 < q_{T} < 80 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_3", "UPQT_3", "(80 < q_{T} < 100 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_3", "UParal_3", "(80 < q_{T} < 100 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_3", "UPerp_3", "(80 < q_{T} < 100 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_4", "UPQT_4", "(100 < q_{T} < 120 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_4", "UParal_4", "(100 < q_{T} < 120 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_4", "UPerp_4", "(100 < q_{T} < 120 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_5", "UPQT_5", "(120 < q_{T} < 140 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_5", "UParal_5", "(120 < q_{T} < 140 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_5", "UPerp_5", "(120 < q_{T} < 140 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_6", "UPQT_6", "(140 < q_{T} < 160 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_6", "UParal_6", "(140 < q_{T} < 160 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_6", "UPerp_6", "(140 < q_{T} < 160 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_7", "UPQT_7", "(160 < q_{T} < 200 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_7", "UParal_7", "(160 < q_{T} < 200 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_7", "UPerp_7", "(160 < q_{T} < 200 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_8", "UPQT_8", "(200 < q_{T} < 240 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_8", "UParal_8", "(200 < q_{T} < 240 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_8", "UPerp_8", "(200 < q_{T} < 240 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_9", "UPQT_9", "(240 < q_{T} < 300 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_9", "UParal_9", "(240 < q_{T} < 300 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_9", "UPerp_9", "(240 < q_{T} < 300 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPQT_10", "UPQT_10", "(300 < q_{T} < 400 GeV) u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UParal_10", "UParal_10", "(300 < q_{T} < 400 GeV) u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  His->AddTH1("UPerp_10", "UPerp_10", "(300 < q_{T} < 400 GeV) u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);

  His->AddTH1("RMetX", "Raw MetX", "Raw #slash{E}_{x} [GeV]", "Events / 8 GeV",  50, -200, 200, 0, 1);
  His->AddTH1("RMetY", "Raw MetY", "Raw #slash{E}_{y} [GeV]", "Events / 8 GeV",  50, -200, 200, 0, 1);
  His->AddTH1("MetX", "MetX", "#slash{E}_{x} [GeV]", "Events / 8 GeV",  50, -200, 200, 0, 1);
  His->AddTH1("MetY", "MetY", "#slash{E}_{y} [GeV]", "Events / 8 GeV",  50, -200, 200, 0, 1);
  His->AddTPro("MetScale", "MetScale", "Z/#gamma q_{T} [GeV]", "-<u_{#parallel}>/q_{T}",  500, 0, 1000);
  double xbin[12] = {0, 40 , 60, 80, 100, 120, 140, 160, 200, 240, 300, 400};
  TProfile* pro1 = new TProfile( "MetResP", "MetResP;Z/#gamma q_{T} [GeV];#sigma(u_{#parallel}) [GeV]",  11, xbin, "S");
  His->AddTPro(pro1);
  TProfile* pro2 = new TProfile( "MetResT", "MetResT;Z/#gamma q_{T} [GeV];#sigma(u_{#perp}) [GeV]",  11, xbin, "S");
  His->AddTPro(pro2);
  His->AddTPro("MetResX", "MetResX", "HT [GeV]", "#sigma(#slash{E}_{x}) [GeV]",  50, 0, 400);
  His->AddTPro("MetResY", "MetResY", "HT [GeV]", "#sigma(#slash{E}_{y}) [GeV]",  50, 0, 400);
  His->AddTPro("MetXMean", "MetXMean", "HT [GeV]", "<#slash{E}_{x}> [GeV]",  50, 0, 400);
  His->AddTPro("MetYMean", "MetYMean", "HT [GeV]", "<#slash{E}_{y}> [GeV]",  50, 0, 400);
  His->AddTPro("RMetResX", "RMetResX", "HT [GeV]", "#sigma(Raw #slash{E}_{x}) [GeV]",  50, 0, 400);
  His->AddTPro("RMetResY", "RMetResY", "HT [GeV]", "#sigma(Raw #slash{E}_{y}) [GeV]",  50, 0, 400);
  His->AddTPro("RMetXMean", "RMetXMean", "HT [GeV]", "<Raw #slash{E}_{x}> [GeV]",  50, 0, 400);
  His->AddTPro("RMetYMean", "RMetYMean", "HT [GeV]", "<Raw #slash{E}_{y}> [GeV]",  50, 0, 400);

  His->AddTH1("UtNJets", "Num. of Jets in UT Cal", 20, 0, 20 );
  His->AddTH1("UtNPhotons", "Num. of photon in UT Cal", 20, 0, 20 );
  His->AddTH1("UTJetsPT", "Sum PT of Jets in UT Cal", 200, 0, 400);
  His->AddTH1("UTPhotonsPT", "Sum PT of Photons in UT Cal", 200, 0, 400);
  return 1;
}       // -----  end of function DelCut::BookMetPerf  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillMetPerf
//  Description:  filling all the histogram for the MET performance study
// ===========================================================================
int DelCut::FillMetPerf() const
{
  // Met Study
  Ana->MetDiLepton();
  His->FillTH1("MLL", Ana->Mll);
  His->FillTH1("UT", Ana->UT);
  His->FillTH1("QT", Ana->QT);
  His->FillTH1("UPQT", Ana->UParallel + Ana->QT);
  His->FillTH1("UPerp", Ana->UTransverse);
  His->FillTH1("UParal", Ana->UParallel);
  His->FillTH1("MetX", Ana->PUCorMet->Px());
  His->FillTH1("MetY", Ana->PUCorMet->Py());
  His->FillTH1("RMetX", Ana->RawMet.Px());
  His->FillTH1("RMetY", Ana->RawMet.Py());
  His->FillTPro("MetScale", Ana->QT, Ana->MetScale);
  His->FillTPro("MetResP", Ana->QT, Ana->UParallel);
  His->FillTPro("MetResT", Ana->QT, Ana->UTransverse);
  His->FillTPro("MetResX", *Ana->HT, Ana->PUCorMet->Px());
  His->FillTPro("MetResY", *Ana->HT, Ana->PUCorMet->Py());
  His->FillTPro("RMetResX", Ana->RHT, Ana->RawMet.Px());
  His->FillTPro("RMetResY", Ana->RHT, Ana->RawMet.Py());
  His->FillTPro("MetXMean", *Ana->HT, Ana->PUCorMet->Px());
  His->FillTPro("MetYMean", *Ana->HT, Ana->PUCorMet->Py());
  His->FillTPro("RMetXMean", Ana->RHT, Ana->RawMet.Px());
  His->FillTPro("RMetYMean", Ana->RHT, Ana->RawMet.Py());

  His->FillTH1("UtNJets", Ana->UtNJets);
  His->FillTH1("UtNPhotons", Ana->UtNPhotons);
  His->FillTH1("UTJetsPT", Ana->UTJetsPT);
  His->FillTH1("UTPhotonsPT", Ana->UTPhotonsPT);



  // Filling the slicing
  double xbin[12] = {0, 40 , 60, 80, 100, 120, 140, 160, 200, 240, 300, 400};
  for (int i = 0; i < 11; ++i)
  {
    int low = xbin[i];
    int high = xbin[i+1];
    
    if (Ana->QT >= low && Ana->QT < high)
    {
      char temp[50];
      sprintf(temp, "UPQT_%d", i);
      His->FillTH1(temp, Ana->UParallel + Ana->QT);
      sprintf(temp, "UPerp_%d", i);
      His->FillTH1(temp, Ana->UTransverse);
      sprintf(temp, "UParal_%d", i);
      His->FillTH1(temp, Ana->UParallel);
      break;
    }
  }
  return 1;
}       // -----  end of function DelCut::FillMetPerf  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillNEVT
//  Description:  Pulic function for filling in the NEVT, it should be same
//  among DelCuts
// ===========================================================================
bool DelCut::FillNEVT(double weight) const
{
  His->FillTH1("NEVT", 1, weight); //the NEVT with weight 
}       // -----  end of function DelCut::FillNEVT  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckSysLep
//  Description:  For systematic study
// ===========================================================================
bool DelCut::CheckSysLep() const
{
  assert(ProName.find("Sys") != std::string::npos);

  int nele = 0;
  int nmuon = 0;


  if (ProName.find("SysWev") != std::string::npos)
  {
    nele = 1; 
    nmuon = 0;
  }
  if (ProName.find("SysWmv") != std::string::npos)
  {
    nele = 0; 
    nmuon = 1;
  }
  if (ProName.find("SysZee") != std::string::npos
      || ProName.find("SysEE") != std::string::npos)
  {
    nele = 2; 
    nmuon = 0;
  }
  if (ProName.find("SysZmm") != std::string::npos
      || ProName.find("SysMM") != std::string::npos)
  {
    nele = 0; 
    nmuon = 2;
  }

  return Ana->vElectron->size() == nele  && Ana->vMuon->size() == nmuon ;
}       // -----  end of function DelCut::CheckSysLep  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::SysMet
//  Description:  
// ===========================================================================
double DelCut::SysMet() const
{
  TLorentzVector temp = *(Ana->MHT);

  for (int i = 0; i < Ana->vElectron->size(); ++i)
  {
    temp -= Ana->vElectron->at(i).P4();
  }

  for (int i = 0; i < Ana->vMuon->size(); ++i)
  {
    temp -= Ana->vMuon->at(i).P4();
  }
  
  double met_x = -temp.Px();
  double met_y = -temp.Py();

  TVector2 NewMet(met_x, met_y);

  return NewMet.Mod();
}       // -----  end of function DelCut::SysMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::BookLeptonEff
//  Description:  
// ===========================================================================
bool DelCut::BookLeptonEff()
{
  His->AddTH1C("GenElePt", "GenElePt", "Pt_{Gen e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("GenMuonPt", "GenMuonPt", "Pt_{Gen m} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("GenTauPt", "GenTauPt", "Pt_{Gen t} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("ElePt", "ElePt", "Pt_{e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("MuonPt", "MuonPt", "Pt_{m} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("TauPt", "TauPt", "Pt_{t} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );

  His->AddTH1C("GenEleEta", "GenEleEta", "#eta_{Gen e}", "Events",  50, -5, 5 );
  His->AddTH1C("GenMuonEta", "GenMuonEta", "#eta_{Gen m}","Events",   50, -5, 5 );
  His->AddTH1C("GenTauEta", "GenTauEta", "#eta_{Gen t}","Events",   50, -5, 5 );
  His->AddTH1C("EleEta", "EleEta", "#eta_{e}", "Events",  50, -5, 5 );
  His->AddTH1C("MuonEta", "MuonEta", "#eta_{m}", "Events",  50, -5, 5 );
  His->AddTH1C("TauEta", "TauEta", "#eta_{t}", "Events",  50, -5, 5 );

  His->AddTH2C("GenEle", "Gen Electron", "#eta_{Gen e}", "Pt_{Gen e}", 50, -5, -5, 200, 0, 800);
  His->AddTH2C("GenMuon", "Gen Muon", "#eta_{Gen m}", "Pt_{Gen m}", 50, -5, -5, 200, 0, 800);
  His->AddTH2C("GenTau", "Gen Tau", "#eta_{Gen t}", "Pt_{Gen t}", 50, -5, -5, 200, 0, 800);
  His->AddTH2C("RecoEle", "Reco Electron", "#eta_{e}", "Pt_{e}", 50, -5, -5, 200, 0, 800);
  His->AddTH2C("RecoMuon", "Reco Muon", "#eta_{m}", "Pt_{m}", 50, -5, -5, 200, 0, 800);
  His->AddTH2C("RecoTau", "Reco Tau", "#eta_{t}", "Pt_{t}", 50, -5, -5, 200, 0, 800);

  return true;
}       // -----  end of function DelCut::BookLeptonEff  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::BookJetEff
//  Description:  
// ===========================================================================
bool DelCut::BookJetEff()
{
  His->AddTH1C("GenJetPt", "GenJetPt", "Pt_{Gen Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("GenJetEta", "GenJetEta", "#eta_{Gen Jet}", "Events",  50, -5, 5 );
  His->AddTH1C("JetPt", "JetPt", "Pt_{Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("JetEta", "JetEta", "#eta_{Jet}", "Events", 50, -5, 5 );
  His->AddTH1C("JetPTScale", "JetPTScale", "Reco Jet / Gen Jet", "Events", 40, -2, 2 );
  His->AddTH2C("GenJet", "Gen Jet", "#eta_{Gen Jet}", "Pt_{Gen Jet}", 50, -5, -5, 200, 0, 1000);
  His->AddTH2C("RecoJet", "Reco Jet", "#eta_{Jet}", "Pt_{Jet}", 50, -5, -5, 200, 0, 1000);
  return true;
}       // -----  end of function DelCut::BookJetEff  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillLepton
//  Description:  
// ===========================================================================
int DelCut::FillLepton(int NCut) const
{
//----------------------------------------------------------------------------
//  Fill in Gen Lepton
//----------------------------------------------------------------------------
  int GenSize =Ana->vGenParticle->size(); 
  for (int i = 0; i < GenSize; ++i)
  {
    GenParticle p = Ana->vGenParticle->at(i);
    if (p.Status != 3 || p.M1 > GenSize || p.M2 > GenSize )  continue;
    if (std::fabs(p.PID) == 11) //Electron 
    {
      His->FillTH1(NCut, "GenElePt", p.PT);
      His->FillTH1(NCut, "GenEleEta", p.Eta);
      His->FillTH2(NCut, "GenEle", p.Eta, p.PT);
    }
    if (std::fabs(p.PID) == 13) //Muon
    {
      His->FillTH1(NCut, "GenMuonPt", p.PT);
      His->FillTH1(NCut, "GenMuonEta", p.Eta);
      His->FillTH2(NCut, "GenMuon", p.Eta, p.PT);
    }
    if (std::fabs(p.PID) == 15) //Muon
    {
      His->FillTH1(NCut, "GenTauPt", p.PT);
      His->FillTH1(NCut, "GenTauEta", p.Eta);
      His->FillTH2(NCut, "GenTau", p.Eta, p.PT);
    }
  }

//----------------------------------------------------------------------------
//  Fill in Reco Leptons
//----------------------------------------------------------------------------
  for (int i = 0; i < Ana->vElectron->size(); ++i)
  {
    His->FillTH1(NCut, "ElePt", Ana->vElectron->at(i).PT);
    His->FillTH1(NCut, "EleEta", Ana->vElectron->at(i).Eta);
    His->FillTH2(NCut, "RecoEle", Ana->vElectron->at(i).Eta, Ana->vElectron->at(i).PT);
  }

  for (int i = 0; i < Ana->vMuon->size(); ++i)
  {
    His->FillTH1(NCut, "MuonPt", Ana->vMuon->at(i).PT);
    His->FillTH1(NCut, "MuonEta", Ana->vMuon->at(i).Eta);
    His->FillTH2(NCut, "RecoMuon", Ana->vMuon->at(i).Eta, Ana->vMuon->at(i).PT);
  }

  for (int i = 0; i < Ana->vJet->size(); ++i)
  {
    Jet j = Ana->vJet->at(i);
    if (j.TauTag)
    {
      His->FillTH1(NCut, "TauPt", j.PT);
      His->FillTH1(NCut, "TauEta", j.Eta);
      His->FillTH2(NCut, "RecoTau", j.Eta, j.PT);
    }
  }
  return true;
}       // -----  end of function DelCut::FillLepton  -----
