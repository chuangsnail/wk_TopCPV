#include "TopCPViolation/select/interface/SelMgr.h"
//#include "TopCPViolation/select/interface/Reco.h"
//#include "../interface/SelMgr.h"

#include "TopCPViolation/select/interface/MVAvar.h"

//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/20var/TMVAClassification_MLP.class.C"
//#include "/wk_cms2/cychuang/CMSSW_9_4_13/src/TopCPViolation/select/interface/MVA_Algo/p05_0-2/TMVAClassification_MLP.class.C"

#include "TopCPViolation/select/interface/mva_header.h"
//*****lepton selection!*****//

int SelMgr::TightLeptonNo()
{
	int pass = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.SR_SelEl(i) )
		{
			pass++;		
		}
		else if( leps.SR_SelMu(i) )
		{
			pass++;
		}	
	}
	return pass;
}


bool SelMgr::SR_LepVeto()
{
	int pass_no = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.SR_VetoMu(i) || leps.SR_VetoEl(i) )
		{
			++pass_no;
			sel_lep = i;
		}
	}
	if(pass_no != 1) return false;
	return true;
}


bool SelMgr::SR_SelLep()
{
	if( !SR_LepVeto() ) return false;
	if( leps.SR_SelEl(sel_lep) )
	{	channel = "el";	return true;	}
	else if( leps.SR_SelMu(sel_lep) )
	{	channel = "mu";	return true;	}
	return false;
}

bool SelMgr::CR_LepVeto()
{
	int pass_no = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.CR_VetoMu(i) || leps.CR_VetoEl(i) )
		{
			++pass_no;
			sel_lep = i;
		}
	}
	if(pass_no != 1) return false;
	return true;
}


bool SelMgr::CR_SelLep()
{
	if( !CR_LepVeto() ) return false;
	if( leps.CR_SelEl(sel_lep) )
	{	channel = "el";	return true;	}
	else if( leps.CR_SelMu(sel_lep) )
	{	channel = "mu";	return true;	}
	return false;
}

bool SelMgr::CR_SelLep_invISO()
{
	if( !CR_LepVeto() ) return false;
	if( leps.CR_SelEl_invISO(sel_lep) )
	{	channel = "el";	return true;	}
	else if( leps.CR_SelMu_invISO(sel_lep) )
	{	channel = "mu";	return true;	}
	return false;
}

//with no Iso cut on selected 1 iso lepton and w/o veto
bool SelMgr::PrePreSelLep()
{
	int pass_no = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.PrePreSelMu(i) || leps.PrePreSelEl(i) )
		{
			++pass_no;
			sel_lep = i;
		}
	}
	if( pass_no != 1 ) return false;
	return true;
}

bool SelMgr::PreSelLep()
{
	int pass_no = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.SR_SelMu(i) || leps.SR_SelEl(i) )
		{
			++pass_no;
			sel_lep = i;
		}
	}
	if( pass_no != 1 ) return false;
	return true;
}

bool SelMgr::test_dbl_Lep()
{
	
	int pass_no = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.PrePreSelMu(i) )
		{
			++pass_no;
			sel_lep = i;
			channel = "mu";
		}
		else if( leps.PrePreSelEl(i) )
		{
			++pass_no;
			sel_lep = i;
			channel = "el";
		}
	}
	if(pass_no != 1) return false;
	return true;
}

bool SelMgr::dbl_Lep()
{
	int pass_no = 0;
	for(int i=0;i<(int)leps.Size();++i)
	{
		if( leps.PrePreSelMu(i) )
		{
			++pass_no;
			sel_lep = i;
			channel = "mu";
		}
		else if( leps.PrePreSelEl(i) )
		{
			++pass_no;
			sel_lep = i;
			channel = "el";
		}
	}
	if(pass_no != 1) return false;
	return true;
}

//*****jet selection!*****//

//without energy-related cut  (for systematic uncertainty)
void SelMgr::UncPreSelJets()
{
	for(int i=0;i<(int)jets.Size();++i){
		if( jets.UncPreSelJet(i) ){
			sel_jets.push_back(i);
		}
	}
}


void SelMgr::SelJets()
{
	for(int i=0;i<(int)jets.Size();++i){
		if( jets.SelJet(i) ){
			sel_jets.push_back(i);
		}
	}
}

void SelMgr::JetLepISO()
{
	int ori_jet_size = (int)sel_jets.size();
	vector<int> del_candidate;	//store the idx in sel_jets
	for(int i=0;i<ori_jet_size;++i){
		if( delR( jets.Eta(sel_jets.at(i)), leps.Eta(sel_lep), jets.Phi(sel_jets.at(i)), leps.Phi(sel_lep) ) < 0.4 )
		{	del_candidate.push_back( sel_jets.at(i) );	}
	}
	
	for(int i=0;i<(int)del_candidate.size();++i)
	{	EraseByValue( sel_jets, del_candidate.at(i) );	}
}

void SelMgr::SelBJets()
{
	for(int i=0;i<(int)sel_jets.size();++i)
    {
        if( jets.MediumDeepCSV( sel_jets.at(i) ) )
        {
			sel_b_jets.push_back( sel_jets.at(i) );
        }
    }
}

bool SelMgr::SR_bjets()
{
	vector<int> del_candidate;
	for(int i=0;i<(int)sel_jets.size();++i)
    {
        if( jets.MediumDeepCSV( sel_jets.at(i) ) )
        {
			sel_b_jets.push_back( sel_jets.at(i) );
			del_candidate.push_back( sel_jets.at(i) );
        }
    }

/*
	cout << "before b-jets selection" << endl;
	for(int i=0;i<(int)sel_jets.size();++i)
	{ cout << i << "-th jet is " << sel_jets.at(i) << endl; }
*/

	//delete b-jets from sel_jets list
    for(int i=0;i<(int)del_candidate.size();++i)
    {	EraseByValue( sel_jets, del_candidate.at(i) );	}

/*
	cout << "after b-jets selection" << endl;
	for(int i=0;i<(int)sel_jets.size();++i)
	{ cout << i << "-th jet is " << sel_jets.at(i) << endl; }
	
	cout << "detected b-jets are:" << endl;
	for(int i=0;i<(int)sel_b_jets.size();++i)
	{ cout << i << "-th b-jet is " << sel_b_jets.at(i) << endl; }
*/

    if( (int)sel_b_jets.size() == 2 ) return true;
    return false;
}

bool SelMgr::CR_bjets()
{
	//-- old method --//
/*
	map<int, double> m;
	for(int i=0;i<(int)sel_jets.size();++i){
		double tmp = jets.DeepCSV(i);
		m[ sel_jets.at(i) ] = tmp;
		if( jets.LooseDeepCSV( sel_jets.at(i) ) ) return false;
	}

	//use bubble sort
	for(int i=0;i<(int)sel_jets.size();++i) {
		for(int j=0;j<(int)sel_jets.size() - i - 1;++j) {
			if( m[sel_jets.at(j)] < m[sel_jets.at(j+1)] ) 
			{
				swap_v( sel_jets.at(j), sel_jets.at(j+1) );
			}
		}
	}

	//vector<double> btag_v;
	//btag_v.resize( (int)btag_v.size() );
	//for(int i=0;i<(int)sel_jets.size();++i){
	//	btag_v.at(i) = jets.DeepCSV(i);
	//}
	//sort(btag_v.begin(), btag_v.end(), greater<double>());	//need <algorithm>


	sel_b_jets.push_back( sel_jets.at(0) );
	sel_b_jets.push_back( sel_jets.at(1) );
	sel_jets.erase( sel_jets.begin() );
	sel_jets.erase( sel_jets.begin() );

	return true;
*/
	//-- new method --//
	for(int i=0;i<(int)sel_jets.size();++i){
		if( jets.LooseDeepCSV( sel_jets.at(i) ) ) return false;
	}

	return true;
}



void SelMgr::Chi2Sort()
{
	if( is_SR )
	{
		double min = DBL_MAX;
		int j_size = (int)sel_jets.size();
		int b_size = (int)sel_b_jets.size();

		for(int b=0;b<b_size;++b) {
			for(int j1=0;j1<j_size;++j1) {
				for(int j2=j1+1;j2<j_size;++j2) {
					double tmp = chi2_v( b, j1, j2 );
					if( tmp < min ) {
						min = tmp;
						Hadb = sel_b_jets.at(b);
						J1 = sel_jets.at(j1);
						J2 = sel_jets.at(j2);
						int lb = 1-b;
						Lepb = sel_b_jets.at(lb);
					}
				}
			}
		}

		if( jets.Pt( J1 ) < jets.Pt( J2 ) ) {
			int tmp = J1;
			J1 = J2;
			J2 = tmp;
		}

		reco_algo_value = min;
	}
	else		// it's CR
	{
		double min = DBL_MAX;

		int j_size = (int)sel_jets.size();
		for(int tmp_hadb=0;tmp_hadb<j_size;++tmp_hadb) {
			for(int tmp_j1=0;tmp_j1<j_size;++tmp_j1) {
				if( tmp_j1 == tmp_hadb ) continue;
				for(int tmp_j2=0;tmp_j2<j_size;++tmp_j2) {
					if( tmp_j2 == tmp_hadb || tmp_j2 == tmp_j1 ) continue;
					if( tmp_j1 > tmp_j2 || tmp_j1 == tmp_j2 ) continue; 
					double tmp = chi2_value( sel_jets.at(tmp_hadb), sel_jets.at(tmp_j1), sel_jets.at(tmp_j2) );
					if( tmp < min ) {
						min = tmp;
						Hadb = sel_jets.at( tmp_hadb );
						J1 = sel_jets.at( tmp_j1 );
						J2 = sel_jets.at( tmp_j2 );
					}					
				}
			}
		}

		double min_dR = DBL_MAX;
		for(int tmp_lepb=0;tmp_lepb<j_size;++tmp_lepb)
		{
			int tmp_Lepb = sel_jets.at( tmp_lepb );
			if( tmp_Lepb == Hadb || tmp_Lepb == J1 || tmp_Lepb == J2 ) { continue; }
			double tmp = delR( jets.Eta( tmp_Lepb ), leps.Eta( sel_lep ), jets.Phi( tmp_Lepb ), leps.Phi( sel_lep ) );
			if( tmp < min_dR ) {
				min_dR = tmp;
				Lepb = tmp_Lepb;
			}
		}
		reco_algo_value = min;
			
	}
}


void SelMgr::MVASort()
{
	//if we want to change the algorithm , we need to change the  header file we include in SelMgr.h and change the setting in MVAvar.h
	//training_name = "a05_all_MLP";
	

	if( training_name == "" ) {
		training_name = default_training_name;
	}

	vector<string> inputVars;
	mvatool::AddVarName( inputVars
			//, mva_set 
			);


	//--- it need to be fixed when we change algorithm ---//
	
	//IClassifierReader* reader; // we can use polymorphism
	//if( default_training_name.find( "MLP" ) != string::npos ) { ...
	
	ReadMLP MyMVA( inputVars );
	//ReadBDTG MyMVA( inputVars );
	//ReadBDT MyMVA( inputVars );

	int var_num = (int)inputVars.size();
	
	if( is_SR )
	{	
		double* var = new double[ var_num ];
		for(int _B=0;_B<(int)sel_b_jets.size();_B++)
		{
			int _lB = (_B == 0) ? 1 : 0 ;
			for(int _J1=0;_J1<(int)sel_jets.size();_J1++)
			{
				for(int _J2=_J1+1;_J2<(int)sel_jets.size();_J2++)
				{
					int tmp_mva_lepb = sel_b_jets.at(_lB);
					int tmp_mva_hadb = sel_b_jets.at(_B);
					int tmp_mva_j1 = sel_jets.at(_J1);
					int tmp_mva_j2 = sel_jets.at(_J2);
							
					mvatool::InputVar( var, *(jets.jets), *(leps.leps), *(evt.evt), sel_lep, tmp_mva_hadb, tmp_mva_lepb, tmp_mva_j1, tmp_mva_j2
							//, mva_type 
							);
							
					vector<double> inputValues;
					for(int in=0;in<var_num;in++ )
					{	inputValues.push_back( var[in] );	}

					double tmp_mva_value = MyMVA.GetMvaValue( inputValues );
					if( tmp_mva_value >= reco_algo_value )
					{
						reco_algo_value = tmp_mva_value;
						Hadb = tmp_mva_hadb;
						J1 = tmp_mva_j1;
						J2 = tmp_mva_j2;
						Lepb = tmp_mva_lepb;
					}
				}
			}
		}
		
		if( jets.Pt( J1 ) < jets.Pt( J2 ) ) {
			int tmp = J1;
			J1 = J2;
			J2 = tmp;
		}

		delete [] var;
	}
	else		// it's CR
	{
		double* var = new double[ var_num ];
		double max = -100;

		int j_size = (int)sel_jets.size();
		for(int tmp_hadb=0;tmp_hadb<j_size;++tmp_hadb) {
			for(int tmp_lepb=0;tmp_lepb<j_size;++tmp_lepb) {
				if( tmp_lepb == tmp_hadb ) continue;
				for(int tmp_j1=0;tmp_j1<j_size;++tmp_j1) {
					if( tmp_j1 == tmp_hadb || tmp_j1 == tmp_lepb ) continue;
					for(int tmp_j2=0;tmp_j2<j_size;++tmp_j2) {
						if( tmp_j2 == tmp_hadb || tmp_j2 == tmp_lepb || tmp_j2 == tmp_j1 ) continue;
						if( tmp_j1 > tmp_j2 ) continue;

						int tmp_mva_lepb = sel_jets.at(tmp_lepb);
						int tmp_mva_hadb = sel_jets.at(tmp_hadb);
						int tmp_mva_j1 = sel_jets.at(tmp_j1);
						int tmp_mva_j2 = sel_jets.at(tmp_j2);
							
						mvatool::InputVar( var, *(jets.jets), *(leps.leps), *(evt.evt), sel_lep, tmp_mva_hadb, tmp_mva_lepb, tmp_mva_j1, tmp_mva_j2 );
						vector<double> inputValues;
						for(int in=0;in<var_num;in++ )
						{	inputValues.push_back( var[in] );	}

						double tmp_mva_value = MyMVA.GetMvaValue( inputValues );

						if( tmp_mva_value > max ) {
							max = tmp_mva_value;
							Hadb = sel_jets.at( tmp_hadb );
							J1 = sel_jets.at( tmp_j1 );
							J2 = sel_jets.at( tmp_j2 );
							Lepb = sel_jets.at( tmp_lepb );
						}
					}					
				}
			}
		}

		reco_algo_value = max;
		delete [] var;

		//if the Lepb has not been chose in the mva-term
/*
		double min_dR = DBL_MAX;
		for(tmp_lepb=0;tmp_lepb<j_size;++tmp_lepb)
		{
			int tmp_Lepb = sel_jets.at( tmp_lepb );
			if( tmp_Lepb == Hadb || tmp_Lepb == J1 || tmp_Lepb == J2 ) { continue; }
			double tmp = delR( jets.Eta( tmp_Lepb ), leps.Eta( sel_lep ), jets.Phi( tmp_Lepb ), leps.Phi( sel_lep ) );
			if( tmp < min_dR ) {
				min_dR = tmp;
				Lepb = tmp_Lepb;
			}
		}
*/
	}
}
