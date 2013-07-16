// ===========================================================================
// 
//       Filename:  HTHis.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/10/2013 04:36:44 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "HTHis.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTHis
//      Method:  HTHis
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HTHis::HTHis(std::string dir_, std::string sample_, std::string pileup_) :
  dir(dir_), sample(sample_), pileup(pileup_)
{
  GetFiles();
}  // ~~~~~  end of method HTHis::HTHis  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTHis
//      Method:  HTHis
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HTHis::HTHis ( const HTHis &other )
{
}  // ~~~~~  end of method HTHis::HTHis  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTHis
//      Method:  ~HTHis
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
HTHis::~HTHis ()
{
}  // ~~~~~  end of method HTHis::~HTHis  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  HTHis
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  HTHis&
HTHis::operator = ( const HTHis &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method HTHis::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetFiles
//  Description:  Function to get TFiles matching to the directory and
//  filenames
// ===========================================================================
bool HTHis::GetFiles()
{
//----------------------------------------------------------------------------
//  Getting the files matching the pattern
//----------------------------------------------------------------------------
  glob_t glob_result;
  char pat[100];
  sprintf(pat, "%s/%s*%s.root", dir.c_str(), sample.c_str(), pileup.c_str());
  glob(pat, GLOB_TILDE, NULL, &glob_result);
  for(unsigned int i=0;i<glob_result.gl_pathc;++i){
    //std::cout << " file " << glob_result.gl_pathv[i] << std::endl;
    vFiles.push_back(new TFile(glob_result.gl_pathv[i], "R"));
  }
  globfree(&glob_result);
  return true;
}       // -----  end of function HTHis::GetFiles  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::Print
//  Description:  Print out local variables for testing purpose
// ===========================================================================
bool HTHis::Print() const
{
  
  std::cout << " Dir " << dir <<" sample " << sample << std::endl;
  //std::cout << "vFiles size " << vFiles.size() << std::endl;
  //std::cout << "vHists size" << vHists.size() << std::endl;
  return true;
}       // -----  end of function HTHis::Print  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH1Fs
//  Description:  Get the TH1F from the vFiles
// ===========================================================================
bool HTHis::GetTH1Fs(std::string hisname)
{
  vHists.clear();
  BOOST_FOREACH(TFile* f, vFiles)
  {
    vHists.push_back((TH1F*)f->Get(hisname.c_str()));
  }
  return true;
}       // -----  end of function HTHis::GetTH1Fs  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetScale
//  Description: 
// ===========================================================================
bool HTHis::GetScale(const int Lumi) 
{
  vScales.clear();
  BOOST_FOREACH(TFile* f, vFiles)
  {
      TH1F* hisxs = (TH1F*)f->Get("CrossSection");
      double xs = hisxs->GetBinContent(hisxs->FindBin(1));
      TH1F* hisevt = (TH1F*)f->Get("NEVT");
      double evt = hisevt->GetEntries(); //Temp solution
      vScales.push_back(Lumi*xs/evt);
      //std::cout << " File name " << f->GetName() 
      //<< " event " << evt << " scale " << xs/evt<< std::endl;
  }
  return true;
}       // -----  end of function HTHis::GetScale  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH1
//  Description:  Return the combin TH1F file
// ===========================================================================
TH1F* HTHis::GetTH1(std::string hname, bool WithScale)
{
  GetTH1Fs(hname);
  TH1F* ComHT = (TH1F*)vHists.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHists.size(); ++i)
  {
    TH1F* his = (TH1F*)vHists.at(i)->Clone();
    if (WithScale)
      his->Scale(vScales.at(i));

    ComHT->Add(his);
    delete his;
  }

  return ComHT;
}       // -----  end of function HTHis::GetTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetStack
//  Description: 
// ===========================================================================
THStack* HTHis::GetStack(std::string hname, bool WithScale)
{
  GetTH1Fs(hname);
  TH1F* ComHT = (TH1F*)vHists.front()->Clone();
  THStack *HTstack = new THStack(ComHT->GetName(), ComHT->GetTitle());

  for (int i = 0; i < (int)vHists.size(); ++i)
  {
    TH1F* his = (TH1F*)vHists.at(i)->Clone();
    if (WithScale)
      his->Scale(vScales.at(i));
    his->SetFillColor(i*2+10);
    his->SetLineColor(i*2+10);
    //std::cout << " netry " << his->GetEntries() << std::endl;
    HTstack->Add(his);
  }

  //HTstack->GetXaxis()->SetTitle("d");
  //HTstack->GetYaxis()->SetTitle(ComHT->GetYaxis()->GetTitle());
  return HTstack;

}       // -----  end of function HTHis::GetStack  -----


// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH1
//  Description:  Return the combin TH1F file
// ===========================================================================
TH1F* HTHis::GetTH1(std::string hname, int NCut, bool WithScale)
{
  std::stringstream ss;
  ss<< hname <<"_" <<NCut;
  GetTH1Fs(ss.str());

  TH1F* ComHT = (TH1F*)vHists.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHists.size(); ++i)
  {
    TH1F* his = (TH1F*)vHists.at(i)->Clone();
    if (his->Integral() != 0.0)
    {
      if (WithScale)
        his->Scale(vScales.at(i) * his->Integral(0, his->GetNbinsX()+1)/his->Integral());
      ComHT->Add(his);
    }
    delete his;
  }

  return ComHT;

}       // -----  end of function HTHis::GetTH1  -----

