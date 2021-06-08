/**********************************
 *
 *	File Name : Make_train_data.cc
 *	Date : 201223
 *	Description : for mva to classify the TT semi and TT dilep
 *	Usage : {exe} {mva/chi2} {algo_cut} {sample name} {lumi_weight} {date} [option]
 *
 *	use the python script to control this program
 *	first make an identified directory with the argument {date} 
 *	and then store the training set in this directory
 *
 *	* ********************************/
#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/about_time.h"
#include "TopCPViolation/select/interface/Tool.h"

using namespace std;
using namespace CalTool;

int main(int argc,char* argv[])
{
	if(  argc != 6 && argc != 7 )
	{
		cerr << "[ERROR] wrong argment numbers!" << endl;
		return 0;
	}
	bool is_mva = false;
	string algo_name = "";
	algo_name = string( argv[1] );
	if( algo_name.find( "mva" ) != string::npos )
		is_mva = true;

	double algo_cut = stod( string( argv[2] ) );

	string SetName = "";
	SetName = string( argv[3] );
	string algoSetName = algo_name + "_" + SetName;

	double lumi_weight = 1.;
	lumi_weight = stod( string( argv[4] ) );

	string date_str = "";
	date_str = string( argv[5] );

	string option = "normal";
	if( argc == 7 )
		if( string( argv[6] ).find( "test" ) != string::npos ) option = "test";

	string training_name = "";

	if( is_mva )
		cout << "With mva sort" << endl;
	else
		cout << "With chi2 sort" << endl;

	cout << "algo-cut is " << algo_cut << endl;

	//--------- Start Analysis ----------//

	string startingtime = get_time_str( minute );

	bool is_data = false;
	if( SetName.find("Data") != string::npos ) { is_data = true; }

	if( is_data )
		cout << "input file is data" << endl;

	//--- to prepare the counting objects ---//
	printf("\n The file now dealing with is with %s selection method and under the data sets %s.\n", algo_name.c_str(), SetName.c_str());

	//--- input the file and tree ---//
	
	TChain* root = new TChain( "root" );
	string input_file_name = "/wk_cms2/cychuang/full_sel_16/Nominal/" + SetName + "/full_sel_bpk_ntuple*.root";
	root->Add( (char*)input_file_name.c_str() );

	//--- Register branches ---//
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

	//--- Read in the known info. ---//
	double t_Weight;
	int SelLep;
	int JetsNo;
	int SelJets[20];
	int SelBJets[5];
	string* channel;

	root->SetBranchAddress( "t_Weight", &t_Weight  );
	root->SetBranchAddress( "SelLep", &SelLep  );
	root->SetBranchAddress( "JetsNo", &JetsNo  );
	root->SetBranchAddress( "SelJets", SelJets  );
	root->SetBranchAddress( "SelBJets", SelBJets  );
	root->SetBranchAddress( "Channel", &channel );
	
	int t_entries = root->GetEntries();
	cout << "input entries : " << t_entries << endl;
	if( option != "test" )
		printf("\nAnd the Entries of this data files are : %d\n",t_entries);
	else	//test
	{
		cout << "Just a test !" << endl;
		t_entries = 1000;
	}

	//--- Initialize the new tree's components ---//
	string newFileName = "/wk_cms2/cychuang/TTdiTT/" + date_str + "/" + algoSetName + "_train_sample.root";
	TFile* newFile = new TFile( newFileName.c_str(), "recreate" );
	TTree* NewTree = new TTree( "Variables", "training variables" );

	double weight;
	double var_JNo;		//transfer the int to double
	double var_j1_Pt;
	double var_j2_Pt;
	double var_j1_Eta;
	double var_j2_Eta;
	double var_j1_Phi;
	double var_j2_Phi;
	double var_j1j2_sumPt;
	double var_j1j2_delPt;
	double var_j1j2_delR;
	double var_j1j2_Pt_r;

    double var_hadb_Pt;
    double var_hadb_Eta;
    double var_hadb_Phi;
    double var_lepb_Pt;
    double var_lepb_Eta;
    double var_lepb_Phi;
    double var_lep_Pt;
    double var_lep_Eta;
    double var_lep_Phi;
    double var_hadbj1_sumPt;
    double var_hadbj1_delPt;
    double var_hadbj1_delR;
    double var_hadbj2_sumPt;
    double var_hadbj2_delPt;
    double var_hadbj2_delR;
    double var_hadblepb_sumPt;
    double var_hadblepb_delPt;
    double var_hadblepb_delR;
    double var_hadblep_sumPt;
    double var_hadblep_delPt;
    double var_hadblep_delR;
    double var_lepblep_sumPt;
    double var_lepblep_delPt;
    double var_lepblep_delR;
    double var_hadb_btagv;
    double var_lepb_btagv;
    double var_reco_algov;
	//SpaceTag
	
	NewTree->Branch( "weight", &weight , "weight/D" );
	NewTree->Branch( "var_JNo", &var_JNo , "var_JNo/D" );
	NewTree->Branch( "var_j1_Pt", &var_j1_Pt , "var_j1_Pt/D" );
	NewTree->Branch( "var_j2_Pt", &var_j2_Pt , "var_j2_Pt/D" );
	NewTree->Branch( "var_j1_Eta", &var_j1_Eta , "var_j1_Eta/D" );
	NewTree->Branch( "var_j2_Eta", &var_j2_Eta , "var_j2_Eta/D" );
	NewTree->Branch( "var_j1_Phi", &var_j1_Phi , "var_j1_Phi/D" );
	NewTree->Branch( "var_j2_Phi", &var_j2_Phi , "var_j2_Phi/D" );
	NewTree->Branch( "var_j1j2_sumPt", &var_j1j2_sumPt , "var_j1j2_sumPt/D" );
	NewTree->Branch( "var_j1j2_delPt", &var_j1j2_delPt , "var_j1j2_delPt/D" );
	NewTree->Branch( "var_j1j2_delR", &var_j1j2_delR , "var_j1j2_delR/D" );
	NewTree->Branch( "var_j1j2_Pt_r", &var_j1j2_Pt_r , "var_j1j2_Pt_r/D" );
	
    NewTree->Branch( "var_hadb_Pt", &var_hadb_Pt , "var_hadb_Pt/D" );
	NewTree->Branch( "var_hadb_Eta", &var_hadb_Eta , "var_hadb_Eta/D" );
	NewTree->Branch( "var_hadb_Phi", &var_hadb_Phi , "var_hadb_Phi/D" );
    NewTree->Branch( "var_lepb_Pt", &var_lepb_Pt , "var_lepb_Pt/D" );
	NewTree->Branch( "var_lepb_Eta", &var_lepb_Eta , "var_lepb_Eta/D" );
	NewTree->Branch( "var_lepb_Phi", &var_lepb_Phi , "var_lepb_Phi/D" );
    NewTree->Branch( "var_lep_Pt", &var_lep_Pt , "var_lep_Pt/D" );
	NewTree->Branch( "var_lep_Eta", &var_lep_Eta , "var_lep_Eta/D" );
	NewTree->Branch( "var_lep_Phi", &var_lep_Phi , "var_lep_Phi/D" );
    NewTree->Branch( "var_hadbj1_sumPt", &var_hadbj1_sumPt , "var_hadbj1_sumPt/D" );
	NewTree->Branch( "var_hadbj1_delPt", &var_hadbj1_delPt , "var_hadbj1_delPt/D" );
	NewTree->Branch( "var_hadbj1_delR", &var_hadbj1_delR , "var_hadbj1_delR/D" );
    NewTree->Branch( "var_hadbj2_sumPt", &var_hadbj2_sumPt , "var_hadbj2_sumPt/D" );
	NewTree->Branch( "var_hadbj2_delPt", &var_hadbj2_delPt , "var_hadbj2_delPt/D" );
	NewTree->Branch( "var_hadbj2_delR", &var_hadbj2_delR , "var_hadbj2_delR/D" );
    NewTree->Branch( "var_hadblepb_sumPt", &var_hadblepb_sumPt , "var_hadblepb_sumPt/D" );
	NewTree->Branch( "var_hadblepb_delPt", &var_hadblepb_delPt , "var_hadblepb_delPt/D" );
	NewTree->Branch( "var_hadblepb_delR", &var_hadblepb_delR , "var_hadblepb_delR/D" );
    NewTree->Branch( "var_hadblep_sumPt", &var_hadblep_sumPt , "var_hadblep_sumPt/D" );
	NewTree->Branch( "var_hadblep_delPt", &var_hadblep_delPt , "var_hadblep_delPt/D" );
	NewTree->Branch( "var_hadblep_delR", &var_hadblep_delR , "var_hadblep_delR/D" );
    NewTree->Branch( "var_lepblep_sumPt", &var_lepblep_sumPt , "var_lepblep_sumPt/D" );
	NewTree->Branch( "var_lepblep_delPt", &var_lepblep_delPt , "var_lepblep_delPt/D" );
	NewTree->Branch( "var_lepblep_delR", &var_lepblep_delR , "var_lepblep_delR/D" );
    NewTree->Branch( "var_hadb_btagv", &var_hadb_btagv , "var_hadb_btagv/D" );
    NewTree->Branch( "var_lepb_btagv", &var_lepb_btagv , "var_lepb_btagv/D" );
    NewTree->Branch( "var_reco_algov", &var_reco_algov , "var_reco_algov/D" );
	//BranchTag

	//--- Initialize the selection manager ---//
	SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
	sel.SetTrain( training_name );
	if( is_data ) {	sel.SetIsData(is_data);	}
	int u = 0;	
	for(int entry=0;entry<(int)t_entries;++entry)
	{
		if( (double)entry/t_entries >= 0.1*u ) {	
			cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
			u++;
		}

		root->GetEntry(entry);
		sel.reset();

		//-- Set lumi_Weight first --//
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
		
		TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
		TLorentzVector lepb = sel.JetP4( sel.Idx_Lepb() );
		TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
		TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );
		TLorentzVector lepton = sel.LepP4( sel.Idx_Lep() );

		double Mjjb = ( hadb + j1 + j2 ).M();
		double Mlb = ( lepb + lepton ).M() ;
	
		//cout << "algo value : " << sel.RecoAlgoValue() << endl;	
		if( is_mva ) {
			if( sel.RecoAlgoValue() < algo_cut ) { continue; }		// algo cut
		}
		else {
			if( sel.RecoAlgoValue() > algo_cut ) { continue; }		// algo cut
		}
		if( Mlb > 150 ) continue;									// M_{lb} cut

		//-- Store the variables we want to train --//
		weight = lumi_weight * t_Weight;
		var_JNo = (double)JetsNo;
		var_j1_Pt = j1.Pt();
		var_j2_Pt = j2.Pt();
		var_j1_Eta = j1.Eta();
		var_j2_Eta = j2.Eta();
		var_j1_Phi = j1.Phi();
		var_j2_Phi = j2.Phi();
		var_j1j2_sumPt = var_j1_Pt + var_j2_Pt;
		var_j1j2_delPt = var_j1_Pt - var_j2_Pt;
		var_j1j2_delR = CalTool::delR( var_j1_Eta, var_j2_Eta, var_j1_Phi, var_j2_Phi );
		var_j1j2_Pt_r = var_j2_Pt/var_j1_Pt;

        var_hadb_Pt = hadb.Pt();
        var_hadb_Eta = hadb.Eta();
        var_hadb_Phi = hadb.Phi();
        var_lepb_Pt = lepb.Pt();
        var_lepb_Eta = lepb.Eta();
        var_lepb_Phi = lepb.Phi();
        var_lep_Pt = lepton.Pt();
        var_lep_Eta = lepton.Eta();
        var_lep_Phi = lepton.Phi();
        var_hadbj1_sumPt = var_hadb_Pt + var_j1_Pt;
        var_hadbj1_delPt = var_hadb_Pt - var_j1_Pt;
        var_hadbj1_delR = CalTool::delR( var_hadb_Eta, var_j1_Eta, var_hadb_Phi, var_j1_Phi );
        var_hadbj2_sumPt = var_hadb_Pt + var_j2_Pt;
        var_hadbj2_delPt = var_hadb_Pt - var_j2_Pt;
        var_hadbj2_delR = CalTool::delR( var_hadb_Eta, var_j2_Eta, var_hadb_Phi, var_j2_Phi );
        var_hadblepb_sumPt = var_hadb_Pt + var_lepb_Pt;
        var_hadblepb_delPt = var_hadb_Pt - var_lepb_Pt;
        var_hadblepb_delR = CalTool::delR( var_hadb_Eta, var_lepb_Eta, var_hadb_Phi, var_lepb_Phi );
        var_hadblep_sumPt = var_hadb_Pt + var_lep_Pt;
        var_hadblep_delPt = var_hadb_Pt - var_lep_Pt;
        var_hadblep_delR = CalTool::delR( var_hadb_Eta, var_lep_Eta, var_hadb_Phi, var_lep_Phi );
        var_lepblep_sumPt = var_lepb_Pt + var_lep_Pt;
        var_lepblep_delPt = var_lepb_Pt - var_lep_Pt;
        var_lepblep_delR = CalTool::delR( var_lepb_Eta, var_lep_Eta, var_lepb_Phi, var_lep_Phi );
        var_hadb_btagv = sel.Hadb_DeepCSV();
        var_lepb_btagv = sel.Lepb_DeepCSV();
        var_reco_algov = sel.RecoAlgoValue();
		
        //-- store entry info. in the tree --//
		NewTree->Fill();
	
	}	//end of entry for-loop
	
	//--- Save the new tree in the new TFile ---//
	NewTree->AutoSave();

	string end_time_str = "";
	end_time_str = get_time_str( minute );
	
	cout << "Starting loop time : " << startingtime << endl;
	cout << "Ending loop time : " << end_time_str << endl;
}

