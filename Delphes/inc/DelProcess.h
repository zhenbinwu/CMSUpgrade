// ===========================================================================
// 
//       Filename:  DelProcess.h
// 
//    Description:  A class for splitting the sample into several output root
//    file depending on the process. Each process is allowed to have different
//    cutflow applied.
// 
//        Version:  1.0
//        Created:  04/29/2014 01:01:40 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#ifndef  __DELPROCESS_INC__
#define  __DELPROCESS_INC__

#include <memory>
#include <cstddef>

#include "DelCut.h"
#include "DelCutDM.h"
#include "DelCutHiggs.h"
#include "DelCutVBFb.h"
#include "DelCutISRb.h"

// ===========================================================================
//        Class:  DelProcess
//  Description:  
// ===========================================================================
class DelProcess
{
  public:

    // ====================  LIFECYCLE     ===============================
    DelProcess (DelAna *ana, const std::string& name);        // constructor
    DelProcess ( const DelProcess &other );   // copy constructor
    ~DelProcess ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool AddCutFlow(std::string cuts);

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================
    bool DrawHistogram();
    bool WriteHistogram();
    bool BookHistogram();
    bool FillHistogram(const DelAna *ana);

    bool FillNEVT(double weight=1) const;
    bool FillCut();
    bool SetSampleXS(double xs, double error);

    DelProcess& operator = ( const DelProcess &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    std::string ProName;
    HistTool *His; 

  private:
    // ====================  METHODS       ===============================

    DelAna *Ana;
    // ====================  DATA MEMBERS  ===============================
    std::unique_ptr<TH1F> HNEVT;
    std::unique_ptr<TH1F> HXS;
    std::unique_ptr<TH1F> HWeight;
    std::shared_ptr<TFile> OutFile;
    std::map<std::string, std::unique_ptr<DelCut> > MDelCut; // For separated output

}; // -----  end of class DelProcess  -----


#endif   // ----- #ifndef __DELPROCESS_INC__  -----
