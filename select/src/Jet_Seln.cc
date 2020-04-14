//#include "TopCPViolation/selected/interface/PhysObj.h"
#include "../interface/PhysObj.h"

//********************************  Jet  *********************************//

Jet::Jet( JetInfo* j )	{	RegIn(j);	} 

void Jet::RegIn( JetInfo* j )	{	jets = j;	}

size_t Jet::Size() const	{	return (size_t)jets->Size;	}

double Jet::Pt(const int& idx) const{	return jets->Pt[idx];	}
double Jet::Eta(const int& idx) const{	return jets->Eta[idx];	}
double Jet::Phi(const int& idx) const{	return jets->Phi[idx];	}
double Jet::Px(const int& idx) const{	return jets->Px[idx];	}
double Jet::Py(const int& idx) const{	return jets->Py[idx];	}
double Jet::Pz(const int& idx) const{	return jets->Pz[idx];	}
double Jet::Energy(const int& idx) const{	return jets->Energy[idx];	}
double Jet::GenPt(const int& idx) const{	return jets->GenPt[idx];	}
double Jet::GenEta(const int& idx) const{	return jets->GenEta[idx];	}
double Jet::GenPhi(const int& idx) const{	return jets->GenPhi[idx];	}
double Jet::GenJetPt(const int& idx) const{	return jets->GenJetPt[idx];	}
double Jet::GenJetEta(const int& idx) const{	return jets->GenJetEta[idx];	}
double Jet::GenJetPhi(const int& idx) const{	return jets->GenJetPhi[idx];	}

TLorentzVector Jet::P4(const int& i) const
{
	TLorentzVector p;
	p.SetPxPyPzE( jets->Px[i], jets->Py[i], jets->Pz[i], jets->Energy[i] );
	return p;
}

double Jet::DeepCSV( const int& idx ) const
{	return jets->pfDeepCSVJetTags_probb[idx] + jets->pfDeepCSVJetTags_probbb[idx];	}

bool Jet::TightDeepCSV( const int& idx ) const
{	return this->DeepCSV(idx) > 0.8953;	}

bool Jet::MediumDeepCSV( const int& idx ) const
{	return this->DeepCSV(idx) > 0.6321;	}

bool Jet::LooseDeepCSV( const int& idx ) const
{	return this->DeepCSV(idx) > 0.2217;	}

bool Jet::LooseJet( const int& idx ) const
{
	//Jet Loose ID
	if( jets->NHF[idx] <= 0.99 \
	&& jets->NEF[idx] <= 0.99 \
	&& jets->NConstituents[idx] > 1 \
	&& jets->CHF[idx] > 0. \
	&& jets->NCH[idx] > 0 \
	&& jets->CEF[idx] <= 0.99 \
	)
	{	return true;	}
	return false;
}

bool Jet::SelJet( const int& idx ) const
{
	return Pt(idx) > 30. \
	&& fabs( Eta(idx) ) < 2.4 \
	&& LooseJet( idx );
}

//// JER ////
bool Jet::Is_Well_Match( const int& idx, const double& res )
{
	double deta = Eta(idx) - GenJetEta(idx);
	double dphi = TVector2::Phi_mpi_pi( (double)( Phi(idx) - GenJetPhi(idx) ) );
	double delR = TMath::Sqrt( deta * deta + dphi * dphi );

	if( delR >= 0.4 / 2. ){
		return false;
	}	
	if( fabs( Pt(idx) - GenJetPt(idx) ) >= 3 * res * Pt(idx) ){
		return false;
	}

	return true;
}
double Jet::Scale_Case( const int& idx, const double& ressf )
{
	const double newpt = std::max( 0.0, GenJetPt(idx) + ressf * ( Pt(idx) - GenJetPt(idx) ) );
    const double scale = newpt / Pt(idx);

	return scale;
}

double Jet::Smeared_Case( const double& res, const double& ressf )
{
	const double width = ressf > 1 ? sqrt( ressf * ressf - 1 ) * res : 0;

	std::normal_distribution<> myrand( 0, width );
    std::uint32_t seed              = 37428479;
	std::mt19937 m_random_generator = std::mt19937( seed );

	double scale = 1. + myrand( m_random_generator );
	if( scale <= 0 || isnan( scale ) ){
		return 1;
	}
	else{
		return scale;
	}
}

double Jet::Get_JERScale( const int& i )
{
	double scale = 1.;

	double ressf = jets->JERScale[i];
	double res   = jets->JERPt[i];

	if( Is_Well_Match( i, res ) ){
		scale = Scale_Case( i, ressf );
	}
	else{
		scale = Smeared_Case( res, ressf );
	}
	return scale;
}

//it can change the jets' Pt/P4...(kinematic) //but Eta/Phi can't be change (just change energy not aspection)
void Jet::JERCor()
{
	for(int i=0;i<(int)Size();++i)
	{
		TLorentzVector tmp_jet;
		tmp_jet.SetPxPyPzE( jets->Px[i], jets->Py[i], jets->Pz[i], jets->Energy[i] );
		tmp_jet *= Get_JERScale( i );
		jets->Px[i] = tmp_jet.Px();
		jets->Py[i] = tmp_jet.Py();
		jets->Pz[i] = tmp_jet.Pz();
		jets->Pt[i] = tmp_jet.Pt();
		jets->Energy[i] = tmp_jet.Energy();
	}
}
