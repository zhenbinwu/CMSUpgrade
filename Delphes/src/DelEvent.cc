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
    // Now, get the jet at index
    Jet* jet = (Jet*)branchJet->At(i);

    // Remove or correct jet that matched to lepton
    //CorLepJet(i, jet);

    // Removing the jets from other requirement first
    if (RMjet.find(i) != RMjet.end()) 
      continue;

    // Apply correction to this jet
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
    jit!=lJetPt.end(); ++jit)
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
    jit!=JetPool.end(); ++jit)
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
    jit!=lJetPt.end(); ++jit)
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
    jit!=lJetPt.end(); ++jit)
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
    if (std::fabs(pho->Eta) > JetEtaCut)
      continue;
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
//         Name:  DelEvent::LoadScalarHT
//  Description:  Load the scalar HT stored in Delphes
// ===========================================================================
int DelEvent::LoadScalarHT(TClonesArray *branchHt)
{
  ScalarHT *sht = (ScalarHT*)branchHt->At(0);
  DelHT = sht->HT;
  return 1;
}       // -----  end of function DelEvent::LoadScalarHT  -----

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
//  Description:  Calculate the MHT in the event using the loaded DelEvent
//  instead of the info. stored in the Delphes tree
// ===========================================================================
bool DelEvent::CalPUCorMet() 
{
  HT = 0.0;
  //Calculate the MHT and Met in the event

  //Loop over the jet correction in DelEvent, jet Pt and Eta selection already
  //applied in the LoadJet()
  if (vJet.size() > 0)
    for (unsigned int i = 0; i < vJet.size(); ++i)
    {
      MHT += vJet.at(i).P4();
      HT += vJet.at(i).PT;
    }

  //Loop over the Electron correction
  if (vElectron.size() > 0)
    for (unsigned int i = 0; i < vElectron.size(); ++i)
    {
      MHT += vElectron.at(i).P4();
      HT += vElectron.at(i).PT;
    }

  //Loop over the muon correction
  if (vMuon.size() > 0)
    for (unsigned int i = 0; i < vMuon.size(); ++i)
    {
      MHT += vMuon.at(i).P4();
      HT += vMuon.at(i).PT;
    }

  //Loop over the photon correction
  if (vPhoton.size() > 0)
    for (unsigned int i = 0; i < vPhoton.size(); ++i)
    {
      MHT += vPhoton.at(i).P4();
      HT += vPhoton.at(i).PT;
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
  DelHT = -999.;
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
  LoadScalarHT(branchHt);

  CalPUCorMet();

  return true;
}       // -----  end of function DelEvent::LoadEvent  -----



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
  
  for (unsigned int i = 0; i < vGenParticle.size(); ++i)
  {
    int pid = std::fabs(vGenParticle.at(i).PID);
    if (pid == 12 || pid == 14 || pid == 16)
    {
      NvIdx.push_back(i);
    }
  }

  for (unsigned int i = 0; i < NvIdx.size(); ++i)
  {
    NvSum += vGenParticle.at(i).P4();
  }

  return NvSum.Pt();
}       // -----  end of function DelEvent::GenMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DelEvent::CorLepJet
//  Description:  For jet that matched to the lepton or photons in the event,
//  do something!
//  ==> Normally the Delphes samples has the unique object block to remove
//  these overlap. But for Delphes3.0.9 0PU samples, this is not true. So we
//  need this step.
//  --> Matching jet and lepton in the eta-phi plane. In case of matched,
//  compare the energy fraction: 
//  * fraction > 90% (mostly jet from the lepton), remove this jet
//  * fraction < 90%: additional energy from pileup, just correct this jet by
//  removing the lepton energy
// ===========================================================================
bool DelEvent::CorLepJet(int idx, Jet *jet)
{
  for (unsigned int i = 0; i < vMuon.size(); ++i)
  {
    Muon muon = vMuon.at(i);
    if (jet->P4().DeltaR(muon.P4()) < 0.4)
    {
      if (muon.P4().E()/jet->P4().E() > 0.9)
        RMjet.insert(idx);
      else
        CRJet[idx] = muon.P4();
      break;
    }
  }

  for (unsigned int i = 0; i < vElectron.size(); ++i)
  {
    Electron ele = vElectron.at(i);
    if (jet->P4().DeltaR(ele.P4()) < 0.4)
    {
      if (ele.P4().E()/jet->P4().E() > 0.9)
        RMjet.insert(idx);
      else
        CRJet[idx] = ele.P4();
      break;
    }
  }

  for (unsigned int i = 0; i < vPhoton.size(); ++i)
  {
    Photon pho = vPhoton.at(i);
    if (jet->P4().DeltaR(pho.P4()) < 0.4)
    {
      if (pho.P4().E()/jet->P4().E() > 0.9)
        RMjet.insert(idx);
      else
        CRJet[idx] = pho.P4();
      break;
    }
  }

  return false;
}       // -----  end of function DelEvent::CorLepJet  -----

