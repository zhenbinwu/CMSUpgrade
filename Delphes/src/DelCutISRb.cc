// ===========================================================================
// 
//       Filename:  DelCutISRb.cc
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

#include "DelCutISRb.h"

//----------------------------------------------------------------------------
//       Class:  DelCutISRb
//      Method:  DelCutISRb
// Description:  constructor
//----------------------------------------------------------------------------
DelCutISRb::DelCutISRb (DelAna *ana, std::shared_ptr<TFile> OutFile, 
    std::string name, std::string cut_ )
: DelCut(ana,  OutFile, name, cut_ ), ProName(name), AnaCut(cut_)
{
}  // -----  end of method DelCutISRb::DelCutISRb  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutISRb
//      Method:  DelCutISRb
// Description:  copy constructor
//----------------------------------------------------------------------------
DelCutISRb::DelCutISRb ( const DelCutISRb &other ) : DelCut(other)
{
}  // -----  end of method DelCutISRb::DelCutISRb  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutISRb
//      Method:  ~DelCutISRb
// Description:  destructor
//----------------------------------------------------------------------------
DelCutISRb::~DelCutISRb () 
{
}  // -----  end of method DelCutISRb::-DelCutISRb  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelCutISRb
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  DelCutISRb&
DelCutISRb::operator = ( const DelCutISRb &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method DelCutISRb::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  DelCutISRb::InitCutOrder
//  Description:  
// ===========================================================================
bool DelCutISRb::InitCutOrder(std::string ana)
{
   AnaCut = ana;

  //Clear the objects
  CutOrder.clear();
  CutMap.clear();


  //Add name and order of the cutflow
  CutOrder.push_back("NoCut");
  CutOrder.push_back("METAsys");
  CutOrder.push_back("Leadbjet");
  CutOrder.push_back("MET400");
  CutOrder.push_back("Nbjet");
  CutOrder.push_back("LepVeto");
  CutOrder.push_back("LBjetPT");
  CutOrder.push_back("dPhibMET");
  CutOrder.push_back("MTtop");

  //Set the cutbit of each cut
  CutMap["NoCut"]    = "00000000000000000";
  CutMap["METAsys"]  = "00000000000000001";
  CutMap["Leadbjet"] = "00000000000000111";
  CutMap["MET400"]   = "00000000000001111";
  CutMap["Nbjet"]    = "00000000000011111";
  CutMap["LepVeto"]  = "00000000000111111";
  CutMap["LBjetPT"]  = "00000000001111111";
  CutMap["dPhibMET"] = "00000000011111111";
  CutMap["MTtop"]    = "00000000111111111";


  assert(CutOrder.size() == CutMap.size());
  His->Cutorder(ana, CutOrder, static_cast<unsigned int>(NBITS));

  return true;
}       // -----  end of function DelCutISRb::InitCutOrder  -----


// ===  FUNCTION  ============================================================
//         Name:  DelCutISRb::CheckCut
//  Description:  
// ===========================================================================
bool DelCutISRb::CheckCut()
{
   cutbit.reset();

  cutbit.set(0, Ana->METMHTAsys());

//----------------------------------------------------------------------------
// The leading non b-jet with PT(j1) > 120GeV
//----------------------------------------------------------------------------
  cutbit.set(1, Ana->J1 !=0 && !Ana->J1->BTag & (1<<0));
  if (Ana->J1 != 0)
  cutbit.set(2, Ana->J1->PT > 120);


//----------------------------------------------------------------------------
//  MHT cut on 430 as in Table 1
//----------------------------------------------------------------------------
  cutbit.set(3, Ana->Met > 400);


//----------------------------------------------------------------------------
//  At least 1 bjet with PT > 25 and |eta| < 2.5
//----------------------------------------------------------------------------
  int Nbjet = 0;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->BTag & (1<<0))
      if (it->PT > 25 && fabs(it->Eta) < 2.5)
        Nbjet++;
  }
  cutbit.set(4, Nbjet >= 1);


//----------------------------------------------------------------------------
//  Lepton veto: Pt> 20GeV and |eta| < 2.5
//----------------------------------------------------------------------------
  bool hasLep = false;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->TauTag)
      if (it->PT > 20 && fabs(it->Eta) < 2.5)
        hasLep = true;
    break;
  }
  for(std::vector<Electron>::iterator it=Ana->vElectron->begin();
      it!=Ana->vElectron->end(); ++it)
  {
    if (it->PT > 20 && fabs(it->Eta) < 2.5)
      hasLep = true;
    break;
  }
  for(std::vector<Muon>::iterator it=Ana->vMuon->begin();
      it!=Ana->vMuon->end(); ++it)
  {
    if (it->PT > 20 && fabs(it->Eta) < 2.5)
      hasLep = true;
    break;
  }
  cutbit.set(5, !hasLep);


//----------------------------------------------------------------------------
// Get the Leading b-jet
//----------------------------------------------------------------------------
  Jet Lbjet;
  for(std::vector<Jet>::iterator it=Ana->vJet->begin();
      it!=Ana->vJet->end(); ++it)
  {
    if (it->BTag & (1<<0))
        Lbjet = *it;
    break;
  }

//----------------------------------------------------------------------------
//  Upper limit cut on the leading b-jet
//----------------------------------------------------------------------------
  cutbit.set(5, Lbjet.PT < 100);


//----------------------------------------------------------------------------
//  Delta Phi of leading b-jet and MET
//----------------------------------------------------------------------------
  double deltaphi = Lbjet.P4().DeltaPhi(*Ana->MHT);
  cutbit.set(6, fabs(deltaphi) < 1.8);

//----------------------------------------------------------------------------
//  MTT: too complicated to be implemented
//----------------------------------------------------------------------------
  cutbit.set(7, true);


//----------------------------------------------------------------------------
//  Always fill in the event cutbits information
//----------------------------------------------------------------------------
  std::vector<bool> Vbits;
  for (std::size_t i = 0; i < cutbit.size(); ++i)
    Vbits.push_back(cutbit.test(i));
  His->FillCutBits(Vbits);

  return true;
}       // -----  end of function DelCutISRb::CheckCut  -----



// ===  FUNCTION  ============================================================
//         Name:  DelCutISRb::FillCut
//  Description:  
// ===========================================================================
int DelCutISRb::FillCut()
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
}       // -----  end of function DelCutISRb::FillCut  -----

// ===  FUNCTION  ============================================================
//         Name:  DelCutISRb::BookHistogram
//  Description:  
// ===========================================================================
bool DelCutISRb::BookHistogram()
{
  //BookJetEff();
  DelCut::BookHistogram();
  return true;
}       // -----  end of function DelCutISRb::BookHistogram  -----
