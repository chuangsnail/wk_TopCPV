#include "TopCPViolation/select/interface/DFMgr.h"

using namespace std;


void 
DFMgr::SignDec( const string& obs, const double& det, const double& gen, const double& weight )
{
	if( det*gen > 0. )
		h_Obs[ obs ]->Fill( 0.5, weight );
	else
		h_Obs[ obs ]->Fill( 1.5, weight );
	
}

void 
DFMgr::SignDec_bb( const string& obs, const double& det, const double& gen, const double& weight )
{
	if( det*gen > 0. )
		h_bb_Obs[ obs ]->Fill( 0.5, weight );
	else
		h_bb_Obs[ obs ]->Fill( 1.5, weight );
	
}

void 
DFMgr::SignDec_bbj1( const string& obs, const double& det, const double& gen, const double& weight )
{
	if( det*gen > 0. )
		h_bbj1_Obs[ obs ]->Fill( 0.5, weight );
	else
		h_bbj1_Obs[ obs ]->Fill( 1.5, weight );
	
}

void 
DFMgr::Calculate()
{
	cor_O3 =  h_O3->GetBinContent(1)/(h_O3->GetBinContent(1) + h_O3->GetBinContent(2));
	cor_O6 =  h_O6->GetBinContent(1)/(h_O6->GetBinContent(1) + h_O6->GetBinContent(2));
	cor_O12 =  h_O12->GetBinContent(1)/(h_O12->GetBinContent(1) + h_O12->GetBinContent(2));
	cor_O13 =  h_O13->GetBinContent(1)/(h_O13->GetBinContent(1) + h_O13->GetBinContent(2));
	cor_bb_O3 =  h_bb_O3->GetBinContent(1)/(h_bb_O3->GetBinContent(1) + h_bb_O3->GetBinContent(2));
	cor_bb_O6 =  h_bb_O6->GetBinContent(1)/(h_bb_O6->GetBinContent(1) + h_bb_O6->GetBinContent(2));
	cor_bb_O12 =  h_bb_O12->GetBinContent(1)/(h_bb_O12->GetBinContent(1) + h_bb_O12->GetBinContent(2));
	cor_bb_O13 =  h_bb_O13->GetBinContent(1)/(h_bb_O13->GetBinContent(1) + h_bb_O13->GetBinContent(2));
	cor_bbj1_O3 =  h_bbj1_O3->GetBinContent(1)/(h_bbj1_O3->GetBinContent(1) + h_bbj1_O3->GetBinContent(2));
	cor_bbj1_O6 =  h_bbj1_O6->GetBinContent(1)/(h_bbj1_O6->GetBinContent(1) + h_bbj1_O6->GetBinContent(2));
	cor_bbj1_O12 =  h_bbj1_O12->GetBinContent(1)/(h_bbj1_O12->GetBinContent(1) + h_bbj1_O12->GetBinContent(2));
	cor_bbj1_O13 =  h_bbj1_O13->GetBinContent(1)/(h_bbj1_O13->GetBinContent(1) + h_bbj1_O13->GetBinContent(2));


	DF_O3 = 1.- 2 * h_O3->GetBinContent(2)/(h_O3->GetBinContent(1) + h_O3->GetBinContent(2));
	DF_O6 = 1.- 2 * h_O6->GetBinContent(2)/(h_O6->GetBinContent(1) + h_O6->GetBinContent(2));
	DF_O12 = 1.- 2 * h_O12->GetBinContent(2)/(h_O12->GetBinContent(1) + h_O12->GetBinContent(2));
	DF_O13 = 1.- 2 * h_O13->GetBinContent(2)/(h_O13->GetBinContent(1) + h_O13->GetBinContent(2));
	DF_bb_O3 = 1.- 2 * h_bb_O3->GetBinContent(2)/(h_bb_O3->GetBinContent(1) + h_bb_O3->GetBinContent(2));
	DF_bb_O6 = 1.- 2 * h_bb_O6->GetBinContent(2)/(h_bb_O6->GetBinContent(1) + h_bb_O6->GetBinContent(2));
	DF_bb_O12 = 1.- 2 * h_bb_O12->GetBinContent(2)/(h_bb_O12->GetBinContent(1) + h_bb_O12->GetBinContent(2));
	DF_bb_O13 = 1.- 2 * h_bb_O13->GetBinContent(2)/(h_bb_O13->GetBinContent(1) + h_bb_O13->GetBinContent(2));
	DF_bbj1_O3 = 1.- 2 * h_bbj1_O3->GetBinContent(2)/(h_bbj1_O3->GetBinContent(1) + h_bbj1_O3->GetBinContent(2));
	DF_bbj1_O6 = 1.- 2 * h_bbj1_O6->GetBinContent(2)/(h_bbj1_O6->GetBinContent(1) + h_bbj1_O6->GetBinContent(2));
	DF_bbj1_O12 = 1.- 2 * h_bbj1_O12->GetBinContent(2)/(h_bbj1_O12->GetBinContent(1) + h_bbj1_O12->GetBinContent(2));
	DF_bbj1_O13 = 1.- 2 * h_bbj1_O13->GetBinContent(2)/(h_bbj1_O13->GetBinContent(1) + h_bbj1_O13->GetBinContent(2));

	cor_err_O3 = GetCorErr( h_O3 );
	cor_err_O6 = GetCorErr( h_O6 );
	cor_err_O12 = GetCorErr( h_O12 );
	cor_err_O13 = GetCorErr( h_O13 );
	cor_err_bb_O3 = GetCorErr( h_bb_O3 );
	cor_err_bb_O6 = GetCorErr( h_bb_O6 );
	cor_err_bb_O12 = GetCorErr( h_bb_O12 );
	cor_err_bb_O13 = GetCorErr( h_bb_O13 );
	cor_err_bbj1_O3 = GetCorErr( h_bbj1_O3 );
	cor_err_bbj1_O6 = GetCorErr( h_bbj1_O6 );
	cor_err_bbj1_O12 = GetCorErr( h_bbj1_O12 );
	cor_err_bbj1_O13 = GetCorErr( h_bbj1_O13 );

	DF_err_O3 = 2 * cor_err_O3;
	DF_err_O6 = 2 * cor_err_O6;
	DF_err_O12 = 2 * cor_err_O12;
	DF_err_O13 = 2 * cor_err_O13;
	DF_err_bb_O3 = 2 * cor_err_bb_O3;
	DF_err_bb_O6 = 2 * cor_err_bb_O6;
	DF_err_bb_O12 = 2 * cor_err_bb_O12;
	DF_err_bb_O13 = 2 * cor_err_bb_O13;
	DF_err_bbj1_O3 = 2 * cor_err_bbj1_O3;
	DF_err_bbj1_O6 = 2 * cor_err_bbj1_O6;
	DF_err_bbj1_O12 = 2 * cor_err_bbj1_O12;
	DF_err_bbj1_O13 = 2 * cor_err_bbj1_O13;

}

void
DFMgr::WriteIn( const string& option )
{
	if( option.find( "N" ) != string::npos ) {
		h_O3->Write();
		h_O6->Write();
		h_O12->Write();
		h_O13->Write();
	}
	
	if( option.find( "bb" ) != string::npos ) {
		h_bb_O3->Write();
		h_bb_O6->Write();
		h_bb_O12->Write();
		h_bb_O13->Write();
	}
	
	if( option.find( "bbj1" ) != string::npos ) {
		h_bbj1_O3->Write();
		h_bbj1_O6->Write();
		h_bbj1_O12->Write();
		h_bbj1_O13->Write();
	}
}


void
DFMgr::PrintIn( ofstream& f )
{
	
	f << "Dilution Factor ( N condition )" << endl;
	f << setw(25) << left << "DF O3:" << right << DF_O3 << "+-" << DF_err_O3 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_O3 << "+-" << cor_err_O3 << endl;
	f << setw(25) << left << "DF O6:" << right << DF_O6 << "+-" << DF_err_O6 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_O6 << "+-" << cor_err_O6 << endl;
	f << setw(25) << left << "DF O12:" << right << DF_O12 << "+-" << DF_err_O12 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_O12 << "+-" << cor_err_O12 << endl;
	f << setw(25) << left << "DF O13:" << right << DF_O13 << "+-" << DF_err_O13 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_O13 << "+-" << cor_err_O13 << endl;
	f << "Dilution Factor ( bb condition )" << endl;
	f << setw(25) << left << "DF O3:" << right << DF_bb_O3 << "+-" << DF_err_bb_O3 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bb_O3 << "+-" << cor_err_bb_O3 << endl;
	f << setw(25) << left << "DF O6:" << right << DF_bb_O6 << "+-" << DF_err_bb_O6 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bb_O6 << "+-" << cor_err_bb_O6 << endl;
	f << setw(25) << left << "DF O12:" << right << DF_bb_O12 << "+-" << DF_err_bb_O12 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bb_O12 << "+-" << cor_err_bb_O12 << endl;
	f << setw(25) << left << "DF O13:" << right << DF_bb_O13 << "+-" << DF_err_bb_O13 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bb_O13 << "+-" << cor_err_bb_O13 << endl;
	f << "Dilution Factor ( bb + j1 condition )" << endl;
	f << setw(25) << left << "DF O3:" << right << DF_bbj1_O3 << "+-" << DF_err_bbj1_O3 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bbj1_O3 << "+-" << cor_err_bbj1_O3 << endl;
	f << setw(25) << left << "DF O6:" << right << DF_bbj1_O6 << "+-" << DF_err_bbj1_O6 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bbj1_O6 << "+-" << cor_err_bbj1_O6 << endl;
	f << setw(25) << left << "DF O12:" << right << DF_bbj1_O12 << "+-" << DF_err_bbj1_O12 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bbj1_O12 << "+-" << cor_err_bbj1_O12 << endl;
	f << setw(25) << left << "DF O13:" << right << DF_bbj1_O13 << "+-" << DF_err_bbj1_O13 << " || " << setw(25) << left << "Cor Ratio:" << right << cor_bbj1_O13 << "+-" << cor_err_bbj1_O13 << endl;

}

double
DFMgr::GetCorErr( TH1* h )
{
	double Ns = h->GetBinContent(1);
	double Nw = h->GetBinContent(2);
	double Es = h->GetBinError(1);
	double Ew = h->GetBinError(2);

	return sqrt( pow(Ns,2)/pow(Ns+Nw,4)*pow(Ew,2) + pow(Nw,2)/pow(Ns+Nw,4)*pow(Es,2));
}

