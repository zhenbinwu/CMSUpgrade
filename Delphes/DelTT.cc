// ===========================================================================
// 
//       Filename:  DelTT.cc
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

#include "DelTT.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelTT
//      Method:  DelTT
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelTT::DelTT (double Eta, double Pt) : DelEvent(Eta, Pt)
{
}  // ~~~~~  end of method DelTT::DelTT  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelTT
//      Method:  DelTT
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelTT::DelTT ( const DelTT &other ) : DelEvent(other)
{
}  // ~~~~~  end of method DelTT::DelTT  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelTT
//      Method:  ~DelTT
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelTT::~DelTT ()
{
}  // ~~~~~  end of method DelTT::~DelTT  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelTT
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelTT&
DelTT::operator = ( const DelTT &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelTT::operator =  (assignment operator)  ~~~

// ===  FUNCTION  ============================================================
//         Name:  DelTT::GenLeps
//  Description:  
// ===========================================================================
int DelTT::GenLeps() const
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

}       // -----  end of function DelTT::GenLeps  -----

// ===  FUNCTION  ============================================================
//         Name:  DelTT::CheckFlag
//  Description:  /* cursor */
// ===========================================================================
bool DelTT::CheckFlag(std::string flag)
{
  if (flag == "TTFL") return GenLeps() == 2;
  if (flag == "TTSL") return GenLeps() == 1;
}       // -----  end of function DelTT::CheckFlag  -----


// ===  FUNCTION  ============================================================
//         Name:  DelTT::LoadEvent
//  Description:  
// ===========================================================================
bool DelTT::LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
    return DelEvent::LoadEvent(branchEvent,  branchJet, 
         branchGenJet,   branchCAJet, branchElectron,  branchMuon, 
         branchPhoton,  branchMet, branchHt,  branchParticle);
  
}       // -----  end of function DelTT::LoadEvent  -----
