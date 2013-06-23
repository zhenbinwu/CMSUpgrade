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

// Functions 
std::vector<string> GetAllHis (std::vector<MCsample> ProList);
int Plot (std::string PU, std::string Cut);


//----------------------------------------------------------------------------
//  Global Variables 
//----------------------------------------------------------------------------
double InitLumifb = 500; // in the unit of pb
//const double InitLumifb = 500000; // in the unit of pb
std::string Dir="./LoopCut_5_30/";
std::string TitleLable="5_30";


int main ( int argc, char *argv[] )
{

  //TRint *theApp = new TRint("plot interactively", &argc, argv);

  InitLumifb = InitLumifb * 1000;

  if (argc == 2)
  {
    TitleLable = argv[1];
    //std::stringstream sstream;
    //sstream <<" ./MJJ__";
    //sstream << TitleLable;
    //Dir = sstream.str();
  }

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
  //VCut.push_back("NoCut");
  //VCut.push_back("CTJ1");
  //VCut.push_back("CTJ2");
  //VCut.push_back("CTMjj");
  VCut.push_back("CTJ3BL");
  //VCut.push_back("CTMet200");
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
  //theApp->Run();
  return 1;
}

int Plot (std::string PU, std::string Cut)
{
  //TopStyle();
  StopStyle();

  std::cout <<  "  ======================  PileUp : " << PU << std::endl;
  std::vector<std::string> order; // Order of the cut  
  //// The Cut flow
  //order.push_back("NoCut");
  //order.push_back("CTVBF");
  //order.push_back("CTLepV");
  //order.push_back("CTMet50");
  //order.push_back("CTBJet");
  //order.push_back("CTJ3");
  //order.push_back("CTMjj");
  //order.push_back("CTMet200");
  //order.push_back("AllCut");
  // The index of the Cut on the order 
  // The Cut flow
  // The Cut flow
  order.push_back("NoCut");
  order.push_back("CTJ1");
  order.push_back("CTJ2");
  order.push_back("CTMjj");
  order.push_back("CTJ3BL");
  order.push_back("CTMet200");
  order.push_back("AllCut");
  int hisIdx = -1;
  
  std::vector<std::string>::iterator cit =std::find(order.begin(), order.end(), Cut.c_str());
  if (cit == order.end())
  {
    //std::cout << " Can't find this Cut !! Exiting... " << std::endl;
    return 0;
  }
  else
  hisIdx = static_cast<int>(std::distance(order.begin(), cit));

  if (hisIdx == -1) return 0;
  //std::cout << " hisIdx " << hisIdx << std::endl;
  //return 1;
 
  TString Label = "14TEV_" + PU + "_" + TitleLable;
  //TString Label = "14TEV_" + PU + "_" + Cut;

  //=====================================  Signal
  TString samplename = Dir + "/Wino200_14TeV_"+PU+".root";
  //MCsample Wino200(samplename.Data(), "Wino200", true,  0.02, 7); //14TeV XS 12fb for Wino200
  //MCsample Wino200(samplename.Data(), "Wino200", true,  0.012, 7); //14TeV XS 12fb for Wino200
  MCsample Wino200(samplename.Data(), "Wino200", true,  0.01147, 7); //14TeV Preselected
  samplename = Dir + "/Wino500_14TeV_"+PU+".root";
  //MCsample Wino500(samplename.Data(), "Wino500", true,  0.0005, 3); //14TeV XS 12fb for Wino200
  MCsample Wino500(samplename.Data(), "Wino500", true,  0.00045, 3); //14TeV Preselected
  // Background
  samplename = Dir + "/ZJETS_13TEV_"+PU+".root";
  //MCsample ZJJ(samplename.Data(), "DYNN", false,  12930, 38); //14TeV XS for Z/gamma
  MCsample ZJJ(samplename.Data(), "DYNN", false,  15.08, 38); //14TeV XS for Z/gamma
  samplename = Dir + "/TTBAR_13TEV_FLep_"+PU+".root";
  MCsample TTFL(samplename.Data(), "TTBAR_FL",  false, 0.0278, 93); //14TeV XS for ttbar
  samplename = Dir + "/TTBAR_13TEV_SLep_"+PU+".root";
  MCsample TTSL(samplename.Data(), "TTBAR_SL",  false, 0.0294, 93); //14TeV XS for ttbar
  //MCsample TT(samplename.Data(), "TTBAR",  false, 882.29, 93); //14TeV XS for ttbar
  samplename = Dir + "/WJETS_13TEV_"+PU+".root";
  MCsample WJJ(samplename.Data(), "WJETS",  false, 55.2, 8); //14TeV XS for W
  //MCsample WJJ(samplename.Data(), "WJETS",  false, 63066, 8); //14TeV XS for W

  //========================== List of process to be stacked
  std::vector<MCsample> ProList;
  ProList.push_back(Wino200);
  ProList.push_back(Wino500);
  ProList.push_back(TTFL);
  ProList.push_back(TTSL);
  ProList.push_back(WJJ);
  ProList.push_back(ZJJ);

//----------------------------------------------------------------------------
//  Getting the sample number of events
//----------------------------------------------------------------------------
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
        double bvale = -1;
        TH1F *SEvt = (TH1F*)it->File->Get("CutFlow");
        bvale = SEvt->GetBinContent(SEvt->FindBin(hisIdx)); 
        std::cout << " bvale " << bvale << std::endl;

        //Different way to do it, count up to 300 MEt
        //
        //std::stringstream sstream;
        //sstream <<"CMet_"<< hisIdx;
        //std::string plotname = sstream.str();
        //TH1F *SEvt = (TH1F*)it->File->Get(plotname.c_str());
        //double binl = SEvt->FindBin(300);
        //double binh = SEvt->GetNbinsX();
        //bvale = SEvt->Integral(binl, binh);

        if (bvale != -1 && it->Nevents != 0)
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
        << "\033[0m Selected : \033[0;34m" << it->NSel << "\033[0m" 
        <<  "\033[0m Sample Lumi : \033[0;34m" << SampleLumi << "\033[0m" 
        <<  "\033[0m Sample Scale : \033[0;34m" << SampleScale << "\033[0m" 
        << " scale to sample event : "  << it->Scale * it->NSel<< std::endl;
    }
  }

//----------------------------------------------------------------------------
//  Now let us calculate the S/sqrt(S+B) for each signal and bk
//----------------------------------------------------------------------------
  double sgl=0.0;
  double bkg=0.0;
  double sb = 0.0;
  for(std::vector<MCsample>::iterator it=ProList.begin();
      it!=ProList.end(); it++)
  {
    sgl = 0.0;
    sb = 0.0;
    if (!it->IsSignal) continue;
    //----------------------------------------------------------------------------
    sgl = it->Scale * it->NSel;

    for(std::vector<double>::iterator bit=NBackg.begin();
        bit!=NBackg.end(); bit++)
    {
      bkg += *bit;
    }

    sb = sgl / sqrt(sgl+bkg);
    std::cout<< " For signal : \033[0;36m" << it->Sname 
      << "\033[0m At Cut : \033[0;33m" << Cut 
      << "\033[0m S/sqrt(S+B) = \033[0;31m"<<sb<<"\033[0m"<< std::endl;
  }

  std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 

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
  std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
  for (std::vector<std::string>::iterator hit = passive_var.begin();
      hit != passive_var.end(); hit++ )
  {
    std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
    std::cout << " Drawing His : " << *hit << std::endl;
    if (*hit != "CMet") continue;
    std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
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

    //if (Stackh.size() == 0)
    //{
      //delete BGstack;
      //delete SNstack;
      //continue;
    //}

    BGstack->SetMinimum(1);
    BGstack->Draw();
    BGstack->GetXaxis()->SetTitle(Xlabel.c_str());
    if (*hit == "CMet") BGstack->GetXaxis()->SetLimits(0, 600);
    SNstack->Draw("noStacksame");
    if (pt != 0) pt->Draw();
    f->Draw();

    //Print out the plots
    std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
    TString outname = Label+"_"+*hit +"_"+Cut+ ".png";
    c1->Print(outname);
    outname = Label+"_"+*hit +"_"+Cut+ ".eps";
    c1->Print(outname);

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
