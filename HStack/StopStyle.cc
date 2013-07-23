// ===========================================================================
// 
//       Filename:  StopStyle.cc
// 
//    Description:  This is the style file for single top analysis 
// 
//        Version:  1.0
//        Created:  02/05/2012 05:56:40 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "StopStyle.hh"
#include <iostream>

void StopStyle()
{
  TStyle *SStyle = new TStyle("STOPstyle","STOPstyle");

  //Palette
  double start_angle = 270;
  double stop_angle = 0;
  double light = 0.5;
  double satur = 1;

  const int nMyColors = 99;
  int mycolors[nMyColors];
  for (int i=0; i<nMyColors; i++) {
    float r,g,b;
    float v = (float) i/ (float) nMyColors * (stop_angle - start_angle) + start_angle;
    TColor::HLS2RGB(v, light, satur, r, g, b);
    mycolors[i] = TColor::GetColor(r,g,b);
  }
  SStyle->SetPalette(nMyColors,mycolors);

//----------------------------------------------------------------------------
//  Another Color set for limit
//----------------------------------------------------------------------------
  

  Int_t MyPalette[3];
   Double_t r[3]    = {0.988, 1.0, 1.0};
   Double_t g[3]    = {0.867, 0.604, 0.227};
   Double_t b[3]    = {0.0, 0.0, 0.0};
   Double_t stop[3] = {0.2, 0.2, 0.2};
   //Int_t FI = TColor::CreateGradientColorTable(3, stop, r, g, b, 3);
   //for (int i=0;i<3;i++) MyPalette[i] = FI+i;
   MyPalette[0] = kYellow;
   MyPalette[1] = TColor::GetColor("#ff9a00");
   MyPalette[2] = TColor::GetColor("#ff3a00");
  SStyle->SetPalette(3, MyPalette);
  //gStyle->SetPalette(nBenColors, Bencolors);


  //Canvas
  SStyle->SetCanvasBorderMode(0); //frame color of canvas
  SStyle->SetCanvasColor(0);  //bkrd color of canvas
  SStyle->SetCanvasDefH      (600);
  SStyle->SetCanvasDefW      (600);

  //Pad
  SStyle->SetPadColor(0);
  SStyle->SetPadBorderMode(0);
  //SStyle->SetPadLeftMargin(0.08);
  //SStyle->SetPadRightMargin(0.08);
  //SStyle->SetPadTopMargin(0.10);//(0.05);          
  //SStyle->SetPadBottomMargin(0.10);      
  SStyle->SetPadLeftMargin(0.13);
  SStyle->SetPadRightMargin(0.05);
  SStyle->SetPadTopMargin(0.05); 
  SStyle->SetPadBottomMargin(0.13);      
  SStyle->SetPadTickX(0); //1:ticks on upper,2: ticks+labels on upper xaxis
  SStyle->SetPadTickY(0); //1:ticks on right side

  //Frames
  SStyle->SetFrameLineWidth(3);
 
  //Stat
  SStyle->SetOptStat(0);//1110 rme;110 me;1010 er;11100 mru;1100 mr;1001100 irm;1000100 im
  SStyle->SetOptFit(0);    //Fit paras
  SStyle->SetStatColor(0);  // bkrd color of stat-box
  SStyle->SetStatBorderSize(0); //frame style of stat-box 1
  SStyle->SetStatW(0.5);  //width of stat-box

  //Title
  SStyle->SetTitleBorderSize(0);
  SStyle->SetTitleFillColor(0);
  SStyle->SetLineWidth(2); // width of ticks
  SStyle->SetLineColor(2); // width of ticks

  // Histograms
  SStyle->SetHistLineWidth(3);
  SStyle->SetHistFillColor(2);
  SStyle->SetHistLineColor(2);

  //Axis
  SStyle->SetTickLength ( 0.015,"X");
  SStyle->SetTitleSize  ( 0.07,"X");
  SStyle->SetNdivisions ( 505 ,"X");
  SStyle->SetLabelSize  ( 0.05,"X");
  SStyle->SetTitleOffset( 0.8,"X");
  //SStyle->SetTitleColor ( 1    ,"X");
  //SStyle->SetLabelOffset( 0.01,"X");
  //SStyle->SetLabelFont  ( 42   ,"X");

  SStyle->SetTickLength ( 0.015,"Y");
  SStyle->SetTitleSize  ( 0.07,"Y");
  SStyle->SetLabelSize  ( 0.05,"Y");
  SStyle->SetTitleOffset( 1.3,"Y");
  SStyle->SetNdivisions ( 505   ,"Y");
  //SStyle->SetLabelOffset( 0.01,"Y");
  //SStyle->SetLabelFont  ( 42   ,"Y");
  //SStyle->SetTitleFont  ( 42   ,"Y");

  //Legends 
      //leg->SetBorderSize(0);
      //leg->SetFillStyle(0); //transparent hollow?
      //leg->SetTextFont(62); 
      //leg->SetTextSize(0.045);

  SStyle->SetStatBorderSize(0);
  SStyle->SetStatFont      (42);
  SStyle->SetOptStat       (0);
  SStyle->SetStatColor     (0);

  gROOT->SetStyle("STOPstyle");
}


void TopStyle()
{

  //****************************STYLE***********************************

  //const char* modified = "Borrowed and adapted from paus et al";

  TStyle *RootStyle = new TStyle("Root-Style","The Perfect Style for Plots ;-)");
  
#ifdef __CINT__
  TStyle *GloStyle;
  GloStyle = gStyle;                          // save the global style reference
  gStyle = RootStyle;
#endif

// otherwise you need to call TROOT::SetStyle("Root-Style")

  // Paper size

  RootStyle->SetPaperSize(TStyle::kUSLetter);

  // Canvas

  RootStyle->SetCanvasColor     (0);
  RootStyle->SetCanvasBorderSize(10);
  RootStyle->SetCanvasBorderMode(0);
  RootStyle->SetCanvasDefH      (600);
  RootStyle->SetCanvasDefW      (600);
  RootStyle->SetCanvasDefX      (10);
  RootStyle->SetCanvasDefY      (10);

  // Pads

  RootStyle->SetPadColor       (0);
  RootStyle->SetPadBorderSize  (2);
  RootStyle->SetPadBorderMode  (0);
  RootStyle->SetPadBottomMargin(0.15);
  RootStyle->SetPadTopMargin   (0.10);
  RootStyle->SetPadLeftMargin  (0.14);
  RootStyle->SetPadRightMargin (0.07);
  RootStyle->SetPadGridX       (0);
  RootStyle->SetPadGridY       (0);
  RootStyle->SetPadTickX       (1);
  RootStyle->SetPadTickY       (1);

  // Frames

  RootStyle->SetFrameFillStyle ( 0);
  RootStyle->SetFrameFillColor ( 0);
  RootStyle->SetFrameLineColor ( 1);
  RootStyle->SetFrameLineStyle ( 0);
  RootStyle->SetFrameLineWidth ( 2);
  RootStyle->SetFrameBorderSize(10);
  RootStyle->SetFrameBorderMode( 0);


  // Histograms

  // RootStyle->SetHistFillColor(2);
//   RootStyle->SetHistFillStyle(1);
  //RootStyle->SetHistLineColor(1);
  //RootStyle->SetHistLineStyle(0);
  RootStyle->SetHistLineWidth(2);

  // Functions

  RootStyle->SetFuncColor(1);
  RootStyle->SetFuncStyle(0);
  RootStyle->SetFuncWidth(2);

  //Legends 

  RootStyle->SetStatBorderSize(0);
  RootStyle->SetStatFont      (42);
  RootStyle->SetOptStat       (1111);
//   RootStyle->SetOptStat       (0);
  RootStyle->SetStatColor     (0);
//  RootStyle->SetStatX         (1.2);
//   RootStyle->SetStatY         (1.2);
   RootStyle->SetStatW         (0.25);
   RootStyle->SetStatH         (0.20);

  // Labels, Ticks, and Titles

  RootStyle->SetTickLength ( 0.015,"X");
  RootStyle->SetTitleSize  ( 0.060,"X");
  RootStyle->SetTitleColor ( 1    ,"X");
  RootStyle->SetTitleOffset( 1.100,"X");
  RootStyle->SetLabelOffset( 0.015,"X");
  RootStyle->SetLabelSize  ( 0.050,"X");
  RootStyle->SetLabelFont  ( 42   ,"X");
  RootStyle->SetNdivisions ( 505 ,"X");

  RootStyle->SetTickLength ( 0.015,"Y");
  RootStyle->SetTitleSize  ( 0.060,"Y");
  RootStyle->SetTitleOffset( 1.100,"Y");
  RootStyle->SetLabelOffset( 0.015,"Y");
  RootStyle->SetLabelSize  ( 0.050,"Y");
  RootStyle->SetLabelFont  ( 42   ,"Y");
  RootStyle->SetTitleFont  ( 42   ,"Y");
  RootStyle->SetNdivisions ( 505   ,"Y");

  RootStyle->SetTickLength ( 0.015,"Z");
  RootStyle->SetTitleSize  ( 0.060,"Z");
  RootStyle->SetTitleOffset( 1.100,"Z");
  RootStyle->SetLabelOffset( 0.015,"Z");
  RootStyle->SetLabelSize  ( 0.050,"Z");
  RootStyle->SetLabelFont  ( 42   ,"Z");
  RootStyle->SetTitleFont  ( 42   ,"Z");
  RootStyle->SetNdivisions ( 505   ,"Z");


  RootStyle->SetTitleBorderSize  (0);
  RootStyle->SetTitleFillColor  (0);  
  RootStyle->SetTitleFont  (42);
  RootStyle->SetTitleColor  (1);

  // Options

  RootStyle->SetOptFit     (11);
  RootStyle->SetOptStat    (1110);
//RootStyle->SetOptFit     (0);
// RootStyle->SetOptStat    (0);
 //  RootStyle->SetMarkerStyle(20);
  RootStyle->SetMarkerSize(0.9);

  RootStyle->SetPalette(42, NULL);

}

void SetTitleLumi(std::string title, double lumi)
{
  TLatex CDFBox;
  CDFBox.SetNDC();
  CDFBox.SetTextSize(0.04);
  TString datafb;
    datafb+=lumi;

  CDFBox.DrawLatex(0.558,0.94,"CDF II Preliminary "+datafb+" fb^{-1}");//------------------------- PRD
  std::cout<< "lumi " << datafb << std::endl;
  //CDFBox.DrawLatex(0.69,0.94,"CDF II "+datafb+" fb^{-1}");//---------------------------------------- PRD
  
  TLatex TitleBox;
  TitleBox.SetNDC();
  TitleBox.SetTextSize(0.04);
  TitleBox.DrawLatex(0.178,0.94,title.c_str());
}

