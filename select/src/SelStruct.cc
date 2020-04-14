#include "TopCPViolation/select/interface/SelMgr.h"
//#include "TopCPViolation/select/interface/Reco.h"
//#include "../interface/SelMgr.h"


//constructor and test
SelMgr::SelMgr( JetInfo* j, LeptonInfo* l, EvtInfo* e, VertexInfo* v, GenInfo* g )
{
	//cout << "constructor of SelMgr.h" << endl;
	jets.RegIn( j );
	leps.RegIn( l, e );
	evt.RegIn( e, v );
	gen.RegIn( g );
	genMgr.RegIn( g, j, l );
	is_data = false;
	reset();
	training_name = "";
	reco_algo_value = -10.;
}

//just reset one event
void SelMgr::reset()
{
	sel_jets.clear();
	sel_b_jets.clear();
	sel_lep = -1;
	weight = 1.;
	lep_weight = 1.;
	btag_weight = 1.;
	other_weight = 1.;
	channel = "";
	Hadb = -1;			Lepb = -1;
	J1 = -1;			J2 = -1;
	reco_algo_value = -10.;
}


void SelMgr::test_sel_print()
{
	//root->GetEntry(i) outside the class at main()
	int size = jets.jets->Size;
	cout << "---Jets' Part---" << endl;
	for(int i=0;i<size;i++)
	{
		cout << "Idx[" << i << "]->" << endl; 
		cout <<	"Pt:" << jets.Pt(i);
		cout << " GenJetPt:" << jets.jets->GenJetPt[i];
	  	cout << " GenPt:" << jets.jets->GenPt[i] << endl;
		cout << "Eta:" << jets.jets->Eta[i];	
		cout << " GenJetEta:" << jets.jets->GenJetEta[i];	
		cout << " GenEta:" << jets.jets->GenEta[i] << endl;	
		cout << "Phi:" << jets.jets->Phi[i];	
		cout << " GenJetPhi:" << jets.jets->GenJetPhi[i];	
		cout << " GenPhi:" << jets.jets->GenPhi[i] << endl;	
		cout << " GenFlavor:" << jets.jets->GenFlavor[i];
		cout << " GenPdgID:" << jets.jets->GenPdgID[i];	
		cout << endl;
	}

	cout << "---gen part---" << endl;
	for(int k=0;k<gen.gen->Size;k++)
	{
		cout << "idx:" << k << " PdgID:" << gen.gen->PdgID[k] << " Mo1Pdg:" << gen.gen->Mo1PdgID[k] \
		<< " Mo2Pdg:" << gen.gen->Mo2PdgID[k] << " Pt:" <<gen.Pt(k) << " Eta:" << gen.Eta(k) << \
		" Phi:" << gen.Phi(k) << endl;
	}

	/*
	cout << "---Leptons' Part---" << endl;
	for(int j=0;j<leps.leps->Size;j++)
	{
		if( leps.Pt(j) >= 10. )
		{
			cout << "Idx[" << j << "] : " << leps.Pt(j) << endl;
		}
	}
	*/
}
void 
SelMgr::SelJets_deepCSV( vector<double>& v , const string& option = "all" )
{
	v.clear();
	if( option == "all" )
	{
		for(size_t i=0;i<sel_b_jets.size();++i)
		{
			v.push_back( jets.DeepCSV(i) );
		}
		for(int i=0;i<(int)sel_jets.size();++i)
		{
			v.push_back( jets.DeepCSV(i) );
		}
	}
	else
		return;
}


//private tool
double SelMgr::delR(const double& eta1, const double& eta2, const double& phi1, const double& phi2)
{
	double delta_eta = eta1 - eta2;
	double delta_phi = TVector2::Phi_mpi_pi(phi1 - phi2);		//TLorentzVector.h
	double del_R = sqrt( delta_eta * delta_eta + delta_phi * delta_phi );
	return del_R;
}

void SelMgr::swap_v(int& a, int& b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

double SelMgr::chi2_v( const int& idx_b, const int& idx_j1, const int& idx_j2 )
{	
	double pdg_t_mass = 172.5;
	double pdg_w_mass = 82.9;
	double pdg_t_sigma = 16.3;
	double pdg_w_sigma = 9.5;

	TLorentzVector j1, j2, b;

	j1.SetPxPyPzE( jets.Px(sel_jets[idx_j1]), jets.Py(sel_jets[idx_j1]), jets.Pz(sel_jets[idx_j1]), jets.Energy(sel_jets[idx_j1]) );
    j2.SetPxPyPzE( jets.Px(sel_jets[idx_j2]), jets.Py(sel_jets[idx_j2]), jets.Pz(sel_jets[idx_j2]), jets.Energy(sel_jets[idx_j2]) );
    b.SetPxPyPzE( jets.Px(sel_b_jets[idx_b]), jets.Py(sel_b_jets[idx_b]), jets.Pz(sel_b_jets[idx_b]), jets.Energy(sel_b_jets[idx_b]) );
	

	double t_mass = ( j1 + j2 + b ).M();
	double w_mass = ( j1 + j2 ).M();

	return pow( (t_mass - pdg_t_mass)/pdg_t_sigma , 2 ) + pow( (w_mass - pdg_w_mass)/pdg_w_sigma , 2 );
}

double SelMgr::factorial(int x)
{
    if(x == 1)
    {return 1;}
    if(x == 0)
    {return 1;}
    
    return x * factorial(x-1);
}

void SelMgr::EraseByValue( vector<int>& vec, int& value)
{
	vector<int>::iterator position = std::find(vec.begin(), vec.end(), value);
	if(position != vec.end()) vec.erase( position );
}

// public tool

TLorentzVector& SelMgr::JetP4(const int& i) const
{	
	TLorentzVector* p = new TLorentzVector();
	p->SetPxPyPzE( jets.Px(i), jets.Py(i), jets.Pz(i), jets.Energy(i) );
	return *p;
}

TLorentzVector& SelMgr::LepP4(const int& i) const
{	
	TLorentzVector* p = new TLorentzVector();
	p->SetPxPyPzE( leps.Px(i), leps.Py(i), leps.Pz(i), leps.Energy(i) );
	return *p;
}

