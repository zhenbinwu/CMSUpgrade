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

      //if (jentry > 5000) break;
      if (jentry % 5000 == 0)
        std::cout << "--------------------" << jentry << std::endl;
      if (!LoadDelEvent())  continue;


      for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
          it!=MDelCut.end(); it++)
      {
        if (ANA->CheckFlag(it->first))
        {
          it->second->FillCut();
          FillCMSHist(it->second);
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
      BookCMSHist(it->second);
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
  DEV->CalPUCorMet();

  return true;
}       // -----  end of function CMSAUX::LoadDelEvent  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadJet
//  Description:  This function is less sophisticated than the
//  DelEvent::LoadJet(). Need to improve
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


    bool muonjet = false;
    for (int i = 0; i < nMuons; ++i)
    {
      if (jet.P4().DeltaR(muonsLVec->at(i))<0.4)
      {
        muonjet = true;
        break;
      }
    }

    bool elejet = false;
    for (int i = 0; i < nElectrons; ++i)
    {
      if (jet.P4().DeltaR(elesLVec->at(i)) < 0.4)
      {
        elejet = true;
        break;
      }
    }
    if (muonjet) continue;
    if (elejet) continue;



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
  if (muonsIso->size() != muonsLVec->size()) return false;

  for (int i = 0; i < muonsLVec->size(); ++i)
  {
    Muon muon;
    muon.PT = muonsLVec->at(i).Pt();
    muon.Eta = muonsLVec->at(i).Eta();
    muon.Phi = muonsLVec->at(i).Phi();
    muon.Charge = 0;
    muon.P4() = muonsLVec->at(i);

    if (muonsIso->at(i).at(2) < 0.5)
    {
      DEV->vMuon.push_back(muon);
    }
  }
  return true;
}       // -----  end of function CMSAUX::LoadMuon  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadEvent
//  Description:  Dummy so far
// ===========================================================================
bool CMSAUX::LoadEvent()
{
  LHEFEvent e;
  e.Weight = evtWeight;
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
  missing.MET = met;
  missing.Phi = metphi;
  DEV->vMissingET.push_back(missing);
  return true;
}       // -----  end of function CMSAUX::LoadMet  -----


// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::LoadPhoton
//  Description:  No photon stored in the current anaTree 
// ===========================================================================
bool CMSAUX::LoadPhoton()
{
  //Photon p;
  //p.P4() = TLorentzVector(0, 0, 0, 0);
  //DEV->vPhoton.push_back(p);
  return true;
}       // -----  end of function CMSAUX::LoadPhoton  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::BookCMSHist
//  Description:  This function will call the DelCut->HistTool to book
//  histograms apart from the DelCut. It allow us to study some distribution
//  locally for the fullsim sample
// ===========================================================================
bool CMSAUX::BookCMSHist(DelCut* DCUT)
{
  DCUT->His->AddTH1("MuonIso1", "MuonsIso1", "Muon Isolation Type 1", "Events", 50, 0., 2, 0, 1);
  DCUT->His->AddTH1("MuonIso2", "MuonsIso2", "Muon Isolation Type 2", "Events", 50, 0., 2, 0, 1);
  DCUT->His->AddTH1("MuonIso3", "MuonsIso3", "Muon Isolation Type 3", "Events", 50, 0., 2, 0, 1);

  DCUT->His->AddTH1("MUPQT", "UPQT", "u_{#parallel}+q_{T} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  DCUT->His->AddTH1("MUParal", "UParal", "u_{#parallel} [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  DCUT->His->AddTH1("MUPerp", "UPerp", "u_{#perp}  [GeV]", "Events / 6 GeV",  100, -300, 300, 0, 1);
  DCUT->His->AddTPro("MMetScale", "MetScale", "Z/#gamma q_{T} [GeV]", "-<u_{#parallel}>/q_{T}",  50, 0, 400);
  double xbin[12] = {0, 40 , 60, 80, 100, 120, 140, 160, 200, 240, 300, 400};
  TProfile* pro1 = new TProfile( "MMetResP", "MetResP;Z/#gamma q_{T} [GeV];#sigma(u_{#parallel}) [GeV]",  11, xbin, "s");
  pro1->BuildOptions(-300, 300, "s");
  DCUT->His->AddTPro(pro1);
  TProfile* pro2 = new TProfile( "MMetResT", "MetResT;Z/#gamma q_{T} [GeV];#sigma(u_{#perp}) [GeV]",  11, xbin, "s");
  pro2->BuildOptions(-300, 300, "s");
  DCUT->His->AddTPro(pro2);
  return true;
}       // -----  end of function CMSAUX::BookCMSHist  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::FillCMSHist
//  Description:  This function will fill locally the histogram booked by
//  CMSAUX::BookCMSHist
// ===========================================================================
bool CMSAUX::FillCMSHist(DelCut* DCUT)
{
  for (int i = 0; i < muonsIso->size(); ++i)
  {
    DCUT->His->FillTH1("MuonIso1", muonsIso->at(i).at(0));
    DCUT->His->FillTH1("MuonIso2", muonsIso->at(i).at(1));
    DCUT->His->FillTH1("MuonIso3", muonsIso->at(i).at(2));
  }

//----------------------------------------------------------------------------
//  For MET performance study for rawmet
//----------------------------------------------------------------------------
  {
    // Initial values
    // The Qt of Z/gramma*
    TLorentzVector Qt(0, 0, 0, 0);

    double Mll         = -999.;
    double UParallel   = -999.;
    double UTransverse = -999.;
    double QT          = -999.;
    double UT          = -999.;
    double MetScale    = -999.;

    int sign=1;
    if (DEV->vMuon.size() == 2 && DEV->vElectron.size() == 0)
    {
      for (int i = 0; i < DEV->vMuon.size(); ++i)
      {
        Qt += DEV->vMuon.at(i).P4();
        sign *= DEV->vMuon.at(i).Charge;
      }
    } else if (DEV->vMuon.size() == 0 && DEV->vElectron.size() == 2) {
      for (int i = 0; i < DEV->vElectron.size(); ++i)
      {
        Qt += DEV->vElectron.at(i).P4();
        sign *= DEV->vElectron.at(i).Charge;
      }
    }

    if (sign > 0) return false;
    if (Qt.M() < 60 || Qt.M() > 120) return false;
    if (Qt.Pt() < 50) return false;

    // Ut , sum of other objects 
    TLorentzVector Ut(0, 0, 0, 0);
    Ut -= Qt;

    TLorentzVector MET(met*std::cos(metphi), met*std::sin(metphi), 0, met);
    //if(MET.Pt() != met || MET.Phi() != metphi);
    //{
     //std::cout << " MET.pt " << MET.Pt() << " MET.phi " << MET.Phi()  << std::endl;
     //std::cout << " met.pt " << met << " met.phi " << metphi << std::endl;

    //}


    Ut -=MET;
    // Testing rotation:
    double Dphi =  Ut.DeltaPhi(Qt);
    UTransverse = Ut.Pt() * std::sin(Dphi);
    UParallel = Ut.Pt() * std::cos(Dphi);
    QT = Qt.Pt();
    UT = Ut.Pt();
    Mll = Qt.M();
    MetScale = -1 * UParallel / Qt.Pt();
    DCUT->His->FillTH1("MUPQT", UParallel + QT);
    DCUT->His->FillTH1("MUParal", UParallel);
    DCUT->His->FillTH1("MUPerp", UTransverse);
    DCUT->His->FillTPro("MMetScale", QT, MetScale);
    DCUT->His->FillTPro("MMetResP", QT, UParallel);
    DCUT->His->FillTPro("MMetResT", QT, UTransverse);
  }
  return true;
}       // -----  end of function CMSAUX::FillCMSHist  -----

