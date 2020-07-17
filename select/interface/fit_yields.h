
#ifndef _FIT_YIELDS_H_
#define _FIT_YIELDS_H_

#include "RooFit_class.h"
#include "frameDef.h"


using namespace std;
using namespace RooFit;

void Get_fit_result( const string& condition, const string& NOMSIGroot, const string& NOMBKGroot,\
	   	const string& SNGroot, const string& BKGroot, const string& channel,\
	   	const string& obs_name, const int& seed = 1, const int& set_no = 200 );
void FIT_1( TH1D* h_PD, TH1D* h_signal, TH1D* h_B, double* yields, const string& pdf_name );
TH2D* generate( TRandom3& rnd, TH2D* h_ori, const int& required_no );
double* Make_cdf_Grid( TH2D* h_ori );
TH2D* generate_2( TRandom3& rnd, TH2D* h_ori, const int& required_no, const double* grid );
double GetTH2BinContent( TH2* h, double _x, double _y );
double Ratio_Minus( TH1D* h );
void RYields_RMinus_150cut( TH2D* h, double* par );
void Draw_templates( TH2D* h_PD, TH2D* h_signal, TH2D* h_B );

#define Normalize(h)                        \
h->Scale( 1./(h->Integral(1,h->GetXaxis()->GetNbins()+1)) );

#define IntegralAll(h)  \
h->Integral(0,h->GetNbinsX()+1);


#endif
