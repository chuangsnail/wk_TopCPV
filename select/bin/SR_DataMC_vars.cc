/**********************************
 *
 *	File Name : SR_DataMC_vars.cc
 *	Date : 200730
 *	Description : for mva method
 *	Usage : {exe} {mva/chi2} {algo_cut} {test/mormal}	
 *
 * ********************************/
//#include "TopCPViolation/select/interface/ttt.h"
#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/CalAcp.h"
//#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/Hists.h"
#include "TopCPViolation/select/interface/about_time.h"
#include "TopCPViolation/select/interface/MVAvar.h"

using namespace std;
using namespace std;

int main(int argc,char* argv[])
{
	if( argc != 3 && argc != 4 )
	{
		cerr << "[ERROR] wrong argment numbers!" << endl;
		return 0;
	}
	bool is_mva = false;
	if( string( argv[1] ).find( "mva" ) != string::npos )
		is_mva = true;

	double algo_cut = stod( string( argv[2] ) );

	bool is_test = false;
	string option = "normal";
	if( argc == 4 )
		option = string( argv[3] );

	if( option.find("test") != string::npos )
	{
		is_test = true;
		cout << ">> Mode test <<" << endl;
	}

	string training_name = "";

	if( is_mva )
		cout << "With mva sort" << endl;
	else
		cout << "With chi2 sort" << endl;

	cout << "algo-cut is " << algo_cut << endl;
	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//--- To finish something about weight ---//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	//get_lumi_weight( Weights_map , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR.txt" );
	//get_lumi_weight( Weights_map , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR_nominal.txt" );
	get_lumi_weight_tmp( Weights_map );
	cout << "Finish getting lumi-weight" << endl;

	//*********************About path***********************//
	
	//vector<char*> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	vector<string> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	//map< string, vector<char*>* > Data_Set_Path;
	map< string, vector<string>* > Data_Set_Path;
	Data_Set_Path[data_sets_name[0]] = &TT;				Data_Set_Path[data_sets_name[1]] = &DY;
	Data_Set_Path[data_sets_name[2]] = &WJets;			Data_Set_Path[data_sets_name[3]] = &VV;
	Data_Set_Path[data_sets_name[4]] = &ST;				Data_Set_Path[data_sets_name[5]] = &QCD;
	Data_Set_Path[d6] = &Data_SM;						Data_Set_Path[d7] = &Data_SE;
	//get_path( Data_Set_Path , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR.txt" );
	get_path( Data_Set_Path , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR_nominal.txt" );
	//get_path_tmp( Data_Set_Path );
	cout << "Finish getting Path info." << endl;

	//**********initial the files and TChain and make the file map and weight map***********//

	char tree_name[20] = "root";

	map< string , vector<TChain*>* > files_map;
	
	vector<TChain*> files_TT;		vector<TChain*> files_DY;		vector<TChain*> files_WJets;
	vector<TChain*> files_VV;		vector<TChain*> files_ST;		vector<TChain*> files_QCD;

	TChain* ch_ = new TChain( tree_name );
	ch_->Add( TT[0].c_str() );
	files_TT.push_back( ch_ );
	for(int i=0;i<(int)DY.size();i++){
		TChain* _ch = new TChain( tree_name );
		_ch->Add( DY[i].c_str() );
		files_DY.push_back( _ch );
	}
	for(int i=0;i<(int)WJets.size();i++){
		TChain* _ch = new TChain( tree_name );
		_ch->Add( WJets[i].c_str() );
		files_WJets.push_back( _ch );
	}
	for(int i=0;i<(int)VV.size();i++){
		TChain* _ch = new TChain( tree_name );
		_ch->Add( VV[i].c_str() );
		files_VV.push_back( _ch );
	}
	for(int i=0;i<(int)ST.size();i++){
		TChain* _ch = new TChain( tree_name );
		_ch->Add( ST[i].c_str() );
		files_ST.push_back( _ch );
	}	
	for(int i=0;i<(int)QCD.size();i++){
		TChain* _ch = new TChain( tree_name );
		_ch->Add( QCD[i].c_str() );
		files_QCD.push_back( _ch );
	}

	files_map[data_sets_name[0]] = &files_TT;		files_map[data_sets_name[1]] = &files_DY;
	files_map[data_sets_name[2]] = &files_WJets;	files_map[data_sets_name[3]] = &files_VV;
	files_map[data_sets_name[4]] = &files_ST;		files_map[data_sets_name[5]] = &files_QCD;

	vector<TChain*> files_Data;
	
	TChain* _ch1 = new TChain( tree_name );
	for(int i=0;i<(int)Data_SM.size();i++){
		_ch1->Add( Data_SM[i].c_str() );
	}
	TChain* _ch2 = new TChain( tree_name );
	for(int i=0;i<(int)Data_SE.size();i++){
		_ch2->Add( Data_SE[i].c_str() );
	}
	
	files_Data.push_back(_ch1);		files_Data.push_back(_ch2);
	files_map[ data_sets_name[6] ] = &files_Data;

	//**********************Start Analysis***********************//

	//*****************declare/make some object ( histograms or vector ......etc.)******************//
	//*** make histograms ***//
	
	Hists h_algo;
	if( is_mva )
	{
		if( string(argv[1]).find( "MLP" ) != string::npos ) {
			h_algo.SetBinInfo( 40, 0., 1. );	//for MLP
		}
		else if( string(argv[1]).find( "BDT" ) != string::npos ) {
			h_algo.SetBinInfo( 40, -1., 1. );	//for BDT(G)
		}
	}
	h_algo.NoCutModeON();
	h_algo.OneCutModeON();
	h_algo.TwoCutModeON();

	Hists h_mjj;
	h_mjj.SetBinInfo( 50, 0, 200 );
	h_mjj.AllCutModeON();
	
	Hists h_j1j2_sumPt;
	h_j1j2_sumPt.SetBinInfo( 50, 0, 500 );
	h_j1j2_sumPt.AllCutModeON();
	Hists h_j1j2_absdelPt;
	h_j1j2_absdelPt.SetBinInfo( 50, 0, 200 );
	h_j1j2_absdelPt.AllCutModeON();
	Hists h_j1j2_delR;
	h_j1j2_delR.SetBinInfo( 50, 0., 10. );
	h_j1j2_delR.AllCutModeON();
	
	Hists h_whadb_sumPt;
	h_whadb_sumPt.SetBinInfo( 50, 0, 500 );
	h_whadb_sumPt.AllCutModeON();
	Hists h_whadb_delPt;
	h_whadb_delPt.SetBinInfo( 50, -200, 200 );
	h_whadb_delPt.AllCutModeON();
	Hists h_whadb_delR;
	h_whadb_delR.SetBinInfo( 50, 0., 10. );
	h_whadb_delR.AllCutModeON();

	Hists h_hadblepton_sumPt;
	h_hadblepton_sumPt.SetBinInfo( 50, 0, 500 );
	h_hadblepton_sumPt.AllCutModeON();
	Hists h_hadblepton_delPt;
	h_hadblepton_delPt.SetBinInfo( 50, -200, 200 );
	h_hadblepton_delPt.AllCutModeON();
	Hists h_hadblepton_delR;
	h_hadblepton_delR.SetBinInfo( 50, 0., 10. );
	h_hadblepton_delR.AllCutModeON();
	
	Hists h_hadwlepton_sumPt;
	h_hadwlepton_sumPt.SetBinInfo( 50, 0, 500 );
	h_hadwlepton_sumPt.AllCutModeON();
	Hists h_hadwlepton_delPt;
	h_hadwlepton_delPt.SetBinInfo( 50, -200., 200. );
	h_hadwlepton_delPt.AllCutModeON();
	Hists h_hadwlepton_delR;
	h_hadwlepton_delR.SetBinInfo( 50, 0., 10. );
	h_hadwlepton_delR.AllCutModeON();

	Hists h_hadbmet_delPhi;
	h_hadbmet_delPhi.SetBinInfo( 50, -5., 5. );
	h_hadbmet_delPhi.AllCutModeON();
	Hists h_hadbmet_sumPt;
	h_hadbmet_sumPt.SetBinInfo( 50, 0., 500. );
	h_hadbmet_sumPt.AllCutModeON();
	Hists h_hadbmet_delPt;
	h_hadbmet_delPt.SetBinInfo( 50, -200., 200. );
	h_hadbmet_delPt.AllCutModeON();
	
	Hists h_hadwmet_delPhi;
	h_hadwmet_delPhi.SetBinInfo( 50, -5., 5. );
	h_hadwmet_delPhi.AllCutModeON();
	Hists h_hadwmet_sumPt;
	h_hadwmet_sumPt.SetBinInfo( 50, 0., 500. );
	h_hadwmet_sumPt.AllCutModeON();
	Hists h_hadwmet_delPt;
	h_hadwmet_delPt.SetBinInfo( 50, -200., 200. );
	h_hadwmet_delPt.AllCutModeON();
	
	string startingtime = get_time_str( minute );

	for(int k=0;k<(int)files_map.size();k++)		//
	{
		string Set_name = data_sets_name[k];
		bool is_data = false;
		if( Set_name == "Data" ) { is_data = true; }
		//if( Set_name != "VV" ) continue;

		//*** to prepare the counting objects ***//

		double NO_ncut_mu = 0.;
		double NO_1cut_mu = 0.;
		double NO_2cut_mu = 0.;

		double NO_ncut_el = 0.;
		double NO_1cut_el = 0.;
		double NO_2cut_el = 0.;

		printf("\n The file now dealing with is under the data sets %s.",Set_name.c_str());

		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{	
			if(!is_data){
				if( r >= (int)Data_Set_Path[ Set_name ]->size() )
				{	break;	}
			}
			//*** Get lumi_weight ***//
			double lumi_weight = 1.;
			if(!is_data)
			{	lumi_weight = Weights_map[ Set_name ]->at(r);	}

			//*** Register branches ***//

			//printf("\n The file now dealing with is under the data sets %s, the %d one.",Set_name.c_str(),r+1);

			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r) , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r) , "LepInfo" );
			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r) );
			GenInfo genInfo;
			genInfo.Register( files_map[ Set_name ]->at(r) );		
			VertexInfo vertexInfo;
			vertexInfo.Register( files_map[ Set_name ]->at(r) , "VertexInfo" );

			//*** Read in the known info. ***//

			double t_Weight;
		 	int SelLep;
			int JetsNo;
			int SelJets[20];
			int SelBJets[5];
			//string channel;
			string* channel;

			( files_map[ Set_name ]->at(r) )->SetBranchAddress( "t_Weight", &t_Weight  );
			( files_map[ Set_name ]->at(r) )->SetBranchAddress( "SelLep", &SelLep  );
			( files_map[ Set_name ]->at(r) )->SetBranchAddress( "JetsNo", &JetsNo  );
			( files_map[ Set_name ]->at(r) )->SetBranchAddress( "SelJets", SelJets  );
			( files_map[ Set_name ]->at(r) )->SetBranchAddress( "SelBJets", SelBJets  );
			( files_map[ Set_name ]->at(r) )->SetBranchAddress( "Channel", &channel );
			
			int t_entries = (files_map[ Set_name ]->at(r))->GetEntries();
			if( is_test )
			{	t_entries = 200;	}
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//--- Initialize the selection manager ---//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			sel.SetTrain( training_name );
			if( is_data ) {	sel.SetIsData(is_data);	}

			//AcpMgr acpMgr( &leptonInfo, &jetInfo );

			int u = 0;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//if(entry>=3000.) break;		//test
				
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r) )->GetEntry(entry);
				sel.reset();

				//** Set lumi_Weight first **//

				sel.ScaleWeight( lumi_weight );
				sel.ScaleWeight( t_Weight );
				
				//use mva to choose best choice of reconstruct top quark's mass 

				vector<int> sel_jets;
				vector<int> sel_b_jets;

				for(int i=0;i<JetsNo;++i)
				{	sel_jets.push_back( SelJets[i] );	}
				for(int i=0;i<2;++i)
				{	sel_b_jets.push_back( SelBJets[i] );	}

				sel.SetSelJets( sel_jets );
				sel.SetSelBJets( sel_b_jets );
				sel.Setidx_Lep( SelLep );

				if( is_mva )
					sel.MVASort();
				else
					sel.Chi2Sort();
				
				TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

				double Mlb = ( lepb + lepton ).M() ;
				double algo_value = sel.GetRecoAlgoValue();

				double* var = new double[22];
				mvatool::InputVar( var, jetInfo, leptonInfo, evtInfo, \
				sel.Idx_Lep(), sel.Idx_Hadb(), sel.Idx_Lepb(), sel.Idx_J1(), sel.Idx_J2() );
				
				if( is_test && entry%500 == 0 )
					cout << "entry : " << entry << " , algo : " <<  algo_value << endl;

				h_algo.FillHist( "NC", k, *channel, algo_value, 0., sel.Weight() );
				h_mjj.FillHist( "NC", k, *channel, var[1], 0., sel.Weight() );
				h_j1j2_sumPt.FillHist( "NC", k, *channel, var[2], 0., sel.Weight() );
				h_j1j2_absdelPt.FillHist( "NC", k, *channel, var[3], 0., sel.Weight() );
				h_j1j2_delR.FillHist( "NC", k, *channel, var[4], 0., sel.Weight() );
				h_whadb_sumPt.FillHist( "NC", k, *channel, var[5], 0., sel.Weight() );
				h_whadb_delPt.FillHist( "NC", k, *channel, var[6], 0., sel.Weight() );
				h_whadb_delR.FillHist( "NC", k, *channel, var[7], 0., sel.Weight() );
				h_hadblepton_sumPt.FillHist( "NC", k, *channel, var[8], 0., sel.Weight() );
				h_hadblepton_delPt.FillHist( "NC", k, *channel, var[9], 0., sel.Weight() );
				h_hadblepton_delR.FillHist( "NC", k, *channel, var[10], 0., sel.Weight() );
				h_hadwlepton_sumPt.FillHist( "NC", k, *channel, var[11], 0., sel.Weight() );
				h_hadwlepton_delPt.FillHist( "NC", k, *channel, var[12], 0., sel.Weight() );
				h_hadwlepton_delR.FillHist( "NC", k, *channel, var[13], 0., sel.Weight() );
				h_hadbmet_delPhi.FillHist( "NC", k, *channel, var[14], 0., sel.Weight() );
				h_hadbmet_sumPt.FillHist( "NC", k, *channel, var[15], 0., sel.Weight() );
				h_hadbmet_delPt.FillHist( "NC", k, *channel, var[16], 0., sel.Weight() );
				h_hadwmet_delPhi.FillHist( "NC", k, *channel, var[17], 0., sel.Weight() );
				h_hadwmet_sumPt.FillHist( "NC", k, *channel, var[18], 0., sel.Weight() );
				h_hadwmet_delPt.FillHist( "NC", k, *channel, var[19], 0., sel.Weight() );
		
				if( is_mva ) {
					if( sel.RecoAlgoValue() < algo_cut ) { continue; }
                }
				else {
					if( sel.RecoAlgoValue() > algo_cut ) { continue; }
                }

				h_algo.FillHist( "1C", k, *channel, algo_value, 0., sel.Weight() );
				h_mjj.FillHist( "1C", k, *channel, var[1], 0., sel.Weight() );
				h_j1j2_sumPt.FillHist( "1C", k, *channel, var[2], 0., sel.Weight() );
				h_j1j2_absdelPt.FillHist( "1C", k, *channel, var[3], 0., sel.Weight() );
				h_j1j2_delR.FillHist( "1C", k, *channel, var[4], 0., sel.Weight() );
				h_whadb_sumPt.FillHist( "1C", k, *channel, var[5], 0., sel.Weight() );
				h_whadb_delPt.FillHist( "1C", k, *channel, var[6], 0., sel.Weight() );
				h_whadb_delR.FillHist( "1C", k, *channel, var[7], 0., sel.Weight() );
				h_hadblepton_sumPt.FillHist( "1C", k, *channel, var[8], 0., sel.Weight() );
				h_hadblepton_delPt.FillHist( "1C", k, *channel, var[9], 0., sel.Weight() );
				h_hadblepton_delR.FillHist( "1C", k, *channel, var[10], 0., sel.Weight() );
				h_hadwlepton_sumPt.FillHist( "1C", k, *channel, var[11], 0., sel.Weight() );
				h_hadwlepton_delPt.FillHist( "1C", k, *channel, var[12], 0., sel.Weight() );
				h_hadwlepton_delR.FillHist( "1C", k, *channel, var[13], 0., sel.Weight() );
				h_hadbmet_delPhi.FillHist( "1C", k, *channel, var[14], 0., sel.Weight() );
				h_hadbmet_sumPt.FillHist( "1C", k, *channel, var[15], 0., sel.Weight() );
				h_hadbmet_delPt.FillHist( "1C", k, *channel, var[16], 0., sel.Weight() );
				h_hadwmet_delPhi.FillHist( "1C", k, *channel, var[17], 0., sel.Weight() );
				h_hadwmet_sumPt.FillHist( "1C", k, *channel, var[18], 0., sel.Weight() );
				h_hadwmet_delPt.FillHist( "1C", k, *channel, var[19], 0., sel.Weight() );
				
				if( Mlb > 150 ){ continue; }
				h_algo.FillHist( "2C", k, *channel, algo_value, 0., sel.Weight() );
				h_mjj.FillHist( "2C", k, *channel, var[1], 0., sel.Weight() );
				h_j1j2_sumPt.FillHist( "2C", k, *channel, var[2], 0., sel.Weight() );
				h_j1j2_absdelPt.FillHist( "2C", k, *channel, var[3], 0., sel.Weight() );
				h_j1j2_delR.FillHist( "2C", k, *channel, var[4], 0., sel.Weight() );
				h_whadb_sumPt.FillHist( "2C", k, *channel, var[5], 0., sel.Weight() );
				h_whadb_delPt.FillHist( "2C", k, *channel, var[6], 0., sel.Weight() );
				h_whadb_delR.FillHist( "2C", k, *channel, var[7], 0., sel.Weight() );
				h_hadblepton_sumPt.FillHist( "2C", k, *channel, var[8], 0., sel.Weight() );
				h_hadblepton_delPt.FillHist( "2C", k, *channel, var[9], 0., sel.Weight() );
				h_hadblepton_delR.FillHist( "2C", k, *channel, var[10], 0., sel.Weight() );
				h_hadwlepton_sumPt.FillHist( "2C", k, *channel, var[11], 0., sel.Weight() );
				h_hadwlepton_delPt.FillHist( "2C", k, *channel, var[12], 0., sel.Weight() );
				h_hadwlepton_delR.FillHist( "2C", k, *channel, var[13], 0., sel.Weight() );
				h_hadbmet_delPhi.FillHist( "2C", k, *channel, var[14], 0., sel.Weight() );
				h_hadbmet_sumPt.FillHist( "2C", k, *channel, var[15], 0., sel.Weight() );
				h_hadbmet_delPt.FillHist( "2C", k, *channel, var[16], 0., sel.Weight() );
				h_hadwmet_delPhi.FillHist( "2C", k, *channel, var[17], 0., sel.Weight() );
				h_hadwmet_sumPt.FillHist( "2C", k, *channel, var[18], 0., sel.Weight() );
				h_hadwmet_delPt.FillHist( "2C", k, *channel, var[19], 0., sel.Weight() );
			
			}	//end of entry for-loop
			if( is_mva )	
				training_name = sel.GetTrain();
			//cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
		}		//end of r for-loop

	}			//end of k for-loop

   	//--- Draw & Store ---//
	 
	//Save these hists to be a root file
	
	if( !is_mva )
		training_name = "chi2";
	string time_str = "";
	time_str = get_time_str( minute );
	
	string new_file_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/VarResult/" + training_name + "/" + string("SRvars_");

	TFile* f_1 = new TFile( (char*)(new_file_name + "algo.root").c_str() , "recreate" );
	h_algo.WriteIn( "NC 1C 2C" );
	f_1->Close();

	TFile* f_0 = new TFile( (char*)(new_file_name + "mjj.root").c_str() , "recreate" );
	h_mjj.WriteIn( "NC 1C 2C" );
	f_0->Close();
	TFile* f_2 = new TFile( (char*)(new_file_name + "j1j2_sumPt.root").c_str() , "recreate" );
	h_j1j2_sumPt.WriteIn( "NC 1C 2C" );
	f_2->Close();
	TFile* f_3 = new TFile( (char*)(new_file_name + "j1j2_absdelPt.root").c_str() , "recreate" );
	h_j1j2_absdelPt.WriteIn( "NC 1C 2C" );
	f_3->Close();
	TFile* f_4 = new TFile( (char*)(new_file_name + "j1j2_delR.root").c_str() , "recreate" );
	h_j1j2_delR.WriteIn( "NC 1C 2C" );
	f_4->Close();
	TFile* f_5 = new TFile( (char*)(new_file_name + "whadb_sumPt.root").c_str() , "recreate" );
	h_whadb_sumPt.WriteIn( "NC 1C 2C" );	
	f_5->Close();
	TFile* f_6 = new TFile( (char*)(new_file_name + "whadb_delPt.root").c_str() , "recreate" );
	h_whadb_delPt.WriteIn( "NC 1C 2C" );
	f_6->Close();
	TFile* f_7 = new TFile( (char*)(new_file_name + "whadb_delR.root").c_str() , "recreate" );
	h_whadb_delR.WriteIn( "NC 1C 2C" );
	f_7->Close();
	TFile* f_8 = new TFile( (char*)(new_file_name + "hadblepton_sumPt.root").c_str() , "recreate" );
	h_hadblepton_sumPt.WriteIn( "NC 1C 2C" );
	f_8->Close();
	TFile* f_9 = new TFile( (char*)(new_file_name + "hadblepton_delPt.root").c_str() , "recreate" );
	h_hadblepton_delPt.WriteIn( "NC 1C 2C" );
	f_9->Close();
	TFile* f_10 = new TFile( (char*)(new_file_name + "hadblepton_delR.root").c_str() , "recreate" );
	h_hadblepton_delR.WriteIn( "NC 1C 2C" );
	f_10->Close();
	TFile* f_11 = new TFile( (char*)(new_file_name + "hadwlepton_sumPt.root").c_str() , "recreate" );
	h_hadwlepton_sumPt.WriteIn( "NC 1C 2C" );
	f_11->Close();
	TFile* f_12 = new TFile( (char*)(new_file_name + "hadwlepton_delPt.root").c_str() , "recreate" );
	h_hadwlepton_delPt.WriteIn( "NC 1C 2C" );
	f_12->Close();
	TFile* f_13 = new TFile( (char*)(new_file_name + "hadwlepton_delR.root").c_str() , "recreate" );
	h_hadwlepton_delR.WriteIn( "NC 1C 2C" );
	f_13->Close();
	TFile* f_14 = new TFile( (char*)(new_file_name + "hadbmet_delPhi.root").c_str() , "recreate" );
	h_hadbmet_delPhi.WriteIn( "NC 1C 2C" );
	f_14->Close();
	TFile* f_15 = new TFile( (char*)(new_file_name + "hadbmet_sumPt.root").c_str() , "recreate" );
	h_hadbmet_sumPt.WriteIn( "NC 1C 2C" );
	f_15->Close();
	TFile* f_16 = new TFile( (char*)(new_file_name + "hadbmet_delPt.root").c_str() , "recreate" );
	h_hadbmet_delPt.WriteIn( "NC 1C 2C" );
	f_16->Close();
	TFile* f_17 = new TFile( (char*)(new_file_name + "hadwmet_delPhi.root").c_str() , "recreate" );
	h_hadwmet_delPhi.WriteIn( "NC 1C 2C" );
	f_17->Close();
	TFile* f_18 = new TFile( (char*)(new_file_name + "hadwmet_sumPt.root").c_str() , "recreate" );
	h_hadwmet_sumPt.WriteIn( "NC 1C 2C" );
	f_18->Close();
	TFile* f_19 = new TFile( (char*)(new_file_name + "hadwmet_delPt.root").c_str() , "recreate" );
	h_hadwmet_delPt.WriteIn( "NC 1C 2C" );
	f_19->Close();

	
	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
}

