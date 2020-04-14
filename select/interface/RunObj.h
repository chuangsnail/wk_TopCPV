#ifndef RUNOBJ_H
#define RUNOBJ_H

//#include "TopCPViolation/selected/interface/frameDef.h"
//#include "TopCPViolation/select/interface/BranchInfo.h"
#include "frameDef.h"

class Event
{
	friend class SelMgr;
private:
	EvtInfo* evt;
	VertexInfo* vtx;
public:

	//Constructor
	Event() {}
	Event( EvtInfo* e , VertexInfo* v ) { RegIn(e,v); }
	void RegIn( EvtInfo* e, VertexInfo* v ) { evt = e; vtx = v; }
		
	//info for pile up
	int nBX() const;
	int nPU(const int&) const;
	int BXPU(const int&) const;
	int TrueIT(const int&) const;
	
	//for electron ID
	float Rho() const;

	//info for MET
	double MET() const;
	double METPhi() const;

	//for vertex
	bool IsGoodPVertex( const int& i ) const;
	bool GoodVertex() const;
};




class Gen
{
	friend class SelMgr;
private:
	GenInfo* gen;
public:

	//Constructor
	Gen() {}
	Gen( GenInfo* g ) { RegIn(g); }
	void RegIn( GenInfo* g ) { gen = g; }
		
	double Pt(const int&) const;
	double Eta(const int&) const;
	double Phi(const int&) const;
	double Weight() const;
};


#endif //RUNOBJS_H
