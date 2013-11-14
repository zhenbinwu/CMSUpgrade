// ===========================================================================
// 
//       Filename:  teest.C
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/20/2013 12:59:03 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================


#include <cstdlib>
#include "TFile.h"
#include "TH1F.h"
#include "HTHis.h"
#include "TCanvas.h"
#include <sstream>
#include "StopStyle.hh"
#include "TLegend.h"
#include <string>

int  main ( int argc, char *argv[] )
{
  StopStyle();
  //HTHis his("./LPC/", "Wino100_14TeV", "140PileUp", "PhaseII3");
  //HTHis his("./LPC/", "Wlv*_14TEV_HT", "NoPileUp", "PhaseI");
  HTHis his("./ECFA_200/", "Wino100_14TeV", "140PileUp", "PhaseII4v2");
  //HTHis his("./ECFA_200/", "Wino100_14TeV", "140PileUp", "PhaseII4v2");
  //HTHis his("./ECFA_50/", "Wlv*_14TEV_HT", "140PileUp", "PhaseII4v2");
  //HTHis his("./ECFA_200/", "Wlv*_14TEV_HT", "140PileUp", "PhaseI");
  his.GetScale(3000*1000);
  TCanvas *c1 = new TCanvas("fdf", " dfsdf", 600, 500);
  c1->SetLogy();
  std::vector<std::string> var;
  //for (int i = 1; i < 5; ++i)
  //{
    //for (int j = 1; j < 11; ++j)
    //{
      //std::stringstream ss;
      //ss <<"JetPTScale" << i <<"-"<<j <<"_0";
      //std::string name = ss.str();
      //var.push_back(name);
    //}
  //}
  std::vector<int> VColor;

  VColor.push_back(1);
  VColor.push_back(2);
  VColor.push_back(4);
  VColor.push_back(kOrange+9);
  VColor.push_back(kGreen+2);
  VColor.push_back(6);
  VColor.push_back(60);

    TLegend *f = new TLegend(0.6744966,0.6313559,0.9110738,0.9258475,NULL,"brNDC");
    f->SetBorderSize(0);
    f->SetFillStyle(0); //transparent hollow?
    f->SetTextFont(62); 
    f->SetTextSize(0.03);
  //var.push_back("JetEta");
  //var.push_back("JetPt");
  //var.push_back("GenJetEta_0");
  //var.push_back("GenJetPt_0");
  //var.push_back("MatchedJetEta_0");
  //var.push_back("MatchedJetPt_0");
  //var.push_back("JetScaleEta");
  //var.push_back("JetScalePT");
  //var.push_back("GenEle");
  //var.push_back("GenMuon_0");
  //var.push_back("GenMuon_13");
  //var.push_back("GenTau");
  //var.push_back("CutFlow");
  //var.push_back("NEle");
  //var.push_back("NMuon");
  //var.push_back("NJet");
  //var.push_back("MET_9");
  //var.push_back("GenMet_0");
  //var.push_back("MET_0");
  //var.push_back("MHT_0");
  var.push_back("GenMet");
  var.push_back("MET");
  var.push_back("MHT");

  //var.push_back("JetEta");
  //var.push_back("PUJetEta_0");
  const int cut = 0;
  //const int cut = 1;
  //const int cut = 7;

  TAxis *yaxis = NULL;
  TAxis *xaxis = NULL;
  double ymax = 0;
  double ymin = 0;
  std::string label = "";
  for (int i = 0; i < var.size(); ++i)
  {
    //TH1F* temp  = his.GetTH1(var.at(i).c_str());
    TH1F* temp  = his.GetTH1(var.at(i).c_str(), cut);
    temp->Rebin(2);

    std::cout << " inte " << temp->Integral() << std::endl;
    if (i == 0)
    {
      //std::string title =temp->GetTitle();
      //unsigned bra = title.find("(");
      //label = title.substr(bra, title.find(")") - bra + 1);
      temp->Draw();
      yaxis = temp->GetYaxis();
      xaxis = temp->GetXaxis();
    }
    else
      temp->Draw("same");


        ymin = ymin < temp->GetMinimum() ? ymin : temp->GetMinimum();
        ymax = ymax > temp->GetMaximum() ? ymax : temp->GetMaximum();

    temp->SetTitle("");
    temp->SetLineWidth(3);
    temp->SetLineColor(VColor.at(i));
    f->AddEntry(temp, var.at(i).c_str(), "l");
  }

  //TString ylabel = "Number of Jets";
  TString ylabel = "Events";
  //TString ylabel = "Events " + label;
    yaxis->SetTitle(ylabel);
    //xaxis->SetRangeUser(0, 500);
    ymin = ymin == 0 ? ymax/100 : ymin;
    yaxis->SetRangeUser(0.1*ymin, 1.5*ymax);
    //xaxis->SetRangeUser(0, 400);
    xaxis->SetTitle("Missing Transverse Energy [GeV]");
    yaxis->SetTitleOffset(1.0);
    xaxis->SetTitleOffset(0.9);
    xaxis->SetTitleSize(0.06);
    yaxis->SetTitleSize(0.06);
    xaxis->SetLabelSize(0.05);
    yaxis->SetLabelSize(0.05);
    f->Draw("same");

  std::stringstream ss;
  //ss<< "Wlv_PhaseI_MET_" << cut <<".pdf";
  ss<< "Wino100_PhaseII4v2_MET_" << cut <<".pdf";
  //ss<< "Wlv_PhaseII4v2_Jet.pdf";
  //ss<< "Wlv_PhaseII4v2_Jet_" << cut <<".pdf";
  //TString out = "Wino100_PhaseII3_MJJ.pdf";
  c1->SaveAs(ss.str().c_str());
  //ss.str("");
  //ss<< "Wino100_PhaseII42_MET_" << cut <<".png";
  ////TString out = "Wino100_PhaseII3_MJJ.pdf";
  //c1->SaveAs(ss.str().c_str());


  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------


