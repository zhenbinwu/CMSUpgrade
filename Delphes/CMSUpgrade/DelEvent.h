/*
 * =====================================================================================
 *
 *       Filename:  DelEvent.h
 *
 *    Description:  A dummy class to hold the Delphes events, so that I can
 *    play all the tricks on it
 *
 *        Version:  1.0
 *        Created:  06/03/2013 11:12:10 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * =====================================================================================
 */

#ifndef  __DELEVENT_INC__
#define  __DELEVENT_INC__


//#include <forward_list>
#include <list>
#include <map>
#include <iostream>
#include <set>
#include "TClonesArray.h"
#include "classes/DelphesClasses.h"
#include "DelAna.h"
/*
 * =====================================================================================
 *        Class:  DelEvent
 *  Description:  A class to hold the loaded Del event
 * =====================================================================================
 */
class DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelEvent (double Eta, double Pt);   /* constructor      */
    DelEvent ( const DelEvent &other ); /* copy constructor */
    ~DelEvent ();                       /* destructor       */


    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelEvent& operator = ( const DelEvent &other ); /* assignment operator */


    /* ====================  ACCESSORS     ======================================= */
    // Public access to load the event, virtual for Zll fakign zvv
    virtual bool LoadEvent(TClonesArray *branchEvent,TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet, TClonesArray *branchElectron, 
        TClonesArray *branchMuon, TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

    // To store the event information
    std::vector<Event> vEvent;
    std::vector<GenParticle> vGenParticle;
    std::vector<Photon> vPhoton;
    std::vector<Electron> vElectron;
    std::vector<Muon> vMuon;
    std::vector<Jet> vJet, vGenJet, vCAJet;
    std::vector<MissingET> vMissingET;
    TVector2 PUCorMet;
    TLorentzVector MHT;
    double HT;

    // For Met Study
    double UParallel;
    double UTransverse;
    double QT;
    double MetScale;


  protected:
    /* ====================  DATA MEMBERS  ======================================= */
    int CleanEvent();
    bool CalPUCorMet(TClonesArray *branchJet, TClonesArray *branchElectron, 
                TClonesArray *branchMuon, TClonesArray *branchPhoton);
    int LoadEvent(TClonesArray *branchEvent);
    int LoadGenParticle(TClonesArray *branchGenParticle);
    int LoadRawMet(TClonesArray *branchMet);
    int LoadPhoton(TClonesArray *branchPhoton);
    int LoadMuon(TClonesArray *branchMuon);
    int LoadElectron(TClonesArray *branchElectron);
    Jet* JetCor(Jet jet, TLorentzVector CorVet);
    bool JetSel(Jet j);
    int LoadJet(TClonesArray *branchJet);
    int LoadGenJet(TClonesArray *branchJet);
    int LoadCAJet(TClonesArray* branchCAJet);
    // GenParticle level preselection, which rejects almost all the events
    double GenMet();
    bool GenPreSelected();

    // For Met study
    bool DiMuonMet();
    bool DiEleMet();

    //Flags for the selection on the event
    double JetEtaCut;
    double JetPtCut;
    // The index of jets to be vetoed by the different function
    // Using set for the unique elements
    std::set<int>  RMjet;
    // A mapping to correct the jet energy
    std::map<int, TLorentzVector> CRJet;

    /*---------------------------------------------------------------------------
     *  Virtual functions
     *--------------------------------------------------------------------------*/
    // Virtual preselected for the signal sample, which is generated with the
    // preselection
    virtual bool PreSelected();
    // Public access to check the flag of selection in the event, usually
    // separated out for different DelCut in the filling
    virtual bool CheckFlag(std::string flag);

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    
    friend class DelAna;

}; /* -----  end of class DelEvent  ----- */


#endif   /* ----- #ifndef __DELEVENT_INC__  ----- */
