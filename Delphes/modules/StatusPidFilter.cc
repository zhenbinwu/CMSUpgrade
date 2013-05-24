/** \class StatusPidFilter
 *
 *  Removes all generated particles except electrons, muons, taus,
 *  and particles with status == 3.
 *
 *  $Date: 2013-04-07 00:12:34 +0200 (Sun, 07 Apr 2013) $
 *  $Revision: 1079 $
 *
 *
 *  \author J. Hirschauer - FNAL
 *
 */

#include "modules/StatusPidFilter.h"

#include "classes/DelphesClasses.h"
#include "classes/DelphesFactory.h"
#include "classes/DelphesFormula.h"

#include "ExRootAnalysis/ExRootResult.h"
#include "ExRootAnalysis/ExRootFilter.h"
#include "ExRootAnalysis/ExRootClassifier.h"

#include "TMath.h"
#include "TString.h"
#include "TFormula.h"
#include "TRandom3.h"
#include "TObjArray.h"
#include "TDatabasePDG.h"
#include "TLorentzVector.h"

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

//------------------------------------------------------------------------------

StatusPidFilter::StatusPidFilter() :
  fItInputArray(0)
{
}

//------------------------------------------------------------------------------

StatusPidFilter::~StatusPidFilter()
{
}

//------------------------------------------------------------------------------

void StatusPidFilter::Init()
{
  // PT threshold

  fPTMin = GetDouble("PTMin", 0.5);

  // import input array
  fInputArray = ImportArray(GetString("InputArray", "Delphes/allParticles"));
  fItInputArray = fInputArray->MakeIterator();

  // create output array

  fOutputArray = ExportArray(GetString("OutputArray", "filteredParticles"));
}

//------------------------------------------------------------------------------

void StatusPidFilter::Finish()
{
  if(fItInputArray) delete fItInputArray;
}

//------------------------------------------------------------------------------

void StatusPidFilter::Process()
{
  Candidate *candidate;
  Int_t status, pdgCode;

  fItInputArray->Reset();
  while((candidate = static_cast<Candidate*>(fItInputArray->Next())))
  {
    status = candidate->Status;
    pdgCode = TMath::Abs(candidate->PID);

    // Write all electrons, muons, taus and status == 3;
//    if(pdgCode != 11 && pdgCode != 13 && pdgCode != 15 && status != 3) continue;
    if(candidate->Momentum.Pt() <= fPTMin) continue;
    if (status == 3 || pdgCode == 5 || pdgCode == 6 || pdgCode == 23 || pdgCode == 24 || pdgCode == 25 || pdgCode == 11 || pdgCode == 12 || pdgCode == 13 || pdgCode == 14 || pdgCode == 15 || pdgCode == 16 || pdgCode == 32 || pdgCode == 37) fOutputArray->Add(candidate);
  }
}

