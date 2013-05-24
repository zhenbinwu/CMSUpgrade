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
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "TStyle.h"
#include "TColor.h"
#include "TROOT.h"
#include "TLatex.h"

void StopStyle();
void TopStyle();
void SetTitleLumi(std::string title, double lumi);
