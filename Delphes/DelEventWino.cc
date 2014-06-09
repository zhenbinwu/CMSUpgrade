// ===========================================================================
// 
//       Filename:  DelEventWino.cc
// 
//    Description:  Class for the signal sample 
// 
//        Version:  1.0
//        Created:  06/17/2013 10:14:18 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CMS@FNAL
// 
// ===========================================================================

#include "DelEventWino.h"

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventWino
//      Method:  DelEventWino
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEventWino::DelEventWino (double Eta, double Pt): DelEvent(Eta, Pt)
{
}  // ~~~~~  end of method DelEventWino::DelEventWino  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventWino
//      Method:  DelEventWino
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEventWino::DelEventWino ( const DelEventWino &other ) : DelEvent(other)
{
}  // ~~~~~  end of method DelEventWino::DelEventWino  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventWino
//      Method:  ~DelEventWino
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelEventWino::~DelEventWino ()
{
}  // ~~~~~  end of method DelEventWino::~DelEventWino  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelEventWino
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelEventWino&
DelEventWino::operator = ( const DelEventWino &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelEventWino::operator =  (assignment operator)  ~~~



// ===  FUNCTION  ============================================================
//         Name:  DelEventWino::PreSelected
//  Description:  
// ===========================================================================
bool DelEventWino::PreSelected()
{
  return true;
}       // -----  end of function DelEventWino::PreSelected  -----
