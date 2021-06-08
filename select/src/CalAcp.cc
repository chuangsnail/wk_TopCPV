#include "../interface/CalAcp.h"

AcpMgr::AcpMgr( LeptonInfo* l, JetInfo* j )
{
	RegIn( l, j );
	lep_charge = 0.;
}

void AcpMgr::RegIn( LeptonInfo* l , JetInfo* j )
{	leps = l;	jets = j;	}

void AcpMgr::InputSelObjs( const int& hadb, const int& lepb, const int& hardjet, const int& isolep )
{
	lep_charge = leps->Charge[ isolep ];
	if( lep_charge > 0 )
	{
		SetLorentzVector( lepb, "b" );
		SetLorentzVector( hadb, "bbar" );
	}
	else
	{
		SetLorentzVector( hadb, "b" );
		SetLorentzVector( lepb, "bbar" );
	}
	SetLorentzVector( hardjet, "j1" );
	SetLorentzVector( isolep, "lep" );
	ConvertToVect();
}

void
AcpMgr::Reset()
{
		
}

//used in change info method
void AcpMgr::InputSelObjs_p4( const TLorentzVector& hadb, const TLorentzVector& lepb, const TLorentzVector& hardjet, const int& isolep )
{	
	lep_charge = leps->Charge[ isolep ];
	if( lep_charge > 0 )
	{
		b = lepb;
		bbar = hadb;
	}
	else
	{
		b = hadb;
		bbar = lepb;
	}
	j1 = hardjet;
	SetLorentzVector( isolep, "lep" );
	ConvertToVect();

}

void AcpMgr::SetLorentzVector( const int& idx, const string& option = "jet" )
{
	if( option == "b" )
	{	b.SetPxPyPzE( jets->Px[idx], jets->Py[idx], jets->Pz[idx], jets->Energy[idx] );	}
	else if( option == "bbar" )
	{	bbar.SetPxPyPzE( jets->Px[idx], jets->Py[idx], jets->Pz[idx], jets->Energy[idx] );	}
	else if( option == "j1" )
	{	j1.SetPxPyPzE( jets->Px[idx], jets->Py[idx], jets->Pz[idx], jets->Energy[idx] );	}
	else if( option == "lep" )
	{	lep.SetPxPyPzE( leps->Px[idx], leps->Py[idx], leps->Pz[idx], leps->Energy[idx] );	}
	else
	{	cerr << "There is no option \'" << option << "\'" << endl;	}
}

void AcpMgr::ConvertToVect()
{
	_b = 		b.Vect();
	_bbar =		bbar.Vect();
	_j1 = 		j1.Vect();
	_lep = 		lep.Vect();
}

void AcpMgr::BoostAll( const TVector3& cm )
{
	b.Boost( cm );
	bbar.Boost( cm );
	j1.Boost( cm );
	lep.Boost( cm );
}

double AcpMgr::Obs3()
{
	TVector3 bbcm = -( b + bbar ).BoostVector();
	BoostAll( bbcm );
	
	double res = lep_charge * ( ( b.Vect() ).Dot( ( lep.Vect() ).Cross( j1.Vect() ) ) ) ;
	if( res == 0 ) return DBL_MAX;
	//return (res > 0) ? 1 : -1;
	return res;
}

double AcpMgr::Obs6()
{
	double res = lep_charge * ( ( _b - _bbar ).Dot( _lep.Cross( _j1 ) ) );
	if( res == 0 ) return DBL_MAX;
	//return (res > 0) ? 1 : -1;
	return res;
}

double AcpMgr::Obs12()
{
	double res = ( _b - _bbar ).Pz() * ( _b.Cross( _bbar ) ).Pz() ;
	if( res == 0 ) return DBL_MAX;
	//return (res > 0) ? 1 : -1;
	return res;
}

//actually it's Obs14
double AcpMgr::Obs13()
{
	//double res = lep_charge * ( ( _b + _bbar ).Dot( _lep.Cross( _j1 ) ) );
	double res = ( ( _b + _bbar ).Dot( _lep.Cross( _j1 ) ) );
	if( res == 0. ) return DBL_MAX;
	//return (res > 0) ? 1 : -1;
	return res;
}

TH1D* CalAcp( TH1D* p, TH1D* n )
{
	TH1D* num = new TH1D();
	num	= (TH1D*)( p->Clone() );
	//cout << "p : " << p << endl;
	//cout << "num : " << num << endl;
	TH1D* den = (TH1D*)( p->Clone() );
	num->Add( n, -1 );
	den->Add( n );
	num->Divide( den );

	return num;
}
