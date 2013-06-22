#define CMSAUX_cxx
#include "CMSAUX.h"
#include <TH2.h>
#include <iostream>
#include <TStyle.h>
#include <TCanvas.h>

void CMSAUX::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L CMSAUX.C
//      Root > CMSAUX t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if (jentry > 5000 ) break;
      //if (jentry % 5000 == 0)
        //std::cout << "--------------------" << jentry << std::endl;
      if (!LoadDelEvent())  continue;

      for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
          it!=MDelCut.end(); it++)
      {
        if (ANA->CheckFlag(it->first))
        {
          it->second->FillCut();
        }
      } 

   }
}


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::PreLooping
//  Description:  Prepared for the looping
// ===========================================================================
int CMSAUX::PreLooping()
{
  
    for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
        it!=MDelCut.end(); it++)
    {
      it->second->InitCutOrder();
      it->second->BookHistogram();
    } 
    return 1;

}       // -----  end of function CMSAUX::PreLooping  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::PostLooping
//  Description:  After the looping
// ===========================================================================
int CMSAUX::PostLooping()
{  
  for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    //it->second->DrawHistogram();
    it->second->WriteHistogram();
  } 
  return 1;
}       // -----  end of function CMSAUX::PostLooping  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadDelEvent
//  Description:  
// ===========================================================================
bool CMSAUX::LoadDelEvent()
{
  DEV->CleanEvent();
  LoadJet();
  LoadMet();
  LoadGen();
  LoadElectron();
  LoadMuon();
  LoadPhoton();
  LoadGen();
  LoadEvent();

  return true;
  //return DEV->PreSelected();
}       // -----  end of function CMSAUX::LoadDelEvent  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadJet
//  Description:  
// ===========================================================================
bool CMSAUX::LoadJet()
{
  std::vector<Jet> tempJet;
  std::list< std::pair<double, int> > lJetPt; //Sorting jet energy 

  int jetsize = jetsLVec->size();
  int index = 0;
  for (int i = 0; i < jetsize; ++i)
  {
    Jet jet;
    
    jet.P4() = jetsLVec->at(i);

    jet.PT = jetsLVec->at(i).Pt();
    jet.Eta = jetsLVec->at(i).Eta();
    jet.Phi = jetsLVec->at(i).Phi();
    jet.Mass = jetsLVec->at(i).M();
    if (jet.PT < PUCorJetPt || std::fabs(jet.Eta) > PUCorJetEta) 
      continue;

    // Missing info
    jet.DeltaEta = 0.0;
    jet.DeltaPhi = 0.0;
    if (i < recoJetsBtag_0->size())
    {
      jet.BTag = recoJetsBtag_0->at(i) > 0.679 ? 1 : 0;
    }
    jet.TauTag = 0;
    jet.Charge = 0;
    jet.EhadOverEem = -999.;

    lJetPt.push_back(std::make_pair(jet.PT, index));
    tempJet.push_back(jet);
    index++;
  }

  lJetPt.sort();
  lJetPt.reverse();

  for(std::list< std::pair<double, int> >::iterator jit=lJetPt.begin();
    jit!=lJetPt.end(); jit++)
  {
    DEV->vJet.push_back(tempJet.at(jit->second));
  }
  return 1;

}       // -----  end of function CMSAUX::LoadJet  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadElectron
//  Description:  
// ===========================================================================
bool CMSAUX::LoadElectron()
{
  
  for (int i = 0; i < elesLVec->size(); ++i)
  {
    Electron ele;
    ele.PT = elesLVec->at(i).Pt();
    ele.Eta = elesLVec->at(i).Eta();
    ele.Phi = elesLVec->at(i).Phi();
    ele.Charge = 0;
    ele.EhadOverEem = -999.;
    ele.P4() = elesLVec->at(i);
    DEV->vElectron.push_back(ele);
  }

  return true;
}       // -----  end of function CMSAUX::LoadElectron  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadMuon
//  Description:  
// ===========================================================================
bool CMSAUX::LoadMuon()
{
  for (int i = 0; i < muonsLVec->size(); ++i)
  {
    Muon muon;
    muon.PT = muonsLVec->at(i).Pt();
    muon.Eta = muonsLVec->at(i).Eta();
    muon.Phi = muonsLVec->at(i).Phi();
    muon.Charge = 0;
    muon.P4() = muonsLVec->at(i);

    DEV->vMuon.push_back(muon);
  }
  return true;
}       // -----  end of function CMSAUX::LoadMuon  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadEvent
//  Description:  Dummy so far
// ===========================================================================
bool CMSAUX::LoadEvent()
{
  Event e;
  DEV->vEvent.push_back(e);

  return true;
}       // -----  end of function CMSAUX::LoadEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadGen
//  Description:   Dummy
// ===========================================================================
bool CMSAUX::LoadGen()
{
  GenParticle gen;
  DEV->vGenParticle.push_back(gen);
  return true;
}       // -----  end of function CMSAUX::LoadGen  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadMet
//  Description:  
// ===========================================================================
bool CMSAUX::LoadMet()
{
  
  MissingET missing;
  DEV->vMissingET.push_back(missing);

  TVector2 Met;
  Met.SetMagPhi(met, metphi);
  DEV->PUCorMet = Met;
  return true;
}       // -----  end of function CMSAUX::LoadMet  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadPhoton
//  Description:  
// ===========================================================================
bool CMSAUX::LoadPhoton()
{
  Photon p;
  DEV->vPhoton.push_back(p);
  return true;
}       // -----  end of function CMSAUX::LoadPhoton  -----

