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
    bool InitCutOrder();
    bool CutFlow(std::bitset<10> cutbit);
    int FillCut();
    bool CheckCut(std::bitset<10> cutflag);
    bool CheckPhenoCut(std::bitset<10> cutflag);
    int WriteHistogram();
    int DrawHistogram();
    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelCut& operator = ( const DelCut &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    DelAna *Ana;
    HistTool *His;

    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

    // Filling 
    int FillJets(int NCut);
    int FillEle(int NCut);
    int FillMet(int NCut);
}; /* -----  end of class DelCut  ----- */





