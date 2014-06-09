/*
 * =====================================================================================
 *
 *       Filename:  DelEventTT.h
 *
 *    Description:  A class for ttbar samples only, inherited from DelEvent
 *
 *        Version:  1.0
 *        Created:  06/05/2013 04:43:40 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * =====================================================================================
 */

#include "DelEvent.h"

/*
 * =====================================================================================
 *        Class:  DelEventTT
 *  Description:  
 * =====================================================================================
 */
class DelEventTT : public DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelEventTT (double Eta, double Pt);                             /* constructor      */
    DelEventTT ( const DelEventTT &other );   /* copy constructor */
    ~DelEventTT ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelEventTT& operator = ( const DelEventTT &other ); /* assignment operator */
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


}; /* -----  end of class DelEventTT  ----- */

