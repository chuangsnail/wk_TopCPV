/*
#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/about_time.h"
*/

#include "../interface/SelMgr.h"
#include "../interface/File.h"
#include "../interface/dataset.h"
#include "../interface/about_time.h"

using namespace std;

int main(int argc,char* argv[])
{

	//*******To prepare the things about MC reweigh********//

	TChain* root = new TChain( "bprimeKit/root" );
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");

	//*** To capture original files' name ***//

	string option = "";
	if( argc == 5 )
		option = string( argv[4] );

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

	//*** To make the output files' name ***//

	char path_filename[500] = "/wk_cms2/cychuang/";
	strcat(path_filename,argv[2]);
	char temp_path_filename[500];
	strcpy( temp_path_filename, path_filename );

	bool is_data = false;
	if( std::stod( std::string( argv[3] ) ) == 1. ) {	is_data = true;	}
	
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
			
	//*** prepare loop file object and new branch things ***//
	
	TH1D* h_Mjj = new TH1D( "h_Mjj", "", 600, 0., 600.);
	TH1D* h_Mjjb = new TH1D( "h_Mjjb", "", 1000, 0., 1000.);

	int pre_sel_No = 0;	
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
	int total_entries = root->GetEntries();
	
	if(total_entries == 0)	{	not_end = false;	}

	double weight;		//from pileup and genweight ( member 'otherweight' in SelMgr)
	double gm_Mjj;
	double gm_Mjjb;

	string channel;
	
    TTree* root_new;

	//*** prepare plugin object ***//

	string pu_filename = "";
	if( string(pre_name).find("2016") != string::npos ) {
		pu_filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/base_data/pileupweights_16_69200.csv";
	}
	else if( string(pre_name).find("2017") != string::npos ) {
		pu_filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/base_data/pileupweights_17_69200.csv";
	}
	else if( string(pre_name).find("2018") != string::npos ) {
		pu_filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/base_data/pileupweights_18_69200.csv";
	}
	else {
		pu_filename = "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/pileupweights_69200.csv";
	}

	PileUpMgr pumgr( &evtInfo );
	pumgr.RegInPUvec( (char*)pu_filename.c_str() );

	//*** Initialize the selection manager ***//
	SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );
	if( is_data ) 
	{	
		sel.SetIsData(is_data);
		cerr << "It's just for MC sample!" << endl;
		return 0;	
	}
	else
	{	
		//sel.PlugInReweightTool( &bmgr, &lepsfmgr );
		sel.PlugInPUTool( &pumgr );
	}

    while( not_end )
    {
		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "GenMatchingMass_%d.root", k );
		strcat( path_filename, filename_no );

        TFile* f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );
		root_new->SetBranchStatus( "*", 0 );

		root_new->Branch( "gm_Mjj", &gm_Mjj, "gm_Mjj/D" );
		root_new->Branch( "gm_Mjjb", &gm_Mjjb, "gm_Mjjb/D" );
		
		root_new->Branch( "Channel", &channel );
		root_new->Branch( "Weight", &weight, "Weight/D" );
		
		int u = 1;	
		for(entry=temp;entry<(int)total_entries;++entry)
		{
			//test!!!
			//if(entry>=3000.) break;
			root->GetEntry(entry);
	
			weight = 1.;	
			channel = "";
			gm_Mjj = 0.;
			gm_Mjjb = 0.;

            if(entry == total_entries - 1)
            {	
				root_new->AutoSave();
                not_end = false;
				break;
				//Here we directly abandon the last event, it is needed to be solve in the future
			}
				
			sel.reset();
		
			if( !sel.Find_GenTW() ) continue;	//MC reweight is included in it
			
			weight = sel.GetOtherWeight();
			channel = sel.GetChannel();
			//cout << weight << endl;

			TLorentzVector hadb = sel.JetP4( sel.Idx_Hadb() );
			TLorentzVector j1 = sel.JetP4( sel.Idx_J1() );
			TLorentzVector j2 = sel.JetP4( sel.Idx_J2() );

			gm_Mjj = ( j1 + j2 ).M();
			gm_Mjjb = ( j1 + j2 + hadb ).M();

			h_Mjj->Fill( gm_Mjj, weight );
			h_Mjjb->Fill( gm_Mjjb, weight );
			
			//Now Fill in the pre-selection case		
			root_new->Fill();
			pre_sel_No++;
            
            if(root_new->GetEntries() == 1000000)
            {
                temp = entry+1;
                root_new->AutoSave();
				k++;
                break;
            }

		}	//end of entry for-loop
		f_new->Close();
	}	//end of while loop	

	cout << "pre_sel_No : " << pre_sel_No << endl;

	string store_path = "/wk_cms2/cychuang/" + string(argv[2]) + "Mass_Hist.root";
	TFile* fn = new TFile( (char*)store_path.c_str(), "recreate" );
	h_Mjj->Write();
	h_Mjjb->Write();
	fn->Close();

	//*****make space free*****//
}

