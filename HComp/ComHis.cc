// ===========================================================================
// 
//       Filename:  ComHis.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  06/19/2013 01:17:04 PM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "ComHis.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  ComHis
//      Method:  ComHis
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComHis::ComHis ()
{
}  // ~~~~~  end of constructor of template class ComHis  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  ComHis
//      Method:  ComHis
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComHis::ComHis ( const ComHis &other )
{
}  // ~~~~~  end of copy constructor of template class ComHis  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  ComHis
//      Method:  ~ComHis
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComHis::~ComHis ()
{
}  // ~~~~~  end of destructor of template class ComHis  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  ComHis
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ComHis& ComHis::operator = ( const ComHis &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of assignment operator of template class ComHis  ~~~~~


bool ComHis::Initial(TH1F* ahis, std::string atag, std::vector<std::string> aVTag)
{
  his    = ahis;
  tag    = atag;
  VTag   = aVTag;
  xlabel = "";
  ylabel = "";
  logx   = false;
  logy   = false;
  color  = 0;
  style  = 1;
  size   = 1;
  rebin  = 1;
  line   = true;
  drawopt = "";
  scale  = 1;
  xmin = -999.;
  xmax = -999.;
  ymin = -999.;
  ymax = -999.;


  return Setup();
}  // ~~~~~  end of constructor of template class ComHis  ~~~~~



// ===  FUNCTION  ============================================================
//         Name:  ComHis::SetAxis
//  Description:  
// ===========================================================================
bool ComHis::SetAxis() 
{
  std::string label = his->GetXaxis()->GetTitle();
  std::size_t found = xlabel.find("log_");
  if (found != std::string::npos)
  {
    label.replace(found, 4, "");
    xlabel = label;
    logx = true;
  } else {
    xlabel = label;
    logx = false;
  }

  label = his->GetYaxis()->GetTitle();
  found = label.find("log_");
  if (found != std::string::npos)
  {
    label.replace(found, 4, "");
    ylabel = label;
    logy = true;
  } else {
    ylabel = label;
    logy = false;
  }
  return true;
}       // -----  end of function ComHis::SetAxis  -----


// ===  FUNCTION  ============================================================
//         Name:  ComHis::SetStyle
//  Description:  
// ===========================================================================
bool ComHis::SetStyle()
{
  std::vector<std::string>::iterator tagit = std::find(VTag.begin(), VTag.end(), tag.c_str());
  //assert(tagit != VTag.end());
  int tagsize = VTag.size();
  int tagidx = tagit - VTag.begin();

  if (tagsize == 2 && tagit == VTag.end())
  {
    line = false;
    size = 1;
    style = 20;
    drawopt = "HISTP";
  }

  // No Green color as Dr. Ken suggested
  if (tagidx > 1)
    color = tagidx+2;
  else
    color = tagidx+1;

  return true;

}       // -----  end of function ComHis::SetStyle  -----

// ===  FUNCTION  ============================================================
//         Name:  ComHis::Setup
//  Description:  Dummy function to call all the setting function
// ===========================================================================
bool ComHis::Setup()
{
  SetAxis();
  SetStyle();

  return true;
}       // -----  end of function ComHis::Setup  -----

// ===  FUNCTION  ============================================================
//         Name:  ComHis::Print
//  Description:  Print out the current information of this plot
// ===========================================================================
bool ComHis::Print() const
{
  
  std::cout << "============================================================" << std::endl;
  std::cout << " Tag : "<< tag << " Name :" << his->GetName() << std::endl;
  std::cout << " XLabel : "<< xlabel << " Ylabel :" << ylabel << std::endl;
  std::cout << " LogX : "<< logx << " LogY :" << logy << std::endl;
  std::cout << " Line : "<< line << " color :" << color << " style: " << style << " size: "<< size  << std::endl;
  std::cout << " Rebin : " << rebin <<" Draw: " << drawopt << std::endl;
  std::cout << "============================================================" << std::endl;

  return true;

}       // -----  end of function ComHis::Print  -----
