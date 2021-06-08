/**************************************************************
 *
 *	File Name : full_sel_SR_unc.cc
 *
 *	Description : sel from UncPreSel to uncertainty mode ! 
 *
 *		store the weight up/down information without cut jet energy.
 *		store the JetNo, SelJet, SelbJet ... the nominal selection result.
 *
 *
 * ***********************************************************/

#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/about_time.h"

using namespace std;

int main(int argc,char* argv[])
{

	//*******To prepare the things about MC reweigh********//

	TFile* f1 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/old_lep/muIDSF.root");
	TH2F* h_tightMuIDSF;		f1->GetObject("abseta_pt_ratio",h_tightMuIDSF);

	TFile* f2 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/Lep2016/muiso_Run16All.root");
	TH2D* h_MuISOSF;			f2->GetObject("muiso",h_MuISOSF);

	TFile* f3 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/Lep2016/HLT16_mu_SF.root");
	TH2F* h_MuTrgSF;			f3->GetObject("abseta_pt_ratio",h_MuTrgSF);

	TFile* f4 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/old_lep/elTightIDSF.root");
	TH2F* h_tightElIDSF;		f4->GetObject("EGamma_SF2D",h_tightElIDSF);

	TFile* f5 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/Lep2016/EGM2D_BtoH_GT20GeV_RecoSF_Legacy2016.root");
	TH2F* h_ElRECOSF;			f5->GetObject("EGamma_SF2D",h_ElRECOSF);

	TFile* f6 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/Lep2016/HLT_SF_16.root");
	TH2D* h_ElTrgSF;			f6->GetObject("abseta_pt_ratio",h_ElTrgSF);
	
	TH2F* eff_b;		TH2F* eff_c;		TH2F* eff_l;
	TFile* f7 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/beffPlot_TTbar_0pt6321.root");
	f7->GetObject( "eff_b", eff_b );	f7->GetObject( "eff_c", eff_c );	f7->GetObject( "eff_l", eff_l );

	bool is_data = false;
	if( std::stod( std::string( argv[3] ) ) == 1. ) {	is_data = true;	}

	string tree_name = "root";
	//if( is_data )
	//	tree_name = "bprimeKit/root";

	TChain* root = new TChain( tree_name.c_str() );
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");

	//--- deal with JER/ JES thing ---//

	string option = "normal";
	string jet_energy_sel = "Nominal/";
	if( argc == 5 )		//JER JES
		jet_energy_sel = string( argv[4] );
	else if( argc == 6 ) {
		jet_energy_sel = string( argv[4] );
		option = string( argv[5] );
	}

	int classfied_case = 0;
	if( jet_energy_sel != "Nominal/" )
	{
		if( jet_energy_sel.find( "JER" ) != string::npos )
		{
			if( jet_energy_sel.find( "up" ) != string::npos ) {
				classfied_case = 1;
			}
			else if( jet_energy_sel.find( "down" ) != string::npos ) {
				classfied_case = 2;
			}
		}
		else if( jet_energy_sel.find( "JES" ) != string::npos )
		{
			if( jet_energy_sel.find( "up" ) != string::npos ) {
				classfied_case = 3;
			}
			else if( jet_energy_sel.find( "down" ) != string::npos ) {
				classfied_case = 4;
			}
		}
	}
	
	//--- To capture original files' name ---//

	char star_point_root[10] = "*.root";						//There is a star!!!!!!!!!!!!!!!!!!
	char point_root[10] = ".root";
	char pre_name[500];
	strcpy(pre_name,argv[1]);
	//Ex. pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_"
	
	if( option == "test" )
		strcat(pre_name,point_root);
	else
		strcat(pre_name,star_point_root);
	//Ex.pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_*.root"
	
	root->Add( pre_name );

	//--- To make the output files' name ---//

	// argv[2] ex. TTbar/
	char path_filename[500] = "/wk_cms2/cychuang/full_sel_16/";
	strcat( path_filename, jet_energy_sel.c_str() );
	strcat(path_filename,argv[2]);
	char temp_path_filename[500];
	strcpy( temp_path_filename, path_filename );

	
	//*** Register branches ***//

	EvtInfo evtInfo ;
	evtInfo.Register( root );
	JetInfo jetInfo ;
	jetInfo.Register( root , "JetInfo" );
	LeptonInfo leptonInfo;
	leptonInfo.Register( root , "LepInfo" );
	GenInfo genInfo;
	genInfo.Register( root );		
	TrgInfo trgInfo;
	trgInfo.Register( root , "TrgInfo" );
	VertexInfo vertexInfo;
	vertexInfo.Register( root , "VertexInfo" );
			
	//--- prepare loop file object and new branch things ---//
	int pre_sel_No = 0;	
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
	int total_entries = root->GetEntries();
	
	cout << endl << "At the start of the full-seletion , and the total entries number before full-selection is " << total_entries << endl;
	if(total_entries == 0)	{	not_end = false;	}

	double weight;
	double lep_weight;
	double lep_weight_up;
	double lep_weight_down;
	double btag_weight;
	double btag_weight_up;
	double btag_weight_down;
	double pu_weight;
	double pu_weight_up;
	double pu_weight_down;
	double other_weight;
	int SelLep;
	int SelBJets[5];
	int SelJets[20];
	int JetsNo;
	for(int i=0;i<5;++i)
	{	SelBJets[i] = -1;	}
	for(int i=0;i<20;++i)
	{	SelJets[i] = -1;	}
	string channel;
	
    TTree* root_new;

	//--- prepare plugin object ---//
	HLTMgr hltmgr( &trgInfo );

	BtagMgr bmgr( &jetInfo );
	bmgr.Register_Init_Maps();
	bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );
		
	PileUpMgr pumgr( &evtInfo );
	pumgr.RegInPUvec( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/pileupweights_69200.csv" );
	pumgr.RegInPUvec( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/pileupweights_72660.csv", "up" );
	pumgr.RegInPUvec( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/pileupweights_65740.csv", "down" );

	LeptonSFMgr lepsfmgr;
	lepsfmgr.RegInTH2(h_tightMuIDSF,h_MuISOSF,h_MuTrgSF,h_tightElIDSF,h_ElRECOSF,h_ElTrgSF);

	//--- Initialize the selection manager ---//
	SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
	if( is_data ) 
	{	
		sel.SetIsData(is_data);	
		sel.PlugInHLTTool( &hltmgr );
	}
	else
	{	
		sel.PlugInReweightTool( &bmgr, &lepsfmgr );
		sel.PlugInPUTool( &pumgr );
		sel.PlugInHLTTool( &hltmgr );
	}

	switch( classfied_case ) {
		case 0:
			break;
		case 1:
			sel.Set_Opt_JER( "up" );
			cout << "case : JER up " << endl;
			break;
		case 2:
			sel.Set_Opt_JER( "down" );
			cout << "case : JER down " << endl;
			break;
		case 3:
			sel.Set_Opt_JES( "up" );
			cout << "case : JES up " << endl;
			break;
		case 4:
			sel.Set_Opt_JES( "down" );
			cout << "case : JES down " << endl;
			break;
		default:
				break;
	}

    while( not_end )
    {
		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "full_sel_bpk_ntuple_%d.root", k );
		strcat( path_filename, filename_no );

        TFile* f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );

		root_new->Branch( "t_Weight", &weight, "t_Weight/D" );
		root_new->Branch( "LepWeight", &lep_weight, "LepWeight/D" );
		root_new->Branch( "LepWeight_up", &lep_weight_up, "LepWeight_up/D" );
		root_new->Branch( "LepWeight_down", &lep_weight_down, "LepWeight_down/D" );
		root_new->Branch( "BtagWeight", &btag_weight, "BtagWeight/D" );
		root_new->Branch( "BtagWeight_up", &btag_weight_up, "BtagWeight_up/D" );
		root_new->Branch( "BtagWeight_down", &btag_weight_down, "BtagWeight_down/D" );
		root_new->Branch( "PUWeight", &pu_weight, "PUWeight/D" );
		root_new->Branch( "PUWeight_up", &pu_weight_up, "PUWeight_up/D" );
		root_new->Branch( "PUWeight_down", &pu_weight_down, "PUWeight_down/D" );
		
		root_new->Branch( "OtherWeight", &other_weight, "OtherWeight/D" );
		
		root_new->Branch( "SelLep", &SelLep, "SelLep/I" );
		root_new->Branch( "JetsNo", &JetsNo, "JetsNo/I" );
		root_new->Branch( "SelJets", SelJets, "SelJets[JetsNo]/I" );
		root_new->Branch( "SelBJets", SelBJets, "SelBJets[2]/I" );
		
		root_new->Branch( "Channel", &channel );
				
		int u = 1;	
		for(entry=temp;entry<(int)total_entries;++entry)
		{
			//test!!!
			//if(entry>=3000.) break;
			root->GetEntry(entry);
		
			weight = 1.;	
			lep_weight = 1.;		lep_weight_up = 1.;		lep_weight_down = 1.;
			btag_weight = 1.;		btag_weight_up = 1.;	btag_weight_down = 1.;
			pu_weight = 1.;			pu_weight_up = 1.;		pu_weight_down = 1.;
			other_weight = 1.;
			
			SelLep = -1;
			for(int i=0;i<5;++i)
			{	SelBJets[i] = -1;	}
			for(int i=0;i<20;++i)
			{	SelJets[i] = -1;	}
			JetsNo = 0;
			channel = "";

            if(entry == total_entries - 1)
            {	
				root_new->AutoSave();
                not_end = false;
				break;
				//Here we directly abandon the last event, it is needed to be solve in the future
			}
				
			sel.reset();

			//--- nominal/JER/JES selection result ---//

			if( !is_data )
				sel.JetCorrection();

			if( !sel.SR_select() ) continue;	//MC reweight is included in it

			SelLep = sel.Getidx_Lep(); 

			vector<int>& sel_jets = sel.Sel_Jets();
			vector<int>& sel_b_jets = sel.Sel_BJets();
			JetsNo = (int)sel_jets.size();
			for(int i=0;i<JetsNo;++i) {
				SelJets[i] = sel_jets.at(i);
			}
			for(int i=0;i<(int)sel_b_jets.size();++i) {
				SelBJets[i] = sel_b_jets.at(i);
			}
			channel = sel.GetChannel();

			if( !is_data )
			{
				//-- store nominal weight --//
				weight = sel.GetWeight();
				lep_weight = sel.GetLepWeight();
				btag_weight = sel.GetBtagWeight();
				pu_weight = sel.GetPUWeight();
				other_weight = sel.GetOtherWeight();

				//-- store syst, weight --//
				lep_weight_up = sel.GetLepWeight("up");
				lep_weight_down = sel.GetLepWeight("down");
				btag_weight_up = sel.GetBtagWeight("up");
				btag_weight_down = sel.GetBtagWeight("down");
				pu_weight_up = sel.GetPUWeight("up");
				pu_weight_down = sel.GetPUWeight("down");
			}
			
			//Now Fill in the pre-selection case		
			root_new->Fill();
			pre_sel_No++;
            
            if(root_new->GetEntries() == 100000)
            {
                temp = entry+1;
                root_new->AutoSave();
				k++;
                break;
            }

		}	//end of entry for-loop
	}	//end of while loop	

	//----- Print out the information of full-selection -----//
	cout << "At the end of the full-seletion , and the total entries number before full-selection is " << pre_sel_No << endl;
	//----- make space free -----//
	
	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

