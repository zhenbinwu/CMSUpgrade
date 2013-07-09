// ===========================================================================
// 
//       Filename:  DelCut.cc
// 
//    Description:  Class for Cut flow 
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
  Ana = ana;
  His = new HistTool(name);
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
  
//----------------------------------------------------------------------------
//  Booking global histogram
//----------------------------------------------------------------------------
  His->AddTH1("NEVT", "Num. of Events", 2, 0, 2 );
  His->AddTH1("Weight", "Weight", 100, 0, 10 );
  His->AddTH1("CutFlow", "CutFlow " ,  10, 0 , 10 );
  His->AddTH1("NJets", "Num. of Jets", "Number of Jets", "Events", 40, 0, 40 );
  His->AddTH1("NEle", "Num. of Electrons", 10, 0, 10 );
  His->AddTH1("NMuon", "Num. of Muons", 10, 0, 10 );
  His->AddTH1("NPhoton", "Num. of Photons", 10, 0, 10 );

  His->AddTH1("Met", "Met", "#slash{H}_{T} [GeV]", "Events / 8 GeV",  100, 0, 800, 0, 1);
  His->AddTH1("RawMet", "RawMet", "Raw #slash{E}_{T} [GeV]", "Events / 8 GeV",  100, 0, 800, 0, 1);

  His->AddTH1("JetEta", "#eta_{Jets}", 280, -7, 7 );
//----------------------------------------------------------------------------i
//  Booking histogram for MET performance study
//----------------------------------------------------------------------------
  BookMetPerf();

//----------------------------------------------------------------------------
//  Booking histogram for each cut
//----------------------------------------------------------------------------
  His->AddTH1C("CMet", "CMet", "PU Corrected #slash{E}_{T} [GeV]", 
      "Events/ 20 GeV", 100, 0, 800, 0, 1);
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

  His->AddTH1C("MJJ", "M_{J1, J2}", 4000, 0, 8000.0 );
  His->AddTH1C("dPtJJ", "#Delta Pt_{J1, J2}", 40, 0, 1200 );
  His->AddTH1C("dPhiJJ", "#Delta #phi_{J1, J2}", 14, -7, 7 );
  His->AddTH1C("dEtaJJ", "#Delta #eta_{J1, J2}", 14, -7, 7 );
  His->AddTH1C("dRJJ", "#Delta R_{J1, J2}", 20, 0, 10.0 );

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2D ~~~~~
  //His->AddTH1("J12D", "J1  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  //His->AddTH1("J22D", "J2  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  //His->AddTH1("J32D", "J3  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
}       // -----  end of function DelCut::BookHistogram  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCut::InitCutOrder
//  Description:  
// ===========================================================================
bool DelCut::InitCutOrder()
{
   // The Cut flow
   CutOrder.push_back("NoCut");
   CutOrder.push_back("CTJ1");
   CutOrder.push_back("CTJ2");
   CutOrder.push_back("CTMjj");
   CutOrder.push_back("CTJ3BL");
   CutOrder.push_back("CTMet200");
   CutOrder.push_back("AllCut");

   CutMap["NoCut"]    = "0000000000";
   CutMap["CTJ1"]     = "0000000001";
   CutMap["CTJ2"]     = "0000000011";
   CutMap["CTMjj"]    = "0000000111";
   CutMap["CTJ3BL"]   = "0000001111";
   CutMap["CTMet200"] = "0000011111";
   CutMap["AllCut"]   = "1111111111";

   His->Cutorder(CutOrder);
  
}       // -----  end of function DelCut::InitCutOrder  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CutFlow
//  Description:  
// ===========================================================================
bool DelCut::CutFlow(std::bitset<10> cutbit)
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
  Ana->Clear();
  Ana->GetBasic();

  His->SetWeight(Ana->Weight);
  His->FillTH1("NEVT", 1, 1);
  His->FillTH1("Weight", Ana->Weight);
  His->FillTH1("NJets", (int)Ana->vJet->size());
  His->FillTH1("NEle", (int)Ana->vElectron->size());
  His->FillTH1("NMuon", (int)Ana->vMuon->size());
  His->FillTH1("NPhoton", (int)Ana->vPhoton->size());
  His->FillTH1("RawMet", Ana->RawMet.Mod());
  His->FillTH1("Met", Ana->PUCorMet->Mod());

//----------------------------------------------------------------------------
//  Filling jets
//----------------------------------------------------------------------------
   FillJets();

//----------------------------------------------------------------------------
//  Filling histogram for MET performance study
//----------------------------------------------------------------------------
  FillMetPerf();

  for (int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<10> locbit(CutMap[CutOrder.at(i)]);
    if (CheckCut(locbit) == false) continue;
    His->FillTH1("CutFlow", i);
    // Filling by functions
    FillJets(i);
    FillMet(i);
  }

  return 1;
}       // -----  end of function DelCut::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckCut
//  Description:  
// ===========================================================================
bool DelCut::CheckCut(std::bitset<10> cutflag)
{
  return CheckPhenoCut(cutflag);
  //return true;
}       // -----  end of function DelCut::CheckCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::CheckPhenoCut
//  Description:  
// ===========================================================================
bool DelCut::CheckPhenoCut(std::bitset<10> cutflag)
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

  His->FillTH1(NCut, "J1Pt", Ana->J1->PT);
  His->FillTH1(NCut, "J1Eta", Ana->J1->Eta);
  His->FillTH1(NCut, "J1Phi", Ana->J1->Phi);
  
  if (Ana->J2 != 0)
  {
    His->FillTH1(NCut, "J2Pt", Ana->J2->PT);
    His->FillTH1(NCut, "J2Eta", Ana->J2->Eta);
    His->FillTH1(NCut, "J2Phi", Ana->J2->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

    His->FillTH1(NCut, "MJJ", Ana->Mjj);
    His->FillTH1(NCut, "dPtJJ", Ana->J1->PT-Ana->J2->PT);
    His->FillTH1(NCut, "dPhiJJ", Ana->J1->P4().DeltaPhi(Ana->J2->P4()));
    His->FillTH1(NCut, "dEtaJJ", Ana->J1->Eta - Ana->J2->Eta);
    His->FillTH1(NCut, "dRJJ", Ana->J1->P4().DeltaR(Ana->J2->P4()));
  }

  
  if (Ana->J3 != 0)
  {
    His->FillTH1(NCut, "J3Pt", Ana->J3->PT);
    His->FillTH1(NCut, "J3Eta", Ana->J3->Eta);
    His->FillTH1(NCut, "J3Phi", Ana->J3->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

  }
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
  His->FillTH1(NCut, "CMet", Ana->PUCorMet->Mod());
  return 1;
}       // -----  end of function DelCut::FillEle  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCut::FillSampleXS
//  Description:  Save the cross section in bin 1 in histogram XS
// ===========================================================================
bool DelCut::FillSampleXS(double xs)
{
  His->AddTH1("CrossSection", "Cross Section", 2, 0, 2);
  His->FillTH1("CrossSection", 1, xs);
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
  His->AddTPro("MetScale", "MetScale", "Z/#gamma q_{T} [GeV]", "-<u_{#parallel}>/q_{T}",  50, 0, 400);
  double xbin[12] = {0, 40 , 60, 80, 100, 120, 140, 160, 200, 240, 300, 400};
  TProfile* pro1 = new TProfile( "MetResP", "MetResP;Z/#gamma q_{T} [GeV];#sigma(u_{#parallel}) [GeV]",  11, xbin, "S");
  pro1->BuildOptions(-300, 300, "s");
  His->AddTPro(pro1);
  TProfile* pro2 = new TProfile( "MetResT", "MetResT;Z/#gamma q_{T} [GeV];#sigma(u_{#perp}) [GeV]",  11, xbin, "S");
  pro2->BuildOptions(-300, 300, "s");
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
