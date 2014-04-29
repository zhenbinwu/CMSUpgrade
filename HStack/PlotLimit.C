// ===========================================================================
// 
//       Filename:  PlotLimit.C
// 
//    Description:  Plot limit
// 
//        Version:  1.0
//        Created:  09/15/2013 04:00:58 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================



#include <cstdlib>
#include <map>
#include "TH1F.h"
#include "TF1.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include <sstream>
#include "StopStyle.hh"
#include "TString.h"
#include <fstream>
#include "TGraph.h"
#include "TLegend.h"

//const std::string filename = "./Default_CLimit.log";
//const std::string filename = "./Default_Discovery.log";
//const std::string filename = "./Reduce_CLimit.log";
//const std::string filename = "./NewDefault_Discovery2.log";
std::string filename = "./NewReduce_Discovery.log";
//const std::string filename = "./Reduce_Discovery.log";
int plateu = 1;
//std::string style = "CLimit";
std::string style = "";
std::vector<int> VColor;

double GetNumber(int plateu, int sig, std::string pu, std::string det);
TGraph* PlotLine(int plateu, std::string pu, std::string det);
bool PlotLines(TCanvas* c1, TLegend* leg);

int main ( int argc, char *argv[] )
{
  filename = argv[1];
  plateu = atoi(argv[2]);

  StopStyle();

  TCanvas *c1 = new TCanvas("XS" , "fuck" , 600, 500);
  c1->SetLogy();
  //TLegend *leg = new TLegend(0.5889262,0.5889831,0.9446309,0.9385593,NULL,"brNDC");
  //TLegend *leg = new TLegend(0.1526846,0.1588983,0.5083893,0.5084746,NULL,"brNDC");
  TLegend *leg = new TLegend(0.3456376,0.5949367,0.7013423,0.8987342,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0); //transparent hollow?
  leg->SetTextFont(62); 
  leg->SetTextSize(0.03);
  TAxis *Xaxis = NULL;
  TAxis *Yaxis = NULL;

  VColor.push_back(1);
  VColor.push_back(2);
  VColor.push_back(4);
  VColor.push_back(kOrange+9);
  VColor.push_back(kGreen+2);
  VColor.push_back(6);
  VColor.push_back(60);

  if (filename.find("Discovery") != std::string::npos)
    style = "Discovery";
  if (filename.find("CLimit") != std::string::npos)
    style = "CLimit";

//----------------------------------------------------------------------------
//  Default input cross section
//----------------------------------------------------------------------------
  std::map<int, double> SigXs;
  SigXs[112] = 46;
  SigXs[200] = 11.47;
  SigXs[500] = 0.45;

  double x[3], y[3];

  x[0] = 112;
  x[1] = 200;
  x[2] = 500;

  y[0] = SigXs[x[0]] * 1;
  y[1] = SigXs[x[1]] * 1;
  y[2] = SigXs[x[2]] * 1;


//----------------------------------------------------------------------------
//  A Test pave
//----------------------------------------------------------------------------
//// Add to the plot the S/B ratio
  TPaveText *pt = 0;
  pt = new TPaveText(0.5587248,0.7669492,0.9446309,0.9110169,"brNDC");
  pt->SetFillColor(0);
  pt->SetBorderSize(0);
  pt->SetTextSize(0.04);
  pt->SetTextColor(4);
  std::stringstream lumi;

  lumi << "3000 fb^{-1}, 14 TeV";
  pt->AddText(lumi.str().c_str());
  lumi.str("");

  if (style == "CLimit")
  lumi << "95\% C.L. Limit";
  if (style == "Discovery")
  lumi << "Discovery sensitivity";
  pt->AddText(lumi.str().c_str());
  lumi.str("");

  char scenario = ' ';
  if (plateu == 10) scenario = 'A';
  if (plateu == 5) scenario = 'B';
  if (plateu == 1) scenario = 'C';
  if (scenario != ' ')
  {
    lumi << "Scenario " << scenario;
    pt->AddText(lumi.str().c_str());
  }

//----------------------------------------------------------------------------
//  Cross Section
//----------------------------------------------------------------------------
  TGraph* xs = new TGraph(3, x, y);

  xs->SetMarkerStyle(10);
  xs->SetTitle("");
  xs->SetLineColor(VColor[0]);

  Xaxis = xs->GetXaxis();
  Yaxis = xs->GetYaxis();
  leg->AddEntry(xs, "Wino", "l");
  xs->Draw("ALP");

  PlotLines(c1, leg);

////----------------------------------------------------------------------------
//// End
////----------------------------------------------------------------------------
  Yaxis->SetRangeUser(0.1, 300);
  ////Yaxis->SetRangeUser(0, 70);
  Yaxis->SetTitle("#sigma (fb)");
  Xaxis->SetTitle("m(#tilde{#chi}^{0}_{1}) [GeV]");

  //fit->Draw("same");
  pt->Draw();
  TString outname = filename;
  outname.ReplaceAll(".log", "");
  std::stringstream ss;
  ss<< outname.Data()<< "_" << plateu << "p.pdf";
  c1->SaveAs(ss.str().c_str());
  ss.str("");
  ss<< outname.Data()<< "_" << plateu << "p.png";
  c1->SaveAs(ss.str().c_str());

  return EXIT_SUCCESS;
}				// ----------  end of function main  ----------

// ===  FUNCTION  ============================================================
//         Name:  PlotLines
//  Description:  
// ===========================================================================
bool PlotLines(TCanvas* c1, TLegend* leg)
{
  
  c1->cd();
  std::vector<std::string> vLegend;
  vLegend.push_back("PhaseI_0PU");
  vLegend.push_back("PhaseI_140PU");
  vLegend.push_back("PhaseII3_140PU");
  vLegend.push_back("PhaseII4_140PU");

  std::map<std::string, TGraph*> vMap;
  vMap["PhaseI_0PU"] = PlotLine(plateu, "NoPileUp", "PhaseI");
  vMap["PhaseI_140PU"] = PlotLine(plateu, "140PileUp", "PhaseI");
  vMap["PhaseII3_140PU"] = PlotLine(plateu, "140PileUp", "PhaseII3");
  //vMap["PhaseII4_140PU"] = PlotLine(plateu, "140PileUp", "PhaseII4");
  vMap["PhaseII4_140PU"] = PlotLine(plateu, "140PileUp", "PhaseII4v2");


  for (int i = 0; i < vLegend.size(); ++i)
  {
    vMap[vLegend.at(i)]->SetLineColor(VColor[i+1]);
    leg->AddEntry( vMap[vLegend.at(i)], vLegend.at(i).c_str(), "l");
    vMap[vLegend.at(i)]->Draw("sameLP");
  }

  leg->Draw("same");
  return true;
}       // -----  end of function PlotLines  -----

// ===  FUNCTION  ============================================================
//         Name:  GetLine
//  Description:  
// ===========================================================================
TGraph* PlotLine(int plateu, std::string pu, std::string det)
{
  

//----------------------------------------------------------------------------
//  Default input cross section
//----------------------------------------------------------------------------
  std::map<int, double> SigXs;
  SigXs[112] = 46;
  SigXs[200] = 11.47;
  SigXs[500] = 0.45;

  double x[3], y[3];

  x[0] = 112;
  x[1] = 200;
  x[2] = 500;
  
  for (int i = 0; i < 3; ++i)
  {
    y[i] = SigXs[x[i]] * GetNumber(plateu, x[i], pu, det) ;
    std::cout << " pu " << pu << " det " << det <<" signal " << x[i] <<" scal " << y[i] << std::endl;
  }

  TGraph* xs = new TGraph(3, x, y);
  xs->SetMarkerStyle(34);

  return xs;
}       // -----  end of function PlotLine  -----

// ===  FUNCTION  ============================================================
//         Name:  GetNumber
//  Description:  
// ===========================================================================
double GetNumber(int plateu, int sig, std::string pu, std::string det)
{


  std::fstream file;
  file.open(filename.c_str(), std::fstream::in);
  if (!file.is_open())
  {
    std::cout << " Error to open the Cross Section file!" << std::endl;
    return false;
  }

//----------------------------------------------------------------------------
//  Get the line we are looking for
//----------------------------------------------------------------------------
   std::stringstream ss;
   if (sig == 112) sig = 100;
   ss << "_" << plateu <<"p_" << "Wino" << sig << "_" << pu <<"_" << det;

//----------------------------------------------------------------------------
//  Reading file to get cross section
//----------------------------------------------------------------------------
  std::string line;
  float xs = 0;
  float xserr = 0;


  while (!file.eof())
  {
    getline(file, line);
    if (line.find(ss.str()) == std::string::npos)
        continue;
    
    std::cout << line << std::endl;
    char inputtext[100];
    char buf[100];

    if (style == "CLimit")
      sscanf(line.c_str(), "%s %f%%: r < %f", inputtext, &xserr, &xs);


    if (style == "Discovery")
-    sscanf(line.c_str(), "%s ===== Signal Scale %f , Significance: %f",  inputtext, &xs, &xserr);


      std::cout << "=============" << inputtext <<"  " << xs <<"  " << xserr<< std::endl;

    //std::cout << inputtext <<"  " << xs <<"  " << xserr << std::endl;
  }

  file.close();

  return xs;
}       // -----  end of function GetNumber  -----


