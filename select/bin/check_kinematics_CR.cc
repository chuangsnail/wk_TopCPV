/**********************************
 *
 *	File Name : check_kinematics_CR.cc
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

#include <algorithm>


using namespace std;

int main(int argc,char* argv[])
{
	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	get_lumi_weight( Weights_map , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_CR.txt" );
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
	get_path( Data_Set_Path , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_CR.txt" );
	//TT.push_back(string("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/"));
	cout << "Finish getting Path info." << endl;

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

	//**************** open the file we want to store the result **********************************//
	
	//initialize ntuple must be after the initialization of the TFile
	
	//*****************declare/make some object ( histograms or vector ......etc.)******************//
	//*** make histograms ***//
	Hists hists;
	hists.SetAxisInfo( " ;Leading Jet Pt;Events[No.]", "" );
	//hists.SetBinInfo( 50, 0., 200. );	//for LepPt
	hists.SetBinInfo( 60, -3., 9. );	//for Eta
	hists.NoCutModeON();
	hists.OneCutModeON();
	hists.TwoCutModeON();

	string startingtime = get_time_str( minute );


	for(int k=0;k<(int)files_map.size();k++)		//
	{
		string Set_name = data_sets_name[k];
		bool is_data = false;
		if( Set_name == "Data" ) { is_data = true; }
		//if( Set_name != "TT" ) continue;
		
		//*** to prepare the counting objects ***//

		double NO_ncut_mu = 0.;
		double NO_1cut_mu = 0.;
		double NO_2cut_mu = 0.;

		double NO_ncut_el = 0.;
		double NO_1cut_el = 0.;
		double NO_2cut_el = 0.;
		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{	
			if(!is_data){
				if( r >= (int)Data_Set_Path[ Set_name ]->size() ) break;
			}
			//*** Get lumi_weight ***//
			double lumi_weight = 1.;
			if(!is_data)
			{	lumi_weight = Weights_map[ Set_name ]->at(r);	}

			//*** Register branches ***//

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
/*			
			TrgInfo trgInfo;
			trgInfo.Register( files_map[ Set_name ]->at(r).ch , "TrgInfo" );
			RunInfo runInfo;
			runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
			PhotonInfo photonInfo;
			photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
*/

			double t_Weight;
			double lep_Weight;
			double other_Weight;
		 	int SelLep;
			int JetsNo;
			int SelJets[20];
			int SelBJets[5];
			//string channel;
			string* channel;

			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "t_Weight", &t_Weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "LepWeight", &lep_Weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "OtherWeight", &other_Weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelLep", &SelLep  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "JetsNo", &JetsNo  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelJets", SelJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelBJets", SelBJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "Channel", &channel );
			
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//*** Initialize the selection manager ***//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			if( is_data ) {	sel.SetIsData(is_data);	}

			AcpMgr acpMgr( &leptonInfo, &jetInfo );

			int u = 0;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//if(entry>=3000.) break;		//test
				
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;	u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
				sel.reset();

				//** Set lumi_Weight first **//

				sel.ScaleWeight( lumi_weight );
				//sel.ScaleWeight( t_Weight );
				double Weight_wobtag = lep_Weight * other_Weight;
				sel.ScaleWeight( Weight_wobtag );
				
				vector<int> sel_jets;
				vector<int> sel_b_jets;

				for(int i=0;i<JetsNo;++i)	{	sel_jets.push_back( SelJets[i] );		}
				for(int i=0;i<2;++i)		{	sel_b_jets.push_back( SelBJets[i] );	}

				sel.SetSelJets( sel_jets );
				sel.SetSelBJets( sel_b_jets );

				sel.Setidx_Lep( SelLep );

				sel.Chi2Sort();
				
				TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
				TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
				TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );
				TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

				double Mjjb = ( hadb + j1 + j2 ).M();
				double Mlb = ( lepb + lepton ).M() ;
				
				map< double, TLorentzVector > Pt_List;

				Pt_List[ hadb.Pt() ] = hadb;
				Pt_List[ lepb.Pt() ] = lepb;
				Pt_List[ j1.Pt() ] = j1;
				Pt_List[ j2.Pt() ] = j2;

				auto iter = Pt_List.end();
				iter--;
				if( entry % 1000 == 0 )
					cout << iter->second.Pt() << endl;

				double kk = (iter->second).Eta();
				
				if( *channel == "mu" )
					NO_ncut_mu += sel.Weight();
				else if( *channel == "el" )
					NO_ncut_el += sel.Weight();
				hists.FillHist( "NC", k, *channel, kk, 0., sel.Weight() );
				
				if( sel.RecoAlgoValue() > 20. ) continue;
				if( *channel == "mu" )
					NO_1cut_mu += sel.Weight();
				else if( *channel == "el" )
					NO_1cut_el += sel.Weight();
				hists.FillHist( "1C", k, *channel, kk, 0., sel.Weight() );


			}	//end of entry for-loop	
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;			
		}		//end of r for-loop
		
		ofstream f;
		f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/CR_kinematics.txt",fstream::app );		
		
		f << endl << setw(15) << left << "Sample" << right << ":" << setw(15) << right << Set_name << endl;
		f <<  setw(15) << left << "No Cut(mu)" << right << ":" << setw(15) << right << NO_ncut_mu << endl;
		f <<  setw(15) << left << "No Cut(el)" << right << ":" << setw(15) << right << NO_ncut_el << endl;
		f <<  setw(15) << left << "1 Cut(mu)" << right << ":" << setw(15) << right << NO_1cut_mu << endl;
		f <<  setw(15) << left << "1 Cut(el)" << right << ":" << setw(15) << right << NO_1cut_el << endl;
		//f <<  setw(15) << left << "2 Cut(mu)" << right << ":" << setw(15) << right << NO_2cut_mu << endl;
		//f <<  setw(15) << left << "2 Cut(el)" << right << ":" << setw(15) << right << NO_2cut_el << endl;
		for(int i=0;i<30;i++) f << "=";

		f.close();

	}			//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//
	// apply QCD data-driven here
/*
	TH1* h_data_mu;
	TH1* h_data_el;
	TH1* h_data_mu_c;
	TH1* h_data_el_c;

	string dd_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/" + string( argv[1] );
	TFile* dd_file = new TFile( (char*)dd_name.c_str() );
	dd_file->GetObject("h_Data_mu",h_data_mu);
	dd_file->GetObject("h_Data_el",h_data_el);
	dd_file->GetObject("h_Data_mu_c",h_data_mu_c);
	dd_file->GetObject("h_Data_el_c",h_data_el_c);

	DataDriven( hists.h_QCD_mu, h_data_mu );
	DataDriven( hists.h_QCD_el, h_data_el );
	DataDriven( hists.h_QCD_mu_c, h_data_mu_c );
	DataDriven( hists.h_QCD_el_c, h_data_el_c );
*/

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("check_LJetEta_CR_") + time_str + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	//*** store! ***//
	hists.WriteIn("NC 1C");

	f_out->Close();

	//dd_file->Close();
	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
	//*****make space free*****//
}

