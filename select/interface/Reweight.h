#ifndef REWEIGHT_H
#define REWEIGHT_H

//#include "TopCPViolation/select/interface/frameDef.h"
#include "frameDef.h"



class SF_2D
{
public:
	TH2F* h;
	TH2D* hd;

	SF_2D(TH2F* h_input)
	{	h = h_input;		}
	
	SF_2D(TH2D* h_input_d)
	{	hd = h_input_d;		}

	~SF_2D() {}

	double GetTH2FBinContent(double _x,double _y);
	double GetTH2DBinContent(double _x,double _y);
	
};

class LeptonSFMgr
{
private:
	TH2F* h_tightMuIDSF;
	TH2D* h_MuISOSF;
	TH2F* h_MuTrgSF;

	TH2F* h_tightElIDSF;
	TH2F* h_ElRECOSF;
	TH2D* h_ElTrgSF;

public:
	LeptonSFMgr()
	{
		h_tightMuIDSF = NULL;
		h_MuISOSF = NULL;
		h_MuTrgSF = NULL;

		h_tightElIDSF = NULL;
		h_ElRECOSF = NULL;
		h_ElTrgSF = NULL;
	}

	void RegInTH2( TH2F* a, TH2D* b, TH2F* c, TH2F* d, TH2F* e, TH2D* f )
	{
		h_tightMuIDSF = a;
		h_MuISOSF = b;
		h_MuTrgSF = c;

		h_tightElIDSF = d;
		h_ElRECOSF = e;
		h_ElTrgSF = f;
	}

	double GetLepSF( const string&, const double&, const double& );
};



class BtagMgr
{
public:
	
    JetInfo* jets;

	//map<BTagEntry::OperatingPoint, BTagCalibrationReader> map_b, map_c, map_l;
	map<BTagEntry::OperatingPoint, BTagCalibrationReader> map_;

	BTagEntry::OperatingPoint op;			//this is the case for all with the same operating point
	
    vector<int> tagged_b;	//(10,-1);
	vector<int> ntagged_b;	//(10,-1);

	TH2F* eff_b;
	TH2F* eff_c;
	TH2F* eff_l;

	BtagMgr( JetInfo* jetInfo );		//Constructor

	void Set_OP( BTagEntry::OperatingPoint );

	void Register_Init_Maps();
	void Register_Init_TH2( TH2F* , TH2F* , TH2F* );

	void Reset_idx_capacity();
	void Reset_b_tagged_jets_idx();
	void Reset_b_ntagged_jets_idx();
	void Set_b_tagged_jets_idx( vector<int>& sel_b_jets );
	void Set_b_ntagged_jets_idx( vector<int>& sel_jets );

	double Get_Btag_Scale_Factor( const int );
	double Get_Btag_Efficiency( const int );		//just for op = Medium
	double Get_Btag_Weight();
	
	~BtagMgr() 
	{
        tagged_b.clear();
        ntagged_b.clear();
	}		
};

class PileUpMgr
{
private:
	EvtInfo* evt;
	double PU_weight;
	vector<double> pileupweights;
public:
	PileUpMgr( EvtInfo* e ) { RegIn(e); PU_weight = 1.; }
	void RegIn( EvtInfo* e ) { evt = e; }

	void RegInPUvec(const string&);
	bool PassPileUpWeight();
	double GetPUWeight() { return PU_weight; }

};

class HLTMgr
{
private:
	TrgInfo* trg;
	vector<int> HLT_MC_mu;
	vector<int> HLT_MC_el;
	vector<int> HLT_Data_mu;
	vector<int> HLT_Data_el;	

public:
	HLTMgr( TrgInfo* t ) 
	{
		RegIn(t);
		GetHLT();
	}

	void GetHLT() 
	{
		GetHLTinfo("MC_mu");
		GetHLTinfo("MC_el");
		GetHLTinfo("Data_mu");
		GetHLTinfo("Data_el");
	}
	void RegIn( TrgInfo* t ) { trg = t; }
	void GetHLTinfo( const string& );
	bool PassHLT( const bool&, const string& );

};

#endif //REWEIGHT_H
