// ===========================================================================
// 
//       Filename:  DelFill.cc
// 
//    Description:   
// 
//        Version:  1.0
//        Created:  05/19/2013 11:58:44 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelFill.h"
#include "TRefArray.h"
/*
 *--------------------------------------------------------------------------------------
 *       Class:  DPhes
 *      Method:  DPhes
 * Description:  constructor
 *--------------------------------------------------------------------------------------
 */
DPhes::DPhes (TChain* chain)
{
  fChain = chain;

}  /* -----  end of method DPhes::DPhes  (constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  DPhes
 *      Method:  DPhes
 * Description:  copy constructor
 *--------------------------------------------------------------------------------------
 */
DPhes::DPhes ( const DPhes &other )
{
}  /* -----  end of method DPhes::DPhes  (copy constructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  DPhes
 *      Method:  ~DPhes
 * Description:  destructor
 *--------------------------------------------------------------------------------------
 */
DPhes::~DPhes ()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   for(std::map<std::string, TH1F*>::iterator it=HisMap.begin();
       it!=HisMap.end(); it++)
   {
     delete it->second;
   }
   for(std::map<std::string, TH2D*>::iterator it=HisMap2D.begin();
       it!=HisMap2D.end(); it++)
   {
     delete it->second;
   }
}  /* -----  end of method DPhes::~DPhes  (destructor)  ----- */

/*
 *--------------------------------------------------------------------------------------
 *       Class:  DPhes
 *      Method:  operator =
 * Description:  assignment operator
 *--------------------------------------------------------------------------------------
 */
  DPhes&
DPhes::operator = ( const DPhes &other )
{
  if ( this != &other ) {
  }
  return *this;
}  /* -----  end of method DPhes::operator =  (assignment operator)  ----- */

// ===  FUNCTION  ============================================================
//         Name:  DPhes::InitDelPhes
//  Description:  
// ===========================================================================
int DPhes::InitDelPhes(std::string process, std::string pu)
{
  treeReader        = 0;
  branchJet        = 0;
  branchElectron   = 0;
  branchMuon       = 0;
  branchPhoton     = 0;
  branchMet        = 0;
  branchHt         = 0;
  branchParticle   = 0;
  branchEFlowTrack = 0;
  branchEFlowTower = 0;

  NEntries = 0;
  //helper
  result =0;
  FakingZNN = false;

  // Set the output name
  SetPreName(process, pu);

  return 1;
}       // -----  end of function DPhes::InitDelPhes  -----


// ===  FUNCTION  ============================================================
//         Name:  DPhes::SetPreName
//  Description:  Get the process and sample name for output files
// ===========================================================================
int DPhes::SetPreName(std::string process, std::string pu)
{
  if (process.find("ZJETS") != std::string::npos)
  {
    FakingZNN = true;
  }
  TString name = process+"_"+pu;
  OutPicName = name.Data();
  name += ".root";
  OutFileName = name;
  return 1;
}       // -----  end of function DPhes::SetPreName  -----


// ===  FUNCTION  ============================================================
//         Name:  DPhes::ReadDelPhes
//  Description:  Provide interface to read in the Delphes class
// ===========================================================================
int DPhes::ReadDelPhes()
{

  // Create object of class ExRootTreeReader
  treeReader = new ExRootTreeReader(fChain);

  if (fChain->GetEntries() != treeReader->GetEntries())
  {
    std::cout << "Shit happened!!" << std::endl;
  }
  else
    NEntries = fChain->GetEntries();

  // Get pointers to branches used in this analysis
  branchJet         = treeReader->UseBranch("Jet");
  branchElectron    = treeReader->UseBranch("Electron");
  branchMuon        = treeReader->UseBranch("Muon");
  branchPhoton      = treeReader->UseBranch("Photon");
  branchMet         = treeReader->UseBranch("MissingET");
  branchHt          = treeReader->UseBranch("ScalarHT");
  //branchGenParticle = treeReader->UseBranch("GenParticle");
  branchParticle = treeReader->UseBranch("Particle");
  branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  branchEFlowTower = treeReader->UseBranch("EFlowTower");
}       // -----  end of function DPhes::ReadDelPhes  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::BookHistogram
//  Description:  Booking all the TH1 or TH2 histrogram using ExRootResult
// ===========================================================================
int DPhes::BookHistogram() 
{

  //HisMap["Test1"] =  new TH1F("jet_pt", "jet P_{T}", 100, 0.0, 100.0);
  //HisMap["Test2"] = new TH1F("mass", "M_{inv}(e_{1}, e_{2})", 100, 40.0, 140.0);
  //----------------------------------------------------------------------------
  //  Define His
  //----------------------------------------------------------------------------
  HisMap["NEVT"] = new TH1F("NEVT", "Num. of Events", 2, 0, 2 );
  HisMap["NEVTS"] = new TH1F("NEVTS", "Selected Num. of Events", 2, 0, 2 );
  HisMap["Njet"] = new TH1F("Njet", "Num. of Jets", 20, 0, 20.0 );
  HisMap["Nele"] = new TH1F("Nele", "Num. of Eles", 10, 0, 10.0 );
  HisMap["Nmuon"] = new TH1F("Nmuon", "Num. of Muons", 10, 0, 10.0 );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Met ~~~~~
  HisMap["Met"] = new TH1F("Met", "MET", 40, 0, 800.0 );
  HisMap["Metx"] = new TH1F("Metx", "MET_X", 40, -400, 400.0 );
  HisMap["Mety"] = new TH1F("Mety", "MET_Y", 40, -400, 400.0 );
  HisMap["MetPhi"] = new TH1F("MetPhi", "#Phi_{MET}", 16, 0, 8 );
  HisMap["MetSgn"] = new TH1F("MetSgn", "Met Sgnf.", 16, 0, 8 );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ben Defined Variable ~~~~~
  HisMap["CMet"] = new TH1F("CMet", "PU Corrected MET", 40, 0, 800.0 );
  HisMap["CMetx"] = new TH1F("CMetx", "PU Corrected MET_X", 40, -400, 400.0 );
  HisMap["CMety"] = new TH1F("CMety", "PU Corrected MET_Y", 40, -400, 400.0 );
  HisMap["CMetPhi"] = new TH1F("CMetphi", "PU Corrected MET_Phi", 40, 0, 8 );
  HisMap["CMHT"] = new TH1F("CMHT", "C MHT", 40, 0, 800.0 );
  HisMap["CMHTEta"] = new TH1F("CMHTEta", "C MHT Eta", 40, 0, 800.0 );
  HisMap["CMHTPhi"] = new TH1F("CMHTPHi", "C MHT Phi", 40, 0, 800.0 );
  HisMap["CHT"] = new TH1F("CHT", "C HT", 40, 0, 800.0 );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HT ~~~~~
  HisMap["HT"] = new TH1F("HT", "HT", 40, 0, 1600.0 );
  HisMap["MT"] = new TH1F("MT", "W Transverse Mass", 40, -200, 800.0 );
  HisMap["MHT"] = new TH1F("MHT", "MHT", 40, 0, 800.0 );
  HisMap["MhtPhi"] = new TH1F("MhtPhi", "#Phi_{MHT}", 14, -7, 7 );
  HisMap["MhtSgn"] = new TH1F("MhtSgn", "MHT Sgnf.", 8, -4, 4 );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jets ~~~~~
  HisMap["J1Pt"] = new TH1F("J1Pt", "Pt_{J1}", 40, 0, 1200.0 );
  HisMap["J1Eta"] = new TH1F("J1Eta", "#eta_{J1}", 14, -7, 7 );
  HisMap["J1Phi"] = new TH1F("J1Phi", "#phi_{J1}", 14, -7, 7);
  HisMap["J2Pt"] = new TH1F("J2Pt", "Pt_{J2}", 40, 0, 800.0 );
  HisMap["J2Eta"] = new TH1F("J2Eta", "#eta_{J2}", 14, -7, 7 );
  HisMap["J2Phi"] = new TH1F("J2Phi", "#phi_{J2}", 14, -7, 7);
  HisMap["J3Pt"] = new TH1F("J3Pt", "Pt_{J3}", 40, 0, 800.0 );
  HisMap["J3Eta"] = new TH1F("J3Eta", "#eta_{J3}", 14, -7, 7 );
  HisMap["J3Phi"] = new TH1F("J3Phi", "#phi_{J3}", 14, -7, 7);

  HisMap["MJJ"] = new TH1F("MJJ", "M_{J1, J2}", 40, 0, 2800.0 );
  HisMap["dPtJJ"] = new TH1F("dPtJJ", "#Delta Pt_{J1, J2}", 40, 0, 1200 );
  HisMap["dPhiJJ"] = new TH1F("dPhiJJ", "#Delta #phi_{J1, J2}", 14, -7, 7 );
  HisMap["dEtaJJ"] = new TH1F("dEtaJJ", "#Delta #eta_{J1, J2}", 14, -7, 7 );
  HisMap["dRJJ"] = new TH1F("dRJJ", "#Delta R_{J1, J2}", 20, 0, 10.0 );
 
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 2D ~~~~~
  HisMap2D["J12D"] = new TH2D("J12D", "J1  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  HisMap2D["J22D"] = new TH2D("J22D", "J2  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  HisMap2D["J32D"] = new TH2D("J32D", "J3  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  return 1;
}       // -----  end of function DPhes::BookHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::Looping
//  Description:  Looping the chain
// ===========================================================================
int DPhes::Looping()
{
  
  std::cout << "NEntriese" << NEntries << std::endl;
  // Loop over all events
  for(Int_t entry = 0; entry < NEntries ; ++entry)
  {
    if (entry % 2000 == 0)
      std::cout << "--------------------" << entry << std::endl;
    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);


    if ((branchJet->GetEntries()+branchElectron->GetEntries()+branchMuon->GetEntries()+branchPhoton->GetEntries()) == 0)
    {
      HisMap["NEVT"]->Fill(0);
    }
    else
      HisMap["NEVT"]->Fill(1);
    // Clear our the jet ordering list
    OrderJet();

    //// Apply cuts before filling up the histogram
    if (Cut(cutbit) == false) continue;
    HisMap["NEVTS"]->Fill(1);

    FillEvents();
    FillEle();
    FillMet();
    FillMuon();
    FillHT();
    FillJets();
    PUCorrectedMet();

  }

  return 1;
}       // -----  end of function DPhes::Looping  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::DrawHistogram
//  Description:  
// ===========================================================================
int DPhes::DrawHistogram()
{
  TFile f(OutFileName.c_str(), "RECREATE");
  f.cd();
  TCanvas *c1 = new TCanvas("fd", "fdj", 600, 500);
  for(std::map<std::string, TH1F*>::iterator i=HisMap.begin();
    i!=HisMap.end(); i++)
  {
    c1->cd();
    c1->Clear();
    i->second->Write();
    //i->second->Draw();
    //TString name = OutPicName +"_"+ i->first + ".png";
    //c1->Print(name);
  }
  delete c1;
  f.Close();
  return 1;
}       // -----  end of function DPhes::DrawHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::FillEvents
//  Description:  Filling up event information
// ===========================================================================
int DPhes::FillEvents()
{
  return 1;
}       // -----  end of function DPhes::FillEvents  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::FillMet
//  Description:  Filling up Met information
// ===========================================================================
int DPhes::FillMet()
{
  if (FakingZNN)
  {
    TVector2 met = ZLLMet();
    HisMap["Met"]->Fill(met.Mod());
    HisMap["MetPhi"]->Fill(met.Phi());
  } else{
    // If event contains at least 1 jet
    if(branchMet->GetEntries() != 1)
      return 0;

    MissingET *met = (MissingET*) branchMet->At(0);
    HisMap["Met"]->Fill(met->MET);
    HisMap["MetPhi"]->Fill(met->Phi);
    TVector2 met2V;
    met2V.SetMagPhi(met->MET, met->Phi);
    HisMap["Metx"]->Fill(met2V.Px());
    HisMap["Mety"]->Fill(met2V.Py());
  }

  return 1;
}       // -----  end of function DPhes::FillMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::PUCorrectedMet
//  Description:  Try to calculate the MHT, HT and Met by hand
// ===========================================================================
TVector2 DPhes::PUCorrectedMet()
{
 //Calculate the MHT and Met in the event
  TLorentzVector MHT(0,0,0,0);
  double HT=0;

  //Loop over the jet correction
  if (branchJet->GetEntries() > 0)
    for (int i = 0; i < branchJet->GetEntries(); ++i)
    {
      MHT += ((Jet*)branchJet->At(i))->P4();
      HT += ((Jet*)branchJet->At(i))->P4().Mag();
    }

  //Loop over the Electron correction
  if (branchElectron->GetEntries() > 0)
    for (int i = 0; i < branchElectron->GetEntries(); ++i)
    {
      MHT += ((Electron*)branchElectron->At(i))->P4();
      HT += ((Electron*)branchElectron->At(i))->P4().Mag();
    }

  //Loop over the muon correction
  if (branchMuon->GetEntries() > 0)
    for (int i = 0; i < branchMuon->GetEntries(); ++i)
    {
      MHT += ((Muon*)branchMuon->At(i)) ->P4();
      HT += ((Muon*)branchMuon->At(i))->P4().Mag();
    }

  //Loop over the photon correction
  if (branchPhoton->GetEntries() > 0)
    for (int i = 0; i < branchPhoton->GetEntries(); ++i)
    {
      MHT += ((Photon*)branchPhoton->At(i))->P4();
      HT += ((Photon*)branchPhoton->At(i))->P4().Mag();
    }

  if (MHT == TLorentzVector(0, 0, 0,0))
  {
    return TVector2(0,0);
  }

  double met_x = -MHT.Px();
  double met_y = -MHT.Py();
  TVector2 CMet(met_x, met_y);
  HisMap["CMet"]->Fill(CMet.Mod());
  HisMap["CMetx"]->Fill(met_x);
  HisMap["CMety"]->Fill(met_y);
  HisMap["CMetPhi"]->Fill(CMet.Phi());
  HisMap["CMHT"]->Fill(MHT.Mag());
  HisMap["CMHTEta"]->Fill(MHT.Eta());
  HisMap["CMHTPhi"]->Fill(MHT.Phi());
  HisMap["CHT"]->Fill(HT);
  return CMet;

}       // -----  end of function DPhes::PUCorrectedMet  -----
// ===  FUNCTION  ============================================================
//         Name:  DPhes::FillEle
//  Description:  Filling up electron information
// ===========================================================================
int DPhes::FillEle()
{
  if (FakingZNN) return 1;

  // If event contains at least 1 jet
  int Eleentries = branchElectron->GetEntries();
  HisMap["Nele"]->Fill(Eleentries);

  if(branchElectron->GetEntries() <= 0)
    return 0;

  return 1;
}       // -----  end of function DPhes::FillEle  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::FillMuon
//  Description:  Filling up Muonctron information
// ===========================================================================
int DPhes::FillMuon()
{
  if (FakingZNN) return 1;

  // If event contains at least 1 jet
  int Mentries = branchMuon->GetEntries();
  HisMap["Nmuon"]->Fill(Mentries);

  if(branchMuon->GetEntries() <= 0)
    return 0;

  return 1;
}       // -----  end of function DPhes::FillMuon  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::FillHT
//  Description:  Filling up the HT and MHT information
// ===========================================================================
int DPhes::FillHT()
{
  // If event contains at least 1 jet
  if(branchHt->GetEntries() != 1)
    return 0;

  ScalarHT* Ht = (ScalarHT*) branchHt->At(0);
  HisMap["HT"]->Fill(Ht->HT);
  return 1;
}       // -----  end of function DPhes::FillHT  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::OrderJet
//  Description:  Reorder the jet list by the jet pt
// ===========================================================================
int DPhes::OrderJet()
{
  jet_map.clear();
  //CatoTree order jets with decreasing Et while WHAM default by decreasing
  //Et_corr. Also for DeltaRj1j2 we should use Et_corr ordered jets.
  //Thus we need to reshuffle the jets[] by Et_corr.
  //Here we just generate the index of jets, it should work.

  for (int i = 0; i < branchJet->GetEntries(); ++i)
  {
    double Et_corr = ((Jet*)branchJet->At(i))->PT;
    jet_map.push_back(std::make_pair(Et_corr, i));
  }
  jet_map.sort();
  jet_map.reverse();
  return 1;
}       // -----  end of function DPhes::OrderJet  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::FillJets
//  Description:  All the histogram of Jets are filling up here
// ===========================================================================
int DPhes::FillJets()
{

  int jentries =  branchJet->GetEntries();
  HisMap["Njet"]->Fill(jentries);
  // If event contains at least 1 jet
  if(jentries <= 0)
    return 0;




  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1 ~~~~~
  std::list< std::pair<double, int> >::iterator jit = jet_map.begin();
  Jet *J1 = (Jet*) branchJet->At(jit->second);

  HisMap["J1Pt"]->Fill(J1->PT);
  HisMap["J1Eta"]->Fill(J1->Eta);
  HisMap["J1Phi"]->Fill(J1->Phi);
  HisMap2D["J12D"]->Fill(J1->Eta, J1->Phi);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J2 ~~~~~
  if (jet_map.size() >= 2)
  {
    jit++;
    Jet *J2 = (Jet*) branchJet->At(jit->second);
    HisMap["J2Pt"]->Fill(J2->PT);
    HisMap["J2Eta"]->Fill(J2->Eta);
    HisMap["J2Phi"]->Fill(J2->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

    HisMap2D["J22D"]->Fill(J2->Eta, J2->Phi);
    HisMap["MJJ"]->Fill((J1->P4()+J2->P4()).M());
    HisMap["dPtJJ"]->Fill(J1->PT-J2->PT);
    HisMap["dPhiJJ"]->Fill(J1->P4().DeltaPhi(J2->P4()));
    HisMap["dEtaJJ"]->Fill(J1->Eta - J2->Eta);
    HisMap["dRJJ"]->Fill(J1->P4().DeltaR(J2->P4()));

  }

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J2 ~~~~~
  if (jet_map.size() >= 3)
  {
    jit++;
    Jet *J3 = (Jet*) branchJet->At(jit->second);
    HisMap["J3Pt"]->Fill(J3->PT);
    HisMap["J3Eta"]->Fill(J3->Eta);
    HisMap["J3Phi"]->Fill(J3->Phi);
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

    HisMap2D["J32D"]->Fill(J3->Eta, J3->Phi);
    //HisMap["MJJ"]->Fill((J1+J2).M());
    //HisMap["dPtJJ"]->Fill(J1.Pt()-J2.Pt());
    //HisMap["dPhiJJ"]->Fill(J1.DeltaPhi(J2));
    //HisMap["dEtaJJ"]->Fill(J1.Eta() - J2.Eta());
    //HisMap["dRJJ"]->Fill(J1.DeltaR(J2));

  }
  return 1;

}       // -----  end of function DPhes::FillJets  -----


// ===  FUNCTION  ============================================================
//         Name:  DPhes::SetCutBit
//  Description:  Interface for setting the cut bit
// ===========================================================================
int DPhes::SetCutBit(std::string inp)
{
  if (inp == "-1")
  {
    cutbit.set();
  }else if (inp == "0")
  {
    cutbit = std::bitset<10>(std::string("0000000000")); // No selections
  }
  else
  cutbit = std::bitset<10>(inp); //No selection
  return 1;
}       // -----  end of function DPhes::SetCutBit  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::ZLLMet
//  Description:  Calculating the met from ZLL as the ZNN sample
// ===========================================================================
TVector2 DPhes::ZLLMet()
{
  // First get the PU corrected Met in the event
  TVector2 oldMet = PUCorrectedMet();

//----------------------------------------------------------------------------
//  Looping the events for Z decay products 
//----------------------------------------------------------------------------
  GenParticle *particle = 0;
  std::vector<GenParticle*> VPart; //Vector of particle from Z
  
//----------------------------------------------------------------------------
//  Adding electron
//----------------------------------------------------------------------------
  for (int i = 0; i < branchElectron->GetEntries(); ++i)
  {
    Electron* ele = (Electron*) branchElectron->At(i);
    TVector2 temp(0, 0);
    particle = 0;
    temp.SetMagPhi(ele->P4().Perp(), ele->P4().Phi());
    oldMet += temp;
    particle = (GenParticle*) ele->Particle.GetObject();
    VPart.push_back(particle);
  }

//----------------------------------------------------------------------------
//  Adding muon
//----------------------------------------------------------------------------
  for (int i = 0; i < branchMuon->GetEntries(); ++i)
  {
    Muon* muon = (Muon*) branchMuon->At(i);
    TVector2 temp(0, 0);
    temp.SetMagPhi(muon->P4().Perp(), muon->P4().Phi());
    oldMet += temp;
    particle = (GenParticle*) muon->Particle.GetObject();
    VPart.push_back(particle);
  }
  

  if (VPart.size() > 2 )
  {
    std::cout << " ??  More than 2 leptons? Should not happen! " << std::endl;
  }

  if (VPart.size() != 2)
  {
    oldMet += FindZProduct(VPart, CheckZ());
  }

  return oldMet;
}       // -----  end of function DPhes::ZLLMet  -----

bool DPhes::Cut(std::bitset<10> cutflag)
{
  
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   TLorentzVector J1(0, 0, 0, 0);
   TLorentzVector J2(0, 0, 0, 0);
   double MET = 0;
//----------------------------------------------------------------------------
//  VBF Selection
//----------------------------------------------------------------------------
  if (cutflag.test(0)) 
  {
    //
    // nJes
    //
    if (jet_map.size()<2) return false;
    //
    // Leading jet 
    //
    std::list< std::pair<double, int> >::iterator jit = jet_map.begin();
    J1 = ((Jet*) branchJet->At(jit->second))->P4();
    if (J1.Pt() < 50) return false;
    //
    // Next-to-Leading jet 
    //
    jit++;
    J2 = ((Jet*) branchJet->At(jit->second))->P4();
    if (J2.Pt() < 50) return false;      
    //
    // Opposive eta and eta separation
    //
    if (J1.Eta() * J2.Eta() > 0) return false;      
    if ( fabs(J1.Eta() - J2.Eta()) < 4.2) return false;      
    //
    // Dijet mass cut
    //
    if ( (J1+J2).M()<700. ) return false;

  }   

//----------------------------------------------------------------------------
//  Lepton Veto
//----------------------------------------------------------------------------
  if (cutflag.test(1))
  {
    if (branchElectron->GetEntries()>0) return false;
    if (branchMuon->GetEntries()>0) return false;
  }

//----------------------------------------------------------------------------
//  Met Cut
//----------------------------------------------------------------------------
  if (cutflag.test(2))
  {
    if (FakingZNN)
      MET = ZLLMet().Mod();
    else
      MET = ((MissingET*) branchMet->At(0))->MET;
    if (MET < 50) return false;
  }


//----------------------------------------------------------------------------
//  MJJ cut
//----------------------------------------------------------------------------
  if (cutflag.test(3))
  {
    if ( (J1+J2).M()< 1500 ) return false;
  }
//----------------------------------------------------------------------------
// Large Met Cut
//----------------------------------------------------------------------------
  if (cutflag.test(4))
  {
    if (MET < 200) return false;
  }
  return true;
}

// ===  FUNCTION  ============================================================
//         Name:  DPhes::AddTH1F
//  Description:  /* cursor */
// ===========================================================================
TH1F *DPhes::AddTH1F(const char *name, const char *title,
                 const char *xlabel, const char *ylabel,
                 Int_t nxbins, Axis_t xmin, Axis_t xmax,
                 Int_t logx, Int_t logy)
{
  TH1F *hist = new TH1F(name, title, nxbins, xmin, xmax);
  hist->GetXaxis()->SetTitle(xlabel);
  hist->GetYaxis()->SetTitle(ylabel);
  return hist;
}       // -----  end of function DPhes::AddTH1F  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::CheckZ
//  Description:  This is a function to check the Z decay productions
// ===========================================================================
std::list<int> DPhes::CheckZ()
{
  // Actually this function should name differently now
  // I loop over the particle list to find the leptons 
  // The mother tracing method didn't work because Delphes cuts away
  // the particles list to save the disk
  std::list<int> VLep;

    // Find Z and take the next two as the lepton 
  for (int i = 0; i < branchParticle->GetEntries(); ++i)
  {
    GenParticle* p = (GenParticle*)branchParticle->At(i);
    if (p->PID == 23)
    {
      VLep.push_back(++i);
      VLep.push_back(++i);
      break;
    } 
  }
  if (VLep.size() == 2) return VLep;

  // In case of DY ... the first two leptons in the order
  for (int i = 0; i < branchParticle->GetEntries(); ++i)
  {
    GenParticle* p = (GenParticle*)branchParticle->At(i);

    if  (std::abs(p->PID) == 11 || std::abs(p->PID) == 13 || std::abs(p->PID) == 15)
    {
      VLep.push_back(i);
      VLep.push_back(++i);
      break;
    }
  }
  if (VLep.size() == 2) return VLep;

  std::cout << "Should never come to here! " << VLep.size() << std::endl;
}       // -----  end of function DPhes::CheckZ  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::FindZProduct
//  Description:  This step is getting too big, so separate out as a function
//  We need to find out where the other particles goes when there is only 1 or
//  0 lepton in the event
// ===========================================================================
TVector2 DPhes::FindZProduct(std::vector<GenParticle*> VPart, std::list<int> LFound) const
{
  TVector2 addmet(0, 0);
  GenParticle *particle = 0;
  std::vector<int> VMet;

  // Compare the decay particle to the particles identified as lepton
  // And remove them from the list
  for(std::vector<GenParticle*>::iterator lit=VPart.begin();
      lit!=VPart.end(); lit++)
  {

    // Lepton particle is referred as lit
    std::list<int>::iterator it=LFound.begin();
    while( it!=LFound.end() )
    {
      GenParticle* p = (GenParticle*)branchParticle->At(*it);
      if (p->P4() == (*lit)->P4() && p->Charge == (*lit)->Charge)
        it = LFound.erase(it);
      else it++;
    }
  }

  std::list<int>::iterator it=LFound.begin();
  while (it!=LFound.end())
  {
    GenParticle* p = (GenParticle*)branchParticle->At(*it);
    addmet += TVector2(p->Px, p->Py);
    it++;
  }

  return addmet;
}       // -----  end of function DPhes::FindZProduct  -----
