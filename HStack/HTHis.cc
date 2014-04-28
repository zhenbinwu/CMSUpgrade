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
HTHis::HTHis(std::string dir_, std::string sample_, std::string pileup_ , std::string detector_) :
  dir(dir_), sample(sample_), pileup(pileup_), detector(detector_)
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

  // Pattern to match * in the input string
  std::vector<std::string> pattern;
  if (sample.find("*") == std::string::npos)
    pattern.push_back("");   //B
  else
  {
    pattern.push_back("");   //B
    pattern.push_back("J");  //BJ
    pattern.push_back("JJ"); //BJJ
    //pattern.push_back("1");   //B
    //pattern.push_back("2");  //BJ
    //pattern.push_back("3");  //BJ
    //pattern.push_back("4");  //BJ
  }

  BOOST_FOREACH(std::string p, pattern)
  {
    TString temp = sample;
    temp.ReplaceAll("*", p);
    sprintf(pat, "%s/%s*%s_%s.root", dir.c_str(), temp.Data(), pileup.c_str(), detector.c_str());
    glob(pat, GLOB_TILDE, NULL, &glob_result);
    for(unsigned int i=0;i<glob_result.gl_pathc;++i){
      //std::cout << " file " << glob_result.gl_pathv[i] << std::endl;
      vFiles.push_back(new TFile(glob_result.gl_pathv[i], "R"));
    }
    globfree(&glob_result);
  }

  return true;
}       // -----  end of function HTHis::GetFiles  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::Print
//  Description:  Print out local variables for testing purpose
// ===========================================================================
bool HTHis::Print() const
{
  
  std::cout << " Dir " << dir <<" sample " << sample << std::endl;
  std::cout << "vFiles size " << vFiles.size() << std::endl;
  std::cout << "vHists size" << vHists.size() << std::endl;
  return true;
}       // -----  end of function HTHis::Print  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH1Fs
//  Description:  Get the TH1F from the vFiles
// ===========================================================================
bool HTHis::GetTH1Fs(std::string hisname)
{
  assert(vFiles.front()->Get(hisname.c_str())->IsA() == TH1F::Class());
  vHists.clear();
  BOOST_FOREACH(TFile* f, vFiles)
  {
    std::cout << "find file " << f->GetName() << std::endl;
    TH1F* histemp_ = (TH1F*)f->Get(hisname.c_str());
    //TH1F* histemp = (TH1F*) histemp_->Clone();
    ////Sumup the underflow bin
    //double underlow = histemp->GetBinContent(0);
    //double firstbin = histemp->GetBinContent(1);
    //double lastbin = histemp->GetBinContent(1);
    //double overlow = histemp->GetBinContent(histemp->GetNbinsX()+1);
    //histemp->SetBinContent(1, underlow+firstbin);
    //histemp->SetBinContent(histemp->GetNbinsX(), overlow+lastbin);
    //vHists.push_back(histemp);
    

    std::cout << " f " << hisname <<" " << histemp_->GetEntries()
      <<"      " <<histemp_->Integral() << std::endl;
    //std::cout << " f " << hisname <<" " << ((TH1F*)f->Get(hisname.c_str()))->GetEntries() 
      //<<"      " << ((TH1F*)f->Get(hisname.c_str()))->Integral() << std::endl;
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
      double evt = hisevt->GetEntries(); 
      vScales.push_back(Lumi*xs/evt);
      std::cout << " File name " << f->GetName() << " Cross Section " << xs
      << " event " << evt << "Lumi " <<Lumi<< " scale " << Lumi*xs/evt<< std::endl;
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
    {
      //std::cout << " org "<< his->Integral() <<" scale " <<vScales.at(i) <<" final " <<his->Integral()*vScales.at(i) << std::endl;
      assert(!vScales.empty());
      his->Scale(vScales.at(i));
    }
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
    his->SetFillColor(i+1);
    his->SetLineColor(i+1);
    HTstack->Add(his);
  }

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
      //std::cout << " index " << i  << " integal "<< his->Integral() << " Scale " << vScales.at(i) << std::endl;
      if (WithScale)
        his->Scale(vScales.at(i));
      ComHT->Add(his);
    }
    delete his;
  }

  return ComHT;

}       // -----  end of function HTHis::GetTH1  -----


// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH2Ds
//  Description:  
// ===========================================================================
bool HTHis::GetTH2Ds(std::string hisname)
{
  vHist2Ds.clear();
  assert(vFiles.front()->Get(hisname.c_str())->IsA() == TH2D::Class());

  BOOST_FOREACH(TFile* f, vFiles)
  {
    vHist2Ds.push_back((TH2D*)f->Get(hisname.c_str()));
  }
  
  return true;
}       // -----  end of function HTHis::GetTH2Ds  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH2D
//  Description:  
// ===========================================================================
TH2D* HTHis::GetTH2D(std::string hname, int NCut, bool WithScale)
{
  std::stringstream ss;
  ss<< hname <<"_" <<NCut;
  GetTH2Ds(ss.str());


  TH2D* ComHT = (TH2D*)vHist2Ds.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHist2Ds.size(); ++i)
  {
    TH2D* his = (TH2D*)vHist2Ds.at(i)->Clone();
    if (his->Integral() != 0.0)
    {
      if (WithScale)
        his->Scale(vScales.at(i));
      ComHT->Add(his);
    }
    delete his;
  }
  return ComHT;
}       // -----  end of function HTHis::GetTH2D  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTH2D
//  Description:  
// ===========================================================================
TH2D* HTHis::GetTH2D(std::string hname, bool WithScale)
{
  GetTH2Ds(hname);


  TH2D* ComHT = (TH2D*)vHist2Ds.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHist2Ds.size(); ++i)
  {
    TH2D* his = (TH2D*)vHist2Ds.at(i)->Clone();
    if (his->Integral() != 0.0)
    {
      if (WithScale)
        his->Scale(vScales.at(i));
      ComHT->Add(his);
    }
    delete his;
  }
  return ComHT;
}       // -----  end of function HTHis::GetTH2D  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTPros
//  Description:  
// ===========================================================================
bool HTHis::GetTPros(std::string hisname) 
{
  vHistPros.clear();

  assert(vFiles.front()->Get(hisname.c_str())->IsA() == TProfile::Class());

  BOOST_FOREACH(TFile* f, vFiles)
  {
    vHistPros.push_back((TProfile*)f->Get(hisname.c_str()));
  }
  return true;

}       // -----  end of function HTHis::GetTPros  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTPro
//  Description: 
// ===========================================================================
TProfile* HTHis::GetTPro(std::string hname, int NCut, bool WithScale)
{
  std::stringstream ss;
  ss<< hname <<"_" <<NCut;
  GetTPros(ss.str());


  TProfile* ComHT = (TProfile*)vHist2Ds.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHist2Ds.size(); ++i)
  {
    TProfile* his = (TProfile*)vHist2Ds.at(i)->Clone();
    if (his->Integral() != 0.0)
    {
      if (WithScale)
        his->Scale(vScales.at(i));
      ComHT->Add(his);
    }
    delete his;
  }
  return ComHT;
}       // -----  end of function HTHis::GetTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetTPro
//  Description: 
// ===========================================================================
TProfile* HTHis::GetTPro(std::string hname, bool WithScale)
{
  GetTPros(hname);

  TProfile* ComHT = (TProfile*)vHist2Ds.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHist2Ds.size(); ++i)
  {
    TProfile* his = (TProfile*)vHist2Ds.at(i)->Clone();
    if (his->Integral() != 0.0)
    {
      if (WithScale)
        his->Scale(vScales.at(i));
      ComHT->Add(his);
    }
    delete his;
  }
  return ComHT;
}       // -----  end of function HTHis::GetTPro  -----

// ===  FUNCTION  ============================================================
//         Name:  HTHis::GetKFactor
//  Description:  Return the KFactor
// ===========================================================================
double HTHis::GetKFactor(std::string hname, int NCut)
{
  if (NCut == -1)
    GetTH1Fs(hname);
  else
  {
    std::stringstream ss;
    ss<< hname <<"_" <<NCut;
    GetTH1Fs(ss.str());
  }

  double FinalXS =0.0;
  TH1F* ComHT = (TH1F*)vHists.front()->Clone();
  ComHT->Reset();

  for (int i = 0; i < (int)vHists.size(); ++i)
  {
    TH1F* his = (TH1F*)vHists.at(i)->Clone();

    TH1F* hisxs = (TH1F*)vFiles.at(i)->Get("CrossSection");
    double LOxs = hisxs->GetBinContent(hisxs->FindBin(1));

    TH1F* hisevt = (TH1F*)vFiles.at(i)->Get("NEVT");
    double total = hisevt->GetBinContent(hisxs->FindBin(1));
    if (his->Integral() != 0.0)
    {
      double kfactor = his->Integral(0, his->GetNbinsX()+1);
      std::cout << "At HT" << i 
        << " Sample Cross Section: "<< LOxs
        << " K-Factor: " << kfactor/ his->GetEntries()
        <<" NLO Cross Section: " << LOxs * kfactor << std::endl;
      FinalXS += LOxs * kfactor/ total;
      ComHT->Add(his);
    }
    delete his;
  }

  std::cout << "Total K-Factor " << ComHT->Integral()/ComHT->GetEntries() << " FinalXS " << FinalXS << std::endl;

  return FinalXS;
  

}       // -----  end of function HTHis::GetKFactor  -----
