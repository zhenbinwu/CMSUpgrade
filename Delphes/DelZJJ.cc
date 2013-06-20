// ===========================================================================
// 
//       Filename:  DelZJJ.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/05/2013 05:05:26 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelZJJ.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelZJJ
//      Method:  DelZJJ
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelZJJ::DelZJJ (bool Faking, double Eta, double Pt) : DelEvent(Eta, Pt)
{
  FakingZvv = Faking;
}  // ~~~~~  end of method DelZJJ::DelZJJ  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelZJJ
//      Method:  DelZJJ
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelZJJ::DelZJJ ( const DelZJJ &other ) : DelEvent(other)
{
}  // ~~~~~  end of method DelZJJ::DelZJJ  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelZJJ
//      Method:  ~DelZJJ
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelZJJ::~DelZJJ ()
{
}  // ~~~~~  end of method DelZJJ::~DelZJJ  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelZJJ
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelZJJ&
DelZJJ::operator = ( const DelZJJ &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelZJJ::operator =  (assignment operator)  ~~~

// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::LoadEvent
//  Description:  virtual fucntion for loading the event whether faking the
//  ZLL sample
// ===========================================================================
bool DelZJJ::LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
  if (FakingZvv)
    return LoadZLLEvent(branchEvent,  branchJet, 
         branchGenJet,   branchCAJet, branchElectron,  branchMuon, 
         branchPhoton,  branchMet, branchHt,  branchParticle);
  else
  {
    // This is a temp solution for the MEt study, don't apply the preselection
    DelEvent::LoadEvent(branchEvent,  branchJet, 
        branchGenJet,   branchCAJet, branchElectron,  branchMuon, 
        branchPhoton,  branchMet, branchHt,  branchParticle);
    return  true;
  }

}       // -----  end of function DelZJJ::LoadEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::LoadZLLEvent
//  Description:  Faking the Zvv sample from the ZLL sample
// ===========================================================================
bool DelZJJ::LoadZLLEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
  //std::cout << " Get branch? " << branchJet->GetEntries()<< std::endl;
  //First, clean out the local event
  CleanEvent();
  
//----------------------------------------------------------------------------
//  For Faking the Zvv samples
//----------------------------------------------------------------------------
  LoadGenParticle(branchParticle);
  if (!CheckZ()) return false;
  
  //Now, start to load up the event
  DelEvent::LoadEvent(branchEvent);
  CalPUCorMet(branchJet, branchElectron, branchMuon, branchPhoton);
  LoadRawMet(branchMet);
  LoadPhoton(branchPhoton);

  //Get the correction for the ZLL samples
  PUCorMet = ZLLMet(branchParticle, branchElectron, branchMuon, branchJet);

  LoadZLLElectron(branchElectron);
  LoadZLLMuon(branchMuon);
  LoadJet(branchJet);
  LoadGenJet(branchGenJet);
  LoadCAJet(branchCAJet);

  //Whether this event is preselected?
  return PreSelected();
}       // -----  end of function DelZJJ::LoadZLLEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::CheckZ
//  Description:  
// ===========================================================================
bool DelZJJ::CheckZ()
{
  // Actually this function should name differently now
  // I loop over the particle list to find the leptons 
  // The mother tracing method didn't work because Delphes cuts away
  // the particles list to save the disk

  VZLep.clear();
  ZPT = -999.;

  //----------------------------------------------------------------------------
  //  // Find Z and take the next two as the lepton  
  //----------------------------------------------------------------------------
  //
  int Zindex = 0;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    if (vGenParticle.at(i).PID == 23)
    {
      Zindex = i;
      break;
    } 
  }

  //Loop over again to find Z decay products
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (p.M1 == Zindex || p.M2 == Zindex)
    {
      // Only Leptons
      if  (std::abs(p.PID) == 11 || std::abs(p.PID) == 13 || std::abs(p.PID) == 15)
        VZLep.push_back(i);
    }
  }

  int sign=1;
  // Double check the parents of the Z decay products
  for(std::list<int>::iterator it=VZLep.begin();
      it!=VZLep.end(); it++)
  {
    GenParticle* p = &(vGenParticle.at(*it));
    sign *= p->PID;
    if (p->M1 != -1)
    {
      GenParticle* Mp = &(vGenParticle.at(p->M1));
      if (Mp->PID == 23)
        continue;
      else
        std::cout << " Mp 1->PID" <<Mp->PID<< std::endl;
    }
    if (p->M2 != -1)
    {
      GenParticle* Mp = &(vGenParticle.at(p->M2));
      if (Mp->PID == 23)
        if (Mp->PID == 23)
          continue;
        else
          std::cout << " Mp 2->PID" <<Mp->PID<< std::endl;
    }
    std::cout << "some thing wrong!"  << std::endl;
  }

  if (VZLep.size() > 0)
  {
    if (sign > 0)
    {
      std::cout << "Same sign from Z? " << VZLep.size() << std::endl;
      return false;
    }
  }

  if (VZLep.size() == 2) 
  {
    TLorentzVector DY;
    double metx=0.0;
    double mety=0.0;
    for(std::list<int>::iterator it=VZLep.begin();
        it!=VZLep.end(); it++)
    {
      GenParticle* p = &(vGenParticle.at(*it));
      DY += p->P4();
      metx += p->P4().Pt()*cos(p->Phi);
      mety +=  p->P4().Pt()*sin(p->Phi);
    }
    TVector2 GenMet(metx, mety);
    ZPT = DY.Pt();
    return true;
  }

  //----------------------------------------------------------------------------
  //// In case of DY ... the first two leptons in the order  
  //----------------------------------------------------------------------------
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle* p = &(vGenParticle.at(i));

    if  (std::abs(p->PID) == 11 || std::abs(p->PID) == 13 || std::abs(p->PID) == 15)
    {
      VZLep.push_back(i);
      // Search another lepton afterward
      for (int j = i; j < vGenParticle.size(); ++j)
      {
        GenParticle* p2 = &(vGenParticle.at(j));
        if  (std::abs(p2->PID) == 11 || std::abs(p2->PID) == 13 || std::abs(p2->PID) == 15)
        {
          if (p2->P4() != p->P4() && p->PID * p2->PID < 0)
          {
            VZLep.push_back(j);
            break;
          }
        }
      }
      break;
    }
  }

  // Double check the DY Leptons
  // Problem: Can't identify the selected lepton parent since there are cut
  // off on the GenParticle list
  // Solution: Just check the DY Mass

  TLorentzVector DY;
  sign = 1;
  double metx=0.0;
  double mety=0.0;
  for(std::list<int>::iterator it=VZLep.begin();
      it!=VZLep.end(); it++)
  {
    GenParticle* p = &(vGenParticle.at(*it));
    DY   += p->P4();
    metx += p->P4().Pt()*cos(p->Phi);
    mety += p->P4().Pt()*sin(p->Phi);
    sign *= p->PID;
  }

  TVector2 GenMet(metx, mety);
  ZPT = DY.Pt();

  if (VZLep.size()>0)
    if (sign > 0) return false;

  if (DY.M() < 60 || DY.M() > 120) return false;

  if (VZLep.size() == 2) return true;

  if (VZLep.size() != 2)  return false;
}       // -----  end of function DelZJJ::CheckZ  -----

// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::ZLLMet
//  Description:  Calculating the met from ZLL as the ZNN sample
// ===========================================================================
TVector2 DelZJJ::ZLLMet(TClonesArray *branchParticle, TClonesArray *branchElectron, 
    TClonesArray *branchMuon, TClonesArray *branchJet)
{
  // First get the PU corrected Met in the event
  TVector2 oldMet = PUCorMet;

  //----------------------------------------------------------------------------
  //  Looping the events for Z decay products 
  //----------------------------------------------------------------------------
  GenParticle *particle = 0;
  std::map<int, GenParticle*> EleGen; 
  std::map<int, GenParticle*> MuonGen; 

  //----------------------------------------------------------------------------
  //  Adding electron
  //----------------------------------------------------------------------------
  for (int i = 0; i < branchElectron->GetEntries(); ++i)
  {
    Electron* ele = (Electron*) branchElectron->At(i);
    particle = (GenParticle*) ele->Particle.GetObject();
    EleGen[i] = particle;
  }

  //----------------------------------------------------------------------------
  //  Adding muon
  //----------------------------------------------------------------------------
  for (int i = 0; i < branchMuon->GetEntries(); ++i)
  {
    Muon* muon = (Muon*) branchMuon->At(i);
    particle = (GenParticle*) muon->Particle.GetObject();
    MuonGen[i] = particle;
  }

  oldMet += ZLLLep(EleGen, MuonGen, branchParticle, branchElectron, branchMuon, branchJet);
  return oldMet;
}       // -----  end of function DelZJJ::ZLLMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::ZLLLep
//  Description:  Looking up the system for different number of leptons
// ===========================================================================
TVector2 DelZJJ::ZLLLep(std::map<int, GenParticle*> EleGen, 
    std::map<int, GenParticle*> MuonGen,  TClonesArray *branchParticle,
    TClonesArray *branchElectron, TClonesArray *branchMuon, TClonesArray *branchJet)
{


  TVector2 addmet(0, 0);
  //Check the LGen length is 2 lepton
  if (VZLep.size() != 2) return addmet;

  //Clear out the temp object
  MatchedEle.clear();
  MatchedMuon.clear();
  RMjet.clear();
  CRJet.clear();

  // Create a map to check whether the lepton has been identified as Ele or
  // Muon
  std::map<int, int> GenStat;
  for(std::list<int>::iterator it=VZLep.begin();
      it!=VZLep.end(); it++)
  {
    GenStat[*it] = 0;
  }


  // No way to know the index of particle from ele or muon
  // This is getting ugly
  //----------------------------------------------------------------------------
  //  Matching to the electron in the event
  //----------------------------------------------------------------------------
  for(std::map<int, GenParticle*>::iterator it=EleGen.begin();
      it!=EleGen.end(); it++)
  {
    if (it->second != 0)
    {
      for(std::map<int, int>::iterator git=GenStat.begin();
          git!=GenStat.end(); git++)
      {
        if (git->second == 1) continue;
        GenParticle* p = (GenParticle*)branchParticle->At(git->first);
        if (it->second->P4() == p->P4() || it->second->P4().DeltaR(p->P4()) < 0.4)
        {
          git->second += 1;
          Electron* ele = (Electron*) branchElectron->At(it->first);
          addmet += TVector2(ele->P4().Pt()*cos(ele->Phi), ele->P4().Pt()*sin(ele->Phi));
          MatchedEle.insert(it->first);
        }
      }
    } else { // For High PU, no real Gen was referred 
      Electron* ele = (Electron*) branchElectron->At(it->first);
      for(std::map<int, int>::iterator git=GenStat.begin();
          git!=GenStat.end(); git++)
      {
        if (git->second == 1) continue;
        GenParticle* p = (GenParticle*)branchParticle->At(git->first);

        if (ele->P4().DeltaR(p->P4()) < 0.4)
        {
          git->second += 1;
          addmet += TVector2(ele->P4().Pt()*cos(ele->Phi), ele->P4().Pt()*sin(ele->Phi));
          MatchedEle.insert(it->first);
        }
      }
    }
  }

  //----------------------------------------------------------------------------
  //  Matching to the muon in the event
  //----------------------------------------------------------------------------
  for(std::map<int, GenParticle*>::iterator it=MuonGen.begin();
      it!=MuonGen.end(); it++)
  {
    if (it->second != 0 ) //In case the ref to GenParticle is real 
    {
      for(std::map<int, int>::iterator git=GenStat.begin();
          git!=GenStat.end(); git++)
      {
        if (git->second == 1) continue;
        GenParticle* p = (GenParticle*)branchParticle->At(git->first);
        if (it->second->P4() == p->P4() || it->second->P4().DeltaR(p->P4()) < 0.4)
        {
          git->second += 1;
          Muon* muon = (Muon*) branchMuon->At(it->first);
          addmet += TVector2(muon->P4().Pt()*cos(muon->Phi), muon->P4().Pt()*sin(muon->Phi));
          MatchedMuon.insert(it->first);
        }
      }
    } else { // For high PU, not all the ref are real 
      Muon* muon = (Muon*) branchMuon->At(it->first);
      for(std::map<int, int>::iterator git=GenStat.begin();
          git!=GenStat.end(); git++)
      {
        if (git->second == 1) continue;
        GenParticle* p = (GenParticle*)branchParticle->At(git->first);
        if (muon->P4().DeltaR(p->P4()) < 0.4)
        {
          git->second += 1;
          addmet += TVector2(muon->P4().Pt()*cos(muon->Phi), muon->P4().Pt()*sin(muon->Phi));
          MatchedMuon.insert(it->first);
        }
      }
    }
  }

  //----------------------------------------------------------------------------
  //  Matching to the jet in the event
  //----------------------------------------------------------------------------
  for(std::map<int, int>::iterator git=GenStat.begin();
      git!=GenStat.end(); git++)
  {
    if (git->second == 1) continue;
    GenParticle* p = (GenParticle*)branchParticle->At(git->first);
    for (int i = 0; i < branchJet->GetEntries(); ++i)
    {
      Jet* jet = (Jet*) branchJet->At(i);
      if (jet->P4().DeltaR(p->P4()) < 0.4)
      {
        if (jet->PT < p->PT)
        {
          git->second +=1;
          RMjet.insert(i);
          addmet += TVector2(jet->P4().Pt()*cos(jet->Phi), jet->P4().Pt()*sin(jet->Phi));
        }
        else
        {
          addmet += TVector2(p->P4().Pt()*cos(p->Phi), p->P4().Pt()*sin(p->Phi));
          CRJet[i] = p->P4();
        }
      }
    }
  }

  //----------------------------------------------------------------------------
  //  Still no found?? consider it as the misidentify in the system, which is
  //  included in the PU corrected met
  //----------------------------------------------------------------------------
  /*
   *for(std::map<int, int>::iterator git=GenStat.begin();
   *    git!=GenStat.end(); git++)
   *{
   *  if (git->second == 0);
   *}
   */
  return addmet;
}       // -----  end of function DelZJJ::ZLL1Lep  -----




// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::LoadElectron
//  Description:  Functions to load the electron
// ===========================================================================
int DelZJJ::LoadZLLElectron(TClonesArray *branchElectron)
{
  
  for (int i = 0; i < branchElectron->GetEntries(); ++i)
  {
    if (MatchedEle.find(i) != MatchedEle.end()) continue;
    Electron* ele = (Electron*)branchElectron->At(i);
    vElectron.push_back(*ele);
  }

  return 1;
} // -----  end of function DelZJJ::LoadElectron  -----



// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::LoadMuon
//  Description:  Functions to load the Muon
// ===========================================================================
int DelZJJ::LoadZLLMuon(TClonesArray *branchMuon)
{
  
  for (int i = 0; i < branchMuon->GetEntries(); ++i)
  {
    if (MatchedMuon.find(i) != MatchedMuon.end()) continue;
    Muon* muon = (Muon*)branchMuon->At(i);
    vMuon.push_back(*muon);
  }

  return 1;
}       // -----  end of function DelZJJ::LoadMuon  -----


// ===  FUNCTION  ============================================================
//         Name:  DelZJJ::CheckFlag
//  Description:  
// ===========================================================================
bool DelZJJ::CheckFlag(std::string flag)
{
  if (flag == "MetDiMuon" && !FakingZvv)
    return DelEvent::DiMuonMet();
  if (flag == "MetDiEle" && !FakingZvv)
    return DelEvent::DiEleMet();
  return true;
}       // -----  end of function DelZJJ::CheckFlag  -----
