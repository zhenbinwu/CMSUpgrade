// ===========================================================================
// 
//       Filename:  HTHis.hh
// 
//    Description:  A class for handling the HTbin histograms
// 
//        Version:  1.0
//        Created:  07/10/2013 04:34:45 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#ifndef  __HTHIS_INC__
#define  __HTHIS_INC__


#include <iostream>
#include <vector>
#include <sstream>

#include "glob.h"
#include <boost/foreach.hpp>

#include "TProfile.h"
#include "TAxis.h"
#include "TH1F.h"
#include "THStack.h"
#include "TFile.h"
// ===========================================================================
//        Class:  HTHis
//  Description:  This class will read in all the HTbin histogram from the
//  list of files, combined them properly and them return a TH1F pointer
// ===========================================================================
class HTHis
{
  public:

    // ====================  LIFECYCLE     ===============================
    HTHis (std::string dir_, std::string sample_, std::string pileup_);
    HTHis ( const HTHis &other );   // copy constructor
    ~HTHis ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool GetFiles();
    bool GetTH1Fs(std::string hisname);
    bool GetScale(const int Lumi);
    bool Print() const;
    TH1F* GetTH1(std::string hname, bool WithScale=true);
    TH1F* GetTH1(std::string hname, int NCut, bool WithScale=true);
    THStack* GetStack(std::string hname, bool WithScale=true);


    // ====================  MUTATORS      ===============================

    // ====================  OPERATORS     ===============================

    HTHis& operator = ( const HTHis &other ); // assignment operator

  protected:
    // ====================  DATA MEMBERS  ===============================

  private:
    // ====================  DATA MEMBERS  ===============================
    std::string dir;
    std::string sample;
    std::string pileup;
    std::vector<TFile*> vFiles;  // Vector to store files
    std::vector<TH1F*> vHists;   // Vector to store files
    std::vector<double> vScales; // Vector to store scale factor
}; // -----  end of class HTHis  -----



#endif   // ----- #ifndef __HTHIS_INC__  -----
