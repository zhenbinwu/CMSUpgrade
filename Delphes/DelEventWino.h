/*
 * ===========================================================================
 *
 *       Filename:  DelEventWino.h
 *
 *    Description:  Interface for Wino sample 
 *
 *        Version:  1.0
 *        Created:  06/10/2013 04:13:08 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * ===========================================================================
 */


#ifndef  DelEventWino_INC
#define  DelEventWino_INC


#include "DelEvent.h"

/*
 * ===========================================================================
 *        Class:  DelEventWino
 *  Description:  
 * ===========================================================================
 */
class DelEventWino : public DelEvent
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelEventWino (double Eta, double Pt);  /* constructor      */
    DelEventWino ( const DelEventWino &other ); /* copy constructor */
    ~DelEventWino ();                      /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelEventWino& operator = ( const DelEventWino &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    bool PreSelected();

}; /* -----  end of class DelEventWino  ----- */


#endif   /* ----- #ifndef DelEventWino_INC  ----- */
