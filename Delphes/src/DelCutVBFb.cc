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
    if (it->BTag & (1<<0)) hasB = true;
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
  BookJetEff();
  return true;
}       // -----  end of function DelCutVBFb::BookHistogram  -----
