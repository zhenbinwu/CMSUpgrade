// ===========================================================================
// 
//       Filename:  DelCutHiggs.cc
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

#include "DelCutHiggs.h"

//----------------------------------------------------------------------------
//       Class:  DelCutHiggs
//      Method:  DelCutHiggs
// Description:  constructor
//----------------------------------------------------------------------------
DelCutHiggs::DelCutHiggs (DelAna *ana, std::shared_ptr<TFile> OutFile, 
    std::string name, std::string cut_ )
: DelCut(ana,  OutFile, name, cut_ ), ProName(name), AnaCut(cut_)
{
}  // -----  end of method DelCutHiggs::DelCutHiggs  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutHiggs
//      Method:  DelCutHiggs
// Description:  copy constructor
//----------------------------------------------------------------------------
DelCutHiggs::DelCutHiggs ( const DelCutHiggs &other ) : DelCut(other)
{
}  // -----  end of method DelCutHiggs::DelCutHiggs  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutHiggs
//      Method:  ~DelCutHiggs
// Description:  destructor
//----------------------------------------------------------------------------
DelCutHiggs::~DelCutHiggs () 
{
}  // -----  end of method DelCutHiggs::-DelCutHiggs  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutHiggs
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  DelCutHiggs&
DelCutHiggs::operator = ( const DelCutHiggs &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method DelCutHiggs::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  DelCutHiggs::InitCutOrder
//  Description:  
// ===========================================================================
bool DelCutHiggs::InitCutOrder(std::string ana)
{
  AnaCut = ana;

  //Clear the objects
  CutOrder.clear();
  CutMap.clear();

  //Add name and order of the cutflow
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

  //Set the cutbit of each cut

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

  assert(CutOrder.size() == CutMap.size());
  His->Cutorder(ana, CutOrder);

  return true;
}       // -----  end of function DelCutHiggs::InitCutOrder  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutHiggs::CheckCut
//  Description:  
// ===========================================================================
bool DelCutHiggs::CheckCut()
{
  cutbit.reset();

//----------------------------------------------------------------------------
//  Trigger, just 2 jet trigger?
//----------------------------------------------------------------------------
  cutbit.set(0, Ana->vJet->size() >= 2);
 

//----------------------------------------------------------------------------
//  MET Filter
//----------------------------------------------------------------------------
  cutbit.set(1, Ana->METMHTAsys());

//----------------------------------------------------------------------------
//  Electron Veto
//----------------------------------------------------------------------------
  cutbit.set(2, Ana->vElectron->size() == 0);

//----------------------------------------------------------------------------
//  Muon Veto
//----------------------------------------------------------------------------
  cutbit.set(3, Ana->vMuon->size() == 0);

//----------------------------------------------------------------------------
//  J1 J2 Pt 
//----------------------------------------------------------------------------
  if (Ana->J1 != 0 && Ana->J2 != 0)
    cutbit.set(4, Ana->J1->PT >= 50 && std::fabs(Ana->J1->Eta) <= 4.7
        && Ana->J2->PT >= 50 && std::fabs(Ana->J2->Eta) <= 4.7);

//----------------------------------------------------------------------------
//  J1J2 Eta
//----------------------------------------------------------------------------
  if (Ana->J1 != 0 && Ana->J2 != 0)
    cutbit.set(5, Ana->J1->Eta * Ana->J2->Eta < 0);

//----------------------------------------------------------------------------
//  J1 J2 Delta Eta
//----------------------------------------------------------------------------
  if (Ana->J1 != 0 && Ana->J2 != 0)
    cutbit.set(6, fabs(Ana->J1->Eta - Ana->J2->Eta ) >= 4.2);

//----------------------------------------------------------------------------
//  MET 130
//----------------------------------------------------------------------------
  cutbit.set(7, Ana->Met >= 130);

//----------------------------------------------------------------------------
//  MJJ 
//----------------------------------------------------------------------------
  cutbit.set(8,  Ana->Mjj >= 1100. );


//----------------------------------------------------------------------------
//  Central Jet Veto
//----------------------------------------------------------------------------
  if (Ana->J1 != 0 && Ana->J2 != 0 )
  {
    cutbit.set(9,  true);
    if (Ana->J3 != 0)
    {
      // Jet3 is within jet1 and jet2 
      if ( (Ana->J3->Eta > Ana->J1->Eta && Ana->J3->Eta < Ana->J2->Eta) || 
          (Ana->J3->Eta > Ana->J2->Eta && Ana->J3->Eta < Ana->J1->Eta))
      {
        cutbit.set(9,  Ana->J3->PT < 30);
      }
    }
  }

//----------------------------------------------------------------------------
//  Delta Phi 
//----------------------------------------------------------------------------
  if (Ana->J1 != 0 && Ana->J2 != 0)
  {
    double deltaphi = Ana->J1->P4().DeltaPhi(Ana->J2->P4());
    cutbit.set(10, std::fabs(deltaphi) <= 1.0 );
  }

//---------------------------------------------------------------------------
//  Tau Veto
//----------------------------------------------------------------------------
  bool hasTau = false;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->TauTag) hasTau = true;
    break;
  }
  cutbit.set(11, !hasTau);

//---------------------------------------------------------------------------
//  B Veto
//----------------------------------------------------------------------------
  bool hasB = false;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->BTag) hasB = true;
    break;
  }
  cutbit.set(12, !hasB);

  return true;
}       // -----  end of function DelCutHiggs::CheckCut  -----



// ===  FUNCTION  ============================================================
//         Name:  DelCutHiggs::FillCut
//  Description:  
// ===========================================================================
int DelCutHiggs::FillCut()
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
}       // -----  end of function DelCutHiggs::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutHiggs::BookHistogram
//  Description:  
// ===========================================================================
bool DelCutHiggs::BookHistogram()
{
  BookJetEff();
  return true;
}       // -----  end of function DelCutHiggs::BookHistogram  -----
