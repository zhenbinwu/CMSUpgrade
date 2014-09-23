// ===========================================================================
// 
//       Filename:  HTSample.cc
// 
//    Description: 1gt 
// 
//        Version:  1.0
//        Created:  07/15/2013 03:17:02 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "HTSample.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTSample
//      Method:  HTSample
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HTSample::HTSample(std::string dir_, std::string sample_, std::string pileup_, std::string detector_) :
  dir(dir_), sample(sample_), pileup(pileup_), detector(detector_)
{
    isHT = false;
    isSignal = false;
    MC   = NULL;
    HT   = NULL;
}  // ~~~~~  end of method HTSample::HTSample  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTSample
//      Method:  HTSample
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HTSample::HTSample ( const HTSample &other )
{
  isHT     = other.isHT;
  isSignal = other.isSignal;
  MC       = other.MC;
  HT       = other.HT;
}  // ~~~~~  end of method HTSample::HTSample  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTSample
//      Method:  ~HTSample
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HTSample::~HTSample ()
{
}  // ~~~~~  end of method HTSample::~HTSample  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTSample
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  HTSample&
HTSample::operator = ( const HTSample &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method HTSample::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  HTSample::InitSample
//  Description:  
// ===========================================================================
bool HTSample::InitSample(const int Lumi)
{
  if (sample.find("HT") != std::string::npos)
  {
    HT = new HTHis(dir, sample, pileup, detector);
    HT->GetScale(Lumi);
    isHT = true;
  }
  else{
    MC = new MCHis(dir, sample, pileup, detector);
    MC->GetScale(Lumi);
    isHT = false;
  }
  return true;
}       // -----  end of function HTSample::InitSample  -----

// ===  FUNCTION  ============================================================
//         Name:  HTSample::SetOptions
//  Description:  Set user defined options
// ===========================================================================
bool HTSample::SetOptions(bool isSignal_, int color_)
{
  isSignal = isSignal_;
  Color = color_;
  return true;
}       // -----  end of function HTSample::SetOptions  -----


// ===  FUNCTION  ============================================================
//         Name:  HTSample::GetTH1
//  Description:  
// ===========================================================================
TH1F* HTSample::GetTH1(std::string hname, bool WithScale) const
{
  if (isHT)
    return HT->GetTH1(hname, WithScale);
  else
    return MC->GetTH1(hname, WithScale);

}       // -----  end of function HTSample::GetTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HTSample::GetTH1
//  Description:  
// ===========================================================================
TH1F* HTSample::GetTH1(std::string hname, int NCut, bool WithScale) const
{
  if (isHT)
    return HT->GetTH1(hname, NCut, WithScale);
  else
    return MC->GetTH1(hname, NCut, WithScale);

}       // -----  end of function HTSample::GetTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HTSample::GetTH2D
//  Description:  
// ===========================================================================
TH2D* HTSample::GetTH2D(std::string hname, int NCut, bool WithScale) const
{
  if (isHT)
    return  HT->GetTH2D(hname, NCut, WithScale);
  else
    return MC->GetTH2D(hname, NCut, WithScale);
}       // -----  end of function HTSample::GetTH2D  -----

// ===  FUNCTION  ============================================================
//         Name:  HTSample::SetAnalysis
//  Description:  Set analysis
// ===========================================================================
bool HTSample::SetAnalysis(const std::string& analysis_) const
{
  if (isHT)
    return HT->SetAnalysis(analysis_);
  else
    return MC->SetAnalysis(analysis_);
}       // -----  end of function HTSample::SetAnalysis  -----
