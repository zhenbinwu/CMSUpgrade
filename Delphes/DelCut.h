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

#include <sstream>
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
    bool FillSampleXS(double xs, double error);
    bool InitCutOrder(std::string ana="DM");
    int FillCut();
    bool FillNEVT(double weight=1) const;
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
    bool CutFlow(std::bitset<20> cutbit);
    bool CheckCut(std::bitset<20> cutflag);
    bool CheckPhenoCut(std::bitset<20> cutflag);
    bool CheckDMCut(std::bitset<20> cutflag);
    bool CheckHiggsCut(std::bitset<20> cutflag);

    bool CheckSysLep() const;
    double SysMet() const;
    bool DetectorAccp(double Eta) const;
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

    // Filling variables globally
    int FillJets() const;
    int FillLepton(int NCut) const;
    bool BookLeptonEff();
    bool BookJetEff();

    bool FillLepton();
    // Filling variables for each cut
    int FillJets(int NCut);
    int FillEle(int NCut);
    int FillMet(int NCut);

    
    // For MET Performance study
    int BookMetPerf() const;
    int FillMetPerf() const;
}; /* -----  end of class DelCut  ----- */





