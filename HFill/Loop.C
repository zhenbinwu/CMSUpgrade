// ===========================================================================
// 
//       Filename:  Loop.C
// 
//    Description:  A simple function to loop over 
// 
//        Version:  1.0
//        Created:  05/17/2013 08:33:47 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "AUX.h"
#ifdef __MAKECINT__
#pragma link C++ class std::map<std::string, TH1F*>+;
#endif /* __MAKECINT__ */

int Loop()
{

 
  typedef  std::map<std::string, std::string>  maps;
  typedef  std::map<std::string, std::string>::iterator  maps_it;
  maps CutMap;
  CutMap["0"] = "NoCut";
  CutMap["0000000001"] = "CTVBF";
  CutMap["0000000011"] = "CTLepV";
  CutMap["0000000111"] = "CTMet50";
  CutMap["0000001111"] = "CTMjj";
  CutMap["0000011111"] = "CTMet200";
  CutMap["-1"] = "AllCut";

  maps FileMap;
  FileMap["Root/CMS/VBFSusy_14TeV_C1-500_Stau-350_N1-250_PU0.root"] = "C1PU0";
  FileMap["Root/CMS/VBFSusy_14TeV_C1-500_Stau-350_N1-250_PU50.root"] = "C1PU50";
  FileMap["Root/CMS/VBFSusy_14TeV_C1-500_Stau-350_N1-250_PU50bx25.root"] = "C1PU50x25";
  FileMap["Root/CMS/VBFSusy_14TeV_DYToNuNu.root"] = "DYNN";
  FileMap["Root/CMS/VBFSusy_14TeV_TtoAnything_pythia6_PU50.root"] = "TAll";
  FileMap["Root/CMS/VBFSusy_14TeV_Wino_DM_200GeV_PU0.root"] = "Wino200PU0";

  std::string selection = "histAndTree_forVBFSusy/AUX"; // Name of the tree

  for(maps::iterator f_it=FileMap.begin();
    f_it!=FileMap.end(); f_it++)
  {
    for(maps::iterator c_it=CutMap.begin();
      c_it!=CutMap.end(); c_it++)
    {
      AUX *f = new AUX(selection.c_str(), f_it->first.c_str());
      f->SetCutBit(c_it->first.c_str());
      TString name = f_it->second + "_" + c_it->second.c_str();
      f->SetName(name.Data());
      f->Loop();
      delete f;
     }
    
  }
}
