/*
 * ===========================================================================
 *
 *       Filename:  HTSample.h
 *
 *    Description:  A simple class like MCSample in HistStack for drawing
 *    purpose
 *
 *        Version:  1.0
 *        Created:  07/15/2013 03:17:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * ===========================================================================
 */

#ifndef  __HTSAMPLE_INC__
#define  __HTSAMPLE_INC__

#include "MCHis.h"
#include "HTHis.h"
/*
 * ===========================================================================
 *        Class:  HTSample
 *  Description:  Even though the class name is called HTSample, this class
 *  should be able to handle both HTbin or inclusive samples. 
 * ===========================================================================
 */
class HTSample
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    HTSample(std::string dir_, std::string sample_, std::string pileup_, std::string detector);
    HTSample ( const HTSample &other );   /* copy constructor */
    ~HTSample ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool InitSample(const int Lumi);
    bool SetOptions(bool isSignal_, int color_);

    /* ====================  MUTATORS      ======================================= */
    TH1F* GetTH1(std::string hname, bool WithScale=true) const;
    TH1F* GetTH1(std::string hname, int NCut, bool WithScale=true) const;

    TH2D* GetTH2D(std::string hname, int NCut, bool WithScale=true) const;
    /* ====================  OPERATORS     ======================================= */

    HTSample& operator = ( const HTSample &other ); /* assignment operator */

    // For drawing style
    bool isSignal;
    double Color;
  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    std::string dir;
    std::string sample;
    std::string pileup;
    std::string detector;


    // For inclusive or exclusive sample during the initialization
    int Lumi;
    bool isHT;
    MCHis* MC;
    HTHis* HT;

}; /* -----  end of class HTSample  ----- */



#endif   // ----- #ifndef __HTSAMPLE_INC__  -----
