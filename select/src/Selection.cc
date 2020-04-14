#include "TopCPViolation/select/interface/SelMgr.h"
//#include "TopCPViolation/select/interface/Reco.h"
//#include "../interface/SelMgr.h"

//***** Selecting! *****//

// ElCor, JetCor, PileUp, one tight lep w/o ISO, > 4 Jets
bool SelMgr::PrePreSel()
{
	leps.ElEnergyCor();
	jets.JERCor();
	
	if( !PrePreSelLep() ) return false;

	if( !puMgr->PassPileUpWeight() ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	return true;
}

bool SelMgr::PreSel()
{
	if( !PreSelLep() ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	return true;
}


bool SelMgr::SR_select()
{
	if( !SR_SelLep() ) return false;

	if( !hltMgr->PassHLT(is_data, channel) ) return false;

	SelJets();
	if( seljets_No() < 4 ) return false;
	
	JetLepISO();
	if( seljets_No() < 4 ) return false;

	if( !SR_bjets() ) return false;

	if( !is_data ) 
		if( !SR_ReweightMC() ) return false;
	
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


void SelMgr::PlugInReweightTool( BtagMgr* b, LeptonSFMgr* l )
{	btagMgr = b;	lepsfMgr = l;	}
void SelMgr::PlugInPUTool( PileUpMgr* p ) { puMgr = p; }
void SelMgr::PlugInHLTTool( HLTMgr* h ) { hltMgr = h; }

bool SelMgr::SR_ReweightMC()
{
	//pile up part
	if( !puMgr->PassPileUpWeight() ) return false;
	other_weight *= puMgr->GetPUWeight();

	//btag part
	btagMgr->Reset_idx_capacity();
	btagMgr->Set_b_tagged_jets_idx( sel_b_jets );
	btagMgr->Set_b_ntagged_jets_idx( sel_jets );
	btag_weight = btagMgr->Get_Btag_Weight();

	//lepton part
	lep_weight = lepsfMgr->GetLepSF( channel, leps.Pt(sel_lep), leps.Eta(sel_lep) );

	//GenWeight
	other_weight *= (gen.Weight() > 0) ? 1. : -1.;

	//all weight
	weight = other_weight * lep_weight * btag_weight;
	
	return true;	
}

bool SelMgr::CR_ReweightMC()
{
	//pile up part
	if( !puMgr->PassPileUpWeight() ) return false;
	other_weight = puMgr->GetPUWeight();

	//btag part
	btagMgr->Reset_idx_capacity();
	vector<int> all_jets( sel_jets );
	/*
	for(int i=0;i<(int)seljets_No();++i) {
		all_jets.push_back( sel_jets.at(i) );
	}
	*/
	for(int i=0;i<(int)selbjets_No();++i) {
		all_jets.push_back( sel_b_jets.at(i) );
	}
	btagMgr->Set_b_ntagged_jets_idx( all_jets );
	btag_weight = btagMgr->Get_Btag_Weight();

	//lepton part
	lep_weight = lepsfMgr->GetLepSF( channel, leps.Pt(sel_lep), leps.Eta(sel_lep) );

	//GenWeight
	other_weight *= (gen.Weight() > 0) ? 1. : -1.;
	
	//all weight
	weight = other_weight * lep_weight * btag_weight;
	
	return true;	
}

//*** for MVA training ***//


