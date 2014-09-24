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
  //CutOrder.push_back("CTJ3");
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
//CutMap["CTJ3"]      = "00000001111111111";
  CutMap["CTBJ"]      = "00000101111111111";
  CutMap["CTLep"]     = "00001101111111111";
  CutMap["CTMet200"]  = "00011101111111111";
  CutMap["CTdPhi"]    = "00111101111111111";
  CutMap["ALL"]       = "00111101111111111";

  assert(CutOrder.size() == CutMap.size());
  His->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));

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
    cutbit.set(6, fabs(Ana->J1->Eta - Ana->J2->Eta ) >= 3.5);

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
    cutbit.set(12, !hasTau && Ana->vElectron->size() == 0 &&  Ana->vPhoton->size() == 0 &&  Ana->vMuon->size() == 0);
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
}       // -----  end of function DelCutDM::CheckCut  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutDM::FillCut
//  Description:  Main function to be called in DelFill
// ===========================================================================
int DelCutDM::FillCut()
{
  His->FillTH1("MetRelSys", Ana->METAsys);
  His->FillTPro("MHTAsys", Ana->Met, Ana->METAsys);
  His->FillTPro("METAsys", Ana->RawMet.Mod(), Ana->METAsys);
//----------------------------------------------------------------------------
// Filling jets Globally
//----------------------------------------------------------------------------
   FillJets();
   FillLepton();

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
    FillBJet(i);
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
  DelCut::BookHistogram();
}       // -----  end of function DelCutDM::BookHistogram  -----

