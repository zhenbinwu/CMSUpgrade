//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jun 17 18:58:14 2013 by ROOT version 5.32/00
// from TChain histAndTree_forVBFSusy/AUX/
//////////////////////////////////////////////////////////

#ifndef CMSAUX_h
#define CMSAUX_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <sys/stat.h>
#include "TLorentzVector.h"
// Classes from Ben
#include "DelEvent.h"
#include "DelAna.h"
#include "DelZJJ.h"
#include "classes/DelphesClasses.h"
#include "DelCut.h"

// Header file for the classes stored in the TTree if any.
#include <vector>
using namespace std;

// Fixed size dimensions of array or collections stored in the TTree if any.

class CMSAUX {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           event;
   Int_t           lumi;
   Int_t           npv;
   Double_t        avg_npv;
   Int_t           nm1;
   Int_t           n0;
   Int_t           np1;
   Double_t        tru_npv;
   Int_t           vtxSize;
   Int_t           nJets;
   Int_t           nMuons;
   Int_t           nMuons_CUT;
   Int_t           nElectrons;
   Int_t           nElectrons_CUT;
   Int_t           nTaus;
   Double_t        evtWeight;
   Double_t        mht;
   Double_t        mht_forSgnf;
   Double_t        ht;
   Double_t        met;
   Double_t        type1met;
   Double_t        mt;
   Double_t        mhtphi;
   Double_t        mhtphi_forSgnf;
   Double_t        metphi;
   Double_t        metSgnf;
   Double_t        metSgnfProb;
   Double_t        mhtSgnf;
   Double_t        mhtSgnfProb;
   vector<TLorentzVector> *jetsLVec;
   vector<TLorentzVector> *muonsLVec;
   vector<vector<double> > *muonsIso;
   vector<TLorentzVector> *elesLVec;
   vector<TLorentzVector> *tausLVec;
   Double_t        dPhi0_CUT;
   Double_t        dPhi1_CUT;
   Double_t        dPhi2_CUT;
   Int_t           nJets_CUT;
   Int_t           externalBit;
   vector<int>     *recoJetsFlavor;
   vector<string>  *bTagKeyString;
   vector<double>  *recoJetsBtag_0;
   vector<TLorentzVector> *hepTaggerJets;
   vector<vector<TLorentzVector> > *hepTaggerSubJets;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_event;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_npv;   //!
   TBranch        *b_avg_npv;   //!
   TBranch        *b_nm1;   //!
   TBranch        *b_n0;   //!
   TBranch        *b_np1;   //!
   TBranch        *b_tru_npv;   //!
   TBranch        *b_vtxSize;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_nMuons;   //!
   TBranch        *b_nMuonsForVeto;   //!
   TBranch        *b_nElectrons;   //!
   TBranch        *b_nElectronsForVeto;   //!
   TBranch        *b_nTaus;   //!
   TBranch        *b_evtWeight;   //!
   TBranch        *b_mht;   //!
   TBranch        *b_mht_forSgnf;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_met;   //!
   TBranch        *b_type1met;   //!
   TBranch        *b_mt;   //!
   TBranch        *b_mhtphi;   //!
   TBranch        *b_mhtphi_forSgnf;   //!
   TBranch        *b_metphi;   //!
   TBranch        *b_metSgnf;   //!
   TBranch        *b_metSgnfProb;   //!
   TBranch        *b_mhtSgnf;   //!
   TBranch        *b_mhtSgnfProb;   //!
   TBranch        *b_jetsLVec;   //!
   TBranch        *b_muonsLVec;   //!
   TBranch        *b_muonsIso;
   TBranch        *b_elesLVec;   //!
   TBranch        *b_tausLVec;   //!
   TBranch        *b_dPhi0_CUT;   //!
   TBranch        *b_dPhi1_CUT;   //!
   TBranch        *b_dPhi2_CUT;   //!
   TBranch        *b_nJets_CUT;   //!
   TBranch        *b_externalBitToTree_TR;   //!
   TBranch        *b_recoJetsFlavor;   //!
   TBranch        *b_bTagKeyString;   //!
   TBranch        *b_recoJetsBtag_0;   //!
   TBranch        *b_hepTaggerJets;   //!
   TBranch        *b_hepTaggerSubJets;   //!


/*---------------------------------------------------------------------------
 *  Local Variable
 *--------------------------------------------------------------------------*/
   DelEvent *DEV;
   DelAna *ANA;
   std::map<std::string, DelCut*> MDelCut; // For separated output

   // Define event
   double PUCorJetEta; // The Eta range of the jet into the Met correction
   double PUCorJetPt; // The Pt of the jet into the Met correction

/*---------------------------------------------------------------------------
 *  Ben's Function
 *--------------------------------------------------------------------------*/
   int SetPreName(std::string process, std::string pu, std::string outdir);
   bool SetPUCorrection(double JetPT, double JetEta);
   int PreLooping();
   int PostLooping();
   bool LoadDelEvent();
   bool LoadPhoton();
   bool LoadMet();
   bool LoadGen();
   bool LoadEvent();
   bool LoadMuon();
   bool LoadElectron();
   bool LoadJet();
   bool CalPUCorMet();

   bool FillCMSHist(DelCut* DCUT);
   bool BookCMSHist(DelCut* DCUT);

   CMSAUX(TTree *tree=0);
   virtual ~CMSAUX();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef CMSAUX_cxx
CMSAUX::CMSAUX(TTree *tree) : fChain(0) 
{
   Init(tree);
}

CMSAUX::~CMSAUX()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
   delete DEV;
   delete ANA;
   for(std::map<std::string, DelCut*>::iterator it=MDelCut.begin();
       it!=MDelCut.end(); it++)
   {
     delete it->second;
   }
}

Int_t CMSAUX::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t CMSAUX::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void CMSAUX::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jetsLVec = 0;
   muonsLVec = 0;
   muonsIso = 0;
   elesLVec = 0;
   tausLVec = 0;
   recoJetsFlavor = 0;
   bTagKeyString = 0;
   recoJetsBtag_0 = 0;
   hepTaggerJets = 0;
   hepTaggerSubJets = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("npv", &npv, &b_npv);
   fChain->SetBranchAddress("avg_npv", &avg_npv, &b_avg_npv);
   fChain->SetBranchAddress("nm1", &nm1, &b_nm1);
   fChain->SetBranchAddress("n0", &n0, &b_n0);
   fChain->SetBranchAddress("np1", &np1, &b_np1);
   fChain->SetBranchAddress("tru_npv", &tru_npv, &b_tru_npv);
   fChain->SetBranchAddress("vtxSize", &vtxSize, &b_vtxSize);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("nMuons", &nMuons, &b_nMuons);
   fChain->SetBranchAddress("nMuons_CUT", &nMuons_CUT, &b_nMuonsForVeto);
   fChain->SetBranchAddress("nElectrons", &nElectrons, &b_nElectrons);
   fChain->SetBranchAddress("nElectrons_CUT", &nElectrons_CUT, &b_nElectronsForVeto);
   fChain->SetBranchAddress("nTaus", &nTaus, &b_nTaus);
   fChain->SetBranchAddress("evtWeight", &evtWeight, &b_evtWeight);
   fChain->SetBranchAddress("mht", &mht, &b_mht);
   fChain->SetBranchAddress("mht_forSgnf", &mht_forSgnf, &b_mht_forSgnf);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("met", &met, &b_met);
   fChain->SetBranchAddress("type1met", &type1met, &b_type1met);
   fChain->SetBranchAddress("mt", &mt, &b_mt);
   fChain->SetBranchAddress("mhtphi", &mhtphi, &b_mhtphi);
   fChain->SetBranchAddress("mhtphi_forSgnf", &mhtphi_forSgnf, &b_mhtphi_forSgnf);
   fChain->SetBranchAddress("metphi", &metphi, &b_metphi);
   fChain->SetBranchAddress("metSgnf", &metSgnf, &b_metSgnf);
   fChain->SetBranchAddress("metSgnfProb", &metSgnfProb, &b_metSgnfProb);
   fChain->SetBranchAddress("mhtSgnf", &mhtSgnf, &b_mhtSgnf);
   fChain->SetBranchAddress("mhtSgnfProb", &mhtSgnfProb, &b_mhtSgnfProb);
   fChain->SetBranchAddress("jetsLVec", &jetsLVec, &b_jetsLVec);
   fChain->SetBranchAddress("muonsLVec", &muonsLVec, &b_muonsLVec);
   fChain->SetBranchAddress("muonsIso", &muonsIso, &b_muonsIso);
   fChain->SetBranchAddress("elesLVec", &elesLVec, &b_elesLVec);
   fChain->SetBranchAddress("tausLVec", &tausLVec, &b_tausLVec);
   fChain->SetBranchAddress("dPhi0_CUT", &dPhi0_CUT, &b_dPhi0_CUT);
   fChain->SetBranchAddress("dPhi1_CUT", &dPhi1_CUT, &b_dPhi1_CUT);
   fChain->SetBranchAddress("dPhi2_CUT", &dPhi2_CUT, &b_dPhi2_CUT);
   fChain->SetBranchAddress("nJets_CUT", &nJets_CUT, &b_nJets_CUT);
   fChain->SetBranchAddress("externalBit", &externalBit, &b_externalBitToTree_TR);
   fChain->SetBranchAddress("recoJetsFlavor", &recoJetsFlavor, &b_recoJetsFlavor);
   fChain->SetBranchAddress("bTagKeyString", &bTagKeyString, &b_bTagKeyString);
   fChain->SetBranchAddress("recoJetsBtag_0", &recoJetsBtag_0, &b_recoJetsBtag_0);
   fChain->SetBranchAddress("hepTaggerJets", &hepTaggerJets, &b_hepTaggerJets);
   fChain->SetBranchAddress("hepTaggerSubJets", &hepTaggerSubJets, &b_hepTaggerSubJets);
   Notify();


}

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::SetPUCorrection
//  Description:  
// ===========================================================================
bool CMSAUX::SetPUCorrection(double JetPT, double JetEta)
{
  PUCorJetPt = JetPT;
  PUCorJetEta = JetEta;
  return true;
}       // -----  end of function CMSAUX::SetPUCorrection  -----

// ===  FUNCTION  ============================================================
//         Name:  CMSAUX::SetPreName
//  Description:  Get the process and sample name for output files
// ===========================================================================
int CMSAUX::SetPreName(std::string process, std::string pu, std::string outdir)
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

  // Default
  DEV = new DelEvent(PUCorJetEta, PUCorJetPt);
  ANA = new DelAna(DEV);
  MDelCut[process] = new DelCut(ANA, name.Data());
  return 1;
}       // -----  end of function CMSAUX::SetPreName  -----

Bool_t CMSAUX::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void CMSAUX::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t CMSAUX::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef CMSAUX_cxx
