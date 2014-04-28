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
#include <cassert>

#include "glob.h"
#include <boost/foreach.hpp>

#include "TProfile.h"
#include "TAxis.h"
#include "TH2D.h"
#include "TProfile.h"
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
    HTHis (std::string dir_, std::string sample_, std::string pileup_, std::string detector_);
    HTHis ( const HTHis &other );   // copy constructor
    ~HTHis ();                            // destructor

    // ====================  ACCESSORS     ===============================
    bool Print() const;
    double GetKFactor(std::string hanme, int NCut=-1);

    THStack* GetStack(std::string hname, bool WithScale=true);

    TH1F* GetTH1(std::string hname, bool WithScale=true);
    TH1F* GetTH1(std::string hname, int NCut, bool WithScale=true);

    TH2D* GetTH2D(std::string hname, bool WithScale);
    TH2D* GetTH2D(std::string hname, int NCut, bool WithScale);

    TProfile* GetTPro(std::string hname, bool WithScale);
    TProfile* GetTPro(std::string hname, int NCut, bool WithScale);
    
    // ====================  MUTATORS      ===============================

    bool GetScale(const int Lumi);
    // ====================  OPERATORS     ===============================

    HTHis& operator = ( const HTHis &other ); // assignment operator

  protected:
    // ====================  DATA MEMBERS  ===============================
    bool GetFiles();
    bool GetTH1Fs(std::string hisname);
    bool GetTH2Ds(std::string hisname);
    bool GetTPros(std::string hisname);

  private:
    // ====================  DATA MEMBERS  ===============================
    std::string dir;
    std::string sample;
    std::string pileup;
    std::string detector;
    std::vector<TFile*> vFiles;  // Vector to store files
    std::vector<TH1F*> vHists;   // Vector to store files
    std::vector<TH2D*> vHist2Ds;   // Vector to store files
    std::vector<TProfile*> vHistPros;   // Vector to store files
    std::vector<double> vScales; // Vector to store scale factor
}; // -----  end of class HTHis  -----



#endif   // ----- #ifndef __HTHIS_INC__  -----
