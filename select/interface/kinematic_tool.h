#ifndef KINEMATIC_TOOL_H
#define KINEMATIC_TOOL_H

#include <cmath>
#include <iostream>

//#include "TopCPViolation/select/interface/BranchInfo.h"
#include "BranchInfo.h"

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"

using namespace std;

TLorentzVector GetJetP4( JetInfo& jets, const int& idx );

TLorentzVector GetLepP4( LeptonInfo& leps, const int& idx );

double delta_R( const double& eta1, const double& eta2, const double& phi1,const double& phi2 );

#endif
