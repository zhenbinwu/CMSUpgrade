/*
 * ===========================================================================
 *
 *       Filename:  DelFill.h
 *
 *    Description:  The interface class to load the Delphes tree and looping
 *
 *        Version:  1.0
 *        Created:  06/10/2013 11:40:02 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * ===========================================================================
 */

#ifndef __DPHES_H__
#define __DPHES_H__
  
#include <iostream>
#include <list>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <bitset>
#include <memory>
#include <sys/stat.h>
#include "stdlib.h"

#include "TChain.h"
#include "TH1F.h"
#include "TH1.h"
#include "TClonesArray.h"
#include "TVector2.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TTreeCache.h"

// Classes from Ben
#include "DelEvent.h"
#include "DelEventWino.h"
#include "DelEventZJJ.h"
#include "DelEventTT.h"
#include "DelEventLL.h"
#include "DelEventHTB.h"
#include "DelAna.h"
#include "DelProcess.h"

// Classes from Delphes
#include "ExRootAnalysis/ExRootConfReader.h"
#include "ExRootAnalysis/ExRootTreeReader.h"
#include "ExRootAnalysis/ExRootResult.h"
#include "classes/DelphesClasses.h"



/*
 * ===========================================================================
 *        Class:  DPhes
 *  Description:  
 * ===========================================================================
 */
class DPhes
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DPhes (TChain* chain);                    /* constructor         */
    DPhes ( const DPhes &other );             /* copy constructor    */
    ~DPhes ();                                /* destructor          */

    /* ====================  ACCESSORS     ======================================= */
    bool SetPUCorMet(double JetPT, double JetEta);
    int InitDelPhes(std::string process, std::string pu, std::string outdir);
    int SetPreName(std::string process, std::string pu, std::string outdir);
    int ReadDelPhes();
    int PreLooping(const std::vector<std::string>& VCuts);
    int Looping();
    int PostLooping();
    bool GetCrossSection(std::string process);

    /* ====================  MUTATORS      ======================================= */
    int ParseMemoryLine(char* line);
    double GetMemoryValue();

    /* ====================  OPERATORS     ======================================= */

    DPhes& operator = ( const DPhes &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */
    TString ModifiedPreName(const TString output, std::string org, std::string rep) const;

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    TChain* fChain; //Chain of input files

    // Delphes Classes
    ExRootTreeReader *treeReader;
    DelEvent *DEV;
    DelAna *ANA;
    std::map<std::string, std::unique_ptr<DelProcess> > MDelPro; // For separated output

    // Delphes branches
    TClonesArray *branchEvent;
    TClonesArray *branchParticle;
    TClonesArray *branchGenJet;
    TClonesArray *branchCAJet;
    TClonesArray *branchJet;
    TClonesArray *branchElectron;
    TClonesArray *branchPhoton;
    TClonesArray *branchMuon;
    TClonesArray *branchMet;
    TClonesArray *branchHt;


    // Define event
    double PUCorJetEta; // The Eta range of the jet into the Met correction
    double PUCorJetPt; // The Pt of the jet into the Met correction

    // Cross Section
    double CrossSection;      // For HTBin sample, store the cross section according to the process
    double CrossSectionError; // For HTBin sample, store the cross section according to the process

    // Process and PU
    std::string Process;
    std::string PU;
}; /* -----  end of class DPhes  ----- */


#endif /* __DPHES_H__ */
