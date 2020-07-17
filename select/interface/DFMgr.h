#include "frameDef.h"

class DFMgr
{
private:
	TH1D* h_O3;
	TH1D* h_O6;
	TH1D* h_O12;
	TH1D* h_O13;
	TH1D* h_bb_O3;
	TH1D* h_bb_O6;
	TH1D* h_bb_O12;
	TH1D* h_bb_O13;
	TH1D* h_bbj1_O3;
	TH1D* h_bbj1_O6;
	TH1D* h_bbj1_O12;
	TH1D* h_bbj1_O13;
	map< string, TH1D* > h_Obs, h_bb_Obs, h_bbj1_Obs;

public:
	//--- set some member public for convenience ---//
	double DF_O3;
	double DF_O6;
	double DF_O12;
	double DF_O13;
	double DF_bb_O3;
	double DF_bb_O6;
	double DF_bb_O12;
	double DF_bb_O13;
	double DF_bbj1_O3;
	double DF_bbj1_O6;
	double DF_bbj1_O12;
	double DF_bbj1_O13;
	
	double DF_err_O3;
	double DF_err_O6;
	double DF_err_O12;
	double DF_err_O13;
	double DF_err_bb_O3;
	double DF_err_bb_O6;
	double DF_err_bb_O12;
	double DF_err_bb_O13;
	double DF_err_bbj1_O3;
	double DF_err_bbj1_O6;
	double DF_err_bbj1_O12;
	double DF_err_bbj1_O13;

	double cor_O3;
	double cor_O6;
	double cor_O12;
	double cor_O13;
	double cor_bb_O3;
	double cor_bb_O6;
	double cor_bb_O12;
	double cor_bb_O13;
	double cor_bbj1_O3;
	double cor_bbj1_O6;
	double cor_bbj1_O12;
	double cor_bbj1_O13;
	
	double cor_err_O3;
	double cor_err_O6;
	double cor_err_O12;
	double cor_err_O13;
	double cor_err_bb_O3;
	double cor_err_bb_O6;
	double cor_err_bb_O12;
	double cor_err_bb_O13;
	double cor_err_bbj1_O3;
	double cor_err_bbj1_O6;
	double cor_err_bbj1_O12;
	double cor_err_bbj1_O13;

	DFMgr()
	{
		h_O3 = new  TH1D( "h_O3", "", 3, 0., 3. );
		h_O6 = new  TH1D( "h_O6", "", 3, 0., 3. );
		h_O12 = new  TH1D( "h_O12", "", 3, 0., 3. );
		h_O13 = new  TH1D( "h_O13", "", 3, 0., 3. );
		h_bb_O3 = new  TH1D( "h_bb_O3", "", 3, 0., 3. );
		h_bb_O6 = new  TH1D( "h_bb_O6", "", 3, 0., 3. );
		h_bb_O12 = new  TH1D( "h_bb_O12", "", 3, 0., 3. );
		h_bb_O13 = new  TH1D( "h_bb_O13", "", 3, 0., 3. );
		h_bbj1_O3 = new  TH1D( "h_bbj1_O3", "", 3, 0., 3. );
		h_bbj1_O6 = new  TH1D( "h_bbj1_O6", "", 3, 0., 3. );
		h_bbj1_O12 = new  TH1D( "h_bbj1_O12", "", 3, 0., 3. );
		h_bbj1_O13 = new  TH1D( "h_bbj1_O13", "", 3, 0., 3. );
		h_Obs[ "Obs3" ] = h_O3;
		h_Obs[ "Obs6" ] = h_O6;
		h_Obs[ "Obs12" ] = h_O12;
		h_Obs[ "Obs13" ] = h_O13;
		h_bb_Obs[ "Obs3" ] = h_bb_O3;
		h_bb_Obs[ "Obs6" ] = h_bb_O6;
		h_bb_Obs[ "Obs12" ] = h_bb_O12;
		h_bb_Obs[ "Obs13" ] = h_bb_O13;
		h_bbj1_Obs[ "Obs3" ] = h_bbj1_O3;
		h_bbj1_Obs[ "Obs6" ] = h_bbj1_O6;
		h_bbj1_Obs[ "Obs12" ] = h_bbj1_O12;
		h_bbj1_Obs[ "Obs13" ] = h_bbj1_O13;
		
		cor_O3 = 0.;
		cor_O6 = 0.;
		cor_O12 = 0.;
	 	cor_O13 = 0.;
		cor_bb_O3 = 0.;
		cor_bb_O6 = 0.;
		cor_bb_O12 = 0.;
		cor_bb_O13 = 0.;
	 	cor_bbj1_O3 = 0.;
	 	cor_bbj1_O6 = 0.;
	 	cor_bbj1_O12 = 0.;
	 	cor_bbj1_O13 = 0.;

		DF_O3 = 0.;
		DF_O6 = 0.;
		DF_O12 = 0.;
	 	DF_O13 = 0.;
		DF_bb_O3 = 0.;
		DF_bb_O6 = 0.;
		DF_bb_O12 = 0.;
		DF_bb_O13 = 0.;
	 	DF_bbj1_O3 = 0.;
	 	DF_bbj1_O6 = 0.;
	 	DF_bbj1_O12 = 0.;
	 	DF_bbj1_O13 = 0.;
		
		cor_err_O3 = 0.;
		cor_err_O6 = 0.;
		cor_err_O12 = 0.;
	 	cor_err_O13 = 0.;
		cor_err_bb_O3 = 0.;
		cor_err_bb_O6 = 0.;
		cor_err_bb_O12 = 0.;
		cor_err_bb_O13 = 0.;
	 	cor_err_bbj1_O3 = 0.;
	 	cor_err_bbj1_O6 = 0.;
	 	cor_err_bbj1_O12 = 0.;
	 	cor_err_bbj1_O13 = 0.;

		DF_err_O3 = 0.;
		DF_err_O6 = 0.;
		DF_err_O12 = 0.;
	 	DF_err_O13 = 0.;
		DF_err_bb_O3 = 0.;
		DF_err_bb_O6 = 0.;
		DF_err_bb_O12 = 0.;
		DF_err_bb_O13 = 0.;
	 	DF_err_bbj1_O3 = 0.;
	 	DF_err_bbj1_O6 = 0.;
	 	DF_err_bbj1_O12 = 0.;
	 	DF_err_bbj1_O13 = 0.;

	}

	~DFMgr()
	{
		delete h_O3;
		delete h_O6;
		delete h_O12;
		delete h_O13;
		delete h_bb_O3;
		delete h_bb_O6;
		delete h_bb_O12;
		delete h_bb_O13;
		delete h_bbj1_O3;
		delete h_bbj1_O6;
		delete h_bbj1_O12;
		delete h_bbj1_O13;
	}

	void SignDec( const string& obs, const double& det, const double& gen, const double& weight = 1. );
	void SignDec_bb( const string& obs, const double& det, const double& gen, const double& weight = 1. );
	void SignDec_bbj1( const string& obs, const double& det, const double& gen, const double& weight = 1. );

	void Calculate();
	void WriteIn( const string& option = "N bb bbj1" );
	void PrintIn( ofstream& );

	double GetCorErr(TH1*);

};


