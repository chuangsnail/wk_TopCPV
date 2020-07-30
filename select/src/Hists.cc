/***************************************************************************
 *
 *	FileName : Hists.cc
 *	Description : Make a class to wrap histagrams we want to use 
 *	Date : 2019 Nov 8 ~
 *	Author : Chen-Yu Chuang
 *
 * ************************************************************************/
//#include "TopCPViolation/selected/interface/Hists.h"
#include "../interface/Hists.h"

using namespace CalTool;

void
Hists_Acp::Init( const string& option = "all" )
{
	if( option.find( "div" ) != string::npos )
	{
		h_Obs3_mu_P = new TH1D( "h_Obs3_mu_P", "", 2, 0.5, 2.5 ); 
		h_Obs3_mu_N = new TH1D( "h_Obs3_mu_N", "", 2, 0.5, 2.5 );
		h_Obs6_mu_P = new TH1D( "h_Obs6_mu_P", "", 2, 0.5, 2.5 ); 
		h_Obs6_mu_N = new TH1D( "h_Obs6_mu_N", "", 2, 0.5, 2.5 );
		h_Obs12_mu_P = new TH1D( "h_Obs12_mu_P", "", 2, 0.5, 2.5 ); 
		h_Obs12_mu_N = new TH1D( "h_Obs12_mu_N", "", 2, 0.5, 2.5 );
		h_Obs13_mu_P = new TH1D( "h_Obs13_mu_P", "", 2, 0.5, 2.5 ); 
		h_Obs13_mu_N = new TH1D( "h_Obs13_mu_N", "", 2, 0.5, 2.5 );

		h_Obs3_el_P = new TH1D( "h_Obs3_el_P", "", 2, 0.5, 2.5 ); 
		h_Obs3_el_N = new TH1D( "h_Obs3_el_N", "", 2, 0.5, 2.5 );
		h_Obs6_el_P = new TH1D( "h_Obs6_el_P", "", 2, 0.5, 2.5 ); 
		h_Obs6_el_N = new TH1D( "h_Obs6_el_N", "", 2, 0.5, 2.5 );
		h_Obs12_el_P = new TH1D( "h_Obs12_el_P", "", 2, 0.5, 2.5 ); 
		h_Obs12_el_N = new TH1D( "h_Obs12_el_N", "", 2, 0.5, 2.5 );
		h_Obs13_el_P = new TH1D( "h_Obs13_el_P", "", 2, 0.5, 2.5 ); 
		h_Obs13_el_N = new TH1D( "h_Obs13_el_N", "", 2, 0.5, 2.5 );
	}
	if( option.find( "all" ) != string::npos )
	{
		h_Obs_mu_P = new TH1D( "h_Obs_mu_P", "", 5, 0, 5 ); 
		h_Obs_mu_N = new TH1D( "h_Obs_mu_N", "", 5, 0, 5 );
		h_Obs_el_P = new TH1D( "h_Obs_el_P", "", 5, 0, 5 ); 
		h_Obs_el_N = new TH1D( "h_Obs_el_N", "", 5, 0, 5 );
		h_Obs_t_P = new TH1D( "h_Obs_t_P", "", 5, 0, 5 ); 
		h_Obs_t_N = new TH1D( "h_Obs_t_N", "", 5, 0, 5 );
	}

	InitMap();
}

void 
Hists_Acp::InitMap()
{
	h_Obs3_P[ "mu" ] = h_Obs3_mu_P;			h_Obs3_N[ "mu" ] = h_Obs3_mu_N;
	h_Obs6_P[ "mu" ] = h_Obs6_mu_P;			h_Obs6_N[ "mu" ] = h_Obs6_mu_N;
	h_Obs12_P[ "mu" ] = h_Obs12_mu_P;		h_Obs12_N[ "mu" ] = h_Obs12_mu_N;
	h_Obs13_P[ "mu" ] = h_Obs13_mu_P;		h_Obs13_N[ "mu" ] = h_Obs13_mu_N;
	
	h_Obs3_P[ "el" ] = h_Obs3_el_P;			h_Obs3_N[ "el" ] = h_Obs3_el_N;
	h_Obs6_P[ "el" ] = h_Obs6_el_P;			h_Obs6_N[ "el" ] = h_Obs6_el_N;
	h_Obs12_P[ "el" ] = h_Obs12_el_P;		h_Obs12_N[ "el" ] = h_Obs12_el_N;
	h_Obs13_P[ "el" ] = h_Obs13_el_P;		h_Obs13_N[ "el" ] = h_Obs13_el_N;

	h_Obs_P[ "mu" ] = h_Obs_mu_P;			h_Obs_N[ "mu" ] = h_Obs_mu_N;
	h_Obs_P[ "el" ] = h_Obs_el_P;			h_Obs_N[ "el" ] = h_Obs_el_N;
}

void Hists_Acp::
FillIn( const string& option, const string& ch , const double& obs, const double& weight )
{

	if( obs == DBL_MAX ) return;
	
    if( option == "Obs3" )
	{
		if( obs > 0. )
		{
			h_Obs_t_P->Fill( 1., weight );
			h_Obs_P[ ch ]->Fill( 1., weight );
		}
		else
		{
			h_Obs_t_N->Fill( 1., weight );
			h_Obs_N[ ch ]->Fill( 1., weight );
		}
	}
	else if( option == "Obs6" )
	{
		if( obs > 0. )
		{
			h_Obs_t_P->Fill( 2., weight );
			h_Obs_P[ ch ]->Fill( 2., weight );
		}
		else
		{
			h_Obs_t_N->Fill( 2., weight );
			h_Obs_N[ ch ]->Fill( 2., weight );
		}
	}
	else if( option == "Obs12" )
	{
		if( obs > 0. )
		{
			h_Obs_t_P->Fill( 3., weight );
			h_Obs_P[ ch ]->Fill( 3., weight );
		}
		else
		{
			h_Obs_t_N->Fill( 3., weight );
			h_Obs_N[ ch ]->Fill( 3., weight );
		}
	}
	else if( option == "Obs13" )
	{
		if( obs > 0. )
		{
			h_Obs_t_P->Fill( 4., weight );
			h_Obs_P[ ch ]->Fill( 4., weight );
		}
		else
		{
			h_Obs_t_N->Fill( 4., weight );
			h_Obs_N[ ch ]->Fill( 4., weight );
		}
	}
}


void
Hists_Acp::WriteIn()
{
/*
	h_Obs3_mu_P->Write(); 
	h_Obs3_mu_N->Write();
	h_Obs6_mu_P->Write(); 
	h_Obs6_mu_N->Write();
	h_Obs12_mu_P->Write(); 
	h_Obs12_mu_N->Write();
	h_Obs13_mu_P->Write(); 
	h_Obs13_mu_N->Write();
	
	h_Obs3_el_P->Write(); 
	h_Obs3_el_N->Write();
	h_Obs6_el_P->Write(); 
	h_Obs6_el_N->Write();
	h_Obs12_el_P->Write(); 
	h_Obs12_el_N->Write();
	h_Obs13_el_P->Write(); 
	h_Obs13_el_N->Write();
*/
	h_Obs_mu_P->Write();
	h_Obs_mu_N->Write();
	h_Obs_el_P->Write();
	h_Obs_el_N->Write();
	h_Obs_t_P->Write();
	h_Obs_t_N->Write();

}

void Hists_proper::Init()
{

	h_TT_mu = new TH1F("h_TT_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	h_TT_el = new TH1F("h_TT_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	
	h_DY_mu = new TH1F("h_DY_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	h_DY_el = new TH1F("h_DY_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);

	h_WJets_mu = new TH1F("h_WJets_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	h_WJets_el = new TH1F("h_WJets_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	
	h_VV_mu = new TH1F("h_VV_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	h_VV_el = new TH1F("h_VV_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);

	h_ST_mu = new TH1F("h_ST_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	h_ST_el = new TH1F("h_ST_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	
	h_QCD_mu = new TH1F("h_QCD_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	h_QCD_el = new TH1F("h_QCD_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);
	
	h_Data_mu = new TH1F("h_Data_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);	
	h_Data_el = new TH1F("h_Data_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);

	h_mu.push_back(h_TT_mu);		h_mu.push_back(h_DY_mu);
	h_mu.push_back(h_WJets_mu);		h_mu.push_back(h_VV_mu);
	h_mu.push_back(h_ST_mu);		h_mu.push_back(h_QCD_mu);
	h_mu.push_back(h_Data_mu);

	h_el.push_back(h_TT_el);		h_el.push_back(h_DY_el);
	h_el.push_back(h_WJets_el);		h_el.push_back(h_VV_el);
	h_el.push_back(h_ST_el);		h_el.push_back(h_QCD_el);
	h_el.push_back(h_Data_el);
	
}

void
Hists_proper::WriteIn()
{
	h_TT_mu->Write();		
	h_TT_el->Write();	
	h_DY_mu->Write();		
	h_DY_el->Write();
	h_WJets_mu->Write();	
	h_WJets_el->Write();
	h_VV_mu->Write();		
	h_VV_el->Write();
	h_ST_mu->Write();		
	h_ST_el->Write();
	h_QCD_mu->Write();		
	h_QCD_el->Write();
	h_Data_mu->Write();	
	h_Data_el->Write();
}

void
Hists_proper::FillHist( const string& option, const int& k, const string& ch, const double& proper_value, const double& weight )
{
	if( ch == "mu" )
	{
		h_mu.at(k)->Fill(proper_value, weight);
	}
	else if( ch == "el" )
	{
		h_el.at(k)->Fill(proper_value, weight);
	}
	else
	{
		cerr << "WRONG in channel when fill histograms!" << endl;
	}
}

void Hists::GetObjs( string& file_name, string& option )
{
	TFile* f = new TFile( (char*)file_name.c_str() );
	

	if( option.find("NC") != string::npos )
	{
		h_TT_mu = new TH1F();		h_TT_el = new TH1F();
		h_DY_mu = new TH1F();		h_DY_el = new TH1F();
		h_WJets_mu = new TH1F();	h_WJets_el = new TH1F();
		h_VV_mu = new TH1F();		h_VV_el = new TH1F();
		h_ST_mu = new TH1F();		h_ST_el = new TH1F();
		h_QCD_mu = new TH1F();		h_QCD_el = new TH1F();
		h_Data_mu = new TH1F();		h_Data_el = new TH1F();
		h_l_TT_mu = new TH1F();		h_l_TT_el = new TH1F();
		h_l_DY_mu = new TH1F();		h_l_DY_el = new TH1F();
		h_l_WJets_mu = new TH1F();	h_l_WJets_el = new TH1F();
		h_l_VV_mu = new TH1F();		h_l_VV_el = new TH1F();
		h_l_ST_mu = new TH1F();		h_l_ST_el = new TH1F();
		h_l_QCD_mu = new TH1F();	h_l_QCD_el = new TH1F();
		h_l_Data_mu = new TH1F();	h_l_Data_el = new TH1F();

		f->GetObject("h_TT_mu",h_TT_mu);
		f->GetObject("h_DY_mu",h_DY_mu);
		f->GetObject("h_WJets_mu",h_WJets_mu);
		f->GetObject("h_VV_mu",h_VV_mu);
		f->GetObject("h_ST_mu",h_ST_mu);
		f->GetObject("h_QCD_mu",h_QCD_mu);
		f->GetObject("h_Data_mu",h_Data_mu);

		f->GetObject("h_TT_el",h_TT_el);
		f->GetObject("h_DY_el",h_DY_el);
		f->GetObject("h_WJets_el",h_WJets_el);
		f->GetObject("h_VV_el",h_VV_el);
		f->GetObject("h_ST_el",h_ST_el);
		f->GetObject("h_QCD_el",h_QCD_el);
		f->GetObject("h_Data_el",h_Data_el);

		f->GetObject("h_l_TT_mu",h_l_TT_mu);
		f->GetObject("h_l_DY_mu",h_l_DY_mu);
		f->GetObject("h_l_WJets_mu",h_l_WJets_mu);
		f->GetObject("h_l_VV_mu",h_l_VV_mu);
		f->GetObject("h_l_ST_mu",h_l_ST_mu);
		f->GetObject("h_l_QCD_mu",h_l_QCD_mu);
		f->GetObject("h_l_Data_mu",h_l_Data_mu);

		f->GetObject("h_l_TT_el",h_l_TT_el);
		f->GetObject("h_l_DY_el",h_l_DY_el);
		f->GetObject("h_l_WJets_el",h_l_WJets_el);
		f->GetObject("h_l_VV_el",h_l_VV_el);
		f->GetObject("h_l_ST_el",h_l_ST_el);
		f->GetObject("h_l_QCD_el",h_l_QCD_el);
		f->GetObject("h_l_Data_el",h_l_Data_el);

		InitNoCutVector();
	
	}
	
	if( option.find("1C") != string::npos )
	{
		
		h_TT_mu_c = new TH1F();		h_TT_el_c = new TH1F();
		h_DY_mu_c = new TH1F();		h_DY_el_c = new TH1F();
		h_WJets_mu_c = new TH1F();	h_WJets_el_c = new TH1F();
		h_VV_mu_c = new TH1F();		h_VV_el_c = new TH1F();
		h_ST_mu_c = new TH1F();		h_ST_el_c = new TH1F();
		h_QCD_mu_c = new TH1F();		h_QCD_el_c = new TH1F();
		h_Data_mu_c = new TH1F();		h_Data_el_c = new TH1F();
		h_l_TT_mu_c = new TH1F();		h_l_TT_el_c = new TH1F();
		h_l_DY_mu_c = new TH1F();		h_l_DY_el_c = new TH1F();
		h_l_WJets_mu_c = new TH1F();	h_l_WJets_el_c = new TH1F();
		h_l_VV_mu_c = new TH1F();		h_l_VV_el_c = new TH1F();
		h_l_ST_mu_c = new TH1F();		h_l_ST_el_c = new TH1F();
		h_l_QCD_mu_c = new TH1F();	h_l_QCD_el_c = new TH1F();
		h_l_Data_mu_c = new TH1F();	h_l_Data_el_c = new TH1F();

		f->GetObject("h_TT_mu_c",h_TT_mu_c);
		f->GetObject("h_DY_mu_c",h_DY_mu_c);
		f->GetObject("h_WJets_mu_c",h_WJets_mu_c);
		f->GetObject("h_VV_mu_c",h_VV_mu_c);
		f->GetObject("h_ST_mu_c",h_ST_mu_c);
		f->GetObject("h_QCD_mu_c",h_QCD_mu_c);
		f->GetObject("h_Data_mu_c",h_Data_mu_c);

		f->GetObject("h_TT_el_c",h_TT_el_c);
		f->GetObject("h_DY_el_c",h_DY_el_c);
		f->GetObject("h_WJets_el_c",h_WJets_el_c);
		f->GetObject("h_VV_el_c",h_VV_el_c);
		f->GetObject("h_ST_el_c",h_ST_el_c);
		f->GetObject("h_QCD_el_c",h_QCD_el_c);
		f->GetObject("h_Data_el_c",h_Data_el_c);

		f->GetObject("h_l_TT_mu_c",h_l_TT_mu_c);
		f->GetObject("h_l_DY_mu_c",h_l_DY_mu_c);
		f->GetObject("h_l_WJets_mu_c",h_l_WJets_mu_c);
		f->GetObject("h_l_VV_mu_c",h_l_VV_mu_c);
		f->GetObject("h_l_ST_mu_c",h_l_ST_mu_c);
		f->GetObject("h_l_QCD_mu_c",h_l_QCD_mu_c);
		f->GetObject("h_l_Data_mu_c",h_l_Data_mu_c);

		f->GetObject("h_l_TT_el_c",h_l_TT_el_c);
		f->GetObject("h_l_DY_el_c",h_l_DY_el_c);
		f->GetObject("h_l_WJets_el_c",h_l_WJets_el_c);
		f->GetObject("h_l_VV_el_c",h_l_VV_el_c);
		f->GetObject("h_l_ST_el_c",h_l_ST_el_c);
		f->GetObject("h_l_QCD_el_c",h_l_QCD_el_c);
		f->GetObject("h_l_Data_el_c",h_l_Data_el_c);
		
		InitOneCutVector();
	}
	
	if( option.find("2C") != string::npos )
	{
		h_TT_mu_cc = new TH1F();		h_TT_el_cc = new TH1F();
		h_DY_mu_cc = new TH1F();		h_DY_el_cc = new TH1F();
		h_WJets_mu_cc = new TH1F();	h_WJets_el_cc = new TH1F();
		h_VV_mu_cc = new TH1F();		h_VV_el_cc = new TH1F();
		h_ST_mu_cc = new TH1F();		h_ST_el_cc = new TH1F();
		h_QCD_mu_cc = new TH1F();		h_QCD_el_cc = new TH1F();
		h_Data_mu_cc = new TH1F();		h_Data_el_cc = new TH1F();
		h_l_TT_mu_cc = new TH1F();		h_l_TT_el_cc = new TH1F();
		h_l_DY_mu_cc = new TH1F();		h_l_DY_el_cc = new TH1F();
		h_l_WJets_mu_cc = new TH1F();	h_l_WJets_el_cc = new TH1F();
		h_l_VV_mu_cc = new TH1F();		h_l_VV_el_cc = new TH1F();
		h_l_ST_mu_cc = new TH1F();		h_l_ST_el_cc = new TH1F();
		h_l_QCD_mu_cc = new TH1F();	h_l_QCD_el_cc = new TH1F();
		h_l_Data_mu_cc = new TH1F();	h_l_Data_el_cc = new TH1F();

		f->GetObject("h_TT_mu_cc",h_TT_mu_cc);
		f->GetObject("h_DY_mu_cc",h_DY_mu_cc);
		f->GetObject("h_WJets_mu_cc",h_WJets_mu_cc);
		f->GetObject("h_VV_mu_cc",h_VV_mu_cc);
		f->GetObject("h_ST_mu_cc",h_ST_mu_cc);
		f->GetObject("h_QCD_mu_cc",h_QCD_mu_cc);
		f->GetObject("h_Data_mu_cc",h_Data_mu_cc);

		f->GetObject("h_TT_el_cc",h_TT_el_cc);
		f->GetObject("h_DY_el_cc",h_DY_el_cc);
		f->GetObject("h_WJets_el_cc",h_WJets_el_cc);
		f->GetObject("h_VV_el_cc",h_VV_el_cc);
		f->GetObject("h_ST_el_cc",h_ST_el_cc);
		f->GetObject("h_QCD_el_cc",h_QCD_el_cc);
		f->GetObject("h_Data_el_cc",h_Data_el_cc);

		f->GetObject("h_l_TT_mu_cc",h_l_TT_mu_cc);
		f->GetObject("h_l_DY_mu_cc",h_l_DY_mu_cc);
		f->GetObject("h_l_WJets_mu_cc",h_l_WJets_mu_cc);
		f->GetObject("h_l_VV_mu_cc",h_l_VV_mu_cc);
		f->GetObject("h_l_ST_mu_cc",h_l_ST_mu_cc);
		f->GetObject("h_l_QCD_mu_cc",h_l_QCD_mu_cc);
		f->GetObject("h_l_Data_mu_cc",h_l_Data_mu_cc);

		f->GetObject("h_l_TT_el_cc",h_l_TT_el_cc);
		f->GetObject("h_l_DY_el_cc",h_l_DY_el_cc);
		f->GetObject("h_l_WJets_el_cc",h_l_WJets_el_cc);
		f->GetObject("h_l_VV_el_cc",h_l_VV_el_cc);
		f->GetObject("h_l_ST_el_cc",h_l_ST_el_cc);
		f->GetObject("h_l_QCD_el_cc",h_l_QCD_el_cc);
		f->GetObject("h_l_Data_el_cc",h_l_Data_el_cc);

		InitTwoCutVector();
	}
	if( (option.find("NC") == string::npos) && (option.find("1C") == string::npos) && (option.find("2C") == string::npos) )
	{
		cerr << "Illegal option of Hists::GetObjs(\"file_name\",\"option\") ," << endl \
			 << "option should include NC/1C/2C " << endl;
	}
}

void Hists::InitNoCutVector()
{
	h_mu.push_back(h_TT_mu);		h_mu.push_back(h_DY_mu);
	h_mu.push_back(h_WJets_mu);		h_mu.push_back(h_VV_mu);
	h_mu.push_back(h_ST_mu);		h_mu.push_back(h_QCD_mu);
	h_mu.push_back(h_Data_mu);

	h_el.push_back(h_TT_el);		h_el.push_back(h_DY_el);
	h_el.push_back(h_WJets_el);		h_el.push_back(h_VV_el);
	h_el.push_back(h_ST_el);		h_el.push_back(h_QCD_el);
	h_el.push_back(h_Data_el);
	
	h_l_mu.push_back(h_l_TT_mu);		h_l_mu.push_back(h_l_DY_mu);
	h_l_mu.push_back(h_l_WJets_mu);		h_l_mu.push_back(h_l_VV_mu);
	h_l_mu.push_back(h_l_ST_mu);		h_l_mu.push_back(h_l_QCD_mu);
	h_l_mu.push_back(h_l_Data_mu);
	
	h_l_el.push_back(h_l_TT_el);		h_l_el.push_back(h_l_DY_el);
	h_l_el.push_back(h_l_WJets_el);		h_l_el.push_back(h_l_VV_el);
	h_l_el.push_back(h_l_ST_el);		h_l_el.push_back(h_l_QCD_el);
	h_l_el.push_back(h_l_Data_el);
}

void Hists::InitOneCutVector()
{
	h_mu_c.push_back(h_TT_mu_c);		h_mu_c.push_back(h_DY_mu_c);
	h_mu_c.push_back(h_WJets_mu_c);		h_mu_c.push_back(h_VV_mu_c);
	h_mu_c.push_back(h_ST_mu_c);		h_mu_c.push_back(h_QCD_mu_c);
	h_mu_c.push_back(h_Data_mu_c);

	h_el_c.push_back(h_TT_el_c);		h_el_c.push_back(h_DY_el_c);
	h_el_c.push_back(h_WJets_el_c);		h_el_c.push_back(h_VV_el_c);
	h_el_c.push_back(h_ST_el_c);		h_el_c.push_back(h_QCD_el_c);
	h_el_c.push_back(h_Data_el_c);
	
	h_l_mu_c.push_back(h_l_TT_mu_c);		h_l_mu_c.push_back(h_l_DY_mu_c);
	h_l_mu_c.push_back(h_l_WJets_mu_c);		h_l_mu_c.push_back(h_l_VV_mu_c);
	h_l_mu_c.push_back(h_l_ST_mu_c);		h_l_mu_c.push_back(h_l_QCD_mu_c);
	h_l_mu_c.push_back(h_l_Data_mu_c);
	
	h_l_el_c.push_back(h_l_TT_el_c);		h_l_el_c.push_back(h_l_DY_el_c);
	h_l_el_c.push_back(h_l_WJets_el_c);		h_l_el_c.push_back(h_l_VV_el_c);
	h_l_el_c.push_back(h_l_ST_el_c);		h_l_el_c.push_back(h_l_QCD_el_c);
	h_l_el_c.push_back(h_l_Data_el_c);
}


void Hists::InitTwoCutVector()
{
	h_mu_cc.push_back(h_TT_mu_cc);		h_mu_cc.push_back(h_DY_mu_cc);
	h_mu_cc.push_back(h_WJets_mu_cc);		h_mu_cc.push_back(h_VV_mu_cc);
	h_mu_cc.push_back(h_ST_mu_cc);		h_mu_cc.push_back(h_QCD_mu_cc);
	h_mu_cc.push_back(h_Data_mu_cc);

	h_el_cc.push_back(h_TT_el_cc);		h_el_cc.push_back(h_DY_el_cc);
	h_el_cc.push_back(h_WJets_el_cc);		h_el_cc.push_back(h_VV_el_cc);
	h_el_cc.push_back(h_ST_el_cc);		h_el_cc.push_back(h_QCD_el_cc);
	h_el_cc.push_back(h_Data_el_cc);
	
	h_l_mu_cc.push_back(h_l_TT_mu_cc);		h_l_mu_cc.push_back(h_l_DY_mu_cc);
	h_l_mu_cc.push_back(h_l_WJets_mu_cc);		h_l_mu_cc.push_back(h_l_VV_mu_cc);
	h_l_mu_cc.push_back(h_l_ST_mu_cc);		h_l_mu_cc.push_back(h_l_QCD_mu_cc);
	h_l_mu_cc.push_back(h_l_Data_mu_cc);
	
	h_l_el_cc.push_back(h_l_TT_el_cc);		h_l_el_cc.push_back(h_l_DY_el_cc);
	h_l_el_cc.push_back(h_l_WJets_el_cc);		h_l_el_cc.push_back(h_l_VV_el_cc);
	h_l_el_cc.push_back(h_l_ST_el_cc);		h_l_el_cc.push_back(h_l_QCD_el_cc);
	h_l_el_cc.push_back(h_l_Data_el_cc);
}

void Hists::TH2ModeON( const int& algo_bins_No, const double& m, const double& M )
{
	double algo_v_min = m;
	double algo_v_max = M;
	h_mvamax_hadmass_mu = new TH2D("h_mvamax_hadmass_mu","",bins_No,hist_min,hist_max,algo_bins_No,algo_v_min,algo_v_max);
	h_mvamax_hadmass_el = new TH2D("h_mvamax_hadmass_el","",bins_No,hist_min,hist_max,algo_bins_No,algo_v_min,algo_v_max);
	h_mvamax_hadmass[ "mu" ] = h_mvamax_hadmass_mu;			
	h_mvamax_hadmass[ "el" ] = h_mvamax_hadmass_el;
	
	h_mvamax_lepmass_mu = new TH2D("h_mvamax_lepmass_mu","",bins_No,hist_min,hist_max,algo_bins_No,algo_v_min,algo_v_max);
	h_mvamax_lepmass_el = new TH2D("h_mvamax_lepmass_el","",bins_No,hist_min,hist_max,algo_bins_No,algo_v_min,algo_v_max);
	h_mvamax_lepmass[ "mu" ] = h_mvamax_lepmass_mu;			
	h_mvamax_lepmass[ "el" ] = h_mvamax_lepmass_el;
}

void Hists::AllCutModeON()
{
	NoCutModeON();
	OneCutModeON();
	TwoCutModeON();
}

void Hists::NoCutModeON()
{
	h_TT_mu = new TH1F("h_TT_mu","",bins_No,hist_min,hist_max);
	h_TT_el = new TH1F("h_TT_el","",bins_No,hist_min,hist_max);
	
	h_DY_mu = new TH1F("h_DY_mu","",bins_No,hist_min,hist_max);
	h_DY_el = new TH1F("h_DY_el","",bins_No,hist_min,hist_max);

	h_WJets_mu = new TH1F("h_WJets_mu","",bins_No,hist_min,hist_max);
	h_WJets_el = new TH1F("h_WJets_el","",bins_No,hist_min,hist_max);
	
	h_VV_mu = new TH1F("h_VV_mu","",bins_No,hist_min,hist_max);
	h_VV_el = new TH1F("h_VV_el","",bins_No,hist_min,hist_max);

	h_ST_mu = new TH1F("h_ST_mu","",bins_No,hist_min,hist_max);
	h_ST_el = new TH1F("h_ST_el","",bins_No,hist_min,hist_max);
	
	h_QCD_mu = new TH1F("h_QCD_mu","",bins_No,hist_min,hist_max);
	h_QCD_el = new TH1F("h_QCD_el","",bins_No,hist_min,hist_max);
	
	h_Data_mu = new TH1F("h_Data_mu",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);	
	h_Data_el = new TH1F("h_Data_el",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);

	h_l_TT_mu = new TH1F("h_l_TT_mu","",bins_No,hist_min,hist_max);
	h_l_TT_el = new TH1F("h_l_TT_el","",bins_No,hist_min,hist_max);
	
	h_l_DY_mu = new TH1F("h_l_DY_mu","",bins_No,hist_min,hist_max);
	h_l_DY_el = new TH1F("h_l_DY_el","",bins_No,hist_min,hist_max);

	h_l_WJets_mu = new TH1F("h_l_WJets_mu","",bins_No,hist_min,hist_max);
	h_l_WJets_el = new TH1F("h_l_WJets_el","",bins_No,hist_min,hist_max);
	
	h_l_VV_mu = new TH1F("h_l_VV_mu","",bins_No,hist_min,hist_max);
	h_l_VV_el = new TH1F("h_l_VV_el","",bins_No,hist_min,hist_max);

	h_l_ST_mu = new TH1F("h_l_ST_mu","",bins_No,hist_min,hist_max);
	h_l_ST_el = new TH1F("h_l_ST_el","",bins_No,hist_min,hist_max);
	
	h_l_QCD_mu = new TH1F("h_l_QCD_mu","",bins_No,hist_min,hist_max);
	h_l_QCD_el = new TH1F("h_l_QCD_el","",bins_No,hist_min,hist_max);
	
	h_l_Data_mu = new TH1F("h_l_Data_mu",(char*)AxisInfo2.c_str(),bins_No,hist_min,hist_max);
	h_l_Data_el = new TH1F("h_l_Data_el",(char*)AxisInfo2.c_str(),bins_No,hist_min,hist_max);

	InitNoCutVector();
}

void Hists::OneCutModeON()
{
	h_TT_mu_c = new TH1F("h_TT_mu_c","",bins_No,hist_min,hist_max);
	h_TT_el_c = new TH1F("h_TT_el_c","",bins_No,hist_min,hist_max);
	
	h_DY_mu_c = new TH1F("h_DY_mu_c","",bins_No,hist_min,hist_max);
	h_DY_el_c = new TH1F("h_DY_el_c","",bins_No,hist_min,hist_max);

	h_WJets_mu_c = new TH1F("h_WJets_mu_c","",bins_No,hist_min,hist_max);
	h_WJets_el_c = new TH1F("h_WJets_el_c","",bins_No,hist_min,hist_max);
	
	h_VV_mu_c = new TH1F("h_VV_mu_c","",bins_No,hist_min,hist_max);
	h_VV_el_c = new TH1F("h_VV_el_c","",bins_No,hist_min,hist_max);

	h_ST_mu_c = new TH1F("h_ST_mu_c","",bins_No,hist_min,hist_max);
	h_ST_el_c = new TH1F("h_ST_el_c","",bins_No,hist_min,hist_max);
	
	h_QCD_mu_c = new TH1F("h_QCD_mu_c","",bins_No,hist_min,hist_max);
	h_QCD_el_c = new TH1F("h_QCD_el_c","",bins_No,hist_min,hist_max);
	
	h_Data_mu_c = new TH1F("h_Data_mu_c",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);	
	h_Data_el_c = new TH1F("h_Data_el_c",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);

	h_l_TT_mu_c = new TH1F("h_l_TT_mu_c","",bins_No,hist_min,hist_max);
	h_l_TT_el_c = new TH1F("h_l_TT_el_c","",bins_No,hist_min,hist_max);
	
	h_l_DY_mu_c = new TH1F("h_l_DY_mu_c","",bins_No,hist_min,hist_max);
	h_l_DY_el_c = new TH1F("h_l_DY_el_c","",bins_No,hist_min,hist_max);

	h_l_WJets_mu_c = new TH1F("h_l_WJets_mu_c","",bins_No,hist_min,hist_max);
	h_l_WJets_el_c = new TH1F("h_l_WJets_el_c","",bins_No,hist_min,hist_max);
	
	h_l_VV_mu_c = new TH1F("h_l_VV_mu_c","",bins_No,hist_min,hist_max);
	h_l_VV_el_c = new TH1F("h_l_VV_el_c","",bins_No,hist_min,hist_max);

	h_l_ST_mu_c = new TH1F("h_l_ST_mu_c","",bins_No,hist_min,hist_max);
	h_l_ST_el_c = new TH1F("h_l_ST_el_c","",bins_No,hist_min,hist_max);
	
	h_l_QCD_mu_c = new TH1F("h_l_QCD_mu_c","",bins_No,hist_min,hist_max);
	h_l_QCD_el_c = new TH1F("h_l_QCD_el_c","",bins_No,hist_min,hist_max);
	
	h_l_Data_mu_c = new TH1F("h_l_Data_mu_c",(char*)AxisInfo2.c_str(),bins_No,hist_min,hist_max);
	h_l_Data_el_c = new TH1F("h_l_Data_el_c",(char*)AxisInfo2.c_str(),bins_No,hist_min,hist_max);
	
	InitOneCutVector();
}

void Hists::TwoCutModeON()
{
	h_TT_mu_cc = new TH1F("h_TT_mu_cc","",bins_No,hist_min,hist_max);
	h_TT_el_cc = new TH1F("h_TT_el_cc","",bins_No,hist_min,hist_max);
	
	h_DY_mu_cc = new TH1F("h_DY_mu_cc","",bins_No,hist_min,hist_max);
	h_DY_el_cc = new TH1F("h_DY_el_cc","",bins_No,hist_min,hist_max);

	h_WJets_mu_cc = new TH1F("h_WJets_mu_cc","",bins_No,hist_min,hist_max);
	h_WJets_el_cc = new TH1F("h_WJets_el_cc","",bins_No,hist_min,hist_max);
	
	h_VV_mu_cc = new TH1F("h_VV_mu_cc","",bins_No,hist_min,hist_max);
	h_VV_el_cc = new TH1F("h_VV_el_cc","",bins_No,hist_min,hist_max);

	h_ST_mu_cc = new TH1F("h_ST_mu_cc","",bins_No,hist_min,hist_max);
	h_ST_el_cc = new TH1F("h_ST_el_cc","",bins_No,hist_min,hist_max);
	
	h_QCD_mu_cc = new TH1F("h_QCD_mu_cc","",bins_No,hist_min,hist_max);
	h_QCD_el_cc = new TH1F("h_QCD_el_cc","",bins_No,hist_min,hist_max);
	
	h_Data_mu_cc = new TH1F("h_Data_mu_cc",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);	
	h_Data_el_cc = new TH1F("h_Data_el_cc",(char*)AxisInfo.c_str(),bins_No,hist_min,hist_max);

	h_l_TT_mu_cc = new TH1F("h_l_TT_mu_cc","",bins_No,hist_min,hist_max);
	h_l_TT_el_cc = new TH1F("h_l_TT_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_DY_mu_cc = new TH1F("h_l_DY_mu_cc","",bins_No,hist_min,hist_max);
	h_l_DY_el_cc = new TH1F("h_l_DY_el_cc","",bins_No,hist_min,hist_max);

	h_l_WJets_mu_cc = new TH1F("h_l_WJets_mu_cc","",bins_No,hist_min,hist_max);
	h_l_WJets_el_cc = new TH1F("h_l_WJets_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_VV_mu_cc = new TH1F("h_l_VV_mu_cc","",bins_No,hist_min,hist_max);
	h_l_VV_el_cc = new TH1F("h_l_VV_el_cc","",bins_No,hist_min,hist_max);

	h_l_ST_mu_cc = new TH1F("h_l_ST_mu_cc","",bins_No,hist_min,hist_max);
	h_l_ST_el_cc = new TH1F("h_l_ST_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_QCD_mu_cc = new TH1F("h_l_QCD_mu_cc","",bins_No,hist_min,hist_max);
	h_l_QCD_el_cc = new TH1F("h_l_QCD_el_cc","",bins_No,hist_min,hist_max);
	
	h_l_Data_mu_cc = new TH1F("h_l_Data_mu_cc",(char*)AxisInfo2.c_str(),bins_No,hist_min,hist_max);
	h_l_Data_el_cc = new TH1F("h_l_Data_el_cc",(char*)AxisInfo2.c_str(),bins_No,hist_min,hist_max);

	InitTwoCutVector();
}

void
Hists::PrintOn( const string& fname, const string& option )
{
    ofstream fout;
    fout.open( fname.c_str(), ios::app );
    
	fout << "Print time : " << get_time_str( minute ) << endl;
	
	if( option.find( "NC" ) != string::npos )
    {
		double sig_y_mu = CalTool::Integral_All( h_TT_mu );
		double bkg_y_mu = CalTool::Integral_All( h_DY_mu ) + CalTool::Integral_All( h_ST_mu ) + \
					   CalTool::Integral_All( h_WJets_mu ) + CalTool::Integral_All( h_VV_mu ) + CalTool::Integral_All( h_QCD_mu );
		double sig_y_el = CalTool::Integral_All( h_TT_el );
		double bkg_y_el = CalTool::Integral_All( h_DY_el ) + CalTool::Integral_All( h_ST_el ) + \
					   CalTool::Integral_All( h_WJets_el ) + CalTool::Integral_All( h_VV_el ) + CalTool::Integral_All( h_QCD_el );

		fout << "[NC] " << endl;
		fout << "(mu) TT's yields/ratio : " << sig_y_mu << "/ " << sig_y_mu/( sig_y_mu + bkg_y_mu ) << endl;
		fout << "(mu) bkg's yields/ratio : " << bkg_y_mu << "/ " << bkg_y_mu/( sig_y_mu + bkg_y_mu ) << endl;
		
		fout << "(el) TT's yields/ratio : " << sig_y_el << "/ " << sig_y_el/( sig_y_el + bkg_y_el ) << endl;
		fout << "(el) bkg's yields/ratio : " << bkg_y_el << "/ " << bkg_y_el/( sig_y_el + bkg_y_el ) << endl;
    }
    
    if( option.find( "1C" ) != string::npos )
    {
		double sig_y_mu_c = CalTool::Integral_All( h_TT_mu_c );
		double bkg_y_mu_c = CalTool::Integral_All( h_DY_mu_c ) + CalTool::Integral_All( h_ST_mu_c ) + \
					   CalTool::Integral_All( h_WJets_mu_c ) + CalTool::Integral_All( h_VV_mu_c ) + CalTool::Integral_All( h_QCD_mu_c );
		double sig_y_el_c = CalTool::Integral_All( h_TT_el_c );
		double bkg_y_el_c = CalTool::Integral_All( h_DY_el_c ) + CalTool::Integral_All( h_ST_el_c ) + \
					   CalTool::Integral_All( h_WJets_el_c ) + CalTool::Integral_All( h_VV_el_c ) + CalTool::Integral_All( h_QCD_el_c );

		fout << "[1C] " << endl;
		fout << "(mu) TT's yields/ratio : " << sig_y_mu_c << "/ " << sig_y_mu_c/( sig_y_mu_c + bkg_y_mu_c ) << endl;
		fout << "(mu) bkg's yields/ratio : " << bkg_y_mu_c << "/ " << bkg_y_mu_c/( sig_y_mu_c + bkg_y_mu_c ) << endl;
		
		fout << "(el) TT's yields/ratio : " << sig_y_el_c << "/ " << sig_y_el_c/( sig_y_el_c + bkg_y_el_c ) << endl;
		fout << "(el) bkg's yields/ratio : " << bkg_y_el_c << "/ " << bkg_y_el_c/( sig_y_el_c + bkg_y_el_c ) << endl;
    
    }
    
    if( option.find( "2C" ) != string::npos )
    {
		double sig_y_mu_cc = CalTool::Integral_All( h_TT_mu_cc );
		double bkg_y_mu_cc = CalTool::Integral_All( h_DY_mu_cc ) + CalTool::Integral_All( h_ST_mu_cc ) + \
					   CalTool::Integral_All( h_WJets_mu_cc ) + CalTool::Integral_All( h_VV_mu_cc ) + CalTool::Integral_All( h_QCD_mu_cc );
		double sig_y_el_cc = CalTool::Integral_All( h_TT_el_cc );
		double bkg_y_el_cc = CalTool::Integral_All( h_DY_el_cc ) + CalTool::Integral_All( h_ST_el_cc ) + \
					   CalTool::Integral_All( h_WJets_el_cc ) + CalTool::Integral_All( h_VV_el_cc ) + CalTool::Integral_All( h_QCD_el_cc );

		fout << "[2C] " << endl;
		fout << "(mu) TT's yields/ratio : " << sig_y_mu_cc << "/ " << sig_y_mu_cc/( sig_y_mu_cc + bkg_y_mu_cc ) << endl;
		fout << "(mu) bkg's yields/ratio : " << bkg_y_mu_cc << "/ " << bkg_y_mu_cc/( sig_y_mu_cc + bkg_y_mu_cc ) << endl;
		
		fout << "(el) TT's yields/ratio : " << sig_y_el_cc << "/ " << sig_y_el_cc/( sig_y_el_cc + bkg_y_el_cc ) << endl;
		fout << "(el) bkg's yields/ratio : " << bkg_y_el_cc << "/ " << bkg_y_el_cc/( sig_y_el_cc + bkg_y_el_cc ) << endl;
    
    }

	fout.close();
    
}

void Hists::WriteIn( const string& option = "NC" )
{
	if( option.find("NT") != string::npos )
	{
		mvav_mass_mu->Write();
		mvav_mass_el->Write();
	}
	if( option.find("NC") != string::npos )
	{
		h_TT_mu->Write();		
		h_TT_el->Write();	
		h_DY_mu->Write();		
		h_DY_el->Write();
		h_WJets_mu->Write();	
		h_WJets_el->Write();
		h_VV_mu->Write();		
		h_VV_el->Write();
		h_ST_mu->Write();		
		h_ST_el->Write();
		h_QCD_mu->Write();		
		h_QCD_el->Write();
		h_Data_mu->Write();	
		h_Data_el->Write();

		h_l_TT_mu->Write();	
		h_l_TT_el->Write();
		h_l_DY_mu->Write();	
		h_l_DY_el->Write();
		h_l_WJets_mu->Write();	
		h_l_WJets_el->Write();
		h_l_VV_mu->Write();	
		h_l_VV_el->Write();
		h_l_ST_mu->Write();	
		h_l_ST_el->Write();
		h_l_QCD_mu->Write();	
		h_l_QCD_el->Write();
		h_l_Data_mu->Write();	
		h_l_Data_el->Write();
	}	
	
	if( option.find("1C") != string::npos )
	{
		h_TT_mu_c->Write();		
		h_TT_el_c->Write();	
		h_DY_mu_c->Write();		
		h_DY_el_c->Write();
		h_WJets_mu_c->Write();	
		h_WJets_el_c->Write();
		h_VV_mu_c->Write();		
		h_VV_el_c->Write();
		h_ST_mu_c->Write();		
		h_ST_el_c->Write();
		h_QCD_mu_c->Write();		
		h_QCD_el_c->Write();
		h_Data_mu_c->Write();	
		h_Data_el_c->Write();

		h_l_TT_mu_c->Write();	
		h_l_TT_el_c->Write();
		h_l_DY_mu_c->Write();	
		h_l_DY_el_c->Write();
		h_l_WJets_mu_c->Write();	
		h_l_WJets_el_c->Write();
		h_l_VV_mu_c->Write();	
		h_l_VV_el_c->Write();
		h_l_ST_mu_c->Write();	
		h_l_ST_el_c->Write();
		h_l_QCD_mu_c->Write();	
		h_l_QCD_el_c->Write();
		h_l_Data_mu_c->Write();	
		h_l_Data_el_c->Write();
	}

	if( option.find("2C") != string::npos )
	{
		h_TT_mu_cc->Write();		
		h_TT_el_cc->Write();	
		h_DY_mu_cc->Write();		
		h_DY_el_cc->Write();
		h_WJets_mu_cc->Write();	
		h_WJets_el_cc->Write();
		h_VV_mu_cc->Write();		
		h_VV_el_cc->Write();
		h_ST_mu_cc->Write();		
		h_ST_el_cc->Write();
		h_QCD_mu_cc->Write();		
		h_QCD_el_cc->Write();
		h_Data_mu_cc->Write();	
		h_Data_el_cc->Write();

		h_l_TT_mu_cc->Write();	
		h_l_TT_el_cc->Write();
		h_l_DY_mu_cc->Write();	
		h_l_DY_el_cc->Write();
		h_l_WJets_mu_cc->Write();	
		h_l_WJets_el_cc->Write();
		h_l_VV_mu_cc->Write();	
		h_l_VV_el_cc->Write();
		h_l_ST_mu_cc->Write();	
		h_l_ST_el_cc->Write();
		h_l_QCD_mu_cc->Write();	
		h_l_QCD_el_cc->Write();
		h_l_Data_mu_cc->Write();	
		h_l_Data_el_cc->Write();
	}
	
	if( (option.find("NC") == string::npos) && (option.find("1C") == string::npos) && (option.find("2C") == string::npos) )
	{
		cerr << "Illegal option of Hists::WriteIn(\"file_name\",\"option\") ," << endl \
			 << "option should include NC/1C/2C " << endl;
	}

}

void
Hists::FillHist( const string& option, const int& k, const string& ch, const double& hadm, const double& lepm, const double& weight )
{
	if( option.find("NC") != string::npos )
	{
		if( ch == "mu" )
		{
			h_mu.at(k)->Fill(hadm, weight);
			h_l_mu.at(k)->Fill(lepm, weight);

		}
		else if( ch == "el" )
		{
			h_el.at(k)->Fill(hadm, weight);
			h_l_el.at(k)->Fill(lepm, weight);
		}
		else
			return;
	}
    else if( option.find("1C") != string::npos )
	{
		if( ch == "mu" )
		{
			h_mu_c.at(k)->Fill(hadm, weight);
			h_l_mu_c.at(k)->Fill(lepm, weight);

		}
		else if( ch == "el" )
		{
			h_el_c.at(k)->Fill(hadm, weight);
			h_l_el_c.at(k)->Fill(lepm, weight);
		}
		else
			return;
	}
    else if( option.find("2C") != string::npos )
	{
		if( ch == "mu" )
		{
			h_mu_cc.at(k)->Fill(hadm, weight);
			h_l_mu_cc.at(k)->Fill(lepm, weight);

		}
		else if( ch == "el" )
		{
			h_el_cc.at(k)->Fill(hadm, weight);
			h_l_el_cc.at(k)->Fill(lepm, weight);
		}
		else
			return;
	}
	else
	{
		cerr << "Illegal option of Hists::WriteIn(\"file_name\",\"option\") ," << endl \
			 << "option should include NC/1C/2C " << endl;
	}
}

void DataDriven( TH1* h_bedd, TH1* h_data )
{
	double ori_entry = t_IntegralTH1( h_bedd );
	double data_entry = t_IntegralTH1( h_data );
	string ori_name = (string)h_bedd->GetName();
	h_bedd = (TH1*)h_data->Clone();
	h_bedd->Scale( ori_entry/data_entry );
	h_bedd->SetName( (char*)ori_name.c_str() );
}




void
Hists_bb::Init()
{
	//we want the binNo at had t mass is same as the mva values'
	h_mvamax_mass_mu = new TH2D("h_mvamax_mass_mu","",bins_No1,hist_min2,hist_max2,bins_No1,hist_min1,hist_max1);
	h_mvamax_mass_el = new TH2D("h_mvamax_mass_el","",bins_No1,hist_min2,hist_max2,bins_No1,hist_min1,hist_max1);
	h_mvamax_mass_t = new TH2D("h_mvamax_mass_t","",bins_No1,hist_min2,hist_max2,bins_No1,hist_min1,hist_max1);
	h_mvamax_mass[ "mu" ] = h_mvamax_mass_mu;			h_mvamax_mass[ "el" ] = h_mvamax_mass_el;
	
	h_correct_t = new TH1F("h_correct_t"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	h_mistag_t = new TH1F("h_mistag_t"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	h_charge_mis_t = new TH1F("h_charge_mis_t"," ;mva value;events",bins_No1,hist_min1,hist_max1); 

	h_correct_mu = new TH1F("h_correct_mu"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	h_mistag_mu = new TH1F("h_mistag_mu"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	h_charge_mis_mu = new TH1F("h_charge_mis_mu"," ;mva value;events",bins_No1,hist_min1,hist_max1); 
	h_correct_el = new TH1F("h_correct_el"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	h_mistag_el = new TH1F("h_mistag_el"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	h_charge_mis_el = new TH1F("h_charge_mis_el"," ;mva value;events",bins_No1,hist_min1,hist_max1);
	

	h_lt_correct_t = new TH1F("h_lt_correct_t"," ;mass;events",bins_No2,hist_min2,hist_max2);
	h_lt_mistag_t = new TH1F("h_lt_mistag_t"," ;mass;events",bins_No2,hist_min2,hist_max2);
	h_lt_charge_mis_t = new TH1F("h_lt_charge_mis_t"," ;mass;events",bins_No2,hist_min2,hist_max2); 
	
	h_lt_correct_mu = new TH1F("h_lt_correct_mu"," ;mass;events",bins_No2,hist_min2,hist_max2);
	h_lt_mistag_mu = new TH1F("h_lt_mistag_mu"," ;mass;events",bins_No2,hist_min2,hist_max2);
	h_lt_charge_mis_mu = new TH1F("h_lt_charge_mis_mu"," ;mass;events",bins_No2,hist_min2,hist_max2); 
	h_lt_correct_el = new TH1F("h_lt_correct_el"," ;mass;events",bins_No2,hist_min2,hist_max2);
	h_lt_mistag_el = new TH1F("h_lt_mistag_el"," ;mass;events",bins_No2,hist_min2,hist_max2);
	h_lt_charge_mis_el = new TH1F("h_lt_charge_mis_el"," ;mass;events",bins_No2,hist_min2,hist_max2);
	
	h_algo_lt_correct_t = new TH2D("h_algo_lt_correct_t","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	h_algo_lt_mistag_t = new TH2D("h_algo_lt_mistag_t","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	h_algo_lt_charge_mis_t = new TH2D("h_algo_lt_charge_mis_t","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);

	h_algo_lt_correct_mu = new TH2D("h_algo_lt_correct_mu","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	h_algo_lt_mistag_mu = new TH2D("h_algo_lt_mistag_mu","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	h_algo_lt_charge_mis_mu = new TH2D("h_algo_lt_charge_mis_mu","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);

	h_algo_lt_correct_el = new TH2D("h_algo_lt_correct_el","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	h_algo_lt_mistag_el = new TH2D("h_algo_lt_mistag_el","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	h_algo_lt_charge_mis_el = new TH2D("h_algo_lt_charge_mis_el","",bins_No1,hist_min1,hist_max1,bins_No2,hist_min2,hist_max2);
	
	
	h_correct[ "mu" ] = h_correct_mu;			h_correct[ "el" ] = h_correct_el;
	h_charge_mis[ "mu" ] = h_charge_mis_mu;		h_charge_mis[ "el" ] = h_charge_mis_el;
	h_mistag[ "mu" ] = h_mistag_mu;				h_mistag[ "el" ] = h_mistag_el;
	h_lt_correct[ "mu" ] = h_lt_correct_mu;				h_lt_correct[ "el" ] = h_lt_correct_el;
	h_lt_charge_mis[ "mu" ] = h_lt_charge_mis_mu;		h_lt_charge_mis[ "el" ] = h_lt_charge_mis_el;
	h_lt_mistag[ "mu" ] = h_lt_mistag_mu;				h_lt_mistag[ "el" ] = h_lt_mistag_el;
	
	h_algo_lt_correct[ "mu" ] = h_algo_lt_correct_mu;				h_algo_lt_correct[ "el" ] = h_algo_lt_correct_el;
	h_algo_lt_charge_mis[ "mu" ] = h_algo_lt_charge_mis_mu;		h_algo_lt_charge_mis[ "el" ] = h_algo_lt_charge_mis_el;
	h_algo_lt_mistag[ "mu" ] = h_algo_lt_mistag_mu;				h_algo_lt_mistag[ "el" ] = h_algo_lt_mistag_el;
}

void
Hists_bb::WriteIn()
{
	
	h_mvamax_mass_mu->Write();
	h_mvamax_mass_el->Write();
	h_mvamax_mass_t->Write();

	h_correct_t->Write();
	h_mistag_t->Write();
	h_charge_mis_t->Write();

	h_correct_mu->Write();
	h_mistag_mu->Write();
	h_charge_mis_mu->Write();

	h_correct_el->Write();
	h_mistag_el->Write();
	h_charge_mis_el->Write();

	h_lt_correct_t->Write();
	h_lt_mistag_t->Write();
	h_lt_charge_mis_t->Write();

	h_lt_correct_mu->Write();
	h_lt_mistag_mu->Write();
	h_lt_charge_mis_mu->Write();

	h_lt_correct_el->Write();
	h_lt_mistag_el->Write();
	h_lt_charge_mis_el->Write();
	
	
	h_algo_lt_correct_t->Write();
	h_algo_lt_mistag_t->Write();
	h_algo_lt_charge_mis_t->Write();

	h_algo_lt_correct_mu->Write();
	h_algo_lt_mistag_mu->Write();
	h_algo_lt_charge_mis_mu->Write();

	h_algo_lt_correct_el->Write();
	h_algo_lt_mistag_el->Write();
	h_algo_lt_charge_mis_el->Write();
}


void Hists_cor::Init()
{
	h_max_mva_mu = new TH1F("h_max_mva_mu","",bins_No1,hist_min1,hist_max1);
	h_max_mva_cor_mu = new TH1F("h_max_mva_cor_mu","",bins_No1,hist_min1,hist_max1);
	h_max_mva_incor_mu = new TH1F("h_max_mva_incor_mu","",bins_No1,hist_min1,hist_max1);
	
	h_max_mva_el = new TH1F("h_max_mva_el","",bins_No1,hist_min1,hist_max1);
	h_max_mva_cor_el = new TH1F("h_max_mva_cor_el","",bins_No1,hist_min1,hist_max1);
	h_max_mva_incor_el = new TH1F("h_max_mva_incor_el","",bins_No1,hist_min1,hist_max1);
	
	h_max_mva_t = new TH1F("h_max_mva_t","",bins_No1,hist_min1,hist_max1);
	h_max_mva_cor_t = new TH1F( "h_max_mva_cor_t","",bins_No1,hist_min1,hist_max1);
	h_max_mva_incor_t = new TH1F( "h_max_mva_incor_t","",bins_No1,hist_min1,hist_max1);
	
	h_max_mva[ "mu" ] = h_max_mva_mu;					h_max_mva[ "el" ] = h_max_mva_el;
	h_max_mva_cor[ "mu" ] = h_max_mva_cor_mu;			h_max_mva_cor[ "el" ] = h_max_mva_cor_el;
	h_max_mva_incor[ "mu" ] = h_max_mva_incor_mu;		h_max_mva_incor[ "el" ] = h_max_mva_incor_el;
	

	h_chosen_mu = new TH1F( "h_chosen_mu","",2,0.,2. );
	h_cor_mu = new TH1F( "h_cor_mu","",2,0.,2. );

	h_chosen_el = new TH1F( "h_chosen_el","",2,0.,2. );
	h_cor_el = new TH1F( "h_cor_el","",2,0.,2. );
	
	h_chosen_t = new TH1F( "h_chosen","",2,0.,2. );
	h_cor_t = new TH1F( "h_cor","",2,0.,2. );

	h_chosen[ "mu" ] = h_chosen_mu;		h_chosen[ "el" ] = h_chosen_el;
	h_cor[ "mu" ] = h_cor_mu;			h_cor[ "el" ] = h_cor_el;
}


void Hists_cor::WriteIn()
{
	h_max_mva_mu->Write();
	h_max_mva_cor_mu->Write();
	h_max_mva_incor_mu->Write();
		
	h_max_mva_el->Write();
	h_max_mva_cor_el->Write();
	h_max_mva_incor_el->Write();

	h_max_mva_t->Write();
	h_max_mva_cor_t->Write();
	h_max_mva_incor_t->Write();
	
	h_chosen_mu->Write();
	h_cor_mu->Write();
		
	h_chosen_el->Write();
	h_cor_el->Write();
	
	h_chosen_t->Write();
	h_cor_t->Write();
}


/*
void
Hists_mva::Initialize( const int& var_no )
{

    FillVec();
}

void
Hists_mva::FillVec()
{
    h_mu.push_back( h_01_mu );
    h_mu.push_back( h_02_mu );
    h_mu.push_back( h_03_mu );
    h_mu.push_back( h_04_mu );
    h_mu.push_back( h_05_mu );
    h_mu.push_back( h_06_mu );
    h_mu.push_back( h_07_mu );
    h_mu.push_back( h_08_mu );
    h_mu.push_back( h_09_mu );
    h_mu.push_back( h_10_mu );
    h_mu.push_back( h_11_mu );
    h_mu.push_back( h_12_mu );
    h_mu.push_back( h_13_mu );
    h_mu.push_back( h_14_mu );
    h_mu.push_back( h_15_mu );
    h_mu.push_back( h_16_mu );
    h_mu.push_back( h_17_mu );
    h_mu.push_back( h_18_mu );
    h_mu.push_back( h_19_mu );
    h_mu.push_back( h_20_mu );
    h_mu.push_back( h_21_mu );
    h_mu.push_back( h_22_mu );
    
    h_el.push_back( h_01_el );
    h_el.push_back( h_02_el );
    h_el.push_back( h_03_el );
    h_el.push_back( h_04_el );
    h_el.push_back( h_05_el );
    h_el.push_back( h_06_el );
    h_el.push_back( h_07_el );
    h_el.push_back( h_08_el );
    h_el.push_back( h_09_el );
    h_el.push_back( h_10_el );
    h_el.push_back( h_11_el );
    h_el.push_back( h_12_el );
    h_el.push_back( h_13_el );
    h_el.push_back( h_14_el );
    h_el.push_back( h_15_el );
    h_el.push_back( h_16_el );
    h_el.push_back( h_17_el );
    h_el.push_back( h_18_el );
    h_el.push_back( h_19_el );
    h_el.push_back( h_20_el );
    h_el.push_back( h_21_el );
    h_el.push_back( h_22_el );
}

*/
