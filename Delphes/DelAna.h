/*
 * ===========================================================================
 *
 *       Filename:  DelAna.h
 *
 *    Description:  Separate out the class for Analysis purpose. This should
 *    be a common interface for Delphes and CMSSW 
 *
 *        Version:  1.0
 *        Created:  06/10/2013 10:03:49 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * ===========================================================================
 */
#ifndef  __DELANA_INC__
#define  __DELANA_INC__


#include <assert.h>
#include "DelEvent.h"

/*
 * ===========================================================================
 *        Class:  DelAna
 *  Description:  
 * ===========================================================================
 */
class DelEvent;

class DelAna
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelAna (DelEvent *DEV, std::string PileUp);  /* constructor      */
    DelAna (const DelAna &other ); /* copy constructor */
    ~DelAna ();                    /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool RunPerEvent();
    bool CheckFlag(std::string name);
    bool Clear();
    int GetBasic();
    bool MetDiLepton();
    bool CalGenZvv();
    bool FindMatchedJet();

    bool METMHTAsys() const;
    TVector2 SystemMet() const;
    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelAna& operator = ( const DelAna &other ); /* assignment operator */

    // To store the event information
    std::vector<LHEFEvent> *vEvent;
    std::vector<GenParticle> *vGenParticle;
    std::vector<Photon> *vPhoton;
    std::vector<Electron> *vElectron;
    std::vector<Muon> *vMuon;
    std::vector<Jet> *vJet;
    std::vector<MissingET> *vMissingET;
    TVector2 *PUCorMet;
    TLorentzVector *MHT;
    double *HT;
    double DelHT;
    double METAsys;

    // Common variable
    Jet *J1;
    Jet *J2;
    Jet *J3;
    double Mjj;
    double Met;
    double Weight;
    TVector2 SysMet;

    // For Met Study
    TVector2 RawMet;
    double RHT;
    double Mll;
    double UParallel;
    double UTransverse;
    double QT;
    double UT;
    double MetScale;
    int UtNJets;
    int UtNPhotons;
    double UTJetsPT;
    double UTPhotonsPT;

    // For Jet Study
    //std::vector<TLorentzVector> GenJet;
    std::vector<TLorentzVector> MatchedJet;
    std::vector<double> JetPtScale;

    
    TLorentzVector GenZvv;
    std::string PileUp;
    /* ====================  DATA MEMBERS  ======================================= */
    DelEvent* DEV;
  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    std::string CurrentTag;

}; /* -----  end of class DelAna  ----- */

#endif   /* ----- #ifndef  __DELANA_INC__  ----- */
