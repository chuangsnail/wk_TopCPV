//#include "TopCPViolation/select/interface/RunObj.h"
#include "../interface/RunObj.h"

//*** Event part ***//

int Event::nBX() const 					{	return evt->nBX;		}
int Event::nPU(const int& i) const 		{	return evt->nPU[i];		}
int Event::BXPU(const int& i) const		{	return evt->BXPU[i];	}
int Event::TrueIT(const int& i) const 	{	return evt->TrueIT[i];	}

float Event::Rho() const				{	return evt->Rho;		}

double Event::MET() const {	return evt->PFMET;	}
double Event::METPhi() const {	return evt->PFMETPhi;	}

int Event::RunNo() const {  return evt->RunNo;  }
int Event::LumiNo() const {  return evt->LumiNo;  }

bool Event::IsGoodPVertex( const int& i ) const
{
	return ( !vtx->isFake[i]			&&
			 vtx->Ndof[i] > 4		&&
			 fabs( vtx->z[i] ) < 24 &&
			 vtx->Rho[i] < 2 );	
}

bool Event::GoodVertex() const {
	for(int i=0;i<vtx->Size;++i) {
		if( IsGoodPVertex(i) ) return true;
	}
	return false;
}

//*** Gen part ***//

double Gen::Pt(const int& idx) const{	return gen->Pt[idx];	}
double Gen::Eta(const int& idx) const{	return gen->Eta[idx];	}
double Gen::Phi(const int& idx) const{	return gen->Phi[idx];	}
double Gen::Weight() const {	return gen->Weight;		}


