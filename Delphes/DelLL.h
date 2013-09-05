// ===========================================================================
// 
//       Filename:  DelLL.h
// 
//    Description:  A class for LL HTBin samples
// 
//        Version:  1.0
//        Created:  09/03/2013 01:32:01 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#ifndef  __DELLL_INC__
#define  __DELLL_INC__

#include "DelEvent.h"
#include <iomanip>

// ===========================================================================
//        Class:  DelLL
//  Description:  A class for handling the LL samples
// ===========================================================================
class DelLL : public DelEvent
{
  public:

    // ====================  LIFECYCLE     ===============================
    DelLL (double Eta, double Pt); // constructor
    DelLL ( const DelLL &other );  // copy constructor
    ~DelLL ();                     // destructor

    // ====================  ACCESSORS     ===============================
    bool LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    DelLL& operator = ( const DelLL &other ); // assignment operator

    bool CheckFlag(std::string flag) const;

  protected:
    // ====================  DATA MEMBERS  ===============================
    int CheckLeptonMode();
    bool PrintGen() const;

  private:
    // ====================  DATA MEMBERS  ===============================
    
    int LepMode;

}; // -----  end of class DelLL  -----


#endif   // ----- #ifndef __DELLL_INC__  -----
