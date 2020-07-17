/**-----------------------------------------
 *
 * FileName : UncPreSel.cc
 * Description : 
 * 		SR lepton selection
 * 		SR jets selection w/o Pt cut 
 * 		SR b-jets selection
 *
 * 		not store any weight 
 * 		not apply JER, trigger, pileup, genWeight, leptonSF and btagSF
 *
 * 		just store, do NOT store anything else.
 *
 * ---------------------------------------**/



#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/about_time.h"

using namespace std;

int main(int argc,char* argv[])
{

	//--- check if it is a testing run ---/

	string option = "";
	if( argc == 5 )
		option = string( argv[4] );


	TChain* root = new TChain( "root" );
    //root->Add("/wk_cms2/yichen/bpk_ntuple/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/dir_001/bpk_ntuple_*.root");

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

	char path_filename[500] = "/wk_cms2/cychuang/UncPreSel_NS/";
	strcat(path_filename,argv[2]);
	char temp_path_filename[500];
	strcpy( temp_path_filename, path_filename );

	bool is_data = false;
	if( std::stod( std::string( argv[3] ) ) == 1. ) {	is_data = true;	}
	
	//--- Register branches ---//

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

	ClearNoUseTree( root );
			
	//--- prepare loop file object and new branch things ---//
	int pre_sel_No = 0;	
    int k = 1;
	int r = 1;
    int entry = 0;
    int temp = 0;
    bool not_end = true;
	int total_entries = root->GetEntries();
	
	cout << endl << "At the start of the seletion , and the total entries number before UncPreSel is " << total_entries << endl;
	if(total_entries == 0)	{	not_end = false;	}

    TTree* root_new;
	
	SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &vertexInfo, &genInfo );

	if( is_data ) {
		cerr << "This program is not for real data!" << endl;
		return 0;
	}

    while( not_end )
    {
		strcpy( path_filename, temp_path_filename );
		char filename_no[50] = "";
		sprintf( filename_no, "pre_sel_bpk_ntuple_%d.root", k );
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
        			
			if( !sel.UncPreSel() ) continue;	//MC reweight is included in it

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

	if( option == "test" )
	{
		cout << "test events ratio : " << (double)pre_sel_No/(double)total_entries << endl;
	}

	//-----make space free-----//
}

