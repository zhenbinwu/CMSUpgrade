/*
 * ===========================================================================
 *
 *       Filename:  Compare.h
 *
 *    Description:  A class to handle the comparison of plots 
 *
 *        Version:  1.0
 *        Created:  06/19/2013 01:27:23 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * ===========================================================================
 */

#include <map>
#include <vector>
#include <iostream>
#include <boost/foreach.hpp>

#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TProfile.h"

#include "ComHis.h"
/*
 * ===========================================================================
 *        Class:  Compare
 *  Description:  
 * ===========================================================================
 */
class Compare
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    Compare (std::map< std::string, std::map<std::string, TFile*> > TagFile,
        std::vector<std::string> VTag, std::string NormName );         
    Compare ( const Compare &other );   /* copy constructor */
    ~Compare ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool CompHist(std::string Hname);

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    Compare& operator = ( const Compare &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */
    bool GetPrefix();
    bool GetScaleIgl();
    bool GetScaleOne();
    bool GetScaleXS();
    bool GetScale();
    bool CompHistPre(std::string prefix, std::string hname);
    bool CompEachHist(std::string prefix, std::string Hname);
    bool CompHistTH1(std::string prefix, std::string hname);
    bool FillVHis(std::vector<ComHis*> &VHis, std::string prefix, std::string Hname);
    bool FillMetResVHis(std::vector<ComHis*> &VHis, std::string prefix, std::string Hname);
    ComHis* InitComHisTH1(std::string tag, std::string prefix, std::string hname);
    ComHis* InitComHisTPro(std::string tag, std::string prefix, std::string hname);
    bool UserHist(ComHis *his, std::string hname);
    bool UserVHist(std::vector<ComHis*> &VHis, std::string hname);
    bool UserLegend();
    TLegend* PlotComp(TCanvas* ca, std::vector<ComHis*> &VHis, std::string prefix, std::string hname);
    bool GetRMSDiff(std::vector<ComHis*> VHis, std::string prefix);
    bool CalRMSDiff();

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    // Tag, prefix, TFile
    std::map<std::string, std::map<std::string, TFile*> > TagFile;
    std::vector<std::string> VTag;
    std::string NormName; // which histogram used for scale 

    // Maping the Hname to an array of 4 double for the Legend location
    std::map<std::string, std::vector<double> > LgLoc;
    // Prefix, tag, TFile
    std::map<std::string, std::map<std::string, double> > HScale;

    std::vector<std::string> VPre; // The vector of prefix for the comparison

    std::map< std::string, std::map<std::string, double> > MRMS;
}; /* -----  end of class Compare  ----- */




