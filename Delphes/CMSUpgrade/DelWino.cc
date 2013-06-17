// ===========================================================================
// 
//       Filename:  DelWino.cc
// 
//    Description:  Class for the signal sample 
// 
//        Version:  1.0
//        Created:  06/17/2013 10:14:18 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelWino.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelWino
//      Method:  DelWino
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelWino::DelWino (double Eta, double Pt): DelEvent(Eta, Pt)
{
}  // ~~~~~  end of method DelWino::DelWino  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelWino
//      Method:  DelWino
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelWino::DelWino ( const DelWino &other ) : DelEvent(other)
{
}  // ~~~~~  end of method DelWino::DelWino  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelWino
//      Method:  ~DelWino
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelWino::~DelWino ()
{
}  // ~~~~~  end of method DelWino::~DelWino  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelWino
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelWino&
DelWino::operator = ( const DelWino &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelWino::operator =  (assignment operator)  ~~~



// ===  FUNCTION  ============================================================
//         Name:  DelWino::PreSelected
//  Description:  
// ===========================================================================
bool DelWino::PreSelected()
{
  return true;
}       // -----  end of function DelWino::PreSelected  -----
