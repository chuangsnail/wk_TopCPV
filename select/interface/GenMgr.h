#ifndef GENMGR_H
#define GENMGR_H

#include "frameDef.h"

#include <limits.h>        //INT_MAX
#include <float.h>        //DBL_MAX
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "TLorentzVector.h"
#include "TVector3.h"
#include "TVector2.h"

enum bb_matching_type 
{ 
	correct ,
   	charge_mis ,
	mistag ,
	no_match
};

class GenMgr
{
public:   
	
    //members
    GenInfo* gen;
    JetInfo* jets;
    LeptonInfo* leps;
	string _ch;

	int counter;
    
    //Constructors
	/*
    GenMgr( GenInfo* genInfo )
    {   gen = genInfo;  }
    */

	GenMgr() 
	{
		_ch = "";
	}

    GenMgr( GenInfo* g, JetInfo* j, LeptonInfo* l )
    {	
		RegIn(g,j,l);	
		_ch = "";
		counter = 0;
	}

	void RegIn( GenInfo* genInfo, JetInfo* jetInfo, LeptonInfo* leptonInfo )
	{
        gen = genInfo;
        jets = jetInfo;
        leps = leptonInfo;
	}

    
    //Member fns
    
    void SetGenBranch( GenInfo* );
    void SetJetBranch( JetInfo* );
    void SetLepBranch( LeptonInfo* );

	string Get_channel() {	return _ch;	}

    int Find_GenParticle( const int& id_pdg, const int& mo_id );
    int Match_Gen( double gen_eta, double gen_phi );
    int GenJetIdx( int idx );       //input the idx in JetInfo, get the index in GenInfo, If there is no match , return -1
    int GenLepIdx( int idx );       //input the idx in LeptonInfo, get the index in GenInfo, If there is no match , return -1
	bool Check_Source_Mo( const int& gen_idx, const int& sourc_mo_pdgid );		//find the mother after checking its propagation
	void PrintInfo();
	int is_Gen_Jet_Seen( const int& idx );
	int is_Gen_Lep_Seen( const int& idx );
	//double delR();

	void DumpPropagateInfo( const int& idx );
	int GetPropagationEnd( const int& idx );
    
    //member & member function for MVA combination choosing
	bool Find_Correct_HadronicTop( int& cor_b, int& cor_j1, int& cor_j2 );

	bb_matching_type Get_bb_Option( const int&, const int&, const int& );
	bool IsJ1Correct( const int& );

	bool IsPositiveAcp( const string& );
	double GenAcp( const string& );

};

#endif
