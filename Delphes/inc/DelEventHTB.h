/*
 * ===========================================================================
 *
 *       Filename:  DelEventHTB.h
 *
 *    Description:  A class for HTBin sample with single boson
 *    B, BJ, BJJ with B = photon, W, Z, H
 *
 *        Version:  1.0
 *        Created:  07/11/2013 12:06:49 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * ===========================================================================
 */


#ifndef  DelEventHTB_INC
#define  DelEventHTB_INC

#include "DelEvent.h"
#include "DelEventZJJ.h"
#include <iomanip>
/*
 * ===========================================================================
 *        Class:  DelEventHTB
 *  Description:  
 * ===========================================================================
 */
class DelEventHTB : public DelEventZJJ
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelEventHTB (double Eta, double Pt); /* constructor      */
    DelEventHTB ( const DelEventHTB &other ); /* copy constructor */
    ~DelEventHTB ();                     /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet, TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle);

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelEventHTB& operator = ( const DelEventHTB &other ); /* assignment operator */

    bool CheckFlag(const std::string flag);

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    int GenBoson() const;
    bool PrintGen() const;
    bool CheckPhoton(bool IsPhoton) const;
    int CheckWMode() const;
    int CheckZMode() const;
    bool CheckBoson(int boson) const;

    int Boson;
    int WMode;
    int ZMode;


}; /* -----  end of class DelEventHTB  ----- */

#endif   /* ----- #ifndef DelEventHTB_INC  ----- */
