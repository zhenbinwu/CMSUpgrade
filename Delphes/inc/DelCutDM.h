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

#ifndef  __DELCUTDM_INC__
#define  __DELCUTDM_INC__

#include "DelEvent.h"
#include "DelAna.h"
#include "HistTool.hh"
#include "DelCut.h"

#include <sstream>
#include <bitset>
/*
 * ===========================================================================
 *        Class:  DelCut
 *  Description:  A class for cut flow
 * ===========================================================================
 */
class DelCutDM : public DelCut
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelCutDM (DelAna *ana, std::shared_ptr<TFile> OutFile, /* constructor      */
        std::string name, std::string cut_ );
    DelCutDM ( const DelCutDM &other );                    /* copy constructor */
    ~DelCutDM ();                                          /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool BookHistogram();
    bool InitCutOrder(std::string ana="DM");
    int FillCut();

    /* ====================  MUTATORS      ======================================= */
    // Move HistTool to public so that the CMSSW can book and fill some
    // histogram apart from the DelCut.
    // Warning: This is not healthy for the code! But we need to scarify for
    // the analysis
    //HistTool *His; 

    /* ====================  OPERATORS     ======================================= */

    DelCutDM& operator = ( const DelCutDM &other ); /* assignment operator */

  protected:
    /* ====================  DATA MEMBERS  ======================================= */
    bool CutFlow(std::bitset<NBITS> cutbit);
    bool CheckCut();

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    DelAna *Ana;
    std::string ProName; // Process name
    std::string AnaCut;  // Flag to choose which analysis cut to apply 
    // "DM"    -> SUSY VBF DM Cut
    // "Higgs" -> Higgs invisible Cut
    // "RA2"   -> SUSY RA2 Cut (To be implemented)

    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

}; /* -----  end of class DelCut  ----- */



#endif   // ----- #ifndef __DELCUTDM_INC__  -----
