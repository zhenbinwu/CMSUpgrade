/*
 * =====================================================================================
 *
 *       Filename:  DelZJJ.h
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

#ifndef  DELZJJ_INC
#define  DELZJJ_INC



#include "DelEvent.h"
/*
 * =====================================================================================
 *        Class:  DelZJJ
 *  Description:  
 * =====================================================================================
 */
class DelZJJ : public DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelZJJ (bool Faking, double Eta, double Pt);
    DelZJJ ( const DelZJJ &other );   /* copy constructor */
    ~DelZJJ ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    //bool CheckFlag(std::string flag);
    bool LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet,
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

    bool CheckFlag(std::string flag);
    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelZJJ& operator = ( const DelZJJ &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
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

}; /* -----  end of class DelZJJ  ----- */

#endif   /* ----- #ifndef DELZJJ_INC  ----- */
