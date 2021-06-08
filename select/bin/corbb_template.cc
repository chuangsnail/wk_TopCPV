/**************************************************************************************
 *
 *	File Name : corbb_MLP_template.cc
 *	Description : for train test!
 *	Date : 191016~
 *	Author : Chen-Yu Chuang
 *
 ****************************************************************************************/

#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/CalAcp.h"
#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/Hists.h"
#include "TopCPViolation/select/interface/about_time.h"
#include "TopCPViolation/select/interface/GenMgr.h"

using namespace std;

int main(int argc,char* argv[])
{
	if( argc != 3 && argc != 2 )
	{
		cerr << "[ERROR] wrong argment numbers!" << endl;
		return 0;
	}
	bool is_mva = false;
	if( string( argv[1] ).find( "mva" ) != string::npos )
		is_mva = true;

	bool is_algo_cut = false;
	double algo_cut = -10.;
	if( argc == 3 ) {
		algo_cut = stod( string( argv[2] ) );
		is_algo_cut = true;
	}

	string training_name = "";

	if( is_mva )
		cout << "With mva sort" << endl;
	else
		cout << "With chi2 sort" << endl;

	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	get_lumi_weight( Weights_map, "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR_nominal.txt" );

	//*********************About path***********************//
	
	//vector<char*> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	vector<string> TT, DY, WJets, VV, ST, QCD, Data_SM, Data_SE;
	map< string, vector<string>* > Data_Set_Path;
	Data_Set_Path[data_sets_name[0]] = &TT;				Data_Set_Path[data_sets_name[1]] = &DY;
	Data_Set_Path[data_sets_name[2]] = &WJets;			Data_Set_Path[data_sets_name[3]] = &VV;
	Data_Set_Path[data_sets_name[4]] = &ST;				Data_Set_Path[data_sets_name[5]] = &QCD;
	Data_Set_Path[d6] = &Data_SM;				Data_Set_Path[d7] = &Data_SE;
	get_path( Data_Set_Path, "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR_nominal.txt"  );
	
	cout << "Finish reading datasets' weights and path" << endl;

	//*****************declare/make some object ( histograms or vector ......etc.)******************//

	Hists_bb histbb;
	histbb.SetAlgoBin(50,0.,1.);
	histbb.Init();

	Hists_cor histcor;
	histcor.SetAlgoBin(50,0.,1.);		//MLP:(0.,1.) BDT(G):(-1.,1.)
	histcor.Init();

	//**********initial the files and TChain and make the file map and weight map***********//

	map< string , vector<File>* > files_map;
	
	vector<File> files_TT;			vector<File> files_DY;
	vector<File> files_WJets;		vector<File> files_VV;
	vector<File> files_ST;			vector<File> files_QCD;

	File f_TT_001((char*)TT[0].c_str());
	files_TT.push_back(f_TT_001);
	for(int i=0;i<(int)DY.size();i++){
		File f_DY((char*)DY[i].c_str());		files_DY.push_back(f_DY);
	}
	for(int i=0;i<(int)WJets.size();i++){
		File f_WJets((char*)WJets[i].c_str());	files_WJets.push_back(f_WJets);
	}
	for(int i=0;i<(int)VV.size();i++){
		File f_VV((char*)VV[i].c_str());		files_VV.push_back(f_VV);
	}
	for(int i=0;i<(int)ST.size();i++){
		File f_ST((char*)ST[i].c_str());		files_ST.push_back(f_ST);
	}	
	for(int i=0;i<(int)QCD.size();i++){
		File f_QCD((char*)QCD[i].c_str());		files_QCD.push_back(f_QCD);
	}

	files_map[data_sets_name[0]] = &files_TT;		files_map[data_sets_name[1]] = &files_DY;
	files_map[data_sets_name[2]] = &files_WJets;	files_map[data_sets_name[3]] = &files_VV;
	files_map[data_sets_name[4]] = &files_ST;		files_map[data_sets_name[5]] = &files_QCD;

	//**************Data Part**************//

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

	//*****prepare cor objects*****//

	double not_tag_t = 0.;
	double not_tag_mu = 0.;
	double not_tag_el = 0.;

	//**********************Start Analysis***********************//

	string startingtime = get_time_str( minute );

	for(int k=0;k<(int)files_map.size();k++)
	{
		double weight = 1.;
		bool is_data = false;

		string Set_name = data_sets_name[k];

		if( Set_name == "Data" )
		{ is_data = true; }
		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{
			//just apply TTMC/Data//
			//if( Set_name != "TT" && Set_name != "Data" )
			if( Set_name != "TT" )	break;	

			if(!is_data){
				if( r >= (int)Data_Set_Path[ Set_name ]->size() )
				{	break;	}
			}
			//Get lumi_weight
			double lumi_weight = 1.;
			if(!is_data)
			{	lumi_weight = Weights_map[ Set_name ]->at(r);	}

			//Register branches

			printf("\n The file now dealing with is under the data sets %s, the %d one.",Set_name.c_str(),r+1);

			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r).ch );
			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r).ch , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r).ch , "LepInfo" );
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
			string* channel;

			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "t_Weight", &t_Weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelLep", &SelLep  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "JetsNo", &JetsNo  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelJets", SelJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelBJets", SelBJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "Channel", &channel );
			
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//*** Initialize the selection manager ***//
			
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			sel.SetTrain( training_name );
			if( is_data ) {	sel.SetIsData(is_data);	}

			GenMgr genMgr( &genInfo, &jetInfo, &leptonInfo );

			int u = 0;
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//test!!!
				//if(entry>=3000.)
				//{	break;	}

				if( (double)entry/t_entries >= 0.1*u )
				{	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);

				sel.reset();

				//** Set lumi_Weight first **//
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

				if( is_mva )
					sel.MVASort();
				else
					sel.Chi2Sort();
				
				if( is_algo_cut ) {
					if( is_mva ) {
						if( sel.RecoAlgoValue() < algo_cut ) { continue; }
					}
					else {
						if( sel.RecoAlgoValue() > algo_cut ) { continue; }
					}
				}

				TLorentzVector p_hadb = sel.JetP4( sel.Idx_Hadb() );
				TLorentzVector p_lepb = sel.JetP4( sel.Idx_Lepb() );
				TLorentzVector p_j1 = sel.JetP4( sel.Idx_J1() );
				TLorentzVector p_j2 = sel.JetP4( sel.Idx_J2() );
				TLorentzVector p_lepton = sel.LepP4( sel.Idx_Lep() );

				//calculate the correctness of mva method
				double Mjjb = ( p_j1 + p_j2 + p_hadb ).M();
				double Mlb = ( p_lepb + p_lepton ).M();
				
				histbb.h_mvamax_mass[*channel]->Fill( Mjjb, sel.RecoAlgoValue(), sel.Weight() );
				histbb.h_mvamax_mass_t->Fill( Mjjb, sel.RecoAlgoValue(), sel.Weight() );

				//To see the part of bbbar separetion of mva part
				
				bb_matching_type bbType = no_match;			//default
				bbType = genMgr.Get_bb_Option( sel.Idx_Hadb(), sel.Idx_Lepb(), sel.Idx_Lep() );
				//***To fill the histogram***//
	
				//Fill the histograms
				switch(bbType)
				{
					case correct:
						histbb.h_correct[*channel]->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histbb.h_correct_t->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histbb.h_lt_correct[*channel]->Fill( Mlb, sel.Weight() );
						histbb.h_lt_correct_t->Fill( Mlb, sel.Weight() );
						break;
					case charge_mis:
						histbb.h_charge_mis[*channel]->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histbb.h_charge_mis_t->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histbb.h_lt_charge_mis[*channel]->Fill( Mlb , sel.Weight() );
						histbb.h_lt_charge_mis_t->Fill( Mlb , sel.Weight() );
						break;
					case mistag:
						histbb.h_mistag[*channel]->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histbb.h_mistag_t->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histbb.h_lt_mistag[*channel]->Fill( Mlb , sel.Weight() );
						histbb.h_lt_mistag_t->Fill( Mlb , sel.Weight() );
						break;
					case no_match:
						histbb.no_match_number++;
						break;
					default:
						histbb.no_match_number++;
				}
			

				//***** do calculate the cor *****//

				int cor_b = -1, cor_j1 = -1, cor_j2 = -1;
        		bool is_good_trained_evt = genMgr.Find_Correct_HadronicTop( cor_b, cor_j1, cor_j2 );
				if( !is_good_trained_evt )
				{	
					not_tag_t += sel.Weight();
					if( *channel == "mu" )
					{ not_tag_mu += sel.Weight(); }
					else if( *channel == "el" )
					{ not_tag_el +=  sel.Weight(); }
				}
				else
				{
					//the denominator is the events which has passed the gen-matching
					histcor.h_chosen[*channel]->Fill(1., sel.Weight() );
					histcor.h_chosen_t->Fill(1., sel.Weight() );

					if( ( sel.Idx_Hadb() == cor_b && sel.Idx_J1() == cor_j1 && sel.Idx_J2() == cor_j2) \
						|| ( sel.Idx_Hadb() == cor_b && sel.Idx_J1() == cor_j2 && sel.Idx_J2() == cor_j1))
					{
						histcor.h_max_mva_cor[*channel]->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histcor.h_max_mva_cor_t->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histcor.h_cor[*channel]->Fill(1., sel.Weight() );
						histcor.h_cor_t->Fill(1., sel.Weight() );
					}
					else
					{
						histcor.h_max_mva_incor[*channel]->Fill( sel.RecoAlgoValue(), sel.Weight() );
						histcor.h_max_mva_incor_t->Fill( sel.RecoAlgoValue(), sel.Weight() );
					}
				}
			}	//end of entry for-loop	
			if( is_mva )	
				training_name = sel.GetTrain();
		}	//end of r for-loop
	}		//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//normalize		
	
	//Save these hists to be a root file
	
	if( !is_mva )
		training_name = "chi2";
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("corbb_") + training_name + "_" + time_str + string(".root");

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );	
	
	histbb.WriteIn();
	histcor.WriteIn();

	f_out->Close();
	
	double t_weight_t = t_IntegralTH1( histcor.h_chosen_t );
	double t_weight_mu = t_IntegralTH1( histcor.h_chosen_mu );
	double t_weight_el = t_IntegralTH1( histcor.h_chosen_el );

	double cor_weight_t = t_IntegralTH1( histcor.h_cor_t );
	double cor_weight_mu = t_IntegralTH1( histcor.h_cor_mu );
	double cor_weight_el = t_IntegralTH1( histcor.h_cor_el );

	histcor.h_cor_t->Divide( histcor.h_chosen_t );
	histcor.h_cor_mu->Divide( histcor.h_chosen_mu );
	histcor.h_cor_el->Divide( histcor.h_chosen_el );

	ofstream f;
	f.open( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/information/cor_result.txt",fstream::app );		

	f << "File name " << training_name << endl;
	f << "Finish time is : " << time_str << endl;

	f << "mis_gen_matching ratio: " << (double)not_tag_t/( t_weight_t + not_tag_t ) << endl;
   	
	f << "cor_mva ratio : " << (double)cor_weight_t/t_weight_t << endl; 
	f << "Error : +-" << (double) histcor.h_cor_t->GetBinError(2) << endl; 	
   	f << "cor_mva_mu ratio : " << (double)cor_weight_mu/t_weight_mu << endl; 	
	f << "Error : +-" << (double) histcor.h_cor_mu->GetBinError(2) << endl; 	
   	f << "cor_mva_el ratio : " << (double)cor_weight_el/t_weight_el << endl; 	
	f << "Error : +-" << (double) histcor.h_cor_el->GetBinError(2) << endl; 	
   	
	f << "no-match number : " <<  histbb.no_match_number << endl;
	f << "==============================================================" << endl; 
	
	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
}

