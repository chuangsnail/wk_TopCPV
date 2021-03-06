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

	string tree_name = "root";
	if( argc == 5 )
	{
		tree_name = string( argv[4] );
	}

	TChain* root = new TChain( tree_name.c_str() );
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");

	//*** To capture original files' name ***//

	char point_root[10] = ".root";
	char pre_name[500];
	strcpy(pre_name,argv[1]);
	//Ex. pre_name now is "/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_003/bpk_ntuple_"
	strcat(pre_name,point_root);
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
			
/*
	RunInfo runInfo;
	runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
	PhotonInfo photonInfo;
	photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
*/
		
	//*** prepare loop file object and new branch things ***//
	int pre_sel_No = 0;	
	int pre_sel_No_mu = 0;
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
	int total_entries = root->GetEntries();
	
	cout << endl << "At the start of the full-seletion , and the total entries number before full-selection is " << total_entries << endl;
	if(total_entries == 0)	{	not_end = false;	}

    TTree* root_new;

	//*** prepare plugin object ***//
	HLTMgr hltmgr( &trgInfo );

	BtagMgr bmgr( &jetInfo );
	bmgr.Register_Init_Maps();
	bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );
		
	PileUpMgr pumgr( &evtInfo );
	pumgr.RegInPUvec( "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/pileupweights_69200.csv" );

	LeptonSFMgr lepsfmgr;
	lepsfmgr.RegInTH2(h_tightMuIDSF,h_MuISOSF,h_MuTrgSF,h_tightElIDSF,h_ElRECOSF,h_ElTrgSF);

	//*** Initialize the selection manager ***//
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

    while( not_end )
    {
		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "test_full_%d.root", k );
		strcat( path_filename, filename_no );

        TFile* f_new = new TFile( path_filename , "recreate" );
        root_new = root->CloneTree( 0 );

		
		int u = 1;	
		for(entry=temp;entry<(int)total_entries;++entry)
		{
			//test!!!
			//if(entry>=3000.) break;
			root->GetEntry(entry);
		
            if(entry == total_entries - 1)
            {	
				root_new->AutoSave();
                not_end = false;
				break;
				//Here we directly abandon the last event, it is needed to be solve in the future
			}
				
			sel.reset();
        			
			if( !sel.full_sel_SR( false ) ) continue;	//MC reweight is included in it

			//Now Fill in the pre-selection case		
			root_new->Fill();
			pre_sel_No++;
			
			if( sel.GetChannel() == "mu" )
			{
				pre_sel_No_mu++;
			}
            
            if(root_new->GetEntries() == 100000)
            {
                temp = entry+1;
                root_new->AutoSave();
				k++;
                break;
            }

		}	//end of entry for-loop
	}	//end of while loop	

	cout << "At the end of the full-seletion , and the total entries number before full-selection is " << pre_sel_No << endl;
	cout << "At the end of the full-seletion , and the total entries(mu) number before full-selection is " << pre_sel_No_mu << endl;
	//*****make space free*****//
	
	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}

