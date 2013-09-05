// ===========================================================================
// 
//       Filename:  DelHTB.cc
// 
//    Description:  
// 
//        Version:  1.0
//        Created:  07/11/2013 12:18:08 AM
//       Revision:  none
//       Compiler:  g++
// 
//         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
//        Company:  Baylor University, CDF@FNAL
// 
// ===========================================================================

#include "DelHTB.h"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  DelHTB
// Description:  constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelHTB::DelHTB (double Eta, double Pt) : DelZJJ(false, Eta, Pt)
{
  Boson = -99;
  WMode = -99;
  ZMode = -99;
}  // ~~~~~  end of method DelHTB::DelHTB  (constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  DelHTB
// Description:  copy constructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelHTB::DelHTB ( const DelHTB &other ) : DelZJJ(other)
{
}  // ~~~~~  end of method DelHTB::DelHTB  (copy constructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  ~DelHTB
// Description:  destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
DelHTB::~DelHTB ()
{
}  // ~~~~~  end of method DelHTB::~DelHTB  (destructor)  ~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//       Class:  DelHTB
//      Method:  operator =
// Description:  assignment operator
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  DelHTB&
DelHTB::operator = ( const DelHTB &other )
{
  if ( this != &other ) {
  }
  return *this;
}  // ~~~~~  end of method DelHTB::operator =  (assignment operator)  ~~~


// ===  FUNCTION  ============================================================
//         Name:  DelHTB::CheckFlag
//  Description:  
// ===========================================================================
bool DelHTB::CheckFlag(const std::string flag)
{

  assert(!FakingZvv);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Separating boson production ~~~~~
  if (flag == "H")      return Boson == 1;
  if (flag == "W")      return Boson == 2;
  if (flag == "Z")      return Boson == 3;
  if (flag == "Photon") return Boson == 4;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Separating W decay mode ~~~~~
  if (flag == "Wev" || flag == "SysWev") return WMode == 1;
  if (flag == "Wmv" || flag == "SysWmv")  return WMode == 2;
  if (flag == "Wtv")  return WMode == 3;
  if (flag == "Wlv")  return WMode >  0;
  if (flag == "Whad") return WMode == 0;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Separating Z decay mode ~~~~~
  if (flag == "Zee" || flag == "SysZee") return ZMode == 1;
  if (flag == "Zmm" || flag == "SysZmm") return ZMode == 2;
  if (flag == "Ztt") return ZMode == 3;
  if (flag == "Zll") return ZMode > 0;
  if (flag == "Zvv") return ZMode == 0;
  if (flag == "Zhad") return ZMode == -1;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ For MET study ~~~~~
  if (flag == "MetDiMuon") 
  {
    if ( Boson != 3) return false;
    else return DiMuonMet();
  }
  if (flag == "MetDiEle") 
  {
    if ( Boson != 3) return false;
    else return DiEleMet();
  }

  return false;
}       // -----  end of function DelHTB::CheckFlag  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::GenBoson
//  Description:  Function to check the genparticles for boson
//                Higgs  -- 1
//                W      -- 2
//                Z      -- 3
//                Photon -- 4
//  
// ===========================================================================
int DelHTB::GenBoson() const
{
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (std::fabs(p.PID) == 25) return 1;    // Higgs
    if (std::fabs(p.PID) == 24) return 2;    // W
    if (std::fabs(p.PID) == 23) return 3;    // Z
    if (std::fabs(p.PID) == 22) return 4;    // photon
  }
  // if can't find W, Z, H and photon
  return 0;
}       // -----  end of function DelHTB::GenBoson  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::LoadEvent
//  Description:  
// ===========================================================================
bool DelHTB::LoadEvent(TClonesArray *branchEvent, TClonesArray *branchJet, 
        TClonesArray *branchGenJet,  TClonesArray *branchCAJet, 
        TClonesArray *branchElectron, TClonesArray *branchMuon, 
        TClonesArray *branchPhoton, TClonesArray *branchMet, 
        TClonesArray *branchHt, TClonesArray *branchParticle)
{
  
  Boson = WMode = ZMode = -99;
  DelEvent::LoadEvent(branchEvent,  branchJet, 
      branchGenJet,   branchCAJet, branchElectron,  branchMuon, 
      branchPhoton,  branchMet, branchHt,  branchParticle);
  Boson = GenBoson();
  WMode = CheckWMode();
  ZMode = CheckZMode();

  if (false)
  {

    std::cout << "All Gen jets without cut -------------------------------------------" << std::endl;
    for (int i = 0; i < branchGenJet->GetEntries(); ++i)
    {
      // Now, get the jet at index
      Jet* p = (Jet*)branchGenJet->At(i);
      std::cout
        << std::setw(5) << " i:  "   << std::setw(3) << i
        << std::setw(5) << " Px "    << std::setw(8) << p->P4().Px()
        << std::setw(5) << " Py "    << std::setw(8) << p->P4().Py()
        << std::setw(5) << " Pz "    << std::setw(8) << p->P4().Pz()
        << std::setw(5) << " Mass  " << std::setw(8) << p->Mass
        << std::setw(5) << " PT  "   << std::setw(8) << p->PT
        << std::setw(5) << " Phi "   << std::setw(8) << p->Phi
        << std::setw(5) << " Eta "   << std::setw(8) << p->Eta
        << std::endl;

    }

    std::cout << "All antiKT jets without cut -------------------------------------------" << std::endl;
    for (int i = 0; i < branchJet->GetEntries(); ++i)
    {
      // Now, get the jet at index
      Jet* p = (Jet*)branchJet->At(i);
      std::cout
        << std::setw(5) << " i:  "   << std::setw(3) << i
        << std::setw(5) << " Px "    << std::setw(8) << p->P4().Px()
        << std::setw(5) << " Py "    << std::setw(8) << p->P4().Py()
        << std::setw(5) << " Pz "    << std::setw(8) << p->P4().Pz()
        << std::setw(5) << " Mass  " << std::setw(8) << p->Mass
        << std::setw(5) << " PT  "   << std::setw(8) << p->PT
        << std::setw(5) << " Phi "   << std::setw(8) << p->Phi
        << std::setw(5) << " Eta "   << std::setw(8) << p->Eta
        << std::endl;

    }
    std::cout << "All CA jets without cut -------------------------------------------" << std::endl;
    for (int i = 0; i < branchCAJet->GetEntries(); ++i)
    {
      // Now, get the jet at index
      Jet* p = (Jet*)branchCAJet->At(i);
      std::cout
        << std::setw(5) << " i:  "   << std::setw(3) << i
        << std::setw(5) << " Px "    << std::setw(8) << p->P4().Px()
        << std::setw(5) << " Py "    << std::setw(8) << p->P4().Py()
        << std::setw(5) << " Pz "    << std::setw(8) << p->P4().Pz()
        << std::setw(5) << " Mass  " << std::setw(8) << p->Mass
        << std::setw(5) << " PT  "   << std::setw(8) << p->PT
        << std::setw(5) << " Phi "   << std::setw(8) << p->Phi
        << std::setw(5) << " Eta "   << std::setw(8) << p->Eta
        << std::endl;

    }
    std::cout << "All CA jets without cut -------------------------------------------" << std::endl;
    for (int i = 0; i < branchCAJet->GetEntries(); ++i)
    {
      // Now, get the jet at index
      Jet* p = (Jet*)branchCAJet->At(i);
      std::cout
        << std::setw(5) << " i:  "   << std::setw(3) << i
        << std::setw(5) << " Px "    << std::setw(8) << p->P4().Px()
        << std::setw(5) << " Py "    << std::setw(8) << p->P4().Py()
        << std::setw(5) << " Pz "    << std::setw(8) << p->P4().Pz()
        << std::setw(5) << " Mass  " << std::setw(8) << p->Mass
        << std::setw(5) << " PT  "   << std::setw(8) << p->PT
        << std::setw(5) << " Phi "   << std::setw(8) << p->Phi
        << std::setw(5) << " Eta "   << std::setw(8) << p->Eta
        << std::endl;

    }
    std::cout << "All jets without cut -------------------------------------------" << std::endl;
  }
  return true;

}       // -----  end of function DelHTB::LoadEvent  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::PrintGen
//  Description:  Print out the GenParticle list
// ===========================================================================
bool DelHTB::PrintGen() const
{
  std::cout << "===========================================" << std::endl;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    std::cout << " i " << i 
      << std::setw(5) << " PID "    << std::setw(3) << p.PID
      << std::setw(5) << " Status "    << std::setw(3) << p.Status
      << std::setw(5) << " M1 "    << std::setw(3) << p.M1
      << std::setw(5) << " M2 "    << std::setw(3) << p.M2
      << std::setw(5) << " Px "     << std::setw(8) << p.Px
      << std::setw(5) << " Py "     << std::setw(8) << p.Py
      << std::setw(5) << " Pz "     << std::setw(8) << p.Pz
      << std::setw(5) << " E   "    << std::setw(8) << p.E
      << std::setw(5) << " PT  "    << std::setw(8) << p.PT
      << std::setw(5) << " Phi "    << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "    << std::setw(8) << p.Eta
      << std::endl;
  }
}       // -----  end of function DelHTB::PrintGen  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::CheckPhoton
//  Description:  
// ===========================================================================
bool DelHTB::CheckPhoton(bool IsPhoton) const
{
  if (!IsPhoton) return false;
  
  std::cout << "===========================================" << std::endl;
  PrintGen();
  std::cout <<  " Printing Gen Photon: " << std::endl;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (std::fabs(p.PID) == 22)
    {
      std::cout 
        << std::setw(5) << " PID "    << std::setw(3) << p.PID
        << std::setw(5) << " Px "     << std::setw(8) << p.Px
        << std::setw(5) << " Py "     << std::setw(8) << p.Py
        << std::setw(5) << " Pz "     << std::setw(8) << p.Pz
        << std::setw(5) << " E   "    << std::setw(8) << p.E
        << std::setw(5) << " PT  "    << std::setw(8) << p.PT
        << std::setw(5) << " Phi "    << std::setw(8) << p.Phi
        << std::setw(5) << " Eta "    << std::setw(8) << p.Eta
        << std::endl;
    }
  }


  std::cout << " Raw Met " << vMissingET.at(0).MET
    <<" MHT "<< PUCorMet.Mod()<< std::endl;
  std::cout << " Object size : jets " << vJet.size() << " ele " << vElectron.size()
    << " muon "<< vMuon.size() << " photon" << vPhoton.size() << std::endl;
  std::cout <<  " Printing Reconstructed Photon: " << std::endl;
  TLorentzVector MPhoton(0., 0., 0., 0.);
  for (int i = 0; i < vPhoton.size(); ++i)
  {
    Photon p = vPhoton.at(i);
    MPhoton += p.P4();
      std::cout 
        << std::setw(5) << " i:  "  << std::setw(3) << i
        << std::setw(5) << " Px "  << std::setw(8) << p.P4().Px()
        << std::setw(5) << " Py "  << std::setw(8) << p.P4().Py()
        << std::setw(5) << " Pz "  << std::setw(8) << p.P4().Pz()
        << std::setw(5) << " E  "  << std::setw(8) << p.E
        << std::setw(5) << " PT  "  << std::setw(8) << p.PT
        << std::setw(5) << " Phi "  << std::setw(8) << p.Phi
        << std::setw(5) << " Eta "  << std::setw(8) << p.Eta
        << std::endl;
  }


  std::cout <<  " Printing Reconstructed Jet: " << std::endl;

  TLorentzVector MJET(0., 0., 0., 0.);
  for (int i = 0; i < vJet.size(); ++i)
  {
    Jet p = vJet.at(i);
    MJET += p.P4();
    std::cout
      << std::setw(5) << " i:  "   << std::setw(3) << i
      << std::setw(5) << " Px "    << std::setw(8) << p.P4().Px()
      << std::setw(5) << " Py "    << std::setw(8) << p.P4().Py()
      << std::setw(5) << " Pz "    << std::setw(8) << p.P4().Pz()
      << std::setw(5) << " Mass  " << std::setw(8) << p.Mass
      << std::setw(5) << " PT  "   << std::setw(8) << p.PT
      << std::setw(5) << " Phi "   << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "   << std::setw(8) << p.Eta
      << std::endl;
  }

  std::cout <<  " MHT from Jet: " << std::endl;
  std::cout
     << std::setw(8) << " MJET: "
     << std::setw(5) << " Px "    << std::setw(8) << MJET.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MJET.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MJET.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MJET.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MJET.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MJET.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MJET.Eta()
    << std::endl;

  std::cout <<  " Printing Reconstructed GenJet: " << std::endl;

  TLorentzVector MGenJet(0., 0., 0., 0.);
  for (int i = 0; i < vGenJet.size(); ++i)
  {
    Jet p = vGenJet.at(i);
    MGenJet += p.P4();
    std::cout
      << std::setw(5) << " i:  "   << std::setw(3) << i
      << std::setw(5) << " Px "    << std::setw(8) << p.P4().Px()
      << std::setw(5) << " Py "    << std::setw(8) << p.P4().Py()
      << std::setw(5) << " Pz "    << std::setw(8) << p.P4().Pz()
      << std::setw(5) << " Mass  " << std::setw(8) << p.Mass
      << std::setw(5) << " PT  "   << std::setw(8) << p.PT
      << std::setw(5) << " Phi "   << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "   << std::setw(8) << p.Eta
      << std::endl;
  }

  std::cout <<  " MHT from GenJet: " << std::endl;
  std::cout
     << std::setw(8) << " MGenJet: "
     << std::setw(5) << " Px "    << std::setw(8) << MGenJet.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MGenJet.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MGenJet.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MGenJet.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MGenJet.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MGenJet.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MGenJet.Eta()
    << std::endl;

  std::cout <<  " MHT from Photon: " << std::endl;
  std::cout
     << std::setw(8) << " MPho: "
     << std::setw(5) << " Px "    << std::setw(8) << MPhoton.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MPhoton.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MPhoton.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MPhoton.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MPhoton.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MPhoton.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MPhoton.Eta()
    << std::endl;

  std::cout <<  " Between MPhoton and MJET :" << std::endl;
  std::cout
     << std::setw(8) << " Pho - Jet: "
     << std::setw(5) << " Px "    << std::setw(8) << MPhoton.Px()  - MJET.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MPhoton.Py()  - MJET.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MPhoton.Pz()  - MJET.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MPhoton.E()   - MJET.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MPhoton.Pt()  - MJET.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MPhoton.Phi() - MJET.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MPhoton.Eta() - MJET.Eta()
    << std::endl;

  std::cout << "" << std::endl;

}       // -----  end of function DelHTB::CheckPhoton  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::CheckBoson
//  Description:  
// ===========================================================================
bool DelHTB::CheckBoson(int boson) const
{
  std::cout << "===========================================" << std::endl;
  PrintGen();
  std::cout <<  " Printing Gen Boson: " << std::endl;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (std::fabs(p.PID) == boson)
    {
      std::cout 
        << std::setw(5) << " PID "    << std::setw(3) << p.PID
        << std::setw(5) << " Px "     << std::setw(8) << p.Px
        << std::setw(5) << " Py "     << std::setw(8) << p.Py
        << std::setw(5) << " Pz "     << std::setw(8) << p.Pz
        << std::setw(5) << " E   "    << std::setw(8) << p.E
        << std::setw(5) << " PT  "    << std::setw(8) << p.PT
        << std::setw(5) << " Phi "    << std::setw(8) << p.Phi
        << std::setw(5) << " Eta "    << std::setw(8) << p.Eta
        << std::endl;
    }
  }


  std::cout << " Raw Met " << vMissingET.at(0).MET
    <<" MHT "<< PUCorMet.Mod()<< std::endl;
  std::cout << " Object size : jets " << vJet.size() << " ele " << vElectron.size()
    << " muon "<< vMuon.size() << " photon" << vPhoton.size() << std::endl;

  std::cout <<  " Printing Reconstructed Jet: " << std::endl;

  TLorentzVector MJET(0., 0., 0., 0.);
  for (int i = 0; i < vJet.size(); ++i)
  {
    Jet p = vJet.at(i);
    MJET += p.P4();
    std::cout
      << std::setw(5) << " i:  "   << std::setw(3) << i
      << std::setw(5) << " Px "    << std::setw(8) << p.P4().Px()
      << std::setw(5) << " Py "    << std::setw(8) << p.P4().Py()
      << std::setw(5) << " Pz "    << std::setw(8) << p.P4().Pz()
      << std::setw(5) << " Mass  " << std::setw(8) << p.Mass
      << std::setw(5) << " PT  "   << std::setw(8) << p.PT
      << std::setw(5) << " Phi "   << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "   << std::setw(8) << p.Eta
      << std::endl;
  }

  std::cout <<  " MHT from Jet: " << std::endl;
  std::cout
     << std::setw(8) << " MJET: "
     << std::setw(5) << " Px "    << std::setw(8) << MJET.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MJET.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MJET.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MJET.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MJET.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MJET.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MJET.Eta()
    << std::endl;

  std::cout <<  " Printing Reconstructed GenJet: " << std::endl;

  TLorentzVector MGenJet(0., 0., 0., 0.);
  for (int i = 0; i < vGenJet.size(); ++i)
  {
    Jet p = vGenJet.at(i);
    MGenJet += p.P4();
    std::cout
      << std::setw(5) << " i:  "   << std::setw(3) << i
      << std::setw(5) << " Px "    << std::setw(8) << p.P4().Px()
      << std::setw(5) << " Py "    << std::setw(8) << p.P4().Py()
      << std::setw(5) << " Pz "    << std::setw(8) << p.P4().Pz()
      << std::setw(5) << " Mass  " << std::setw(8) << p.Mass
      << std::setw(5) << " PT  "   << std::setw(8) << p.PT
      << std::setw(5) << " Phi "   << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "   << std::setw(8) << p.Eta
      << std::endl;
  }

  std::cout <<  " MHT from GenJet: " << std::endl;
  std::cout
     << std::setw(8) << " MGenJet: "
     << std::setw(5) << " Px "    << std::setw(8) << MGenJet.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MGenJet.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MGenJet.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MGenJet.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MGenJet.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MGenJet.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MGenJet.Eta()
    << std::endl;

  std::cout << "" << std::endl;

  std::cout <<  " Printing Reconstructed CAJet: " << std::endl;

  TLorentzVector MCAJet(0., 0., 0., 0.);
  for (int i = 0; i < vCAJet.size(); ++i)
  {
    Jet p = vCAJet.at(i);
    MCAJet += p.P4();
    std::cout
      << std::setw(5) << " i:  "   << std::setw(3) << i
      << std::setw(5) << " Px "    << std::setw(8) << p.P4().Px()
      << std::setw(5) << " Py "    << std::setw(8) << p.P4().Py()
      << std::setw(5) << " Pz "    << std::setw(8) << p.P4().Pz()
      << std::setw(5) << " Mass  " << std::setw(8) << p.Mass
      << std::setw(5) << " PT  "   << std::setw(8) << p.PT
      << std::setw(5) << " Phi "   << std::setw(8) << p.Phi
      << std::setw(5) << " Eta "   << std::setw(8) << p.Eta
      << std::endl;
  }

  std::cout <<  " MHT from CAJet: " << std::endl;
  std::cout
     << std::setw(8) << " MCAJet: "
     << std::setw(5) << " Px "    << std::setw(8) << MCAJet.Px()
     << std::setw(5) << " Py "    << std::setw(8) << MCAJet.Py()
     << std::setw(5) << " Pz "    << std::setw(8) << MCAJet.Pz()
     << std::setw(5) << " E  "    << std::setw(8) << MCAJet.E()
     << std::setw(5) << " PT  "   << std::setw(8) << MCAJet.Pt()
     << std::setw(5) << " Phi "   << std::setw(8) << MCAJet.Phi()
     << std::setw(5) << " Eta "   << std::setw(8) << MCAJet.Eta()
    << std::endl;

  std::cout << "" << std::endl;
}       // -----  end of function DelHTB::CheckBoson  -----


// ===  FUNCTION  ============================================================
//         Name:  DelHTB::CheckWMode
//  Description:  Separate out the W decay mode
//  W->ev --- 1
//  W->mv --- 2
//  W->tv --- 3
//  W->jj --- 0
// ===========================================================================
int DelHTB::CheckWMode() const
{
  // Find whether it is a W event
  int Widx = -1;
  for (int i = 0; i < vGenParticle.size(); ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (std::fabs(p.PID) == 24) 
    {
      Widx = i;  //Find W 
      break;
    }
  }
  if (Widx == -1) return -99;

  // Now find the W decay products
  // Assuming the status 3 electron directly from W
  // Delphes has broken mother links, can't track back to mother W
  int GenSize = vGenParticle.size();
  for (int i = 0; i < GenSize; ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (p.Status != 3 || p.M1 > GenSize || p.M2 > GenSize )  continue;
    if (std::fabs(p.PID) == 11) return 1;
    if (std::fabs(p.PID) == 13) return 2;
    if (std::fabs(p.PID) == 15) return 3;
  }

  //In the case without leptons, it should be hadronic W decay
  return 0;
}       // -----  end of function DelHTB::CheckWMode  -----

// ===  FUNCTION  ============================================================
//         Name:  DelHTB::CheckZMode
//  Description:  Separate out the Z decay mode
//  Z-> vv --- 0
//  Z-> ee --- 1
//  Z-> mm --- 2
//  Z-> tt --- 3
//  Z-> jj --- -1
// ===========================================================================
int DelHTB::CheckZMode() const
{
  // Find whether it is a Z event
  int Zidx = -1;
  int GenSize = vGenParticle.size();
  for (int i = 0; i < GenSize; ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (std::fabs(p.PID) == 23) 
    {
      Zidx = i;  //Find Z 
      break;
    }
  }

  if (Zidx == -1) return -99;

  // Now find the Z decay products
  // Assuming the status 3 electron directly from Z
  // Delphes has broken mother links, can't track back to mother Z
  std::vector<int> vLep;
  std::vector<int> vNv;
  for (int i = 0; i < GenSize; ++i)
  {
    GenParticle p = vGenParticle.at(i);
    if (p.Status != 3 || p.M1 > GenSize || p.M2 > GenSize )  continue;
    if (std::fabs(p.PID) == 11 || std::fabs(p.PID) == 13 ||
        std::fabs(p.PID) == 15 )
    vLep.push_back(p.PID);
    if (std::fabs(p.PID) == 12 || std::fabs(p.PID) == 14 ||
        std::fabs(p.PID) == 16 )
    vNv.push_back(p.PID);
  }
  
   
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Z->hadronic ~~~~~
  if (vLep.size() == 0 && vNv.size() == 0) return -1;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Z->ll ~~~~~
  if (vLep.size() == 2)
  {
    assert(vLep.at(0) == -vLep.at(1));
    if (std::fabs(vLep.at(0)) == 11) return 1;
    if (std::fabs(vLep.at(0)) == 13) return 2;
    if (std::fabs(vLep.at(0)) == 15) return 3;
  } 
  else if (vNv.size() == 2) return 0;

  return -99;
}       // -----  end of function DelHTB::CheckZMode  -----
