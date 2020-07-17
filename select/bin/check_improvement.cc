/**********************************
 *
 *	File Name : check_improvement.cc
 *	Date : 200520
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

	double mva_cut = 0.;

	if( is_mva )
	{
		if( argc != 3 ) return 0;
		else
		{
			mva_cut = stod( string( argv[2] ) );
		}
	}

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
	get_lumi_weight( Weights_map , "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR.txt" );
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

	double N_s1_mu[4] = {0.,0.,0.,0.};
	double N_s1_el[4] = {0.,0.,0.,0.};

	double N_s2_mu[4] = {0.,0.,0.,0.};
	double N_s2_el[4] = {0.,0.,0.,0.};
	
	double N_s3_mu[4] = {0.,0.,0.,0.};
	double N_s3_el[4] = {0.,0.,0.,0.};
	
	string startingtime = get_time_str( minute );

	for(int k=0;k<(int)files_map.size();k++)		//
	{
		string Set_name = data_sets_name[k];
		bool is_data = false;
		if( Set_name == "Data" ) { is_data = true; }
		
		//--- for specific sample ---//
		if( Set_name != "TT" ) continue;

        bool is_TT = false;
        if( Set_name != "TT" ) is_TT = true;

		cout << "Processing sample " << Set_name << endl;
		
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
		 	int SelLep;
			int JetsNo;
			int SelJets[20];
			int SelBJets[5];
			//string channel;
			string* channel;

			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "t_Weight", &t_Weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelLep", &SelLep  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "JetsNo", &JetsNo  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelJets", SelJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelBJets", SelBJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "Channel", &channel );
			
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//--- Initialize the selection manager ---//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			sel.SetTrain( training_name );
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

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
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

				if( !is_mva ) {
					sel.Chi2Sort();
				}
				else {
					sel.MVASort();
				}
				
				TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
				TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
				TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );
				TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

				double Mjjb = ( hadb + j1 + j2 ).M();
				double Mlb = ( lepb + lepton ).M() ;
				
                int int_bbtype = 3;
                if( is_TT )
                {
                    bb_matching_type bbType = no_match;			//default
                    bbType = genMgr.Get_bb_Option( sel.Idx_Hadb(), sel.Idx_Lepb(), sel.Idx_Lep() );
                    //Fill the histograms
                    switch(bbType)
                    {
                        case correct:
                            int_bbtype = 0;
                            break;
                        case charge_mis:
                            int_bbtype = 1;
                            break;
                        case mistag:
                            int_bbtype = 2;
                            break;
                        case no_match:
                            int_bbtype = 3;
                            break;
                    }
                }
				
				if( is_TT )
				{
					if( *channel == "mu" ) {
						N_s1_mu[int_bbtype] += sel.Weight();
					}
					else if( *channel == "el" ) {
						N_s1_el[int_bbtype] += sel.Weight();
					}
				}

				if( !is_mva ) {
					if( sel.RecoAlgoValue() > 20. ) continue;	
				}
				else if( is_mva ) {
					if( sel.RecoAlgoValue() < mva_cut ) continue;		//0.22 for a05, 0.2 for a04
				}


				if( is_TT )
				{
					if( *channel == "mu" ) {
						N_s2_mu[int_bbtype] += sel.Weight();
					}
					else if( *channel == "el" ) {
						N_s2_el[int_bbtype] += sel.Weight();
					}
				}

				if( Mlb > 150. ) continue;

				if( is_TT )
				{
					if( *channel == "mu" ) {
						N_s3_mu[int_bbtype] += sel.Weight();
					}
					else if( *channel == "el" ) {
						N_s3_el[int_bbtype] += sel.Weight();
					}
				}

			}	//end of entry for-loop	
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
			training_name = sel.GetTrain();
			
		}		//end of r for-loop
	}			//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );

	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;

	ofstream fout;
	fout.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/check_improvement.txt",ios::app );

	fout << "Just ttbar sample" << endl;
	fout << "Training Name : " << training_name << endl; 
	fout << "--- Muon channel ---" << endl;
	fout << "                  [ cor, cmis, mist, nomatch ]" << endl;
	fout << "step 1 (No Cut)   [" << N_s1_mu[0] << "," << N_s1_mu[1] << "," << N_s1_mu[2] << "," << N_s1_mu[3] << "]  cor rate:" << N_s1_mu[0]/(N_s1_mu[0]+N_s1_mu[1]+N_s1_mu[2]) << endl;
	fout << "					events efficiency : 1" << endl; 
	fout << "step 1 (algo Cut) [" << N_s2_mu[0] << "," << N_s2_mu[1] << "," << N_s2_mu[2] << "," << N_s2_mu[3] << "]  cor rate:" << N_s2_mu[0]/(N_s2_mu[0]+N_s2_mu[1]+N_s2_mu[2]) << endl; 
	fout << "					events efficiency : " << (N_s2_mu[0]+N_s2_mu[1]+N_s2_mu[2])/(N_s1_mu[0]+N_s1_mu[1]+N_s1_mu[2]) << endl; 
	fout << "step 1 (Mlb Cut)  [" << N_s3_mu[0] << "," << N_s3_mu[1] << "," << N_s3_mu[2] << "," << N_s3_mu[3] << "]  cor rate:" << N_s3_mu[0]/(N_s3_mu[0]+N_s3_mu[1]+N_s3_mu[2]) << endl;
	fout << "					events efficiency : " << (N_s3_mu[0]+N_s3_mu[1]+N_s3_mu[2])/(N_s1_mu[0]+N_s1_mu[1]+N_s1_mu[2]) << endl; 
	
	fout << "--- Electron channel ---" << endl;
	fout << "                  [ cor, cmis, mist ]" << endl;
	fout << "step 1 (No Cut)   [" << N_s1_el[0] << "," << N_s1_el[1] << "," << N_s1_el[2] << "," << N_s1_el[3] << "]  cor rate:" << N_s1_el[0]/(N_s1_el[0]+N_s1_el[1]+N_s1_el[2]) << endl;
	fout << "					events efficiency : 1" << endl; 
	fout << "step 1 (algo Cut) [" << N_s2_el[0] << "," << N_s2_el[1] << "," << N_s2_el[2] << "," << N_s2_el[3] << "]  cor rate:" << N_s2_el[0]/(N_s2_el[0]+N_s2_el[1]+N_s2_el[2]) << endl;
	fout << "					events efficiency : " << (N_s2_el[0]+N_s2_el[1]+N_s2_el[2])/(N_s1_el[0]+N_s1_el[1]+N_s1_el[2]) << endl; 
	fout << "step 1 (Mlb Cut)  [" << N_s3_el[0] << "," << N_s3_el[1] << "," << N_s3_el[2] << "," << N_s3_el[3] << "]  cor rate:" << N_s3_el[0]/(N_s3_el[0]+N_s3_el[1]+N_s3_el[2]) << endl;
	fout << "					events efficiency : " << (N_s3_el[0]+N_s3_el[1]+N_s3_el[2])/(N_s1_el[0]+N_s1_el[1]+N_s1_el[2]) << endl; 
 
	fout << "starting loop time : " << startingtime << endl;
	fout << "ending loop time : " << time_str << endl;

	//*****make space free*****//
}

