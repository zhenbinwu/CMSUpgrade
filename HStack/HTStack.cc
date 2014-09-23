// ===========================================================================
// 
//       Filename:  HistComp.cc
// 
//    Description:  This is the code to make the normalized background
//    stacking plots
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
#include "THStack.h"
#include "TPaveText.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <iterator>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <cstdio>

#include "HTSample.h"
#include <boost/foreach.hpp>

int Plot (std::string PU, std::string Cut);
double SBWithSys(double sig, double background, double sys);
std::vector<double> CalSB(THStack *SGstack, THStack *BGstack);
std::vector<std::string> GetAllHis(std::string fname, const std::string& ana="");
std::vector<std::string> GetCutOrder(const std::string& dir, const std::string& ana);

//---------------------------------------------------------------------------- 
// Global Variables 
//----------------------------------------------------------------------------
//const double InitLumifb = 19.6; // in the unit of pb
//const int    InitLumifb = 300;  // in the unit of pb
//const int    InitLumifb = 500;  // in the unit of pb
const   int    InitLumifb = 3000; // in the unit of pb

const std::string DET = "Snowmass";
const std::string Ana = "Snowmass";
const std::string Dir="./Snowmass_5_30/";


int main ( int argc, char *argv[] )
{

  TH1::AddDirectory(kFALSE);
//----------------------------------------------------------------------------
//  PU Vector
//----------------------------------------------------------------------------
  std::vector<std::string> VPU;
  VPU.push_back("NoPileUp");
  VPU.push_back("50PileUp");
  VPU.push_back("140PileUp");

//----------------------------------------------------------------------------
//  Cut to check
//----------------------------------------------------------------------------
  std::vector<std::string> VCut;
  //VCut.push_back("NoCut");
  //VCut.push_back("CTVBF");
  //VCut.push_back("CTJ1");
  //VCut.push_back("CTJ2");
  //VCut.push_back("CTMjj");
  //VCut.push_back("CTJ3");
  //VCut.push_back("CTBJ");
  //VCut.push_back("CTLep");
  VCut.push_back("CTMet200");
  //VCut.push_back("AllCut");

//----------------------------------------------------------------------------
//  Start to loop over the PU and Cut
//----------------------------------------------------------------------------
  for(std::vector<std::string>::iterator it=VPU.begin();
    it!=VPU.end(); it++)
  {
    for(std::vector<std::string>::iterator cit=VCut.begin();
        cit!=VCut.end(); cit++)
    {
      Plot(*it, *cit);
    }
  }

  return 1;
}

int Plot (std::string PU, std::string Cut)
{
  StopStyle();

  //const std::string nostk = "noStack";
  const std::string nostk = "";
  
  std::cout <<  "  ======================  PileUp : " << PU << std::endl;

  std::vector<std::string> order = GetCutOrder(Dir, Ana);
  int hisIdx = -1;

  std::vector<std::string>::iterator cit =std::find(order.begin(), order.end(), Cut.c_str());
  if (cit == order.end())
  {
    std::cout << " Can't find this Cut !! Exiting... " << std::endl;
    return 0;
  } else hisIdx = static_cast<int>(std::distance(order.begin(), cit));

  if (hisIdx == -1) return 0;

  TString Label = "14TEV_" + PU + "_" + DET;

  std::map<std::string, HTSample*> MCSample;
  // Initial the process
  MCSample["Sbottom15"] = new HTSample(Dir, "Sbottom15_14TeV",    PU, DET);
  MCSample["Wino100"] = new HTSample(Dir, "Wino100_14TeV",    PU, DET);
  MCSample["Wino200"] = new HTSample(Dir, "Wino200_14TeV",    PU, DET);
  MCSample["Wino500"] = new HTSample(Dir, "Wino500_14TeV",    PU, DET);
  //MCSample["TT"]      = new HTSample(Dir, "TT_8TEV",      PU);
  //MCSample["TT"]      = new HTSample(Dir, "TT_33TEV_HT",      PU);
  MCSample["TT"]      = new HTSample(Dir, "TT_14TEV_HT",      PU, DET);
  //MCSample["B"]       = new HTSample(Dir, "B_14TEV_HT",       PU);
  //MCSample["BJ"]      = new HTSample(Dir, "BJ_14TEV_HT",      PU);
  //MCSample["BJJ"]     = new HTSample(Dir, "BJJ_14TEV_HT",     PU);
  //MCSample["Wlv"]     = new HTSample(Dir, "W*Jet_8TEV_HT",    PU);
  //MCSample["Wlv"]     = new HTSample(Dir, "Wlv*_33TEV_HT",    PU);
  MCSample["Wlv"]     = new HTSample(Dir, "Wlv*_14TEV_HT",    PU, DET);
  //MCSample["Wev"]     = new HTSample(Dir, "Wev*_14TEV_HT",    PU, DET);
  //MCSample["Wmv"]     = new HTSample(Dir, "Wmv*_14TEV_HT",    PU, DET);
  //MCSample["Wtv"]     = new HTSample(Dir, "Wtv*_14TEV_HT",    PU, DET);
  //MCSample["W"]       = new HTSample(Dir, "WJ*_14TEV_HT",      PU);
  //MCSample["Z"]       = new HTSample(Dir, "Zvv*_14TEV_HT",      PU);
  //MCSample["H"]       = new HTSample(Dir, "H*_14TEV_HT",      PU);
  //MCSample["Photon"]  = new HTSample(Dir, "PhotonJ*_14TEV_HT", PU);
  //MCSample["Whad"]    = new HTSample(Dir, "Whad*_14TEV_HT",   PU);
  //MCSample["WJ"]       = new HTSample(Dir, "WJ_14TEV_HT",      PU);
  //MCSample["ZJ"]       = new HTSample(Dir, "ZJ_14TEV_HT",      PU);
  //MCSample["Zll"]       = new HTSample(Dir, "Zll*_14TEV_HT",      PU);
  //MCSample["Zvv"]       = new HTSample(Dir, "ZJETS_8TEV_HT",      PU);
  //MCSample["Zvv"]       = new HTSample(Dir, "Zvv*_33TEV_HT",      PU);
  MCSample["Zvv"]       = new HTSample(Dir, "Zvv*_14TEV_HT",      PU, DET);
  //MCSample["LL"]       = new HTSample(Dir, "LL*_14TEV_HT",      PU, DET);
  ////MCSample["HJ"]       = new HTSample(Dir, "HJ_14TEV_HT",      PU);
  //MCSample["PhotonJ"]  = new HTSample(Dir, "PhotonJ_14TEV_HT", PU);
  //MCSample["WJJ"]       = new HTSample(Dir, "WJJ_14TEV_HT",      PU);
  //MCSample["ZJJ"]       = new HTSample(Dir, "ZJJ_14TEV_HT",      PU);
  ////MCSample["HJJ"]       = new HTSample(Dir, "HJJ_14TEV_HT",      PU);
  //MCSample["PhotonJJ"]  = new HTSample(Dir, "PhotonJJ_14TEV_HT", PU);
  //MCSample["TTBAR"]   = new HTSample(Dir, "TTBAR_13TEV",      PU);
  //MCSample["WJETS"]   = new HTSample(Dir, "WJETS_13TEV",      PU);
  //MCSample["ZJETS"]   = new HTSample(Dir, "ZJETS_13TEV",      PU);

  // Set the option for each process
  MCSample["Sbottom15"] -> SetOptions(true,  2);
  MCSample["Wino100"] -> SetOptions(true,  1);
  MCSample["Wino200"] -> SetOptions(true,  8);
  MCSample["Wino500"] -> SetOptions(true,  3);
  MCSample["TT"]      -> SetOptions(false, 40);
  //MCSample["Wev"]     -> SetOptions(false, 2);
  //MCSample["Wmv"]     -> SetOptions(false, 2);
  //MCSample["Wtv"]     -> SetOptions(false, 2);
  MCSample["Wlv"]     -> SetOptions(false, 3);
  //MCSample["Whad"]    -> SetOptions(false, 1);
  //MCSample["WJ"]       -> SetOptions(false, 2);
  //MCSample["ZJ"]       -> SetOptions(false, 7);
  ////MCSample["HJ"]       -> SetOptions(false, 4);
  //MCSample["PhotonJ"]  -> SetOptions(false, 3);
  //MCSample["WJJ"]       -> SetOptions(false, 2);
  //MCSample["ZJJ"]       -> SetOptions(false, 7);
  //MCSample["HJJ"]       -> SetOptions(false, 4);
  //MCSample["Photon"]  -> SetOptions(false, 3);
  //MCSample["Wtv"]       -> SetOptions(false, 2);
  //MCSample["Whad"]       -> SetOptions(false, 3);
  //MCSample["Zll"]       -> SetOptions(false, 8);
  MCSample["Zvv"]       -> SetOptions(false, kOrange);
  //MCSample["LL"]       -> SetOptions(false, kRed);
  //MCSample["Zmm"]       -> SetOptions(false, kRed);
  //MCSample["W"]       -> SetOptions(false, 2);
  //MCSample["Z"]       -> SetOptions(false, 7);
  //MCSample["H"]       -> SetOptions(false, 4);
  //MCSample["Photon"]  -> SetOptions(false, 3);
  //MCSample["B"]       -> SetOptions(false, 2);
  //MCSample["BJ"]      -> SetOptions(false, 43);
  //MCSample["BJJ"]     -> SetOptions(false, 73);
  //MCSample["TTBAR"] -> SetOptions(false, 73);
  //MCSample["WJETS"]   -> SetOptions(false, 53);
  //MCSample["ZJETS"]   -> SetOptions(false, 33);

  for(std::map<std::string, HTSample*>::iterator it=MCSample.begin();
    it!=MCSample.end(); it++)
  {
    it->second->InitSample(InitLumifb*1000);
  }


  //----------------------------------------------------------------------------
  // Setting the drawing order by a vector 
  //----------------------------------------------------------------------------
  std::vector<std::string> ProList; 
  //ProList.push_back("TTBAR");
  //ProList.push_back("WJETS");
  //ProList.push_back("ZJETS");
  ProList.push_back("TT");
  //ProList.push_back("B");
  //ProList.push_back("BJ");
  //ProList.push_back("BJJ");
  //ProList.push_back("H");
  //ProList.push_back("Wtv");
  //ProList.push_back("Whad");
  //ProList.push_back("W");
  //ProList.push_back("Z");
  //ProList.push_back("Zll");
  //ProList.push_back("Photon");
  //ProList.push_back("WJ");
  //ProList.push_back("ZJ");
  //ProList.push_back("PhotonJ");
  //ProList.push_back("WJJ");
  //ProList.push_back("ZJJ");
  //ProList.push_back("PhotonJJ");
  ProList.push_back("Wlv");
  //ProList.push_back("LL");
  //ProList.push_back("Wev");
  //ProList.push_back("Wmv");
  //ProList.push_back("Wtv");
  ProList.push_back("Zvv");
  //ProList.push_back("Whad");
  ProList.push_back("Sbottom15");
  ProList.push_back("Wino100");
  ProList.push_back("Wino200");
  //ProList.push_back("Wino500");

  //return true;
  //----------------------------------------------------------------------------
  //  Get the list of containing Hist
  //----------------------------------------------------------------------------
  std::vector<std::string> passive_var;
  //passive_var.push_back("ABSdPhiJJ");
  //passive_var.push_back("J1Eta");
  //passive_var.push_back("J1Phi");
  //passive_var.push_back("JCPt");
  //passive_var.push_back("JFPt");
  //passive_var.push_back("J1Pt");
  //passive_var.push_back("J2Eta");
  //passive_var.push_back("J2Phi");
  //passive_var.push_back("J2Pt");
  //passive_var.push_back("J3Eta");
  //passive_var.push_back("J3Phi");
  //passive_var.push_back("J3Pt");
  //passive_var.push_back("dEtaJJ");
  //passive_var.push_back("dPhiJJ");
  //passive_var.push_back("dPtJJ");
  //passive_var.push_back("dRJJ");
  //passive_var.push_back("MHTMET");
  //passive_var.push_back("MetAsys");
  //passive_var.push_back("MET");
  //passive_var.push_back("MJJ");
  passive_var.push_back("GenEleEta");
  passive_var.push_back("GenElePt");
  passive_var.push_back("GenMuonPt");
  passive_var.push_back("GenMuonEta");
  passive_var.push_back("GenTauPt");
  passive_var.push_back("GenTauEta");
  passive_var.push_back("GenJetPt");
  passive_var.push_back("GenJetEta");
  passive_var.push_back("MHT");
  passive_var.push_back("MET");
  //passive_var.push_back("MetAsys");
  passive_var = GetAllHis("./Condor5M_5_30/Wino200_14TeV_NoPileUp.root");
  //passive_var.push_back("MJJ");

  //The canvas to draw plots
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);

  //----------------------------------------------------------------------------
  //  Looping over each hist
  //----------------------------------------------------------------------------
  for (std::vector<std::string>::iterator hit = passive_var.begin();
      hit != passive_var.end(); hit++ )
  {
    std::cout << " Drawing His : " << *hit <<" at " <<hisIdx << std::endl;
    //if (*hit != "CMet") continue;
    //Preparing for making this plot
    //std::cout << "Ploting " << *hit << std::endl;

    c1->cd();
    c1->Clear();
    c1->SetLogy();

    std::vector<TH1F*> Stackh; //Keep all the stack histograms in this loop
    //std::map<std::string, TH1F*>  MCHis;
    THStack *BGstack = new THStack(hit->c_str(), hit->c_str());
    //TH1F *SNstack = NULL;
    std::vector<TH1F*> SNstack; //Keep all the stack histograms in this loop
    //THStack *SNstack = new THStack(hit->c_str(), hit->c_str());
    //THStack *SNstack = new THStack(hit->c_str(), hit->c_str());
    

    //double maxy = 0;
    //double miny = 0;
    //TAxis* yaix = 0;
    //TLegend *f = new TLegend(0.6879195,0.6525424,0.9244966,0.9470339,NULL,"brNDC");
   TLegend *f = new TLegend(0.5889262,0.6483051,0.8255034,0.9427966,NULL,"brNDC");
    //TLegend *f = new TLegend(0.1996644,0.190678,0.4362416,0.4851695,NULL,"brNDC"); //ABS DPhi 
   //TLegend *f = new TLegend(0.6879195,0.6525424,0.9244966,0.9470339,NULL,"brNDC"); //Pt 
    f->SetBorderSize(0);
    f->SetFillStyle(0); //transparent hollow?
    f->SetTextFont(62); 
    f->SetTextSize(0.03);

    //// Add to the plot the S/B ratio
    TPaveText *pt = 0;
    pt = new TPaveText(0.2416107,0.845339,0.4412752,0.9237288,"brNDC");
    //pt = new TPaveText(0.5939597,0.2097458,0.7936242,0.2881356,"brNDC"); //ABS DPhi 
    //pt = new TPaveText(0.2315436,0.2097458,0.4312081,0.2881356,"brNDC"); //PT 
    pt->SetFillColor(0);
    pt->SetBorderSize(0);
    pt->SetTextSize(0.04);
    pt->SetTextColor(4);
    std::stringstream lumi;

    lumi << InitLumifb << " fb^{-1}, 14 TeV";
    std::cout << "======================================== " << lumi << std::endl;
    pt->AddText(lumi.str().c_str());
    TString name = DET + " detector";
    pt->AddText(name);

    std::string Xlabel;
    for(std::vector<std::string>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {

      std::cout << "=====" << *it << std::endl;
      //TH1F* h = MCSample[*it]->GetTH1("CMet_9");
      TH1F* h = MCSample[*it]->GetTH1(*hit, hisIdx);
      //h->Sumw2();
      //TH2D* h = MCSample[*it]->GetTH2D(*hit, hisIdx);
      if (*hit == "MJJ") h->Rebin(20);
      if (*hit == "MHT") h->Rebin(4);
      //h->Scale(1/h->Integral());
      
      std::cout << "NA : " << h->GetTitle() << " from : " << *it << "int " << h->Integral()<< std::endl;
      Xlabel = h->GetXaxis()->GetTitle();
      if (Xlabel == "")
      {
        Xlabel = h->GetTitle();
      }
      //h->SetTitle(Label.Data());
      h->SetTitle("");
      if (MCSample[*it]->isSignal)
      {
        //h->SetMarkerStyle(20);
        //h->SetMarkerColor(MCSample[*it]->Color);
        //f->AddEntry(h, it->c_str(), "lep");

        //SNstack = h;
        h->SetLineWidth(3);
        h->SetLineStyle(MCSample[*it]->Color);
        if (*it == "Wino100") h->SetLineColor(1);
        if (*it == "Wino200") h->SetLineColor(2);
        if (*it == "Wino500") h->SetLineColor(4);
        if (*it == "Sbottom15") h->SetLineColor(4);
      } else {
        if (nostk == "noStack")
        {
          h->SetLineWidth(3);
          h->SetLineColor(MCSample[*it]->Color);
        }
        else
        {
          h->SetFillColor(MCSample[*it]->Color);
          h->SetLineColor(1);

        }

      }

      if (*it == "Wino100")
      {
        f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 112 GeV", "fl");
      }

      if (*it == "Wino200")
      {
        f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 200 GeV", "fl");
      }
      if (*it == "Wino500")
      {
        f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 500 GeV", "fl");
      }
      if (*it == "Sbottom15")
      {
        f->AddEntry(h, "m(Sbottom) = 15 GeV", "fl");
        //f->AddEntry(h, "99% Wino, m(#tilde{#chi}^{0}_{1}) = 500 GeV", "fl");
      }
      if (*it == "TT")
      {
        f->AddEntry(h, "t#bar{t} + jets", "fl");
      }
      if (*it == "Wlv")
      {
        f->AddEntry(h, "W + jets", "fl");
      }
      if (*it == "Zvv")
      {
        f->AddEntry(h, "Z(#rightarrow#nu#bar{#nu}) + jets", "fl");
      }


      //h->SetLineColor(it->Color);
      // Now let us rescale it 
      //if (*hit == "CMet") h->Rebin(5);

      //if (h->Integral() <= 0)
      //continue;

      //std::cout << " INtegral " << h->Integral() << std::endl;
      Stackh.push_back(h);
      if (MCSample[*it]->isSignal) SNstack.push_back(h);
      else BGstack->Add(h);
      //BGstack->Add(h);
      //std::cout << "Title : " << h->GetTitle() << std::endl;
      BGstack->SetTitle(h->GetTitle());
    }

    BGstack->SetMaximum(10*BGstack->GetMaximum());
    BGstack->SetMinimum(1);
    //BGstack->Draw();
    BGstack->Draw(nostk.c_str());
    //BGstack->GetXaxis()->SetLimits(50, 600);
    //BGstack->GetYaxis()->SetTitle("Events / 20 GeV");
    BGstack->GetXaxis()->SetTitle(Xlabel.c_str());
    if (*hit == "CMet") BGstack->GetXaxis()->SetLimits(0, 600);
    //SNstack->SetMarkerStyle(10);
    //SNstack->SetMarkerColor(1);
    //SNstack->SetLineColor(1);
    BOOST_FOREACH(TH1F* h, SNstack)
    {
      h->Draw("Lsame");
      
    }
    //SNstack->Draw("noStacksame");
    //SNstack->Draw("noStacksame");
    if (pt != 0) pt->Draw();
    f->Draw();

    //Print out the plots
    TString outname = Label+"_"+*hit +"_"+Cut+ ".png";
    //TString outname = Label+"_"+*hit +"_"+Cut+"_"+DET+".png";
    c1->Print(outname);

//----------------------------------------------------------------------------
  //Calculate the current S/B
//----------------------------------------------------------------------------
 //if (SNstack->GetStack()->GetEntries() != 0)
 //{
    //CalSB(SNstack, BGstack);
 //}

    //----------------------------------------------------------------------------
      //Clean out the memory
    //----------------------------------------------------------------------------
    //for(std::vector<TH1F*>::iterator it=Stackh.begin();
        //it!=Stackh.end(); it++)
    //{
      //delete *it;
    //}
    //delete BGstack;
    //delete SNstack;
    //delete f;
  }
  delete c1;
  return 1;
}

// ===  FUNCTION  ============================================================
//         Name:  CalSB
//  Description:  
// ===========================================================================
std::vector<double> CalSB(THStack *SNstack, THStack *BGstack) 
{
  
  // The stack in THStack is accumulated!!
  std::vector<double> sb;
  double bk = 0;
  double sg = 0;
  const int metcut = 200;



  //for (int i = 0; i < BGstack->GetStack()->GetEntries(); ++i)
  //{

    TH1F* temp = (TH1F*)BGstack->GetStack()->At(BGstack->GetStack()->GetEntries()-1);

    double binl = temp->FindBin(metcut);
    double binh = temp->GetNbinsX();
    bk = temp->Integral(binl, binh);
    //std::cout << " name " << temp->GetName() << " int "<< temp->Integral() << std::endl;
    //bk += temp->Integral();
    

    std::cout << " bk " << bk << std::endl;
  //}
  


    temp = (TH1F*)SNstack->GetStack()->At(0);
    binl = temp->FindBin(metcut);
    binh = temp->GetNbinsX();
    sg = temp->Integral(binl, binh);
    std::cout << "Sg "<< sg << std::endl;
    //TH1F* temp = (TH1F*)SNstack->GetStack()->At(0);
    //std::cout << " signal " << 0<< " int " << temp->Integral() << std::endl;
    std::cout << " SB 0 "<< SBWithSys(sg, bk, 0.0) << std::endl;
    std::cout << " SB 1"<< SBWithSys(sg, bk, 0.01) << std::endl;
    std::cout << " SB 5"<< SBWithSys(sg, bk, 0.05) << std::endl;
    std::cout << " SB 10"<< SBWithSys(sg, bk, 0.10) << std::endl;


  for (int i = 1; i < SNstack->GetStack()->GetEntries(); ++i)
  {

    TH1F* temp1 = (TH1F*)SNstack->GetStack()->At(i-1);
    TH1F* temp2 = (TH1F*)SNstack->GetStack()->At(i);


    //std::cout << " signal " << i<< " int " << temp2->Integral() - temp1->Integral() << std::endl;
    std::cout << " SB "<< SBWithSys(temp2->Integral() - temp1->Integral(), bk, 0) << std::endl;
    //std::cout << " name " << temp->GetName() << " int "<< temp->Integral() << std::endl;
    //bk += temp->Integral();
    
    //std::cout << " bk " << bk << std::endl;
  }




  sb.push_back(bk);
  return sb;
  
}       // -----  end of function CalSB  -----

// ===  FUNCTION  ============================================================
//         Name:  SBWithSys
//  Description:  
// ===========================================================================
double SBWithSys(double sig, double background, double sys)
{
  //double sys = 0; // 10% systematics
  //double sys = 0.01; // 10% systematics
  //double sys = 0.1; // 10% systematics

  double domi = background + pow(sys*background, 2);
  std::cout << " sys " <<  pow(sys*background, 2) <<" back " <<background << std::endl;
  return sig/sqrt(sig+domi);
  //return sig/sqrt(sig+domi);
  
}       // -----  end of function SBWithSys  -----


  /*
  //=====================================  Signal
  TString samplename = Dir + "/Wino200_14TeV_"+PU+".root";
  MCsample Wino200(samplename.Data(), "Wino200", true,  0.01147, 7); //14TeV Preselected



  samplename = Dir + "/Wino500_14TeV_"+PU+".root";
  MCsample Wino500(samplename.Data(), "Wino500", true,  0.00045, 3); //14TeV Preselected

  // Background
  samplename = Dir + "/ZJETS_13TEV_"+PU+".root";
  MCsample ZJJ(samplename.Data(), "DYNN", false,  12930, 38); //14TeV XS for Z/gamma
  MCsample TT(samplename.Data(), "TTBAR",  false, 882.29, 93); //14TeV XS for ttbar
  samplename = Dir + "/WJETS_13TEV_"+PU+".root";
  //MCsample WJJ(samplename.Data(), "WJETS",  false, 63066, 8); //14TeV XS for W

  //========================== List of process to be stacked
  ProList.push_back(Wino200);
  ProList.push_back(Wino500);
  ProList.push_back(TTFL);
  ProList.push_back(TTSL);
  ProList.push_back(WJJ);
  ProList.push_back(ZJJ);

  //----------------------------------------------------------------------------
  //  Now get all the input variable to draw..
  //----------------------------------------------------------------------------
 
  //----------------------------------------------------------------------------
  //  Get the list of containing Hist
  //----------------------------------------------------------------------------
  std::vector<std::string> passive_var = GetAllHis(ProList);

  //The canvas to draw plots
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);

  //----------------------------------------------------------------------------
  //  Looping over each hist
  //----------------------------------------------------------------------------
  for (std::vector<std::string>::iterator hit = passive_var.begin();
      hit != passive_var.end(); hit++ )
  {
    std::cout << " Drawing His : " << *hit << std::endl;
    if (*hit != "CMet") continue;
    //Preparing for making this plot
    //std::cout << "Ploting " << *hit << std::endl;

    c1->cd();
    c1->Clear();
    c1->SetLogy();

    std::vector<TH1F*> Stackh; //Keep all the stack histograms in this loop
    THStack *BGstack = new THStack(hit->c_str(), hit->c_str());
    THStack *SNstack = new THStack(hit->c_str(), hit->c_str());

    //double maxy = 0;
    //double miny = 0;
    //TAxis* yaix = 0;
    // Add Legend?? Need a better handling of this
    TLegend *f = new TLegend(0.6862416,0.5995763,0.9228188,0.8940678,NULL,"brNDC");
    f->SetBorderSize(0);
    f->SetFillStyle(0); //transparent hollow?
    f->SetTextFont(62); 
    f->SetTextSize(0.045);

    // Add to the plot the S/B ratio
    TPaveText *pt = 0;
    if (NSignal.size() == 1)
    {
      pt = new TPaveText(0.7080537,0.9194915,0.9077181,0.9745763,"brNDC");
      pt->SetFillColor(0);
      pt->SetBorderSize(0);
      pt->SetTextSize(0.05);
      TString Tsb =  Form( "S/#sqrt{S+B} = %.1f", sb);
      pt->AddText(Tsb);
    }

    std::string Xlabel;
    for(std::vector<MCsample>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {
      char hisname[100];
      sprintf(hisname, "%s_%d", hit->c_str(), hisIdx);
      TH1F* h = (TH1F*)it->File->Get(hisname);

      //std::cout << "NA : " << h->GetTitle() << " from : " << it->Fname<< std::endl;
      f->AddEntry(h, it->Sname.c_str(), "fl");
      Xlabel = h->GetTitle();
      h->SetTitle(Label.Data());
      if (it->IsSignal)
      {
        h->SetLineWidth(3);
        h->SetLineStyle(it->Color);
      }
      else
        h->SetFillColor(it->Color);

      h->SetLineColor(1);
      //h->SetLineColor(it->Color);
      // Now let us rescale it 
      //if (*hit == "CMet") h->Rebin(5);

      //if (h->Integral() <= 0)
        //continue;

      h->Scale(it->Scale * it->NSel / h->Integral());

      Stackh.push_back(h);
      if (it->IsSignal) SNstack->Add(h);
      else BGstack->Add(h);
      //std::cout << "Title : " << h->GetTitle() << std::endl;
      BGstack->SetTitle(h->GetTitle());
    }

    BGstack->SetMinimum(1);
    BGstack->Draw();
    BGstack->GetXaxis()->SetTitle(Xlabel.c_str());
    if (*hit == "CMet") BGstack->GetXaxis()->SetLimits(0, 600);
    SNstack->Draw("noStacksame");
    if (pt != 0) pt->Draw();
    f->Draw();

    //Print out the plots
    TString outname = Label+"_"+*hit +"_"+Cut+ ".png";
    c1->Print(outname);
    outname = Label+"_"+*hit +"_"+Cut+ ".eps";
    c1->Print(outname);

//----------------------------------------------------------------------------
//  Clean out the memory
//----------------------------------------------------------------------------
    for(std::vector<TH1F*>::iterator it=Stackh.begin();
        it!=Stackh.end(); it++)
    {
      delete *it;
    }
    delete BGstack;
    delete SNstack;
    delete pt;
    delete f;
  }
  delete c1;
  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------
*/

std::vector<std::string> GetAllHis (std::string fname, const std::string& ana)
{

  //Similar as LoadFile(), but just looking at the first file so far
  //So it is the user's job to make sure all the M2Info shared the same
  //histogram configuration. If the M2 couldn't find the corresponding
  //histogram, it would crash
  //For future improvement, would be nice to loop over the whole file list
  //to obtain the possible out put histograms
  //Loading of the root file list containig the Method2Info objects
  //Now loop over the samples in the list and read in the information

  TFile m2File(fname.c_str(),"R");
  m2File.cd(ana.c_str());
  TIter next (gDirectory->GetListOfKeys());
  //TIter next (m2File.GetListOfKeys());
  std::map<std::string, int> variable;
  std::vector<std::string> varVec;
  TKey *key;
      std::string name;
  while ((key= (TKey*) next()))
  {
    std::string stdname=key->GetName();
    variable.insert(std::pair<std::string, int>(stdname, 1));
  }

  for (std::map<std::string, int>::iterator it = variable.begin(); 
      it != variable.end(); ++it)
  {
    if (it->first.find_last_of("_") == std::string::npos)
    {
      varVec.push_back(it->first);
      std::cout << " it->first" << it->first  << std::endl;
    }
  }
  varVec.push_back("CMet");
  return varVec;
}		 //-----  end of method MethodIICalc::GetAllHisFromM2Info  -----

// ===  FUNCTION  ============================================================
//         Name:  GetCutOrder
//  Description:  return the cut order
// ===========================================================================
std::vector<std::string> GetCutOrder(const std::string& dir, const std::string& ana)
{
  glob_t glob_result;
  char pat[100];
  std::vector<std::string> order;
  TFile *f = NULL;


  sprintf(pat, "%s/*.root", dir.c_str());
  glob(pat, GLOB_TILDE, NULL, &glob_result);
  if (glob_result.gl_pathc > 0)
    f = new TFile(glob_result.gl_pathv[0], "R");
  else 
    return order;

  globfree(&glob_result);

  assert(f != NULL);

  TString cutflow = ana + "/CutFlow";
  TH1F* cut = (TH1F*) f->Get(cutflow);

  for (int i = 0; i < cut->GetNbinsX(); ++i)
  {
    std::string d = cut->GetXaxis()->GetBinLabel(i+1);
    //std::cout <<i << " "<<  d << std::endl;
    order.push_back(d);
  }

  return order;

}       // -----  end of function GetCutOrder  -----
