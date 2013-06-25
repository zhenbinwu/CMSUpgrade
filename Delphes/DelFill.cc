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

#include "DelFill.h"
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
int DPhes::InitDelPhes(std::string process, std::string pu, std::string outdir)
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
  branchEFlowTrack = 0;
  branchEFlowTower = 0;
  NEntries         = 0;

  // Double check the filename and get the cross section
  GetCrossSection(process);

  // Set the output name
  SetPreName(process, pu, outdir);

  return 1;
}       // -----  end of function DPhes::InitDelPhes  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::SetPreName
//  Description:  Get the process and sample name for output files
// ===========================================================================
int DPhes::SetPreName(std::string process, std::string pu, std::string outdir)
{

  TString name;
  if (outdir != "")
  {
    struct stat st;
    if (stat(outdir.c_str(), &st) == 0 && S_ISDIR(st.st_mode))
      ;
    else
      mkdir(outdir.c_str(), S_IRWXU);
    name = outdir + "/" +process+"_"+pu;
  }
  else
    name = process+"_"+pu;


//----------------------------------------------------------------------------
//  For Z+Jets sample
//----------------------------------------------------------------------------
  if (process.find("ZJETS") != std::string::npos)
  {
      
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Default for the S/B ~~~~~
    //DEV = new DelZJJ(true, PUCorJetEta, PUCorJetPt);
    //ANA = new DelAna(DEV);

    //MDelCut["Default"] = new DelCut(ANA, name.Data());

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ For Met Resolution study ~~~~~
    DEV = new DelZJJ(false, PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV);
    TString tempname = name;
    tempname.ReplaceAll(process.c_str(), "MetDiMuon");
    MDelCut["MetDiMuon"] = new DelCut(ANA, tempname.Data());

    tempname = name;
    tempname.ReplaceAll(process.c_str(), "MetDiEle");
    MDelCut["MetDiEle"] = new DelCut(ANA, tempname.Data());

    return 1;
  }

//----------------------------------------------------------------------------
//  For TTBar sample
//----------------------------------------------------------------------------
  if (process.find("TTBAR") != std::string::npos)
  {
    DEV = new DelTT(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV);
    //MDelCut["Default"] = new DelCut(ANA, name.Data());

    TString tempname = name;
    tempname.ReplaceAll(process.c_str(), process+"_FLep");
    MDelCut["TTFL"] = new DelCut(ANA, tempname.Data());

    tempname = name;
    tempname.ReplaceAll(process.c_str(), process+"_SLep");
    MDelCut["TTSL"] = new DelCut(ANA, tempname.Data());

    tempname = name;
    tempname.ReplaceAll(process.c_str(), process+"_Had");
    MDelCut["TTHD"] = new DelCut(ANA, tempname.Data());

    return 1;
  }


//----------------------------------------------------------------------------
//  For Wino singal sample
//----------------------------------------------------------------------------
  if (process.find("Wino") != std::string::npos)
  {
    DEV = new DelWino(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV);
    MDelCut["Default"] = new DelCut(ANA, name.Data());
    return 1;
  }

  // Default
  DEV = new DelEvent(PUCorJetEta, PUCorJetPt);
  ANA = new DelAna(DEV);
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

  NEntries = fChain->GetEntries();

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
  branchEFlowTrack = treeReader->UseBranch("EFlowTrack");
  branchEFlowTower = treeReader->UseBranch("EFlowTower");
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
      it->second->InitCutOrder();
      it->second->BookHistogram();
      it->second->FillSampleXS(CrossSection);
    } 
    return 1;

}       // -----  end of function DPhes::PreLooping  -----

// ===  FUNCTION  ============================================================
//         Name:  DPhes::Looping
//  Description:  Looping the chain
// ===========================================================================
int DPhes::Looping()
{

  //----------------------------------------------------------------------------
  //  Too many messy stuff dump in here for the Desy. Need to clean up afterward
  //----------------------------------------------------------------------------
  std::cout << "NEntriese" << NEntries << std::endl;
  // Loop over all events
  for(Int_t entry = 0; entry < NEntries ; ++entry)
  {
    //if (entry > 2000 ) break;
    if (entry % 2000 == 0)
      std::cout << "--------------------" << entry << std::endl;

    // Load selected branches with data from specified event
    treeReader->ReadEntry(entry);

//----------------------------------------------------------------------------
//  Loading the current event and perform the preselection
//----------------------------------------------------------------------------
    if (!DEV->LoadEvent(branchEvent, branchJet, branchGenJet, branchCAJet,
          branchElectron, branchMuon, branchPhoton, 
          branchMet, branchHt, branchParticle))
      continue;
     
    for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
        it!=MDelCut.end(); it++)
    {
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
bool DPhes::GetCrossSection(std::string process)
{
  std::fstream file;
  file.open("FileList/CrossSection.list", std::fstream::in);
  if (!file.is_open())
  {
    std::cout << " Error to open the Cross Section file!" << std::endl;
    return false;
  }
  
  std::string line;
  while (!file.eof())
  {
    getline(file, line);
    if (line.empty()) continue;
    if (line[0] == '#') continue;

    std::string pro;
    float xs;

    std::stringstream(line) >>  pro >> xs;
    if (pro == process)
    {
      CrossSection = xs;
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
