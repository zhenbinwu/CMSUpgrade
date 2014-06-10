// ===========================================================================
// 
//       Filename:  DelProcess.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  04/29/2014 01:04:10 PM
//       Compiler:  g++ -std=c++11
// 
//         Author:  Zhenbin Wu (benwu)
//          Email:  benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL, CMS@LPC
// 
// ===========================================================================

#include "DelProcess.h"

//----------------------------------------------------------------------------
//       Class:  DelProcess
//      Method:  DelProcess
// Description:  constructor
//----------------------------------------------------------------------------
DelProcess::DelProcess (DelAna *ana, const std::string& name):
  Ana(ana)
{
  TString OutFileName;
  if (name.find(".root") == std::string::npos)
    OutFileName = name + ".root";
  else 
    OutFileName = name;
  std::cout << "outfile name " << OutFileName << std::endl;
  OutFile = std::shared_ptr<TFile>(new TFile(OutFileName, "RECREATE"));

  ProName = name.substr(0, name.find_first_of('_'));

  HNEVT   = std::unique_ptr<TH1F>(new TH1F("NEVT", "Num. of Events", 2, 0, 2 ));
  HXS     = std::unique_ptr<TH1F>(new TH1F("CrossSection", "Cross Section", 2, 0, 2));
  HWeight = std::unique_ptr<TH1F>(new TH1F("Weight", "Weight", 100, 0, 10 ));

  His     = new HistTool(OutFile, ProName);
  std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 

}  // -----  end of method DelProcess::DelProcess  (constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelProcess
//      Method:  DelProcess
// Description:  copy constructor
//----------------------------------------------------------------------------
DelProcess::DelProcess ( const DelProcess &other )
{
}  // -----  end of method DelProcess::DelProcess  (copy constructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelProcess
//      Method:  ~DelProcess
// Description:  destructor
//----------------------------------------------------------------------------
DelProcess::~DelProcess ()
{
  OutFile->cd();
  HNEVT->Write();
  HXS->Write();
  HWeight->Write();
  
  //OutFile->Close();
}  // -----  end of method DelProcess::-DelProcess  (destructor)  -----

//----------------------------------------------------------------------------
//       Class:  DelProcess
//      Method:  operator =
// Description:  assignment operator
//----------------------------------------------------------------------------
  DelProcess&
DelProcess::operator = ( const DelProcess &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // -----  end of method DelProcess::operator =  (assignment operator)  ---


// ===  FUNCTION  ============================================================
//         Name:  DelProcess::AddCutFlow
//  Description:  
// ===========================================================================
bool DelProcess::AddCutFlow(std::string cuts)
{
  std::cout << "cut s " << cuts << std::endl;
  if (cuts == "DM")
  {
    MDelCut[cuts] = std::unique_ptr<DelCutDM>(new DelCutDM(Ana, OutFile, ProName, cuts));
  }
  else
  {

    MDelCut[cuts] = std::unique_ptr<DelCut>(new DelCut(Ana, OutFile, ProName, cuts));
    std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
  }

  MDelCut[cuts]->InitCutOrder(cuts);
  MDelCut[cuts]->BookHistogram();
  return true;
}       // -----  end of function DelProcess::AddCutFlow  -----

// ===  FUNCTION  ============================================================
//         Name:  DelProcess::SetSampleXS
//  Description:  
// ===========================================================================
bool DelProcess::SetSampleXS(double xs, double error)
{
  HXS->Fill(1.0, xs);
  HXS->Fill(0.0, error);
  return true;
}       // -----  end of function DelProcess::SetSampleXS  -----


// ===  FUNCTION  ============================================================
//         Name:  DelProcess::FillNEVT
//  Description:  
// ===========================================================================
bool DelProcess::FillNEVT(double weight) const
{
  HNEVT->Fill(1, weight); //the NEVT with weight 
  HWeight->Fill(weight);
  His->SetWeight(weight); 
  return true;
}       // -----  end of function DelProcess::FillNEVT  -----


// ===  FUNCTION  ============================================================
//         Name:  DelProcess::FillCut
//  Description:  
// ===========================================================================
bool DelProcess::FillCut()
{
  for(std::map<std::string, std::unique_ptr<DelCut> >::const_iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    it->second->FillCut();
  } 
  return true;
}       // -----  end of function DelProcess::FillCut  -----


// ===  FUNCTION  ============================================================
//         Name:  DelProcess::WriteHistogram
//  Description:  
// ===========================================================================
bool DelProcess::WriteHistogram()
{
  His->WriteTH1();
  His->WriteTPro();
  His->WriteTH2();

  for(std::map<std::string, std::unique_ptr<DelCut> >::const_iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    it->second->WriteHistogram();
  } 
  return true;
}       // -----  end of function DelProcess::WriteHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DelProcess::DrawHistogram
//  Description:  
// ===========================================================================
bool DelProcess::DrawHistogram()
{
  His->DrawTH1();
  His->DrawTPro();
  His->DrawTH2();

  for(std::map<std::string, std::unique_ptr<DelCut> >::const_iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    it->second->DrawHistogram();
  } 
  return true;
}       // -----  end of function DelProcess::DrawHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DelProcess::BookHistogram
//  Description:  
// ===========================================================================
bool DelProcess::BookHistogram()
{
  His->AddTH1("NEle", "Num. of Electrons", 10, 0, 10 );
  His->AddTH1("NMuon", "Num. of Muons", 10, 0, 10 );
  His->AddTH1("NPhoton", "Num. of Photons", 10, 0, 10 );
  return true;
}       // -----  end of function DelProcess::BookHistogram  -----

// ===  FUNCTION  ============================================================
//         Name:  DelProcess::FillHistogram
//  Description:  
// ===========================================================================
bool DelProcess::FillHistogram(const DelAna *ana)
{
  His->FillTH1("NEle", (int)Ana->vElectron->size());
  His->FillTH1("NMuon", (int)Ana->vMuon->size());
  His->FillTH1("NPhoton", (int)Ana->vPhoton->size());
  return true;
}       // -----  end of function DelProcess::FillHistogram  -----
