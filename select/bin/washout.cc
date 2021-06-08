/**********************************
 *
 *	File Name : det_bias_change_mva.cc
 *	Date : 200128
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

//exe mva/chi2 algo-cut is_Mlb_cut normal/test changeNo dir_name

int main(int argc,char* argv[])
{
	if( argc != 7 )
	{
		cerr << "[ERROR] wrong argment numbers!" << endl;
		return 0;
	}
	bool is_mva = false;
	if( string( argv[1] ).find( "mva" ) != string::npos )
		is_mva = true;

	double algo_cut = stod( string( argv[2] ) );

	bool is_Mlb_cut = false;
	if( stoi( string( argv[3] ) ) != 0 )
		is_Mlb_cut = true;
	

	bool is_test = false;
	string option = "normal";
	option = string( argv[4] );

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

	int change_number = stoi( string(argv[5]) );

	string dir_name = string( argv[6] );

	TChain* root = new TChain( "root" );
	root->Add( "/wk_cms2/cychuang/full_sel_16/Nominal/TTbar/full_sel_bpk_ntuple_*.root" );
	
	//*** make histograms ***//

	Hists_Acp hists_acp;
	hists_acp.Init( string("all") );

	string startingtime = get_time_str( minute );

	bool is_data = false;
	
			//*** Register branches ***//

			JetInfo jetInfo ;
			jetInfo.Register( root , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( root , "LepInfo" );
			EvtInfo evtInfo ;
			evtInfo.Register( root );
			GenInfo genInfo;
			genInfo.Register( root );		
			VertexInfo vertexInfo;
			vertexInfo.Register( root , "VertexInfo" );
/*			
			TrgInfo trgInfo;
			trgInfo.Register( files_map[ Set_name ]->at(r).ch , "TrgInfo" );
			RunInfo runInfo;
			runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
			PhotonInfo photonInfo;
			photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
*/

			//*** Read in the known info. ***//

			double t_Weight;
		 	int SelLep;
			int JetsNo;
			int SelJets[20];
			int SelBJets[5];
			//string channel;
			string* channel;

			( root )->SetBranchAddress( "t_Weight", &t_Weight  );
			( root )->SetBranchAddress( "SelLep", &SelLep  );
			( root )->SetBranchAddress( "JetsNo", &JetsNo  );
			( root )->SetBranchAddress( "SelJets", SelJets  );
			( root )->SetBranchAddress( "SelBJets", SelBJets  );
			( root )->SetBranchAddress( "Channel", &channel );
			
			int t_entries = (root)->GetEntries();
			if( is_test ) t_entries = 2000;

			//*** Initialize the selection manager ***//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			sel.SetTrain( training_name );
			if( is_data ) {	sel.SetIsData(is_data);	}

			AcpMgr acpMgr( &leptonInfo, &jetInfo );
			GenMgr genMgr( &genInfo, &jetInfo, &leptonInfo );

			//*** prepare for changing info. method ***//
			int NO = 1000;
			TLorentzVector change_b[NO];
			TLorentzVector change_j1[NO];
			//vector<bool> flag;
			//flag.assign(NO, false);

			for(int i=0;i<NO;i++)
			{
				change_b[i] = TLorentzVector();
			}
			for(int i=0;i<NO;i++)
			{
				change_j1[i] = TLorentzVector();
			}

			int u = 0;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( root )->GetEntry(entry);
				
				sel.reset();

				//** Set lumi_Weight first **//

				//sel.ScaleWeight( lumi_weight );
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

				double w_o3 = 0.;
				double w_o6 = 0.;
				double w_o12 = 0.;
				double w_o13 = 0.;
				if( genMgr.IsPositiveAcp( "Obs3" ) ) 
				{	w_o3 = 1.05;	}
				else
				{	w_o3 = 0.95;	}
				if( genMgr.IsPositiveAcp( "Obs6" ) ) 
				{	w_o6 = 1.05;	}
				else
				{	w_o6 = 0.95;	}
				if( genMgr.IsPositiveAcp( "Obs12" ) ) 
				{	w_o12 = 1.05;	}
				else
				{	w_o12 = 0.95;	}
				if( genMgr.IsPositiveAcp( "Obs13" ) ) 
				{	w_o13 = 1.05;	}
				else
				{	w_o13 = 0.95;	}
				
				TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
				TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
				//TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );
				TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

				//double Mjjb = ( hadb + j1 + j2 ).M();
				double Mlb = ( lepb + lepton ).M() ;
				
				if( is_mva ) {
					if( sel.RecoAlgoValue() < algo_cut ) { continue; }
                }
				else {
					if( sel.RecoAlgoValue() > algo_cut ) { continue; }
                }
				if( is_Mlb_cut ) {
					if( Mlb > 150 ) continue;
				}
			
				int change_to = entry % change_number;
				
				acpMgr.InputSelObjs_p4( change_b[ change_to ], lepb, change_j1[ change_to ], sel.Idx_Lep() );

				hists_acp.FillIn( "Obs6", *channel, acpMgr.Obs6(), w_o6 );
				hists_acp.FillIn( "Obs12", *channel, acpMgr.Obs12(), w_o12 );
				hists_acp.FillIn( "Obs13", *channel, acpMgr.Obs13(), w_o13 );
				hists_acp.FillIn( "Obs3", *channel, acpMgr.Obs3(), w_o3 );
				
				change_b[ change_to ] = hadb;
				change_j1[ change_to ] = j1;

			}	//end of entry for-loop	
			if( is_mva )	
				training_name = sel.GetTrain();

	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string sample_path = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/washout_" + dir_name + "/";
	string new_file_name = sample_path + training_name + string("_ChangeInfo_Sample-") + string(argv[5]) + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	hists_acp.WriteIn();

	f_out->Close();
/*
	ofstream f;
	f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/cor_result.txt",fstream::app );		

	f << "File name " << training_name << endl;
	f << "starting time is : " << startingtime << endl;
	f << "Finish time is : " << time_str << endl;

	f << "==============================================================" << endl; 
*/	
	
	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
}

