/*
 * ===========================================================================
 *
 *       Filename:  DelAna.h
 *
 *    Description:  Separate out the class for Analysis purpose. This should
 *    be a common interface for Delphes and CMSSW 
 *
 *        Version:  1.0
 *        Created:  06/10/2013 10:03:49 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Zhenbin Wu (benwu), benwu@fnal.gov
 *        Company:  Baylor University, CDF@FNAL
 *
 * ===========================================================================
 */
#ifndef  __DELANA_INC__
#define  __DELANA_INC__


#include <assert.h>
#include "DelEvent.h"

/*
 * ===========================================================================
 *        Class:  DelAna
 *  Description:  
 * ===========================================================================
 */
class DelEvent;

class DelAna
{
  public:

    /* ====================  LIFECYCLE     =============================== */
    DelAna (DelEvent *DEV);        /* constructor      */
    DelAna (const DelAna &other ); /* copy constructor */
    ~DelAna ();                    /* destructor       */

    /* ====================  ACCESSORS     ======================================= */
    bool CheckFlag(std::string name);
    bool Clear();
    int GetBasic();
    int MetDiLepton();
    int GenLeps() const;
    bool FromWTop(GenParticle p) const;

    /* ====================  MUTATORS      ======================================= */

    /* ====================  OPERATORS     ======================================= */

    DelAna& operator = ( const DelAna &other ); /* assignment operator */

    // To store the event information
    std::vector<Event> *vEvent;
    std::vector<GenParticle> *vGenParticle;
    std::vector<Photon> *vPhoton;
    std::vector<Electron> *vElectron;
    std::vector<Muon> *vMuon;
    std::vector<Jet> *vJet;
    std::vector<MissingET> *vMissingET;
    TVector2 *PUCorMet;
    TLorentzVector *MHT;
    double *HT;

    // Common variable
    Jet *J1;
    Jet *J2;
    Jet *J3;
    double Mjj;
    double Met;

    // For Met Study
    double Mll;
    double UParallel;
    double UTransverse;
    double QT;
    double MetScale;

    /* ====================  DATA MEMBERS  ======================================= */
    DelEvent* DEV;
  protected:
    /* ====================  DATA MEMBERS  ======================================= */

  private:
    std::string CurrentTag;

}; /* -----  end of class DelAna  ----- */

#endif   /* ----- #ifndef  __DELANA_INC__  ----- */
