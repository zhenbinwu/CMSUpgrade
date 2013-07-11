/*
 * ===========================================================================
 *
 *       Filename:  DelHTB.h
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


#ifndef  DELHTB_INC
#define  DELHTB_INC

#include "DelEvent.h"
#include "DelZJJ.h"
/*
 * ===========================================================================
 *        Class:  DelHTB
 *  Description:  
 * ===========================================================================
 */
class DelHTB : public DelZJJ
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelHTB (double Eta, double Pt); /* constructor      */
    DelHTB ( const DelHTB &other ); /* copy constructor */
    ~DelHTB ();                     /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelHTB& operator = ( const DelHTB &other ); /* assignment operator */

    bool CheckFlag(const std::string flag);

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    int GenBonson() const;
    bool PrintGen() const;

}; /* -----  end of class DelHTB  ----- */

#endif   /* ----- #ifndef DELHTB_INC  ----- */
