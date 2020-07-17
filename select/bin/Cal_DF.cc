/**********************************
 *
 *	File Name : Cal_DF.cc
 *	Date : 200530
 *	Description : for mva method	
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
	string algo = "chi2";
	bool is_mva = false;
	if( argc >= 2 )
	{
		algo = string(argv[1]);
	}
	if( algo.find("chi2") != string::npos ) {
		is_mva = false;
	}
	else if( algo.find("mva") != string::npos ) {
		is_mva = true;
	}

	double algo_cut = stod( string( argv[2] ) );
	bool is_Mlb_cut = false;
	if( stoi( string( argv[3] ) ) != 0 )
		is_Mlb_cut = true;

	string training_name = "";

	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	//get_lumi_weight( Weights_map , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR.txt" );
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
	get_path( Data_Set_Path , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR.txt" );
	cout << "Finish getting Path info." << endl;

	//**********initial the files and TChain and make the file map and weight map***********//

	map< string , vector<TChain*>* > files_map;
	
	vector<TChain*> files_TT;		vector<TChain*> files_DY;		vector<TChain*> files_WJets;
	vector<TChain*> files_VV;		vector<TChain*> files_ST;		vector<TChain*> files_QCD;

	char tree_name[20] = "root";

	TChain* ch_ = new TChain( "root" );
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

	//**************** open the file we want to store the result **********************************//
	
	//initialize ntuple must be after the initialization of the TFile
	
	//*****************declare/make some object ( histograms or vector ......etc.)******************//
	//*** make histograms ***//
	
	DFMgr dfMgr;
	
	string startingtime = get_time_str( minute );

	for(int k=0;k<(int)files_map.size();k++)		//
	{
		string Set_name = data_sets_name[k];
		bool is_data = false;
		if( Set_name == "Data" ) { is_data = true; }
		if( Set_name != "TT" ) continue;

		//*** to prepare the counting objects ***//

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

			//*** Initialize the selection manager ***//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			sel.SetTrain( training_name );
			if( is_data ) {	sel.SetIsData(is_data);	}

			AcpMgr acpMgr( &leptonInfo, &jetInfo );
			GenMgr genMgr( &genInfo, &jetInfo, &leptonInfo );

			printf("\nAnd the Entries of this data files are : %d\n",t_entries);
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

				//--- Set Weight and reset physical object in an event ---//

				sel.ScaleWeight( lumi_weight );
				sel.ScaleWeight( t_Weight );
				
				vector<int> sel_jets;
				vector<int> sel_b_jets;

				for(int i=0;i<JetsNo;++i)
				{	sel_jets.push_back( SelJets[i] );	}
				for(int i=0;i<2;++i)
				{	sel_b_jets.push_back( SelBJets[i] );	}

				sel.SetSelJets( sel_jets );
				sel.SetSelBJets( sel_b_jets );
				sel.Setidx_Lep( SelLep );

				//use mva to choose best choice of reconstruct top quark's mass 

				if( is_mva )
					sel.MVASort();
				else
					sel.Chi2Sort();
			
				if( is_mva ) {
					if( sel.RecoAlgoValue() < algo_cut ) { continue; }
                }
				else {
					if( sel.RecoAlgoValue() > algo_cut ) { continue; }
                }
				
				if( is_Mlb_cut ) {
					TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
					TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

					double Mlb = ( lepb + lepton ).M() ;
					if( Mlb > 150 ) continue;
				}

				//--- Calculate the Dilution Factor at the first time ---//
				
				bb_matching_type bbType = no_match;			//default
				bbType = genMgr.Get_bb_Option( sel.Idx_Hadb(), sel.Idx_Lepb(), sel.Idx_Lep() );
				
				acpMgr.InputSelObjs( sel.Idx_Hadb(), sel.Idx_Lepb(), sel.Idx_J1(), sel.Idx_Lep() );

				double det_O3 = acpMgr.Obs3();	
				double det_O6 = acpMgr.Obs6();	
				double det_O12 = acpMgr.Obs12();	
				double det_O13 = acpMgr.Obs13();
					
				double gen_O3 = genMgr.GenAcp( "Obs3" );
				double gen_O6 = genMgr.GenAcp( "Obs6" );
				double gen_O12 = genMgr.GenAcp( "Obs12" );
				double gen_O13 = genMgr.GenAcp( "Obs13" );

				//--- Calculate the Dilution Factor at 3 step ---//

				dfMgr.SignDec( "Obs3", det_O3, gen_O3, sel.Weight() );
				dfMgr.SignDec( "Obs6", det_O6, gen_O6, sel.Weight() );
				dfMgr.SignDec( "Obs12", det_O12, gen_O12, sel.Weight() );
				dfMgr.SignDec( "Obs13", det_O13, gen_O13, sel.Weight() );

				if( bbType != bb_matching_type::correct ) continue;

				dfMgr.SignDec_bb( "Obs3", det_O3, gen_O3, sel.Weight() );
				dfMgr.SignDec_bb( "Obs6", det_O6, gen_O6, sel.Weight() );
				dfMgr.SignDec_bb( "Obs12", det_O12, gen_O12, sel.Weight() );
				dfMgr.SignDec_bb( "Obs13", det_O13, gen_O13, sel.Weight() );

				if( !genMgr.IsJ1Correct( sel.Idx_J1() ) ) continue;
				
				dfMgr.SignDec_bbj1( "Obs3", det_O3, gen_O3, sel.Weight() );
				dfMgr.SignDec_bbj1( "Obs6", det_O6, gen_O6, sel.Weight() );
				dfMgr.SignDec_bbj1( "Obs12", det_O12, gen_O12, sel.Weight() );
				dfMgr.SignDec_bbj1( "Obs13", det_O13, gen_O13, sel.Weight() );
			
			}	//end of entry for-loop	
			training_name = sel.GetTrain();
			cout << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;

			cout << "genMgr.counter : " << genMgr.counter << endl;
			cout << "total entries : " << t_entries << endl;
		}		//end of r for-loop
	}			//end of k for-loop

	//--- Calculate Dilution Factor ---//
	
	dfMgr.Calculate();	

	//Save these hists to be a root file
	
	if( !is_mva ) training_name = "Chi2";
	
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = training_name + "_DF_" + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	dfMgr.WriteIn( "N bb bbj1" );	

	f_out->Close();
	ofstream f;
	f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/DF.txt",fstream::app );		

	f << "Algorithm " << algo << endl;
	f << "algo cut " << algo_cut << endl;
	f << "Mlb cut ? " << (int)is_Mlb_cut << endl;
	f << "starting time is : " << startingtime << endl;
	f << "Finish time is : " << time_str << endl;
	dfMgr.PrintIn( f );

	//f << "total entries #: " <<  << endl;

	f << "==============================================================" << endl; 
	
	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
}

