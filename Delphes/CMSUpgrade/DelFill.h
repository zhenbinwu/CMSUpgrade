/*
 * =====================================================================================
 *
 *       Filename:  DelFill.h
 *
 *    Description:  A class that handle the Hist Filling from the Delphes
 *    sample
 *
 *        Version:  1.0
 *        Created:  05/19/2013 11:50:03 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * =====================================================================================
 */
#ifndef __DPHES_H__
#define __DPHES_H__
  
#include <iostream>
#include <list>
#include <bitset>
#include "TChain.h"
#include "TH1F.h"
#include "TH1.h"
#include "TClonesArray.h"
#include "TVector2.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"

#include "ExRootAnalysis/ExRootConfReader.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "classes/DelphesClasses.h"
//#include "DelphesClasses.h"

/*
 * =====================================================================================
 *        Class:  DPhes
 *  Description:  A class that analysis the Delphes sample, taking a Tchain as
 *  input
 * =====================================================================================
 */
class DPhes
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DPhes (TChain* chain);                             /* constructor      */
    DPhes ( const DPhes &other );   /* copy constructor */
    ~DPhes ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    int InitDelPhes(std::string process, std::string pu);
    // Test the input sample types from the name
    // Set the output file name etc.
    int SetPreName(std::string process, std::string pu);
    // Read in the Delphes interface
    int LoopCutFill(int Ncut, std::string name, double value);
    int ReadDelPhes();
    int BookHistogram();
    int Looping();
    int DrawHistogram(std::string Dir = "");
    int SetCutBit(std::string inp);

    // Setting flags for different analysis .....
    bool SetTTBar(bool leptonic, double metthred=0.0);
    bool SetPUCorMet(bool CorMet, double JetPT, double JetEta);


    /* ====================  MUTATORS      ======================================= */
    bool Cut(std::bitset<10> cutflag);

    /* ====================  OPERATORS     ======================================= */
    int OrderJet();
    TVector2 ZLLMet(std::list<int> LGen);
    int FillEvents();
    TVector2 PUCorrectedMet();
    TVector2 FindZProduct(std::vector<GenParticle*> VPart, std::list<int> LFound) const;
    std::list<int> CheckZ();
    int FillEle();
    int FillMet();
    int FillMuon();
    int FillPUCorMet();
    int FillHT();
    int PreLoopCut();
    int LoopCut();
    int FillJets();
    
    TH1F *AddTH1F(const char *name, const char *title,
                 const char *xlabel, const char *ylabel,
                 Int_t nxbins, Axis_t xmin, Axis_t xmax,
                 Int_t logx = 0, Int_t logy = 0);
    DPhes& operator = ( const DPhes &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    TChain* fChain; //Chain of input files

    // Delphes Classes
    ExRootTreeReader *treeReader;
    Long64_t NEntries;

    // Delphes branches
    TClonesArray *branchJet;
    TClonesArray *branchElectron;
    TClonesArray *branchMuon;
    TClonesArray *branchPhoton;
    TClonesArray *branchMet;
    TClonesArray *branchHt;
    TClonesArray *branchParticle;
    TClonesArray *branchEFlowTrack;
    TClonesArray *branchEFlowTower;

    // Setting flags for different analysis .....
    // PU corrected Met
    bool PUCorMet;
    double PUCorJetEta;
    double PUCorJetPt;

    // Intrisic Vs Leptonic Met
    bool LeptonicTT;
    double TTBarMetThre;


    // Useful ExRoot helper
    ExRootResult *result;
    //
    std::string OutFileName;
    std::string OutPicName;

    std::bitset<10> cutbit; //Cut flow  
    bool FakingZNN;
    bool TTBARSam;
    TVector2 RelMet; //Use the real Met instead of the met from delphes 
    TLorentzVector RelMHT;
    double RelHT;
    std::vector<int> ZJet; // Jet that matched to the Z decay 
    double ZPT; // Keep track of the Gen Z PT
    std::list< std::pair<double, int> > jet_map; //Sorting jet energy 
    std::map<std::string, TH1F*> HisMap; //Saving the 1D Hist 
    std::map<std::string, TH1F*> HisMapCL; //Saving the 1D Hist 
    std::map<std::string, TH2D*> HisMap2D; //Saving the 2D Hist 
    std::map<std::string, TH2D*> HisMap2DCL; //Saving the 2D Hist 

    bool ZVeto; // Whether to Veto this Z event
    bool IgnoreDY;
    TVector2 ZLLLep(std::list<int> LGen, std::map<int, GenParticle*> EleGen, std::map<int, GenParticle*> MuonGen);

    int LCFillJets(int NCut);
    int LCFillHT(int NCut);
    int LCFillMuon(int NCut);
    int LCFillEle(int NCut);
    int LCFillPUCorMet(int NCut);
}; /* -----  end of class DPhes  ----- */

#endif /* __DPHES_H__ */
