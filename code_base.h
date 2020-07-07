 //==============================================================
/// \file       : code_base.h
/// \brief Basic data structures for CGT and ECC codes
 //--------------------------------------------------------------
/// \author              Janos Tapolcai
/// \author              tapolcai@tmit.bme.hu
/// \date                May, 2012
 //==============================================================

#ifndef CODE_BASE_H
#define CODE_BASE_H

#include <iostream>
#include <math.h>
#include <list>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <time.h>


#include "lemon/messenger.h"
extern unsigned logging_priority_level;
extern unsigned number_of_errors;
extern const bool LOG_MSG;

using namespace std;

class CodeBase{
	public:
	/// an object of a test
	class Test{
		public:
		Test(){};
		vector<int> ones;
		/// reset the test
		void reset(){ones.clear();}
		/// set bit to one
		void add(int item) {
			/// store the items in increasing order
			if (ones.empty() || ones.back()<item){
			   ones.push_back(item);
			   return;
   			}
			for(vector<int>::iterator it = ones.begin(); it != ones.end(); ++it){
				if (*it>item) {
					//it--;
					ones.insert(it,item);
					return;
				}
				if (*it==item) {
					log_msg(3)<<"Item "<<item<<" is already part of the test";
					return;
				}
			}
		};
		/// returns true if the test has item
		bool has(int item){
			for(vector<int>::iterator it = ones.begin(); it != ones.end(); ++it)
				if(*it == item) return true;
			return false;
		}
	};
	
	/// an object of to sotre a code
	class Code{
		public:
		Code(int l){length=l;};
		vector<int> ones;
		int length;
		/// reset the code
		void reset(){ones.clear();}
		/// set bit to one
		void add(int item) {
			/// store the items in increasing order
			if (ones.empty() || ones.back()<item){
			   ones.push_back(item);
			   return;
   			}
			for(vector<int>::iterator it = ones.begin(); it != ones.end(); ++it){
				if (*it==item) {
					log_msg(4)<<"Item "<<item<<" is already part of the test";
					return;
				}
				if (*it>item) {
					ones.insert(it,item);
					return;
				}

			}
		};
		/// returns true if the test has item
		bool has(int item) const {
			for(vector<int>::const_iterator it = ones.begin(); it != ones.end(); ++it)
				if(*it == item) return true;
			return false;
		}
		void setDecimal(int dec) {
			int element_index = dec;
			for(int position = 0; position < length; position++){
                if(position > 0) element_index = (int)floor((double)(element_index/2));
				if(element_index%2 == 1){
					add(position);
				}
			}
		}
		/// this is needed for set
		/// returns true if this < other
		bool operator<( const Code& other ) const {
            if (ones.size() != other.ones.size())
                return ones.size() < other.ones.size();

            for (int i = 0; i < ones.size(); i++){
                if (ones[i] != other.ones[i])
                    return ones[i] < other.ones[i];
            }

            return false;

        }
        /// returns true if one code is part of the other
        bool operator<=( const Code& other ) const {
            int j = 0;
            for (int i = 0; i < ones.size(); i++){
                while(other.ones[j]<ones[i] && j < other.ones.size()) j++;
                if (j == other.ones.size())
                    return false; // there is one more 1
                if (other.ones[j] != ones[i])
                    return false;
            }
            return true;
        }
        bool operator==( const Code& other ) const {
            if (ones.size() != other.ones.size())
                return false;

            for (int i = 0; i < ones.size(); i++){
                if (ones[i] != other.ones[i])
                    return false;
            }

            return true;
        }
        Code& operator+=(const Code& other){
            for(vector<int>::const_iterator it = other.ones.begin(); it != other.ones.end(); ++it) add((*it));
            return *this;
        }
        // create a test as Bitwise OR of two tests
	    Code(const Code &t1, const Code &t2){
		   for(vector<int>::const_iterator it = t1.ones.begin(); it != t1.ones.end(); ++it) add((*it));
		   for(vector<int>::const_iterator it = t2.ones.begin(); it != t2.ones.end(); ++it) add((*it));
		   length=t1.length;
		};
		friend std::ostream& operator<< (std::ostream &os, Code &code){
			for(int i=0;i<code.length;i++) { if (code.has(i)) os<<"1"; else os<<"0";};
			return os;
		}
	};
	void convertCodes2Tests(const set<Code> & codes, int l){
		reset();
		for(int i=0;i<l;i++){
			Test test;
			int id=1;
			for(set<Code>::const_iterator it=codes.begin(); it!=codes.end();++it){
				if (it->has(i)) test.add(id);
				id++;
			}
			tests.push_back(test);
		}
	}
	void convertTest2Codes(set<Code> & codes, int l){
		for(int i=0;i<l;i++){
			Code code(code_length);
			int id=0;
			for(vector< Test >::iterator it = tests.begin();it != tests.end(); ++it){
				if (it->has(i+1)) code.add(id);
				id++;
			}

            if (code.ones.size() != 0 && codes.find(code) == codes.end())
			    codes.insert(code);
		}
	};
	//
	void addEmptyTests(int number){
		for(int i=0; i<number; i++) tests.push_back(Test());
	}
	int hammingDistance(char a1, char a2, char a3, char b1, char b2, char b3){
		int ret=0;
		if (a1!='x' && b1!='x' && a1!=b1) ret++;
		if (a2!='x' && b2!='x' && a2!=b2) ret++;
		if (a3!='x' && b3!='x' && a3!=b3) ret++;
		return ret;
	}		
public:
	// the resulted tests.
	vector< Test  > tests;
	int code_length;
	/// the maxinum number of items with the same number of tests
	int max_items;
	/// the minimum number of items with the same number of tests
	int min_items;
	/// the maxinum number of items in the next step of increasing the tests	
	int next_code_length;
	/// number of items
	int n;
	/// number of defective items
	int d;
	/// the name of the method
	string method;
	
	CodeBase() {code_length=0;};
	
	void reset(){
		for(vector < Test >::iterator it = tests.begin(); it != tests.end(); ++it){
			(*it).reset();
		}
		tests.clear();
		code_length=0;
	};
	
	virtual int generate(int _n) {
		error_msg<<"This is a dummy fucntion at CodeBase. Problem with virtual functions!";
		return 0;
	};
	// returns the next step of codes
	virtual int nextCodeLength(){
		 return next_code_length; 
	}
	
	bool verify(){
		bool ret=true;
		if (d>=1) {
			set<Code> codes;
			convertTest2Codes(codes,max_items);
			if (max_items!=codes.size()){
				error_msg<<"there are "<<max_items-codes.size()<<" duplicated codes";
				return false;
			}
			if (d>=2){
				set<Code> doublecodes;
				for(set<Code>::iterator it1=codes.begin(); it1!=codes.end();++it1){
					for(set<Code>::iterator it2=it1; it2!=codes.end();++it2){
						if (it1==it2) continue;
						Code dd(*it1,*it2);
						log_msg(2)<<"double codes "<<dd;
						if (codes.find(dd)!=codes.end()) {
							log_msg(1)<<"not unique A u B = C "<<dd;
							return false;					
						}
						if (doublecodes.find(dd)!=doublecodes.end()){
							log_msg(1)<<"not unique A u B = C u D "<<dd;
							return false;					
						}
						doublecodes.insert(dd);
					}
				}
			}
		}
		log_msg(0)<<"codes are verified";
		return ret;
	}
	/// write codes
	friend std::ostream& operator<< (std::ostream &os, CodeBase &cgt){
		int test_number = 0;
		os << "bitnum m="<<cgt.code_length<< " size of universe n="<<cgt.max_items<<endl;
		if (logging_priority_level<1) return os;
		#ifdef HTML_LOG
			os << "<br/>";
		#endif
		for(vector< Test >::iterator it = cgt.tests.begin();it != cgt.tests.end(); ++it){
			++test_number;
			os << "bit" << setw(2)<<setfill(' ') << test_number << ": ";
			for(int i = 1; i <= cgt.max_items; ++i){
				if(it->has(i)) os << 1;
				else os << 0;
				//if (i==cgt.n && cgt.method == "sep" && cgt.d==2) os <<" ";
			}
			#ifdef HTML_LOG
			os << "<br/>";
			#endif
			os << endl;
		}
		/// or
		//os <<endl<<"or"<<endl;
		#ifdef HTML_LOG
			os << "<br/>";
		#endif
		/*for(int i = 1; i <= cgt.max_items; ++i){
			for(vector< Test >::iterator it = cgt.tests.begin();it != cgt.tests.end(); ++it){
				if(it->has(i)) os << 1;
				else os << 0;
			}
			#ifdef HTML_LOG
			os << "<br/>";
			#endif
			os << endl;
		}*/
		return os;
	}
};
#endif //CODE_BASE_H
