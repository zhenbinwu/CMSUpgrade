/*
 * =====================================================================================
 *
 *       Filename:  DelTT.h
 *
 *    Description:  A class for ttbar samples only, inherited from DelEvent
 *
 *        Version:  1.0
 *        Created:  06/05/2013 04:43:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * =====================================================================================
 */

#include "DelEvent.h"

/*
 * =====================================================================================
 *        Class:  DelTT
 *  Description:  
 * =====================================================================================
 */
class DelTT : public DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelTT (double Eta, double Pt);                             /* constructor      */
    DelTT ( const DelTT &other );   /* copy constructor */
    ~DelTT ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelTT& operator = ( const DelTT &other ); /* assignment operator */
    bool LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

  protected:
    /* ====================  DATA MEMBERS  ======================================= */


  private:
    /* ====================  DATA MEMBERS  ======================================= */
    int GenLeps() const;
    bool CheckFlag(std::string flag);


}; /* -----  end of class DelTT  ----- */

