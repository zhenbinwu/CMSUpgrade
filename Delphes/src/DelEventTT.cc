// ===========================================================================
// 
//       Filename:  DelEventTT.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/07/2013 09:16:49 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "DelEventTT.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventTT
//      Method:  DelEventTT
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEventTT::DelEventTT (double Eta, double Pt) : DelEvent(Eta, Pt)
{
}  // ~~~~~  end of method DelEventTT::DelEventTT  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventTT
//      Method:  DelEventTT
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEventTT::DelEventTT ( const DelEventTT &other ) : DelEvent(other)
{
}  // ~~~~~  end of method DelEventTT::DelEventTT  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventTT
//      Method:  ~DelEventTT
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEventTT::~DelEventTT ()
{
}  // ~~~~~  end of method DelEventTT::~DelEventTT  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventTT
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelEventTT&
DelEventTT::operator = ( const DelEventTT &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelEventTT::operator =  (assignment operator)  ~~~

// ===  FUNCTION  ============================================================
//         Name:  DelEventTT::GenLeps
//  Description:  
// ===========================================================================
int DelEventTT::GenLeps() const
{
  std::list<int> VLep;

  int GenSize = vGenParticle.size();
  for (int i = 0; i < GenSize; ++i)
  {
    GenParticle p = vGenParticle.at(i);

    if  ( p.M1 < GenSize && p.M2 < GenSize &&
        (std::abs(p.PID) == 11 || std::abs(p.PID) == 13 || std::abs(p.PID) == 15))
    {
      VLep.push_back(i);
      // Search another lepton afterward
      for (int j = i; j < vGenParticle.size(); ++j)
      {
        GenParticle p2 = vGenParticle.at(j);
        if  ( p2.M1 < GenSize && p2.M2 < GenSize &&
            (std::abs(p2.PID) == 11 || std::abs(p2.PID) == 13 || std::abs(p2.PID) == 15))
        {
          if (p2.P4() != p.P4() && p.P4().DeltaR(p2.P4()) > 0.8)
          {
            VLep.push_back(j);
            break;
          }
        }
      }
      break;
    }
  }

  return VLep.size();

}       // -----  end of function DelEventTT::GenLeps  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEventTT::CheckFlag
//  Description:  /* cursor */
// ===========================================================================
bool DelEventTT::CheckFlag(std::string flag)
{
  if (flag == "TTFL") return GenLeps() == 2;
  if (flag == "TTSL") return GenLeps() == 1;
  if (flag == "TTHD") return GenLeps() == 0;
}       // -----  end of function DelEventTT::CheckFlag  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEventTT::LoadEvent
//  Description:  
// ===========================================================================
bool DelEventTT::LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
  DelEvent::LoadEvent(branchEvent,  branchJet, 
      branchGenJet,   branchCAJet, branchElectron,  branchMuon, 
      branchPhoton,  branchMet, branchHt,  branchParticle);
  return true;
  
}       // -----  end of function DelEventTT::LoadEvent  -----
