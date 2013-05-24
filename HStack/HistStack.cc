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
#include <iostream>
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
  double Xsec;
  double Color;
  double Nevents;
  double Scale;
  double NEff;
  double NEff4j;
  TFile* File;
  MCsample(std::string fname="Test", std::string sname="TestSample", 
      double xsec = 0.0, double color=0, double nevents = 0, 
        double scale = 0.0, double neff = 0.0, TFile* file=NULL):
      Fname(fname), Sname(sname), Xsec(xsec), Color(color), Nevents(nevents), 
      Scale(scale), NEff(neff), File(file){}
};

std::vector<string> GetAllHis (std::vector<MCsample> ProList);

int main ( int argc, char *argv[] )
{
  //TopStyle();
  StopStyle();
  TRint *theApp = new TRint("plot interactively", &argc, argv);

  const double InitLumifb = 700; // in the unit of /fb

  MCsample Wino200("./Wino/Wino200_14TeV_140PileUp_NoCut.root", "Wino200_140PileUp_NoCut", 964.6, 1); //14TeV XS 
  MCsample Wino250("./Wino/Wino200_14TeV_140PileUp_CTMjj.root", "Wino200_140PileUp_CTMjj", 964.6, 3); //14TeV XS 
  MCsample Wino500("./Wino/Wino200_14TeV_NoPileUp_CTMet50.root", "Wino200_NoPileUp_CTMet50", 21022, 2); //14TeV XS 
  //MCsample TTbar("../HComp/Delphes/TTBAR_13TEV_140PileUp_AllCut.root", 964.6, 1); //14TeV XS 
  //MCsample WJJ("../HComp/Delphes/WJETS_13TEV_NoPileUp_NoCut.root", 21022, 2); //14TeV XS 

  Wino500.Fname="df";


  std::vector<MCsample> ProList; //List of process to be stacked
  const std::string Prefix = "Test";
  ProList.push_back(Wino200);
  ProList.push_back(Wino250);
  ProList.push_back(Wino500);

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

      if (it->NEff == 0)
      {
        TH1F *SEvt = (TH1F*)it->File->Get("NEVTS");
        double bvale = SEvt->GetBinContent(SEvt->FindBin(1)); 
        if (bvale != 0 && it->Nevents != 0)
        {
          it->NEff = bvale / it->Nevents;
          delete SEvt;
        }
        else
          std::cout << "Some thing is wrong! " << std::endl;
      }
      //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Now let do the calculation! ~~~~~
      double SampleLumi = it->Nevents / it->Xsec;
      double SampleScale = InitLumifb / SampleLumi;
      it->Scale = SampleScale * it->NEff;

      std::cout << "Inital Nevent : " << it->Nevents << " Eff : " << it->NEff << std::endl;
    }


  }


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
    std::vector<TH1F*> Stackh; //Keep all the stack histograms in this loop
  THStack *Hstack = new THStack(hit->c_str(), hit->c_str());

    double maxy = 0;
    double miny = 0;
    TAxis* yaix = 0;
    // Add Legend?? Need a better handling of this
    TLegend f(0.6862416,0.5995763,0.9228188,0.8940678,NULL,"brNDC");
    f.SetBorderSize(0);
    f.SetFillStyle(0); //transparent hollow?
    f.SetTextFont(62); 
    f.SetTextSize(0.045);
    //

    for(std::vector<MCsample>::iterator it=ProList.begin();
        it!=ProList.end(); it++)
    {
      TH1F* h = (TH1F*)it->File->Get(hit->c_str());
      std::cout << "NA : " << h->GetTitle() << " from : " << it->Fname<< std::endl;
      h->SetFillColor(it->Color);
      h->SetLineColor(it->Color);
      // Now let us rescale it 
      if (h->Integral() <= 0)
      {
        std::cout << "Shit!! what should I do? Let us go party..." << std::endl;
        break;
      }
      h->Scale(it->Scale/ h->Integral());

      Stackh.push_back(h);
      Hstack->Add(h);
      f.AddEntry(h, it->Sname.c_str(), "f");
      std::cout << "Title : " << h->GetTitle() << std::endl;
      Hstack->SetTitle(h->GetTitle());
    }

    Hstack->Draw();
    f.Draw();

    //Print out the plots
    TString outname = Prefix+"_"+*hit + ".png";
    c1->Print(outname);
    //outname = cit->first+ *hit + ".root";
    //c1->Print(outname);

    for(std::vector<TH1F*>::iterator it=Stackh.begin();
      it!=Stackh.end(); it++)
    {
      delete *it;
    }
    //break;
    delete Hstack;
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

