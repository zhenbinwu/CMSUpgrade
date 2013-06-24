// ===========================================================================
// 
//       Filename:  Compare.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/19/2013 02:32:10 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "Compare.h"
#include "TH1F.h"
#include <cmath>
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  Compare
//      Method:  Compare
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Compare::Compare ( std::map<std::string, std::map<std::string, TFile*> > aTagFile,
        std::vector<std::string> aVTag, 
        std::string aNormName )
{
  TagFile = aTagFile;
  VTag = aVTag;
  NormName = aNormName;

  GetScale();
  UserLegend();
}  // ~~~~~  end of method Compare::Compare  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  Compare
//      Method:  Compare
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Compare::Compare ( const Compare &other )
{
  TagFile = other.TagFile;
  VTag = other.VTag;
  NormName = other.NormName;

}  // ~~~~~  end of method Compare::Compare  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  Compare
//      Method:  ~Compare
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Compare::~Compare ()
{
}  // ~~~~~  end of method Compare::~Compare  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  Compare
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Compare&
Compare::operator = ( const Compare &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method Compare::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  Compare::GetScale
//  Description:  General function for get scale factor
// ===========================================================================
bool Compare::GetScale()
{
  // First, get the vector of prefix for all the comparison
  GetPrefix();
  //return GetScaleOne();
  return GetScaleIgl();
}       // -----  end of function Compare::GetScale  -----


// ===  FUNCTION  ============================================================
//         Name:  Compare::GetScale
//  Description:  
// ===========================================================================
bool Compare::GetScaleIgl() 
{
  BOOST_FOREACH(std::string prefix, VPre)
  {

    //Defind local variables for the scale
    std::map<std::string, TH1F*> THmap;

    BOOST_FOREACH(std::string tag, VTag)
    {
      // Normally should use TH1F for normalization
      THmap[tag] = (TH1F*)TagFile[tag][prefix]->Get(NormName.c_str());
      //std::cout << " File name " << TagFile[tag][prefix]->GetName() << std::endl;
    }

    double normal = THmap[*VTag.begin()]->Integral();

    BOOST_FOREACH(std::string tag, VTag)
    {
      HScale[prefix][tag] = normal/THmap[tag]->Integral();
    }

  }
  return true;
}       // -----  end of function Compare::GetScale  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::GetScaleOne
//  Description:  Don't apply scale, which mean just one
// ===========================================================================
bool Compare::GetScaleOne()
{
  BOOST_FOREACH(std::string prefix, VPre)
  {
    BOOST_FOREACH(std::string tag, VTag)
    {
      HScale[prefix][tag] = 1;
    }

  }
  return true;
}       // -----  end of function Compare::GetScaleOne  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::GetPrefix
//  Description:  Cross check and get the prefix of all the comparison
// ===========================================================================
bool Compare::GetPrefix()
{
  // Count for the prefix for the comparison, they should be comsistence
  std::map<std::string, int> PreCount;

    
  BOOST_FOREACH(std::string tag, VTag)
  {
    for(std::map<std::string, TFile*>::iterator it=TagFile[tag].begin();
        it!=TagFile[tag].end(); it++)
    {
      PreCount[it->first]++;
    }
  }

  int pcount = PreCount.begin()->second;
  for(std::map<std::string, int>::iterator it=PreCount.begin();
    it!=PreCount.end(); it++)
  {
    if (it->second != pcount)
    {
      std::cout << " No consistent within the prefix ....." << std::endl;
      return false;
    } else VPre.push_back(it->first);
  }
  return true;
  
}       // -----  end of function Compare::GetPrefix  -----


// ===  FUNCTION  ============================================================
//         Name:  Compare::CompHist
//  Description:  For each his name, compare for each prefix
// ===========================================================================
bool Compare::CompHist(std::string Hname)
{
  // Usually case

  BOOST_FOREACH(std::string prefix, VPre)
  {
    CompEachHist(prefix, Hname);
  }
  return true;
}       // -----  end of function Compare::CompHist  -----


// ===  FUNCTION  ============================================================
//         Name:  Compare::CompEachHist
//  Description:  A general function for the comparison. It first detects
//  whether to plot TH1F or TProfile
// ===========================================================================
bool Compare::CompEachHist(std::string prefix, std::string Hname)
{
  return CompHistTH1(prefix, Hname);
}       // -----  end of function Compare::CompEachHist  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::CompHistTH1
//  Description:  
// ===========================================================================
bool Compare::CompHistTH1(std::string prefix, std::string Hname)
{
   // :TODO:06/19/2013 04:27:22 PM:benwu: Detect whether it is a TH1F or
   // TProfile before initial this vector
   std::vector<ComHis*> VHis; // Vector of ComHis class for the comparison
//----------------------------------------------------------------------------
//  Get the vector of ComHis class
//----------------------------------------------------------------------------

   if (Hname.find("MetRes") != std::string::npos)
   {
    FillMetResVHis(VHis, prefix, Hname);
   }
   else FillVHis(VHis, prefix, Hname);

  if (Hname == "MetX" || Hname == "MetY")
    GetRMSDiff(VHis, prefix);

//----------------------------------------------------------------------------
//  Making Comparison 
//----------------------------------------------------------------------------
   TCanvas* c1 = new TCanvas("Local", "Local only", 600, 500);
   TLegend* lg = PlotComp(c1, VHis, prefix, Hname);

//----------------------------------------------------------------------------
//  Save the plot
//----------------------------------------------------------------------------
   TString outfile = prefix +"_"+ Hname+".png";
   c1->Print(outfile);

   delete c1;
   delete lg;
   return true;
}       // -----  end of function Compare::CompHistTH1  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::InitComHisTH1
//  Description:  Construct a ComHis object as return
// ===========================================================================return
ComHis* Compare::InitComHisTH1(std::string tag, std::string prefix, std::string hname)
{
  ComHis *CH = new ComHis();
  CH->Initial((TH1F*)TagFile[tag][prefix]->Get(hname.c_str()), tag, VTag);
  CH->scale = HScale[prefix][tag];
  UserHist(CH, hname);
  return CH;
}       // -----  end of function Compare::InitComHis  -----


// ===  FUNCTION  ============================================================
//         Name:  Compare::InitComHisTPro
//  Description:  
// ===========================================================================
ComHis* Compare::InitComHisTPro(std::string tag, std::string prefix, std::string hname)
{
  ComHis *CH = new ComHis();
  CH->Initial((TH1F*)TagFile[tag][prefix]->Get(hname.c_str()), tag, VTag);
  //UserDefine(CH, hname);
  return CH;
}       // -----  end of function Compare::InitComHisTPro  -----


// ===  FUNCTION  ============================================================
//         Name:  Compare::UserHist
//  Description:  User to define sepacial setting according to the hisname
// ===========================================================================
bool Compare::UserHist(ComHis *his, std::string hname)
{
  
  if (hname == "MetScale")
  {
    if (his->line) his->drawopt = "HIST";
    his->scale =1;
  }

  if (hname == "MetResX" || hname == "MetResY")
  {
    his->drawopt = "HISTP";
    his->size = 1;
    
    if (hname == "MetResX") his->ylabel = "<#slash{E}_{x}> [GeV]";
    if (hname == "MetResY") his->ylabel = "<#slash{E}_{y}> [GeV]";
    his->line = false;
    his->style = 20;
    his->scale =1;
  }

  return true;
}       // -----  end of function Compare::UserHist  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::PlotComp
//  Description:  
// ===========================================================================
TLegend* Compare::PlotComp(TCanvas* c1, std::vector<ComHis*> &VHis, 
    std::string prefix, std::string hname)
{
  c1->cd();
  c1->Update();

  TLegend *lg = NULL;
  if (LgLoc.find(hname) != LgLoc.end())
    lg = new TLegend(LgLoc[hname].at(0),LgLoc[hname].at(1),
        LgLoc[hname].at(2),LgLoc[hname].at(3), NULL, "brNDC"); 
  else 
    lg = new TLegend(0.6862416,0.5995763,0.9228188,0.8940678, NULL,"brNDC");

  lg->SetBorderSize(0);
  lg->SetFillStyle(0); //transparent hollow?
  lg->SetTextFont(62); 
  lg->SetTextSize(0.045);
  
  int count = 0;
  double ymax = 0.0;
  double ymin = 0.0;
  TAxis *yaxis = NULL;
  TAxis *xaxis = NULL;
  bool LogY = false;
  bool LogX = false;
  BOOST_FOREACH(ComHis *His, VHis)
  {

    His->his->SetTitle("");
    //His->his->SetTitle(prefix.c_str());
    His->his->Scale(His->scale);
    His->his->Rebin(His->rebin);
    if (count == 0)
    {
      if (His->logx) LogX = true;
      if (His->logy) LogY = true;
      His->his->Draw(His->drawopt.c_str());
      His->his->GetXaxis()->SetTitle(His->xlabel.c_str());
      His->his->GetXaxis()->SetTitleSize(0.05);
      His->his->GetYaxis()->SetTitleSize(0.05);
      if (His->rebin != 1) His->his->GetYaxis()->SetTitle("Events");
      else His->his->GetYaxis()->SetTitle(His->ylabel.c_str());

      // For axis
      if (His->ymin != -999.) 
        ymin = His->his->GetMinimum() < His->ymin ? His->his->GetMinimum() : His->ymin;
      else ymin = His->his->GetMinimum();
      if (His->ymax != -999.) 
        ymax = His->his->GetMaximum() > His->ymax ? His->his->GetMaximum() : His->ymax;
      else ymax = His->his->GetMaximum();
      yaxis = His->his->GetYaxis();
      xaxis = His->his->GetXaxis();

      count++;
    } else {
      ymin = ymin < His->his->GetMinimum() ? ymin : His->his->GetMinimum();
      ymax = ymax > His->his->GetMaximum() ? ymax : His->his->GetMaximum();

      //His->his->SetMaximum(10*ymax);
      //His->his->SetMinimum(0.1*ymin);
      TString overlay = "same"+His->drawopt;
      His->his->Draw(overlay);
    }

    //Set line or style
    if (His->line) 
    {
      His->his->SetLineWidth(3);
      His->his->SetLineColor(His->color);
      His->his->SetLineStyle(His->style);
      lg->AddEntry(His->his, His->tag.c_str(), "l");
    } else {
      His->his->SetMarkerSize(His->size);
      His->his->SetMarkerColor(His->color);
      His->his->SetLineColor(His->color);
      His->his->SetMarkerStyle(His->style);
      //his.th->SetMarkerStyle(his.style);
      lg->AddEntry(His->his, His->tag.c_str(), "lep");
    }
  }

  if (!LogY) yaxis->SetRangeUser(0.8*ymin, 1.2*ymax);
  else{
    ymin = ymin == 0 ? ymax/100000 : ymin;
    //yaxis->SetRangeUser(0.1*ymin, 5*ymax);
  }

  yaxis->SetTitleOffset(1.0);
  xaxis->SetTitleOffset(0.8);
  if (hname == "MetScale")
  {
    c1->SetGridx();
    c1->SetGridy();
  }
  if (LogX) c1->SetLogx();
  if (LogY) c1->SetLogy();
  lg->Draw();

  return lg;
}       // -----  end of function Compare::PlotComp  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::UserLegend
//  Description:  User function for defind legend location for different plot
//  name
// ===========================================================================
bool Compare::UserLegend() 
{
  //LgLoc["MetScale"] = {{0.1, 0.4, 0.8, 0.9}};
  double lt[4] = {0.1241611,0.6398305,0.3842282,0.8983051};
  double rb[4] = {0.6526846,0.1588983,0.9127517,0.4173729};
  double lb[4] = {0.1409396,0.1377119,0.3775168,0.4322034};

  LgLoc["MetScale"].assign(rb, rb+4);
  LgLoc["MetResP"].assign(lt, lt+4);
  LgLoc["MetResT"].assign(lt, lt+4);
  LgLoc["MetResX"].assign(lt, lt+4);
  LgLoc["MetResY"].assign(lt, lt+4);

  return true;

}       // -----  end of function Compare::UserLegend  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::FillVHis
//  Description:  
// ===========================================================================
bool Compare::FillVHis(std::vector<ComHis*> &VHis, std::string prefix, std::string Hname)
{
  BOOST_FOREACH(std::string tag, VTag)
  {
    ComHis *temp = InitComHisTH1(tag, prefix,  Hname);
    VHis.push_back(temp);
  }
  return true;
}       // -----  end of function Compare::FillVHis  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::FillMetResVHis
//  Description:  For Met resolution, we need to derive the function 
// ===========================================================================
bool Compare::FillMetResVHis(std::vector<ComHis*> &VHis, std::string prefix, std::string Hname)
{
  int cot = 1;
  BOOST_FOREACH(std::string tag, VTag)
  {
    ComHis *CH = new ComHis();
    TH1F* tempH = new TH1F();

    TProfile *pro = (TProfile*)TagFile[tag][prefix]->Get(Hname.c_str());
    tempH = (TH1F*)pro->ProjectionX();
    tempH->Clear();
    for (int i = 0; i <= pro->GetNbinsX(); ++i)
    {
      //std::cout << i << " bin Error " << pro->GetBinError(i) << std::endl;
      tempH->SetBinContent(i, pro->GetBinError(i));
      double error = pro->GetBinError(i) / std::sqrt(2 * pro->GetBinEntries(i));
      if (pro->GetBinError(i)!= 0)
      {

        tempH->SetBinError(i, error);
      }
    }
    CH->Initial(tempH, tag, VTag);

    CH->line = false;
    CH->scale = 1;
    CH->drawopt = "X0E1";
    CH->style = 20;
    if (cot == 3) cot++; // skip the green
    
    if (Hname == "MetResT") 
    {
      CH->ymin = 10;
      CH->ymax = 80;
    }

    CH->color = cot++;
    CH->size = 1;
    VHis.push_back(CH);
  }
  return true;
}       // -----  end of function Compare::FillMetResVHis  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::GetRMSDiff
//  Description:  
// ===========================================================================
bool Compare::GetRMSDiff(std::vector<ComHis*> VHis, std::string prefix)
{
  
  BOOST_FOREACH(ComHis* his, VHis)
  {
    MRMS[his->tag][prefix] = his->his->GetRMS();
  }
     return true;

}       // -----  end of function Compare::GetRMSDiff  -----

// ===  FUNCTION  ============================================================
//         Name:  Compare::CalRMSDiff
//  Description:  
// ===========================================================================
bool Compare::CalRMSDiff()
{
  std::map<std::string, std::string> PUPre;
  PUPre["0PU"] = "Jet_5_30_0PU";
  PUPre["50PU"] = "Jet_5_30_50PU";
  PUPre["140PU"] = "Jet_5_30_140PU";
  BOOST_FOREACH(std::string tag, VTag)
  {
    
    std::cout << tag << " |";
    std::cout << MRMS[tag][PUPre["0PU"]]<< " |";
    std::cout << MRMS[tag][PUPre["50PU"]]<< " |";
    std::cout << MRMS[tag][PUPre["140PU"]]<< " |";
    double rms = std::sqrt(MRMS[tag][PUPre["50PU"]]* MRMS[tag][PUPre["50PU"]]
        - MRMS[tag][PUPre["0PU"]]*MRMS[tag][PUPre["0PU"]] );
    std::cout << rms<< " |";
    rms = std::sqrt(MRMS[tag][PUPre["140PU"]]* MRMS[tag][PUPre["140PU"]]
        - MRMS[tag][PUPre["0PU"]]*MRMS[tag][PUPre["0PU"]] );
    std::cout << rms<< " |-";
    std::cout << std::endl;
  }
  return true;
  

}       // -----  end of function Compare::CalRMSDiff  -----
