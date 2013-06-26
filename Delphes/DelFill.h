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
#include <bitset>
#include <sys/stat.h>
#include "TChain.h"
#include "TH1F.h"
#include "TH1.h"
#include "TClonesArray.h"
#include "TVector2.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TProfile.h"

// Classes from Ben
#include "DelEvent.h"
#include "DelWino.h"
#include "DelZJJ.h"
#include "DelTT.h"
#include "DelAna.h"
#include "DelCut.h"

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
    int PreLooping();
    int Looping();
    int PostLooping();
    bool GetCrossSection(std::string process);

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DPhes& operator = ( const DPhes &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    TChain* fChain; //Chain of input files

    // Delphes Classes
    ExRootTreeReader *treeReader;
    DelEvent *DEV;
    DelAna *ANA;
    std::map<std::string, DelCut*> MDelCut; // For separated output

    // Delphes branches
    TClonesArray *branchEvent;
    TClonesArray *branchJet;
    TClonesArray *branchGenJet;
    TClonesArray *branchCAJet;
    TClonesArray *branchElectron;
    TClonesArray *branchMuon;
    TClonesArray *branchPhoton;
    TClonesArray *branchMet;
    TClonesArray *branchHt;
    TClonesArray *branchParticle;
    TClonesArray *branchEFlowTrack;
    TClonesArray *branchEFlowTower;



    // Define event
    double PUCorJetEta; // The Eta range of the jet into the Met correction
    double PUCorJetPt; // The Pt of the jet into the Met correction

    // Cross Section
    double CrossSection; // For HTBin sample, store the cross section according to the process
}; /* -----  end of class DPhes  ----- */

#endif /* __DPHES_H__ */
