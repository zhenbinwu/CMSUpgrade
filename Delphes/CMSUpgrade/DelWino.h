/*
 * ===========================================================================
 *
 *       Filename:  DelWino.h
 *
 *    Description:  Interface for Wino sample 
 *
 *        Version:  1.0
 *        Created:  06/10/2013 04:13:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * ===========================================================================
 */


#ifndef  DELWINO_INC
#define  DELWINO_INC


#include "DelEvent.h"

/*
 * ===========================================================================
 *        Class:  DelWino
 *  Description:  
 * ===========================================================================
 */
class DelWino : public DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelWino (double Eta, double Pt);  /* constructor      */
    DelWino ( const DelWino &other ); /* copy constructor */
    ~DelWino ();                      /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelWino& operator = ( const DelWino &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    bool PreSelected();

}; /* -----  end of class DelWino  ----- */


#endif   /* ----- #ifndef DELWINO_INC  ----- */
