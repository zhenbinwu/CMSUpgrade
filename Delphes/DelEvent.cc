// ===========================================================================
// 
//       Filename:  DelEvent.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/03/2013 11:13:31 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "DelEvent.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEvent
//      Method:  DelEvent
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEvent::DelEvent (double Eta, double Pt)
{
  JetEtaCut = Eta;
  JetPtCut = Pt;
}  // ~~~~~  end of method DelEvent::DelEvent  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEvent
//      Method:  DelEvent
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEvent::DelEvent ( const DelEvent &other )
{
}  // ~~~~~  end of method DelEvent::DelEvent  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEvent
//      Method:  ~DelEvent
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEvent::~DelEvent ()
{
}  // ~~~~~  end of method DelEvent::~DelEvent  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEvent
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelEvent&
DelEvent::operator = ( const DelEvent &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelEvent::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadJet
//  Description:  The interface to load the delphes jets, where the magic
//  happens
// ===========================================================================
int DelEvent::LoadJet(TClonesArray *branchJet)
{
  
//----------------------------------------------------------------------------
//  Define the local variables for loading the jet list
//----------------------------------------------------------------------------
  // Building the index of jet sorted by the jet energy
  // Using more eff. forward_list instead of list. It only allows for
  // one-direction searching..
  std::list< std::pair<double, int> > lJetPt; //Sorting jet energy 


  //Collector of jets from new
  std::map<int, Jet*> JetPool; 

//----------------------------------------------------------------------------
//  Loading the branchjet into our local vector of jets
//----------------------------------------------------------------------------
  for (int i = 0; i < branchJet->GetEntries(); ++i)
  {
    // Removing the jets from other requirement first
    if (RMjet.find(i) != RMjet.end()) 
      continue;

    // Now, get the jet at index
    Jet* jet = (Jet*)branchJet->At(i);

    // Apply correction to all the jet
    if (CRJet.find(i) != CRJet.end())
    {
      jet = JetCor(*jet, CRJet[i]);
      JetPool[i]=jet;
    }

    if (!JetSel(*jet)) continue; // Whether this jet pass our 

    lJetPt.push_back(std::make_pair(jet->PT, i));
  }

  lJetPt.sort();
  lJetPt.reverse();

  for(std::list< std::pair<double, int> >::iterator jit=lJetPt.begin();
    jit!=lJetPt.end(); jit++)
  {
    if (JetPool.find(jit->second) != JetPool.end()) // Corrected jet
    {
      vJet.push_back(*JetPool[jit->second]);
    } else {                      // original jet
      Jet* jet = (Jet*)branchJet->At(jit->second); 
      vJet.push_back(*jet);
    }
  }

  // Release the jets
  for(std::map<int, Jet*>::iterator jit=JetPool.begin();
    jit!=JetPool.end(); jit++)
  {
    delete jit->second;
  }

  return 1;

}       // -----  end of function DelEvent::LoadJet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadGenJet
//  Description:  /* cursor */
// ===========================================================================
int DelEvent::LoadGenJet(TClonesArray* branchGenJet)
{
//----------------------------------------------------------------------------
//  Define the local variables for loading the jet list
//----------------------------------------------------------------------------
  // Building the index of jet sorted by the jet energy
  // Using more eff. forward_list instead of list. It only allows for
  // one-direction searching..
  std::list< std::pair<double, int> > lJetPt; //Sorting jet energy 


  for (int i = 0; i < branchGenJet->GetEntries(); ++i)
  {
    
    // Now, get the jet at index
    Jet* jet = (Jet*)branchGenJet->At(i);
    lJetPt.push_back(std::make_pair(jet->PT, i));
  }

  lJetPt.sort();
  lJetPt.reverse();

  for(std::list< std::pair<double, int> >::iterator jit=lJetPt.begin();
    jit!=lJetPt.end(); jit++)
  {
      Jet* jet = (Jet*)branchGenJet->At(jit->second); 
      vGenJet.push_back(*jet);
  }
  return 1;
}       // -----  end of function DelEvent::LoadGenJet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadCAJet
//  Description:  /* cursor */
// ===========================================================================
int DelEvent::LoadCAJet(TClonesArray* branchCAJet)
{
//----------------------------------------------------------------------------
//  Define the local variables for loading the jet list
//----------------------------------------------------------------------------
  // Building the index of jet sorted by the jet energy
  // Using more eff. forward_list instead of list. It only allows for
  // one-direction searching..
  std::list< std::pair<double, int> > lJetPt; //Sorting jet energy 


  for (int i = 0; i < branchCAJet->GetEntries(); ++i)
  {
    // Now, get the jet at index
    Jet* jet = (Jet*)branchCAJet->At(i);
    lJetPt.push_back(std::make_pair(jet->PT, i));
  }

  lJetPt.sort();
  lJetPt.reverse();

  for(std::list< std::pair<double, int> >::iterator jit=lJetPt.begin();
    jit!=lJetPt.end(); jit++)
  {
      Jet* jet = (Jet*)branchCAJet->At(jit->second); 
      vCAJet.push_back(*jet);
  }

  return 1;

}       // -----  end of function DelEvent::LoadCAJet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::JetSel
//  Description:  The selection on the jets in the event
// ===========================================================================
bool DelEvent::JetSel(Jet j)
{
  if(std::fabs(j.Eta) > JetEtaCut || j.PT < JetPtCut)
    return false;
  else
    return true;
}       // -----  end of function DelEvent::JetSel  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::JetCor
//  Description:  
// ===========================================================================
Jet* DelEvent::JetCor(Jet jet, TLorentzVector CorVet)
{
  Jet *mjet = new Jet(jet);

  TLorentzVector mVect  = jet.P4();

  mVect -= CorVet;

  mjet->P4() = mVect;
  mjet->PT  = mVect.Pt();
  mjet->Eta = mVect.Eta();
  mjet->Phi  = mVect.Phi();
  mjet->Mass = mVect.M();

  return mjet;
  
}       // -----  end of function DelEvent::JetCor  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadElectron
//  Description:  Functions to load the electron
// ===========================================================================
int DelEvent::LoadElectron(TClonesArray *branchElectron)
{
  
  for (int i = 0; i < branchElectron->GetEntries(); ++i)
  {
    Electron* ele = (Electron*)branchElectron->At(i);
    vElectron.push_back(*ele);
  }

  return 1;
}       // -----  end of function DelEvent::LoadElectron  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadMuon
//  Description:  Functions to load the Muon
// ===========================================================================
int DelEvent::LoadMuon(TClonesArray *branchMuon)
{
  
  for (int i = 0; i < branchMuon->GetEntries(); ++i)
  {
    Muon* muon = (Muon*)branchMuon->At(i);
    vMuon.push_back(*muon);
  }

  return 1;
}       // -----  end of function DelEvent::LoadMuon  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadPhoton
//  Description:  Functions to load the Photon
// ===========================================================================
int DelEvent::LoadPhoton(TClonesArray *branchPhoton)
{
  
  for (int i = 0; i < branchPhoton->GetEntries(); ++i)
  {
    Photon* pho = (Photon*)branchPhoton->At(i);
    vPhoton.push_back(*pho);
  }

  return 1;
}       // -----  end of function DelEvent::LoadPhoton  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadRawMet
//  Description:  
// ===========================================================================
int DelEvent::LoadRawMet(TClonesArray *branchMet) 
{
  for (int i = 0; i < branchMet->GetEntries(); ++i)
  {
    MissingET* met = (MissingET*)branchMet->At(i);
    vMissingET.push_back(*met);
  }
  return 1;
}       // -----  end of function DelEvent::LoadRawMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadEvent
//  Description:  Functions to load the Event
// ===========================================================================
int DelEvent::LoadEvent(TClonesArray *branchEvent)
{
  
  for (int i = 0; i < branchEvent->GetEntries(); ++i)
  {
    LHEFEvent* evt = (LHEFEvent*)branchEvent->At(i);
    vEvent.push_back(*evt);
  }

  return 1;
}       // -----  end of function DelEvent::LoadEvent  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadGenParticle
//  Description:  Functions to load the GenParticle
// ===========================================================================
int DelEvent::LoadGenParticle(TClonesArray *branchGenParticle)
{
  
  for (int i = 0; i < branchGenParticle->GetEntries(); ++i)
  {
    GenParticle* gen = (GenParticle*)branchGenParticle->At(i);
    vGenParticle.push_back(*gen);
  }

  return 1;
}       // -----  end of function DelEvent::LoadGenParticle  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::PUCorMet
//  Description:  
// ===========================================================================
bool DelEvent::CalPUCorMet(TClonesArray *branchJet, TClonesArray *branchElectron, 
    TClonesArray *branchMuon, TClonesArray *branchPhoton) 
{
  HT = 0.0;

  //Calculate the MHT and Met in the event

  //Loop over the jet correction
  if (branchJet->GetEntries() > 0)
    for (int i = 0; i < branchJet->GetEntries(); ++i)
    {
      Jet* jet = (Jet*)branchJet->At(i);
      if(std::fabs(jet->Eta) > JetEtaCut || jet->PT < JetPtCut)
        continue;
      MHT += jet->P4();
      //std::cout << "MHT"<< MHT.Pt() << std::endl;
      HT += jet->P4().Pt();
    }

  //Loop over the Electron correction
  if (branchElectron->GetEntries() > 0)
    for (int i = 0; i < branchElectron->GetEntries(); ++i)
    {
      Electron *ele = (Electron*)branchElectron->At(i);
      //if (std::fabs(ele->Eta) > JetEtaCut)
        //continue;
      MHT += ele->P4();
      HT += ele->P4().Pt();
    }

  //Loop over the muon correction
  if (branchMuon->GetEntries() > 0)
    for (int i = 0; i < branchMuon->GetEntries(); ++i)
    {
      Muon *muon = (Muon*)branchMuon->At(i);
      //if (std::fabs(muon->Eta) > JetEtaCut)
        //continue;
      MHT += muon->P4();
      HT += muon->P4().Pt();
    }

  //Loop over the photon correction
  if (branchPhoton->GetEntries() > 0)
    for (int i = 0; i < branchPhoton->GetEntries(); ++i)
    {
      Photon* pho = (Photon*)branchPhoton->At(i);
      if (std::fabs(pho->Eta) > JetEtaCut)
        continue;
      MHT += pho->P4();
      HT += pho->P4().Pt();
    }

  double met_x = -MHT.Px();
  double met_y = -MHT.Py();
  PUCorMet.Set(met_x, met_y);

  return true;

}       // -----  end of function DelEvent::PUCorMet  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::PreSelected
//  Description:  Preselection based on the reconstruct objects
// ===========================================================================
bool DelEvent::PreSelected()
{
  
    if (vJet.size() < 2) return false;
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    TLorentzVector J1(0, 0, 0, 0);
    TLorentzVector J2(0, 0, 0, 0);
    double MET = PUCorMet.Mod();

    if (MET < 50) return false;

    J1 = vJet.at(0).P4();
    J2 = vJet.at(1).P4();
    if (J1.Pt() < 30 || std::fabs(J1.Eta()) > 5) return false;
    if (J2.Pt() < 30 || std::fabs(J2.Eta()) > 5) return false;

    //
    // Opposive eta and eta separation
    //
    if (J1.Eta() * J2.Eta() > 0) return false;      

    if ( fabs(J1.Eta() - J2.Eta()) < 4.2) return false;      

    return true;


}       // -----  end of function DelEvent::PreSelected  -----



// ===  FUNCTION  ============================================================
//         Name:  DelEvent::GenPreSelected
//  Description:  
// ===========================================================================
bool DelEvent::GenPreSelected()
{
//----------------------------------------------------------------------------
//  Preselection as defined in the Pheno paper, in the Gen level
//----------------------------------------------------------------------------
  // Met > 50 GeV
  if (GenMet() < 50 ) return false;

  if (vGenJet.size() < 2) return false;

  // Jets related
  std::vector<Jet>::const_iterator j1 = vGenJet.begin(); // Jet1
  std::vector<Jet>::const_iterator j2 = j1+1; // Jet2

  // Jet 1 requirement
  if (j1->PT < 30 || std::fabs(j1->Eta) > 5) return false;

  // Jet 2 requirement
  if (j2->PT < 30 || std::fabs(j2->Eta) > 5) return false;

  // J1 J2 in opposite hemispheres
  if (j1->Eta * j2->Eta > 0) return false;

  // J1 J2 well separated
  if (std::fabs(j1->Eta - j2->Eta) < 4.2) return false;

  return true;
}       // -----  end of function DelEvent::GenPreSelected  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::CleanEvent
//  Description:  
// ===========================================================================
int DelEvent::CleanEvent()
{
  //Clear vector from the file
  vEvent.clear();
  vGenParticle.clear();
  vPhoton.clear();
  vElectron.clear();
  vMuon.clear();
  vJet.clear();
  vGenJet.clear();
  vCAJet.clear();
  vMissingET.clear();

  //Clear the locale variables
  PUCorMet.Set(0., 0.);
  MHT.SetPxPyPzE(0, 0, 0, 0);
  HT= -999.;

  RMjet.clear();
  CRJet.clear();
  

}       // -----  end of function DelEvent::CleanEvent  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::LoadEvent
//  Description:  
// ===========================================================================
bool DelEvent::LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
  //std::cout << " Get branch? " << branchJet->GetEntries()<< std::endl;
  //First, clean out the local event
  CleanEvent();
  
  //Now, start to load up the event
  LoadEvent(branchEvent);
  LoadElectron(branchElectron);
  LoadMuon(branchMuon);
  LoadPhoton(branchPhoton);
  LoadGenParticle(branchParticle);
  LoadRawMet(branchMet);
  LoadJet(branchJet);
  LoadGenJet(branchGenJet);
  LoadCAJet(branchCAJet);

  CalPUCorMet(branchJet, branchElectron, branchMuon, branchPhoton);
  //Whether this event is preselected?
  //return PreSelected();
  return true;
  
}       // -----  end of function DelEvent::LoadEvent  -----


// ===  FUNCTION  ============================================================
//         Name:  DelEvent::DiMuonMet
//  Description:  
// ===========================================================================
bool DelEvent::DiMuonMet()
{
  if (vMuon.size() != 2) return false;
  if (vElectron.size()> 0) return false;

  int sign = 1;
  TLorentzVector Qt(0, 0, 0, 0);
  for (int i = 0; i < vMuon.size(); ++i)
  {
    Qt += vMuon.at(i).P4();
    sign *= vMuon.at(i).Charge;
  }
  if (sign > 0) return false;
  if (Qt.M() < 60 || Qt.M() > 120) return false;
  //if (Qt.Pt() < 50) return false;

  return true;
}       // -----  end of function DelEvent::DiMuonMet  -----
 
// ===  FUNCTION  ============================================================
//         Name:  DelEvent::DiEleMet
//  Description:  
// ===========================================================================
bool DelEvent::DiEleMet()
{
  if (vElectron.size() != 2) return false;
  if (vMuon.size()> 0) return false;

  int sign = 1;
  TLorentzVector Qt(0, 0, 0, 0);
  for (int i = 0; i < vElectron.size(); ++i)
  {
    Qt += vElectron.at(i).P4();
    sign *= vElectron.at(i).Charge;
  }
  if (sign > 0) return false;
  if (Qt.M() < 60 || Qt.M() > 120) return false;
  //if (Qt.Pt() < 50) return false;


  return true;
  
}       // -----  end of function DelEvent::DiEleMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::CheckFlag
//  Description:  
// ===========================================================================
bool DelEvent::CheckFlag(std::string flag)
{
  return true;
}       // -----  end of function DelEvent::CheckFlag  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::GenMet
//  Description:  
// ===========================================================================
double DelEvent::GenMet()
{
  std::vector<int> NvIdx;
  TLorentzVector NvSum(0,0,0,0);
  
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    int pid = std::fabs(vGenParticle.at(i).PID);
    if (pid == 12 || pid == 14 || pid == 16)
    {
      NvIdx.push_back(i);
    }
  }

  for (int i = 0; i < NvIdx.size(); ++i)
  {
    NvSum += vGenParticle.at(i).P4();
  }

  return NvSum.Pt();
}       // -----  end of function DelEvent::GenMet  -----
