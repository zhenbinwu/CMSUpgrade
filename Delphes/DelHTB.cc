// ===========================================================================
// 
//       Filename:  DelHTB.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/11/2013 12:18:08 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelHTB.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  DelHTB
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelHTB::DelHTB (double Eta, double Pt) : DelZJJ(false, Eta, Pt)
{

}  // ~~~~~  end of method DelHTB::DelHTB  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  DelHTB
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelHTB::DelHTB ( const DelHTB &other ) : DelZJJ(other)
{
}  // ~~~~~  end of method DelHTB::DelHTB  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  ~DelHTB
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelHTB::~DelHTB ()
{
}  // ~~~~~  end of method DelHTB::~DelHTB  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelHTB&
DelHTB::operator = ( const DelHTB &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelHTB::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DelHTB::CheckFlag
//  Description:  
// ===========================================================================
bool DelHTB::CheckFlag(const std::string flag)
{

  assert(!FakingZvv);
  if (flag == "H") return GenBonson() == 1;
  if (flag == "W") return GenBonson() == 2;
  if (flag == "Z") return GenBonson() == 3;
  if (flag == "Photon")  return GenBonson() == 0;
  if (flag == "MetDiMuon") 
  {
    if ( GenBonson() != 3) return false;
    return DiMuonMet();
  }
  if (flag == "MetDiEle") 
  {
    if ( GenBonson() != 3) return false;
    return DiEleMet();
  }
  return false;
}       // -----  end of function DelHTB::CheckFlag  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::GenBonson
//  Description:  Function to check the genparticles for boson
//                Higgs  -- 1
//                W      -- 2
//                Z      -- 3
//                Photon -- 0
//  
// ===========================================================================
int DelHTB::GenBonson() const
{
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (p.PID == 25) return 1; //Higgs 
    if (p.PID == 24) return 2; //W  
    if (p.PID == 23) return 3;  // Z
  }
  // if can't find W, Z and H, this should be photon+jets
  return 0;
}       // -----  end of function DelHTB::GenBonson  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::PrintGen
//  Description:  Print out the GenParticle list
// ===========================================================================
bool DelHTB::PrintGen() const
{
  std::cout << "===========================================" << std::endl;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    std::cout << " i " << i  << " PID " << p.PID << " M1 " << p.M1 << " M2 " << p.M2<< std::endl;
  }
}       // -----  end of function DelHTB::PrintGen  -----
