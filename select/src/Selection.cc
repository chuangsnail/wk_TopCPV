#include "TopCPViolation/select/interface/SelMgr.h"
//#include "TopCPViolation/select/interface/Reco.h"
//#include "../interface/SelMgr.h"

//***** Selecting! *****//
/*
bool SelMgr::Pass_Golden_Json() {
    if( is_data ) {
        if( !checkEvt.isGoodEvt( evt.RunNo(), evt.LumiNo() ) )
            return false;
    }
    return true;
}
*/
void SelMgr::JetCorrection()
{
	jets.JERCor( opt_jer );
	jets.JESCor( opt_jes );
}

//full sel on lepton and Jet Selection with No Energy Cut
bool SelMgr::UncPreSel()
{
/*
	if( !evt.GoodVertex() ) return false;

    if( !is_data )    
	    leps.ElEnergyCor();
*/
	
	if( !SR_SelLep() ) return false;

	UncPreSelJets();		//without jet energy cut
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;
	
	if( !SR_bjets() ) return false;
	
	return true;
}

// ElCor, JetCor, PileUp, one tight lep w/o ISO, > 4 Jets
bool SelMgr::PrePreSel()
{
	if( !evt.GoodVertex() ) return false;
	leps.ElEnergyCor();
	jets.JERCor();
	
	if( !PrePreSelLep() ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;
	
	return true;
}

//should be updated for
bool SelMgr::PreSel()
{
	if( !PreSelLep() ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	return true;
}


bool SelMgr::SR_select( const bool& is_reweight )
{
	if( !SR_SelLep() ) return false;

	if( !hltMgr->PassHLT(is_data, channel) ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	if( !SR_bjets() ) return false;

	if( is_reweight ) {
		if( !is_data ) 
			if( !SR_ReweightMC() ) return false;
	}
	
	return true;
}

bool SelMgr::CR_select()
{
	if( !CR_SelLep() ) return false;

	if( !hltMgr->PassHLT(is_data, channel) ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	if( !CR_bjets() ) return false;

	if( !is_data ) 
		if( !CR_ReweightMC() ) return false;
	
	return true;
}

bool SelMgr::CR_select_invISO()
{
	if( !CR_SelLep_invISO() ) return false;

	if( !hltMgr->PassHLT(is_data, channel) ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	if( !CR_bjets() ) return false;

	if( !is_data ) 
		if( !CR_ReweightMC() ) return false;
	
	return true;
}

bool SelMgr::test_pre_from_dbl_SR()
{
    if( !SR_SelLep() ) return false;
	
	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	return true;
}

bool SelMgr::test_dbl_SR()
{

    if( !evt.GoodVertex() ) return false;

    if( !is_data )    
	    leps.ElEnergyCor();

	if( !test_dbl_Lep() ) return false;
	
	SelJets();
	if( seljets_No() < 4 ) return false;

    JetCorrection();

	return true;
}

bool SelMgr::dbl_SR()
{
    if( !evt.GoodVertex() ) return false;

    if( !is_data )    
	    leps.ElEnergyCor();

	if( !dbl_Lep() ) return false;
	
	UncPreSelJets();		//without jet energy cut
	if( seljets_No() < 4 ) return false;

	return true;
}

bool SelMgr::full_sel_SR( const bool& is_reweight )
{
    //if( !Pass_Golden_Json() ) return false;

    if( !evt.GoodVertex() ) return false;

    if( !is_data )    
	    leps.ElEnergyCor();
	
    if( !SR_SelLep() ) return false;
	
    if( !hltMgr->PassHLT(is_data, channel) ) return false;

    JetCorrection();

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	if( !SR_bjets() ) return false;

	if( is_reweight ) {
		if( !is_data ) 
			if( !SR_ReweightMC() ) return false;
	}

    return true;
}

bool SelMgr::full_sel_CR( const bool& is_reweight )
{
    //if( !Pass_Golden_Json() ) return false;

    if( !evt.GoodVertex() ) return false;

    if( !is_data )    
	    leps.ElEnergyCor();
    
	if( !CR_SelLep() ) return false;

	if( !hltMgr->PassHLT(is_data, channel) ) return false;
	
    JetCorrection();
	
    SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	if( !CR_bjets() ) return false;

	if( !is_data ) 
		if( !CR_ReweightMC() ) return false;
	
    return true;
}

void SelMgr::PlugInReweightTool( BtagMgr* b, LeptonSFMgr* l )
{	btagMgr = b;	lepsfMgr = l;	}
void SelMgr::PlugInPUTool( PileUpMgr* p ) { puMgr = p; }
void SelMgr::PlugInHLTTool( HLTMgr* h ) { hltMgr = h; }

double SelMgr::GetLepWeight( const string& opt ) const 
{ 
	if( opt == "" )
		return lep_weight;
	else {
		return lepsfMgr->GetLepSF( channel, leps.Pt(sel_lep), leps.Eta(sel_lep), opt );
	}	
}

double SelMgr::GetBtagWeight( const string& opt ) const 
{ 
	if( opt == "" )
		return btag_weight; 
	else {
		btagMgr->Reset_idx_capacity();
		btagMgr->Set_b_tagged_jets_idx( sel_b_jets );
		btagMgr->Set_b_ntagged_jets_idx( sel_jets );
		return btagMgr->Get_Btag_Weight( opt );
	}	
}

double SelMgr::GetPUWeight( const string& opt ) const 
{
	if( opt == "" )
		return pu_weight; 
	else {
		puMgr->PassPileUpWeight();
		return puMgr->GetPUWeight( opt );
	}	
}

//This function also do the systematic uncertainty things...
bool SelMgr::SR_ReweightMC()
{
	//opt_pu, opt_lepsf, opt_btag are "central" in default!

	//pile up part
	if( !puMgr->PassPileUpWeight() ) return false;
	pu_weight = puMgr->GetPUWeight( opt_pu );

	//btag part
	btagMgr->Reset_idx_capacity();
	btagMgr->Set_b_tagged_jets_idx( sel_b_jets );
	btagMgr->Set_b_ntagged_jets_idx( sel_jets );
	btag_weight = btagMgr->Get_Btag_Weight( opt_btag );

	//lepton part
	lep_weight = lepsfMgr->GetLepSF( channel, leps.Pt(sel_lep), leps.Eta(sel_lep), opt_lepsf );

	//GenWeight
	other_weight = ( (gen.Weight() > 0) ? 1. : -1. );

	//all weight
	weight = other_weight * lep_weight * btag_weight * pu_weight;
	
	return true;	
}

bool SelMgr::CR_ReweightMC()
{
	//pile up part
	if( !puMgr->PassPileUpWeight() ) return false;
	pu_weight = puMgr->GetPUWeight( opt_pu );

	//btag part
	btagMgr->Reset_idx_capacity();
	vector<int> all_jets( sel_jets );		//copy constructor
	/*
	for(int i=0;i<(int)seljets_No();++i) {
		all_jets.push_back( sel_jets.at(i) );
	}
	*/
	// this is for old method
	//for(int i=0;i<(int)selbjets_No();++i) {
	//	all_jets.push_back( sel_b_jets.at(i) );
	//}
	btagMgr->Set_b_ntagged_jets_idx( all_jets );
	btag_weight = btagMgr->Get_Btag_Weight( opt_btag );

	//lepton part
	lep_weight = lepsfMgr->GetLepSF( channel, leps.Pt(sel_lep), leps.Eta(sel_lep), opt_lepsf );

	//GenWeight
	other_weight = ( (gen.Weight() > 0) ? 1. : -1. );
	
	//all weight
	weight = other_weight * lep_weight * btag_weight * pu_weight;
	
	return true;	
}

//--- for gen-matching(for mass fitting) ---//

bool 
SelMgr::Find_GenTW()
{
	// There are no selections, but we still need  pileup and genweight reweight
	if( !JustGen_ReweightMC() ) return false;

	if( !genMgr.Find_Correct_HadronicTop( Hadb, J1, J2 ) ) return false;
	channel = genMgr.Get_channel();

	return true;
}

bool
SelMgr::JustGen_ReweightMC()
{
	if( !puMgr->PassPileUpWeight() ) return false;
	pu_weight = puMgr->GetPUWeight();
	//other_weight *= (gen.Weight() >= 0) ? 1. : -1.;
	weight = pu_weight * lep_weight * btag_weight;
	return true;
}

