#include "cgt_codegen_EGH_decode.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string> 
#include <fstream>
#include "lemon/arg_parser.h"
#include "lemon/messenger.h"
#include "lemon/time_measure.h"

using namespace std;
using namespace lemon;
unsigned logging_priority_level=2;
unsigned number_of_errors=0;
const bool LOG_MSG=true;


int main(int argc, char** argv){
	ArgParser ap(argc,argv);
	cout <<"Demo of the EGH filter decoding algorithm\n";
	cout <<"[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai, and O. Rottenstreich, Bloom Filter with a False Positive Free Zone\n";
	ap.intOption("n", "The size of the universe", 100)
	  .intOption("d", "The number of items in the EGH filter", 2)
	  .intOption("log","The logging level: 0 - results and errors only, 1 - min, 6 - max info",2) 
	  .boolOption("fig4", "Generate data for Figure 4 of the paper")
	  .run(); 
	if (argc<=1) {
		error_msg<<"Try "<<argv[0]<<" --help\n";
		return 1;
	}
	int ll=ap["log"]; logging_priority_level=((unsigned) ll); 
	int d=ap["d"];
	int n=ap["n"];
	if (ap.given("fig4")){
		vector<int> n;
		n.push_back(1000);
		n.push_back(10000);
		n.push_back(100000);
		ofstream myfile;
		myfile.open ("result.xml");
		myfile << "<root>\n";
		for (int d=2;d<=10;d++){
			for (int i=0;i<n.size();i++){
				for (int j=1; j<1000; j++){
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
	} else {
		CgtCodegenEGHDecode cgt(d, "genl");
		cgt.generate(n);
		vector<vector <int> > y;
		cgt.randomItem(d, y);
		Timer TR;
		cgt.decode(y);
		log_msg(0) << "Run time:"<<TR.userTime();
	}
	return 0;
}
