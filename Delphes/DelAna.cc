// ===========================================================================
// 
//       Filename:  DelAna.cc
// 
//    Description:  After we reconstruct the DelEvent, we use DelAna for
//    analysis purpose. Construction of more complex variables, study of
//    different objects should take place in here.
// 
//        Version:  1.0
//        Created:  06/10/2013 10:12:09 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "DelAna.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelAna
//      Method:  DelAna
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelAna::DelAna (DelEvent *DE, std::string pileup_)
{
  DEV = DE;
  PileUp = pileup_;

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

  Weight = 1.0;
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
  if (name.find("Sys") == std::string::npos)
    Met   = PUCorMet->Mod();
  else
    Met   = SysMet.Mod();
  if (name == "Default") return true;
  return DEV->CheckFlag(name);
}       // -----  end of function DelAna::CheckFlag  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::RunPerEvent
//  Description:  This function sums up all the things that are common for
//  each DelCut process. This could speed up the code for those samples with
//  many DelCut instances.
// ===========================================================================
bool DelAna::RunPerEvent()
{
  Clear();
  GetBasic();
}       // -----  end of function DelAna::RunPerEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::Clear
//  Description:  
// ===========================================================================
bool DelAna::Clear()
{
  J1 = 0;
  J2 = 0;
  J3 = 0;
  Weight = 1.0;
  RawMet.Set(0.0, 0.0);
  SysMet.Set(0.0, 0.0);
  RHT = 0.0;
  Met = -999;
  METAsys = -99;
  DelHT = -999.;

  GenZvv.SetPxPyPzE(0, 0, 0, 0);
}       // -----  end of function DelAna::Clear  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::GetBasic
//  Description:  Get the basic variable per event
// ===========================================================================
int DelAna::GetBasic()
{
  DelHT = DEV->DelHT;
  Met   = PUCorMet->Mod();
  RawMet.SetMagPhi(vMissingET->at(0).MET, vMissingET->at(0).Phi);
  METAsys = std::fabs(Met - RawMet.Mod())/(Met + RawMet.Mod());
  SysMet = SystemMet();

  if (vJet->size() > 0) J1 = &vJet->at(0);
  if (vJet->size() > 1) 
  {
    J2 = &vJet->at(1);
    Mjj = (J1->P4() + J2->P4()).M();
  }
  if (vJet->size() > 2) J3 = &vJet->at(2);
  Weight = vEvent->at(0).Weight;

  for (int i = 0; i < vJet->size(); ++i)
  {
    RHT += vJet->at(i).P4().Mag();
  }
  for (int i = 0; i < vElectron->size(); ++i)
  {
    RHT += vElectron->at(i).P4().Mag();
  }
  for (int i = 0; i < vMuon->size(); ++i)
  {
    RHT += vMuon->at(i).P4().Mag();
  }
  for (int i = 0; i < vPhoton->size(); ++i)
  {
    RHT += vPhoton->at(i).P4().Mag();
  }

  CalGenZvv();
  FindMatchedJet();
  return 1;
}       // -----  end of function DelAna::GetBasic  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::DiLepton
//  Description:  
// ===========================================================================
bool DelAna::MetDiLepton() 
{

  if (CurrentTag != "MetDiMuon" && CurrentTag != "MetDiEle")
    return false;
  // Initial values
  // The Qt of Z/gramma*
  TLorentzVector Qt(0, 0, 0, 0);
  // Ut , sum of other objects 
  TLorentzVector Ut(0, 0, 0, 0);

  Mll         = -999.;
  UParallel   = -999.;
  UTransverse = -999.;
  QT          = -999.;
  MetScale    = -999.;
  UtNJets     = 0;
  UtNPhotons  = 0;
  UTJetsPT    = 0;
  UTPhotonsPT = 0;

  int sign=1;
  if (vMuon->size() == 2 && vElectron->size() == 0)
  {
    for (int i = 0; i < vMuon->size(); ++i)
    {
      Qt += vMuon->at(i).P4();
      sign *= vMuon->at(i).Charge;
    }
  } else if (vMuon->size() == 0 && vElectron->size() == 2) {
    for (int i = 0; i < vElectron->size(); ++i)
    {
      Qt += vElectron->at(i).P4();
      sign *= vElectron->at(i).Charge;
    }
  }


  //Loop over the jet correction
  if (vJet->size()> 0)
    for (int i = 0; i < vJet->size(); ++i)
    {
      if(std::fabs(vJet->at(i).Eta) > DEV->JetEtaCut 
          || vJet->at(i).PT < DEV->JetPtCut)
        continue;
      Ut += vJet->at(i).P4();
      UtNJets++;
      UTJetsPT += vJet->at(i).PT;
    }

  //Loop over the photon correction
  if (vPhoton->size() > 0)
    for (int i = 0; i < vPhoton->size(); ++i)
    {
      if (std::fabs(vPhoton->at(i).Eta) > DEV->JetEtaCut)
        continue;
      Ut += vPhoton->at(i).P4();
      UtNPhotons++;
      UTPhotonsPT += vPhoton->at(i).PT;
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


// ===  FUNCTION  ============================================================
//         Name:  DelAna::METMHTAsys
//  Description:  For HTBin sample, we found that there are jets missing in
//  the event, which cause much larger MHT than expected. For detail, check my
//  elog:  http://hep.baylor.edu/elog/benwu/943
//  In order to supress this problem, we calculate the METMHTAsys by
//  |MHT-MET|/(MHT+MET) and choose the cut value for different PU case
//  as from elog:http://hep.baylor.edu/elog/benwu/947 
// ===========================================================================
bool DelAna::METMHTAsys() const
{
  double AsysCut = -99;
  if (PileUp == "NoPileUp") AsysCut = 0.2;
  if (PileUp == "50PileUp") AsysCut = 0.3;
  if (PileUp == "140PileUp") AsysCut = 0.5;
  assert(AsysCut != -99.);
  return METAsys < AsysCut;
}       // -----  end of function DelAna::METMHTAsys  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::SystemMet
//  Description:  Met from control region, which faking leptons as neutrinos
// ===========================================================================
TVector2 DelAna::SystemMet() const
{
  
  TLorentzVector temp = *MHT;

  for (int i = 0; i < vElectron->size(); ++i)
  {
    temp -= vElectron->at(i).P4();
  }

  for (int i = 0; i < vMuon->size(); ++i)
  {
    temp -= vMuon->at(i).P4();
  }
  
  double met_x = -temp.Px();
  double met_y = -temp.Py();

  TVector2 NewMet(met_x, met_y);

  return NewMet;
}       // -----  end of function DelAna::SystemMet  -----


// ===  FUNCTION  ============================================================
//         Name:  DelAna::GenZvv
//  Description:  
// ===========================================================================
bool DelAna::CalGenZvv()
{
  int GenSize = vGenParticle->size();
  std::vector<int> vNv;
  for (int i = 0; i < vGenParticle->size(); ++i)
  {
    GenParticle p = vGenParticle->at(i);
    if (p.Status != 3 || p.M1 > GenSize || p.M2 > GenSize )  continue;
    if (std::fabs(p.PID) == 12 || std::fabs(p.PID) == 14 ||
        std::fabs(p.PID) == 16 )
    vNv.push_back(i);
  }
  
  if (vNv.size() == 2)
  {

    for (int i = 0; i < vNv.size(); ++i)
    {
      GenZvv += vGenParticle->at(vNv.at(i)).P4();
    }
  }

  return true;
}       // -----  end of function DelAna::GenZvv  -----

// ===  FUNCTION  ============================================================
//         Name:  DelAna::FindMatchedJet
//  Description:  
// ===========================================================================
bool DelAna::FindMatchedJet()
{
  MatchedJet.clear();
  JetPtScale.clear();

  //Get the default GenJet in Delphes
  std::vector<TLorentzVector> vGen;
  for (int i = 0; i < DEV->vGenJet.size(); ++i)
  {
    Jet j = DEV->vGenJet.at(i);
    vGen.push_back(j.P4());
  }

  for (int i = 0; i < vJet->size(); ++i)
  {
    Jet jet = vJet->at(i);
    
    for (int j = 0; j < vGen.size(); ++j)
    {
      if (jet.P4().DeltaR(vGen.at(j)) < 0.4)
      {
        MatchedJet.push_back(jet.P4());
        JetPtScale.push_back(jet.PT/vGen.at(j).Pt());
        vGen.erase(vGen.begin()+j);
        break;
      }
    }
  }

  //std::cout << " vGEn size " << vGen.size() << " vGenJet " << DEV->vGenJet.size() << " MatchedJet "<< MatchedJet.size()<< std::endl;
  
  return true;
}       // -----  end of function DelAna::FindMatchedJet  -----
