/*
 * =====================================================================================
 *
 *       Filename:  DelCut.h
 *
 *    Description:  This is the class to handle the cut flow from the
 *    DelEvent.
 *
 *        Version:  1.0
 *        Created:  06/04/2013 05:58:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * =====================================================================================
 */

#include "DelEvent.h"
#include "DelAna.h"
#include "HistTool.hh"


#include <bitset>
/*
 * ===========================================================================
 *        Class:  DelCut
 *  Description:  A class for cut flow
 * ===========================================================================
 */
class DelCut
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelCut (DelAna *ana, std::string name);       /* constructor      */
    DelCut ( const DelCut &other );   /* copy constructor */
    ~DelCut ();                            /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool BookHistogram();
    bool FillSampleXS(double xs);
    bool InitCutOrder();
    bool CutFlow(std::bitset<10> cutbit);
    int FillCut();
    bool CheckCut(std::bitset<10> cutflag);
    bool CheckPhenoCut(std::bitset<10> cutflag);
    int WriteHistogram();
    int DrawHistogram();
    /* ====================  MUTATORS      ======================================= */
    // Move HistTool to public so that the CMSSW can book and fill some
    // histogram apart from the DelCut.
    // Warning: This is not healthy for the code! But we need to scarify for
    // the analysis
    HistTool *His; 

    /* ====================  OPERATORS     ======================================= */

    DelCut& operator = ( const DelCut &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    DelAna *Ana;

    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

    // Filling variables globally
    int FillJets() const;

    // Filling variables for each cut
    int FillJets(int NCut);
    int FillEle(int NCut);
    int FillMet(int NCut);

    
    // For MET Performance study
    int BookMetPerf() const;
    int FillMetPerf() const;
}; /* -----  end of class DelCut  ----- */





