// ===========================================================================
// 
//       Filename:  PlotEff.C
// 
//    Description:  A script to plot the eff
// 
//        Version:  1.0
//        Created:  09/05/2013 05:08:02 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include <cstdlib>
#include "HTSample.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TCanvas.h"
#include <sstream>
#include <iomanip>
#include "StopStyle.hh"
#include <cstring>
#include <cstdlib>

int DrawTitle();

int main ( int argc, char *argv[] )
{

  StopStyle();
  std::vector<std::string> vDec;
  //vDec.push_back("PhaseI");
  //vDec.push_back("PhaseII3");
  //vDec.push_back("PhaseII4");

  vDec.push_back("PhaseI_0PU");
  vDec.push_back("PhaseI_140PU");
  vDec.push_back("PhaseII3_140PU");
  vDec.push_back("PhaseII4_140PU");


  std::vector<std::string> vLep;
  vLep.push_back("EleEta");
  //vLep.push_back("ElePt");
  vLep.push_back("MuonEta"); 
  //vLep.push_back("MuonPt");
  //vLep.push_back("TauEta"); 
  //vLep.push_back("TauPt");
  //vLep.push_back("JetEta"); 
  //vLep.push_back("JetPt");
  //vLep.push_back("JetPt");
  //vLep.push_back("MET");
  //vLep.push_back("MHT");

    int cut = 0;
    //int cut = 5; //Before lepton veto
    //int cut = 14; //After METCut
  std::vector<int> VColor;
  VColor.push_back(1);
  VColor.push_back(2);
  VColor.push_back(4);
  VColor.push_back(6);
  VColor.push_back(kGreen+2);
  VColor.push_back(kOrange+9);
  VColor.push_back(60);


  TCanvas *c1 = new TCanvas("fd", "dfdf", 600, 500);
  TLegend *lg = NULL;
  //c1->SetGridx();
  //c1->SetGridy();

  for (int j = 0; j < vLep.size(); ++j)
  {
    if (vLep.at(j).find("Pt") != std::string::npos)
      lg = new TLegend(0.602349,0.1631356,0.8389262,0.4576271,NULL,"brNDC");
    else if (vLep.at(j).find("EleEta") != std::string::npos )
      //lg = new TLegend(0.3691275,0.1751055,0.6057047,0.4683544,"W(e#nu) + jets","brNDC");
      lg = new TLegend(0.3238255,0.1673729,0.5486577,0.4618644,"W(e#nu) + jets","brNDC");
    else if (vLep.at(j).find("MuonEta") != std::string::npos)
      //lg = new TLegend(0.3691275,0.1751055,0.6057047,0.4683544,"W(#mu#nu) + jets","brNDC");
      lg = new TLegend(0.3238255,0.1673729,0.5486577,0.4618644,"W(#mu#nu) + jets","brNDC");

      //lg = new TLegend(0.3791946,0.1737288,0.6157718,0.4682203,NULL,"brNDC");
    else if (vLep.at(j).find("JetEta") != std::string::npos)
      lg = new TLegend(0.3657718,0.7139831,0.6241611,0.9533898,NULL,"brNDC");
    else if (vLep.at(j).find("TauEta") != std::string::npos)
      lg = new TLegend(0.3842282,0.654661,0.6208054,0.9491525,NULL,"brNDC");
    else
      lg = new TLegend(0.3791946,0.1737288,0.6157718,0.4682203,NULL,"brNDC");


    //else if (vLep.at(j).find("EleEta") != std::string::npos)
    lg->SetBorderSize(0);
    lg->SetFillStyle(0); //transparent hollow?
    lg->SetTextFont(62); 
    //lg->SetTextSize(0.03);
    lg->SetTextSize(0.04);

    double ymin = 0.0;
    double ymax = 0.0;
    TAxis *yaxis = NULL;
    TAxis *xaxis = NULL;


    for (int i = 0; i < vDec.size(); ++i)
    {

      char pu[10];
      char det[10];
      sscanf(vDec.at(i).c_str(), "%[^_]_%s", det, pu);

      std::cout <<  " pu " << pu << "  "<< strcmp(pu, "0PU") << std::endl;

      std::string pileup ="" ;
      if (strcmp(pu, "0PU") == 0)  pileup = "NoPileUp";
      if (strcmp(pu, "50PU") == 0) pileup = "50PileUp";
      if (strcmp(pu, "140PU") == 0) pileup = "140PileUp";
      std::cout << " pileup " << pileup <<" dec " << det << std::endl;

      HTSample *HT = new HTSample("LPC_MHTCUT/", "Wlv*_14TEV_HT", pileup, det);
      HT->InitSample(3000*1000);
      c1->cd();
      c1->Update();

      TH1F* reco = NULL;
      TH1F* gen = NULL;
      //TH2D* reco = NULL;
      //TH2D* gen = NULL;


      std::cout << " " << vLep.at(j) << std::endl;
      if (vLep.at(j) != "MET" && vLep.at(j) != "MHT")
      {
        TString reconame = "AppMatched"+vLep.at(j);
        TString genname = "AppGen"+vLep.at(j);
        //reco = (TH2D*)HT->GetTH2D(reconame.Data(), cut);
        //gen = (TH2D*)HT->GetTH2D(genname.Data(), cut);
        reco = (TH1F*)HT->GetTH1(reconame.Data(), cut);
        gen = (TH1F*)HT->GetTH1(genname.Data(), cut);
      } else {
        //reco = (TH1F*)HT->GetTH1(vLep.at(j), cut);
        //gen = (TH1F*)HT->GetTH1("GenMet");
      }


      if (vLep.at(j).find("Pt") != std::string::npos)
      {
        reco->Rebin(5);
        gen->Rebin(5);
        
      }
      //TH1F* reco = (TH1F*)HT->GetTH1("JetEta", cut);
      //TH1F* gen = (TH1F*)HT->GetTH1("GenJetEta", cut);
      //TH1F* reco = (TH1F*)HT->GetTH1("MuonPt", cut);
      //reco->Rebin(5);
      //TH1F* gen = (TH1F*)HT->GetTH1("GenMuonPt", cut);
      //gen->Rebin(5);
      //TH1F* reco = (TH1F*)HT->GetTH1("ElePt", cut);
      //
      //TH1F* gen = (TH1F*)HT->GetTH1("GenElePt", cut);

      reco->Divide(gen);
      reco->SetTitle("");
      //gen->Draw();
      //gen->SetLineColor(1);
      reco->SetLineWidth(3);
      reco->SetLineColor(VColor.at(i));

      ymin = ymin < reco->GetMinimum() ? ymin : reco->GetMinimum();
      ymax = ymax > reco->GetMaximum() ? ymax : reco->GetMaximum();
      if (i == 0)
      {
        yaxis = reco->GetYaxis();
        xaxis = reco->GetXaxis();
        reco->Draw();
      }
      else
      {
        reco->Draw("same");
      }

      reco->GetYaxis()->SetTitle("Efficiency");

      if (vDec.at(i).find("PU") != std::string::npos)
      {

        std::cout << " vDec.at(i)" << vDec.at(i) << std::endl;
        std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
        if (vDec.at(i) == "PhaseI_0PU")    
        {
          lg->AddEntry(reco, "Phase I, <PU>=0",   "fl");
          reco->SetLineColor(1);
        }
        if (vDec.at(i) == "PhaseI_140PU")  
        { 
          lg->AddEntry(reco, "Phase I, <PU>=140", "fl");
          reco->SetLineColor(4);
        }
        if (vDec.at(i) == "PhaseII3_140PU") 
        {
          lg->AddEntry(reco, "Phase II Conf3, <PU>=140", "fl");
          //lg->AddEntry(reco, "PhaseII3 <PU>=140", "fl");
          reco->SetLineColor(kGreen+2);
        }
        if (vDec.at(i) == "PhaseII4_140PU") 
        {
          lg->AddEntry(reco, "Phase II Conf4, <PU>=140", "fl");
          reco->SetLineColor(2);
        }


        //TString leg = vDec.at(i);
        //lg->AddEntry(reco, leg.ReplaceAll("_", " "), "l");
      }
      TString xlabel = reco->GetXaxis()->GetTitle();
      std::cout << xlabel << std::endl;
      if (xlabel =="#Pt_{Matched m} [GeV]") 
        std::cout<<"Run to \033[0;31m"<<__func__<<"\033[0m at \033[1;36m"<< __FILE__<<"\033[0m, line \033[0;34m"<< __LINE__<<"\033[0m"<< std::endl; 
      if (xlabel =="#eta_{Matched e}" ) xlabel = "#eta_{e}";
      if (xlabel =="#eta_{Matched m}" ) xlabel = "#eta_{#mu}";
      if (xlabel =="#eta_{Matched t}" ) xlabel = "#eta_{t}";
      if (xlabel =="#Pt_{Matched e} [GeV] " ) xlabel = "#P_{T}^{e} [GeV]";
      if (xlabel =="#Pt_{Matched m} [GeV] " ) xlabel = "#P_{T}^{#mu} [GeV]";
      if (xlabel =="#Pt_{Matched t} [GeV] " ) xlabel = "#P_{T}^{t} [GeV]";
      reco->GetXaxis()->SetTitle(xlabel);



      reco->GetYaxis()->SetTitleOffset(1.0);
      reco->GetXaxis()->SetTitleOffset(0.9);
      reco->GetYaxis()->SetTitleSize(0.06);
      reco->GetXaxis()->SetTitleSize(0.06);
      reco->GetYaxis()->SetLabelSize(0.05);
      reco->GetXaxis()->SetLabelSize(0.05);
      //reco->GetYaxis()->SetRangeUser(0.1*ymin, 5*ymax);
      //c1->Print("Pt.png");
      //c1->Print("MuonEta_0.png");
      //c1->Print("EleEta_0.png");
      //TH1F* jet = (TH1F*)HT->GetTH1("JetPTScale", cut);
      //jet->Draw();
      //c1->Print("Jet.png");

      delete HT;

    }

    yaxis->SetRangeUser(0.8*ymin, 1.2*ymax);
    xaxis->SetRangeUser(-4.5, 4.5);
    lg->Draw();
    DrawTitle();
    c1->RedrawAxis();

    std::stringstream ss;
    ss << "Efficiency_"<<vLep.at(j)<< "_"<< cut<< ".png";
    c1->Print(ss.str().c_str());
    ss.str("");
    ss << "Efficiency_"<<vLep.at(j)<< "_"<< cut<< ".pdf";
    c1->Print(ss.str().c_str());
    ss.str("");
    ss << "Efficiency_"<<vLep.at(j)<< "_"<< cut<< ".C";
    c1->Print(ss.str().c_str());
    ss.str("");
    ss << "Efficiency_"<<vLep.at(j)<< "_"<< cut<< ".root";
    c1->Print(ss.str().c_str());
  }

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

// ===  FUNCTION  ============================================================
//         Name:  DrawTitle
//  Description:  
// ===========================================================================
int DrawTitle()
{
  std::string title = " fd";
  
  TLatex TitleBox;
  TitleBox.SetNDC();
  TitleBox.SetTextSize(0.04);
  TitleBox.DrawLatex(0.13255,0.940928, "CMS Simulation, #sqrt{s} = 14 TeV");
  //TitleBox.DrawLatex(0.13255,0.940928, "CMS Simulation, #sqrt{s} = 14 TeV, L = 3000 fb^{-1}");
  return 1;
}       // -----  end of function DrawTitle  -----
