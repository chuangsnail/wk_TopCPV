#ifndef TTT_H_
#define TTT_H_

#include "TopCPViolation/select/interface/frameDef.h"

//using namespace std;

class TTT
{
public:
	int i;

	TTT() { i=0; }
	~TTT() {}

	//void ttt();

	void ttt()
	{ cout << "Hello World!" << i << endl; }
};


//void TTT::ttt()
//{ cout << "Hello World!" << i << endl; }

inline void tttt()
{	cout << "Hi!" << endl;	}

#endif //TTT_H

