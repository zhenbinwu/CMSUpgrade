// ===========================================================================
// 
//       Filename:  DelAna.cc
// 
//    Description: , G
// 
//        Version:  1.0
//        Created:  06/10/2013 10:12:09 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelAna.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelAna
//      Method:  DelAna
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelAna::DelAna (DelEvent *DE)
{
  DEV = DE;

  vEvent = &DEV->vEvent;
  vGenParticle = &DEV->vGenParticle;
  vPhoton = &DEV->vPhoton;
  vMuon = &DEV->vMuon;
  vElectron = &DEV->vElectron;
  vJet = &DEV->vJet;
  vMissingET = &DEV->vMissingET;

  PUCorMet = &DE->PUCorMet;
  MHT = &DE->MHT;
  HT = &DE->HT;

}  // ~~~~~  end of method DelAna::DelAna  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelAna
//      Method:  DelAna
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelAna::DelAna ( const DelAna &other )
{
}  // ~~~~~  end of method DelAna::DelAna  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelAna
//      Method:  ~DelAna
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelAna::~DelAna ()
{
}  // ~~~~~  end of method DelAna::~DelAna  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelAna
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelAna&
DelAna::operator = ( const DelAna &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelAna::operator =  (assignment operator)  ~~~

// ===  FUNCTION  ============================================================
//         Name:  DelAna::CheckFlag
//  Description:  
// ===========================================================================
bool DelAna::CheckFlag(std::string name)
{
  CurrentTag = name;
  if (name == "Default") return true;
  return DEV->CheckFlag(name);
}       // -----  end of function DelAna::CheckFlag  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::Clear
//  Description:  
// ===========================================================================
bool DelAna::Clear()
{
  J1 = 0;
  J2 = 0;
  J3 = 0;
}       // -----  end of function DelAna::Clear  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::GetBasic
//  Description:  Get the basic variable per event
// ===========================================================================
int DelAna::GetBasic()
{
  Met = PUCorMet->Mod();
  if (vJet->size() > 0) J1 = &vJet->at(0);
  if (vJet->size() > 1) 
  {
    J2 = &vJet->at(1);
    Mjj = (J1->P4() + J2->P4()).M();
  }
  if (vJet->size() > 2) J3 = &vJet->at(2);
  return 1;
}       // -----  end of function DelAna::GetBasic  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::DiLepton
//  Description:  
// ===========================================================================
bool DelAna::MetDiLepton() 
{

  if (CurrentTag != "MetDiMuon" && CurrentTag != "MetDiEle")
    return 0;
  // Initial values
  // The Qt of Z/gramma*
  TLorentzVector Qt(0, 0, 0, 0);
  // Ut , sum of other objects 
  TLorentzVector Ut(0, 0, 0, 0);

  Mll = -999.;
  UParallel = -999.;
  UTransverse = -999.;
  QT = -999.;
  MetScale = -999.;

  if (vMuon->size() == 2 && vElectron->size() == 0)
  {
    for (int i = 0; i < vMuon->size(); ++i)
    {
      Qt += vMuon->at(i).P4();
    }
  } else if (vMuon->size() == 0 && vElectron->size() == 2) {
    
    for (int i = 0; i < vElectron->size(); ++i)
    {
      Qt += vElectron->at(i).P4();
    }

  }

  if (Qt.Pt() < 50) return false;

  //Loop over the jet correction
  if (vJet->size()> 0)
    for (int i = 0; i < vJet->size(); ++i)
    {
      Ut += vJet->at(i).P4();
    }

  //Loop over the photon correction
  if (vPhoton->size() > 0)
    for (int i = 0; i < vPhoton->size(); ++i)
    {
      Ut += vPhoton->at(i).P4();
    }

  //Double check the system : Ut+Qt+Met = 0
  TVector3 MHT(0, 0, 0);
  TVector3 MET(PUCorMet->Px(), PUCorMet->Py(), 0);
  MHT = Qt.Vect() + Ut.Vect() + MET;
  assert(MHT.Pt() < 0.0003);

  // Testing rotation:
  double Dphi =  Ut.DeltaPhi(Qt);
  UTransverse = Ut.Pt() * std::sin(Dphi);
  UParallel = Ut.Pt() * std::cos(Dphi);
  QT = Qt.Pt();
  UT = Ut.Pt();
  Mll = Qt.M();
  MetScale = -1 * UParallel / Qt.Pt();
  return true;
}       // -----  end of function DelAna::DiLepton  -----

