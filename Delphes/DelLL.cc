// ===========================================================================
// 
//       Filename:  DelLL.cc
// 
//    Description:  A class for LL  
// 
//        Version:  1.0
//        Created:  09/04/2013 09:43:12 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelLL.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelLL
//      Method:  DelLL
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelLL::DelLL (double Eta, double Pt) : DelEvent(Eta, Pt)
{
}  // ~~~~~  end of method DelLL::DelLL  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelLL
//      Method:  DelLL
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelLL::DelLL ( const DelLL &other ) : DelEvent(other)
{
}  // ~~~~~  end of method DelLL::DelLL  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelLL
//      Method:  ~DelLL
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelLL::~DelLL ()
{
}  // ~~~~~  end of method DelLL::~DelLL  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelLL
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelLL&
DelLL::operator = ( const DelLL &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelLL::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DelLL::LoadEvent
//  Description:  
// ===========================================================================
bool DelLL::LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
  LepMode = -99;
  DelEvent::LoadEvent(branchEvent,  branchJet, 
      branchGenJet,   branchCAJet, branchElectron,  branchMuon, 
      branchPhoton,  branchMet, branchHt,  branchParticle);
  LepMode = CheckLeptonMode();
  return true;
  
}       // -----  end of function DelLL::LoadEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  DelLL::CheckFlag
//  Description:  
// ===========================================================================
bool DelLL::CheckFlag(std::string flag) const
{
  if (flag == "EleEle")      return LepMode == 1;
  if (flag == "MuMu")      return LepMode == 2;
  if (flag == "TauTau")      return LepMode == 3;
  if (flag == "NvNv")      return LepMode == 0;
  if (flag == "Lep")      return LepMode == -99;
  
  return false;
}       // -----  end of function DelLL::CheckFlag  -----

// ===  FUNCTION  ============================================================
//         Name:  DelLL::CheckLeptonMode
//  Description:  Check whether these are electron, muon, taus, or neutrinos
// ===========================================================================
int DelLL::CheckLeptonMode()
{
  // Assuming the status 3 electron directly from Z
  // Delphes has broken mother links, can't track back to mother Z
  std::vector<int> vEle;
  std::vector<int> vMuon;
  std::vector<int> vTau;
  std::vector<int> vNv;

  int GenSize = vGenParticle.size();
  for (int i = 0; i < GenSize; ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (p.Status != 3 || p.M1 > GenSize || p.M2 > GenSize )  continue;
    if (std::fabs(p.PID) == 11)
      vEle.push_back(p.PID);
    if (std::fabs(p.PID) == 13)
      vMuon.push_back(p.PID);
    if (std::fabs(p.PID) == 15)
      vTau.push_back(p.PID);
    if (std::fabs(p.PID) == 12 || 
        std::fabs(p.PID) == 14 ||
        std::fabs(p.PID) == 16 )
      vNv.push_back(p.PID);
  }
  
   
  assert(vEle.size() + vMuon.size() + vTau.size() + vNv.size() == 2);

  if (vEle.size() == 2) return 1;
  if (vMuon.size() == 2) return 2;
  if (vTau.size() == 2) return 3;
  if (vNv.size() == 2) return 0;

  //PrintGen();
  return -99;
}       // -----  end of function DelLL::CheckLeptonMode  -----


// ===  FUNCTION  ============================================================
//         Name:  DelLL::PrintGen
//  Description:  Print out the GenParticle list
// ===========================================================================
bool DelLL::PrintGen() const
{
  std::cout << "===========================================" << std::endl;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    std::cout << " i " << i 
      << std::setw(5) << " PID "    << std::setw(3) << p.PID
      << std::setw(5) << " Status "    << std::setw(3) << p.Status
      << std::setw(5) << " M1 "    << std::setw(3) << p.M1
      << std::setw(5) << " M2 "    << std::setw(3) << p.M2
      << std::setw(5) << " Px "     << std::setw(8) << p.Px
      << std::setw(5) << " Py "     << std::setw(8) << p.Py
      << std::setw(5) << " Pz "     << std::setw(8) << p.Pz
      << std::setw(5) << " E   "    << std::setw(8) << p.E
      << std::setw(5) << " PT  "    << std::setw(8) << p.PT
      << std::setw(5) << " Phi "    << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "    << std::setw(8) << p.Eta
      << std::endl;
  }

  std::cout <<  " Printing Reconstructed GenJet: " << std::endl;
  for (int i = 0; i < vGenJet.size(); ++i)
  {
    Jet p = vGenJet.at(i);
    std::cout
      << std::setw(5) << " i:  "   << std::setw(3) << i
      << std::setw(5) << " Px "    << std::setw(8) << p.P4().Px()
      << std::setw(5) << " Py "    << std::setw(8) << p.P4().Py()
      << std::setw(5) << " Pz "    << std::setw(8) << p.P4().Pz()
      << std::setw(5) << " Mass  " << std::setw(8) << p.Mass
      << std::setw(5) << " PT  "   << std::setw(8) << p.PT
      << std::setw(5) << " Phi "   << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "   << std::setw(8) << p.Eta
      << std::endl;
  }

}       // -----  end of function DelLL::PrintGen  -----

