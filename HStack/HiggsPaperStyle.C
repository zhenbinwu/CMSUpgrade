#include "StopStyle.hh"
//const Color_t color_data  = kBlack;
//const Color_t color_higgs = kRed;
//const Color_t color_WW    = kAzure-9;
//const Color_t color_tt    = kYellow;
//const Color_t color_Wj    = kGray+1;
//const Color_t color_Zj    = kGreen+2;
//const Color_t color_WZ    = kAzure-2;


//const std::vector<Color_t> vHiggsColor;
//vHiggsColor.push_back(color_data);
//vHiggsColor.push_back(color_higgs);
//vHiggsColor.push_back(color_WW);
//vHiggsColor.push_back(color_tt);
//vHiggsColor.push_back(color_Wj);
//vHiggsColor.push_back(color_Zj);
//vHiggsColor.push_back(color_WZ);

void HiggsPaperStyle()
{
  TStyle* HiggsPaperStyle = new TStyle("HiggsPaperStyle", "HiggsPaperStyle");

  gStyle = HiggsPaperStyle;


  //----------------------------------------------------------------------------
  // Canvas
  //----------------------------------------------------------------------------
  HiggsPaperStyle->SetCanvasBorderMode(  0);
  HiggsPaperStyle->SetCanvasBorderSize( 10);
  HiggsPaperStyle->SetCanvasColor     (  0);
  HiggsPaperStyle->SetCanvasDefH      (600);
  HiggsPaperStyle->SetCanvasDefW      (550);
  HiggsPaperStyle->SetCanvasDefX      ( 10);
  HiggsPaperStyle->SetCanvasDefY      ( 10);


  //----------------------------------------------------------------------------
  // Pad
  //----------------------------------------------------------------------------
  HiggsPaperStyle->SetPadBorderMode  (   0);
  HiggsPaperStyle->SetPadBorderSize  (  10);
  HiggsPaperStyle->SetPadColor       (   0);
  //KH HiggsPaperStyle->SetPadBottomMargin(0.20);
  HiggsPaperStyle->SetPadBottomMargin(0.15);
  HiggsPaperStyle->SetPadTopMargin   (0.08);
  //KH HiggsPaperStyle->SetPadLeftMargin  (0.18);
  HiggsPaperStyle->SetPadLeftMargin  (0.15);
  HiggsPaperStyle->SetPadRightMargin (0.05);


  //----------------------------------------------------------------------------
  // Frame
  //----------------------------------------------------------------------------
  HiggsPaperStyle->SetFrameBorderMode( 0);
  HiggsPaperStyle->SetFrameBorderSize(10);
  HiggsPaperStyle->SetFrameFillStyle ( 0);
  HiggsPaperStyle->SetFrameFillColor ( 0);
  HiggsPaperStyle->SetFrameLineColor ( 1);
  HiggsPaperStyle->SetFrameLineStyle ( 0);
  HiggsPaperStyle->SetFrameLineWidth ( 2);


  //----------------------------------------------------------------------------
  // Hist
  //----------------------------------------------------------------------------
  //KH HiggsPaperStyle->SetHistFillColor(0);
  HiggsPaperStyle->SetHistFillStyle(3001);
  HiggsPaperStyle->SetHistLineColor(1);
  HiggsPaperStyle->SetHistLineStyle(0);
  HiggsPaperStyle->SetHistLineWidth(1);


  //----------------------------------------------------------------------------
  // Axis
  //----------------------------------------------------------------------------
  HiggsPaperStyle->SetLabelFont  (   42, "xyz");
  HiggsPaperStyle->SetLabelOffset(0.015, "xyz");
  HiggsPaperStyle->SetLabelSize  (0.040, "xyz");
  //KH HiggsPaperStyle->SetNdivisions (  505, "xyz");
  HiggsPaperStyle->SetTitleFont  (   42, "xyz");
  //KH HiggsPaperStyle->SetTitleSize  (0.040, "xyz");
  HiggsPaperStyle->SetTitleSize  (0.045, "xyz");
  //KH HiggsPaperStyle->SetTitleOffset( 1.15,   "x");
  //KH HiggsPaperStyle->SetTitleOffset( 1.15,   "y");
  HiggsPaperStyle->SetTitleOffset( 1.15,   "x");
  HiggsPaperStyle->SetTitleOffset( 1.15,   "y");
  HiggsPaperStyle->SetPadTickX   (           1);
  HiggsPaperStyle->SetPadTickY   (           1);


  //----------------------------------------------------------------------------
  // Title
  //----------------------------------------------------------------------------
  HiggsPaperStyle->SetTitleAlign     (   33);
  HiggsPaperStyle->SetTitleBorderSize(    0);
  HiggsPaperStyle->SetTitleFillColor (   10);
  HiggsPaperStyle->SetTitleFont      (   42);
  HiggsPaperStyle->SetTitleFontSize  (0.045);
  HiggsPaperStyle->SetTitleX         (0.960);
  HiggsPaperStyle->SetTitleY         (0.990);


  //----------------------------------------------------------------------------
  // Stat
  //----------------------------------------------------------------------------
  HiggsPaperStyle->SetOptStat       (   0);
  HiggsPaperStyle->SetStatBorderSize(   0);
  HiggsPaperStyle->SetStatColor     (  10);
  HiggsPaperStyle->SetStatFont      (  42);
  HiggsPaperStyle->SetStatFontSize  (0.045);
  HiggsPaperStyle->SetStatX         (0.94);
  HiggsPaperStyle->SetStatY         (0.91);
}
