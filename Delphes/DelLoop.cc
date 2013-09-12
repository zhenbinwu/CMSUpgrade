/*
 * ===========================================================================
 *
 *       Filename:  DelFill.c
 *
 *    Description: :wqa 
 *
 *        Version:  1.0
 *        Created:  06/10/2013 11:41:35 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CMS@FNAL
 *
 * ===========================================================================
 */

#include "DelLoop.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DPhes
//      Method:  DPhes
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DPhes::DPhes (TChain* chain)
{
  fChain = chain;
  CrossSection = -999.;
}  /* -----  end of method DPhes::DPhes  (constructor)  ----- */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DPhes
//      Method:  DPhes
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DPhes::DPhes ( const DPhes &other )
{
}  // ~~~~~  end of method DPhes::DPhes  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DPhes
//      Method:  ~DPhes
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DPhes::~DPhes ()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();

  // Delete all the local classes
  delete DEV;
  delete ANA;
  for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    delete it->second;
  }
}  // ~~~~~  end of method DPhes::~DPhes  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DPhes
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DPhes&
DPhes::operator = ( const DPhes &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DPhes::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DPhes::SetPUCorMet
//  Description:  Interface for setting the PU corrected info
// ===========================================================================
bool DPhes::SetPUCorMet(double JetPT, double JetEta)
{
  PUCorJetPt = JetPT;
  PUCorJetEta = JetEta;
  return true;
}       // -----  end of function DPhes::SetPUCorMet  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::InitDelPhes
//  Description:  
// ===========================================================================
int DPhes::InitDelPhes(std::string process, std::string pu, std::string detector)
{
  // Get the intreface for Delphes tree
  treeReader       = 0;
  branchEvent      = 0;
  branchJet        = 0;
  branchElectron   = 0;
  branchMuon       = 0;
  branchPhoton     = 0;
  branchMet        = 0;
  branchHt         = 0;
  branchParticle   = 0;

  Process = process;
  PU = pu;

  // Double check the filename and get the cross section
  GetCrossSection(process);

  // Set the output name
  SetPreName(process, pu, detector);

  return 1;
}       // -----  end of function DPhes::InitDelPhes  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::SetPreName
//  Description:  Get the process and sample name for output files
// ===========================================================================
int DPhes::SetPreName(std::string process, std::string pu, std::string detector)
{

  TString name = process+"_"+pu+"_"+detector;

  //----------------------------------------------------------------------------
  //  For Z+Jets sample
  //----------------------------------------------------------------------------
  if (process.find("ZJETS") != std::string::npos)
  {

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Default for the S/B ~~~~~
    DEV = new DelZJJ(true, PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);

    MDelCut["Default"] = new DelCut(ANA, name.Data());

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ For Met Resolution study ~~~~~
    //DEV = new DelZJJ(false, PUCorJetEta, PUCorJetPt);
    //ANA = new DelAna(DEV, pu, detector);
    //TString tempname = name;
    //tempname.ReplaceAll("ZJETS", "MetDiMuon");
    //MDelCut["MetDiMuon"] = new DelCut(ANA, tempname.Data());

    //tempname = name;
    //tempname.ReplaceAll("ZJETS", "MetDiEle");
    //MDelCut["MetDiEle"] = new DelCut(ANA, tempname.Data());

    return 1;
  }

  //----------------------------------------------------------------------------
  //  For TTBar sample
  //----------------------------------------------------------------------------
  if (process.find("TT") != std::string::npos)
  {
    DEV = new DelTT(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    MDelCut["Default"] = new DelCut(ANA, name.Data());

    TString tempname = name;
    tempname.ReplaceAll("TT", "TTFLep");
    MDelCut["TTFL"] = new DelCut(ANA, tempname.Data());

    tempname = name;
    tempname.ReplaceAll("TT", "TTSLep");
    MDelCut["TTSL"] = new DelCut(ANA, tempname.Data());

    tempname = name;
    tempname.ReplaceAll("TT", "TTHad");
    MDelCut["TTHD"] = new DelCut(ANA, tempname.Data());

    return 1;
  }


  //----------------------------------------------------------------------------
  //  For Wino singal sample
  //----------------------------------------------------------------------------
  if (process.find("Wino") != std::string::npos)
  {
    DEV = new DelWino(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    MDelCut["Default"] = new DelCut(ANA, name.Data());
    return 1;
  }


//----------------------------------------------------------------------------
//  For HTBin single boson sample
//----------------------------------------------------------------------------

  if (std::count(process.begin(), process.end(), 'B') == 1) //TODO: make sure only one B,  
  {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Inclusive Bonson+Jets ~~~~~
    DEV = new DelHTB(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    //MDelCut["Default"] = new DelCut(ANA, name.Data());

    TString tempname = name;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Higgs+Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "H");
    //MDelCut["H"] = new DelCut(ANA, tempname.Data());

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ W + Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "W");
    //MDelCut["W"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "Wev");
    MDelCut["Wev"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "Wmv");
    MDelCut["Wmv"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "Wtv");
    MDelCut["Wtv"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "Wlv");
    MDelCut["Wlv"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "Whad");
    MDelCut["Whad"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "SysWev");
    MDelCut["SysWev"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "SysWmv");
    MDelCut["SysWmv"] = new DelCut(ANA, tempname.Data());
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Z + Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "Z");
    //MDelCut["Z"] = new DelCut(ANA, tempname.Data());

    //tempname = ModifiedPreName(name, "B", "Zee");
    //MDelCut["Zee"] = new DelCut(ANA, tempname.Data());

    //tempname = ModifiedPreName(name, "B", "Zmm");
    //MDelCut["Zmm"] = new DelCut(ANA, tempname.Data());

    //tempname = ModifiedPreName(name, "B", "Ztt");
    //MDelCut["Ztt"] = new DelCut(ANA, tempname.Data());

    //tempname = ModifiedPreName(name, "B", "Zll");
    //MDelCut["Zll"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "Zvv");
    MDelCut["Zvv"] = new DelCut(ANA, tempname.Data());

    //tempname = ModifiedPreName(name, "B", "Zhad");
    //MDelCut["Zhad"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "SysZee");
    MDelCut["SysZee"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "SysZmm");
    MDelCut["SysZmm"] = new DelCut(ANA, tempname.Data());
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Photon + Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "Photon");
    //MDelCut["Photon"] = new DelCut(ANA, tempname.Data());

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Met Study ~~~~~
    tempname = ModifiedPreName(name, "B", "MetDiMuon");
    MDelCut["MetDiMuon"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "B", "MetDiEle");
    MDelCut["MetDiEle"] = new DelCut(ANA, tempname.Data());

    return 1;
  }

//----------------------------------------------------------------------------
//  For HTBin LL sample
//  TODO: The process find LL is not optimized. There are LLB etc process,
//  thus this need to be improved
//----------------------------------------------------------------------------
  if (process.find("LL") != std::string::npos)
  {
    DEV = new DelLL(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    MDelCut["Default"] = new DelCut(ANA, name.Data());

    TString tempname = name;
    tempname = ModifiedPreName(name, "LL", "EleEle");
    MDelCut["EleEle"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "LL", "MuMu");
    MDelCut["MuMu"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "LL", "NvNv");
    MDelCut["NvNv"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "LL", "TauTau");
    MDelCut["TauTau"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "LL", "Lep");
    MDelCut["Lep"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "LL", "SysEE");
    MDelCut["SysEE"] = new DelCut(ANA, tempname.Data());

    tempname = ModifiedPreName(name, "LL", "SysMM");
    MDelCut["SysMM"] = new DelCut(ANA, tempname.Data());

    return 1;
  }

//----------------------------------------------------------------------------
//  Default
//----------------------------------------------------------------------------
  DEV = new DelEvent(PUCorJetEta, PUCorJetPt);
  ANA = new DelAna(DEV, pu, detector);
  MDelCut[process] = new DelCut(ANA, name.Data());
  return 1;
}       // -----  end of function DPhes::SetPreName  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::ReadDelPhes
//  Description:  Provide interface to read in the Delphes class
// ===========================================================================
int DPhes::ReadDelPhes()
{

  // Create object of class ExRootTreeReader
  treeReader = new ExRootTreeReader(fChain);

  // Get pointers to branches used in this analysis
  branchEvent      = treeReader->UseBranch("Event");
  branchJet        = treeReader->UseBranch("Jet");
  branchGenJet     = treeReader->UseBranch("GenJet");
  branchCAJet      = treeReader->UseBranch("CAJet");
  branchElectron   = treeReader->UseBranch("Electron");
  branchMuon       = treeReader->UseBranch("Muon");
  branchPhoton     = treeReader->UseBranch("Photon");
  branchMet        = treeReader->UseBranch("MissingET");
  branchHt         = treeReader->UseBranch("ScalarHT");
  branchParticle   = treeReader->UseBranch("Particle");
}       // -----  end of function DPhes::ReadDelPhes  -----


// ===  FUNCTION  ============================================================
//         Name:  DPhes::PreLooping
//  Description:  Prepared for the looping
// ===========================================================================
int DPhes::PreLooping()
{
  for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    it->second->InitCutOrder("DM");
    //it->second->InitCutOrder("Higgs");
    it->second->BookHistogram();
    it->second->FillSampleXS(CrossSection, CrossSectionError);
  } 
  
//----------------------------------------------------------------------------
//  Enable TTree Cache for speeding
//----------------------------------------------------------------------------
  fChain->SetCacheSize(20*1024*1024);
  TTreeCache::SetLearnEntries(1);

  return 1;

}       // -----  end of function DPhes::PreLooping  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::Looping
//  Description:  Looping the chain
// ===========================================================================
int DPhes::Looping()
{
  // Loop over all events
  int entry = 0;
  while (true) //Using the break from treeReader 
  {
    //if (entry > 5000 ) break;
    if (entry % 5000 == 0)
      std::cout << "--------------------" << entry << std::endl;

    // Load selected branches with data from specified event
    if (! treeReader->ReadEntry(entry)) break;
    entry++;

    //----------------------------------------------------------------------------
    //  Loading the current event and perform general calculations in DelAna.
    //----------------------------------------------------------------------------
    if (!DEV->LoadEvent(branchEvent, branchJet, branchGenJet, branchCAJet,
          branchElectron, branchMuon, branchPhoton, 
          branchMet, branchHt, branchParticle))
      continue;
    ANA->RunPerEvent();

    //----------------------------------------------------------------------------
    //  Filling the Hist for different DelCut according to the Flag
    //----------------------------------------------------------------------------
    for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
        it!=MDelCut.end(); it++)
    {
      // For each DelCut, fill NEVT with weight. 07/18/2013 10:41:34 AM
      // This can include all the information with the TH1F
      // During the normalization, using TH1F->GetEntries() to get the 
      // number of event without weight, which is needed to include the
      // kfactor, which will normalized to NLO cross section
      it->second->FillNEVT(ANA->Weight);

      if (ANA->CheckFlag(it->first))
      {
        it->second->FillCut();
      }
    } 
  }
  return 1;
}       // -----  end of function DPhes::Looping  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::PostLooping
//  Description:  After the looping
// ===========================================================================
int DPhes::PostLooping()
{  
  for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
      it!=MDelCut.end(); it++)
  {
    //it->second->DrawHistogram();
    it->second->WriteHistogram();
  } 
  return 1;
}       // -----  end of function DPhes::PostLooping  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::GetCrossSection
//  Description:  This function will get the cross section from
//  FileList/HTBin/CrossSection.list file. For the Delphes-3.0.7, use the
//  default value so far.
// ===========================================================================
bool DPhes::GetCrossSection(const std::string process_)
{
//----------------------------------------------------------------------------
//  Opening the CrossSection.list
//----------------------------------------------------------------------------
  std::fstream file;
  file.open("FileList/CrossSection.list", std::fstream::in);
  if (!file.is_open())
  {
    std::cout << " Error to open the Cross Section file!" << std::endl;
    return false;
  }

//----------------------------------------------------------------------------
//  Correct process name for splitted sample
//----------------------------------------------------------------------------
  std::string process = process_; //local process
  if (process.find("HT") != std::string::npos)
  {
    int  idx = process.find_last_of('_');
    if (idx != std::string::npos && process.find("HT") < idx)
    {
      process.erase(process.find_first_of("_", process.find("HT")), process.length());
    }
  } else { //For splited inclusive sample 

    int  idx = process.find_last_of('_');
    if (idx != std::string::npos && process.find("V") < idx)
    {
      process.erase(process.find_first_of("_", process.find("TeV")), process.length());
    }
  }

//----------------------------------------------------------------------------
//  Reading file to get cross section
//----------------------------------------------------------------------------
  std::string line;
  while (!file.eof())
  {
    getline(file, line);
    if (line.empty()) continue;
    if (line[0] == '#') continue;

    std::string pro;
    float xs;
    float xserr;

    std::stringstream(line) >>  pro >> xs >> xserr;
    if (pro == process)
    {
      CrossSection = xs;
      CrossSectionError = xserr;
      break;
    }
  }
  file.close();

  if (CrossSection == -999.)
  {
    std::cerr << "Unable to find the process and its cross section!" << std::endl;
    exit(1);
  }

  return EXIT_SUCCESS;
}       // -----  end of function DPhes::GetCrossSection  -----


// ===  FUNCTION  ============================================================
//         Name:  DPhes::ModifiedPreName
//  Description:  Modified the PreName (the output file name) according to
//  different splitting mode
// ===========================================================================
TString DPhes::ModifiedPreName(const TString output, std::string org, std::string rep) const
{
  
  TString tempPro = Process;
  TString tempOut = output;
  tempPro.ReplaceAll(org, rep);
  return tempOut.ReplaceAll(Process, tempPro);
}       // -----  end of function DPhes::ModifiedPreName  -----

