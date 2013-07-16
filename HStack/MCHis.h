/*
 * ===========================================================================
 *
 *       Filename:  MCHis.h
 *
 *    Description:  A small class like HTHis for handling the his for the
 *    inclusive sample
 *
 *        Version:  1.0
 *        Created:  07/15/2013 03:22:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * ===========================================================================
 */

#ifndef  __MCHIS_INC__
#define  __MCHIS_INC__

#include "TFile.h"
#include "TH1F.h"
#include <sstream>
#include <iostream>

/*
 * ===========================================================================
 *        Class:  MCHis
 *  Description:  A small class like HTHis for handling the his for the inclusive sample
 * ===========================================================================
 */
class MCHis
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    MCHis (std::string dir_, std::string sample_,
           std::string pileup_);                  /* constructor      */
    MCHis ( const MCHis &other );                 /* copy constructor */
    ~MCHis ();                                    /* destructor       */

    /* ====================  ACCESSORS     ======================================= */

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    MCHis& operator = ( const MCHis &other ); /* assignment operator */

    bool GetFile();
    bool GetScale(const int Lumi);
    TH1F* GetTH1(std::string hname, bool WithScale=true);
    TH1F* GetTH1(std::string hname, int NCut, bool WithScale=true);
  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    int Lumi;
    std::string dir;
    std::string sample;
    std::string pileup;
    TFile* File;
    TH1F* Hist;
    double Scale;

}; /* -----  end of class MCHis  ----- */

#endif   // ----- #ifndef __MCHIS_INC__  -----
