/**********************************
 *
 *	File Name : artificial_acp.cc
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

int main(int argc,char* argv[])
{
	string training_name = "a05_all_MLP";
	double mva_cut = stod( string(argv[1]) );

	cout << "mva-cut is " << mva_cut << endl;
	
	TChain* root = new TChain( "root" );
	root->Add( "/wk_cms2/cychuang/16_full_SR/TTbar/full_sel_bpk_ntuple_*.root" );

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
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//*** Initialize the selection manager ***//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			if( is_data ) {	sel.SetIsData(is_data);	}

			AcpMgr acpMgr( &leptonInfo, &jetInfo );	
			GenMgr genMgr( &genInfo, &jetInfo, &leptonInfo );

			int u = 0;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//if(entry>=3000.) break;		//test
				
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( root )->GetEntry(entry);
				sel.reset();

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

				sel.MVASort();
				
				TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
				TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
				//TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );
				TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

				//double Mjjb = ( hadb + j1 + j2 ).M();
				double Mlb = ( lepb + lepton ).M() ;
				
				if( sel.RecoAlgoValue() < mva_cut ) continue;
				if( Mlb > 150 ) continue;
			
				acpMgr.InputSelObjs_p4( hadb, lepb, j1, sel.Idx_Lep() );

				hists_acp.FillIn( "Obs6", *channel, acpMgr.Obs6(), w_o6 );
				hists_acp.FillIn( "Obs12", *channel, acpMgr.Obs12(), w_o12 );
				hists_acp.FillIn( "Obs13", *channel, acpMgr.Obs13(), w_o13 );
				hists_acp.FillIn( "Obs3", *channel, acpMgr.Obs3(), w_o3 );

			
			}	//end of entry for-loop	
			
	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string sample_path = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/";
	string new_file_name = sample_path + training_name + string("_artificial_acp_") + time_str + ".root";

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

