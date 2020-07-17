/**********************************
 *
 *	File Name : Cal_Acp.cc
 *	Date : 200103
 *	
 *
 *
 *
 *
 * ********************************/


#include "TopCPViolation/select/interface/SelMgr.h"
#include "TopCPViolation/select/interface/CalAcp.h"
#include "TopCPViolation/select/interface/MVAvar.h"
#include "TopCPViolation/select/interface/BranchInfo.h"
#include "TopCPViolation/select/interface/File.h"
#include "TopCPViolation/select/interface/dataset.h"
#include "TopCPViolation/select/interface/Hists.h"
#include "TopCPViolation/select/interface/about_time.h"
#include "/wk_cms2/cychuang/CMSSW_9_4_2/src/wk_mva/p03_0-2/weights/TMVAClassification_MLP.class.C"


using namespace std;

/*
int main()
{
	TChain* root = new TChain("bprimeKit/root");
	root->Add("/wk_cms2/youying/public/bprimekit_2016_Legacy/TTToSemiLeptonic_TuneCP5_PSweights_13TeV-powheg-pythia8/bpk_ntuple_1.root");

	TH1F* h = new TH1F("h","",50,0.,500.);

	JetInfo jetInfo;
	jetInfo.Register( root, "JetInfo" );
	LeptonInfo leptonInfo;
	leptonInfo.Register( root, "LepInfo" );
	EvtInfo evtInfo ;
	evtInfo.Register( root );
	GenInfo genInfo;
	genInfo.Register( root );
	TrgInfo trgInfo;
	trgInfo.Register( root, "TrgInfo" );

	SelMgr sel( &jetInfo, &leptonInfo, &evtInfo, &genInfo );

	int total_entries = root->GetEntries();
	for(int i=0;i<total_entries;i++)
	{
		root->GetEntry(i);
		cout << "In entry : " << i << " {" << endl;
		//sel.test_sel_print();
		
		sel.reset();
		if( !sel.SR_select() ) continue;

		TLorentzVector b = sel.JetP4( sel.Getidx_Hadb() );
		TLorentzVector j1 = sel.JetP4( sel.Getidx_J1() );
		TLorentzVector j2 = sel.JetP4( sel.Getidx_J2() );

		double mass = ( b + j1 + j2 ).M();
		h->Fill( mass, sel.GetWeight() );

		cout << "}" << endl;
	}

	TFile* f = new TFile("test_new_frame.root","recreate");
	h->Write();
	f->Close();
}
*/


using namespace std;
using namespace mvatool;

int main(int argc,char* argv[])
{
	cout << "The mva cut is on:" << argv[1] << endl;
	double mva_cut = stod( string(argv[1]) );
	string training_name = string("p03") +  string("_MLP");
	
	string data_sets_name[7] = {"TT","DY","WJets","VV","ST","QCD","Data"};
	string d6 = data_sets_name[6] + "_SM";
	string d7 = data_sets_name[6] + "_SE";
	
	//***********To finish something about weight***********//

	vector<double> w_TT, w_DY, w_WJets, w_VV, w_ST, w_QCD;
	map< string, vector<double>* > Weights_map;
	Weights_map[data_sets_name[0]] = &w_TT;				Weights_map[data_sets_name[1]] = &w_DY;
	Weights_map[data_sets_name[2]] = &w_WJets;			Weights_map[data_sets_name[3]] = &w_VV;
	Weights_map[data_sets_name[4]] = &w_ST;				Weights_map[data_sets_name[5]] = &w_QCD;
	get_lumi_weight( Weights_map , "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./full_sel_data.txt" );
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
	get_path( Data_Set_Path , "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/selected/info./full_sel_data.txt" );
	cout << "Finish getting Path info." << endl;

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
	TFile* f7 = new TFile("/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/data/beffPlot_TTbar_0pt6321.root");
	f7->GetObject( "eff_b", eff_b );	f7->GetObject( "eff_c", eff_c );	f7->GetObject( "eff_l", eff_l );


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

	//******prepare mva object******//
	
	vector<string> inputVars;
	mvatool::AddVarName( inputVars );
	ReadMLP MyMVA( inputVars );

	//**********************Start Analysis***********************//

	//**************** open the file we want to store the result **********************************//

	/*
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("SR_") + training_name + "_" + time_str + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );
	*/
	//initialize ntuple must be after the initialization of the TFile
	
	//*****************declare/make some object ( histograms or vector ......etc.)******************//
	//*** make histograms ***//
	Hists hists;

	//double* ntvar;		//for temporally store the ntuple vars
	//hists.NtupleModeON();
	
	for(int k=0;k<(int)files_map.size();k++)		//
	{
		bool is_data = false;

		string Set_name = data_sets_name[k];

		if( Set_name == "Data" ) { is_data = true; }

		//if( Set_name != "WJets" ) continue;
		
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

			EvtInfo evtInfo ;
			evtInfo.Register( files_map[ Set_name ]->at(r).ch );
			JetInfo jetInfo ;
			jetInfo.Register( files_map[ Set_name ]->at(r).ch , "JetInfo" );
			LeptonInfo leptonInfo;
			leptonInfo.Register( files_map[ Set_name ]->at(r).ch , "LepInfo" );
			GenInfo genInfo;
			genInfo.Register( files_map[ Set_name ]->at(r).ch );		
			TrgInfo trgInfo;
			trgInfo.Register( files_map[ Set_name ]->at(r).ch , "TrgInfo" );
			VertexInfo vertexInfo;
			vertexInfo.Register( files_map[ Set_name ]->at(r).ch , "VertexInfo" );
/*
			RunInfo runInfo;
			runInfo.Register( files_map[ Set_name ]->at(r).ch , "RunInfo" );
			PhotonInfo photonInfo;
			photonInfo.Register( files_map[ Set_name ]->at(r).ch , "PhotonInfo" );
*/
			
			int t_entries = (files_map[ Set_name ]->at(r).ch)->GetEntries();
			printf("\nAnd the Entries of this data files are : %d\n",t_entries);
			string channel = "";	//lep is muon or electron

			AcpMgr acpMgr( leptonInfo, jetInfo );

			//*** prepare plugin object ***//
			HLTMgr hltmgr( &trgInfo );

			BtagMgr bmgr( &jetInfo );
			bmgr.Register_Init_Maps();
			bmgr.Register_Init_TH2( eff_b, eff_c, eff_l );
		
			PileUpMgr pumgr( &evtInfo );
			pumgr.RegInPUvec( "/wk_cms2/cychuang/CMSSW_9_4_2/src/TopCPViolation/data/pileupweights_69200.csv" );

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
			

			int u = 1;	
			for(int entry=0;entry<(int)t_entries;++entry)
			{
				//test!!!
				//if(entry>=3000.) break;
				
				if( (double)entry/t_entries >= 0.1*u ) {	
					cout << "At percentage : " << (double)0.1 * u * 100 << " %" << endl;
					u++;
				}

				( files_map[ Set_name ]->at(r).ch )->GetEntry(entry);

				//sel.reset();

				//** Set lumi_Weight first **//
				sel.ScaleWeight( lumi_weight );		//data's lumi-weight == 1e
				if( !sel.SR_select() ) continue;	//MC reweight is included in it

				//use mva to choose best choice of reconstruct top quark's mass 

				vector<int>& sel_jets = sel.Get_seljets();
				vector<int>& sel_b_jets = sel.Get_selbjets();

				double max_mva_value = -10.;
				double had_t_mass = -1.;
				double lep_t_mass = -1.;
				int mva_hadb = -1, mva_j1 = -1, mva_j2 = -1;
				int mva_lepb = -1 ;
				int var_num = (int)inputVars.size();
				double* var = new double[ var_num ];
				for(int B=0;B<(int)sel_b_jets.size();B++)
				{
					int lB = (B == 0) ? 1 : 0 ;
					for(int J1=0;J1<(int)sel_jets.size();J1++)
					{
						for(int J2=J1+1;J2<(int)sel_jets.size();J2++)
						{
							int tmp_mva_lepb = sel_b_jets.at(lB);
							int tmp_mva_hadb = sel_b_jets.at(B);
							int tmp_mva_j1 = sel_jets.at(J1);
							int tmp_mva_j2 = sel_jets.at(J2);
							
							mvatool::InputVar( var, jetInfo, leptonInfo, evtInfo, sel_jets, sel_b_jets, sel.Getidx_lep(), tmp_mva_hadb, tmp_mva_lepb, tmp_mva_j1, tmp_mva_j2 );
							
							vector<double> inputValues;
							for(int in=0;in<var_num;in++ )
							{	inputValues.push_back( var[in] );	}
								
							double tmp_mva_value = MyMVA.GetMvaValue( inputValues );
							if( tmp_mva_value >= max_mva_value )
							{
								max_mva_value = tmp_mva_value;
								mva_hadb = tmp_mva_hadb;
								mva_j1 = tmp_mva_j1;
								mva_j2 = tmp_mva_j2;
								mva_lepb = tmp_mva_lepb;
							}
						}
					}
				}

				delete [] var;
				
				//check mva-reco
				if( max_mva_value == -10. || mva_lepb == -1 )
				{
					cerr << "There is something wrong in the process of mva-reco " << endl;
					continue;	
				}

				acpMgr.InputSelObjs( mva_hadb, mva_lepb, mva_j1, sel.Getidx_Lep() );

				/*** start cal Acp ***/
					

			}	//end of entry for-loop	
			cout << endl << "The end of the file-sets " << Set_name << " " << r+1 << " " << endl;
			
		}		//end of r for-loop
	}			//end of k for-loop

	//*****Drawing Plotting or Outputting files*****//

	//Save these hists to be a root file
	
	string time_str = "";
	time_str = get_time_str( minute );
	string new_file_name = string("SR_") + training_name + "_" + time_str + ".root";

	TFile* f_out = new TFile( new_file_name.c_str() , "recreate" );

	//*** store! ***//
	hists.WriteIn("NC 1C 2C");

	f_out->Close();

	//*****make space free*****//
	
	delete f7;
	delete f6;
	delete f5;
	delete f4;
	delete f3;
	delete f2;
	delete f1;
}



