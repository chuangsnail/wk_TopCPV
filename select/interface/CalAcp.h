#ifndef CALACP_H
#define CALACP_H
#include "frameDef.h"


class AcpMgr
{
private:
	LeptonInfo* leps;
	JetInfo* 	jets;
	
	TLorentzVector b;
	TLorentzVector bbar;
	TLorentzVector j1;
	TLorentzVector lep;

	double lep_charge;

	TVector3 _b;
	TVector3 _bbar;
	TVector3 _j1;
	TVector3 _lep;
	
	void SetLorentzVector( const int&, const string& );
	void ConvertToVect();
	void BoostAll( const TVector3& );



public:
	AcpMgr() {}
	AcpMgr( LeptonInfo*, JetInfo* ); 

	void RegIn( LeptonInfo*, JetInfo* );
	void InputSelObjs( const int& hadb, const int& lepb, const int& r, const int& s); //r==j1, s==lep
	void InputSelObjs_p4( const TLorentzVector& hadb, const TLorentzVector& lepb, const TLorentzVector& hardjet, const int& isolep );
	double Obs3();
	double Obs6();
	double Obs12();
	double Obs13();

};

void CalAcp( double* var, TH1D* p, TH1D* n );

#endif	//CALACP_H
