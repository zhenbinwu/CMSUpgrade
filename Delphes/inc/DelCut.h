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

#ifndef  __DELCUT_INC__
#define  __DELCUT_INC__

#include "DelEvent.h"
#include "DelAna.h"
#include "HistTool.hh"

#include <sstream>
#include <bitset>
#include <memory>


#define NBITS 20
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
    DelCut (DelAna *ana, std::shared_ptr<TFile> OutFile, /* constructor      */
            std::string name, std::string cut_ );
    DelCut ( const DelCut &other );                      /* copy constructor */
    virtual ~DelCut ();                                  /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    virtual bool BookHistogram();
    virtual bool InitCutOrder(std::string ana="DM");
    virtual int FillCut();
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
    std::bitset<NBITS> cutbit;
    DelAna *Ana;
    virtual bool CheckCut();

    // Filling variables globally
    bool BookLeptonEff();
    bool BookJetEff();
    bool BookBJet();
    int BookSUSYVar();

    int FillJets() const;
    bool FillLepton();

    // Filling variables for each cut
    int FillJets(int NCut);
    int FillBJet(int NCut);
    int FillEle(int NCut);
    int FillMet(int NCut);
    
    int FillLepton(int NCut) const;
    int FillSUSYVar(int NCut ) const;
    // For MET Performance study
    int BookMetPerf() const;
    int FillMetPerf() const;

    // Systematics samples for hadronic events
    bool CheckSysLep() const;
    double SysMet() const;
    bool DetectorAccp(double Eta) const;

  private:
    /* ====================  DATA MEMBERS  ======================================= */
    std::string ProName; // Process name
    std::string AnaCut;  // Flag to choose which analysis cut to apply 
    // "DM"    -> SUSY VBF DM Cut
    // "Higgs" -> Higgs invisible Cut
    // "RA2"   -> SUSY RA2 Cut (To be implemented)

    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

}; /* -----  end of class DelCut  ----- */


#endif   // ----- #ifndef __DELCUT_INC__  -----
