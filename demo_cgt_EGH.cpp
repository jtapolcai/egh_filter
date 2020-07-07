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
	cout <<"EGH filter constructions\n";
	cout <<"[1]  S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai, and O. Rottenstreich, Bloom Filter with a False Positive Free Zone\n";
	ap.intOption("n", "The size of the universe", 100)
	  .intOption("d", "The number of items in the EGH filter", 2)
	  .boolOption("codes","Print the items in the EGH filter as bitvectors")
	  .boolOption("verify","Verify codes")
	  .boolOption("fig_fixd", "Generate a chart with fixed number of failures (Fig 2)")
	  .boolOption("fig_fixn", "Generate a chart with fixed number of item")
	  .intOption("fixm", "Search n and d for a maximum code length", 200)
	  .intOption("log","The logging level: 0 - results and errors only, 1 - min, 6 - max info",2)
	  .stringOption("file_prefix","Prefix of the file the results are written")
	  .run(); 
	if (argc<=1) {
		error_msg<<"Try "<<argv[0]<<" --help\n";
		return 1;
	}
	int ll=ap["log"]; logging_priority_level=((unsigned) ll); 
	int d=ap["d"];
	int n=ap["n"];
	if (ap.given("fig_fixn") || ap.given("fig_fixd")) {
		string out_file_name=((string)ap["file_prefix"])+"egh_filter.xml";
		log_msg(1)<<"Saving results in file " << out_file_name;
		ofstream os(out_file_name.c_str());
		if (!os) {
			error_msg<<"Error opening file "<<out_file_name;
			exit (-1);
		}
		os<<"<EGH_filter>\n";
		if (ap.given("fig-fixd")){
			int element=10;
			int lenght_was=0;
			do {
				CgtCodegenEGH cgt(d, "genl");
				cgt.generate(element);
				int max_items=cgt.max_items;
				os<<"\t<code>\n";
				os<<"\t\t<d>"<<d<<"</d>\n";
				os<<"\t\t<m>"<<cgt.code_length<<"</m>\n";
				os<<"\t\t<n>"<<cgt.min_items<<"</n>\n";
				os<<"\t</code>\n";
				os<<"\t<code>\n";
				os<<"\t\t<d>"<<d<<"</d>\n";
				os<<"\t\t<m>"<<cgt.code_length<<"</m>\n";
				os<<"\t\t<n>"<<max_items<<"</n>\n";
				os<<"\t</code>\n";
				cgt.nextCodeLength();
				if (element>=max_items) element++;
				else element=max_items+1;
			} while (element<n);
		}
		if (ap.given("fig-fixn")){
			for(int j=1;j<d;j++){
				CgtCodegenEGH cgt(j, "genl");
				cgt.generate(n);
				os<<"\t<code>\n";
				os<<"\t\t<d>"<<j<<"</d>\n";
				os<<"\t\t<m>"<<cgt.code_length<<"</m>\n";
				os<<"\t\t<n>"<<n<<"</n>\n";
				os<<"\t</code>\n";
				if (cgt.code_length>n) break;
			}
		}
		os<<"</EGH_filter>\n";
		os.close();
	} else {
		CgtCodegenEGH cgt(d, "genl");
		Timer TR;
        if (ap.given("fixm")){
            int mm=ap["fixm"];
            long j=5;
            long maxj=1000000*mm;
            log_msg(1)<<"Start iterative search till "<<maxj<<" mm="<<mm;
            for(;j<maxj;j++){
                log_msg(1)<<"try n="<<j;
                CgtCodegenEGH cgt_tmp(d, "genl");
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
		log_msg(1) <<"runtime was "<< TR.userTime()<<" sec";
		log_msg(1) <<"code length "<<cgt.code_length;
		if (ap.given("codes" )) {
			log_msg(1)<<cgt;
		}
		if (ap.given("verify")){
			cgt.verify();
		}
	}
	return 0;
}
