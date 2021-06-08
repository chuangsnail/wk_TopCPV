#include "TopCPViolation/select/interface/fit_yields.h"

using namespace std;
using namespace RooFit;

void
Get_fit_result( const string& condition, const string& NOMSIGroot, const string& NOMBKGroot,\
	   	const string& SNGroot, const string& BKGroot, const string& channel,\
	   	const string& obs_name, const int& seed, const int& set_no, const string& strategy )
{
    //channel == "el" or "mu"
    //obs_name == "O3" or "O6" ...

    string output_name = condition + "_" + channel + "_" + obs_name;

    TFile* fns = new TFile( (char*)NOMSIGroot.c_str() );
    TFile* fnb = new TFile( (char*)NOMBKGroot.c_str() ); //used to data-driven the bkg template
    TFile* fs = new TFile( (char*)SNGroot.c_str() );
    TFile* fb = new TFile( (char*)BKGroot.c_str() );

    string sig_temp_name = "h_Mlb_" + obs_name + "_" + channel;
    string bkg_temp_name = "h_Mlb_" + obs_name + "_" + channel;

    TH2D* h2_nomsig_tp  = (TH2D*)( fns->Get( sig_temp_name.c_str() )->Clone() );
    h2_nomsig_tp->SetName("h2_nomsig_tp");
    TH2D* h2_nombkg_tp  = (TH2D*)( fnb->Get( bkg_temp_name.c_str() )->Clone() );
    h2_nombkg_tp->SetName("h2_nombkg_tp");
    TH2D* h2_sig_tp  = (TH2D*)( fs->Get( sig_temp_name.c_str() )->Clone() );    //could be up, down
    h2_sig_tp->SetName("h2_sig_tp");
    TH2D* h2_bkg_tp  = (TH2D*)( fb->Get( bkg_temp_name.c_str() )->Clone() );
    h2_bkg_tp->SetName("h2_bkg_tp");

    // check template
/*
    TH1D* pdf1 = (TH1D*)h2_nomsig_tp->ProjectionX()->Clone();
    TH1D* pdf2 = (TH1D*)h2_bkg_tp->ProjectionX()->Clone();

    Normalize(pdf1);
    Normalize(pdf2);

    pdf1->SetLineColor(50);
    pdf2->SetLineColor(8);

    TCanvas* c = new TCanvas();

    pdf1->Draw("HIST");
    pdf2->Draw("HIST same");

	c->Print( (condition + "_test.pdf").c_str() );
*/
    // -- data-driven the CR-data to SR-bkg -- //

    double n1 = (double)IntegralAll( h2_bkg_tp->ProjectionX() );
    double n2 = (double)IntegralAll( h2_nombkg_tp->ProjectionX() );

    h2_bkg_tp->Scale( (double)n2/n1 );

    // -- make PD's template -- //
    TH2D* h2_Data_tp = (TH2D*)h2_nomsig_tp->Clone();
    h2_Data_tp->Add( h2_bkg_tp );

    // -- 
    double data_exp_No = IntegralAll( h2_Data_tp->ProjectionX() );
    double bkg_exp_No = IntegralAll( h2_bkg_tp->ProjectionX() );
    //cout << "data_exp_No : " << data_exp_No << endl;
    //cout << "bkg_exp_No : " << bkg_exp_No << endl;

//    return;
	
	// -- Make PD's and B's cdf grid -- //
	// used to generate pdf
	
	double* PD_grid = Make_cdf_Grid( h2_Data_tp );
	double* B_grid = Make_cdf_Grid( h2_bkg_tp );

	// -- start looping -- //
	
	TRandom3 rnd( seed );
	for(int i=0;i<set_no;++i)
	{
		TH2D* h_PD; //= new TH2D();
		TH2D* h_B; //= new TH2D();
		h_PD = generate_2( rnd, h2_Data_tp, rnd.Poisson(data_exp_No), PD_grid );
		h_PD->SetName("h_PD");
		h_B = generate_2( rnd, h2_bkg_tp, rnd.Poisson(bkg_exp_No), B_grid );
		h_B->SetName("h_B");

		string hist_f_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/hist/" + strategy + "/" + output_name +  "_" + to_string(i) + ".root";
		TFile* f_tmp = new TFile( hist_f_name.c_str() ,"recreate");
		h_PD->Write();
		h2_sig_tp->Write();
		h_B->Write();
		f_tmp->Close();

		//use h_sig_tp and h_B to fit h_PD 

		double yields[2] = {0., 0.};

		string pdf_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/yields_pdf/" + strategy + "/" + output_name + "_" + to_string(i) + ".pdf";

		FIT_1( h_PD->ProjectionX(), h2_sig_tp->ProjectionX(), h_B->ProjectionX(), yields, pdf_name );

		//cout << "data yields : " << yields[0] + yields[1] <<  ", data Obs ratio (-) : " << Ratio_Minus( h_PD->ProjectionY() ) << endl;
		//cout << "bkg yields : " << yields[1] <<  ", bkg Obs ratio (-) : " << Ratio_Minus( h_B->ProjectionY() ) << endl;
		
		// -- after cut at Mlb == 150 -- //

		double PD_yr_cut[2] = {0.,0.};
		double B_yr_cut[2] = {0.,0.};
		RYields_RMinus_150cut( h_PD, PD_yr_cut );
		RYields_RMinus_150cut( h_B, B_yr_cut );

		PD_yr_cut[0] = PD_yr_cut[0] * ( yields[0] + yields[1] );
		B_yr_cut[0] = B_yr_cut[0] * yields[1];

		ofstream fout;
		fout.open(  ( string("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/Uncertainty_fit/result/") + strategy + "/"  + output_name + string(".txt") ).c_str() ,ios::app );

		fout << yields[0] + yields[1] << "," << Ratio_Minus( h_PD->ProjectionY() ) << ",";
		fout << yields[1] << "," << Ratio_Minus( h_B->ProjectionY() ) << ",";

		fout <<  PD_yr_cut[0] << "," << PD_yr_cut[1] << ",";
		fout <<  B_yr_cut[0] << "," << B_yr_cut[1] << endl;

		fout.close();

		// 'new' in function generate(), so delete it
		delete h_B;
		delete h_PD;
	}

	delete [] B_grid;
	delete [] PD_grid;
}


void FIT_1( TH1D* h_PD, TH1D* h_signal, TH1D* h_B, double* yields, const string& pdf_name )
{   
	RooMsgService::instance().setGlobalKillBelow(RooFit::FATAL);

    //RooRealVar x( "x", "M_{lb}", 0., 500. );
    RooRealVar x( "x", "M_{lb}", h_PD->GetXaxis()->GetXmin(), h_PD->GetXaxis()->GetXmax() );

    // signal template
    
    TH1D* h_sig = (TH1D*)h_signal->Clone();
    Normalize(h_sig);
    h_sig->SetDirectory( 0 );

    RooDataHist* dh1 = new RooDataHist( "dh1", "dh", x, Import( *h_sig ) );
    RooHistPdf sg( "sg", "sg", x, *dh1 );

    // background template
    
    TH1D* h_bkg = (TH1D*)h_B->Clone();
    Normalize(h_bkg);
    h_bkg->SetDirectory( 0 );

    RooDataHist* dh2 = new RooDataHist( "dh2", "dh2", x, Import( *h_bkg ) );
    RooHistPdf bg( "bg", "bg", x, *dh2 );

    // Construct composite pdf
    RooRealVar nsg( "nsg", "nsg", 120000, 10000., 400000. );
    RooRealVar nbg( "nbg", "nbg", 4000, 0., 40000. );
    RooAddPdf model( "model", "model", RooArgList( sg, bg ), RooArgList( nsg, nbg ) );

    // data
    
    TH1D* h_data = (TH1D*)h_PD->Clone();
    h_data->SetDirectory( 0 );

    RooDataHist* data = new RooDataHist( "data", "data", x, Import( *h_data ) );

    // fitting
    model.fitTo( *data, Extended( true ) );
    RooFitResult* result = model.fitTo( *data, Save( true ), Extended( true ) );

    yields[0] = nsg.getValV();
    yields[1] = nbg.getValV();

/*
    //--- for fitting error ---//

    TH1D* h1_up = (TH1D*)h1->Clone();
    TH1D* h2_up = (TH1D*)h2->Clone();
    TH1D* h1_down = (TH1D*)h1->Clone();
    TH1D* h2_down = (TH1D*)h2->Clone();

    h1_up->Scale( ( nsg.getError() + nsg.getValV() ) / nsg.getValV() );
    h2_up->Scale( ( nbg.getError() + nbg.getValV() ) / nbg.getValV() );

    h1_down->Scale( ( nsg.getError() - nsg.getValV() ) / nsg.getValV() );
    h2_down->Scale( ( nbg.getError() - nbg.getValV() ) / nbg.getValV() );
*/

	//-- Drawing --//
/*

    TCanvas* c = new TCanvas();
	TGaxis::SetMaxDigits(3);

    double errors[2] = { nsg.getError(), nbg.getError() };

    h_sig->Scale( yields[0] );     //if normalized
    h_bkg->Scale( yields[1] );

	const TMatrixDSym& cor = result->correlationMatrix() ;
	const TMatrixDSym& cov = result->covarianceMatrix() ;

	double ratio_fit_err_mean = 0.;
	double fit_total_yields_stat_error = 0.;
	fit_total_yields_stat_error = sqrt( cov(0,0) + cov(0,0) + 2* cov(0,1) );
	ratio_fit_err_mean = fit_total_yields_stat_error/(yields[0]+yields[1]);
	double fit_signal_ratio_stat_error = 0.;
	fit_signal_ratio_stat_error = sqrt( pow(yields[0],2)*cov(1,1) + pow(yields[1],2)*cov(0,0) - 2* yields[0] * yields[1] * cov(1,0)  )/pow((yields[0]+yields[1]),2);

	TH1D* mc_sum = (TH1D*)h_sig->Clone();
	mc_sum->Add( h_bkg );
	for(int i=1;i<mc_sum->GetNbinsX();++i) {
		//cout << i << "  " << mc_sum->GetBinContent(i) << "  " << ratio_fit_err_mean << endl;
		mc_sum->SetBinError( i, mc_sum->GetBinContent(i)*ratio_fit_err_mean );	
	}

    mc_sum->SetMarkerSize(0);
    mc_sum->SetFillStyle(3244);
    mc_sum->SetFillColor(16);

    THStack* mc = new THStack( "mc", ";M_{lb} [GeV];Events" );
    h_sig->SetLineColor( kBlue-9 );
    h_sig->SetFillColor( kBlue-9 );
    h_bkg->SetLineColor( kRed-9 );
    h_bkg->SetFillColor( kRed-9 );
    mc->Add( h_bkg );
    mc->Add( h_sig );

	string test_sig_y = "Signal yields:" + to_string( yields[0] ) + "+-" + to_string( fit_total_yields_stat_error );
	string test_bkg_y = "Background yields:" + to_string( yields[1] );
	string text_sig_ratio = "Signal ratio:" + to_string( yields[0]/( yields[0] + yields[1] ) ) + "+-" + to_string( fit_signal_ratio_stat_error ) + "%";

    TLegend* leg = new TLegend( 0.65, 0.41, 0.8, 0.81 );
    leg->SetLineColor( kWhite );
    leg->AddEntry( h_data, "Data",              "LE" );
    leg->AddEntry( h_sig, "Fitted signal (t#bar{t})",   "F" );
    leg->AddEntry( h_bkg, "Fitted background", "F" );
    leg->AddEntry( mc_sum, "fitted 1#sigma Stat. err" );
	leg->AddEntry((TObject*)0, test_sig_y.c_str(),"");
	leg->AddEntry((TObject*)0, test_bkg_y.c_str(),"");

    mc->SetMaximum(mc->GetMaximum()*1.3);

    mc->Draw( "HIST" );
    mc_sum->Draw( "E2 same" );
    h_data->Draw( "EP same" );
    leg->Draw( "same" );
    
    c->Print( pdf_name.c_str() );

*/
}

// with Monte Carlo method
// use a pdf template (h_ori) and yields to generate a new TH2 sample
TH2D* generate( TRandom3& rnd, TH2D* h_ori, const int& required_no )
{
    TH2D* h_gen = new TH2D("h_gen","", h_ori->GetNbinsX(), h_ori->GetXaxis()->GetXmin(),h_ori->GetXaxis()->GetXmax(), h_ori->GetNbinsY(), h_ori->GetYaxis()->GetXmin(),h_ori->GetYaxis()->GetXmax());

    //cout << "X range/min " << h_ori->GetXaxis()->GetXmax() - h_ori->GetXaxis()->GetXmin()  << "/" << h_ori->GetXaxis()->GetXmin() << endl;
    //cout << "Y range/min " << h_ori->GetYaxis()->GetXmax() - h_ori->GetYaxis()->GetXmin()  << "/" << h_ori->GetYaxis()->GetXmin() << endl;

    int pass_no = 0;
    while( pass_no < required_no )
    {
        double X = rnd.Rndm() * ( h_ori->GetXaxis()->GetXmax() - h_ori->GetXaxis()->GetXmin() );
        double Y = ( rnd.Rndm() - 0.5 ) * ( h_ori->GetYaxis()->GetXmax() - h_ori->GetYaxis()->GetXmin() );
        double Z = rnd.Rndm() * ( h_ori->GetMaximum() * 1.01 ) ;
        int x_label = ( X - h_ori->GetXaxis()->GetXmin() )/h_ori->GetXaxis()->GetBinWidth(1) + 1;
        int y_label = ( Y - h_ori->GetYaxis()->GetXmin() )/h_ori->GetYaxis()->GetBinWidth(1) + 1;

        if( Z < h_ori->GetBinContent(x_label, y_label) )
        {
            h_gen->Fill(X,Y);
            pass_no++;
        }
    }

    return h_gen;
}

double* Make_cdf_Grid( TH2D* h_ori )
{
	double total = h_ori->ProjectionX()->Integral( 0, h_ori->GetNbinsX()+1 );
	int Nbin_X = h_ori->GetNbinsX() + 2;	// 0 ~ h_ori->GetNbinsX()+1
	int Nbin_Y = h_ori->GetNbinsY() + 2;
	int grid_no = Nbin_X * Nbin_Y;

	double* grid = new double[ grid_no ];

	for(int i=0;i<Nbin_X;++i)
	{
		for(int j=0;j<Nbin_Y;++j)
		{	
			int pos = i * Nbin_Y + j;
			grid[pos] = 0.;
		}	
	}

	double tmp = 0.;
	for(int i=0;i<Nbin_X;++i)
	{
		for(int j=0;j<Nbin_Y;++j)
		{	
			int pos = i * Nbin_Y + j;
			grid[pos] = tmp;
			//cout << "(" << i << "," << j << ") : " << tmp << endl;
			tmp += h_ori->GetBinContent( i, j )/total;
		}	
	}

	return grid;
	// need to be delete outside the function
}


TH2D* generate_2( TRandom3& rnd, TH2D* h_ori, const int& required_no, const double* grid )
{
    TH2D* h_gen = new TH2D("h_gen","", h_ori->GetNbinsX(), h_ori->GetXaxis()->GetXmin(),h_ori->GetXaxis()->GetXmax(), h_ori->GetNbinsY(), h_ori->GetYaxis()->GetXmin(),h_ori->GetYaxis()->GetXmax());

	int Nbin_X = h_ori->GetNbinsX() + 2;	// 0 ~ h_ori->GetNbinsX()+1
	int Nbin_Y = h_ori->GetNbinsY() + 2;
	int grid_no = Nbin_X * Nbin_Y;

/*
	double* grid = new double[ grid_no ];

	for(int i=0;i<Nbin_X;++i)
	{
		for(int j=0;j<Nbin_Y;++j)
		{	
			int pos = i * Nbin_Y + j;
			grid[pos] = 0.;
		}	
	}

	double tmp = 0.;
	for(int i=0;i<Nbin_X;++i)
	{
		for(int j=0;j<Nbin_Y;++j)
		{	
			int pos = i * Nbin_Y + j;
			grid[pos] = tmp;
			//cout << "(" << i << "," << j << ") : " << tmp << endl;
			tmp += h_ori->GetBinContent( i, j )/total;
		}	
	}
*/

    int pass_no = 0;
    while( pass_no < required_no )
    {
		int X_label = -1;
		int Y_label = -1;
		double point = rnd.Rndm();
		bool found = false;

		//cout << "point : " << point << endl;

		for(int s=0;s<grid_no;++s)
		{
			if( point >= grid[s] && point < grid[s+1] )
			{
				found = true;
				X_label = (int)s/Nbin_Y;
				Y_label = (int)(s % Nbin_Y);
				break;
			}	
		}

		if( !found )
		{
			X_label = Nbin_X-1;
			Y_label = Nbin_Y-1;
		}

		//cout << "X_label " << X_label << endl;
		//cout << "Y_label " << Y_label << endl;

		//if( !found ) cerr << "[WRONG]" << endl;

		double x_value = h_ori->GetXaxis()->GetBinWidth(1) * ( (double)X_label - 0.5 );	//+ h_ori->GetXaxis()->GetXmin(); ( == 0 )
		double y_value = h_ori->GetYaxis()->GetBinWidth(1) * ( (double)Y_label - 0.5 ) + h_ori->GetYaxis()->GetXmin();
		h_gen->Fill( x_value, y_value );

		pass_no++;
    }

	//delete [] grid;

    return h_gen;
}

double GetTH2BinContent( TH2* h, double _x, double _y )
{
    int _xLabel = 1, _yLabel = 1;
    for(int i=1;i<=h->GetXaxis()->GetNbins();i++)
    {
        double low_edge = h->GetXaxis()->GetBinLowEdge(i);
        double up_edge = h->GetXaxis()->GetBinUpEdge(i);
        if( ( _x > low_edge  ) && ( _x <= up_edge ) )
        {
            _xLabel = i;
            break;
        }
    }   
    for(int j=1;j<=h->GetYaxis()->GetNbins();j++)
    {
        double low_edge = h->GetYaxis()->GetBinLowEdge(j);
        double up_edge = h->GetYaxis()->GetBinUpEdge(j);
        if( ( _y > low_edge  ) && ( _y <= up_edge ) )
        {
            _yLabel = j;
            break;
        }
    }
    double result = 0.;
    result = h->GetBinContent( _xLabel , _yLabel );
    
    return result;
}

double Ratio_Minus( TH1D* h )
{
    return double( h->Integral( 0, (int)h->GetNbinsX()/2 ) )/(double)IntegralAll(h);
}

void RYields_RMinus_150cut( TH2D* h, double* par )
{
	double minus_ratio = 0.;

	double total = h->ProjectionX()->Integral(0,25);

	minus_ratio = h->Integral( 0, 25, 0, 25 );

	minus_ratio /= total;

	double total_ratio = total/h->ProjectionX()->Integral(0,51);		//to be a total ratio

	par[0] = total_ratio;
	par[1] = minus_ratio;
}


void Draw_templates( TH2D* h_PD, TH2D* h_signal, TH2D* h_B )
{

    string name = "fitted_template.pdf";

    double n1, n2, n3;
    n1 = IntegralAll( h_PD->ProjectionX() );
    n2 = IntegralAll( h_signal->ProjectionX() );
    n3 = IntegralAll( h_B->ProjectionX() );
    cout << "PD's events : " <<  n1 << endl;
    cout << "sig tp's events : " <<  n2 << endl;
    cout << "bkg tp's events : " <<  n3 << endl;


    TCanvas* c = new TCanvas();

    h_PD->SetTitle("PD");
    h_PD->Draw("lego");
    c->Print( ( string(name) + string("(") ).c_str() );

    h_signal->SetTitle("signal");
    h_signal->Draw("lego");
    c->Print( name.c_str() );

    h_B->SetTitle("B");
    h_B->Draw("lego");
    c->Print( ( string(name) + string(")") ).c_str() );

}


