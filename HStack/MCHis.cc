// ===========================================================================
// 
//       Filename:  MCHis.cc
// 
//    Description: :
// 
//        Version:  1.0
//        Created:  07/15/2013 05:13:36 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "MCHis.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  MCHis
//      Method:  MCHis
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCHis::MCHis(std::string dir_, std::string sample_, std::string pileup_, std::string detector_):
  dir(dir_), sample(sample_), pileup(pileup_),detector(detector_),analysis("")
{
  GetFile();
}  // ~~~~~  end of method MCHis::MCHis  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  MCHis
//      Method:  MCHis
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCHis::MCHis ( const MCHis &other )
{
}  // ~~~~~  end of method MCHis::MCHis  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  MCHis
//      Method:  ~MCHis
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
MCHis::~MCHis ()
{
}  // ~~~~~  end of method MCHis::~MCHis  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  MCHis
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  MCHis&
MCHis::operator = ( const MCHis &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method MCHis::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  MCHis::GetFile
//  Description:  
// ===========================================================================
bool MCHis::GetFile()
{
  char pat[100];
  sprintf(pat, "%s/%s_%s_%s.root", dir.c_str(), sample.c_str(), pileup.c_str(), detector.c_str());
  File = new TFile(pat, "R");
  return true;
}       // -----  end of function MCHis::GetFile  -----


// ===  FUNCTION  ============================================================
//         Name:  MCHis::GetScale
//  Description:  
// ===========================================================================
bool MCHis::GetScale(const int Lumi)
{
  TH1F* hisxs = (TH1F*)File->Get("CrossSection");
  double xs = hisxs->GetBinContent(hisxs->FindBin(1));
  TH1F* hisevt = (TH1F*)File->Get("NEVT");
  double evt = hisevt->GetEntries();
  std::cout << " File name " << File->GetName() << " event " << evt << " Cross " << xs <<" scale " << xs/evt<< std::endl;
  Scale = Lumi*xs/evt;
  return true;

}       // -----  end of function MCHis::GetScale  -----

// ===  FUNCTION  ============================================================
//         Name:  MCHis::GetTH1
//  Description:  Return the combin TH1F file
// ===========================================================================
TH1F* MCHis::GetTH1(std::string hname, bool WithScale)
{
  if (analysis != "") hname = analysis + "/" + hname;
  TH1F* his = (TH1F*)File->Get(hname.c_str());

  if (his->Integral() != 0.0)
  {
    if (WithScale)
      his->Scale(Scale * his->Integral(0, his->GetNbinsX()+1)/his->Integral());
  }

  return his;
}       // -----  end of function MCHis::GetTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  MCHis::GetTH1
//  Description:  Return the combin TH1F file
// ===========================================================================
TH1F* MCHis::GetTH1(std::string hname, int NCut, bool WithScale)
{
  if (analysis != "") hname = analysis + "/" + hname;
  std::stringstream ss;
  ss<< hname <<"_" <<NCut;
  TH1F* his = (TH1F*)File->Get(ss.str().c_str());

  if (his->Integral() != 0.0)
  {
    if (WithScale)
      his->Scale(Scale* his->Integral(0, his->GetNbinsX()+1)/his->Integral());
  }

  return his;
}       // -----  end of function MCHis::GetTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  MCHis::SetAnalysis
//  Description:  
// ===========================================================================
bool MCHis::SetAnalysis(const std::string& analysis_)
{
  analysis = analysis_;
  return true;
}       // -----  end of function MCHis::SetAnalysis  -----

// ===  FUNCTION  ============================================================
//         Name:  MCHis::GetTH2D
//  Description:  
// ===========================================================================
TH2D* MCHis::GetTH2D(std::string hname, int NCut, bool WithScale)
{
  if (analysis != "") hname = analysis + "/" + hname;
  std::stringstream ss;
  ss<< hname <<"_" <<NCut;
  TH2D* his = (TH2D*)File->Get(ss.str().c_str());

  if (his->Integral() != 0.0)
  {
    if (WithScale)
      his->Scale(Scale* his->Integral(0, his->GetNbinsX()+1)/his->Integral());
  }

  return his;
}       // -----  end of function MCHis::GetTH2D  -----
