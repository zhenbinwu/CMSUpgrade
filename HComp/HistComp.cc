// ===========================================================================
// 
//       Filename:  HistComp.cc
// 
//    Description:  This is the code to make pretty comparison among files.
//           TODO:  1. Support HT bin comparison
//                  2. Comparison of HTbin combined samples
//                  3. Comparison among cuts
//                  4. Get the list of input histograms
// 
//        Version:  1.0
//        Created:  05/17/2013 10:07:32 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================
#include <cstdlib>
#include "StopStyle.hh"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TF1.h"
#include "TLegend.h"
#include "TKey.h"
#include "TRint.h"
#include "TLine.h"
#include "TStyle.h"
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <string>

#include <boost/foreach.hpp>

// Local class
#include "Compare.h"

//----------------------------------------------------------------------------
//  Global Types
//----------------------------------------------------------------------------
typedef std::pair<std::string, std::string> StrPair;
typedef std::vector<std::pair<std::string, std::string> > VecPair;
typedef std::vector<std::pair<std::string, std::string> >::iterator VecPairIt;
const std::string GobalTag = "TEST";

std::map<std::string, TFile*> GetFile( VecPair VDir, VecPair VPro, VecPair VPU);
int MakeComparison(std::map<std::string, TFile*> Files, VecPair Com, std::vector<std::string> HisName);
//int MakeCutFlow(std::map<std::string, Tmaps> Com, std::vector<std::string> order);

int main ( int argc, char *argv[] )
{
//----------------------------------------------------------------------------
//  Setup Style
//----------------------------------------------------------------------------
  StopStyle();
  //TRint *theApp = new TRint("plot interactively", &argc, argv);


//----------------------------------------------------------------------------
//  Flags for what to do?
//----------------------------------------------------------------------------
  //std::string study = "Dir"; // Compare among dirs
  //std::string study = "Pro"; // Compare among processes
  std::string study = "PU";  // Compare among PU
  //std::string study = "HT";  // Compare among HT      // todo
  //std::string study = "Cut"; // Compare among CutFlow // todo


//----------------------------------------------------------------------------
//  Input root files, separated as dir, process, pileup,
//----------------------------------------------------------------------------
  VecPair VDir;
  VDir.push_back(std::make_pair("HTBinJ", "../HStack/Jul20ALL_5_30/"));
  //VDir.push_back(std::make_pair("HTBinJ", "../HStack/Condor5M_5_30/"));
  //VDir.push_back(std::make_pair("NoQtCut", "MET_NoQTCut__4_30/"));
  //VDir.push_back(std::make_pair("Qt50", "MET_4_30"));
  //VDir.push_back(std::make_pair("Inclusive", "MET_20_0/"));

  // Eta cut 
  //VDir.push_back(std::make_pair("Jet_2_30", "MET_2_30/"));
  //VDir.push_back(std::make_pair("Jet_3_30", "MET_3_30/"));
  //VDir.push_back(std::make_pair("Jet_4_30", "MET_4_30/"));
  //VDir.push_back(std::make_pair("Jet_5_30", "MET_5_30/"));
  //VDir.push_back(std::make_pair("Jet_10_30", "MET_10_30/"));

  // Energy cut 
  //VDir.push_back(std::make_pair("Jet_5_30", "MET_5_30/"));
  //VDir.push_back(std::make_pair("Jet_5_40", "MET_5_40/"));
  //VDir.push_back(std::make_pair("Jet_5_50", "MET_5_50/"));
  //VDir.push_back(std::make_pair("Jet_5_70", "MET_5_70/"));
  //VDir.push_back(std::make_pair("Jet_5_100", "MET_5_100/"));

  VecPair VPro;
  //VPro.push_back(std::make_pair("ZJETS", "ZJETS"));
  //VPro.push_back(std::make_pair("DiElectron", "MetDiEle"));
  //VPro.push_back(std::make_pair("DiMuon", "MetDiMuon"));
  //VPro.push_back(std::make_pair("TTBAR_FLep", "TTBAR_13TEV_FLep"));
  //VPro.push_back(std::make_pair("TTBAR_SLep_Pt", "TTBAR_13TEV_SLep"));
  //VPro.push_back(std::make_pair("TT_Had", "TTBAR_13TEV_Had"));
  //VPro.push_back(std::make_pair("TT_HT1", "TT_14TEV_HT1"));
  //VPro.push_back(std::make_pair("TT_HT2", "TT_14TEV_HT2"));
  //VPro.push_back(std::make_pair("TT_HT3", "TT_14TEV_HT3"));
  //VPro.push_back(std::make_pair("TT_HT4", "TT_14TEV_HT4"));
  //VPro.push_back(std::make_pair("TT_HT5", "TT_14TEV_HT5"));
  //VPro.push_back(std::make_pair("Wino200", "Wino200_14TeV"));
  //VPro.push_back(std::make_pair("Wino500", "Wino500_14TeV"));
  VPro.push_back(std::make_pair("WlvJ_HT1", "WlvJ_14TEV_HT1"));
  VPro.push_back(std::make_pair("WlvJ_HT2", "WlvJ_14TEV_HT2"));
  VPro.push_back(std::make_pair("WlvJ_HT3", "WlvJ_14TEV_HT3"));
  VPro.push_back(std::make_pair("WlvJ_HT4", "WlvJ_14TEV_HT4"));
  VPro.push_back(std::make_pair("WlvJ_HT5", "WlvJ_14TEV_HT5"));
  VPro.push_back(std::make_pair("WlvJ_HT6", "WlvJ_14TEV_HT6"));
  VPro.push_back(std::make_pair("WlvJ_HT7", "WlvJ_14TEV_HT7"));
  VPro.push_back(std::make_pair("WlvJJ_HT1", "WlvJJ_14TEV_HT1"));
  VPro.push_back(std::make_pair("WlvJJ_HT2", "WlvJJ_14TEV_HT2"));
  VPro.push_back(std::make_pair("WlvJJ_HT3", "WlvJJ_14TEV_HT3"));
  VPro.push_back(std::make_pair("WlvJJ_HT4", "WlvJJ_14TEV_HT4"));
  VPro.push_back(std::make_pair("WlvJJ_HT5", "WlvJJ_14TEV_HT5"));
  
  //VPro.push_back(std::make_pair("DiMuonJJ_HT1", "MetDiMuonJJ_14TEV_HT1"));
  //VPro.push_back(std::make_pair("DiMuonJJ_HT2", "MetDiMuonJJ_14TEV_HT2"));
  //VPro.push_back(std::make_pair("DiMuonJJ_HT3", "MetDiMuonJJ_14TEV_HT3"));
  //VPro.push_back(std::make_pair("DiMuonJJ_HT4", "MetDiMuonJJ_14TEV_HT4"));
  //VPro.push_back(std::make_pair("DiMuonJJ_HT5", "MetDiMuonJJ_14TEV_HT5"));

  VecPair VPU;
  VPU.push_back(std::make_pair("0PU", "NoPileUp")); 
  VPU.push_back(std::make_pair("50PU", "50PileUp")); 
  VPU.push_back(std::make_pair("140PU", "140PileUp")); 
  //VPU.push_back(std::make_pair("50PU50ns", "50PileUp50ns"));
  //VPU.push_back(std::make_pair("50PU25ns", "50PileUp25ns"));
  //VPU.push_back(std::make_pair("140PU25ns", "140PileUp25ns"));

  std::vector<std::string> VCut;
  VCut.push_back("NoCut");
  VCut.push_back("CTJ1");
  VCut.push_back("CTJ2");
  VCut.push_back("CTMjj");
  VCut.push_back("CTJ3BL");
  VCut.push_back("CTMet200");
  VCut.push_back("AllCut");

  //----------------------------------------------------------------------------
  //  Creat mapping to the TFile pointer
  //----------------------------------------------------------------------------
  std::map<std::string, TFile*> FileList = GetFile(VDir, VPro, VPU);

//----------------------------------------------------------------------------
//  Get the list of hist for comparison
//----------------------------------------------------------------------------
  std::vector<std::string> Vvar;
  Vvar.push_back("RawMet");
  //Vvar.push_back("Met");
  //Vvar.push_back("MJJ_0");
  //Vvar.push_back("MLL");
  //Vvar.push_back("QT");
  //Vvar.push_back("UT");
  Vvar.push_back("JetEta");
  //Vvar.push_back("MJJ_0");
  //Vvar.push_back("J1Pt_0");
  //Vvar.push_back("J2Pt_0");
  //Vvar.push_back("J3Pt_0");
  //Vvar.push_back("NJets");
  //Vvar.push_back("NEle");
  //Vvar.push_back("NMuon");
  //Vvar.push_back("NPhoton");
  //Vvar.push_back("UtNPhotons");
  //Vvar.push_back("UTJetsPT");
  //Vvar.push_back("UTPhotonsPT");
  //Vvar.push_back("UPQT");
  //Vvar.push_back("UParal");
  //Vvar.push_back("UPerp");
  //Vvar.push_back("MetScale");
  //Vvar.push_back("MetResP");
  //Vvar.push_back("MetResT");
  //Vvar.push_back("MUPQT");
  //Vvar.push_back("MUParal");
  //Vvar.push_back("MUPerp");
  //Vvar.push_back("MMetScale");
  //Vvar.push_back("MMetResP");
  //Vvar.push_back("MMetResT");
  //Vvar.push_back("MetX");
  //Vvar.push_back("MetY");
  //Vvar.push_back("MetResX");
  //Vvar.push_back("MetResY");
  //Vvar.push_back("RMetX");
  //Vvar.push_back("RMetY");
  //Vvar.push_back("RMetResX");
  //Vvar.push_back("RMetResY");
  //Vvar.push_back("RMetXMean");
  //Vvar.push_back("RMetYMean");
  //Vvar.push_back("MetXMean");
  //Vvar.push_back("MetYMean");
  //Vvar.push_back("MuonIso1");
  //Vvar.push_back("MuonIso2");
  //Vvar.push_back("MuonIso3");

  //for (int i = 0; i < 11; ++i)
  //{
      //char temp[50];
      //sprintf(temp, "UPQT_%d", i);
      //Vvar.push_back(temp);
      //sprintf(temp, "UPerp_%d", i);
      //Vvar.push_back(temp);
      //sprintf(temp, "UParal_%d", i);
      //Vvar.push_back(temp);
  //}
//----------------------------------------------------------------------------
//  Making the comparison plots with different function
//----------------------------------------------------------------------------
  // Input list of files, which vector for comparison, what variables
  if (study == "Dir" ) MakeComparison(FileList, VDir, Vvar);
  if (study == "Pro" ) MakeComparison(FileList, VPro, Vvar);
  if (study == "PU"  ) MakeComparison(FileList, VPU,  Vvar);

  //theApp->Run();
  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

// ===  FUNCTION  ============================================================
//         Name:  GetFile
//  Description:  Get all the file open with tag
// ===========================================================================
std::map<std::string, TFile*> GetFile( VecPair VDir,  VecPair VPro, VecPair VPU)
{
    //----------------------------------------------------------------------------
    //  First open all the files for comparison
    //----------------------------------------------------------------------------
    std::map<std::string, TFile*> Fmap;
    BOOST_FOREACH(StrPair Dir, VDir)
    {
      BOOST_FOREACH(StrPair Pro, VPro)
      {
        BOOST_FOREACH(StrPair PU, VPU)
        {
          TString tag = Dir.first + "_" + Pro.first + "_" + PU.first;
          TString filename = "./"+Dir.second +"/"+Pro.second+"_"+PU.second+".root";
          Fmap[tag.Data()]=new TFile(filename, "R");
        }
      }
    }
    return Fmap;
}       // -----  end of function GetFile  -----


// ===  FUNCTION  ============================================================
//         Name:  MakeComparison
//  Description:  Currently we have Dir_Pro_PU. If we want to compare against
//  Dir, then for each Dir, we have combination of Pro_PU
// ===========================================================================
int MakeComparison(std::map<std::string, TFile*> Files, VecPair Com, std::vector<std::string> HisName)
{
  
  // Local Varaibles
  std::map<std::string, std::map<std::string, TFile*> > TagFile;
  std::vector<std::string> VTag;

  // Clear out the information
  BOOST_FOREACH(StrPair sp, Com)
  {
    for(std::map<std::string, TFile*>::iterator it=Files.begin();
      it!=Files.end(); it++)
    {
      std::string tagname = "";
      if (sp.first.find("0PU") != std::string::npos)
      {
        tagname += "_" + sp.first;
      }
      else tagname = sp.first +"_";

      size_t found = it->first.find(tagname);
      if (found != std::string::npos)
      {
        std::string temppre = it->first;
        temppre.replace(found, tagname.size(), "");
        TagFile[sp.first][temppre] = it->second;
      }
    }
    VTag.push_back(sp.first);
  }

  //Get the Scale of the tags
  //std::map<std::string, double> TagScale = ScalePUSample(TagFile, *VTag.begin(), "MLL");
  Compare CH(TagFile, VTag, "NEVT");

  // Get Vector of His to be compare for each his name
  BOOST_FOREACH(std::string name, HisName)
  {
    std::cout << "Drawing comparison for histogram: " <<name << std::endl;
    CH.CompHist(name);
  }

  return 1;
}       // -----  end of function MakeComparison  -----


/* 
// ===  FUNCTION  ============================================================
//         Name:  MakeCutFlow
//  Description:  The main functions that makes comparison
// ===========================================================================
int MakeCutFlow(std::map<std::string, Tmaps> Com, std::vector<std::string> order)
{
  //The canvas to draw plots
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);
  TFile out("CutFlow.root", "RECREATE");

  for(std::map<std::string, Tmaps>::iterator cit=Com.begin();
      cit!=Com.end(); cit++)
  {
    c1->cd();
    c1->Clear();

    //----------------------------------------------------------------------------
    //  First open all the files for comparison
    //----------------------------------------------------------------------------
    std::map<std::string, TFile*> Fmap;
    for(Tmapit it=cit->second.begin();
        it!=cit->second.end(); it++)
    {
      std::cout << "Getting file : " << it->second << std::endl;
      Fmap[it->first]=new TFile(it->second.c_str(), "R");
    }

    // Book the TH1 for the cut flow
    std::string Comname = cit->first;
    if (cit->first.find(" : ") != std::string::npos)
      Comname.replace(cit->first.find(" : "), 3, "");
    TH1F *CutFlow = new TH1F(Comname.c_str(), Comname.c_str(), 6, 0, 6);
    TAxis *xaxis = CutFlow->GetXaxis();
    int bin = 1;

      // Instead of looping the Tmap, I loop over a vector for the ordering
      for(std::vector<std::string>::iterator vit=order.begin();
          vit!=order.end(); vit++)
      {
        if (*vit == "AllCut") continue;
        Tmapit it=cit->second.find(*vit);
        TH1F* h1 = (TH1F*)Fmap[it->first]->Get("NEVT");
        double total = h1->GetBinContent(h1->FindBin(1));
        TH1F* h2 = (TH1F*)Fmap[it->first]->Get("NEVTS");
        double selected = h2->GetBinContent(h2->FindBin(1));
        std::cout << " Sample : " <<  cit->first << " Cut :  " << *vit <<" Event selected :"  
          << selected <<  " fraction :"   << selected/total << std::endl;
        CutFlow->SetBinContent(bin, selected/total);
        std::string tempname = *vit;
        if (vit->find("CT") != std::string::npos)
          tempname.replace(vit->find("CT"), 2, "");
        xaxis->SetBinLabel(bin, tempname.c_str());
        bin++;
      }
      out.cd();

      CutFlow->Draw();
      CutFlow->Write();
      TString plotname = Comname + ".png";
      c1->Print(plotname);
      delete CutFlow;
      xaxis = 0;

  }
  delete c1;

  return 1;
}

std::vector<std::string> GetAllHis (Tmaps Hmap )
{

  //Similar as LoadFile(), but just looking at the first file so far
  //So it is the user's job to make sure all the M2Info shared the same
  //histogram configuration. If the M2 couldn't find the corresponding
  //histogram, it would crash
  //For future improvement, would be nice to loop over the whole file list
  //to obtain the possible out put histograms
  //Loading of the root file list containig the Method2Info objects
  //Now loop over the samples in the list and read in the information
  for(Tmaps::iterator it=Hmap.begin();
      it!=Hmap.end(); it++)
  {

    TFile m2File(it->second.c_str(),"R");
    if(!m2File.IsOpen()){
      cout<<"Unable to locate first Input M2Info File: "<<it->second
        <<", so I cannot proceed."<<endl;
      exit(-1);
    }
  }
  TFile m2File(Hmap.begin()->second.c_str(),"R");
  TIter next (m2File.GetListOfKeys());
  std::map<std::string, int> variable;
  std::vector<std::string> varVec;
  TKey *key;
  //    std::string name;
  while ((key= (TKey*) next()))
  {
    key->IsA() == TH1F.Class();
    std::string stdname=key->GetName();
    variable.insert(std::pair<std::string, int>(stdname, 1));
  }
  for (std::map<std::string, int>::iterator it = variable.begin(); 
      it != variable.end(); ++it)
  {
    varVec.push_back(it->first);
  }
  return varVec;
}		// -----  end of method MethodIICalc::GetAllHisFromM2Info  -----
*/
