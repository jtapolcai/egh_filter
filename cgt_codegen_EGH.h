 //==============================================================
/// \file       : cgt_codegen_EGH.h
/// \brief Eppstein, Goodrich, Hirschberg, 
///        "Improved Combinatorial Group Testing Algorithms for Real-World Problem Sizes"
 //--------------------------------------------------------------
/// \author              Janos Tapolcai
/// \author              tapolcai@tmit.bme.hu
/// \date                June, 2009
 //==============================================================

#ifndef CGT_CODEGEN_EGH_H
#define CGT_CODEGEN_EGH_H

#include "code_base.h"

class CgtCodegenEGH : public CodeBase {
public:
	typedef vector<int> Primlist;
private:	
	/// backtracking
	pair<int, int* > searchPrimes(int maxpow, double log_target, int start) {
		/// Backtracking search for exponents of prime powers, each at most maxpow,
		/// so that the product of the powers is at least target and the sum of the
		/// nonunit powers is minimized. Returns the pair [sum,list of exponents].		
		log_msg(4)<<"searchPrimes(maxpow="<<maxpow<<", target="<<exp(log_target)<<", strat="<<start<<")";
		int primnum=allprimes.size();
		pair<int, int* > best(0 , NULL); // no solution found yet
		if (primnum==0) return best;
		if (log_target <= 0) // all unit powers will work?
		{
			best.second=new int[primnum];
			int i=0;
			for(Primlist::const_iterator itl=allprimes.begin();itl!=allprimes.end();++itl){
				best.second[i]=0;
				i++;
			}
			log_msg(3)<<" ok";
			return best;
		}
		else if (primnum==0 || start>=primnum /*|| pow(maxpow,primnum-start) < target*/){
			log_msg(3)<<" no";
			return best; // no primes supplied, no solution exists
		}
		if (primnum<=start) {			
		    log_msg(3)<<"no more primes avaliable";
		    return best;
		}
		int p = allprimes[start];
		int i = 0;
		double log_pi=0;
		if (p<=1){
			error_msg<<"It is not a prime: "<<p; 
			exit(-1);
		}
		while ( exp(log_pi) <= maxpow) { // loop through possible exponents of p
			log_msg(3)<<"try "<<p<<"^"<<i;
			pair<int,int* > s = searchPrimes(maxpow, log_target-log_pi, start+1);
			if (s.second!=NULL) {
				s.first += (i==0?0:exp(log_pi)); // i and pi;
				s.second[start]=i;
				if (best.second==NULL || best.first>s.first) {
					delete best.second;
					best.first  =  s.first;
					best.second = s.second;
					if (start==0){
						log_msg(2)<<"reached ("<<best.first<<"): ";
						for(int j=0;j<allprimes.size();j++){
							int prim=(int)floor(pow((double)allprimes[j],best.second[j])+0.5);
							if (prim>1) {
								log_msg(2)<<prim<<", ";
							}
						}
					}
				} else {
					delete s.second;
					//break;
				}
			}
			i ++;
			log_pi+=log(p);
		}
		return best;
	}
	// generating primes has been finished
	void generatePrimesTill(double log_nd){
		log_msg(3)<<"prime list till "<<exp(log_nd)<<" q="<<q<<" product="<< exp(log_product);
		if (log_product > log_nd) return;
		while (log_product <= log_nd)
		{
			Mapcomp::iterator it=D.find(q);
			if (it== D.end()) {
				allprimes.push_back(q);
				log_product+=log(q);
				sum+=q;				
				D[q*q].push_back(q);
			} else {
				for(list<int>::iterator itl=it->second.begin();itl!=it->second.end();++itl){
					D[*itl+q].push_back(*itl);	
				}
				D.erase(it);
			}
			q++;
			if (log_product>log_nd) break;
		};	
		log_msg(2)<<"Primes till Prod="<<exp(log_nd)<<" ("<<exp(log_product)<<") are generated:";	
		if (logging_priority_level>=2){
			for(Primlist::iterator itl=allprimes.begin();itl!=allprimes.end();++itl){
				cout<<(*itl)<<", ";
			}
		}
	}
	int getLengthChineseRemainder(int _n){
		int ret=0;
		if (log_product<=0) EppsteinGoodrichHirschberg();
		double log_nd=log(_n)*d;//pow((double)_n,d);
		generatePrimesTill(log_nd);
		ret=sum;
		if(method == "bktrk"){
			///  heuristic to reduce the number of primes
			int maxpow=allprimes[allprimes.size()-1];
			pair<int, int*> e=searchPrimes(maxpow, log_nd,0);
			primes.clear();
			if(e.second){
				int sum_new=0;
				for(int i=0;i<allprimes.size();i++){
					int prim=(int)(pow((double)allprimes[i],e.second[i])+0.5);					
					if (prim>1) {
						log_msg(2)<<prim<<", ";
						sum_new+=prim;
						primes.push_back(prim);
					}
				}
				ret=sum_new;
			}
		}
		return ret;
	}
	public: 
	Primlist primes;
	Primlist allprimes;
	double log_product;
	int sum;
	/// Generate the sequence of prime numbers via the sieve of Eratosthenes.
	typedef map< int,  list < int > > Mapcomp;
	Mapcomp D; // map composite integers to primes witnessing their compositeness
	int q;   // first integer to test for primality
	/// Improved Combinatorial Group Testing Algorithms for Real-World Problem Sizes
	void EppsteinGoodrichHirschberg(){
		log_msg(1)<<"Generating "<<d<<"-disjunct codes with n="<<n;		
		allprimes.clear();
		log_product=0;
		sum=0;
		double log_nd=log(n)*d;//pow((double)n,d);
		/// Generate the sequence of prime numbers via the sieve of Eratosthenes.
		D.clear();
		q=2;   // first integer to test for primality
		generatePrimesTill(log_nd);		
		log_msg(2)<<"multiplied together: "<<exp(log_product)<<" while the sum is "<<sum;
		code_length=sum;
		max_items=floor(exp(log_product/d));
		int maxpow=allprimes[allprimes.size()-1];
		int maxprime=allprimes[allprimes.size()-1];
		min_items=ceil(exp((log_product-log(maxprime))/d));
		log_msg(2)<<"max items:"<<max_items<<", min items:"<<min_items;				
		log_msg(2)<<"maxpow: "<<(int)(log_nd/log((double)2)+0.5)<<" (last prim="<<maxprime<<")";
		if(method == "genl"){
			primes.clear();
			for(Primlist::iterator itl=allprimes.begin();itl!=allprimes.end();++itl){
				for(int i = 0; i < *itl; ++i){
					Test test;
					for(int j = 1; j <= max_items; j++)
						if(j%(*itl) == (i+1)%(*itl)) test.add(j);
					tests.push_back(test);
				}
				primes.push_back(*itl);
			}
			return;
		}
		if(method == "bktrk"){
			log_msg(1)<<"Run backtracking";
			///  heuristic to reduce the number of primes
			pair<int, int*> e=searchPrimes(maxpow, log_nd,0);
			if(e.second){
				// generating relative primes
				log_msg(2)<<"After optimisation relative primes till "<<exp(log_nd)<<" ";
				double log_product_new=0;
				int sum_new=0;
				primes.clear();
				int max_prime=0;
				for(int i=0;i<allprimes.size();i++){
					int prim=(int)(pow((double)allprimes[i],e.second[i])+0.5);					
					if (prim>1) {
						log_msg(2)<<prim<<", ";
						sum_new+=prim;
						log_product_new+=log(prim);
						primes.push_back(prim);
						if (prim>max_prime) max_prime=prim;
					}
				}
				log_msg(2)<<"multiplied together: "<<exp(log_product_new)<<" while the sum is "<<sum_new<<" (="<<e.first<<")";
				max_items=floor(exp(log_product_new/d));
				min_items=ceil(exp((log_product-log(max_prime))/d)); // ???
			}
			for(Primlist::iterator itl=primes.begin();itl!=primes.end();++itl){
				for(int i = 0; i < *itl; ++i){
					if(*itl == 1) break;
					Test test;
					for(int j = 1; j <= max_items; j++)
						if(j%(*itl) == (i+1)%(*itl)) test.add(j);
					tests.push_back(test);
				}
			}
		}
	}
	
public:
	
	CgtCodegenEGH(int _d, string method_): CodeBase() {
		code_length=0;
		d=_d;
		method=method_;
		log_product=-1;
	};
	

	/// generate codes
	virtual int generate(int _n) {
		n=_n;
		reset();
		if(method == "genl" || method == "bktrk"){
			EppsteinGoodrichHirschberg();
		} else {
		    error_msg << "Incorrect CGT codegen method!" <<method;
		}
		code_length=tests.size();
		return code_length;
	}
		
	// returns the next step of codes
	virtual int nextCodeLength(){
		if (method == "genl" || method == "bktrk"){
			int element=max_items+1;
			int i=100000;
			do {
				int length=getLengthChineseRemainder(element);
				if (code_length>=length) element++;
				else {
			 		max_items=element;
					return code_length;
				}
				i--;
			} while (i>0);
			error_msg<<"Error: recuriosn was limitted to 100000";
			return 2*code_length;
		}
		else return next_code_length; 
	}	
};
#endif //CGT_CODEGEN_H
