/**********************************
 *
 *	File Name : SR_mva.cc
 *	Date : 200103
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
    if( argc != 2 )
    {
        cerr << "we need 3 arguments! { [exe] [old/new] } " << endl;
        return 0;
    }
	
	bool is_new = false;
    string version = string( argv[1] );

	if( version == "new" )
		is_new = true;
	
	string path_file_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR.txt";

	if( is_new )
	{
		path_file_name = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/full_sel/full_16_SR_nominal.txt";
	}

	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	get_lumi_weight( Weights_map , path_file_name );
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
	get_path( Data_Set_Path , path_file_name );
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

	string startingtime = get_time_str( minute );

    TH1D* h_t_Weight_mu = new TH1D( "h_t_Weight_mu", "", 100, 0., 2. );
    TH1D* h_btag_weight_mu = new TH1D( "h_btag_weight_mu", "", 100, 0., 2. );
    TH1D* h_lepsf_weight_mu = new TH1D( "h_lepsf_weight_mu", "", 100, 0., 2. );
    TH1D* h_pu_weight_mu = new TH1D( "h_pu_weight_mu", "", 100, 0., 2. );
    
	TH1D* h_t_Weight_el = new TH1D( "h_t_Weight_el", "", 100, 0., 2. );
    TH1D* h_btag_weight_el = new TH1D( "h_btag_weight_el", "", 100, 0., 2. );
    TH1D* h_lepsf_weight_el = new TH1D( "h_lepsf_weight_el", "", 100, 0., 2. );
    TH1D* h_pu_weight_el = new TH1D( "h_pu_weight_el", "", 100, 0., 2. );

	for(int k=0;k<(int)files_map.size();k++)		//
	{
		string Set_name = data_sets_name[k];
		bool is_data = false;
		if( Set_name == "Data" ) { is_data = true; }
		if( Set_name != "TT" ) continue;
		
		//loop around in a kind of data set that is files_map[ data_sets_name[k] ] (r-loop)
		for(int r=0;r<(int)files_map[ Set_name ]->size();r++)
		{	
			if(!is_data){
				if( r >= (int)Data_Set_Path[ Set_name ]->size() )
				{	break;	}
			}
			//--- Get lumi_weight ---//
/*
			double lumi_weight = 1.;
			if(!is_data)
			{	lumi_weight = Weights_map[ Set_name ]->at(r);	}
*/

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

			double t_Weight;
            double btag_weight;
            double lepsf_weight;
            double pu_weight;
		 	int SelLep;
			int JetsNo;
			int SelJets[20];
			int SelBJets[5];
			string* channel;

			string pu_weight_name = "";
            if( !is_new )
            {
                pu_weight_name = "OtherWeight";
            }
            else
            {
                pu_weight_name = "PUWeight";
            }

			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "t_Weight", &t_Weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "BtagWeight", &btag_weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "LepWeight", &lepsf_weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( pu_weight_name.c_str(), &pu_weight  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelLep", &SelLep  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "JetsNo", &JetsNo  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelJets", SelJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "SelBJets", SelBJets  );
			( files_map[ Set_name ]->at(r).ch )->SetBranchAddress( "Channel", &channel );
			
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);

			//*** Initialize the selection manager ***//
/*
			SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
			if( is_data ) {	sel.SetIsData(is_data);	}

			AcpMgr acpMgr( &leptonInfo, &jetInfo );
*/
			int u = 0;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//if(entry>=3000.) break;		//test
				
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);
/*
				sel.reset();

				//-- Set lumi_Weight first --//
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
*/
				if( *channel == "mu" )
				{
					h_t_Weight_mu->Fill( t_Weight );
					h_btag_weight_mu->Fill( btag_weight );
					h_lepsf_weight_mu->Fill( lepsf_weight );
					h_pu_weight_mu->Fill( pu_weight );
				}
				else if( *channel == "el" )
				{
					h_t_Weight_el->Fill( t_Weight );
					h_btag_weight_el->Fill( btag_weight );
					h_lepsf_weight_el->Fill( lepsf_weight );
					h_pu_weight_el->Fill( pu_weight );
				}

/*				
				if( *channel == "mu" )
				{

				}
				else if( *channel == "el" )
				{
				}

				if( is_mva )
					if( sel.RecoAlgoValue() < 0.22 ) continue;
				else
					if( sel.RecoAlgoValue() > 20. ) continue;

				if( *channel == "mu" )
				{
				}
				else if( *channel == "el" )
				{
				}


				if( Mlb > 150 ) continue;

				if( *channel == "mu" )
				{
				}
				else if( *channel == "el" )
				{
				}
*/

			}	//end of entry for-loop	
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
			
		}		//end of r for-loop
	}			//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("check_SR_") +  version + "_" + time_str + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	h_t_Weight_mu->Write();
	h_btag_weight_mu->Write();
	h_lepsf_weight_mu->Write();
	h_pu_weight_mu->Write();
	
	h_t_Weight_el->Write();
	h_btag_weight_el->Write();
	h_lepsf_weight_el->Write();
	h_pu_weight_el->Write();

	f_out->Close();

	cout << "starting loop time : " << startingtime << endl;
	cout << "ending loop time : " << time_str << endl;
	//*****make space free*****//
}

