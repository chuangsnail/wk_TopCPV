//#include "TopCPViolation/selected/interface/PhysObj.h"
#include "../interface/PhysObj.h"


//********************************  Lepton  *********************************//

Lepton::Lepton( LeptonInfo* l , EvtInfo* e )
{	RegIn(l, e);	}
void Lepton::RegIn( LeptonInfo* l , EvtInfo* e )
{	leps = l;	evt = e;	}

size_t Lepton::Size() const	{	return (size_t)leps->Size;	}

double Lepton::Pt(const int& idx) const{	return leps->Pt[idx];	}
double Lepton::Eta(const int& idx) const{	return leps->Eta[idx];	}
double Lepton::Phi(const int& idx) const{	return leps->Phi[idx];	}
double Lepton::Px(const int& idx) const{	return leps->Px[idx];	}
double Lepton::Py(const int& idx) const{	return leps->Py[idx];	}
double Lepton::Pz(const int& idx) const{	return leps->Pz[idx];	}
double Lepton::GenPt(const int& idx) const{		return leps->GenPt[idx];	}
double Lepton::GenEta(const int& idx) const{	return leps->GenEta[idx];	}
double Lepton::GenPhi(const int& idx) const{	return leps->GenPhi[idx];	}


double Lepton::Energy(const int& idx) const{	return leps->Energy[idx];	}
TLorentzVector Lepton::P4(const int& i) const
{
	TLorentzVector p;
	p.SetPxPyPzE( leps->Px[i], leps->Py[i], leps->Pz[i], leps->Energy[i] );
	return p;
}

//--- Muon Part ---//

double Lepton::RelISOR04(const int& i) const	//PFISO
{
	double rel = leps->ChargedHadronIsoR04[i] + max(float(0.), \
    leps->NeutralHadronIsoR04[i] + leps->PhotonIsoR04[i] - float(0.5) * leps->sumPUPtR04[i] );
    rel /= leps->Pt[i];
    return rel;
}

bool Lepton::SelMuISO( const int& idx ) const {	return RelISOR04(idx) < 0.15;	}
bool Lepton::VetoMuISO( const int& idx ) const {	return RelISOR04(idx) < 0.25;	}

bool Lepton::IsGlobalMu(const int& idx) const {	return (leps->MuType[idx]) & 0x02;	}
bool Lepton::IsTrackerMu(const int& idx) const {	return (leps->MuType[idx]) & 0x04;	}

bool Lepton::TightMuID( const int& i ) const
{
	return (
		IsGlobalMu( i )							&&
		leps->isPFMuon[i] 						&&
    	leps->MuGlobalNormalizedChi2[i] < 10. 	&&
    	leps->MuNMuonhits[i] > 0				&&
    	leps->MuNMatchedStations[i] > 1			&&
    	fabs(leps->MuInnerTrackDxy[i]) < 0.2	&&
    	fabs(leps->MuInnerTrackDz[i]) < 0.5		&&
    	leps->MuNPixelLayers[i] > 0				&&
    	leps->MuNTrackLayersWMeasurement[i] > 5 );
}

bool Lepton::LooseMuID( const int& i ) const
{	return ( IsGlobalMu(i) || IsTrackerMu(i) ) && leps->isPFMuon[i];	}

bool Lepton::SelMuKinematic( const int& i ) const
{	return ( Pt(i) > 30. ) && ( fabs(Eta(i)) < 2.4 ) ;	}
bool Lepton::VetoMuKinematic( const int& i ) const
{	return ( Pt(i) > 15. ) && ( fabs(Eta(i)) < 2.4 ) ;	}

bool Lepton::SR_SelMu(const int& i) const
{	return TightMuID(i) && SelMuISO(i) && SelMuKinematic(i);	}
bool Lepton::SR_VetoMu(const int& i) const
{	return LooseMuID(i) && VetoMuISO(i) && VetoMuKinematic(i);	}
bool Lepton::CR_SelMu(const int& i) const	//same as SR case
{	return TightMuID(i) && SelMuISO(i) && SelMuKinematic(i);	}
bool Lepton::CR_VetoMu(const int& i) const
{	return LooseMuID(i) && VetoMuKinematic(i);	}

bool Lepton::PrePreSelMu(const int& i) const
{	return TightMuID(i) && SelMuKinematic(i);	}

bool Lepton::CR_SelMu_invISO(const int& i) const
{	return TightMuID(i) && !SelMuISO(i) && SelMuKinematic(i);	}

//--- Electron Part ---//

double Lepton::ElISO( const int& i ) const	//PFISO
{	return leps->PFIsoRhoCorrR03[i] / Pt(i);	}
bool Lepton::TightElISO(const int& i) const
{
	if( fabs( Eta(i) ) <= 1.479 ){
	   return ElISO( i ) < ( 0.0287 + 0.506 / Pt(i) );
	}
	else{  //absEta > 1.479
		return ElISO( i ) < ( 0.0445 + 0.963 / Pt(i) );	
	}
}

bool Lepton::TightElID( const int& i ) const
{	return leps->EgammaCutBasedEleIdTIGHT[i];	}
bool Lepton::LooseElID( const int& i ) const
{	return leps->EgammaCutBasedEleIdLOOSE[i] ;	}

bool Lepton::TightElID_woISO( const int& i ) const
{
    const float ecal_energy_inverse = 1.0 / leps->ElEcalE[i];
    const float eSCoverP = leps->ElEoverP[i];
    float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP)*ecal_energy_inverse;
	if( fabs( leps->Eta[i] ) <= 1.479 )
	{	
		if(
			leps->SigmaIetaIeta[i] < 0.0104 	&&
			fabs( leps->EldeltaPhiSeed[i] ) < 0.00255 &&	//EldEtaInSeed
			fabs( leps->EldeltaPhi[i] ) < 0.022 		&&	//EldPhiIn
			leps->ElHadoverEm[i] < 0.026 + 1.15 / leps->EnergySC[i] + 0.0324 * evt->Rho / leps->EnergySC[i] &&		
			//ElGsfEleHadronicOverEMCut() < 0.026 + 1.15 / EnergySC() + 0.0324 * EvtRho() / EnergySC()
			fabs(GsfEleEInverseMinusPInverseCut) < 0.159 &&
			leps->NumberOfExpectedInnerHits[i] <= 1 &&
			!leps->ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
	else
	{
		if(
			leps->SigmaIetaIeta[i] < 0.0353 	&&
			fabs( leps->EldeltaPhiSeed[i] ) < 0.00501 &&	//EldEtaInSeed
			fabs( leps->EldeltaPhi[i] ) < 0.0236 		&&	//EldPhiIn
			leps->ElHadoverEm[i] < 0.0188 + 2.06 / leps->EnergySC[i] + 0.183 * evt->Rho / leps->EnergySC[i] &&		
			//ElGsfEleHadronicOverEMCut() < 0.0188 + 2.06 / EnergySC() + 0.183 * EvtRho() / EnergySC()
			fabs(GsfEleEInverseMinusPInverseCut) < 0.197 &&
			leps->NumberOfExpectedInnerHits[i] <= 1 &&
			!leps->ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
}

bool Lepton::LooseElID_woISO( const int& i ) const
{
    const float ecal_energy_inverse = 1.0 / leps->ElEcalE[i];
    const float eSCoverP = leps->ElEoverP[i];
    float GsfEleEInverseMinusPInverseCut = fabs(1.0 - eSCoverP)*ecal_energy_inverse;
	if( fabs( leps->Eta[i] ) <= 1.479 )
	{	
		if(
			leps->SigmaIetaIeta[i] < 0.0112 	&&
			fabs( leps->EldeltaPhiSeed[i] ) < 0.00377 &&	//EldEtaInSeed
			fabs( leps->EldeltaPhi[i] ) < 0.0884 		&&	//EldPhiIn
			leps->ElHadoverEm[i] < 0.005 + 1.16 / leps->EnergySC[i] + 0.0324 * evt->Rho / leps->EnergySC[i] &&		
			fabs(GsfEleEInverseMinusPInverseCut) < 0.193 &&
			leps->NumberOfExpectedInnerHits[i] <= 1 &&
			!leps->ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
	else
	{
		if(
			leps->SigmaIetaIeta[i] < 0.0425 	&&
			fabs( leps->EldeltaPhiSeed[i] ) < 0.00674 &&	//EldEtaInSeed
			fabs( leps->EldeltaPhi[i] ) < 0.169 		&&	//EldPhiIn
			leps->ElHadoverEm[i] < 0.0441 + 2.54 / leps->EnergySC[i] + 0.183 * evt->Rho / leps->EnergySC[i] &&		
			fabs(GsfEleEInverseMinusPInverseCut) < 0.111 &&
			leps->NumberOfExpectedInnerHits[i] <= 1 &&
			!leps->ElhasConv[i]
	  	)
		{	return true;	}
		else
		{	return false;	}
	}
}

bool Lepton::SelElKinematic( const int& i ) const
{	
	return ( Pt(i) > 38. && fabs(Eta(i)) < 2.1 && \
		!( fabs(Eta(i)) > 1.44 && fabs(Eta(i)) < 1.57 ) );
}

bool Lepton::VetoElKinematic( const int& i ) const
{	
	return ( Pt(i) > 15. && fabs(Eta(i)) < 2.4 && \
		!( fabs(Eta(i)) > 1.44 && fabs(Eta(i)) < 1.57 ) );
}

bool Lepton::Pass_ImpactParameter( const int& i ) const
{
	bool PassImpactParameter = true;
    if( fabs(Eta(i)) < 1.45 )
    {
        if( fabs(leps->ElTrackDxy[i]) > 0.05 )
        { PassImpactParameter = false; }
        if( fabs(leps->ElTrackDz[i])  > 0.10 )
        { PassImpactParameter = false; }
    }
    else
    {
        if( fabs(leps->ElTrackDxy[i]) > 0.10 )
        { PassImpactParameter = false; }
        if( fabs(leps->ElTrackDz[i]) > 0.20 )
        { PassImpactParameter = false; }
	}
	return PassImpactParameter;
}

bool Lepton::SR_SelEl(const int& i) const
{	return TightElID(i) && SelElKinematic(i) && Pass_ImpactParameter(i);	}
bool Lepton::SR_VetoEl(const int& i) const
{	return LooseElID(i) && VetoElKinematic(i);	}
bool Lepton::CR_SelEl(const int& i) const
{	return TightElID_woISO(i) && TightElISO(i) && SelElKinematic(i) && Pass_ImpactParameter(i);	}
bool Lepton::CR_VetoEl(const int& i) const
{	return LooseElID_woISO(i) && VetoElKinematic(i);	}

bool Lepton::PrePreSelEl(const int& i) const
{	return TightElID_woISO(i) && SelElKinematic(i) && Pass_ImpactParameter(i);	}

bool Lepton::CR_SelEl_invISO(const int& i) const
{	return TightElID_woISO(i) && !TightElISO(i) && SelElKinematic(i) && Pass_ImpactParameter(i);	}

//ElEnergyCor
void Lepton::ElEnergyCor()
{
	for(int i=0;i<(int)Size();++i)
	{
		if( leps->LeptonType[i] == 11 )
		{
			double CF = leps->ElEnergyCorrFactor[i];
			(leps->Px[i]) *= CF;
			(leps->Py[i]) *= CF;
			(leps->Pz[i]) *= CF;
			(leps->Pt[i]) *= CF;
			(leps->Energy[i]) *= CF;
		}
	}
}
