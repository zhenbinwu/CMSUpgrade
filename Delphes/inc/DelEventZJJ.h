/*
 * =====================================================================================
 *
 *       Filename:  DelEventZJJ.h
 *
 *    Description:  a
 *
 *        Version:  1.0
 *        Created:  06/05/2013 04:53:13 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * =====================================================================================
 */

#ifndef  DelEventZJJ_INC
#define  DelEventZJJ_INC



#include "DelEvent.h"
/*
 * =====================================================================================
 *        Class:  DelEventZJJ
 *  Description:  
 * =====================================================================================
 */
class DelEventZJJ : public DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelEventZJJ (bool Faking, double Eta, double Pt);
    DelEventZJJ ( const DelEventZJJ &other );   /* copy constructor */
    ~DelEventZJJ ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    //bool CheckFlag(std::string flag);
    virtual bool LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet,
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

    bool CheckFlag(std::string flag);
    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelEventZJJ& operator = ( const DelEventZJJ &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

    bool CheckZ();
    TVector2 ZLLMet(TClonesArray *branchParticle, TClonesArray *branchElectron, 
        TClonesArray *branchMuon, TClonesArray *branchJet);
    TVector2 ZLLLep(std::map<int, GenParticle*> EleGen, std::map<int, GenParticle*> MuonGen, 
        TClonesArray *branchParticle, TClonesArray *branchElectron, 
        TClonesArray *branchMuon, TClonesArray *branchJet);
    bool LoadZLLEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet, TClonesArray *branchElectron, 
        TClonesArray *branchMuon, TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);
    int LoadZLLElectron(TClonesArray *branchElectron);
    int LoadZLLMuon(TClonesArray *branchMuon);

    //bool CheckFlag(std::string flag) const;
    /* ====================  DATA MEMBERS  ======================================= */
    std::list<int> VZLep;
    double ZPT;
    bool FakingZvv;
    std::set<int> MatchedEle;
    std::set<int> MatchedMuon;

    // For Met study
    bool DiMuonMet();
    bool DiEleMet();

  private:

}; /* -----  end of class DelEventZJJ  ----- */

#endif   /* ----- #ifndef DelEventZJJ_INC  ----- */
