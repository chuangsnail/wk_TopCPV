#ifndef HISTS_H
#define HISTS_H

/***************************************************************************
 *
 *	FileName : Hists.h
 *	Description : Make a class to wrap histagrams we want to use 
 *	Date : 2019 Nov 8 ~
 *	Author : Chen-Yu Chuang
 *
 * ************************************************************************/

#include <string>
#include <string.h>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

#include "TH1.h"
#include "TH2.h"
#include "THStack.h"
#include "TFile.h"
#include "TNtupleD.h"
#include <fstream>

#include "TopCPViolation/select/interface/Tool.h"
#include "TopCPViolation/select/interface/about_time.h"

using namespace std;
using namespace CalTool;


//Tool for Hist

inline double t_IntegralTH1( const TH1* h ) { return h->Integral(1,h->GetXaxis()->GetNbins()+1); }

inline void NormalizeTH1( TH1* h ) { h->Scale( (double)1./t_IntegralTH1( h ) ); }

//maybe we can use a mother class and inheritance, and the bins_No,hist_min/max are the mother member, and  Init(),WriteIn() are virtual functions! 

class Hists_Acp
{
public:

	//just for statistical!
	TH1D* h_Obs3_mu_P;
	TH1D* h_Obs3_mu_N;
	TH1D* h_Obs6_mu_P;
	TH1D* h_Obs6_mu_N;
	TH1D* h_Obs12_mu_P;
	TH1D* h_Obs12_mu_N;
	TH1D* h_Obs13_mu_P;
	TH1D* h_Obs13_mu_N;
	
	TH1D* h_Obs3_el_P;
	TH1D* h_Obs3_el_N;
	TH1D* h_Obs6_el_P;
	TH1D* h_Obs6_el_N;
	TH1D* h_Obs12_el_P;
	TH1D* h_Obs12_el_N;
	TH1D* h_Obs13_el_P;
	TH1D* h_Obs13_el_N;

	map<string, TH1D*> h_Obs3_P, h_Obs3_N, h_Obs6_P, h_Obs6_N, h_Obs12_P, h_Obs12_N, h_Obs13_P, h_Obs13_N;

	TH1D* h_Obs_mu_P;
	TH1D* h_Obs_mu_N;
	TH1D* h_Obs_el_P;
	TH1D* h_Obs_el_N;
	
	TH1D* h_Obs_t_P;
	TH1D* h_Obs_t_N;

	map<string, TH1D*> h_Obs_P, h_Obs_N;

	void Init( const string& );
	void InitMap();

	void FillIn( const string& option, const string& ch, const double& obs, const double& weight );
	void WriteIn();
};

class Hists_proper
{
public:
    Hists_proper()
    {
		proper_name = "";
		bins_No = 100;
		hist_max = 1.;
		hist_min = 0.;
		AxisInfo = "";
    }
	
	Hists_proper( const string& n, const string& ai, const int& bn, const double& bmin, const double& bmax )
	{
		proper_name = n;
		AxisInfo = ai;
		bins_No = bn;
		hist_min = bmin;
		hist_max = bmax;
	}
    ~Hists_proper() {}

	string proper_name;
	int bins_No;
	double hist_min;
	double hist_max;
	string AxisInfo;
	
	TH1F* h_TT_mu;		TH1F* h_TT_el;	
	TH1F* h_DY_mu;		TH1F* h_DY_el;
	TH1F* h_WJets_mu;	TH1F* h_WJets_el;
	TH1F* h_VV_mu;		TH1F* h_VV_el;
	TH1F* h_ST_mu;		TH1F* h_ST_el;
	TH1F* h_QCD_mu;		TH1F* h_QCD_el;
	TH1F* h_Data_mu;	TH1F* h_Data_el;

	vector<TH1F*> h_mu, h_el;

	void Init();

	inline void SetBinInfo( const int& n, const double& m, const double& M  ){
		bins_No = n;	
		hist_min = m;	
		hist_max = M;
	}
	
	inline void SetAxisInfo( const string& input ) {
		AxisInfo = input;
	}

	inline void SetProperName( const string& input ) {
		proper_name = input;
	}

	inline void SetInfo( const string& n, const string& ai, const int& bn, const double& bmin, const double& bmax )
	{
		proper_name = n;
		AxisInfo = ai;
		bins_No = bn;
		hist_min = bmin;
		hist_max = bmax;
	}
	void FillHist( const string& option, const int& k, const string& ch, const double& proper_value, const double& weight );

	void WriteIn();
};

class Hists
{
public:
	
	Hists()
	{
		bins_No = 50;
		hist_min = 0.;
		hist_max = 500.;
		AxisInfo = " ;M_{jjb} Mass(GeV);Events(No.)";
		AxisInfo2 = " ;M_{lb} Mass(GeV);Events(No.)";

	}
	~Hists() {}


	int bins_No;
	double hist_min;
	double hist_max;
	string AxisInfo;
	string AxisInfo2;

	vector<TH1F*> h_mu, h_el, h_l_mu, h_l_el;

	TH1F* h_TT_mu;		TH1F* h_TT_el;	
	TH1F* h_DY_mu;		TH1F* h_DY_el;
	TH1F* h_WJets_mu;	TH1F* h_WJets_el;
	TH1F* h_VV_mu;		TH1F* h_VV_el;
	TH1F* h_ST_mu;		TH1F* h_ST_el;
	TH1F* h_QCD_mu;		TH1F* h_QCD_el;
	TH1F* h_Data_mu;	TH1F* h_Data_el;

	TH1F* h_l_TT_mu;	TH1F* h_l_TT_el;
	TH1F* h_l_DY_mu;	TH1F* h_l_DY_el;
	TH1F* h_l_WJets_mu;	TH1F* h_l_WJets_el;
	TH1F* h_l_VV_mu;	TH1F* h_l_VV_el;
	TH1F* h_l_ST_mu;	TH1F* h_l_ST_el;
	TH1F* h_l_QCD_mu;	TH1F* h_l_QCD_el;
	TH1F* h_l_Data_mu;	TH1F* h_l_Data_el;

	vector<TH1F*> h_mu_c, h_el_c, h_l_mu_c, h_l_el_c;

	TH1F* h_TT_mu_c;		TH1F* h_TT_el_c;	
	TH1F* h_DY_mu_c;		TH1F* h_DY_el_c;
	TH1F* h_WJets_mu_c;		TH1F* h_WJets_el_c;
	TH1F* h_VV_mu_c;		TH1F* h_VV_el_c;
	TH1F* h_ST_mu_c;		TH1F* h_ST_el_c;
	TH1F* h_QCD_mu_c;		TH1F* h_QCD_el_c;
	TH1F* h_Data_mu_c;		TH1F* h_Data_el_c;

	TH1F* h_l_TT_mu_c;		TH1F* h_l_TT_el_c;
	TH1F* h_l_DY_mu_c;		TH1F* h_l_DY_el_c;
	TH1F* h_l_WJets_mu_c;	TH1F* h_l_WJets_el_c;
	TH1F* h_l_VV_mu_c;		TH1F* h_l_VV_el_c;
	TH1F* h_l_ST_mu_c;		TH1F* h_l_ST_el_c;
	TH1F* h_l_QCD_mu_c;		TH1F* h_l_QCD_el_c;
	TH1F* h_l_Data_mu_c;	TH1F* h_l_Data_el_c;

	vector<TH1F*> h_mu_cc, h_el_cc, h_l_mu_cc, h_l_el_cc;

	TH1F* h_TT_mu_cc;		TH1F* h_TT_el_cc;	
	TH1F* h_DY_mu_cc;		TH1F* h_DY_el_cc;
	TH1F* h_WJets_mu_cc;	TH1F* h_WJets_el_cc;
	TH1F* h_VV_mu_cc;		TH1F* h_VV_el_cc;
	TH1F* h_ST_mu_cc;		TH1F* h_ST_el_cc;
	TH1F* h_QCD_mu_cc;		TH1F* h_QCD_el_cc;
	TH1F* h_Data_mu_cc;		TH1F* h_Data_el_cc;

	TH1F* h_l_TT_mu_cc;		TH1F* h_l_TT_el_cc;
	TH1F* h_l_DY_mu_cc;		TH1F* h_l_DY_el_cc;
	TH1F* h_l_WJets_mu_cc;	TH1F* h_l_WJets_el_cc;
	TH1F* h_l_VV_mu_cc;		TH1F* h_l_VV_el_cc;
	TH1F* h_l_ST_mu_cc;		TH1F* h_l_ST_el_cc;
	TH1F* h_l_QCD_mu_cc;	TH1F* h_l_QCD_el_cc;
	TH1F* h_l_Data_mu_cc;	TH1F* h_l_Data_el_cc;

	TH2D* h_mvamax_hadmass_mu;		//with hadmass
	TH2D* h_mvamax_hadmass_el;
	TH2D* h_mvamax_lepmass_mu;		//with lepmass
	TH2D* h_mvamax_lepmass_el;
	map<string, TH2D*> h_mvamax_hadmass, h_mvamax_lepmass;
	//double algo_v_min;
	//double algo_v_max;

	TNtupleD* mvav_mass_mu;
	TNtupleD* mvav_mass_el;
	map<string, TNtupleD*> mvav_mass;

	//used before init hists
	inline void SetBinInfo( const int& n, const double& m, const double& M  ){
		bins_No = n;	
		hist_min = m;	
		hist_max = M;
	}

	//used before init hists
	inline void SetAxisInfo( const string& input1, const string& input2 )
	{
		AxisInfo = input1;
		AxisInfo2 = input2;
	}
/*
	void SetAxisTitle( const string& x, const string& y, const string total= "" )
	{
		h_Data
	}
*/

	//for fill and use new things
	void NoCutModeON();
	void OneCutModeON();
	void TwoCutModeON();
	void InitNoCutVector();
	void InitOneCutVector();
	void InitTwoCutVector();

	void TH2ModeON( const int& algo_bins_No, const double&, const double&);

	inline void NtupleModeON(){
		mvav_mass_mu = new TNtupleD( "mvav_mass_mu", "store event by event", "max_mva_value:hadt_mass:lept_mass:weight:dataset" );
		mvav_mass_el = new TNtupleD( "mvav_mass_el", "store event by event", "max_mva_value:hadt_mass:lept_mass:weight:dataset" );
		mvav_mass[ "mu" ] = mvav_mass_mu;
		mvav_mass[ "el" ] = mvav_mass_el;
	}
    
    void PrintOn( const string& fname, const string& option = "" );

	void WriteIn( const string& option );
	
	//for plots or some applicatin
	void GetObjs( string& file_name, string& option );

	void FillHist( const string&, const int&, const string&, const double&, const double&, const double& );
	//how many cut, k(which sample), channel name, hadmass, lepmass, weight


	//Use to do data-driven
};
void DataDriven( TH1* h_bedd, TH1* h_data );

class Hists_bb
{
public:
	
	Hists_bb()
	{
		bins_No1 = 50;
		hist_min1 = 0.;
		hist_max1 = 1.;
		
		bins_No2 = 100;
		hist_min2 = 0.;
		hist_max2 = 500.;

		no_match_number = 0;
	}
	~Hists_bb() {}
	
	map<string,TH1F*> h_correct, h_charge_mis, h_mistag, h_lt_correct, h_lt_charge_mis, h_lt_mistag;
	map<string, TH2D*> h_mvamax_mass;
	
	TH2D* h_mvamax_mass_mu;
	TH2D* h_mvamax_mass_el;
	TH2D* h_mvamax_mass_t;

	TH1F* h_correct_t;
	TH1F* h_mistag_t;
	TH1F* h_charge_mis_t;

	TH1F* h_correct_mu;
	TH1F* h_mistag_mu;
	TH1F* h_charge_mis_mu;

	TH1F* h_correct_el;
	TH1F* h_mistag_el;
	TH1F* h_charge_mis_el;

	TH1F* h_lt_correct_t;
	TH1F* h_lt_mistag_t;
	TH1F* h_lt_charge_mis_t;

	TH1F* h_lt_correct_mu;
	TH1F* h_lt_mistag_mu;
	TH1F* h_lt_charge_mis_mu;

	TH1F* h_lt_correct_el;
	TH1F* h_lt_mistag_el;
	TH1F* h_lt_charge_mis_el;

	int no_match_number;

	//for algo value
	int bins_No1;
	double hist_min1;
	double hist_max1;
	//for Mlb
	int bins_No2;
	double hist_min2;
	double hist_max2;

	inline void SetAlgoBin( const int& n, const double& m, const double& M ){
		bins_No1 = n;	
		hist_min1 = m;	
		hist_max1 = M;
	}
	void Init();
	void WriteIn();
};

class Hists_cor
{
public:

	Hists_cor()
	{
		bins_No1 = 100;	
		hist_min1 = 0.;	
		hist_max1 = 1.;
	}

	~Hists_cor(){}

	//for store	mva
	map<string, TH1F*> h_max_mva, h_max_mva_cor, h_max_mva_incor;
	
	TH1F* h_max_mva_mu;
	TH1F* h_max_mva_cor_mu;
	TH1F* h_max_mva_incor_mu;
		
	TH1F* h_max_mva_el;
	TH1F* h_max_mva_cor_el;
	TH1F* h_max_mva_incor_el;

	TH1F* h_max_mva_t;
	TH1F* h_max_mva_cor_t;
	TH1F* h_max_mva_incor_t;

	int bins_No1;
	double hist_min1;
	double hist_max1;
	
	//for store to calculate Poisson error
	//chosen -> 'all': pass full-sel / 'part': pass full-sel and pass gen-matching		
	//cor -> pass 'all'/'pass' and it's correct permutation(combination)
	
	map<string, TH1F*> h_chosen, h_cor;
	
	TH1F* h_chosen_mu;
	TH1F* h_cor_mu;
		
	TH1F* h_chosen_el;
	TH1F* h_cor_el;
	
	TH1F* h_chosen_t;
	TH1F* h_cor_t;
	
	inline void SetAlgoBin( const int& n, const double& m, const double& M ){
		bins_No1 = n;	
		hist_min1 = m;	
		hist_max1 = M;
	}
	void Init();
	void WriteIn();

};

/*
class Hists_mva
{
public:
    vector<TH1F*> h_01_mu;      vector<TH1F*> h_01_el;
    vector<TH1F*> h_02_mu;      vector<TH1F*> h_02_el;
    vector<TH1F*> h_03_mu;      vector<TH1F*> h_03_el;
    vector<TH1F*> h_04_mu;      vector<TH1F*> h_04_el;
    vector<TH1F*> h_05_mu;      vector<TH1F*> h_05_el;
    vector<TH1F*> h_06_mu;      vector<TH1F*> h_06_el;
    vector<TH1F*> h_07_mu;      vector<TH1F*> h_07_el;
    vector<TH1F*> h_08_mu;      vector<TH1F*> h_08_el;
    vector<TH1F*> h_09_mu;      vector<TH1F*> h_09_el;
    vector<TH1F*> h_10_mu;      vector<TH1F*> h_10_el;
    vector<TH1F*> h_11_mu;      vector<TH1F*> h_11_el;
    vector<TH1F*> h_12_mu;      vector<TH1F*> h_12_el;
    vector<TH1F*> h_13_mu;      vector<TH1F*> h_13_el;
    vector<TH1F*> h_14_mu;      vector<TH1F*> h_14_el;
    vector<TH1F*> h_15_mu;      vector<TH1F*> h_15_el;
    vector<TH1F*> h_16_mu;      vector<TH1F*> h_16_el;
    vector<TH1F*> h_17_mu;      vector<TH1F*> h_17_el;
    vector<TH1F*> h_18_mu;      vector<TH1F*> h_18_el;
    vector<TH1F*> h_19_mu;      vector<TH1F*> h_19_el;
    vector<TH1F*> h_20_mu;      vector<TH1F*> h_20_el;
    vector<TH1F*> h_21_mu;      vector<TH1F*> h_21_el;
    vector<TH1F*> h_22_mu;      vector<TH1F*> h_22_el;

    vector< vector<TH1F*>* > h_mu, h_el;

    void Initialize( const int& var_no );
    void FillVec();
*/

/*
//use to see the discripency 
class Hists_test
{
public:
	int len;
	int now_idx;
	
	TH1F* h_MC;
	TH1F* h_Data;
	
	Hists_test( int& space ) 
	{
		len = space;
		h_MC = new TH1F[space];
		h_Data = new TH1F[space];
		now_idx = 0;
		len = 0;
	};

	~Hists_test()
	{
		if(len == 1){
			delete h_MC;
			delete h_Data;
		}
		else if(len > 1){
			delete [] h_MC;
			delete [] h_Data;
		}
	}

	void Set_idx(const int& i){
		assert(i <= len);
		now_idx = i;
	}

	void FillIn(const bool& is_data ,const double& value, const double& weight = 1. )
	{
		if(now_idx > len){
			len = now_idx;
		}

		if(is_data)
			h_Data[now_idx]->Fill(value, weight);
		else
			h_MC[now_idx]->Fill(value, weight);
		++now_idx;
	}

	void Store_Discrepancy(string& file_name)
	{
		TH1F* result = new TH1F[len];
		for(int i=0;i<len;++i)
		{
			result[i] = (TH1F*)( h_Data[i]->Clone() );
			string name = "discrepancy_" + i;
			result[i]->Divide( h_MC[i] );
			result[i]->SetName( name );
		}

		TFile* f = new TFile((char*)file_name.c_str(),"recreate");
		for(int i=0;i<len;++i){
			result[i]->Write();
		}
		f->Close();
		delete f;
	}

};
*/

#endif
