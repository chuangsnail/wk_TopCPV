/**********************************
 *
 *	File Name : data_driven_QCD.cc
 *	Date : 200225
 *	
 *
 *
 *
 *
 * ********************************/
//#include "TopCPViolation/select/interface/ttt.h"
#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/CalAcp.h"
#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/Hists.h"
#include "TopCPViolation/select/interface/about_time.h"

using namespace std;

int main(int argc,char* argv[])
{

	//*** 									***//
	// Use CR2's data to data-driven CR1's QCD //
	//***									***//

	string CR1_name = string(argv[1]);
	string CR2_name = string(argv[2]);

	//*****Drawing Plotting or Outputting files*****//
	// apply QCD data-driven here

	string option = "NC 1C";

	Hists hists;
	hists.GetObjs( CR1_name, option );


	TH1* h_data_mu;			TH1* h_data_el;
	TH1* h_data_mu_c;		TH1* h_data_el_c;
	TH1* h_l_data_mu;		TH1* h_l_data_el;
	TH1* h_l_data_mu_c;		TH1* h_l_data_el_c;

	TFile* dd_file = new TFile( (char*)CR2_name.c_str() );
	dd_file->GetObject("h_Data_mu",h_data_mu);
	dd_file->GetObject("h_Data_el",h_data_el);
	dd_file->GetObject("h_Data_mu_c",h_data_mu_c);
	dd_file->GetObject("h_Data_el_c",h_data_el_c);
	
	dd_file->GetObject("h_l_Data_mu",h_l_data_mu);
	dd_file->GetObject("h_l_Data_el",h_l_data_el);
	dd_file->GetObject("h_l_Data_mu_c",h_l_data_mu_c);
	dd_file->GetObject("h_l_Data_el_c",h_l_data_el_c);


	double ori_entry = t_IntegralTH1( hists.h_QCD_mu_c );
	double data_entry = t_IntegralTH1( h_data_mu_c );
	hists.h_QCD_mu_c = (TH1F*)h_data_mu_c->Clone();
	hists.h_QCD_mu_c->Scale( ori_entry/data_entry );
	hists.h_QCD_mu_c->SetName("h_QCD_mu_c");

	cout << ori_entry << " " << data_entry << endl;
	
	double ori_entry2 = t_IntegralTH1( hists.h_QCD_el_c );
	double data_entry2 = t_IntegralTH1( h_data_el_c );
	hists.h_QCD_el_c = (TH1F*)h_data_el_c->Clone();
	hists.h_QCD_el_c->Scale( ori_entry2/data_entry2 );
	hists.h_QCD_el_c->SetName("h_QCD_el_c");

	cout << ori_entry2 << " " << data_entry2 << endl;
	
	double ori_entry3 = t_IntegralTH1( hists.h_l_QCD_mu_c );
	double data_entry3 = t_IntegralTH1( h_l_data_mu_c );
	hists.h_l_QCD_mu_c = (TH1F*)h_l_data_mu_c->Clone();
	hists.h_l_QCD_mu_c->Scale( ori_entry3/data_entry3 );
	hists.h_l_QCD_mu_c->SetName("h_l_QCD_mu_c");

	cout << ori_entry3 << " " << data_entry3 << endl;
	
	double ori_entry4 = t_IntegralTH1( hists.h_l_QCD_el_c );
	double data_entry4 = t_IntegralTH1( h_l_data_el_c );
	hists.h_l_QCD_el_c = (TH1F*)h_l_data_el_c->Clone();
	hists.h_l_QCD_el_c->Scale( ori_entry4/data_entry4 );
	hists.h_l_QCD_el_c->SetName("h_l_QCD_el_c");

	cout << ori_entry4 << " " << data_entry4 << endl;

	//DataDriven( hists.h_QCD_mu_c, h_data_mu_c );
	//DataDriven( hists.h_QCD_el_c, h_data_el_c );
	//Save these hists to be a root file
	

	size_t pos_ptroot = CR1_name.find( ".root" );
	string new_file_name = CR1_name.substr( 0, pos_ptroot );
	new_file_name  = new_file_name + "_QCD-DD" + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	//*** store! ***//
	hists.WriteIn("NC 1C");

	f_out->Close();

	//*****make space free*****//
}

