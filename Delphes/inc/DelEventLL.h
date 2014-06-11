// ===========================================================================
// 
//       Filename:  DelEventLL.h
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

#ifndef  __DelEventLL_INC__
#define  __DelEventLL_INC__

#include "DelEvent.h"
#include <iomanip>

// ===========================================================================
//        Class:  DelEventLL
//  Description:  A class for handling the LL samples
// ===========================================================================
class DelEventLL : public DelEvent
{
  public:

    // ====================  LIFECYCLE     ===============================
    DelEventLL (double Eta, double Pt); // constructor
    DelEventLL ( const DelEventLL &other );  // copy constructor
    ~DelEventLL ();                     // destructor

    // ====================  ACCESSORS     ===============================
    bool LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    DelEventLL& operator = ( const DelEventLL &other ); // assignment operator

    bool CheckFlag(std::string flag) const;

  protected:
    // ====================  DATA MEMBERS  ===============================
    int CheckLeptonMode();
    bool PrintGen() const;

  private:
    // ====================  DATA MEMBERS  ===============================
    
    int LepMode;

}; // -----  end of class DelEventLL  -----


#endif   // ----- #ifndef __DelEventLL_INC__  -----
