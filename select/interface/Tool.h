#ifndef TOOL_H
#define TOOL_H

#include "frameDef.h"

namespace CalTool
{
	inline double delR(const double& eta1, const double& eta2, const double& phi1, const double& phi2)
	{
		double delta_eta = eta1 - eta2;
		double delta_phi = TVector2::Phi_mpi_pi(phi1 - phi2);		//TLorentzVector.h
		double del_R = sqrt( delta_eta * delta_eta + delta_phi * delta_phi );
		return del_R;
	}

	//to change the P4 of the original branch value and storethe updated version of P4
	inline void ScaleP4( double& SF, const string& option = "Jet" )
	{
		if( option == "Jet" )
		{
		}
		else if( option == "Lep" )
		{
		}
	}

	inline double Integral_All( TH1* h )
	{
		return h->Integral( 0, h->GetNbinsX()+1 );
	}

		
}

#endif //TOOL_H
