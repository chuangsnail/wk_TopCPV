#ifndef MVAVAR_H
#define MVAVAR_H

//#include "TopCPViolation/select/interface/BranchInfo.h"
//#include "TopCPViolation/select/interface/kinematic_tool.h"

#include "BranchInfo.h"
#include "kinematic_tool.h"
//p03_all



namespace mvatool
{
	//below : used in making the mva trained tree

	class StoreMVATree
	{
	public:

		StoreMVATree() {}
		StoreMVATree( TTree* co, TTree* inco ) : correct(co), incorrect(inco) {}
		~StoreMVATree() {}

		TTree* correct;
		TTree* incorrect;

        double c_evt_weight;                                  //combine all the scale factor in this event

	    double c_top_mass;
	    double c_w_mass;
	    double c_top_mass_dev;
	    double c_w_mass_dev;

	    double c_j1j2_absdelEta;
	    double c_j1j2_delPhi;
	    double c_j1j2_sumPt;
	    double c_j1j2_absdelPt;
	    double c_j1j2_delR;

	    double c_lepblep_absdelEta;
	    double c_lepblep_delPhi;
	    double c_lepblep_sumPt;
	    double c_lepblep_delR;
	
        double c_hadb_deepcsv_v, c_lepb_deepcsv_v;
	    double c_hadblepb_delPt;	
	    double c_whadb_absdelEta;
	    double c_whadb_delPhi;
        double c_whadb_sumPt;
	    double c_whadb_delPt;
	    double c_whadb_delR;

	    double c_hadblepton_absdelEta;
	    double c_hadblepton_delPhi;
	    double c_hadblepton_delPt;
	    double c_hadblepton_sumPt;
	    double c_hadblepton_delR;
	
	    double c_hadwlepton_absdelEta;
	    double c_hadwlepton_delPhi;
	    double c_hadwlepton_delR;
	    double c_hadwlepton_delPt;
	    double c_hadwlepton_sumPt;

	    double c_hadbmet_delPhi;
	    double c_hadbmet_delPt;
	    double c_hadbmet_sumPt;

	    double c_hadwmet_delPhi;
	    double c_hadwmet_delPt;
	    double c_hadwmet_sumPt;
    
		//The incoreect combination's memory need to be used many times in one events, and be stored in many events
        double i_evt_weight;

	    double i_top_mass;
	    double i_w_mass;
	    double i_top_mass_dev;
	    double i_w_mass_dev;
    
	    double i_j1j2_absdelEta;
	    double i_j1j2_delPhi;
	    double i_j1j2_sumPt;
	    double i_j1j2_absdelPt;
	    double i_j1j2_delR;
	
	    double i_lepblep_absdelEta;
	    double i_lepblep_delPhi;
	    double i_lepblep_sumPt;
	    double i_lepblep_delR;
    
        double i_hadb_deepcsv_v, i_lepb_deepcsv_v;
	    double i_hadblepb_delPt;

	    double i_whadb_absdelEta;
	    double i_whadb_delPhi;
        double i_whadb_sumPt;
	    double i_whadb_delPt;
	    double i_whadb_delR;
    
	    double i_hadblepton_absdelEta;
	    double i_hadblepton_delPhi;
	    double i_hadblepton_delPt;
	    double i_hadblepton_sumPt;
	    double i_hadblepton_delR;
	
	    double i_hadwlepton_absdelEta;
	    double i_hadwlepton_delPhi;
	    double i_hadwlepton_delR;
	    double i_hadwlepton_delPt;
	    double i_hadwlepton_sumPt;

	    double i_hadbmet_delPhi;
	    double i_hadbmet_delPt;
	    double i_hadbmet_sumPt;

	    double i_hadwmet_delPhi;
	    double i_hadwmet_delPt;
		double i_hadwmet_sumPt;

		//member function~

		void InitBranch();

		void SetCorVar( JetInfo& jetInfo, LeptonInfo& leptonInfo, EvtInfo& evtInfo,\
			const vector<int>& sel_jets, const vector<int>& sel_b_jets, \
		   	int& idx_Selected_Lep, int& tmp_mva_hadb, int& tmp_mva_lepb, \
			int& tmp_mva_j1, int& tmp_mva_j2 );
		
		void SetIncorVar( JetInfo& jetInfo, LeptonInfo& leptonInfo, EvtInfo& evtInfo,\
			const vector<int>& sel_jets, const vector<int>& sel_b_jets, \
		   	int& idx_Selected_Lep, int& tmp_mva_hadb, int& tmp_mva_lepb, \
			int& tmp_mva_j1, int& tmp_mva_j2 );
		
		void WriteIn();
	};
	
	// below : used in using this mva
	
	void AddVarName( vector<string>& inputVars );

	void InputVar( double* var, JetInfo& jetInfo, LeptonInfo& leptonInfo, \
			EvtInfo& evtInfo, const vector<int>& sel_jets, const vector<int>& sel_b_jets, \
		   	int& idx_Selected_Lep, int& tmp_mva_hadb, int& tmp_mva_lepb, \
			int& tmp_mva_j1, int& tmp_mva_j2 );
}


#endif
