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
//        Company:  Baylor University, CDF@FNAL
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
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <string>

using namespace std;
typedef std::map<std::string, std::string> Tmaps;
typedef std::map<std::string, std::string>::iterator Tmapit;

// Structure for the sample information
struct MCsample
{
  std::string Fname;
  std::string Sname;
  bool IsSignal;
  double Xsec;
  double Color;
  double Nevents;
  double Scale;
  double NSel;
  TFile* File;
  MCsample(std::string fname="Test", std::string sname="TestSample", 
      bool issignal=false, double xsec = 0.0, double color=0, double nevents = 0, 
        double scale = 0.0, double NSel = 0.0, TFile* file=NULL):
      Fname(fname), Sname(sname), IsSignal(issignal),  Xsec(xsec), Color(color), 
      Nevents(nevents), Scale(scale), NSel(NSel), File(file){}
};

std::vector<string> GetAllHis (std::vector<MCsample> ProList);

int main ( int argc, char *argv[] )
{
  //TopStyle();
  StopStyle();
  TRint *theApp = new TRint("plot interactively", &argc, argv);

  const double InitLumifb = 500000; // in the unit of pb

 //const std::string Label = "14TEV_50PileUp_AllCut";
 ////Signal
 //MCsample Wino200("HH/Wino200PU0_AllCut.root", "Wino200", true,  0.02, 100); //14TeV XS 
 //// Background
 //MCsample ZJJ("HH/DYNN_AllCut.root", "DYNN", false,  12930, 38); //14TeV XS for Z/gamma
 //MCsample TT("HH/TAll_AllCut.root", "TTBAR",  false, 882.29, 93); //14TeV XS for ttbar
 //MCsample WJJ("HH/WJETS_AllCut.root", "WJJ",  false, 63066, 8); //14TeV XS for W

 const std::string Label = "14TEV_50PileUp_CTMjj";
 //Signal
 MCsample Wino200("HH/Wino200PU0_CTMjj.root", "Wino200", true,  0.02, 100); //14TeV XS 
 // Background
 MCsample ZJJ("HH/DYNN_CTMjj.root", "DYNN", false,  12930, 38); //14TeV XS for Z/gamma
 MCsample TT("HH/TAll_CTMjj.root", "TTBAR",  false, 882.29, 93); //14TeV XS for ttbar
 MCsample WJJ("HH/WJETS_CTMjj.root", "WJJ",  false, 63066, 8); //14TeV XS for W

  std::vector<MCsample> ProList; //List of process to be stacked
  ProList.push_back(WJJ);
  ProList.push_back(TT);
  ProList.push_back(ZJJ);
  ProList.push_back(Wino200);

  std::vector<double> NSignal; //Number of signal
  std::vector<double> NBackg; //Number of signal
  // First load all the needed information into the structure
  for(std::vector<MCsample>::iterator it=ProList.begin();
      it!=ProList.end(); it++)
  {
    //----------------------------------------------------------------------------
    //  Calculate the area to be normalized
    //  Nevent/ Xsec = sample_lumi. 
    //----------------------------------------------------------------------------
    if (it->File == NULL)
    {
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~ First, getting all the information needed ~~~~~
      it->File = new TFile(it->Fname.c_str(), "R");
      if (!it->File->IsOpen())
      {
        cout<<"Unable to locate first Input M2Info File: "<<it->Fname
          <<", so I cannot proceed."<<endl;
        exit(-1);

      }

      if (it->Nevents == 0) //Getting the sample events from the root file 
      {
        TH1F *Evt = (TH1F*)it->File->Get("NEVT");
        double bvale = Evt->GetBinContent(Evt->FindBin(1)); 
        if (bvale != 0)
        {
          it->Nevents = bvale;
          delete Evt;
        }
        else
          std::cout << "Some thing is wrong! " << std::endl;
      }

      if (it->NSel == 0)
      {
        TH1F *SEvt = (TH1F*)it->File->Get("NEVTS");
        double bvale = SEvt->GetBinContent(SEvt->FindBin(1)); 
        if (bvale != 0 && it->Nevents != 0)
        {
          it->NSel = bvale;
          delete SEvt;
        }
        else
          std::cout << "Some thing is wrong! " << std::endl;
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Now let do the calculation! ~~~~~
      double SampleLumi = it->Nevents / it->Xsec;
      double SampleScale = InitLumifb / SampleLumi;
      it->Scale = SampleScale;
      if (it->IsSignal)
        NSignal.push_back(it->Scale * it->NSel);
      else
        NBackg.push_back(it->Scale * it->NSel);

      std::string cat =  it->IsSignal ? " Signal " : " Background ";
      std::cout << cat << " Sample  : \033[0;31m" << it->Sname 
        <<  "\033[0m  Cross Section : \033[1;36m" << it->Xsec 
        <<  "\033[0m  Inital Nevent : \033[0;34m" << it->Nevents
        << "\033[0m Selected : \033[0;34m" << it->NSel << "\033[0m" << std::endl;
    }
  }

//----------------------------------------------------------------------------
//  Now let us calculate the S/sqrt(S+B)
//----------------------------------------------------------------------------
  double sgl=0.0;
  double bkg=0.0;
  for(std::vector<double>::iterator it=NSignal.begin();
    it!=NSignal.end(); it++)
  {
    sgl += *it;
  }
  for(std::vector<double>::iterator it=NBackg.begin();
    it!=NBackg.end(); it++)
  {
    bkg += *it;
  }
  double sb = sgl / sqrt(sgl+bkg);
  std::cout<<" S/sqrt(S+B) = \033[0;31m"<<sb<<"\033[0m"<< std::endl; 

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
    //Preparing for making this plot
    std::cout << "Ploting " << *hit << std::endl;
    c1->cd();
    c1->Clear();
    c1->SetLogy();
    std::vector<TH1F*> Stackh; //Keep all the stack histograms in this loop
    THStack *Hstack = new THStack(hit->c_str(), hit->c_str());

    double maxy = 0;
    double miny = 0;
    TAxis* yaix = 0;
    // Add Legend?? Need a better handling of this
    TLegend *f = new TLegend(0.6862416,0.5995763,0.9228188,0.8940678,NULL,"brNDC");
    f->SetBorderSize(0);
    f->SetFillStyle(0); //transparent hollow?
    f->SetTextFont(62); 
    f->SetTextSize(0.045);

    // Add to the plot the S/B ratio
    TPaveText *pt = new TPaveText(0.7080537,0.9194915,0.9077181,0.9745763,"brNDC");
    pt->SetFillColor(0);
    pt->SetBorderSize(0);
    pt->SetTextSize(0.05);
    TString Tsb =  Form( "S/#sqrt{S+B} = %.1f", sb);
    pt->AddText(Tsb);

    std::string Xlabel;
    for(std::vector<MCsample>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {
      TH1F* h = (TH1F*)it->File->Get(hit->c_str());
      std::cout << "NA : " << h->GetTitle() << " from : " << it->Fname<< std::endl;
      f->AddEntry(h, it->Sname.c_str(), "fl");
      Xlabel = h->GetTitle();
      h->SetTitle(Label.c_str());
      h->SetFillColor(it->Color);
      h->SetLineColor(1);
      //h->SetLineColor(it->Color);
      // Now let us rescale it 
      if (h->Integral() <= 0)
        continue;
      h->Scale(it->Scale/ h->Integral());

      Stackh.push_back(h);
      Hstack->Add(h);
      std::cout << "Title : " << h->GetTitle() << std::endl;
      Hstack->SetTitle(h->GetTitle());
    }

    if (Stackh.size() == 0)
    {
      delete Hstack;
      continue;
    }

    Hstack->Draw();
    Hstack->GetXaxis()->SetTitle(Xlabel.c_str());
    pt->Draw();
    f->Draw();

    //Print out the plots
    TString outname = Label+"_"+*hit + ".png";
    c1->Print(outname);

    for(std::vector<TH1F*>::iterator it=Stackh.begin();
        it!=Stackh.end(); it++)
    {
      delete *it;
    }
    delete Hstack;
    delete pt;
    delete f;
  }
  //delete c1;
  theApp->Run();
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
    varVec.push_back(it->first);
  }
  return varVec;
}		// -----  end of method MethodIICalc::GetAllHisFromM2Info  -----

