// ===========================================================================
// 
//       Filename:  Run.C
// 
//    Description:  A simple root micro to analysis the tree before I figure
//    out the compilation problem with the vector of TLorentzvector 
// 
//        Version:  1.0
//        Created:  05/17/2013 02:28:12 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

{

  gStyle->SetOptStat(1111111);
  gROOT->ProcessLine(".L AUX.C+"); 
  gROOT->ProcessLine(".L Loop.C+"); 
  Loop();

  //{
    //f.SetCutBit("0000000001");
    //AUX f("histAndTree_forVBFSusy/AUX", "../VBFSusy.root");
    //f.SetName("test");
    //f.Loop();
  //}

  //{
    //AUX f("histAndTree_forVBFSusy/AUX", "Root/CMS/VBFSusy_14TeV_TtoAnything_pythia6_PU50.root");
    //f.SetName("C1PU0_NoCut");
    //f.Loop();
  //}

}
