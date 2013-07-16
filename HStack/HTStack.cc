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
std::vector<double> CalSB(THStack *SGstack, THStack *BGstack);
double SBWithSys(double sig, double background);


//----------------------------------------------------------------------------
//  Global Variables 
//----------------------------------------------------------------------------
const int InitLumifb = 500; // in the unit of pb
const std::string Dir="./Jul163M_5_30_temp/";
const std::string TitleLable="5_30";


int main ( int argc, char *argv[] )
{

//----------------------------------------------------------------------------
//  PU Vector
//----------------------------------------------------------------------------
  std::vector<std::string> VPU;
  VPU.push_back("NoPileUp");
  //VPU.push_back("50PileUp");
  //VPU.push_back("140PileUp");

//----------------------------------------------------------------------------
//  Cut to check
//----------------------------------------------------------------------------
  std::vector<std::string> VCut;
  VCut.push_back("NoCut");
  VCut.push_back("CTVBF");
  VCut.push_back("CTJ1");
  VCut.push_back("CTJ2");
  VCut.push_back("CTMjj");
  VCut.push_back("CTJ3");
  VCut.push_back("CTBJ");
  VCut.push_back("CTLep");
  VCut.push_back("CTMet200");
  VCut.push_back("AllCut");

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
  //TopStyle();
  StopStyle();

  std::cout <<  "  ======================  PileUp : " << PU << std::endl;

  std::vector<std::string> order; // Order of the cut  
  //// The Cut flow
  order.push_back("NoCut");
  order.push_back("CTVBF");
  order.push_back("CTJ1");
  order.push_back("CTJ2");
  order.push_back("CTMjj");
  order.push_back("CTJ3");
  order.push_back("CTBJ");
  order.push_back("CTLep");
  order.push_back("CTMet200");
  order.push_back("AllCut");
  int hisIdx = -1;

  std::vector<std::string>::iterator cit =std::find(order.begin(), order.end(), Cut.c_str());
  if (cit == order.end())
  {
    std::cout << " Can't find this Cut !! Exiting... " << std::endl;
    return 0;
  } else hisIdx = static_cast<int>(std::distance(order.begin(), cit));

  if (hisIdx == -1) return 0;

  TString Label = "14TEV_" + PU + "_" + TitleLable;

  std::map<std::string, HTSample*> MCSample;
  // Initial the process
  MCSample["Wino200"] = new HTSample(Dir, "Wino200_14TeV", PU);
  MCSample["Wino500"] = new HTSample(Dir, "Wino500_14TeV", PU);
  MCSample["TTbar"] = new HTSample(Dir, "TT_14TEV_HT", PU);
  MCSample["B"] = new HTSample(Dir, "B_14TEV_HT", PU);
  MCSample["BJ"] = new HTSample(Dir, "BJ_14TEV_HT", PU);
  MCSample["BJJ"] = new HTSample(Dir, "BJJ_14TEV_HT", PU);


  // Set the option for each process
  MCSample["Wino200"]->SetOptions(true, 7);
  MCSample["Wino500"]->SetOptions(true, 3);
  MCSample["TTbar"]->SetOptions(false, 93);
  MCSample["B"]->SetOptions(false, 2);
  MCSample["BJ"]->SetOptions(false, 43);
  MCSample["BJJ"]->SetOptions(false, 73);

  for(std::map<std::string, HTSample*>::iterator it=MCSample.begin();
    it!=MCSample.end(); it++)
  {
    it->second->InitSample(InitLumifb*1000);
  }


  //----------------------------------------------------------------------------
  // Setting the drawing order by a vector 
  //----------------------------------------------------------------------------
  std::vector<std::string> ProList; 
  ProList.push_back("Wino200");
  ProList.push_back("Wino500");
  ProList.push_back("TTbar");
  ProList.push_back("B");
  ProList.push_back("BJ");
  ProList.push_back("BJJ");

  //----------------------------------------------------------------------------
  //  Get the list of containing Hist
  //----------------------------------------------------------------------------
  std::vector<std::string> passive_var;
  passive_var.push_back("CMet");
  passive_var.push_back("MJJ");

  //The canvas to draw plots
  TCanvas *c1 = new TCanvas("cn", "Canvas", 600, 500);

  //----------------------------------------------------------------------------
  //  Looping over each hist
  //----------------------------------------------------------------------------
  for (std::vector<std::string>::iterator hit = passive_var.begin();
      hit != passive_var.end(); hit++ )
  {
    std::cout << " Drawing His : " << *hit << std::endl;
    //if (*hit != "CMet") continue;
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

    //// Add to the plot the S/B ratio
    //TPaveText *pt = 0;
    //if (NSignal.size() == 1)
    //{
    //pt = new TPaveText(0.7080537,0.9194915,0.9077181,0.9745763,"brNDC");
    //pt->SetFillColor(0);
    //pt->SetBorderSize(0);
    //pt->SetTextSize(0.05);
    //TString Tsb =  Form( "S/#sqrt{S+B} = %.1f", sb);
    //pt->AddText(Tsb);
    //}

    std::string Xlabel;
    for(std::vector<std::string>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {

      TH1F* h = MCSample[*it]->GetTH1(*hit, hisIdx);
      if (*hit == "MJJ") h->Rebin(10);

      std::cout << "NA : " << h->GetTitle() << " from : " << *it<< std::endl;
      f->AddEntry(h, it->c_str(), "fl");
      Xlabel = h->GetTitle();
      h->SetTitle(Label.Data());
      h->SetLineWidth(3);
      if (MCSample[*it]->isSignal)
      {
        h->SetLineWidth(3);
        h->SetLineStyle(MCSample[*it]->Color);
      }
      else
        h->SetLineColor(MCSample[*it]->Color);
        //h->SetFillColor(MCSample[*it]->Color);

      //h->SetLineColor(1);
      //h->SetLineColor(it->Color);
      // Now let us rescale it 
      //if (*hit == "CMet") h->Rebin(5);

      //if (h->Integral() <= 0)
      //continue;

      std::cout << " INtegral " << h->Integral() << std::endl;
      Stackh.push_back(h);
      if (MCSample[*it]->isSignal) SNstack->Add(h);
      else BGstack->Add(h);
      //std::cout << "Title : " << h->GetTitle() << std::endl;
      BGstack->SetTitle(h->GetTitle());
    }

    BGstack->SetMinimum(0.01);
    BGstack->Draw("noStack");
    BGstack->GetXaxis()->SetTitle(Xlabel.c_str());
    if (*hit == "CMet") BGstack->GetXaxis()->SetLimits(0, 600);
    SNstack->Draw("noStacksame");
    //if (pt != 0) pt->Draw();
    f->Draw();

    //Print out the plots
    TString outname = Label+"_"+*hit +"_"+Cut+ ".png";
    c1->Print(outname);

//----------------------------------------------------------------------------
//  Calculate the current S/B
//----------------------------------------------------------------------------
    CalSB(SNstack, BGstack);

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
    delete f;
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



  //for (int i = 0; i < BGstack->GetStack()->GetEntries(); ++i)
  //{

    TH1F* temp = (TH1F*)BGstack->GetStack()->At(BGstack->GetStack()->GetEntries()-1);

    std::cout << " name " << temp->GetName() << " int "<< temp->Integral() << std::endl;
    bk += temp->Integral();
    
    std::cout << " bk " << bk << std::endl;
  //}
  


    temp = (TH1F*)SNstack->GetStack()->At(0);
    std::cout << " signal " << 0<< " int " << temp->Integral() << std::endl;
    std::cout << " SB "<< SBWithSys(temp->Integral(), bk) << std::endl;


  for (int i = 1; i < SNstack->GetStack()->GetEntries(); ++i)
  {

    TH1F* temp1 = (TH1F*)SNstack->GetStack()->At(i-1);
    TH1F* temp2 = (TH1F*)SNstack->GetStack()->At(i);


    std::cout << " signal " << i<< " int " << temp2->Integral() - temp1->Integral() << std::endl;
    std::cout << " SB "<< SBWithSys(temp2->Integral() - temp1->Integral(), bk) << std::endl;
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
double SBWithSys(double sig, double background)
{
  //double sys = 0; // 10% systematics
  //double sys = 0.01; // 10% systematics
  double sys = 0.1; // 10% systematics

  double domi = background + pow(sys*background, 2);
  return sig/sqrt(domi);
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

std::vector<string> GetAllHis (std::vector<MCsample> ProList)
{

  //Similar as LoadFile(), but just looking at the first file so far
  //So it is the user's job to make sure all the M2Info shared the same
  //histogram configuration. If the M2 couldn't find the corresponding
  //histogram, it would crash
  //For future improvement, would be nice to loop over the whole file list
  //to obtain the possible out put histograms
  //Loading of the root file list containig the Method2Info objects
  //Now loop over the samples in the list and read in the information

  TFile m2File(ProList.begin()->Fname.c_str(),"R");
  TIter next (m2File.GetListOfKeys());
  std::map<std::string, int> variable;
  std::vector<std::string> varVec;
  TKey *key;
  //    std::string name;
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
      //std::cout << " it->first" << it->first  << std::endl;
    }
  }
  varVec.push_back("CMet");
  return varVec;
}		// -----  end of method MethodIICalc::GetAllHisFromM2Info  -----
*/
