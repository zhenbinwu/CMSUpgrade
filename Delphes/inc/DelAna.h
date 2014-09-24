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


#include "DelEvent.h"

#include <assert.h>
#include <functional>
#include "boost/bimap.hpp"
#include "boost/bimap/unordered_set_of.hpp"
#include "boost/bimap/multiset_of.hpp"

#ifdef  OXBRIDGE_MT2
#include "Mt2/ChengHanBisect_Mt2_332_Calculator.h"
#endif     // -----  not OXBRIDGE_MT2  -----


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
    double CalMTT();
    double Razor_CalcR();
    double Razor_CalcMRT();
    double Razor_CalcMR();
    std::vector<TLorentzVector> Razor_CombineJets();

    double MT2_CalcMT2(double Inv_mass, TLorentzVector sidea, TLorentzVector sideb);
    std::vector<TLorentzVector> MT2_2SideEta0();
    std::vector<TLorentzVector> MT2_2SideJ1J2();
    double CalcAlphaT();
    /* ====================  OPERATORS     ======================================= */
    double VBFBoostMET();
    double VBFBoostHT();

    DelAna& operator = ( const DelAna &other ); /* assignment operator */

    // To store the event information
    std::vector<LHEFEvent> *vEvent;
    std::vector<GenParticle> *vGenParticle;
    std::vector<Photon> *vPhoton;
    std::vector<Electron> *vElectron;
    std::vector<Muon> *vMuon;
    std::vector<Jet> *vJet;
    std::vector<Jet> vBJet;
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
    double MTT; //Top transverse mass according to arXiv:1204.5182 

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

    /* ====================  SUSY Variables  ======================================= */
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Razor ~~~~~
    std::vector<TLorentzVector> RazorJets;
    double RazorMR;
    double RazorMRT;
    double RazorR;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ MT2 ~~~~~
    std::vector<TLorentzVector> MT2sidesEta;
    std::vector<TLorentzVector> MT2sidesJ1J2;
    double Mt2;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ AlphaT ~~~~~
    double AlphaT;
  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    std::string CurrentTag;

}; /* -----  end of class DelAna  ----- */

#endif   /* ----- #ifndef  __DELANA_INC__  ----- */
