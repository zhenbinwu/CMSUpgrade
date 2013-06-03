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
    DPhes (TChain* chain);                    /* constructor         */
    DPhes ( const DPhes &other );             /* copy constructor    */
    ~DPhes ();                                /* destructor          */
    DPhes& operator = ( const DPhes &other ); /* assignment operator */

    /* ====================  ACCESSORS     ======================================= */
    // Initial the DelPhes process
    int InitDelPhes(std::string process, std::string pu);

    // Read in the Delphes interface
    int ReadDelPhes();

    // Test the input sample types from the name
    // Set the output file name etc.
    int SetPreName(std::string process, std::string pu);

    // Set the CutBit : No useful anymore since we loop over the cut flow
    int SetCutBit(std::string inp);

/*-----------------------------------------------------------------------------
 *  Functions to set for specific process
 *-----------------------------------------------------------------------------*/
    // Setting flags for different analysis .....
    bool SetTTBar(bool leptonic, double metthred=0.0);
    bool SetPUCorMet(bool CorMet, double JetPT, double JetEta);

    // Histo related and looping
    int BookHistogram();
    int Looping();
    int DrawHistogram(std::string Dir = "");

    int LoopCutFill(int Ncut, std::string name, double value);



    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */
    
  protected:
    /* ====================  DATA MEMBERS  ======================================= */
    // Usual stuff for filling 

    int OrderJet();
    bool Cut(std::bitset<10> cutflag);
    TVector2 PUCorrectedMet(); // PU corrected Met

    // ZLL samples related functions
    std::list<int> CheckZ(); //Check the GenParticle for Z  
    TVector2 ZLLMet(std::list<int> LGen);
    TVector2 ZLLLep(std::list<int> LGen, std::map<int, GenParticle*> EleGen, std::map<int, GenParticle*> MuonGen);

    
/*-----------------------------------------------------------------------------
 *  Filling the histogram normal way
 *-----------------------------------------------------------------------------*/
    int FillEvents();
    int FillEle();
    int FillMet();
    int FillJets();
    int FillMuon();
    int FillPUCorMet();
    int FillHT();



/*-----------------------------------------------------------------------------
 *  Looping cut flow filling histogram
 *-----------------------------------------------------------------------------*/
    int PreLoopCut(); // Prepare the histo for looping 
    int LoopCut();
    int LCFillJets(int NCut);
    int LCFillHT(int NCut);
    int LCFillMuon(int NCut);
    int LCFillEle(int NCut);
    int LCFillPUCorMet(int NCut);

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    TChain* fChain; //Chain of input files
    Long64_t NEntries;

    // Delphes Classes
    ExRootTreeReader *treeReader;

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


/*-----------------------------------------------------------------------------
 *  // Setting flags for different analysis ..... 
 *-----------------------------------------------------------------------------*/
   
    // PU corrected Met
    bool PUCorMet; // Wether to do the PU corrected Met 
    double PUCorJetEta; // The Eta range of the jet into the Met correction
    double PUCorJetPt; // The Pt of the jet into the Met correction

    // Intrisic Vs Leptonic Met for ttbar
    bool TTBARSam;
    bool LeptonicTT;
    double TTBarMetThre;


    // Treating the ZLL sample as Zvv
    bool FakingZNN;
    double ZPT; // Keep track of the Gen Z PT
    bool ZVeto; // Whether to Veto this Z event
    bool IgnoreDY; // Whether to ignore this DY event from the sample size
    std::vector<int> ZJet; // Jet that matched to the Z decay , should be vetoed


/*-----------------------------------------------------------------------------
 *  Local variable stored
 *-----------------------------------------------------------------------------*/
    TVector2 RelMet; //Use the real Met instead of the met from delphes 
    TLorentzVector RelMHT;
    double RelHT;
    std::list< std::pair<double, int> > jet_map; //Sorting jet energy 

/*-----------------------------------------------------------------------------
 *  Hist tools
 *-----------------------------------------------------------------------------*/
    std::map<std::string, TH1F*> HisMap; //Saving the 1D Hist 
    std::map<std::string, TH1F*> HisMapCL; //Saving the 1D Hist 
    std::map<std::string, TH2D*> HisMap2D; //Saving the 2D Hist 
    std::map<std::string, TH2D*> HisMap2DCL; //Saving the 2D Hist 

/*-----------------------------------------------------------------------------
 *  Cut Flow variables
 *-----------------------------------------------------------------------------*/
    std::bitset<10> cutbit; //Cut flow   //The cut bit
    std::vector<std::string> order; // Order of the cut  
    std::map<std::string, std::string>  CutMap; //Mapping jet cut to the bitset 

/*-----------------------------------------------------------------------------
 *  Output option
 *-----------------------------------------------------------------------------*/
    // OutPut File
    std::string OutFileName;
    std::string OutPicName;




}; /* -----  end of class DPhes  ----- */

#endif /* __DPHES_H__ */
