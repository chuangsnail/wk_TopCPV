#include <iostream>
#include <string>
#include "string.h"
#include <fstream>

#define YBound 1.0
string obs_name[4] = { "O_{3}", "O_{6}", "O_{12}", "O_{13}" };
string y_title = "A'_{cp} [%]";

void Calculate( const string& );
bool GetOne( const string& filename, double* mean, double* err, const string& channel );
void Plot( TH1D* h_acp, TH1D* h_error1, TH1D* h_error2, const string& filename, const string& description, const string& channel );
double GetAcpError( TH1D* p, TH1D* n, int bin );

void exe()
{
	Calculate( "mu" );
	Calculate( "el" );
	Calculate( "t" );
}

void Calculate( const string& channel = "t" )
{
	//string out = "a05_all_MLP_SRtt_ChangeInfo";
	//string out = "a05_all_MLP_ChangeInfo_washout";
	//string out = "a05_all_MLP_fakeData_ChangeInfo";		// the uncertainty would be quoted as detector and reconstruction bias
	//string out = "chi2_fakeData_ChangeInfo";
	string out = "a05_all_MLP_MlbNo_fakeData_ChangeInfo";

	ofstream f;

	//(1) tt MC use changeInfo
   	//f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/SRtt_ChangeInfo.txt", ios::app );	
   	
	//(2) wash out the artificial A_cp
	//f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/washout.txt", ios::app );	
   	
	//(3) mva 0.22, Mlb 150 cut
	//f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/fake_data.txt", ios::app );	
	
	//(4) chi2 20, Mlb 150 cut
	//f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/fake_data_chi2.txt", ios::app );	
	
	//(5) chi2 20, Mlb 150 cut
	f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/fake_data_a05_MLP_Mlb-No.txt", ios::app );	
	

	int pass = 0;
	double mean[4] = {0., 0., 0., 0.};
	double error[4] = {0., 0., 0., 0.};
	for(int i=2;i<1000;++i)
	{
		string NO = to_string( i );
		//string name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/1000sample/a05_all_MLP_ChangeInfo_Sample-" + NO;
		//string name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/washout/a05_all_MLP_ChangeInfo_Sample-" + NO;
		//string name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/fake_data/a05_all_MLP_ChangeInfo_Sample-" + NO;
		//string name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/fake_data_chi2_Mlb-150/chi2_ChangeInfo_Sample-" + NO;
		string name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/fake_data_a05_MLP_Mlb-No/a05_MLP_ChangeInfo_Sample-" + NO;
		
		if( GetOne( name, mean, error, channel ) ) pass++;
	}
	for(int i=0;i<4;++i)
	{
		mean[i] /= (double)pass;
		error[i] /= (double)pass;
	}
	
	TH1D* h_acp = new TH1D( "h_acp", "", 4, 0, 4 );
	TH1D* h_error1 = new TH1D( "h_error1", "", 4, 0, 4 );	
	TH1D* h_error2 = new TH1D( "h_error2", "", 4, 0, 4 );

	for(int i=0;i<4;++i)
	{
		//double binerr = num->GetBinError(i+2);
		double bincont = mean[i];
		double binerr = error[i];
		h_acp->SetBinContent( i+1, bincont*100 );
		h_acp->SetBinError( i+1, binerr*100 );
		h_error1->SetBinContent( i+1, bincont*100 );
		h_error1->SetBinError( i+1, binerr*100 );
		h_error2->SetBinContent( i+1, bincont*100 );
		h_error2->SetBinError( i+1, binerr*200 );

		f << i << "'s mean : " << bincont << ", error : " << binerr << endl;
	}
	f << "==============================================================" << endl;
   	f.close();	

	Plot( h_acp, h_error1, h_error2, (char*)out.c_str(), "Acp", channel );
}

bool GetOne( const string& filename, double* mean, double* err, const string& channel )
{
	string p_name = "h_Obs_" + channel + "_P";
	string n_name = "h_Obs_" + channel + "_N";
	
	TFile* f = new TFile( (char*)( filename + string(".root") ).c_str() );

	if( f->IsZombie() )
		return false;
	
	TH1D* h_pos;
	TH1D* h_neg;

	cout << filename << endl;	
	//cout << "gg" << endl;

	f->GetObject( (char*)p_name.c_str(), h_pos );
	f->GetObject( (char*)n_name.c_str(), h_neg );

	TH1D* num = (TH1D*)h_pos->Clone();
	TH1D* den = (TH1D*)h_pos->Clone();
	
	//optional
	for(int i=0;i<4;i++)
		err[i] += GetAcpError( num, den, i+2 );

	num->Add( h_neg, -1 );
	den->Add( h_neg );

	num->Divide( den );

	for(int i=0;i<4;++i)
		mean[i] += num->GetBinContent(i+2);

	f->Close();

	return true;
}


void Plot( TH1D* h_acp, TH1D* h_error1, TH1D* h_error2 , const string& filename, const string& description, const string& channel )
{
	TCanvas* c = new TCanvas();

	h_acp->SetStats( false );
	h_error1->SetStats( false );
	h_error2->SetStats( false );
	h_error1->SetFillColor( kGreen );
	h_error2->SetFillColor( kYellow );

	h_acp->SetMaximum( YBound );
	h_acp->SetMinimum( -1 * YBound );

	for(int i=0;i<4;i++){
		h_acp->GetXaxis()->SetBinLabel( i+1, (char*)obs_name[i].c_str() );
	}
	h_acp->GetYaxis()->SetTitle( (char*)y_title.c_str() );
	
	TLegend* l = new TLegend(0.6,0.7,0.85,0.85);
	l->AddEntry( (TObject*)0,"Simulation", "" );
	if( channel != "t" )
		l->AddEntry( (TObject*)0, (char*)( string("ch-") + channel ).c_str() , "" );
	l->AddEntry( h_acp, "Nominal value", "LEP");
	l->AddEntry( h_error1, "1#sigma stat. error", "F");
	l->AddEntry( h_error2, "2#sigma stat. error", "F");
	l->SetBorderSize( 0 );
	

	TLine* l1 = new TLine(0,0,4,0);

	h_acp->Draw();
	h_error2->Draw( "E2 SAME" );
	h_error1->Draw( "E2 SAME" );
	h_acp->Draw( "EP SAME" );
	l->Draw("SAME");
	l1->Draw("SAME");

	string pdfname = description + "_" + filename + "_" + channel + ".pdf";
	c->Print( (char*)pdfname.c_str() );
	
}

double GetAcpError( TH1D* p, TH1D* n, int bin )
{
	double Np =  p->GetBinContent( bin );
	double Nn =  n->GetBinContent( bin );
	return sqrt( ( 4 * Np * Nn ) / pow( (Np + Nn), 3 ) );
}

