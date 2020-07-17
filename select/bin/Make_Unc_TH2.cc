/**********************************
 *
 *	File Name : Make_Unc_TH2.cc
 *	Date : 200613
 *	Description : to store the template used in systematic uncertainty 
 *
 *  Usage : 
 *			exe {mva/chi2} {algo cut value} {which uncertainty_up/down} {SR/CR,sig/bkg/Data}
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

	if( argc != 5 )
	{
		cerr << "wrong input parameter for this program!" << endl;
		return 0;
	}

	// --- deal with argv[1] --- //
	string algo = "chi2";
	bool is_mva = false;
	algo = string(argv[1]);
	
	if( algo.find("chi2") != string::npos ) {
		is_mva = false;
	}
	else if( algo.find("mva") != string::npos ) {
		is_mva = true;
	}
	
	// --- deal with argv[2] --- //
	double algo_cut = stod( string(argv[2]) );
	cout << "algo-cut is " << algo_cut << endl;

	// --- deal with argv[3] --- //

	string unc_name = string( argv[3] );
	int unc_class = 0;
	int sss = 0;

	if( unc_name.find( "btag" ) != string::npos )
		sss = 1;
	else if( unc_name.find( "pu" ) != string::npos )
		sss = 2;
	else if( unc_name.find( "lep" ) != string::npos )
		sss = 3;
	else if( unc_name.find( "JER" ) != string::npos )
		sss = 4;
	else if( unc_name.find( "JES" ) != string::npos )
		sss = 5;
	else
		sss = 0;

	if( unc_name.find( "up" ) != string::npos )
		unc_class = 2 * sss - 1;
	else if( unc_name.find( "down" ) != string::npos )
		unc_class = 2 * sss;
	else if( unc_name.find( "nominal" ) != string::npos )
		unc_class = 0;
	else {
		cerr << "[ERROR] Uncertainty is required to set up/down or input nominal !" << endl;
		return 0;
	}
	
	// --- deal with argv[4] --- //
	string sample_chosen = string( argv[4] );
	
		// -- sample_part : { 0 = data, 1 = signal, 2 = bkg } -- //
	int sample_part = -1;
	if( sample_chosen.find( "data" ) != string::npos )
		sample_part = 0;
	else if( sample_chosen.find( "sig" ) != string::npos )
		sample_part = 1;
	else if( sample_chosen.find( "bkg" ) != string::npos )
		sample_part = 2;
	else
		sample_part = 0;

	// --- deal with argv[3,4] and sample choosing --- //
	string path_string = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/";
	string path_txt_string = "";
	
	if( sample_chosen.find( "SR" ) != string::npos ) 
	{
		switch( unc_class )
		{
			case 7:
				path_txt_string = path_string + "full_16_SR_JER_up.txt";
				break;
			case 8:
				path_txt_string = path_string + "full_16_SR_JER_down.txt";
				break;
			case 9:
				path_txt_string = path_string + "full_16_SR_JES_up.txt";
				break;
			case 10:
				path_txt_string = path_string + "full_16_SR_JES_down.txt";
				break;
			default:
				path_txt_string = path_string + "full_16_SR_nominal.txt";
				break;
		}
	}
	else if( sample_chosen.find( "CR" ) != string::npos ) 
	{
		path_txt_string = path_string + "full_16_CR.txt";
	}
	else
	{
		cerr << "[ERROR] No SR or CR in argv[4] !" << endl;
		return 0;
	}

	cout << "path-file name : " << path_txt_string << endl;

	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";	

	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	//get_lumi_weight_new( Weights_map , path_txt_string );
	get_lumi_weight_tmp( Weights_map );
	cout << "Finish getting lumi-weight" << endl;
/*
	cout << "w_TT" << endl;
	for(int i=0;i<(int)w_TT.size();++i) {
		cout << w_TT.at(i) << endl;
	}
	cout << "w_DY" << endl;
	for(int i=0;i<(int)w_DY.size();++i) {
		cout << w_DY.at(i) << endl;
	}
	cout << "w_WJets" << endl;
	for(int i=0;i<(int)w_WJets.size();++i) {
		cout << w_WJets.at(i) << endl;
	}
	cout << "w_VV" << endl;
	for(int i=0;i<(int)w_VV.size();++i) {
		cout << w_VV.at(i) << endl;
	}
	cout << "w_ST" << endl;
	for(int i=0;i<(int)w_ST.size();++i) {
		cout << w_ST.at(i) << endl;
	}
	cout << "w_QCD" << endl;
	for(int i=0;i<(int)w_QCD.size();++i) {
		cout << w_QCD.at(i) << endl;
	}
*/

	//*********************About path***********************//
	
	//vector<char*> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	vector<string> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	//map< string, vector<char*>* > Data_Set_Path;
	map< string, vector<string>* > Data_Set_Path;
	Data_Set_Path[data_sets_name[0]] = &TT;				Data_Set_Path[data_sets_name[1]] = &DY;
	Data_Set_Path[data_sets_name[2]] = &WJets;			Data_Set_Path[data_sets_name[3]] = &VV;
	Data_Set_Path[data_sets_name[4]] = &ST;				Data_Set_Path[data_sets_name[5]] = &QCD;
	Data_Set_Path[d6] = &Data_SM;						Data_Set_Path[d7] = &Data_SE;
	get_path( Data_Set_Path , path_txt_string.c_str() );
	cout << "Finish getting Path info." << endl;

	cout << "TT.size() " << TT.size() << endl;

	//**********initial the files and TChain and make the file map and weight map***********//

	map< string , vector<File>* > files_map;
	
	vector<File> files_TT;		vector<File> files_DY;		vector<File> files_WJets;
	vector<File> files_VV;		vector<File> files_ST;		vector<File> files_QCD;

	File f_TT_001((char*)TT[0].c_str());
	files_TT.push_back(f_TT_001);
	for(int i=0;i<(int)DY.size();i++){
		File f_DY((char*)DY[i].c_str());			files_DY.push_back(f_DY);
	}
	for(int i=0;i<(int)WJets.size();i++){
		File f_WJets((char*)WJets[i].c_str());		files_WJets.push_back(f_WJets);
	}
	for(int i=0;i<(int)VV.size();i++){
		File f_VV((char*)VV[i].c_str());			files_VV.push_back(f_VV);
	}
	for(int i=0;i<(int)ST.size();i++){
		File f_ST((char*)ST[i].c_str());			files_ST.push_back(f_ST);
	}	
	for(int i=0;i<(int)QCD.size();i++){
		File f_QCD((char*)QCD[i].c_str());			files_QCD.push_back(f_QCD);
	}

	files_map[data_sets_name[0]] = &files_TT;		files_map[data_sets_name[1]] = &files_DY;
	files_map[data_sets_name[2]] = &files_WJets;	files_map[data_sets_name[3]] = &files_VV;
	files_map[data_sets_name[4]] = &files_ST;		files_map[data_sets_name[5]] = &files_QCD;

	vector<File> files_Data;
	
	File f_Data_1((char*)Data_SM[0].c_str());
	for(int i=1;i<(int)Data_SM.size();i++){
		f_Data_1.AddFile((char*)Data_SM[i].c_str());
	}
	File f_Data_2((char*)Data_SE[0].c_str());
	for(int i=1;i<(int)Data_SE.size();i++){
		f_Data_2.AddFile((char*)Data_SE[i].c_str());
	}
	
	files_Data.push_back(f_Data_1);		files_Data.push_back(f_Data_2);
	files_map[ data_sets_name[6] ] = &files_Data;

	//**********************Start Analysis***********************//

	//initialize ntuple must be after the initialization of the TFile
	
	//***************** declare/make some object ( histograms or vector ......etc.) ******************//

	int Mlb_bin = 50;
	double Mlb_max = 300.;
	double obs_max = 3000000;
	int obs_bin = 50;

	TH2D* h_Mlb_O3_mu = new TH2D( "h_Mlb_O3_mu", "M_{lb}-O_{3}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );
	TH2D* h_Mlb_O6_mu = new TH2D( "h_Mlb_O6_mu", "M_{lb}-O_{6}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );
	TH2D* h_Mlb_O12_mu = new TH2D( "h_Mlb_O12_mu", "M_{lb}-O_{12}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );
	TH2D* h_Mlb_O14_mu = new TH2D( "h_Mlb_O14_mu", "M_{lb}-O_{14}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );

	TH2D* h_Mlb_O3_el = new TH2D( "h_Mlb_O3_el", "M_{lb}-O_{3}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );
	TH2D* h_Mlb_O6_el = new TH2D( "h_Mlb_O6_el", "M_{lb}-O_{6}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );
	TH2D* h_Mlb_O12_el = new TH2D( "h_Mlb_O12_el", "M_{lb}-O_{12}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );
	TH2D* h_Mlb_O14_el = new TH2D( "h_Mlb_O14_el", "M_{lb}-O_{14}", Mlb_bin, 0., Mlb_max, obs_bin, -1 * obs_max, obs_max );

	//--- make histograms ---//

	string startingtime = get_time_str( minute );

	double PASS = 0.;

	for(int k=0;k<(int)files_map.size();k++)		//
	{
		string Set_name = data_sets_name[k];
		bool is_data = false;
		if( Set_name == "Data" ) { is_data = true; }
		
		// -- sample part -- //
		if( sample_part == 0 ) {
			if( !is_data ) continue;
		}
		else if( sample_part == 1 ) {
			if( Set_name != "TT" ) continue;
		}
		else if( sample_part == 2 ) {
			if( Set_name == "TT" ) continue;
			if( is_data ) continue;
		}

		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{	
			if(!is_data){
				if( r >= (int)Data_Set_Path[ Set_name ]->size() )
				{	break;	}
			}
			//--- Get lumi_weight ---//

			double lumi_weight = 1.;
			if(!is_data)
			{	lumi_weight = Weights_map[ Set_name ]->at(r);	}

			//--- Register branches ---//

			printf("\n The file now dealing with is under the data sets %s, the %d one.",Set_name.c_str(),r+1);

			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r).ch , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r).ch , "LepInfo" );
			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r).ch );
			GenInfo genInfo;
			genInfo.Register( files_map[ Set_name ]->at(r).ch );		
			VertexInfo vertexInfo;
			vertexInfo.Register( files_map[ Set_name ]->at(r).ch , "VertexInfo" );

			//--- Read in the known info. ---//

			double btag_weight;
			double lepsf_weight;
			double pu_weight;
			double other_weight;
		 	int SelLep;
			int JetsNo;
			int SelJets[20];
			int SelBJets[5];
			//string channel;
			string* channel;

			string btag_weight_name = "BtagWeight";
			string pu_weight_name = "PUWeight";
			string lepsf_weight_name = "LepWeight";

			switch( unc_class )
			{
				case 1:
					btag_weight_name = btag_weight_name + "_up";
					break;
				case 2:
					btag_weight_name = btag_weight_name + "_down";
					break;
				case 3:
					pu_weight_name = pu_weight_name + "_up";
					break;
				case 4:
					pu_weight_name = pu_weight_name + "_down";
					break;
				case 5:
					lepsf_weight_name = lepsf_weight_name + "_up";
					break;
				case 6:
					lepsf_weight_name = lepsf_weight_name + "_down";
					break;
				default:
					break;
			}

			if( !is_data )
			{
				( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( btag_weight_name.c_str(), &btag_weight  );
				( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( lepsf_weight_name.c_str(), &lepsf_weight  );
				( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( pu_weight_name.c_str(), &pu_weight  );
				( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "OtherWeight", &other_weight  );
			}
			else
			{
				btag_weight = 1.;
				lepsf_weight = 1.;
				pu_weight = 1.;
				other_weight = 1.;	
			}

			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelLep", &SelLep  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "JetsNo", &JetsNo  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelJets", SelJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelBJets", SelBJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "Channel", &channel );
				
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//--- Initialize the selection manager ---//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			if( is_data ) {	sel.SetIsData(is_data);	}

			AcpMgr acpMgr( &leptonInfo, &jetInfo );

			int u = 0;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//if(entry>=3000.) break;		//test
				
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
				
				double weight = 1.;
				sel.reset();

				//-- Set weight --//

				weight = other_weight * pu_weight * btag_weight * lepsf_weight * lumi_weight;
				//weight = other_weight * pu_weight * btag_weight * lepsf_weight ;

				vector<int> sel_jets;
				vector<int> sel_b_jets;

				for(int i=0;i<JetsNo;++i)
				{	sel_jets.push_back( SelJets[i] );	}
				for(int i=0;i<2;++i)
				{	sel_b_jets.push_back( SelBJets[i] );	}

				sel.SetSelJets( sel_jets );
				sel.SetSelBJets( sel_b_jets );

				sel.Setidx_Lep( SelLep );

				if( is_mva ) {
					sel.MVASort();
				}
				else {
					sel.Chi2Sort();
				}
				
				TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
				TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
				TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );
				TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

				double Mjjb = ( hadb + j1 + j2 ).M();
				double Mlb = ( lepb + lepton ).M() ;
				
				if( is_mva ) {
					if( sel.RecoAlgoValue() < algo_cut ) continue;
				}
				else {
					if( sel.RecoAlgoValue() > algo_cut ) continue;
				}
			
				acpMgr.InputSelObjs( sel.Idx_Hadb(), sel.Idx_Lepb(), sel.Idx_J1(), sel.Idx_Lep() );
				PASS += weight;
				
				if( *channel == "mu" ) {
					h_Mlb_O3_mu->Fill( Mlb, acpMgr.Obs3(), weight );
					h_Mlb_O6_mu->Fill( Mlb, acpMgr.Obs6(), weight );
					h_Mlb_O12_mu->Fill( Mlb, acpMgr.Obs12(), weight );
					h_Mlb_O14_mu->Fill( Mlb, acpMgr.Obs13(), weight );
				}
				else if( *channel == "el" ) {
					h_Mlb_O3_el->Fill( Mlb, acpMgr.Obs3(), weight );
					h_Mlb_O6_el->Fill( Mlb, acpMgr.Obs6(), weight );
					h_Mlb_O12_el->Fill( Mlb, acpMgr.Obs12(), weight );
					h_Mlb_O14_el->Fill( Mlb, acpMgr.Obs13(), weight );
				}

				if( Mlb > 150. ) continue;

			}	//end of entry for-loop	
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
			
		}		//end of r for-loop
	}			//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string gen_path = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/UncTemplate/" + algo + "/";
	string new_file_name = gen_path + string("Mlb_Obs-") + unc_name + string("-") + sample_chosen + "-2ch-bin100_50" + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	h_Mlb_O3_mu->Write();
	h_Mlb_O6_mu->Write();
	h_Mlb_O12_mu->Write();
	h_Mlb_O14_mu->Write();

	h_Mlb_O3_el->Write();
	h_Mlb_O6_el->Write();
	h_Mlb_O12_el->Write();
	h_Mlb_O14_el->Write();

	f_out->Close();
	
	cout << "Total events : " << PASS << endl;
	
	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
}

