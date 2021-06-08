/******************************************************************
 *
 *	File Name : SelMgr.h
 *	Description : 
 *
 *
 *
 * ****************************************************************/

#ifndef SELMGR_H_
#define SELMGR_H_


#include "TopCPViolation/select/interface/frameDef.h"
#include "TopCPViolation/select/interface/PhysObj.h"
#include "TopCPViolation/select/interface/RunObj.h"
#include "TopCPViolation/select/interface/Reweight.h"
#include "TopCPViolation/select/interface/GenMgr.h"
#include "TopCPViolation/select/interface/DFMgr.h"
//#include "TopCPViolation/select/interface/checkEvtTool.h"

#include "TopCPViolation/select/interface/Hists.h"
#include "TopCPViolation/select/interface/ttt.h"

/*
#include "frameDef.h"
#include "PhysObj.h"
#include "RunObj.h"
#include "Reweight.h"
#include "GenMgr.h"
*/
//#include "MVAvar.h"

using namespace std;

//class IClassifierReader; 

class SelMgr
{
private:
	//some tool functions as private functions
	double delR(const double&, const double&, const double&, const double&);
	void swap_v(int&, int&);
	double chi2_v( const int& idx_b, const int& idx_j1, const int& idx_j2 );//idx in vector ,not in JetInfo
	double chi2_value( const int& idx_b, const int& idx_j1, const int& idx_j2 );//idx in JetInfo
	double factorial(int);
	void EraseByValue( vector<int>&, int& );

protected:

	bool is_SR;		//if it is CR, then it's false.

	Jet jets;
	Lepton leps;
	Event evt;		//include trigger info.
	Gen gen;
	GenMgr genMgr;

	double weight;
	double lep_weight;
	double btag_weight;
	double pu_weight;
	double other_weight;
	string channel;
	bool is_data;

	//--- store reco objects idx after selection ---//
	int sel_lep;
	vector<int> sel_jets;
	vector<int> sel_b_jets;
	int Hadb;
	int Lepb;
	int J1;
	int J2;

	double reco_algo_value;

	//--- For MC reweight (PlugIn method) ---//
	BtagMgr* btagMgr;
	PileUpMgr* puMgr;
	LeptonSFMgr* lepsfMgr;
	HLTMgr* hltMgr;

	string training_name;

	//--- For syst.unc. option ---//
	
	string opt_btag;
	string opt_lepsf;
	string opt_pu;
	string opt_jes;
	string opt_jer;

    //--- for golden json file ---//
    //checkEvtTool checkEvt;

	//TTT t;

public:
	SelMgr(){};			//it can call JetMgr::JetMgr(){};
	SelMgr( JetInfo*, LeptonInfo*, EvtInfo*, VertexInfo* ,GenInfo* );

	void reset();
	void test_sel_print();

	TLorentzVector& JetP4(const int&) const;
	TLorentzVector& LepP4(const int&) const;
	
	void SetChannel( const string& ch ) { channel = ch; }
	string GetChannel() const { return channel; }
	string& Channel() { return channel; }

	void ScaleWeight( const double& sf ) { weight *= sf; }
	double GetWeight() const { return weight;} 
	double GetLepWeight( const string& opt = "" ) const;
	double GetBtagWeight( const string& opt = "" ) const;
	double GetPUWeight( const string& opt = "" ) const; 
	double GetOtherWeight() const { return other_weight; }	
	
	double& Weight() { return weight; }
	double& LepWeight() { return lep_weight; }
	double& BtagWeight() { return btag_weight; }
	double& PUWeight() { return pu_weight; }
	double& OtherWeight() { return other_weight; }
	
	//used to make a copy out of 'this' 
	int Getidx_Lep() const { return sel_lep; }
	int Getidx_Hadb() const { return Hadb; }
	int Getidx_Lepb() const { return Lepb; }
	int Getidx_J1() const { return J1; }
	int Getidx_J2() const { return J2; }

	//--- used to compare or read, we can alse use this to set it's value ---//
	int& Idx_Lep() { return sel_lep; }		
	int& Idx_Hadb() { return Hadb; }
	int& Idx_Lepb() { return Lepb; }
	int& Idx_J1() { return J1; }
	int& Idx_J2() { return J2; }
	
	//--- used to set outside ---//
	void Setidx_Lep( const int& s ) { sel_lep = s; }
	void Setidx_Hadb( const int& s ) { Hadb = s; }
	void Setidx_Lepb( const int& s ) { Lepb = s; }
	void Setidx_J1( const int& s ) { J1 = s; }
	void Setidx_J2( const int& s ) { J2 = s; }
	
	int seljets_No() const { return (int)sel_jets.size(); }
	int selbjets_No() const { return (int)sel_b_jets.size(); }
	
	//return an alias of the vector in the SelMgr object
	vector<int> GetSelJets() const { return sel_jets; }
	vector<int> GetSelBJets() const { return sel_b_jets; }
	
	vector<int>& Sel_Jets() { return sel_jets; }
	vector<int>& Sel_BJets() { return sel_b_jets; }

	void SetSelJets( const vector<int>& v ) 
	{ sel_jets.clear(); sel_jets.assign( v.begin(), v.end() ); }
	void SetSelBJets( const vector<int>& v ) 
	{ sel_b_jets.clear(); sel_b_jets.assign( v.begin(), v.end() ); }

	void SetIsData( const bool& i ) { is_data = i; }

	double& RecoAlgoValue() { return reco_algo_value; }
	double GetRecoAlgoValue() const { return reco_algo_value; }

	//--- for SR/CR ---//
	void SetSR() { is_SR = true; }
	void SetCR() { is_SR = false; }

    //--- for Data ---//
    bool Pass_Golden_Json();

	//--- for lep selectoin ---//

	bool SR_LepVeto();
	bool SR_SelLep();

	bool CR_LepVeto();
	bool CR_SelLep();

	bool CR_SelLep_invISO();

	bool PrePreSelLep();
	bool PreSelLep();
	
	bool test_dbl_Lep();
	bool dbl_Lep();

	//--- for jet selection ---//
	void JetCorrection();
	
	void UncPreSelJets();
	void SelJets();
	void JetLepISO();
	bool SR_bjets();		
	//seperate bjets/non-bjets and pick them from sel_jets to sel_b_jets
	bool CR_bjets();
	//void ChooseCandidates( const string& );
	void Chi2Sort();
	void MVASort();
	string MVAname() { return training_name; }

	bool UncPreSel();
	bool PrePreSel();
	bool PreSel();	
	bool SR_select( const bool& is_reweight = true );
	bool CR_select();
	bool CR_select_invISO();

	bool test_pre_from_dbl_SR();
	bool test_dbl_SR();
	bool dbl_SR();

    bool full_sel_SR( const bool& is_reweight = true );
    bool full_sel_CR( const bool& is_reweight = true );

	//--- for MC reweight ---//

	void PlugInReweightTool( BtagMgr*, LeptonSFMgr* );
	void PlugInPUTool( PileUpMgr* );
	void PlugInHLTTool( HLTMgr* );

	bool SR_ReweightMC();
	bool CR_ReweightMC();

	//--- for sample checking ---//
    double Hadb_DeepCSV() { return jets.DeepCSV( Hadb );  };
    double Lepb_DeepCSV() { return jets.DeepCSV( Lepb );  };
	void SelJets_deepCSV( vector<double>&, const string& );
	int TightLeptonNo();
	void SelBJets();

	// a nested class ( realized in Reco.h/Reco.cc )
	//class Reco;
	
	//--- For gen-matching ---//

	bool Find_GenTW();	//store the found things in Hadb, J1, J2 members ( in Selection.cc )
	bool JustGen_ReweightMC();	//( in Selection.cc )

	//--- For training ---//
	void SetTrain( string& t )
	{	training_name = t;	}
	
	string GetTrain()
	{	return training_name;	}

	//--- For uncertainty option ---//
	// { "up", "down", "central" }
	void Set_Opt_Btag( const string& s ) { opt_btag = s; }
	void Set_Opt_LepSF( const string& s ) { opt_lepsf = s; }
	void Set_Opt_PU( const string& s ) { opt_pu = s; }
	void Set_Opt_JER( const string& s ) { opt_jer = s; }
	void Set_Opt_JES( const string& s ) { opt_jes = s; }

};


#endif	//SELMGR_H
