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
DPhes::DPhes (TChain* chain): 
  fChain(chain), treeReader(NULL), DEV(NULL), ANA(NULL), branchEvent(NULL), 
  branchParticle(NULL), branchGenJet(NULL), branchCAJet(NULL),
  branchJet(NULL), branchElectron(NULL), branchPhoton(NULL), branchMuon(NULL),
  branchMet(NULL), branchHt(NULL), PUCorJetEta(-999.), PUCorJetPt(-999.),
  CrossSection(-999.), CrossSectionError(-999.), Process(""), PU("")
{
}  /* -----  end of method DPhes::DPhes  (constructor)  ----- */

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DPhes
//      Method:  DPhes
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DPhes::DPhes ( const DPhes &other ):
  fChain(other.fChain), treeReader(other.treeReader), DEV(other.DEV), 
  ANA(other.ANA), branchEvent(other.branchEvent), branchParticle(other.branchParticle), 
  branchGenJet(other.branchGenJet), branchCAJet(other.branchCAJet), 
  branchJet(other.branchJet), branchElectron(other.branchElectron),
  branchPhoton(other.branchPhoton), branchMuon(other.branchMuon),
  branchMet(other.branchMet), branchHt(other.branchHt),
  PUCorJetEta(other.PUCorJetEta), PUCorJetPt(other.PUCorJetPt), 
  CrossSection(other.CrossSection), CrossSectionError(other.CrossSectionError),
  Process(other.Process), PU(other.PU)
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

  delete treeReader;

  // Delete all the local classes
  delete DEV;
  delete ANA;
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
    DEV = new DelEventZJJ(true, PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);

    MDelPro["Default"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, name.Data()));

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ For Met Resolution study ~~~~~
    //DEV = new DelEventZJJ(false, PUCorJetEta, PUCorJetPt);
    //ANA = new DelAna(DEV, pu, detector);
    //TString tempname = name;
    //tempname.ReplaceAll("ZJETS", "MetDiMuon");
    //MDelPro["MetDiMuon"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = name;
    //tempname.ReplaceAll("ZJETS", "MetDiEle");
    //MDelPro["MetDiEle"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    return 1;
  }

  //----------------------------------------------------------------------------
  //  For TTBar sample
  //----------------------------------------------------------------------------
  if (process.find("TT") != std::string::npos)
  {
    DEV = new DelEventTT(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    MDelPro["Default"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, name.Data()));

    TString tempname = name;
    tempname.ReplaceAll("TT", "TTFLep");
    MDelPro["TTFL"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = name;
    tempname.ReplaceAll("TT", "TTSLep");
    MDelPro["TTSL"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = name;
    tempname.ReplaceAll("TT", "TTHad");
    MDelPro["TTHD"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    return 1;
  }


  //----------------------------------------------------------------------------
  //  For Wino singal sample
  //----------------------------------------------------------------------------
  if (process.find("Wino") != std::string::npos)
  {
    DEV = new DelEventWino(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    MDelPro["Default"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, name.Data()));
    return 1;
  }


//----------------------------------------------------------------------------
//  For HTBin single boson sample
//----------------------------------------------------------------------------

  if (std::count(process.begin(), process.end(), 'B') == 1) //TODO: make sure only one B,  
  {
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Inclusive Bonson+Jets ~~~~~
    DEV = new DelEventHTB(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    TString tempname = name;
    //MDelPro["Default"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, name.Data()));

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Higgs+Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "H");
    //MDelPro["H"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ W + Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "W");
    //MDelPro["W"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "B", "Wev");
    MDelPro["Wev"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "B", "Wmv");
    MDelPro["Wmv"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Wtv");
    //MDelPro["Wtv"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Wlv");
    //MDelPro["Wlv"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Whad");
    //MDelPro["Whad"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "SysWev");
    //MDelPro["SysWev"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "SysWmv");
    //MDelPro["SysWmv"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Z + Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "Z");
    //MDelPro["Z"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Zee");
    //MDelPro["Zee"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Zmm");
    //MDelPro["Zmm"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Ztt");
    //MDelPro["Ztt"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Zll");
    //MDelPro["Zll"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Zvv");
    //MDelPro["Zvv"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "Zhad");
    //MDelPro["Zhad"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "SysZee");
    //MDelPro["SysZee"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "SysZmm");
    //MDelPro["SysZmm"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Photon + Jets ~~~~~
    //tempname = ModifiedPreName(name, "B", "Photon");
    //MDelPro["Photon"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Met Study ~~~~~
    //tempname = ModifiedPreName(name, "B", "MetDiMuon");
    //MDelPro["MetDiMuon"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    //tempname = ModifiedPreName(name, "B", "MetDiEle");
    //MDelPro["MetDiEle"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    return 1;
  }

//----------------------------------------------------------------------------
//  For HTBin LL sample
//  TODO: The process find LL is not optimized. There are LLB etc process,
//  thus this need to be improved
//----------------------------------------------------------------------------
  if (process.find("LL") != std::string::npos)
  {
    DEV = new DelEventLL(PUCorJetEta, PUCorJetPt);
    ANA = new DelAna(DEV, pu, detector);
    MDelPro["Default"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, name.Data()));

    TString tempname = name;
    tempname = ModifiedPreName(name, "LL", "EleEle");
    MDelPro["EleEle"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "LL", "MuMu");
    MDelPro["MuMu"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "LL", "NvNv");
    MDelPro["NvNv"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "LL", "TauTau");
    MDelPro["TauTau"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "LL", "Lep");
    MDelPro["Lep"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "LL", "SysEE");
    MDelPro["SysEE"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    tempname = ModifiedPreName(name, "LL", "SysMM");
    MDelPro["SysMM"] = std::unique_ptr<DelProcess>(new DelProcess(ANA, tempname.Data()));

    return 1;
  }

//----------------------------------------------------------------------------
//  Default
//----------------------------------------------------------------------------
  DEV = new DelEvent(PUCorJetEta, PUCorJetPt);
  ANA = new DelAna(DEV, pu, detector);
  MDelPro[process] = std::unique_ptr<DelProcess>(new DelProcess(ANA, name.Data()));
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
int DPhes::PreLooping(const std::vector<std::string>& VCuts)
{
  for(std::map<std::string, std::unique_ptr<DelProcess> >::iterator it=MDelPro.begin();
      it!=MDelPro.end(); it++)
  {
    for(std::vector<std::string>::const_iterator sit=VCuts.begin();
      sit!=VCuts.end(); sit++)
    {
      it->second->AddCutFlow(*sit);
    }

    it->second->SetSampleXS(CrossSection, CrossSectionError);
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

    if (entry % 500 == 0)
      std::cout << entry << " -------------------- " <<  GetMemoryValue() << "MB" << std::endl;

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
    for(std::map<std::string, std::unique_ptr<DelProcess> >::iterator it=MDelPro.begin();
        it!=MDelPro.end(); it++)
    {
      // For each DelCut, fill NEVT with weight. 07/18/2013 10:41:34 AM
      // This can include all the information with the TH1F
      // During the normalization, using TH1F->GetEntries() to get the 
      // number of event without weight, which is needed to include the
      // kfactor, which will normalized to NLO cross section
      it->second->FillNEVT(ANA->Weight);

      //Fill in global histograms
      it->second->FillHistogram(ANA); 

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
  for(std::map<std::string, std::unique_ptr<DelProcess> >::iterator it=MDelPro.begin();
      it!=MDelPro.end(); it++)
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
    unsigned int  idx = process.find_last_of('_');
    if (idx != std::string::npos && process.find("HT") < idx)
    {
      process.erase(process.find_first_of("_", process.find("HT")), process.length());
    }
  } else { //For splited inclusive sample 

    unsigned int idx = process.find_last_of('_');
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


int DPhes::ParseMemoryLine(char* line)
{
  int i = strlen(line);
  while (*line < '0' || *line > '9') line++;
  line[i-3] = '\0';
  i = atoi(line);
  return i;
}


double DPhes::GetMemoryValue()
{ 
  FILE* file = fopen("/proc/self/status", "r");
  int result = -1;
  char line[128];

  while (fgets(line, 128, file) != NULL){
    if (strncmp(line, "VmSize:", 7) == 0){
      result = ParseMemoryLine(line);
      break;
    }
  }
  fclose(file);

  //Note: this value is in MB!
  return result/1024;
}

