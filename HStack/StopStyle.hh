// ===========================================================================
// 
//       Filename:  StopStyle.hh
// 
//    Description:  header file for stop style 
// 
//        Version:  1.0
//        Created:  02/05/2012 05:58:23 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "TStyle.h"
#include "TColor.h"
#include "TROOT.h"
#include "TLatex.h"

void StopStyle();
void TopStyle();
void SetTitleLumi(std::string title, double lumi);
void HiggsPaperStyle();

const Color_t color_data  = kBlack;
const Color_t color_higgs = kRed;
const Color_t color_WW    = kAzure-9;
const Color_t color_tt    = kYellow;
const Color_t color_Wj    = kGray+1;
const Color_t color_Zj    = kGreen+2;
const Color_t color_WZ    = kAzure-2;


const std::vector<Color_t> vHiggsColor;
