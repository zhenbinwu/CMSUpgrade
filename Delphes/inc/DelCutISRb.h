// ===========================================================================
// 
//       Filename:  DelCutISRb.h
// 
//    Description:  A cutflow as in "Reach the Bottom Line of the Sbottom Search"
//    arXiv:1204.5182
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


#ifndef  __DELCUTISRB_INC__
#define  __DELCUTISRB_INC__

#include "DelAna.h"
#include "HistTool.hh"
#include "DelCut.h"

#include <sstream>
#include <bitset>

// ===========================================================================
//        Class:  DelCutISRb
//  Description:  
// ===========================================================================
class DelCutISRb : public DelCut
{
  public:

    // ====================  LIFECYCLE     ===============================
    DelCutISRb (DelAna *ana, std::shared_ptr<TFile> OutFile, // constructor
        std::string name, std::string cut_ );
    DelCutISRb ( const DelCutISRb &other );                 // copy constructor
    ~DelCutISRb ();                                          // destructor

    // ====================  ACCESSORS     ===============================
    bool BookHistogram();
    bool InitCutOrder(std::string ana="Higgs");
    int FillCut();

    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    DelCutISRb& operator = ( const DelCutISRb &other ); // assignment operator

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

}; // -----  end of class DelCutISRb  -----


#endif   // ----- #ifndef __DELCUTISRB_INC__  -----
