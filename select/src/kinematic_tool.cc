
#include "TopCPViolation/select/interface/kinematic_tool.h"

TLorentzVector GetJetP4( JetInfo& jets, const int& idx )
{
	TLorentzVector v;
	v.SetPxPyPzE( jets.Px[idx], jets.Py[idx], jets.Pz[idx], jets.Energy[idx] );
	return v;

}	

TLorentzVector GetLepP4( LeptonInfo& leps, const int& idx )
{
	TLorentzVector v;
	v.SetPxPyPzE( leps.Px[idx], leps.Py[idx], leps.Pz[idx], leps.Energy[idx] );
	return v;
}

double delta_R( const double& eta1, const double& eta2, const double& phi1,const double& phi2 )
{
	double delta_eta = eta1 - eta2;
	double delta_phi = TVector2::Phi_mpi_pi(phi1 - phi2);		//TLorentzVector.h
	double del_R = sqrt( pow(delta_eta,2) + pow(delta_phi,2) );
	return del_R;
}

