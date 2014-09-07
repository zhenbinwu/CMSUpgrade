// ===========================================================================
// 
//       Filename:  DelCutVBFb.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/10/2014 08:18:07 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#include "DelCutVBFb.h"

//----------------------------------------------------------------------------
//       Class:  DelCutVBFb
//      Method:  DelCutVBFb
// Description:  constructor
//----------------------------------------------------------------------------
DelCutVBFb::DelCutVBFb (DelAna *ana, std::shared_ptr<TFile> OutFile, 
    std::string name, std::string cut_ )
: DelCut(ana,  OutFile, name, cut_ ), ProName(name), AnaCut(cut_)
{
}  // -----  end of method DelCutVBFb::DelCutVBFb  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutVBFb
//      Method:  DelCutVBFb
// Description:  copy constructor
//----------------------------------------------------------------------------
DelCutVBFb::DelCutVBFb ( const DelCutVBFb &other ) : DelCut(other)
{
}  // -----  end of method DelCutVBFb::DelCutVBFb  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutVBFb
//      Method:  ~DelCutVBFb
// Description:  destructor
//----------------------------------------------------------------------------
DelCutVBFb::~DelCutVBFb () 
{
}  // -----  end of method DelCutVBFb::-DelCutVBFb  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutVBFb
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  DelCutVBFb&
DelCutVBFb::operator = ( const DelCutVBFb &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method DelCutVBFb::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  DelCutVBFb::InitCutOrder
//  Description:  
// ===========================================================================
bool DelCutVBFb::InitCutOrder(std::string ana)
{
   AnaCut = ana;

  //Clear the objects
  CutOrder.clear();
  CutMap.clear();


  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("VBFCut");
  CutOrder.push_back("VBFbVeto");
  CutOrder.push_back("J1J2");
  CutOrder.push_back("Mjj1500");
  CutOrder.push_back("LepVeto");
  CutOrder.push_back("Met200");
  CutOrder.push_back("BjetPT");
  CutOrder.push_back("1Bjet");
  CutOrder.push_back("2Bjet");
  CutOrder.push_back("0Bjet");

  //Set the cutbit of each cut
  CutMap["NoCut"]    = "00000000000000000";
  CutMap["VBFCut"]   = "00000000001111111";
  CutMap["VBFbVeto"] = "00000000011111111";
  CutMap["J1J2"]     = "00000001111111111";
  CutMap["Mjj1500"]  = "00000011111111111";
  CutMap["LepVeto"]  = "00000111111111111";
  CutMap["Met200"]   = "00001111111111111";
  CutMap["BjetPT"]   = "00011111111111111";
  CutMap["1Bjet"]    = "00111111111111111";
  CutMap["2Bjet"]    = "01011111111111111";
  CutMap["0Bjet"]    = "10001111111111111";

  assert(CutOrder.size() == CutMap.size());
  His->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));

  return true;
}       // -----  end of function DelCutVBFb::InitCutOrder  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutVBFb::CheckCut
//  Description:  
// ===========================================================================
bool DelCutVBFb::CheckCut()
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

  // Veto the bjet in VBF jets
  if (Ana->J1 != 0 && Ana->J2 != 0)
    cutbit.set(7, !(Ana->J1->BTag & (1<<0) || Ana->J2->BTag & (1<<0)));

//----------------------------------------------------------------------------
//  Leading jet cut
//----------------------------------------------------------------------------
  if (Ana->PileUp == "140PileUp")
    cutbit.set(8,  Ana->J1 != 0 && Ana->J1->PT >= 200);
  else
    cutbit.set(8,  Ana->J1 != 0 && Ana->J1->PT >= 50);

  if (Ana->PileUp == "140PileUp")
    cutbit.set(9,  Ana->J2 != 0 && Ana->J2->PT >= 100);
  else
    cutbit.set(9,  Ana->J2 != 0 && Ana->J2->PT >= 50);

  cutbit.set(10,  Ana->Mjj >= 1500. );


  /*
   * // By default, passed central jet veto
   *if (Ana->J1 != 0 && Ana->J2 != 0 )
   *{
   *  cutbit.set(10,  true);
   *  if (Ana->J3 != 0)
   *  {
   *    // Jet3 is within jet1 and jet2 
   *    if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
   *        (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
   *    {
   *      cutbit.set(10,  Ana->J3->PT < 30 );
   *    }
   *  }
   *}
   */


  bool hasTau = false;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->TauTag) hasTau = true;
    break;
  }

  if (ProName.find("Sys") == std::string::npos)
    cutbit.set(11, !hasTau && Ana->vElectron->size() == 0 &&  Ana->vMuon->size() == 0);
  else 
    if (CheckSysLep() == false) return false;

  cutbit.set(12, Ana->Met >= 200);

  
  // Count Nbjet with tight tag and PT < 80GeV
  int Nbjet = 0;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->BTag & (1<<0) && it->PT < 80) Nbjet++;
  }
  cutbit.set(13, Nbjet > 0);

  cutbit.set(14, Nbjet == 1);
  cutbit.set(15, Nbjet == 2);
  cutbit.set(16, Nbjet == 0);

//----------------------------------------------------------------------------
//  Always fill in the event cutbits information
//----------------------------------------------------------------------------
  std::vector<bool> Vbits;
  for (std::size_t i = 0; i < cutbit.size(); ++i)
    Vbits.push_back(cutbit.test(i));
  His->FillCutBits(Vbits);

  return true;
}       // -----  end of function DelCutVBFb::CheckCut  -----



// ===  FUNCTION  ============================================================
//         Name:  DelCutVBFb::FillCut
//  Description:  
// ===========================================================================
int DelCutVBFb::FillCut()
{
//----------------------------------------------------------------------------
//  Check cut and fill cut-based plots
//----------------------------------------------------------------------------
  CheckCut();

  for (unsigned int i = 0; i < CutOrder.size(); ++i)
  {
    std::bitset<NBITS> locbit(CutMap[CutOrder.at(i)]);
    if ( (cutbit & locbit) != locbit) continue;

    His->FillTH1("CutFlow", int(i)); 

    // Filling by functions
    FillJets(i);
  }

  return true;
}       // -----  end of function DelCutVBFb::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutVBFb::BookHistogram
//  Description:  
// ===========================================================================
bool DelCutVBFb::BookHistogram()
{
  //BookJetEff();
  DelCut::BookHistogram();
  return true;
}       // -----  end of function DelCutVBFb::BookHistogram  -----
