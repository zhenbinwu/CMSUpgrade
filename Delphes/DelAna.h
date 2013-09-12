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
    DelAna (DelEvent *DEV, std::string PileUp, std::string Detector);  /* constructor      */
    DelAna (const DelAna &other ); /* copy constructor */
    ~DelAna ();                    /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool RunPerEvent();
    bool CheckFlag(std::string name);
    bool Clear();
    int GetBasic();
    bool MetDiLepton();
    bool CalGenMet();
    bool FindMatchedJet();
    bool FindMatchedLep();

    bool FindJetLepton();
    bool METMHTAsys() const;
    TVector2 SystemMet() const;
    bool DetectorApp(double Eta) const;
    int JetMatching(int GenIdx, std::vector<int>& JetIdx) const;
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
    std::vector<Jet> *vGenJet;
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

    // For Efficiency study
    // map<GenParticle index, reco index> 
    std::map<int, int> MatchedEle;
    std::map<int, int> MatchedMuon;
    std::map<int, int> MatchedTau;
    std::map<int, int> MatchedPhoton;
    std::map<int, int> MatchedJet;
    std::map<int, int> MatchedEleJet;
    std::map<int, int> MatchedMuonJet;
    std::map<int, int> MatchedPhotonJet;
    std::vector<int> PileUpJet; //For jets that don't match to GenJet or lost leptons

    TLorentzVector GenMet;
    std::string PileUp;
    std::string Detector;
    /* ====================  DATA MEMBERS  ======================================= */
    DelEvent* DEV;
  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    std::string CurrentTag;

}; /* -----  end of class DelAna  ----- */

#endif   /* ----- #ifndef  __DELANA_INC__  ----- */
