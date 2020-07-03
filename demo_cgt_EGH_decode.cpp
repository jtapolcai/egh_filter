#include "cgt_codegen_EGH_decode.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string> 
#include <fstream>
//#include "lemon/arg_parser.h"
#include "lemon/messenger.h"
#include "lemon/time_measure.h"

using namespace std;
using namespace lemon;
unsigned logging_priority_level=2;
unsigned number_of_errors=0;
const bool LOG_MSG=true;


/*int main(int argc, char** argv){
	cout <<"Demo of the EGH filter decoding algorithm\n";
	cout <<"[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai and O. Rottenstreich, Bloom Filter with a False Positive Free Zone\n";
	int n=100; 
	int d=3;
	if (argc<3) {
		cerr<<"Try "<<argv[0]<<" [n] [d]\n";
		cout<<"[n] is the size of universe\n";
		cout<<"[d] is the number of items in the filter\n";
		cout<<"A demo run with n="<<n<<" and "<<d;
		//return 1;
	} else {
		n=atoi(argv[1]);
		d=atoi(argv[2]);
	}
	CgtCodegenEGHDecode cgt(d, "genl");
	cgt.generate(n);
	vector<vector <int> > y;
	cgt.randomItem(d, y);
	Timer TR;
	cgt.decode(y);
	log_msg(0) << "Run time:"<<TR.userTime();
	return 0;
}*/


int main(int argc, char** argv){
	logging_priority_level=1;
	vector<int> n;
	n.push_back(1000);
	n.push_back(10000);
	n.push_back(100000);
	ofstream myfile;
	myfile.open ("result.xml");
	myfile << "<root>\n";
	for (int d=2;d<=10;d++){
		for (int i=0;i<n.size();i++){
			for (int j=1; j<50; j++){
				CgtCodegenEGHDecode cgt(d, "genl");
				cgt.generate(n[i]);
				vector<vector <int> > y;
				cgt.randomItem(d, y);
				Timer TR;
				cgt.decode(y);
				myfile << "<run>\n";
				myfile <<"  <itemnum>"<<d<<"</itemnum>\n";
				myfile <<"  <universe>"<<n[i]<<"</universe>\n";
				myfile <<"  <time>"<<TR.userTime()<<"</time>\n";
				myfile << "</run>\n";
			}
		}
	}	
	myfile << "</root>\n";
	myfile.close();
	return 0;
}