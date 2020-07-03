#include "cgt_codegen_EGH.h"
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string> 
#include <fstream>
#include <lemon/arg_parser.h>
#include <lemon/time_measure.h>

using namespace lemon;
unsigned logging_priority_level=1;
unsigned number_of_errors=0;
const bool LOG_MSG=true;


int main(int argc, char** argv){
	ArgParser ap(argc,argv); 
	cout <<"Combinatorial Group Testing Constructions\n";
	cout <<"For the details see:\n";
	cout <<"[1]  D. Eppstein, M.T. Goodrich, and D. Hirschberg, Improved Combinatorial Group Testing Algorithms for Real-World Problem Sizes, SIAM Journal on Computing, 36,5 (2007) 1360-1375.\n";
	ap//.boolOption("codes","Print the code matrix")
	  .boolOption("verify","Verify codes")
	  .intOption("items", "Number of items", 10)
	  .boolOption("fixd", "Generate a chart with fixed number of failures")
	  .boolOption("fixn", "Generate a chart with fixed number of item")
      .intOption("falsep", "Count the number of false positives",0)
      .intOption("failnum", "Maximum number of failty items", 2)
    .intOption("max_code_length", "Maximum code length", 200)
	  .intOption("log","The logging level: 0 - results and errors only, 1 - min, 6 - max info",2) 
	  .stringOption("comment","Comment on the simulation")
	  .run(); 
	if (argc<=1) {
		error_msg<<"Try "<<argv[0]<<" --help\n";
		return 1;
	}
	int ll=ap["log"]; logging_priority_level=((unsigned) ll); 
	string method = "genl";
	int d=ap["failnum"];
	int n=ap["items"];
	if (ap.given("fixn") || ap.given("fixd")) {
		string cm=ap["comment"];
		string out_file_name="res/cgt_res_"+method+cm+".xml";
		log_msg(1)<<"Saving results in xml " << out_file_name;
		ofstream os(out_file_name.c_str());
		if (!os) {
			error_msg<<"Error opening file "<<out_file_name;
			exit (-1);                   // abnormal exit: error opening file
		}
		os<<"<cgt>\n";
		if (ap.given("fixd")){
			int element=10;
			int lenght_was=0;
			os<<"\t<Desc>for_d="<<d<<"_"<<method<<"</Desc>\n";
			do {
				CgtCodegenEGH cgt(d, method);
				cgt.generate(element);
				int max_items=cgt.max_items;
				os<<"\t<Network>\n";
				os<<"\t\t<Method>"<<d<<"_"<<method<<"</Method>\n";
				os<<"\t\t<BitNum>"<<cgt.code_length<<"</BitNum>\n";
				os<<"\t\t<ItemNum>"<<cgt.min_items<<"</ItemNum>\n";
				os<<"\t</Network>\n";
				os<<"\t<Network>\n";
				os<<"\t\t<Method>"<<d<<"_"<<method<<"</Method>\n";
				os<<"\t\t<BitNum>"<<cgt.code_length<<"</BitNum>\n";
				os<<"\t\t<ItemNum>"<<max_items<<"</ItemNum>\n";
				os<<"\t</Network>\n";
				cgt.nextCodeLength();
				if (element>=max_items) element++;
				else element=max_items+1;
				
			} while (element<n);
		}
		if (ap.given("fixn")){
			os<<"\t<Desc>for_"<<n<<"_items_"<<method<<"</Desc>\n";
			for(int j=1;j<d;j++){
				CgtCodegenEGH cgt(j, method);
				cgt.generate(n);
				os<<"\t<Network>\n";
				os<<"\t\t<Method>"<<d<<"_"<<method<<"</Method>\n";
				os<<"\t\t<BitNum>"<<cgt.code_length<<"</BitNum>\n";
				os<<"\t\t<ItemNum>"<<n<<"</ItemNum>\n";
				os<<"\t\t<MaxItemNum>"<<cgt.max_items<<"</MaxItemNum>\n";
				os<<"\t\t<FailNum>"<<j<<"</FailNum>\n";
				os<<"\t</Network>\n";
				if (cgt.code_length>n) break;
			}
		}
		os<<"</cgt>\n";
		os.close();
	} else {
		CgtCodegenEGH cgt(d, method);
		Timer TR;
        if (ap.given("max_code_length")){
            int mm=ap["max_code_length"];
            long j=5;
            long maxj=1000000*mm;
            log_msg(1)<<"start iterative search till "<<maxj<<" mm="<<mm;
            for(;j<maxj;j++){
                log_msg(1)<<"try n="<<j;
                CgtCodegenEGH cgt_tmp(d, method);
                cgt_tmp.generate(j);
                if (cgt_tmp.code_length>mm) {
                    n=cgt_tmp.min_items-1;
                    log_msg(1)<<"too large, break";
                    break;
                }
                j=cgt_tmp.max_items+1;
            }
            log_msg(1)<<"We set n="<<n<<" to get code length at most "<<mm;
        }
        cgt.generate(n);
		//os << XML("FullRunningTime",TR.userTime());
		log_msg(1) << TR.userTime();
		log_msg(1)<<"code length "<<cgt.code_length;
		//if (ap.given("codes" )) {
		//	log_msg(0)<<cgt;
		//}
		if (ap.given("verify")){
			cgt.verify();
		}
	}
	return 0;
}
