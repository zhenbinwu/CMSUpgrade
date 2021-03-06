//STARTHEADER
// $Id: LimitedWarning.cc 859 2012-11-28 01:49:23Z pavel $
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//ENDHEADER

#include "fastjet/LimitedWarning.hh"
#include <sstream>
#include <limits>

using namespace std;

FASTJET_BEGIN_NAMESPACE

ostream * LimitedWarning::_default_ostr = &cerr;
std::list< LimitedWarning::Summary > LimitedWarning::_global_warnings_summary;
int LimitedWarning::_max_warn_default = 5;


/// output a warning to ostr
void LimitedWarning::warn(const std::string & warning) {
  warn(warning, _default_ostr);
}

void LimitedWarning::warn(const std::string & warning, std::ostream * ostr) {
  if (_this_warning_summary == 0) {
    // prepare the information for the summary
    _global_warnings_summary.push_back(Summary(warning, 0));
    _this_warning_summary = & (_global_warnings_summary.back());
  }
  if (_n_warn_so_far < _max_warn) {
    // prepare the warning within a string stream
    ostringstream warnstr;
    warnstr << "WARNING: ";
    warnstr << warning;
    _n_warn_so_far++;
    if (_n_warn_so_far == _max_warn) warnstr << " (LAST SUCH WARNING)";
    warnstr << std::endl;
    // arrange for the whole warning to be output in one go (that way
    // user can easily insert their own printout, e.g. event number
    // before the warning string).
    if (ostr) {
      (*ostr) << warnstr.str();
      ostr->flush(); // get something written to file even if the program aborts
    }
  }

  // maintain the count, but do not allow overflow
  if (_this_warning_summary->second < numeric_limits<unsigned>::max()) {
    _this_warning_summary->second++;
  }
}

//----------------------------------------------------------------------
string LimitedWarning::summary() {
  ostringstream str;
  for (list<Summary>::const_iterator it = _global_warnings_summary.begin();
       it != _global_warnings_summary.end(); it++) {
    str << it->second << " times: " << it->first << endl;
  }
  return str.str();
}

FASTJET_END_NAMESPACE
