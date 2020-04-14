#ifndef PHYSOBJ_H
#define PHYSOBJ_H

#include "TopCPViolation/select/interface/frameDef.h"
//#include "frameDef.h"

using namespace std;

class Jet
{
	friend class SelMgr;
private:
	JetInfo* jets;
public:
	Jet(){}
	//Register in
	Jet( JetInfo* );	
	void RegIn( JetInfo* );

	size_t Size() const;

	//kinematic
	double Pt(const int&) const;
	double Eta(const int&) const;
	double Phi(const int&) const;
	double Px(const int&) const;
	double Py(const int&) const;
	double Pz(const int&) const;
	double Energy(const int&) const;

	TLorentzVector P4(const int&) const;

	//GenInfo
	double GenPt(const int&) const;
	double GenEta(const int&) const;
	double GenPhi(const int&) const;
	double GenJetPt(const int&) const;
	double GenJetEta(const int&) const;
	double GenJetPhi(const int&) const;

	double DeepCSV( const int& ) const;
	bool TightDeepCSV( const int& ) const;
	bool MediumDeepCSV( const int& ) const;
	bool LooseDeepCSV( const int& ) const;

	//for selection
	bool LooseJet( const int& ) const;
	bool SelJet( const int& ) const;

	//JER
	bool Is_Well_Match( const int& idx, const double& res );
	double Scale_Case( const int& idx, const double& ressf );
	double Smeared_Case( const double& res, const double& ressf );
	double Get_JERScale( const int& i );
	void JERCor();

};

class Lepton
{
	friend class SelMgr;
private:
	LeptonInfo* leps;
	EvtInfo* evt;
	////tools
		//for Mu
	double RelISOR04(const int&) const;
		//for El
	double ElISO( const int& ) const;
	bool Pass_ImpactParameter( const int& ) const;

public:
	Lepton() {}
	Lepton( LeptonInfo* , EvtInfo* );
	void RegIn( LeptonInfo* , EvtInfo* );

	size_t Size() const;

	//kinematic
	double Pt(const int&) const;
	double Eta(const int&) const;
	double Phi(const int&) const;
	double Px(const int&) const;
	double Py(const int&) const;
	double Pz(const int&) const;
	double Energy(const int&) const;

	//GenInfo
	double GenPt(const int&) const;
	double GenEta(const int&) const;
	double GenPhi(const int&) const;

	TLorentzVector P4(const int&) const;


	//for both

	//for Muon
	bool SelMuISO( const int& ) const;
	bool VetoMuISO( const int& ) const;
	bool IsGlobalMu(const int&) const;
	bool IsTrackerMu(const int&) const;
	bool TightMuID( const int& ) const;
	bool LooseMuID( const int& ) const;
	bool SelMuKinematic( const int& ) const;
	bool VetoMuKinematic( const int& ) const;
	//for SR
	bool SR_SelMu(const int&) const;
	bool SR_VetoMu(const int&) const;
	//for CR-1
	bool CR_SelMu(const int&) const;
	bool CR_VetoMu(const int&) const;
	//for CR-2
	bool CR_SelMu_invISO(const int&) const;
	
	//for PrePreSel
	bool PrePreSelMu(const int&) const;

	//for Electron

	bool TightElISO(const int& ) const;		//used in CR sel

	bool TightElID( const int& ) const;			//with ISO cut
	bool LooseElID( const int& ) const;			//with ISO cut
	bool TightElID_woISO( const int& ) const;	//without ISO cut
	bool LooseElID_woISO( const int& ) const;	//without ISO cut
	bool SelElKinematic( const int& ) const;
	bool VetoElKinematic( const int& ) const;

	//for SR
	bool SR_SelEl(const int&) const;
	bool SR_VetoEl(const int&) const;
	//for CR-1
	bool CR_SelEl(const int&) const;
	bool CR_VetoEl(const int&) const;
	//for CR-2
	bool CR_SelEl_invISO(const int&) const;
	
	//for PrePreSel
	bool PrePreSelEl(const int&) const;

	//for Electron energy cor
	void ElEnergyCor();

};

#endif //PHYSOBJ_H
