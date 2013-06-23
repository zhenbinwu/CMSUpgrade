/*
 * ===========================================================================
 *
 *       Filename:  ComHis.h
 *
 *    Description:  A class for making comparison between histograms
 *
 *        Version:  1.0
 *        Created:  06/18/2013 11:20:10 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * ===========================================================================
 */

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "TH1F.h"
/*
 * ===========================================================================
 *        Class:  ComHis
 *  Description:  
 * ===========================================================================
 */
class ComHis
{
  public:
    /* ====================  LIFECYCLE     =============================== */
    ComHis ();                             /* constructor      */
    ComHis ( const ComHis &other );   /* copy constructor */
    ~ComHis ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool Initial(TH1F* HIS, std::string atag, std::vector<std::string> aVTag);
    bool Print() const;
    bool SetAxis();

    bool SetStyle();
    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    ComHis& operator = ( const ComHis &other ); /* assignment operator */

    /* ====================  DATA MEMBERS  ======================================= */
    TH1F* his;
    std::string tag;
    // Axis
    std::string xlabel;
    std::string ylabel;
    bool logx;
    bool logy;
    // Drawing style
    bool line;
    double color;
    double style;
    double size;
    std::string drawopt;

    // Hist manager
    int rebin;
    double xmin, xmax;
    double ymin, ymax;
    double scale;


  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    std::vector<std::string> VTag;

    bool Setup();

}; /* -----  end of class ComHis  ----- */


