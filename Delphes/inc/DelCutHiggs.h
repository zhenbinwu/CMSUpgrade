// ===========================================================================
// 
//       Filename:  DelCutHiggs.h
// 
//    Description:  A cutflow for the higgs invisiable analysis
// 
//        Version:  1.0
//        Created:  06/10/2014 08:06:29 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================


#ifndef  __DELCUTHIGGS_INC__
#define  __DELCUTHIGGS_INC__

#include "DelAna.h"
#include "HistTool.hh"
#include "DelCut.h"

#include <sstream>
#include <bitset>

// ===========================================================================
//        Class:  DelCutHiggs
//  Description:  
// ===========================================================================
class DelCutHiggs : public DelCut
{
  public:

    // ====================  LIFECYCLE     ===============================
    DelCutHiggs (DelAna *ana, std::shared_ptr<TFile> OutFile, // constructor
        std::string name, std::string cut_ );
    DelCutHiggs ( const DelCutHiggs &other );                 // copy constructor
    ~DelCutHiggs ();                                          // destructor

    // ====================  ACCESSORS     ===============================
    bool BookHistogram();
    bool InitCutOrder(std::string ana="Higgs");
    int FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    DelCutHiggs& operator = ( const DelCutHiggs &other ); // assignment operator

    // ====================  DATA MEMBERS  ===============================

  protected:
    // ====================  METHODS       ===============================
    bool CheckCut();

    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  METHODS       ===============================

    // ====================  DATA MEMBERS  ===============================
    std::string ProName; // Process name
    std::string AnaCut;  // Flag to choose which analysis cut to apply 

    std::vector<std::string> CutOrder;
    std::map<std::string, std::string>  CutMap;

}; // -----  end of class DelCutHiggs  -----


#endif   // ----- #ifndef __DELCUTHIGGS_INC__  -----
