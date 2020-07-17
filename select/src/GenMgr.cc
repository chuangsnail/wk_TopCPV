#include "../interface/GenMgr.h"

void GenMgr::DumpPropagateInfo( const int& self_idx )
{
	if( self_idx == 0 ) { 
		cout << "end of propagate" << endl;
		return; 
	}
	int pdgid = gen->PdgID[self_idx];
	cout << "id:" << self_idx << " Eta:" << gen->Eta[self_idx] << " Phi:" << gen->Phi[self_idx] << endl;
	if( gen->Da1PdgID[self_idx] == pdgid && gen->Da2PdgID[self_idx] == pdgid )
	{ DumpPropagateInfo( gen->Da1[self_idx] ); }
	else if( gen->Da1PdgID[self_idx] == pdgid && gen->Da2PdgID[self_idx] != pdgid )
	{ DumpPropagateInfo( gen->Da1[self_idx] ); }
	else if( gen->Da1PdgID[self_idx] != pdgid && gen->Da2PdgID[self_idx] == pdgid )
	{ DumpPropagateInfo( gen->Da2[self_idx] ); }
	else if( gen->Da1 )
	{ 
		cout << "end of propagate" << endl;
		return;
	}
}

int GenMgr::GetPropagationEnd( const int& idx )
{
	cout << "now propagate id:" << idx << endl;
	int pdgid = gen->PdgID[ idx ];
	if( gen->Da1PdgID[ idx ] == pdgid && gen->Da2PdgID[ idx ] == pdgid )
	{ 
		if( gen->Da1[idx] == 0 )
		{ return idx; }
		return GetPropagationEnd( gen->Da1[ idx ] );
	}
	else if( gen->Da1PdgID[ idx ] == pdgid && gen->Da2PdgID[ idx ] != pdgid )
	{ 
		if( gen->Da1[idx] == 0 )
		{ return idx; }
		return GetPropagationEnd( gen->Da1[ idx ] ); 
	}
	else if( gen->Da1PdgID[ idx ] != pdgid && gen->Da2PdgID[ idx ] == pdgid )
	{ 
		if( gen->Da2[idx] == 0 )
		{ return idx; }
		return GetPropagationEnd( gen->Da2[ idx ] ); 
	}
	else
	{ return idx; }	
}


int GenMgr::is_Gen_Jet_Seen( const int& idx )	//idx is idx in geninfo
{
    double delta_R = DBL_MAX;
	int tmp = -1;		//tmp is idx in JetInfo(jets)
	for(int i=0;i<jets->Size;i++)
	{
		double gen_eta = jets->GenEta[i];
		double gen_phi = jets->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[idx];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[idx]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
			tmp = i;
        }
	}
	
	//cout <<"tmp" << tmp << " dR:" << delta_R << endl;
	return tmp;
}

int GenMgr::is_Gen_Lep_Seen( const int& idx )
{
    double delta_R = DBL_MAX;
	int tmp = -1;
	for(int i=0;i<leps->Size;i++)
	{
		double gen_eta = leps->GenEta[i];
		double gen_phi = leps->GenPhi[i];
		
        double d_eta = gen_eta - gen->Eta[idx];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[idx]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
			tmp = i;
        }
	}
	return tmp;
}


int GenMgr::Match_Gen( double gen_eta,double gen_phi )
{
    int idx = -1;
    double delta_R = DBL_MAX;
    
    for(int i=0; i<(int)gen->Size ;i++)
    {
        double d_eta = gen_eta - gen->Eta[i];
        double d_phi  = TVector2::Phi_mpi_pi(gen_phi - gen->Phi[i]);
        double delta_R_tmp = TMath::Sqrt( d_eta * d_eta + d_phi * d_phi);
        
        if( delta_R_tmp < delta_R && delta_R_tmp < 0.4 )
        {
            delta_R = delta_R_tmp;
            idx = i;
        }
    }
    
    return idx;
}


int GenMgr::Find_GenParticle( const int& id_pdg, const int& mo_id )
{
    int idx = -1;
    for(int i=0;i<(int)gen->Size;i++)
    {
        if( gen->PdgID[i] == id_pdg )
        {
			//just find the first one in an event
			//the later others are its propagation
            if( gen->Mo1PdgID[i] == mo_id || gen->Mo2PdgID[i] == mo_id )
            {
                idx = i;
                break;    //break out of the for loop
            }
        }
    }
    return idx;
}

int GenMgr::GenJetIdx( int idx )
{
    return Match_Gen( jets->GenEta[idx], jets->GenPhi[idx] );
}

//Input the idx of LeptonInfo and return the idx of genInfo
int GenMgr::GenLepIdx( int idx )
{
    return Match_Gen( leps->GenEta[idx], leps->GenPhi[idx] );
}

bool GenMgr::Check_Source_Mo( const int& gen_idx, const int& sourc_mo_pdgid )
{
	int self_pdg_id = gen->PdgID[ gen_idx ];
	int find_pin = gen_idx;
	for(int i=0;i<gen->Size;i++)
	{		
		if( gen->Mo1PdgID[find_pin] == sourc_mo_pdgid || gen->Mo2PdgID[find_pin] == sourc_mo_pdgid )
		{	
			return true;
		}
		if( gen->Mo1PdgID[find_pin] == self_pdg_id || gen->Mo2PdgID[find_pin] == self_pdg_id )
		{
			if( gen->nMo[find_pin] == 1 )
			{
				int next_find_pin = gen->Mo1[ find_pin ];
				find_pin = next_find_pin;
			}
			else if( gen->nMo[find_pin] >= 2 )
			{
				if( gen->Mo1PdgID[find_pin] == self_pdg_id )
				{
					int next_find_pin = gen->Mo1[ find_pin ];
					find_pin = next_find_pin;
					continue;
				}
				if( gen->Mo2PdgID[find_pin] == self_pdg_id )
				{
					int next_find_pin = gen->Mo2[ find_pin ];
					find_pin = next_find_pin;
					continue;
				}
			}
		}
		return false;
	}
	return false;
}


void GenMgr::SetGenBranch( GenInfo* g ){    gen = g;    }
void GenMgr::SetJetBranch( JetInfo* j ){    jets = j;   }
void GenMgr::SetLepBranch( LeptonInfo* l ){ leps = l;   }

void GenMgr::PrintInfo()
{
	for(int i=0;i<gen->Size;i++)
	{
		cout << "idx:" << i << " Selpdg:" << gen->PdgID[i] << " Mo1idx:" << gen->Mo1[i] << " Mo1pdg:" << gen->Mo1PdgID[ i ] << " Mo2idx:" << gen->Mo2[i] << "  Mo2pdg:" << gen->Mo2PdgID[ i ] << endl;
	}
}

bool
GenMgr::Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 )
{
	////check if there is a lepton( muon or electron ), can be seen
	
	int lepl_no = 0;
	int lep_idx = -1;		//in Gen
	
	for(int i=0;i<gen->Size;i++)
	{
		if( fabs(gen->PdgID[i]) == 11 || fabs(gen->PdgID[i]) == 13 || fabs(gen->PdgID[i]) == 15 )
		{
			if( fabs(gen->Mo1PdgID[i]) == 24 || fabs(gen->Mo2PdgID[i]) == 24 )
			{	
				if( is_Gen_Lep_Seen( i ) != -1  )
				{
					lepl_no++;
					lep_idx = i;
				}
			}
		}
		//else if(fabs(gen->PdgID[i]) == 15)
		//{ 
			//return false; 
			//lepl_no++;
			//lep_idx = i;
		//}
	}
	if( lepl_no != 1 )
	{	
		return false;	
	}

	if( abs(gen->PdgID[lep_idx]) == 11 )
		_ch = "el";
	else if( abs(gen->PdgID[lep_idx]) == 13 )
		_ch = "mu";
	else if( abs(gen->PdgID[lep_idx]) == 15 )
		_ch = "tau";
		

	double lep_charge;
	if( gen->PdgID[lep_idx] > 0 )		//charge < 0
	{ lep_charge = -1.; }
	else
	{ lep_charge = 1.; }
   	
   	////check 2 b-quark exist in gen

    int real_gen_b = Find_GenParticle( 5, 6 );            //idx in GenInfo
    int real_gen_bbar = Find_GenParticle( -5, -6 );
    
    if( real_gen_b == -1 || real_gen_bbar == -1 )       //that means there are no b or bbar in this selected event
    {   return false;   }
	
	////check 2 jets from W exist in gen

	vector<int> hadj;	//in Geninfo
	hadj.assign( 10, 0 );
	hadj.clear();

	int w_pdg = 24;			//for hadronic branch W
	if( lep_charge > 0 )	//leptonic branch is W-plus
	{ w_pdg = -24; }

	for(int i=0;i<gen->Size;i++)
	{
		if( ( gen->Mo1PdgID[ i ] == w_pdg || gen->Mo2PdgID[ i ] == w_pdg ) )
		{ 
			int self_id = gen->PdgID[i];
			//if( gen->PdgID[i] != w_pdg )
			if( fabs(self_id) == 1 || fabs(self_id) == 2 || fabs(self_id) == 3 || fabs(self_id) == 4 )
			{
				hadj.push_back( i ); 
			}
		}
	}
	//cout << "hadj.size()" << (int)hadj.size() << endl;
	if( (int)hadj.size() != 2 )
	{ return false; }

	////check b/bbar can be seen
	int b_in_jetinfo = is_Gen_Jet_Seen( real_gen_b );
	int bbar_in_jetinfo = is_Gen_Jet_Seen( real_gen_bbar );

	if( b_in_jetinfo == -1 || bbar_in_jetinfo == -1 ) { return false; }
    
		//to inspect which is hdronic b
	if( lep_charge > 0 )	//hadW is W-minus
	{ cor_b = bbar_in_jetinfo; }
	else
	{ cor_b = b_in_jetinfo; }

	////check j1/j2 can be seen

	//cout << "q1's pdg : " << gen->PdgID[ hadj.at(0) ] << endl;	
	//DumpPropagateInfo( hadj.at(0) );
	//cout << "q2's pdg : " << gen->PdgID[ hadj.at(1) ] << endl;	
	//DumpPropagateInfo( hadj.at(1) );

	//int final_j1 = GetPropagationEnd( hadj.at(0) );
	//int final_j2 = GetPropagationEnd( hadj.at(1) );

	int final_j1 = hadj.at(0);
	int final_j2 = hadj.at(1);

	//cout << "final_j1:" << final_j1 << "  final_j2:" << final_j2 << endl;

	cor_j1 = is_Gen_Jet_Seen( final_j1 );
	if(cor_j1 == -1) { return false; }
    
	cor_j2 = is_Gen_Jet_Seen( final_j2 );
	if(cor_j2 == -1) { return false; }

	if( cor_j1 == cor_j2 )
	{ return false; }

	//cout << endl;
    return true;
}


bb_matching_type GenMgr::Get_bb_Option( const int& had_b, const int& lep_b, const int& lep )
{	
	//***To decide the b and bbar which is had_b and which is lep_b***//
	/*selected lepton's charge sign is same as the hadronic b's charge sign*/
	int b = -1;			//idx in JetInfo
	int bbar = -1;		//idx in JetInfo
	double lep_charge = leps->Charge[ lep ];
	
	if( lep_charge > 0 )
	{
		b = lep_b;
		bbar = had_b;
	}
	else
	{
		b = had_b;
		bbar = lep_b;
	}

    int real_gen_b = Find_GenParticle( 5, 6 );            //idx in GenInfo
    int real_gen_bbar = Find_GenParticle( -5, -6 );
    
    if( real_gen_b == -1 || real_gen_bbar == -1 ) return no_match;

	int gen_b = GenJetIdx( b );			
	int gen_bbar = GenJetIdx( bbar );
	
	if( (gen_b == -1) || (gen_bbar == -1) ) return no_match;

	if(	(gen_b == real_gen_b) && (gen_bbar == real_gen_bbar) )
		return correct;
	else if( (gen_b == real_gen_bbar) && (gen_bbar == real_gen_b) )
		return charge_mis;
	else
		return mistag;

}

bool 
GenMgr::IsJ1Correct( const int& jet_j1 )
{
	
	int lepl_no = 0;
	int lep_idx = -1;		//in Gen
	
	for(int i=0;i<gen->Size;i++)
	{
		if( fabs(gen->PdgID[i]) == 11 || fabs(gen->PdgID[i]) == 13 || fabs(gen->PdgID[i]) == 15 )
		{
			if( fabs(gen->Mo1PdgID[i]) == 24 || fabs(gen->Mo2PdgID[i]) == 24 )
			{	
				if( is_Gen_Lep_Seen( i ) != -1  )
				{
					lepl_no++;
					lep_idx = i;
				}
			}
		}
	}
	if( lepl_no != 1 )
	{	
		return false;	
	}
	
	double lep_charge;
	if( gen->PdgID[lep_idx] > 0 )		//charge < 0
	{ lep_charge = -1.; }
	else
	{ lep_charge = 1.; }
	
	int w_pdg = 24;			//for hadronic branch W
	if( lep_charge > 0 )	//leptonic branch is W-plus
	{ w_pdg = -24; }
	
	vector<int> hadj;						//idx in GenInfo
	for(int i=0;i<gen->Size;i++)
	{
		if( ( gen->Mo1PdgID[ i ] == w_pdg || gen->Mo2PdgID[ i ] == w_pdg ) )
		{ 
			int self_id = gen->PdgID[i];
			if( abs(self_id) == 1 || abs(self_id) == 2 || abs(self_id) == 3 || abs(self_id) == 4 ) {
				hadj.push_back( i ); 
			}
		}
	}
	
	if( (int)hadj.size() != 2 )
	{
	   	//cerr << "Error -> hadj.size() : " << hadj.size() << endl; 
		
		for(int j=0;j<gen->Size;++j)
		{
			if( gen->PdgID[j] == w_pdg && ( gen->Da1PdgID[j] != w_pdg || gen->Da2PdgID[j] != w_pdg ) )
			{
				//cout << "Da1 of W : " << gen->Da1[j] << "  " << gen->Da1PdgID[j] << endl;
				//cout << "Da2 of W : " << gen->Da2[j] << "  " << gen->Da2PdgID[j] << endl;
				hadj.clear();
				hadj.push_back( gen->Da1[j] );
				hadj.push_back( gen->Da2[j] );
			}	
		}

		//cerr << "something wrong if you use semi-leptonic ttbar sample" << endl;
		counter++;
		//return false; 
	}
	
	if( (int)hadj.size() != 2 ) return false;
	
	
	int gen_j1 = GenJetIdx( jet_j1 );
	int gen_q1 = -1;

	if( gen->Pt[ hadj.at(0) ] > gen->Pt[ hadj.at(1) ] )
		gen_q1 = hadj.at(0);
	else
		gen_q1 = hadj.at(1);
	
	if( gen_j1 == gen_q1 )
	{
		return true;
	}
	else
		return false;
}

	
	
	
bool
GenMgr::IsPositiveAcp( const string& obs )
{
	////check if there is a lepton( muon or electron ), can be seen
	
	int lepl_no = 0;
	int lep_idx = -1;		//in Gen
	
	for(int i=0;i<gen->Size;i++)
	{
		if( fabs(gen->PdgID[i]) == 11 || fabs(gen->PdgID[i]) == 13 || fabs(gen->PdgID[i]) == 15 )
		{
			if( fabs(gen->Mo1PdgID[i]) == 24 || fabs(gen->Mo2PdgID[i]) == 24 )
			{	
				if( is_Gen_Lep_Seen( i ) != -1  )
				{
					lepl_no++;
					lep_idx = i;
				}
			}
		}
	}
	if( lepl_no != 1 )
	{	
		return false;	
	}
		

	double lep_charge;
	if( gen->PdgID[lep_idx] > 0 )		//charge < 0
	{ lep_charge = -1.; }
	else
	{ lep_charge = 1.; }
   	
   	////check 2 b-quark exist in gen

    int real_gen_b = Find_GenParticle( 5, 6 );            //idx in GenInfo
    int real_gen_bbar = Find_GenParticle( -5, -6 );
    
    if( real_gen_b == -1 || real_gen_bbar == -1 )       //that means there are no b or bbar in this selected event
    {   return false;   }
	
	////check 2 jets from W exist in gen

	vector<int> hadj;	//in Geninfo
	hadj.assign( 10, 0 );
	hadj.clear();

	int w_pdg = 24;			//for hadronic branch W
	if( lep_charge > 0 )	//leptonic branch is W-plus
	{ w_pdg = -24; }

	for(int i=0;i<gen->Size;i++)
	{
		if( ( gen->Mo1PdgID[ i ] == w_pdg || gen->Mo2PdgID[ i ] == w_pdg ) )
		{ 
			int self_id = gen->PdgID[i];
			//if( gen->PdgID[i] != w_pdg )
			if( fabs(self_id) == 1 || fabs(self_id) == 2 || fabs(self_id) == 3 || fabs(self_id) == 4 )
			{
				hadj.push_back( i ); 
			}
		}
	}
	//cout << "hadj.size()" << (int)hadj.size() << endl;
	if( (int)hadj.size() != 2 )
	{ return false; }

	int j1 = -1;
	if( gen->Pt[hadj.at(0)] > gen->Pt[hadj.at(1)] )
		j1 = hadj.at(0);
	else
		j1 = hadj.at(1);


	//PrintInfo();
	
	int b = real_gen_b;
	int bbar = real_gen_bbar;

/*
	cout << "********Particles in gen********" << endl;
	cout << "b : " << b << endl;
	cout << "bbar : " << bbar << endl;
	cout << "j1 : " << j1 << endl;
	cout << "lep : " << lep_idx << endl;
*/

	TVector3 _b, _bbar, _j1, _lepton;
	_b.SetPtEtaPhi( gen->Pt[b], gen->Eta[b], gen->Phi[b] );
	_bbar.SetPtEtaPhi( gen->Pt[bbar], gen->Eta[bbar], gen->Phi[bbar] );
	_j1.SetPtEtaPhi( gen->Pt[j1], gen->Eta[j1], gen->Phi[j1] );
	_lepton.SetPtEtaPhi( gen->Pt[lep_idx], gen->Eta[lep_idx], gen->Phi[lep_idx] );

	TLorentzVector b_p4, bbar_p4, j1_p4, lep_p4;
	b_p4.SetPtEtaPhiM( gen->Pt[b], gen->Eta[b], gen->Phi[b], gen->Mass[b] );
	bbar_p4.SetPtEtaPhiM( gen->Pt[bbar], gen->Eta[bbar], gen->Phi[bbar], gen->Mass[bbar] );
	j1_p4.SetPtEtaPhiM( gen->Pt[j1], gen->Eta[j1], gen->Phi[j1], gen->Mass[j1] );
	lep_p4.SetPtEtaPhiM( gen->Pt[lep_idx], gen->Eta[lep_idx], gen->Phi[lep_idx], gen->Mass[lep_idx] );

	//*** calculate observables ***//
	
	double res = 0.;

	if( obs == "Obs3" )
	{
		TVector3 bbcm = -( b_p4 + bbar_p4 ).BoostVector();
		b_p4.Boost( bbcm );
		bbar_p4.Boost( bbcm );
		j1_p4.Boost( bbcm );
		lep_p4.Boost( bbcm );

		res = lep_charge * ( ( b_p4.Vect() ).Dot( ( lep_p4.Vect() ).Cross( j1_p4.Vect() ) ) ) ;
	}
	else if( obs == "Obs6" )
	{
		res = lep_charge * ( ( _b - _bbar ).Dot( _lepton.Cross( _j1 ) ) );
	}
	else if( obs == "Obs12" )
	{
		res = ( _b - _bbar ).Pz() * ( _b.Cross( _bbar ) ).Pz() ;
	}
	else if( obs == "Obs13" )
	{
		//res = lep_charge * ( ( _b + _bbar ).Dot( _lepton.Cross( _j1 ) ) );
		res = ( ( _b + _bbar ).Dot( _lepton.Cross( _j1 ) ) );
	}
	else
	{
		cerr << "There is no option in fn. GenMgr::IsPositiveAcp() !!!" << endl;
	}
	
	if( res > 0. ) return true;

	return false;

}
	
double
GenMgr::GenAcp( const string& obs )
{
	////check if there is a lepton( muon or electron ), can be seen
	
	int lepl_no = 0;
	int lep_idx = -1;		//in Gen
	
	for(int i=0;i<gen->Size;i++)
	{
		if( fabs(gen->PdgID[i]) == 11 || fabs(gen->PdgID[i]) == 13 || fabs(gen->PdgID[i]) == 15 )
		{
			if( fabs(gen->Mo1PdgID[i]) == 24 || fabs(gen->Mo2PdgID[i]) == 24 )
			{	
				if( is_Gen_Lep_Seen( i ) != -1  )
				{
					lepl_no++;
					lep_idx = i;
				}
			}
		}
	}
	if( lepl_no != 1 )
	{	
		return false;	
	}
		

	double lep_charge;
	if( gen->PdgID[lep_idx] > 0 )		//charge < 0
	{ lep_charge = -1.; }
	else
	{ lep_charge = 1.; }
   	
   	////check 2 b-quark exist in gen

    int real_gen_b = Find_GenParticle( 5, 6 );            //idx in GenInfo
    int real_gen_bbar = Find_GenParticle( -5, -6 );
    
    if( real_gen_b == -1 || real_gen_bbar == -1 )       //that means there are no b or bbar in this selected event
    {   return false;   }
	
	////check 2 jets from W exist in gen

	vector<int> hadj;	//in Geninfo
	hadj.assign( 10, 0 );
	hadj.clear();

	int w_pdg = 24;			//for hadronic branch W
	if( lep_charge > 0 )	//leptonic branch is W-plus
	{ w_pdg = -24; }

	for(int i=0;i<gen->Size;i++)
	{
		if( ( gen->Mo1PdgID[ i ] == w_pdg || gen->Mo2PdgID[ i ] == w_pdg ) )
		{ 
			int self_id = gen->PdgID[i];
			//if( gen->PdgID[i] != w_pdg )
			if( fabs(self_id) == 1 || fabs(self_id) == 2 || fabs(self_id) == 3 || fabs(self_id) == 4 )
			{
				hadj.push_back( i ); 
			}
		}
	}
	//cout << "hadj.size()" << (int)hadj.size() << endl;
	if( (int)hadj.size() != 2 )
	{ return false; }

	int j1 = -1;
	if( gen->Pt[hadj.at(0)] > gen->Pt[hadj.at(1)] )
		j1 = hadj.at(0);
	else
		j1 = hadj.at(1);


	//PrintInfo();
	
	int b = real_gen_b;
	int bbar = real_gen_bbar;

/*
	cout << "********Particles in gen********" << endl;
	cout << "b : " << b << endl;
	cout << "bbar : " << bbar << endl;
	cout << "j1 : " << j1 << endl;
	cout << "lep : " << lep_idx << endl;
*/

	TVector3 _b, _bbar, _j1, _lepton;
	_b.SetPtEtaPhi( gen->Pt[b], gen->Eta[b], gen->Phi[b] );
	_bbar.SetPtEtaPhi( gen->Pt[bbar], gen->Eta[bbar], gen->Phi[bbar] );
	_j1.SetPtEtaPhi( gen->Pt[j1], gen->Eta[j1], gen->Phi[j1] );
	_lepton.SetPtEtaPhi( gen->Pt[lep_idx], gen->Eta[lep_idx], gen->Phi[lep_idx] );

	TLorentzVector b_p4, bbar_p4, j1_p4, lep_p4;
	b_p4.SetPtEtaPhiM( gen->Pt[b], gen->Eta[b], gen->Phi[b], gen->Mass[b] );
	bbar_p4.SetPtEtaPhiM( gen->Pt[bbar], gen->Eta[bbar], gen->Phi[bbar], gen->Mass[bbar] );
	j1_p4.SetPtEtaPhiM( gen->Pt[j1], gen->Eta[j1], gen->Phi[j1], gen->Mass[j1] );
	lep_p4.SetPtEtaPhiM( gen->Pt[lep_idx], gen->Eta[lep_idx], gen->Phi[lep_idx], gen->Mass[lep_idx] );

	//*** calculate observables ***//
	
	double res = 0.;

	if( obs == "Obs3" )
	{
		TVector3 bbcm = -( b_p4 + bbar_p4 ).BoostVector();
		b_p4.Boost( bbcm );
		bbar_p4.Boost( bbcm );
		j1_p4.Boost( bbcm );
		lep_p4.Boost( bbcm );

		res = lep_charge * ( ( b_p4.Vect() ).Dot( ( lep_p4.Vect() ).Cross( j1_p4.Vect() ) ) ) ;
	}
	else if( obs == "Obs6" )
	{
		res = lep_charge * ( ( _b - _bbar ).Dot( _lepton.Cross( _j1 ) ) );
	}
	else if( obs == "Obs12" )
	{
		res = ( _b - _bbar ).Pz() * ( _b.Cross( _bbar ) ).Pz() ;
	}
	else if( obs == "Obs13" )
	{
		//res = lep_charge * ( ( _b + _bbar ).Dot( _lepton.Cross( _j1 ) ) );
		res = ( ( _b + _bbar ).Dot( _lepton.Cross( _j1 ) ) );
	}
	else
	{
		cerr << "There is no option in fn. GenMgr::IsPositiveAcp() !!!" << endl;
	}

	if( res == 0. )
	{
		cerr << "The GenAcp (" << obs <<  ")  is 0, that is weird" << endl;
	}
	
	return res;

}
