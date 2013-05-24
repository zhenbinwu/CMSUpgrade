#define AUX_cxx
#include "AUX.h"
#include <TH2.h>
#include <iostream>
#include <bitset>
#include <TStyle.h>
#include <list>
#include <TCanvas.h>
#include "TObjArray.h"
#include "TChainElement.h"

#ifdef __MAKECINT__
#pragma link C++ class std::map<std::string, TH1F*>+;
#endif /* __MAKECINT__ */

// ===  FUNCTION  ============================================================
//         Name:  AUX::GetFileList
//  Description:  Get the list of current TChain
// ===========================================================================
int AUX::GetFileList()
{
  TObjArray *fileElements=fChain->GetListOfFiles();
  TIter next(fileElements);
  TChainElement *chEl=0;
  while (( chEl=(TChainElement*)next() )) {
    std::cout << " File : "<< chEl->GetTitle() << std::endl;
  }

}       // -----  end of function AUX::GetFileList  -----


// ===  FUNCTION  ============================================================
//         Name:  AUX::AddFile
//  Description:  Add Filename to the TChain
// ===========================================================================
int AUX::AddFile(std::string name)
{
  return fChain->AddFile(name.c_str());
}       // -----  end of function AUX::AddFile  -----

void AUX::Loop()
{
  //   In a ROOT session, you can do:
  //      Root > .L AUX.C
  //      Root > AUX t
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

  //----------------------------------------------------------------------------
  //  Define His
  //----------------------------------------------------------------------------
  HisMap["NEVT"] = new TH1F("NEVT", "Num. of Events", 2, 0, 2 );
  HisMap["NEVTS"] = new TH1F("NEVTS", "Selected Num. of Events", 2, 0, 2 );
  HisMap["Njet"] = new TH1F("Njet", "Num. of Jets", 20, 0, 20.0 );
  HisMap["Nele"] = new TH1F("Nele", "Num. of Eles", 10, 0, 10.0 );
  HisMap["Nmuon"] = new TH1F("Nmuon", "Num. of Muons", 10, 0, 10.0 );
  HisMap["NVTX"] = new TH1F("NVTX", "Vertex Size", 60, 0, 60.0 );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Met ~~~~~
  HisMap["Met"] = new TH1F("Met", "MET", 40, 0, 800.0 );
  HisMap["MetPhi"] = new TH1F("MetPhi", "#Phi_{MET}", 16, -4, 4 );
  HisMap["MetSgn"] = new TH1F("MetSgn", "Met Sgnf.", 16, 0, 8 );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HT ~~~~~
  HisMap["HT"] = new TH1F("HT", "HT", 40, 0, 1600.0 );
  HisMap["MT"] = new TH1F("MT", "W Transverse Mass", 40, -200, 800.0 );
  HisMap["MHT"] = new TH1F("MHT", "MHT", 40, 0, 800.0 );
  HisMap["MhtPhi"] = new TH1F("MhtPhi", "#Phi_{MHT}", 14, -7, 7 );
  HisMap["MhtSgn"] = new TH1F("MhtSgn", "MHT Sgnf.", 8, -4, 4 );

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jets ~~~~~
  HisMap["JetEta"] = new TH1F("JetEta", "#eta_{Jet}", 28, -7, 7 );
  HisMap["JetPt"] = new TH1F("JetPt", "Pt_{Jet}", 30, 0, 300.0 );
  HisMap["JetPhi"] = new TH1F("JetPhi", "#phi_{Jet}", 28, -7, 7);
  HisMap["JetN"] = new TH1F("JetN", "Pt oreder of Jet", 28, 0, 28);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Jet 1 ~~~~~
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
  HisMap2D["Jet2D"] = new TH2D("Jet2D", "Jets  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  HisMap2D["J12D"] = new TH2D("J12D", "J1  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  HisMap2D["J22D"] = new TH2D("J22D", "J2  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);
  HisMap2D["J32D"] = new TH2D("J32D", "J3  in #eta_#phi plane", 10, -7, 7, 20, -7, 7);

  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of Define His ~~~~~
  Long64_t nentries = fChain->GetEntries();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    HisMap["NEVT"]->Fill(1);

    if (jentry % 2000 == 0)
    {
      std::cout << " =========== "<< ientry <<" : "<< nentries << std::endl;
    }

//----------------------------------------------------------------------------
//  Jets 
//----------------------------------------------------------------------------
    //CatoTree order jets with decreasing Et while WHAM default by decreasing
    //Et_corr. Also for DeltaRj1j2 we should use Et_corr ordered jets.
    //Thus we need to reshuffle the jets[] by Et_corr.
    //Here we just generate the index of jets, it should work.
    std::list< std::pair<double, int> > jet_map;
    for ( int ijet = 0; ijet < jetsLVec->size() ; ijet++) 
    {
      double Et_corr = (*jetsLVec)[ijet].Pt();
      jet_map.push_back(make_pair(Et_corr, ijet));
    }
    jet_map.sort();
    jet_map.reverse();

//----------------------------------------------------------------------------
//  Cuts 
//----------------------------------------------------------------------------
     if (Cut(cutbit, jet_map) == false) continue;

    FillJet(jet_map);
    HisMap["NEVTS"]->Fill(1);
    //----------------------------------------------------------------------------
    //  Start to fill His
    //----------------------------------------------------------------------------
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Number Counts ~~~~~
    HisMap["Njet"]->Fill(nJets);
    HisMap["Nmuon"]->Fill(nMuons);
    HisMap["Nele"]->Fill(nElectrons);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Met ~~~~~
    HisMap["Met"]->Fill(met);
    HisMap["MetPhi"]->Fill(metphi);
    HisMap["MetSgn"]->Fill(metSgnf);
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ HT ~~~~~
    HisMap["HT"]->Fill(ht);
    HisMap["MT"]->Fill(mt);
    HisMap["MHT"]->Fill(mht);
    HisMap["MhtPhi"]->Fill(mhtphi);
    HisMap["MhtSgn"]->Fill(mhtSgnf);

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1 ~~~~~
    if (jetsLVec->size() == 0) continue;
    std::list< std::pair<double, int> >::iterator jit = jet_map.begin();
    TLorentzVector J1 = (*jetsLVec)[jit->second];
    HisMap["J1Pt"]->Fill(J1.Pt());
    HisMap["J1Eta"]->Fill(J1.Eta());
    HisMap["J1Phi"]->Fill(J1.Phi());
    HisMap2D["J12D"]->Fill(J1.Eta(), J1.Phi());

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J2 ~~~~~
    if (jet_map.size() >= 2)
    {
      jit++;
      TLorentzVector J2 = (*jetsLVec)[jit->second];
      HisMap["J2Pt"]->Fill(J2.Pt());
      HisMap["J2Eta"]->Fill(J2.Eta());
      HisMap["J2Phi"]->Fill(J2.Phi());
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

      HisMap2D["J22D"]->Fill(J2.Eta(), J2.Phi());
      HisMap["MJJ"]->Fill((J1+J2).M());
      HisMap["dPtJJ"]->Fill(J1.Pt()-J2.Pt());
      HisMap["dPhiJJ"]->Fill(J1.DeltaPhi(J2));
      HisMap["dEtaJJ"]->Fill(J1.Eta() - J2.Eta());
      HisMap["dRJJ"]->Fill(J1.DeltaR(J2));

    }

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J2 ~~~~~
    if (jet_map.size() >= 3)
    {
      jit++;
      TLorentzVector J3 = (*jetsLVec)[jit->second];
      HisMap["J3Pt"]->Fill(J3.Pt());
      HisMap["J3Eta"]->Fill(J3.Eta());
      HisMap["J3Phi"]->Fill(J3.Phi());
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ J1J2 ~~~~~

      HisMap2D["J32D"]->Fill(J3.Eta(), J3.Phi());
      //HisMap["MJJ"]->Fill((J1+J2).M());
      //HisMap["dPtJJ"]->Fill(J1.Pt()-J2.Pt());
      //HisMap["dPhiJJ"]->Fill(J1.DeltaPhi(J2));
      //HisMap["dEtaJJ"]->Fill(J1.Eta() - J2.Eta());
      //HisMap["dRJJ"]->Fill(J1.DeltaR(J2));

    }
  }

  HistWriteDraw();
}


// ===  FUNCTION  ============================================================
//         Name:  AUX::SetName
//  Description:  Set the prefix name of plots and the output root file name
// ===========================================================================
int AUX::SetName(std::string PicName)
{
  OutPicName = PicName;
  OutFileName = PicName + ".root";
  return 1;
}       // -----  end of function AUX::SetName  -----

// ===  FUNCTION  ============================================================
//         Name:  AUX::HistWriteDraw
//  Description:  After filling, Write to a root file and draw all the output
// ===========================================================================
int AUX::HistWriteDraw() 
{
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TFile ~~~~~
  TFile f(OutFileName.c_str(), "RECREATE");
  f.cd();
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ TCanvas ~~~~~
  TCanvas* c1 = new TCanvas("Canvas", "Name of Canvas", 600, 500);

  for(std::map<std::string, TH1F*>::iterator it=HisMap.begin();
    it!=HisMap.end(); it++)
  {
    std::cout << "Printing :" << it->first << std::endl;
    it->second->Write();
    c1->Clear();
    TString hisname = OutPicName + " : " + it->second->GetTitle();
    it->second->SetTitle(hisname);
    it->second->Draw();
    TString outname = OutPicName+"_"+it->first+".png";
    c1->Print(outname);
  }

  for(std::map<std::string, TH2D*>::iterator it=HisMap2D.begin();
    it!=HisMap2D.end(); it++)
  {
    std::cout << "Printing :" << it->first << std::endl;
    it->second->Write();
    c1->Clear();
    TString hisname = OutPicName + " : " + it->second->GetTitle();
    it->second->SetTitle(hisname);
    it->second->Draw("CONT4Z ");
    TString outname = OutPicName+"_"+it->first+".png";
    c1->Print(outname);
  }
  delete c1;

  f.Close();
  return 1;
}       // -----  end of function AUX::HistWriteDraw  -----

bool AUX::Cut(std::bitset<10> cutflag, std::list< std::pair<double, int> > jet_map)
{
  
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
//----------------------------------------------------------------------------
//  VBF Selection
//----------------------------------------------------------------------------
  if (cutflag.test(0)) 
  {
    //
    // nJes
    //
    if ((*jetsLVec).size()<2) return false;
    //
    // Leading jet 
    //
    TLorentzVector J1 = (*jetsLVec)[jet_map.begin()->second];
    if (J1.Pt() < 50) return false;
    //
    // Next-to-Leading jet 
    //
    TLorentzVector J2 = (*jetsLVec)[(++jet_map.begin())->second];
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
    if (nElectrons_CUT>0) return false;
    if (nMuons_CUT>0) return false;
  }
//----------------------------------------------------------------------------
//  Met Cut
//----------------------------------------------------------------------------
  if (cutflag.test(2))
  {
    if (met < 50) return false;
  }


//----------------------------------------------------------------------------
//  MJJ cut
//----------------------------------------------------------------------------
  if (cutflag.test(3))
  {
    TLorentzVector J1 = (*jetsLVec)[jet_map.begin()->second];
    TLorentzVector J2 = (*jetsLVec)[(++jet_map.begin())->second];
    if ( (J1+J2).M()< 1500 ) return false;
  }
//----------------------------------------------------------------------------
// Large Met Cut
//----------------------------------------------------------------------------
  if (cutflag.test(4))
  {
    if (met < 200) return false;
  }
  return true;
}

// ===  FUNCTION  ============================================================
//         Name:  AUX::SetCutBit
//  Description:  Interface for setting the cut bit
// ===========================================================================
int AUX::SetCutBit(std::string inp)
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
}       // -----  end of function AUX::SetCutBit  -----

// ===  FUNCTION  ============================================================
//         Name:  AUX::FillJet
//  Description:  Fill in all the jet's eta for jet_pt > 20Gev
// ===========================================================================
int AUX::FillJet(std::list< std::pair<double, int> > jet_map)
{
  int order=1;
  for (std::list< std::pair<double, int> >::iterator jit = jet_map.begin();
    jit!=jet_map.end(); jit++)
  {
    TLorentzVector jet = (*jetsLVec)[jit->second];
    if (jet.Pt() > 20 && std::fabs(jet.Eta()) < 2.5)
    {
      HisMap["JetPt"]->Fill(jet.Pt());
      HisMap["JetEta"]->Fill(jet.Eta());
      HisMap["JetPhi"]->Fill(jet.Phi());
      HisMap["JetN"]->Fill(order);
      HisMap2D["Jet2D"]->Fill(jet.Eta(), jet.Phi());
      HisMap["NVTX"]->Fill(vtxSize);
    }
    order++;
  }
  //for ( int ijet = 0; ijet < jetsLVec->size() ; ijet++) 
  //{
    //TLorentzVector jet = (*jetsLVec)[ijet];
    //if (jet.Pt() > 20 && std::fabs(jet.Eta()) < 5)
    //{
      //HisMap["JetPt"]->Fill(jet.Pt());
      //HisMap["JetEta"]->Fill(jet.Eta());
      //HisMap["JetPhi"]->Fill(jet.Phi());
    //}
  //}
  return 1;
}       // -----  end of function AUX::FillJet  -----
