// ===========================================================================
// 
//       Filename:  DelCutDM.cc
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


#include "DelCutDM.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCutDM
//      Method:  DelCutDM
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelCutDM::DelCutDM(DelAna *ana, std::shared_ptr<TFile> OutFile, 
    std::string name, std::string cut_ )
: DelCut(ana,  OutFile, name, cut_ ), Ana(ana), ProName(name), AnaCut(cut_)
{
}  // ~~~~~  end of method DelCutDM::DelCutDM  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCutDM
//      Method:  DelCutDM
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelCutDM::DelCutDM ( const DelCutDM &other ) : DelCut(other)
{
}  // ~~~~~  end of method DelCutDM::DelCutDM  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCutDM
//      Method:  ~DelCutDM
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelCutDM::~DelCutDM ()
{
  //:TRICKY:06/09/2014 12:45:08 PM:benwu: Don't delete the HistTool, which
  //will course the code to crash. I am guessing it is related to how root
  //writing the output file
  
  //delete His;
}  // ~~~~~  end of method DelCutDM::~DelCutDM  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelCutDM
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelCutDM&
DelCutDM::operator = ( const DelCutDM &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelCutDM::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::InitCutOrder
//  Description:  
// ===========================================================================
bool DelCutDM::InitCutOrder(std::string ana)
{
  AnaCut = ana;

  //Clear the objects
  CutOrder.clear();
  CutMap.clear();


  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("VBFCut");
  CutOrder.push_back("J1J2");
  CutOrder.push_back("CTMjj");
  CutOrder.push_back("CTJ3");
  CutOrder.push_back("CTBJ");
  CutOrder.push_back("CTLep");
  CutOrder.push_back("CTMet200");
  CutOrder.push_back("CTdPhi");
  CutOrder.push_back("ALL");

  //Set the cutbit of each cut
  CutMap["NoCut"]     = "00000000000000000";
  CutMap["VBFCut"]    = "00000000001111111";
  CutMap["J1J2"]      = "00000000111111111";
  CutMap["CTMjj"]     = "00000001111111111";
  CutMap["CTJ3"]      = "00000011111111111";
  CutMap["CTBJ"]      = "00000111111111111";
  CutMap["CTLep"]     = "00001111111111111";
  CutMap["CTMet200"]  = "00011111111111111";
  CutMap["CTdPhi"]    = "00111111111111111";
  CutMap["ALL"]       = "00111111111111111";

  assert(CutOrder.size() == CutMap.size());
  His->Cutorder(ana, CutOrder);

  return true;
}       // -----  end of function DelCutDM::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::CheckCut
//  Description:  Define the cutbit per event
// ===========================================================================
bool DelCutDM::CheckCut()
{
  cutbit.reset();

//----------------------------------------------------------------------------
//  VBF Cut
//----------------------------------------------------------------------------
  cutbit.set(0, Ana->METMHTAsys());
  cutbit.set(1, Ana->vJet->size() >= 2);
  cutbit.set(2, Ana->Met >= 50);

  if (Ana->J1 != 0)
    cutbit.set(3, Ana->J1->PT >= 30 && std::fabs(Ana->J1->Eta) <= 5);
  if (Ana->J2 != 0)
    cutbit.set(4, Ana->J2->PT >= 30 && std::fabs(Ana->J2->Eta) <= 5);

  //
  // Opposive eta and eta separation
  //
  if (Ana->J1 != 0 && Ana->J2 != 0)
    cutbit.set(5, Ana->J1->Eta * Ana->J2->Eta < 0);
  if (Ana->J1 != 0 && Ana->J2 != 0)
    cutbit.set(6, fabs(Ana->J1->Eta - Ana->J2->Eta ) >= 4.2);

//----------------------------------------------------------------------------
//  Leading jet cut
//----------------------------------------------------------------------------
  if (Ana->PileUp == "140PileUp")
    cutbit.set(7,  Ana->J1 != 0 && Ana->J1->PT >= 200);
  else
    cutbit.set(7,  Ana->J1 != 0 && Ana->J1->PT >= 50);

  if (Ana->PileUp == "140PileUp")
    cutbit.set(8,  Ana->J2 != 0 && Ana->J2->PT >= 100);
  else
    cutbit.set(8,  Ana->J2 != 0 && Ana->J2->PT >= 50);

  cutbit.set(9,  Ana->Mjj >= 1500. );


  // By default, passed central jet veto
  if (Ana->J1 != 0 && Ana->J2 != 0 )
  {
    cutbit.set(10,  true);
    if (Ana->J3 != 0)
    {

      // Jet3 is within jet1 and jet2 
      if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
          (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
      {
        cutbit.set(10,  Ana->J3->PT < 30);
      }
    }
  }

  bool hasB = false;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->BTag) hasB = true;
    break;
  }
  cutbit.set(11, !hasB);


  bool hasTau = false;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->TauTag) hasTau = true;
    break;
  }

  if (ProName.find("Sys") == std::string::npos)
    cutbit.set(12, !hasTau && Ana->vElectron->size() == 0 &&  Ana->vMuon->size() == 0);
  else 
    if (CheckSysLep() == false) return false;

  cutbit.set(13, Ana->Met >= 200);

//----------------------------------------------------------------------------
//  Delta Phi 
//----------------------------------------------------------------------------
  if (Ana->J1 != 0 && Ana->J2 != 0)
  {
    double deltaphi = Ana->J1->P4().DeltaPhi(Ana->J2->P4());
    cutbit.set(14, std::fabs(deltaphi) <= 1.8 );
  }

  return true;
}       // -----  end of function DelCutDM::CheckCut  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillCut
//  Description:  Main function to be called in DelFill
// ===========================================================================
int DelCutDM::FillCut()
{
//----------------------------------------------------------------------------
//  Set up the DelAna
//----------------------------------------------------------------------------
  His->FillTH1("Weight", Ana->Weight);
  His->FillTH1("NEle", (int)Ana->vElectron->size());
  His->FillTH1("NMuon", (int)Ana->vMuon->size());
  His->FillTH1("NPhoton", (int)Ana->vPhoton->size());
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
   FillLepton();
   His->FillTH1("ZMass", Ana->GenMet.M());

//----------------------------------------------------------------------------
//  Filling histogram for MET performance study
//----------------------------------------------------------------------------
   if (ProName.find("MetDiEle") != std::string::npos
       || ProName.find("MetDiMuon") != std::string::npos)
    FillMetPerf();

//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();
  for (int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    His->FillTH1("CutFlow", i); 


    // Filling by functions
    FillJets(i);
    FillMet(i);
    FillLepton(i);
    FillSUSYVar(i);
  }

  return 1;
}       // -----  end of function DelCutDM::FillCut  -----



// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::BookHistogram
//  Description:  
// ===========================================================================
bool DelCutDM::BookHistogram()
{
  
  BookLeptonEff();
  BookJetEff();
  BookSUSYVar();
  BookBJet();
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

  His->AddTH1C("NJets", "Num. of Jets", "Number of Jets", "Events", 40, 0, 40 );
  His->AddTH1("NEle", "Num. of Electrons", 10, 0, 10 );
  His->AddTH1("NMuon", "Num. of Muons", 10, 0, 10 );
  His->AddTH1("NPhoton", "Num. of Photons", 10, 0, 10 );

  His->AddTH1("ZMass", "Zmass", "M_{Z} [GeV]", "Events / 8 GeV",  50, 0, 400, 0, 1);
  His->AddTH1C("GenMet", "GenMet", "Gen #slash{E}_{T} [GeV]", "Events / 5 GeV",  200, 0, 1000, 0, 1);

  His->AddTH1("JetEta", "JetEta", "#eta_{All Jets}", "Events", 280, -7, 7 );
  His->AddTH1("JetEta20", "JetEta20", "#eta_{jets} (P_{T}^{jet} > 20 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta30", "JetEta30", "#eta_{jets} (P_{T}^{jet} > 30 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta40", "JetEta40", "#eta_{jets} (P_{T}^{jet} > 40 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta50", "JetEta50", "#eta_{jets} (P_{T}^{jet} > 50 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta70", "JetEta70", "#eta_{jets} (P_{T}^{jet} > 70 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta100", "JetEta100", "#eta_{jets} (P_{T}^{jet} > 100 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta120", "JetEta120", "#eta_{jets} (P_{T}^{jet} > 120 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetEta150", "JetEta150", "#eta_{jets} (P_{T}^{jet} > 150 GeV)", "Events", 280, -7, 7 );
  His->AddTH1("JetPt", "JetPt", "Pt_{All Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1("EleEta", "EleEta", "#eta_{All Eles}", "Events", 280, -7, 7 );
  His->AddTH1("ElePt", "ElePt", "Pt_{All Ele} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1("MuonEta", "MuonEta", "#eta_{All Muons}", "Events", 280, -7, 7 );
  His->AddTH1("MuonPt", "MuonPt", "Pt_{All Muon} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1("TauEta", "TauEta", "#eta_{All Taus}", "Events", 280, -7, 7 );
  His->AddTH1("TauPt", "TauPt", "Pt_{All Tau} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1("PhotonEta", "PhotonEta", "#eta_{All Photons}", "Events", 280, -7, 7 );
  His->AddTH1("PhotonPt", "PhotonPt", "Pt_{All Photon} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
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
      "Events / 5 GeV", 200, 0, 1000, 0, 1);
  His->AddTH1C("MHTMET", "MHT - MET", "#slash{H}_{T} - #slash{E}_{T} [GeV]", 
      "Events / 5 GeV", 600, -1000, 2000, 0, 1);
  His->AddTH1C("MET", "CRawMet", "#slash{E}_{T} [GeV]", 
      "Events / 5 GeV", 200, 0, 1000, 0, 1);
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
  His->AddTH1C("J1Pt",  "J1Pt",  "Pt_{J1} [GeV]", "Events / 4 GeV", 300, 0,  1200.0 );
  His->AddTH1C("J1Eta", "J1Eta", "#eta_{J1}",     "Events",         14,  -7, 7      );
  His->AddTH1C("J1Phi", "J1Phi", "#phi_{J1}",     "Events",         14,  -7, 7);
  His->AddTH1C("J2Pt",  "J2Pt",  "Pt_{J2} [GeV]", "Events / 4 GeV", 200, 0,  800.0  );
  His->AddTH1C("J2Eta", "J2Eta", "#eta_{J2}",     "Events",         14,  -7, 7      );

  His->AddTH1C("J2Phi", "J2Phi", "#phi_{J2}",     "Events",         14,  -7, 7);
  His->AddTH1C("J3Pt",  "J3Pt",  "Pt_{J3} [GeV]", "Events / 4 GeV", 200, 0,  800.0 );
  His->AddTH1C("J3Eta", "J3Eta", "#eta_{J3}",     "Events",         14,  -7, 7     );
  His->AddTH1C("J3Phi", "J3Phi", "#phi_{J3}",     "Events",         14,  -7, 7);

  His->AddTH1C("CentralJet3Pt",  "CentralJet3Pt",  "Pt_{Central Jet3} [GeV]", "Events / 4 GeV", 200, 0,  800.0 );
  His->AddTH1C("CentralJet3Eta", "CentralJet3Eta", "#eta_{Central Jet3}",     "Events",         14,  -7, 7     );
  His->AddTH1C("CentralJet3Phi", "CentralJet3Phi", "#phi_{Central Jet3}",     "Events",         14,  -7, 7);
  His->AddTH1C("JCPt", "JCPt", "Pt_{central jets} [GeV]", "Events / 4 GeV", 300, 0, 1200.0 );
  His->AddTH1C("JFPt", "JFPt", "Pt_{forward jets} [GeV]", "Events / 4 GeV", 300, 0, 1200.0 );

  His->AddTH1C("MJJ",       "MJJ",       "M_{J1, J2}",             "Events", 400, 0,  8000.0 );
  His->AddTH1C("dPtJJ",     "dPtJJ",     "#Delta Pt_{J1, J2}",     "Events", 300,   0,  1200   );
  His->AddTH1C("dPhiJJ",    "dPhiJJ",    "#Delta #phi_{J1, J2}",   "Events", 14,   -7, 7      );
  His->AddTH1C("ABSdPhiJJ", "ABSdPhiJJ", "|#Delta #phi_{J1, J2}|", "Events", 32,   0,  3.2    );
  His->AddTH1C("dEtaJJ",    "dEtaJJ",    "#Delta #eta_{J1, J2}",   "Events", 14,   -7, 7      );
  His->AddTH1C("dRJJ",      "dRJJ",      "#Delta R_{J1, J2}",      "Events", 20,   0,  10.0   );

  His->AddTH1C("dPtMHTJ1",  "dPtMHTJ1",  "#Delta Pt_{#slash{H}_{T}, J1}",   "Events", 300, 0,  1200 );
  His->AddTH1C("dPhiMHTJ1", "dPhiMHTJ1", "#Delta #phi_{#slash{H}_{T}, J1}", "Events", 14, -7, 7    );
  His->AddTH1C("dEtaMHTJ1", "dEtaMHTJ1", "#Delta #eta_{#slash{H}_{T}, J1}", "Events", 14, -7, 7    );
  His->AddTH1C("dRMHTJ1",   "dRMHTJ1",   "#Delta R_{#slash{H}_{T}, J1}",    "Events", 20, 0,  10.0 );

  His->AddTH1C("dPtMHTJ2",  "dPtMHTJ2",  "#Delta Pt_{#slash{H}_{T}, J2}",   "Events", 300, 0,  1200 );
  His->AddTH1C("dPhiMHTJ2", "dPhiMHTJ2", "#Delta #phi_{#slash{H}_{T}, J2}", "Events", 14, -7, 7    );
  His->AddTH1C("dEtaMHTJ2", "dEtaMHTJ2", "#Delta #eta_{#slash{H}_{T}, J2}", "Events", 14, -7, 7    );
  His->AddTH1C("dRMHTJ2",   "dRMHTJ2",   "#Delta R_{#slash{H}_{T}, J2}",    "Events", 20, 0,  10.0 );

  His->AddTH1C("dPtMHTJ3",  "dPtMHTJ3",  "#Delta Pt_{#slash{H}_{T}, J3}",   "Events", 300, 0,  1200 );
  His->AddTH1C("dPhiMHTJ3", "dPhiMHTJ3", "#Delta #phi_{#slash{H}_{T}, J3}", "Events", 14, -7, 7    );
  His->AddTH1C("dEtaMHTJ3", "dEtaMHTJ3", "#Delta #eta_{#slash{H}_{T}, J3}", "Events", 14, -7, 7    );
  His->AddTH1C("dRMHTJ3",   "dRMHTJ3",   "#Delta R_{#slash{H}_{T}, J3}",    "Events", 20, 0,  10.0 );
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2D ~~~~~
  //His->AddTH1("J12D", "J1  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  //His->AddTH1("J22D", "J2  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  //His->AddTH1("J32D", "J3  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  if (ProName.find("Photon") != std::string::npos)
      His->AddTH2("MetVsPhoton", "Met Vs Photon", 10, 0 , 10, 100, 0, 1000);
}       // -----  end of function DelCutDM::BookHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillJets
//  Description:  Filling jets relative hist
// ===========================================================================
int DelCutDM::FillJets() const
{

  for (int i = 0; i < Ana->vJet->size(); ++i)
  {
    His->FillTH1("JetEta", Ana->vJet->at(i).Eta);
    His->FillTH1("JetPt", Ana->vJet->at(i).PT);
    His->FillTH2("AllJet", Ana->vJet->at(i).Eta, Ana->vJet->at(i).PT);

    if (Ana->vJet->at(i).PT > 20) His->FillTH1("JetEta20", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 30) His->FillTH1("JetEta30", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 40) His->FillTH1("JetEta40", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 50) His->FillTH1("JetEta50", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 70) His->FillTH1("JetEta70", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 100) His->FillTH1("JetEta100", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 120) His->FillTH1("JetEta120", Ana->vJet->at(i).Eta);
    if (Ana->vJet->at(i).PT > 150) His->FillTH1("JetEta150", Ana->vJet->at(i).Eta);
  }

//----------------------------------------------------------------------------
//  Fill in the jet eff. 
//----------------------------------------------------------------------------
/*
 *   for (int i = 0; i < Ana->DEV->vGenJet.size(); ++i)
 *   {
 *     His->FillTH2("GenJet", Ana->DEV->vGenJet.at(i).Eta, Ana->DEV->vGenJet.at(i).PT);
 *   }
 *
 *   for (int i = 0; i < Ana->MatchedJet.size(); ++i)
 *   {
 *     His->FillTH2("RecoJet", Ana->MatchedJet.at(i).Eta(), Ana->MatchedJet.at(i).Pt());
 *   }
 */
   for (int i = 0; i < Ana->vGenJet->size(); ++i)
   {
     if (Ana->MatchedJet.find(i) != Ana->MatchedJet.end() )
     {
       if (Ana->MatchedJet[i] != -1)
       {
         His->FillTPro("JetScalePT", Ana->vGenJet->at(i).PT, 
             Ana->vJet->at(Ana->MatchedJet[i]).PT/Ana->vGenJet->at(i).PT);
         His->FillTPro("JetScaleEta", Ana->vGenJet->at(i).Eta, 
             Ana->vJet->at(Ana->MatchedJet[i]).PT/Ana->vGenJet->at(i).PT);

       }
     }
   }


  return 1;
}       // -----  end of function DelCutDM::FillJets  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillJets
//  Description:  
// ===========================================================================
int DelCutDM::FillJets(int NCut)
{
//----------------------------------------------------------------------------
//  Inclusive Jet 
//----------------------------------------------------------------------------
  int jentries = Ana->vJet->size();
  His->FillTH1(NCut, "NJets", jentries);
  if(jentries <= 0) return 0;

  for (int i = 0; i < Ana->vJet->size(); ++i)
  {
    if (std::fabs(Ana->vJet->at(i).Eta) <= 2)
    {
      His->FillTH1(NCut, "JCPt", Ana->vJet->at(i).PT);
    } else His->FillTH1(NCut, "JFPt", Ana->vJet->at(i).PT);
  }

//----------------------------------------------------------------------------
//  Jet 1
//----------------------------------------------------------------------------
  if (std::find(Ana->PileUpJet.begin(), Ana->PileUpJet.end(), 0) != Ana->PileUpJet.end())
    His->FillTH1(NCut, "J1PU", 1);
  else
    His->FillTH1(NCut, "J1PU", 0);


  His->FillTH1(NCut, "J1Pt", Ana->J1->PT);
  His->FillTH1(NCut, "J1Eta", Ana->J1->Eta);
  His->FillTH1(NCut, "J1Phi", Ana->J1->Phi);
  
  His->FillTH1(NCut, "dPtMHTJ1", Ana->MHT->Pt() - Ana->J1->PT);
  His->FillTH1(NCut, "dPhiMHTJ1", Ana->J1->P4().DeltaPhi(*Ana->MHT));
  His->FillTH1(NCut, "dEtaMHTJ1", Ana->J1->Eta - Ana->MHT->Eta());
  His->FillTH1(NCut, "dRMHTJ1", Ana->J1->P4().DeltaR(*Ana->MHT));

//----------------------------------------------------------------------------
//  Jet 2
//----------------------------------------------------------------------------
  if (Ana->J2 != 0)
  {
    if (std::find(Ana->PileUpJet.begin(), Ana->PileUpJet.end(), 1) != Ana->PileUpJet.end())
      His->FillTH1(NCut, "J2PU", 1);
    else
      His->FillTH1(NCut, "J2PU", 0);
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

  
//----------------------------------------------------------------------------
//  Jet 3
//----------------------------------------------------------------------------
  if (Ana->J3 != 0)
  {
    if (std::find(Ana->PileUpJet.begin(), Ana->PileUpJet.end(), 2) != Ana->PileUpJet.end())
      His->FillTH1(NCut, "J3PU", 1);
    else
      His->FillTH1(NCut, "J3PU", 0);

    His->FillTH1(NCut, "J3Pt", Ana->J3->PT);
    His->FillTH1(NCut, "J3Eta", Ana->J3->Eta);
    His->FillTH1(NCut, "J3Phi", Ana->J3->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MHTJ3 ~~~~~
    His->FillTH1(NCut, "dPtMHTJ3", Ana->MHT->Pt() - Ana->J3->PT);
    His->FillTH1(NCut, "dPhiMHTJ3", Ana->J3->P4().DeltaPhi(*Ana->MHT));
    His->FillTH1(NCut, "dEtaMHTJ3", Ana->J3->Eta - Ana->MHT->Eta());
    His->FillTH1(NCut, "dRMHTJ3", Ana->J3->P4().DeltaR(*Ana->MHT));
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Central Jet ~~~~~
    // Jet3 is within jet1 and jet2 
    if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
        (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
    {
      His->FillTH1(NCut, "CentralJet3Pt", Ana->J3->PT);
      His->FillTH1(NCut, "CentralJet3Eta", Ana->J3->Eta);
      His->FillTH1(NCut, "CentralJet3Phi", Ana->J3->Phi);
    }

    for (int i = 0; i < Ana->vJet->size(); ++i)
    {
      Jet jet = Ana->vJet->at(i);
      if ( (jet.Eta > Ana->J1->Eta && jet.Eta < Ana->J2->Eta) || 
          (jet.Eta > Ana->J2->Eta && jet.Eta < Ana->J1->Eta))
      {
        His->FillTH1(NCut, "CentralJetPt", jet.PT);
        His->FillTH1(NCut, "CentralJetEta", jet.Eta);
        His->FillTH1(NCut, "CentralJetOrder", i);
        if (std::find(Ana->PileUpJet.begin(), Ana->PileUpJet.end(), i) != Ana->PileUpJet.end())
          His->FillTH1(NCut, "CentralPUJetOrder", i);
      }
      
    }

  }

//----------------------------------------------------------------------------
//  Fill in the jet eff. 
//----------------------------------------------------------------------------
   for (int i = 0; i < Ana->vGenJet->size(); ++i)
   {
     if (Ana->MatchedJet.find(i) != Ana->MatchedJet.end() )
     {
       His->FillTH1(NCut, "GenJetPt", Ana->vGenJet->at(i).PT);
       His->FillTH1(NCut, "GenJetEta", Ana->vGenJet->at(i).Eta);
       His->FillTH2(NCut, "GenJet", Ana->vGenJet->at(i).Eta, Ana->vGenJet->at(i).PT);

       if (Ana->MatchedJet[i] != -1)
       {
         His->FillTH1(NCut, "MatchedJetPt", Ana->vGenJet->at(i).PT);
         His->FillTH1(NCut, "MatchedJetEta", Ana->vGenJet->at(i).Eta);
         His->FillTH1(NCut, "JetPt", Ana->vJet->at(Ana->MatchedJet[i]).PT);
         His->FillTH1(NCut, "JetEta", Ana->vJet->at(Ana->MatchedJet[i]).Eta);
         His->FillTH1(NCut, "JetPTScale", Ana->vJet->at(Ana->MatchedJet[i]).PT/Ana->vGenJet->at(i).PT);
         His->FillTH2(NCut, "RecoJet", Ana->vJet->at(Ana->MatchedJet[i]).Eta, 
             Ana->vJet->at(Ana->MatchedJet[i]).PT);
       }
     }
   }

//----------------------------------------------------------------------------
//  Fill in PileUp Jet
//----------------------------------------------------------------------------
   for (int i = 0; i < Ana->PileUpJet.size(); ++i)
   {
     His->FillTH1(NCut, "PUJetPt", Ana->vJet->at(Ana->PileUpJet.at(i)).PT);
     His->FillTH1(NCut, "PUJetEta", Ana->vJet->at(Ana->PileUpJet.at(i)).Eta);
     His->FillTH2(NCut, "PUJet", Ana->vJet->at(Ana->PileUpJet.at(i)).Eta, 
         Ana->vJet->at(Ana->PileUpJet.at(i)).PT);
   }

//----------------------------------------------------------------------------
//  Fill in Jet PT Scale in slides
//----------------------------------------------------------------------------
  double eta[5] ={ -5, -2.5, 0, 2.5, 5 };
  double Pt[11] ={30, 50, 70, 100, 150, 200, 300, 500, 700, 900, 1200};
  for (int i = 0; i < Ana->vGenJet->size(); ++i)
  {
    if (Ana->MatchedJet.find(i) != Ana->MatchedJet.end() )
    {
      if (Ana->MatchedJet[i] != -1)
      {
        for (int j = 1; j < 5; ++j)
        {
          if (Ana->vGenJet->at(i).Eta < eta[j-1] || Ana->vGenJet->at(i).Eta > eta[j]) 
            continue;
          for (int k = 1; k < 11; ++k)
          {
            if (Ana->vGenJet->at(i).PT > Pt[k-1] && Ana->vGenJet->at(i).PT < Pt[k])
            {
              std::stringstream ss;
              ss <<"JetPTScale" << j <<"-"<<k;
              His->FillTH1(NCut, ss.str().c_str(), Ana->vJet->at(Ana->MatchedJet[i]).PT/Ana->vGenJet->at(i).PT);
            }

          }
        }
      }
    }
  }
//----------------------------------------------------------------------------
//  Fill in BJet information
//----------------------------------------------------------------------------
  His->FillTH1(NCut, "NBJets", int(Ana->vBJet.size()));
  int centralb = 0;
   for (int i = 0; i < Ana->vBJet.size(); ++i)
   {
     Jet bjet = Ana->vBJet.at(i);
     His->FillTH1(NCut, "BJetPt", bjet.PT);
     His->FillTH1(NCut, "BJetEta", bjet.Eta);
     if ( (bjet.Eta > Ana->J1->Eta && bjet.Eta < Ana->J2->Eta) || 
         (bjet.Eta > Ana->J2->Eta && bjet.Eta < Ana->J1->Eta))
     {
       His->FillTH1(NCut, "CentralBJetPt", bjet.PT);
       His->FillTH1(NCut, "CentralBJetEta", bjet.Eta);
       centralb++;
       His->FillTH1(NCut, "CentralBJetOrder", i);
     }
     if (std::find(Ana->PileUpJet.begin(), Ana->PileUpJet.end(), i) != Ana->PileUpJet.end())
       His->FillTH1(NCut, "CentralPUBJetOrder", i);
   }

   His->FillTH1(NCut, "NCentralBJets", centralb);

   return 1;
}       // -----  end of function DelCutDM::FillJets  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillEle
//  Description:  
// ===========================================================================
int DelCutDM::FillEle(int NCut)
{


  return 1;
}       // -----  end of function DelCutDM::FillEle  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillMet
//  Description:  
// ===========================================================================
int DelCutDM::FillMet(int NCut)
{
  His->FillTH1(NCut, "GenMet", Ana->GenMet.Pt());
  His->FillTH1(NCut, "MHT", Ana->Met);
  His->FillTH1(NCut, "MET", Ana->RawMet.Mod());
  His->FillTH1(NCut, "MHTMET", Ana->Met - Ana->RawMet.Mod());
  His->FillTH1(NCut, "MetAsys", Ana->METAsys);
  //His->FillTH2(NCut, "MetMHT", Ana->Met, Ana->RawMet.Mod());
  return 1;
}       // -----  end of function DelCutDM::FillEle  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillSampleXS
//  Description:  Save the cross section in bin 1 in histogram XS
// ===========================================================================
bool DelCutDM::FillSampleXS(double xs, double error)
{
  His->AddTH1("CrossSection", "Cross Section", 2, 0, 2);
  His->FillTH1("CrossSection", 1, xs);
  His->FillTH1("CrossSection", 0, error);
}       // -----  end of function DelCutDM::FillSampleXS  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::BookMetPerf
//  Description:  Booking all the histogram for the MET performance study
// ===========================================================================
int DelCutDM::BookMetPerf() const
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
}       // -----  end of function DelCutDM::BookMetPerf  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::BookSUSYVar
//  Description:  
// ===========================================================================
int DelCutDM::BookSUSYVar()
{
  His->AddTH1C("RazorMR", "Razor MR", 350, 0, 3500 );
  His->AddTH1C("RazorR", "Razor R", 15, 0, 1.5 );
  His->AddTH1C("RazorR2", "Razor R^2", 15, 0, 1.5 );
  His->AddTH1C("RazorMRT", "Razor MRT", 200, 0, 1000);
  His->AddTH2C("RazorR_MR", "Razor R vs MR", 200, 0, 1000, 15, 0, 1.5);
  His->AddTH1C("AlphaT", "AlphaT", 20, 0, 1 );
  His->AddTH1C("MT2_Eta", "MT2", 200, 0, 2000 );
  His->AddTH1C("MT2_J1J2", "MT2", 200, 0, 2000 );
  His->AddTH1C("MT2_Razor", "MT2", 200, 0, 2000 );
  His->AddTH1C("MHTHT", "MHT/#sqrt(HT)", 200, 0, 5 );
  His->AddTH1C("METHT", "MET/#sqrt(HT)", 200, 0, 5 );
  His->AddTH1C("BoostMHT", "MHT w.r.t dijet", 200, 0, 1000 );
  His->AddTH1C("BoostHT", "HT w.r.t dijet", 200, 0, 1000 );
  return 1;
}       // -----  end of function DelCutDM::BookSUSYVar  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillMetPerf
//  Description:  filling all the histogram for the MET performance study
// ===========================================================================
int DelCutDM::FillMetPerf() const
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
  His->FillTPro("MetScale", Ana->QT, Ana->MetScale, 1);
  His->FillTPro("MetResP", Ana->QT, Ana->UParallel, 1);
  His->FillTPro("MetResT", Ana->QT, Ana->UTransverse, 1);
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
}       // -----  end of function DelCutDM::FillMetPerf  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillNEVT
//  Description:  Pulic function for filling in the NEVT, it should be same
//  among DelCutDMs
// ===========================================================================
bool DelCutDM::FillNEVT(double weight) const
{
  His->FillTH1("NEVT", 1, weight); //the NEVT with weight 
  return true;
}       // -----  end of function DelCutDM::FillNEVT  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::CheckSysLep
//  Description:  For systematic study
// ===========================================================================
bool DelCutDM::CheckSysLep() const
{
  assert(ProName.find("Sys") != std::string::npos);

  unsigned int nele = 0;
  unsigned int nmuon = 0;


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
}       // -----  end of function DelCutDM::CheckSysLep  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::SysMet
//  Description:  
// ===========================================================================
double DelCutDM::SysMet() const
{
  TLorentzVector temp = *(Ana->MHT);

  for (unsigned int i = 0; i < Ana->vElectron->size(); ++i)
  {
    temp -= Ana->vElectron->at(i).P4();
  }

  for (unsigned int i = 0; i < Ana->vMuon->size(); ++i)
  {
    temp -= Ana->vMuon->at(i).P4();
  }
  
  double met_x = -temp.Px();
  double met_y = -temp.Py();

  TVector2 NewMet(met_x, met_y);

  return NewMet.Mod();
}       // -----  end of function DelCutDM::SysMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::BookLeptonEff
//  Description:  
// ===========================================================================
bool DelCutDM::BookLeptonEff()
{
  His->AddTH1C("GenElePt", "GenElePt", "Pt_{Gen e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("GenMuonPt", "GenMuonPt", "Pt_{Gen m} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("GenTauPt", "GenTauPt", "Pt_{Gen t} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("MatchedElePt", "MatchedElePt", "Pt_{Matched e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("MatchedMuonPt", "MatchedMuonPt", "Pt_{Matched m} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("MatchedTauPt", "MatchedTauPt", "Pt_{Matched t} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("AppGenElePt", "AppGenElePt", "Pt_{Gen e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("AppGenMuonPt", "AppGenMuonPt", "Pt_{Gen m} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("AppGenTauPt", "AppGenTauPt", "Pt_{Gen t} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("AppMatchedElePt", "AppMatchedElePt", "Pt_{Matched e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("AppMatchedMuonPt", "AppMatchedMuonPt", "Pt_{Matched m} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );
  His->AddTH1C("AppMatchedTauPt", "AppMatchedTauPt", "Pt_{Matched t} [GeV] ","Events / 4 GeV",  200, 0, 800.0 );

  His->AddTH1C("ElePt", "ElePt", "Pt_{e} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("MuonPt", "MuonPt", "Pt_{m} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );
  His->AddTH1C("TauPt", "TauPt", "Pt_{t} [GeV] ", "Events / 4 GeV", 200, 0, 800.0 );

  His->AddTH1C("GenEleEta", "GenEleEta", "#eta_{Gen e}", "Events",  50, -5, 5 );
  His->AddTH1C("GenMuonEta", "GenMuonEta", "#eta_{Gen m}","Events",   50, -5, 5 );
  His->AddTH1C("GenTauEta", "GenTauEta", "#eta_{Gen t}","Events",   50, -5, 5 );
  His->AddTH1C("MatchedEleEta", "MatchedEleEta", "#eta_{Matched e}", "Events",  50, -5, 5 );
  His->AddTH1C("MatchedMuonEta", "MatchedMuonEta", "#eta_{Matched m}","Events",   50, -5, 5 );
  His->AddTH1C("MatchedTauEta", "MatchedTauEta", "#eta_{Matched t}","Events",   50, -5, 5 );
  His->AddTH1C("AppGenEleEta", "AppGenEleEta", "#eta_{Gen e}", "Events",  50, -5, 5 );
  His->AddTH1C("AppGenMuonEta", "AppGenMuonEta", "#eta_{Gen m}","Events",   50, -5, 5 );
  His->AddTH1C("AppGenTauEta", "AppGenTauEta", "#eta_{Gen t}","Events",   50, -5, 5 );
  His->AddTH1C("AppMatchedEleEta", "AppMatchedEleEta", "#eta_{Matched e}", "Events",  50, -5, 5 );
  His->AddTH1C("AppMatchedMuonEta", "AppMatchedMuonEta", "#eta_{Matched m}","Events",   50, -5, 5 );
  His->AddTH1C("AppMatchedTauEta", "AppMatchedTauEta", "#eta_{Matched t}","Events",   50, -5, 5 );
  His->AddTH1C("EleEta", "EleEta", "#eta_{e}", "Events",  50, -5, 5 );
  His->AddTH1C("MuonEta", "MuonEta", "#eta_{m}", "Events",  50, -5, 5 );
  His->AddTH1C("TauEta", "TauEta", "#eta_{t}", "Events",  50, -5, 5 );

  His->AddTH2C("GenEle", "Gen Electron", "#eta_{Gen e}", "Pt_{Gen e} [GeV]", 120, -6, 6, 200, 0, 800);
  His->AddTH2C("GenMuon", "Gen Muon", "#eta_{Gen m}", "Pt_{Gen m}", 120, -6., 6., 200, 0., 800.);
  His->AddTH2C("GenTau", "Gen Tau", "#eta_{Gen t}", "Pt_{Gen t}", 120, -6, 6, 200, 0, 800);
  His->AddTH2C("RecoEle", "Reco Electron", "#eta_{e}", "Pt_{e}", 120, -6, 6, 200, 0, 800);
  His->AddTH2C("RecoMuon", "Reco Muon", "#eta_{m}", "Pt_{m}", 120, -6, 6, 200, 0, 800);
  His->AddTH2C("RecoTau", "Reco Tau", "#eta_{t}", "Pt_{t}", 120, -6, 6, 200, 0, 800);

  His->AddTH2C("GenEleJet", "Gen Electron", "#eta_{Gen e}", "Pt_{Gen e} [GeV]", 120, -6, 6, 200, 0, 800);
  His->AddTH2C("GenMuonJet", "Gen Muon", "#eta_{Gen m}", "Pt_{Gen m}", 120, -6., 6., 200, 0., 800.);
  His->AddTH2C("RecoEleJet", "Reco Electron", "#eta_{e}", "Pt_{e}", 120, -6, 6, 200, 0, 800);
  His->AddTH2C("RecoMuonJet", "Reco Muon", "#eta_{m}", "Pt_{m}", 120, -6, 6, 200, 0, 800);
  return true;
}       // -----  end of function DelCutDM::BookLeptonEff  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::BookJetEff
//  Description:  
// ===========================================================================
bool DelCutDM::BookJetEff()
{

//----------------------------------------------------------------------------
//  GenJet, Matched Jet Eff. Matched Jet
//----------------------------------------------------------------------------
  His->AddTH1C("GenJetPt", "GenJetPt", "Pt_{Gen Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("GenJetEta", "GenJetEta", "#eta_{Gen Jet}", "Events",  60, -6, 6 );
  His->AddTH1C("MatchedJetPt", "MatchedJetPt", "Pt_{Matched Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("MatchedJetEta", "MatchedJetEta", "#eta_{Matched Jet}", "Events",  60, -6, 6 );
  His->AddTH1C("JetPt", "JetPt", "Pt_{Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("JetEta", "JetEta", "#eta_{Jet}", "Events", 60, -6, 6 );
  His->AddTH1C("PUJetPt", "PUJetPt", "Pt_{PUJet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("PUJetEta", "PUJetEta", "#eta_{PUJet}", "Events", 60, -6, 6 );
  His->AddTH1C("JetPTScale", "JetPTScale", "Reco Jet / Gen Jet", "Events", 200, -1, 3 );

  double eta[5] ={ -5, -2.5, 0, 2.5, 5 };
  double Pt[11] ={30, 50, 70, 100, 150, 200, 300, 500, 700, 900, 1200};
  for (int i = 1; i < 5; ++i)
  {
    for (int j = 1; j < 11; ++j)
    {
      std::stringstream ss;
      ss <<"JetPTScale" << i <<"-"<<j;
      std::string name = ss.str();
      ss.str("");
      ss << "Reco Jet / Gen Jet " << "( " << eta[i-1] << 
        " < #eta < " << eta[i] << ", " << Pt[j-1] <<" < P_{T} < " << Pt[j] <<" )" ;
      His->AddTH1C(name.c_str(), name.c_str(), ss.str().c_str(), "Events", 200, -1, 3 );
    }
  }
  

  His->AddTH2("AllJet", "All Jet", "#eta_{All Jet}", "Pt_{All Jet} [GeV]", 120, -6, 6, 500, 0, 1000);
  His->AddTH2C("GenJet", "Gen Jet", "#eta_{Gen Jet}", "Pt_{Gen Jet} [GeV]", 120, -6, 6, 500, 0, 1000);
  His->AddTH2C("RecoJet", "Reco Jet", "#eta_{Jet}", "Pt_{Jet}", 120, -6, 6, 500, 0, 1000);
  His->AddTH2C("PUJet", "PU Jet", "#eta_{PUJet}", "Pt_{PUJet}", 120, -6, 6, 500, 0, 1000);
  His->AddTPro("JetScalePT", "JetScalePT", "Pt_{Gen Jet} [GeV]", "Reco Jet / Gen Jet",  500, 0, 1000);
  His->AddTPro("JetScaleEta", "JetScaleEta", "#eta_{Gen Jet}", "Reco Jet / Gen Jet",  60, -6, 6);


//----------------------------------------------------------------------------
//  Whether the first jet is the pileup jet
//----------------------------------------------------------------------------
  His->AddTH1C("J1PU", "Jet 1 Orignal", 2, 0, 1 );
  His->AddTH1C("J2PU", "Jet 2 Orignal", 2, 0, 1 );
  His->AddTH1C("J3PU", "Jet 3 Orignal", 2, 0, 1 );

//----------------------------------------------------------------------------
// Central Jets
//----------------------------------------------------------------------------
  His->AddTH1C("CentralJetPt", "CentralJetPt", "Pt_{Central Jet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("CentralJetEta", "CentralJetEta", "#eta_{Central Jet}", "Events",  60, -6, 6 );
  His->AddTH1C("CentralJetOrder", "CentralJetOrder", "Order of Central Jet", "Events",  40, 0, 40);
  His->AddTH1C("CentralPUJetOrder", "CentralPUJetOrder", "Order of Central PUJet", "Events",  40, 0, 40);


  return true;

}       // -----  end of function DelCutDM::BookJetEff  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillLepton
//  Description:  
// ===========================================================================
int DelCutDM::FillLepton(int NCut) const
{

//----------------------------------------------------------------------------
//  Fill in All Gen Lepton
//----------------------------------------------------------------------------
  for(auto x: Ana->MatchedEle)
  {
    int i = x.first;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gen Ele ~~~~~ 
    His->FillTH1(NCut, "GenElePt", Ana->vGenParticle->at(i).PT);
    His->FillTH1(NCut, "GenEleEta", Ana->vGenParticle->at(i).Eta);
    His->FillTH2(NCut, "GenEle", Ana->vGenParticle->at(i).Eta, Ana->vGenParticle->at(i).PT);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Matched Ele ~~~~~ 
    if (Ana->MatchedEle[i] != -1)
    {
      His->FillTH1(NCut, "MatchedElePt", Ana->vGenParticle->at(i).PT);
      His->FillTH1(NCut, "MatchedEleEta", Ana->vGenParticle->at(i).Eta);
      His->FillTH1(NCut, "ElePt", Ana->vElectron->at(Ana->MatchedEle[i]).PT);
      His->FillTH1(NCut, "EleEta", Ana->vElectron->at(Ana->MatchedEle[i]).Eta);
      His->FillTH2(NCut, "RecoEle", Ana->vElectron->at(Ana->MatchedEle[i]).Eta, Ana->vElectron->at(Ana->MatchedEle[i]).PT);
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gen Muon ~~~~~ 
  for(auto x: Ana->MatchedMuon)
  {
    int i = x.first;
    His->FillTH1(NCut, "GenMuonPt", Ana->vGenParticle->at(i).PT);
    His->FillTH1(NCut, "GenMuonEta", Ana->vGenParticle->at(i).Eta);
    His->FillTH2(NCut, "GenMuon", Ana->vGenParticle->at(i).Eta, Ana->vGenParticle->at(i).PT);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Matched Muon ~~~~~ 
    if (Ana->MatchedMuon[i] != -1)
    {
      His->FillTH1(NCut, "MatchedMuonPt", Ana->vGenParticle->at(i).PT);
      His->FillTH1(NCut, "MatchedMuonEta", Ana->vGenParticle->at(i).Eta);
      His->FillTH1(NCut, "MuonPt", Ana->vMuon->at(Ana->MatchedMuon[i]).PT);
      His->FillTH1(NCut, "MuonEta", Ana->vMuon->at(Ana->MatchedMuon[i]).Eta);
      His->FillTH2(NCut, "RecoMuon", Ana->vMuon->at(Ana->MatchedMuon[i]).Eta, Ana->vMuon->at(Ana->MatchedMuon[i]).PT);
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gen Tau ~~~~~ 
  for(auto x: Ana->MatchedTau)
  {
    int i = x.first;
    His->FillTH1(NCut, "GenTauPt", Ana->vGenParticle->at(i).PT);
    His->FillTH1(NCut, "GenTauEta", Ana->vGenParticle->at(i).Eta);
    His->FillTH2(NCut, "GenTau", Ana->vGenParticle->at(i).Eta, Ana->vGenParticle->at(i).PT);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Matched Tau ~~~~~ 
    if (Ana->MatchedTau[i] != -1)
    {
      His->FillTH1(NCut, "MatchedTauPt", Ana->vGenParticle->at(i).PT);
      His->FillTH1(NCut, "MatchedTauEta", Ana->vGenParticle->at(i).Eta);
      His->FillTH1(NCut, "TauPt", Ana->vJet->at(Ana->MatchedTau[i]).PT);
      His->FillTH1(NCut, "TauEta", Ana->vJet->at(Ana->MatchedTau[i]).Eta);
      His->FillTH2(NCut, "RecoTau", Ana->vJet->at(Ana->MatchedTau[i]).Eta, Ana->vJet->at(Ana->MatchedTau[i]).PT);
    }
  }

 

//----------------------------------------------------------------------------
//  Fill in Gen Lepton within acceptance
//----------------------------------------------------------------------------
  for(auto x: Ana->MatchedEle)
  {
    int i = x.first;
    if (Ana->DetectorApp(Ana->vGenParticle->at(i).Eta))
    {
      His->FillTH1(NCut, "AppGenElePt", Ana->vGenParticle->at(i).PT);
      if (Ana->MatchedEle[i] != -1)
        His->FillTH1(NCut, "AppMatchedElePt", Ana->vGenParticle->at(i).PT);
    }

    if (Ana->vGenParticle->at(i).PT > 10)
    {
      His->FillTH1(NCut, "AppGenEleEta", Ana->vGenParticle->at(i).Eta);
      if (Ana->MatchedEle[i] != -1)
        His->FillTH1(NCut, "AppMatchedEleEta", Ana->vGenParticle->at(i).Eta);
    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Matched Ele ~~~~~ 
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gen Muon ~~~~~ 
  for(auto x: Ana->MatchedMuon)
  {
    int i = x.first;
    if (Ana->DetectorApp(Ana->vGenParticle->at(i).Eta) )
    {
      His->FillTH1(NCut, "AppGenMuonPt", Ana->vGenParticle->at(i).PT);
      if (Ana->MatchedMuon[i] != -1)
        His->FillTH1(NCut, "AppMatchedMuonPt", Ana->vGenParticle->at(i).PT);
    }
    if( Ana->vGenParticle->at(i).PT > 5)
    {
      His->FillTH1(NCut, "AppGenMuonEta", Ana->vGenParticle->at(i).Eta);
      if (Ana->MatchedMuon[i] != -1)
        His->FillTH1(NCut, "AppMatchedMuonEta", Ana->vGenParticle->at(i).Eta);
    }
  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gen Tau ~~~~~ 
  for(auto x: Ana->MatchedTau)
  {
    int i = x.first;
    if (Ana->DetectorApp(Ana->vGenParticle->at(i).Eta))
    {
      His->FillTH1(NCut, "AppGenTauPt", Ana->vGenParticle->at(i).PT);
      if (Ana->MatchedTau[i] != -1)
        His->FillTH1(NCut, "AppMatchedTauPt", Ana->vGenParticle->at(i).PT);
    }
    if(Ana->vGenParticle->at(i).PT > Ana->DEV->JetPtCut)
    {
      His->FillTH1(NCut, "AppGenTauEta", Ana->vGenParticle->at(i).Eta);
      if (Ana->MatchedTau[i] != -1)
        His->FillTH1(NCut, "AppMatchedTauEta", Ana->vGenParticle->at(i).Eta);
    }
  }

//----------------------------------------------------------------------------
//  Fill in All Gen Lepton matched to jet
//----------------------------------------------------------------------------
  for(auto x: Ana->MatchedEleJet)
  {
    int i = x.first;
    if (Ana->MatchedEleJet[i] != -1)
    {
      His->FillTH2(NCut, "GenEleJet", Ana->vGenParticle->at(i).Eta, Ana->vGenParticle->at(i).PT);
      His->FillTH2(NCut, "RecoEleJet", Ana->vJet->at(Ana->MatchedEleJet[i]).Eta, Ana->vJet->at(Ana->MatchedEleJet[i]).PT);
    }

  }

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Gen Muon ~~~~~ 
  for(auto x: Ana->MatchedMuonJet)
  {
    int i = x.first;
    if (Ana->MatchedMuonJet[i] != -1)
    {
      His->FillTH2(NCut, "GenMuonJet", Ana->vGenParticle->at(i).Eta, Ana->vGenParticle->at(i).PT);
      His->FillTH2(NCut, "RecoMuonJet", Ana->vJet->at(Ana->MatchedMuonJet[i]).Eta, Ana->vJet->at(Ana->MatchedMuonJet[i]).PT);
    }
  }


  return true;
}       // -----  end of function DelCutDM::FillLepton  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillLepton
//  Description:  
// ===========================================================================
bool DelCutDM::FillLepton()
{
  for (unsigned int i = 0; i < Ana->vElectron->size(); ++i)
  {
    His->FillTH1("EleEta", Ana->vElectron->at(i).Eta);
    His->FillTH1("ElePt", Ana->vElectron->at(i).PT);
  }

  for (unsigned int i = 0; i < Ana->vMuon->size(); ++i)
  {
    His->FillTH1("MuonEta", Ana->vMuon->at(i).Eta);
    His->FillTH1("MuonPt", Ana->vMuon->at(i).PT);
  }

  for (unsigned int i = 0; i < Ana->vJet->size(); ++i)
  {
    if (Ana->vJet->at(i).TauTag)
    {
      His->FillTH1("TauEta", Ana->vJet->at(i).Eta);
      His->FillTH1("TauPt", Ana->vJet->at(i).PT);
    }
  }

  for (unsigned int i = 0; i < Ana->vPhoton->size(); ++i)
  {
    His->FillTH1("PhotonEta", Ana->vPhoton->at(i).Eta);
    His->FillTH1("PhotonPt", Ana->vPhoton->at(i).PT);
  }

  return true;
} // -----  end of function DelCutDM::FillLepton  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillSUSYVar
//  Description:  
// ===========================================================================
int DelCutDM::FillSUSYVar(int NCut ) const
{
  His->FillTH1(NCut, "AlphaT", Ana->AlphaT);
  His->FillTH1(NCut, "RazorMR", Ana->RazorMR);
  His->FillTH1(NCut, "RazorMRT", Ana->RazorMRT);
  His->FillTH1(NCut, "RazorR", Ana->RazorR);
  His->FillTH1(NCut, "RazorR2", Ana->RazorR*Ana->RazorR);
  His->FillTH2(NCut, "RazorR_MR", Ana->RazorMR, Ana->RazorR);

  //assert(MT2sides.size() == 2);
  //Mt2 = MT2_CalcMT2(MT2sides.at(0), MT2sides.at(1));
  if (Ana->MT2sidesEta.size() >= 2)
    His->FillTH1(NCut, "MT2_Eta", Ana->MT2_CalcMT2(0, Ana->MT2sidesEta.at(0), Ana->MT2sidesEta.at(1)));
  if (Ana->MT2sidesJ1J2.size() >= 2)
    His->FillTH1(NCut, "MT2_J1J2", Ana->MT2_CalcMT2(0, Ana->MT2sidesJ1J2.at(0), Ana->MT2sidesJ1J2.at(1)));
  if (Ana->RazorJets.size() >= 2)
    His->FillTH1(NCut, "MT2_Razor", Ana->MT2_CalcMT2(0, Ana->RazorJets.at(0), Ana->RazorJets.at(1)));

  His->FillTH1(NCut, "MHTHT", Ana->PUCorMet->Mod()/sqrt(*Ana->HT));
  His->FillTH1(NCut, "METHT", Ana->RawMet.Mod()/sqrt(*Ana->HT));
  His->FillTH1(NCut, "BoostMHT", Ana->VBFBoostMET());
  His->FillTH1(NCut, "BoostHT", Ana->VBFBoostHT());
  return 1;
}       // -----  end of function DelCutDM::FillSUSYVar  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::BookBJet
//  Description:  
// ===========================================================================
bool DelCutDM::BookBJet()
{

  His->AddTH1C("NGenBJets", "Num. of GenBJets", "Number of GenBJets", "Events", 10, 0, 10 );
  His->AddTH1C("GenBJetPt", "GenBJetPt", "Pt_{Gen BJet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("GenBJetEta", "GenBJetEta", "#eta_{Gen BJet}", "Events",  60, -6, 6 );
  His->AddTH1C("NBJets", "Num. of BJets", "Number of BJets", "Events", 10, 0, 10 );
  His->AddTH1C("BJetPt", "BJetPt", "Pt_{BJet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("BJetEta", "BJetEta", "#eta_{BJet}", "Events", 60, -6, 6 );
//----------------------------------------------------------------------------
// Central BJets
//----------------------------------------------------------------------------
  His->AddTH1C("NCentralBJets", "Num. of CentralBJets", "Number of CentralBJets", "Events", 10, 0, 10 );
  His->AddTH1C("CentralBJetPt", "CentralBJetPt", "Pt_{Central BJet} [GeV]", "Events / 2 GeV", 500, 0, 1000.0 );
  His->AddTH1C("CentralBJetEta", "CentralBJetEta", "#eta_{Central BJet}", "Events",  60, -6, 6 );
  His->AddTH1C("CentralBJetOrder", "CentralBJetOrder", "Order of Central BJet", "Events",  10, 0, 10);
  His->AddTH1C("CentralPUBJetOrder", "CentralPUBJetOrder", "Order of Central PUBJet", "Events",  10, 0, 10);
  return true;
}       // -----  end of function DelCutDM::BookBJet  -----
