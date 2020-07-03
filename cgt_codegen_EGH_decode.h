 //==============================================================
/// \file       : cgt_codegen_EGH_decode.h
/// \brief Listing EGH filter 
///        S. Z. Kiss, E. Hosszu, J. Tapolcai, L. Ronyai and O. Rottenstreich, 
///        "Bloom Filter with a False Positive Free Zone"
 //--------------------------------------------------------------
/// \author              Janos Tapolcai, 
/// \author              janos.tapolcai@gmail.com
///
///                      The Strum Sequence solver is from
///                      Using Sturm Sequences to Bracket Real Roots of Polynomial Equations
///                      by D.G. Hook and P.R. McAree
///                      from "Graphics Gems", Academic Press, 1990
///
///                      The codes of solving involve modular linear 
///                      equations is taken from Stanford ACM-ICPC repository
///                      https://github.com/jaehyunp/stanfordacm
///
///                      For arbitrary precision number arithmetics we use
///                      Bignum C++ library by Tomasz Sowa <t.sowa AnTispam ttmath.org>
///
/// \date                2020
///==============================================================

#ifndef CGT_CODEGEN_EGH_DECODE_H
#define CGT_CODEGEN_EGH_DECODE_H

#include "code_base.h"
#include "cgt_codegen_EGH.h"
#include <iostream>
#include <vector>
#include <algorithm>

#include <ttmath/ttmath.h>

using namespace std;

#define	 	COEFTYPE  		ttmath::Big<6,3>
#define	 	BIGINT  		ttmath::Int<7>
//#define	 	COEFTYPE  		ttmath::Big<4,2>
//#define	 	BIGINT  		ttmath::Int<4>
//#define	 	BIGUINT  		ttmath::UInt<2>

#define	 	MAX_ORDER  		11
/* maximum order for a polynomial */

#define NONZERO(X) X>RELERROR || X<-RELERROR
#define ZERO(X) X>-RELERROR && X<RELERROR
#define	  	RELERROR	  		1.0e-3
/* smallest relative error we want */

#define	  	MAXPOW	    	32		
/* max power of 10 we wish to search to */

#define	  	MAXIT	     	800
/* max number of iterations */

/* a coefficient smaller than SMALL_ENOUGH is considered to 
   be zero (0.0). */

#define	  	SMALL_ENOUGH		1.0e-2

class CgtCodegenEGHDecode : public CgtCodegenEGH {

	typedef vector<BIGINT> VI;
	typedef pair<BIGINT,BIGINT> PII;
	
	// return a % b (positive value)
	BIGINT mod(BIGINT a, BIGINT b) {
	  BIGINT ret=((a%b)+b)%b;
	  log_msg(4)<<"mod("<<a<<","<<b<<")="<<ret;
	  return ret;
	}
	
	// computes gcd(a,b)
	BIGINT gcd(BIGINT a, BIGINT b) {
	  BIGINT tmp;
	  while(b>0){a%=b; tmp=a; a=b; b=tmp;}
	  return a;
	}
	
	// computes lcm(a,b)
	BIGINT lcm(BIGINT a, BIGINT b) {
	  return a/gcd(a,b)*b;
	}
	
	// returns d = gcd(a,b); finds x,y such that d = ax + by
	BIGINT extended_euclid(BIGINT a, BIGINT b, BIGINT &x, BIGINT &y) {  
	  BIGINT xx = y = 0;
	  BIGINT yy = x = 1;
	  while (b!=0) {
	    BIGINT q = a/b;
	    BIGINT t = b; b = a%b; a = t;
	    t = xx; xx = x-q*xx; x = t;
	    t = yy; yy = y-q*yy; y = t;
	  }
	  return a;
	}
	
	// finds all solutions to ax = b (mod n)
	VI modular_linear_equation_solver(BIGINT a, BIGINT b, BIGINT n) {
	  BIGINT x, y;
	  VI solutions;
	  BIGINT d = extended_euclid(a, n, x, y);
	  if (b%d==0) {
	    x = mod (x*(b/d), n);
	    for (BIGINT i = 0; i < d; i++)
	      solutions.push_back(mod(x + i*(n/d), n));
	  }
	  return solutions;
	}
	
	// computes b such that ab = 1 (mod n), returns -1 on failure
	BIGINT mod_inverse(BIGINT a, BIGINT n) {
	  BIGINT x, y;
	  BIGINT d = extended_euclid(a, n, x, y);
	  if (d > 1) return -1;
	  return mod(x,n);
	}
	
	// Chinese remainder theorem (special case): find z such that
	// z % x = a, z % y = b.  Here, z is unique modulo M = lcm(x,y).
	// Return (z,M).  On failure, M = -1.
	PII chinese_remainder_theorem_special_case(BIGINT x, BIGINT a, BIGINT y, BIGINT b) {
	  log_msg(4)<<"Chinese Remainder Theorem Special Case "<<x<<" "<<a<<" "<<y<<" "<<b;
	  BIGINT s, t;
	  BIGINT d = extended_euclid(x, y, s, t);
	  if (a%d != b%d) return make_pair(0, -1);
	  log_msg(4)<<"s "<<s<<" t "<<t;
	  BIGINT mm=mod(s*b*x+t*a*y,x*y);
	  log_msg(4)<<"Extended Euclid "<<d<<" mm "<<mm;
	  return make_pair(mm/d, x*y/d);
	}
	
	// Chinese remainder theorem: find z such that
	// z % x[i] = a[i] for all i.  Note that the solution is
	// unique modulo M = lcm_i (x[i]).  Return (z,M).  On 
	// failure, M = -1.  Note that we do not require the a[i]'s
	// to be relatively prime.
	PII chinese_remainder_theorem(const VI &x, const VI &a) {
	  PII ret = make_pair( a[0], x[0]);
	  for (int i = 1; i < x.size(); i++) {
	    ret = chinese_remainder_theorem_special_case(ret.second, ret.first, x[i], a[i]);
	    if (ret.second == -1) break;
	  }
	  return ret;
	}
	PII chinese_remainder_theorem_int(const vector<int> &x, const vector<int> &a) {
	  PII ret = make_pair((BIGINT) a[0], (BIGINT) x[0]);
	  for (int i = 1; i < x.size(); i++) {
	    ret = chinese_remainder_theorem_special_case(ret.second, ret.first, (BIGINT) x[i], (BIGINT) a[i]);
	    log_msg(4)<<"Chinese Remainder Theorem Special Case returned " << ret.first << " modulo " << ret.second ;
		if (ret.second == -1) break;
	  }
	  log_msg(3)<<"Chinese Remainder Theorem " << ret.first << " modulo " << ret.second ;
	  return ret;
	}

	/// sturm chain
	/*
	 * structure type for representing a polynomial
	 */
	typedef  	struct	p {
			     int	ord;
			     COEFTYPE	coef[MAX_ORDER+1];
	} poly;
	
	/*
	 * modp
	 *
	 *	calculates the modulus of u(x) / v(x) leaving it in r, it
	 *  returns 0 if r(x) is a constant.
	 *  note: this function assumes the leading coefficient of v 
	 *	is 1 or -1
	 */
	static int
	modp(poly	*u, poly *v, poly *r)
	{
		int		k, j;
		COEFTYPE	*nr, *end, *uc;
	
		nr = r->coef;
		end = &u->coef[u->ord];
	
		uc = u->coef;
		while (uc <= end)
				*nr++ = *uc++;
	
		if (v->coef[v->ord] < 0.0) {
	
	
				for (k = u->ord - v->ord - 1; k >= 0; k -= 2)
					r->coef[k] = -r->coef[k];
	
				for (k = u->ord - v->ord; k >= 0; k--)
					for (j = v->ord + k - 1; j >= k; j--)
						r->coef[j] = -r->coef[j] - r->coef[v->ord + k]
						* v->coef[j - k];
		} else {
				for (k = u->ord - v->ord; k >= 0; k--)
					for (j = v->ord + k - 1; j >= k; j--)
					r->coef[j] -= r->coef[v->ord + k] * v->coef[j - k];
		}
	
		k = v->ord - 1;
		while (k >= 0 && r->coef[k] < SMALL_ENOUGH && r->coef[k] > -SMALL_ENOUGH) {
			r->coef[k] = 0.0;
			k--;
		}
	
		r->ord = (k < 0) ? 0 : k;
	
		return(r->ord);
	}
	
	/*
	 * buildsturm
	 *
	 *	build up a sturm sequence for a polynomial in smat, returning
	 * the number of polynomials in the sequence
	 */
	int
	buildsturm(int ord, poly *sseq)
	{
		int		i;
		COEFTYPE	f, *fp, *fc;
		poly	*sp;
	
		sseq[0].ord = ord;
		sseq[1].ord = ord - 1;
	
	
		/*
		 * calculate the derivative and normalise the leading
		 * coefficient.
		 */
		f = sseq[0].coef[ord] * ord;
		if (f<0) f=-f; //fabs(
		fp = sseq[1].coef;
		fc = sseq[0].coef + 1;
		for (i = 1; i <= ord; i++)
				*fp++ = *fc++ * i / f;
	
		/*
		 * construct the rest of the Sturm sequence
		 */
		for (sp = sseq + 2; modp(sp - 2, sp - 1, sp); sp++) {
	
			/*
			 * reverse the sign and normalise
			 */
			if (sp->coef[sp->ord] < 0) 
				f = sp->coef[sp->ord];
			else f = -sp->coef[sp->ord];
			//f = -fabs(sp->coef[sp->ord]);
			for (fp = &sp->coef[sp->ord]; fp >= sp->coef; fp--)
					*fp /= f;
		}
	
		sp->coef[0] = -sp->coef[0];	/* reverse the sign */
	
		return(sp - sseq);
	}
	
	/*
	 * numroots
	 *
	 *	return the number of distinct real roots of the polynomial
	 * described in sseq.
	 */
	int
	numroots(int np, poly *sseq, int *atneg, int *atpos)
	{
			int		atposinf, atneginf;
			poly	*s;
			COEFTYPE	f, lf;
	
			atposinf = atneginf = 0;
	
	
		/*
		 * changes at positive infinity
		 */
		lf = sseq[0].coef[sseq[0].ord];
	
		for (s = sseq + 1; s <= sseq + np; s++) {
				f = s->coef[s->ord];
				if (lf == 0.0 || lf * f < 0)
					atposinf++;
			lf = f;
		}
	
		/*
		 * changes at negative infinity
		 */
		if (sseq[0].ord & 1)
				lf = -sseq[0].coef[sseq[0].ord];
		else
				lf = sseq[0].coef[sseq[0].ord];
	
		for (s = sseq + 1; s <= sseq + np; s++) {
				if (s->ord & 1)
					f = -s->coef[s->ord];
				else
					f = s->coef[s->ord];
				if (lf == 0.0 || lf * f < 0)
					atneginf++;
				lf = f;
		}
	
		*atneg = atneginf;
		*atpos = atposinf;
	
		return(atneginf - atposinf);
	}
	
	/*
	 * numchanges
	 *
	 *	return the number of sign changes in the Sturm sequence in
	 * sseq at the value a.
	 */
	int
	numchanges(int np, poly *sseq, COEFTYPE a)	
	{
		int		changes;
		COEFTYPE	f, lf;
		poly	*s;
	
		changes = 0;
	
		lf = evalpoly(sseq[0].ord, sseq[0].coef, a);
		for (s = sseq + 1; s <= sseq + np; s++) {
				f = evalpoly(s->ord, s->coef, a);
				if (lf == 0.0 || lf * f < 0)
					changes++;
				lf = f;
		}
		log_msg(4)<<"Numchanges :"<<changes;
		return(changes);
	}
	
	/*
	 * sbisect
	 *
	 *	uses a bisection based on the sturm sequence for the polynomial
	 * described in sseq to isolate intervals in which roots occur,
	 * the roots are returned in the roots array in order of magnitude.
	 */
	void sbisect(int np, poly *sseq, COEFTYPE min, COEFTYPE max, int atmin, int atmax, COEFTYPE *roots)
	{
		COEFTYPE	mid;
		int		n1 = 0, n2 = 0, its, atmid, nroot;
	
		if ((nroot = atmin - atmax) == 1) {
	
			/*
			 * first try a less expensive technique.
			 */
			if (modrf(sseq->ord, sseq->coef, min, max, &roots[0]))
				return;
	
	
			/*
			 * if we get here we have to evaluate the root the hard
			 * way by using the Sturm sequence.
			 */
			for (its = 0; its < MAXIT; its++) {
					mid = (min + max) / 2;
	
					atmid = numchanges(np, sseq, mid);
	
					if (NONZERO(mid)) {
						if (ZERO((max - min) / mid)) {
							roots[0] = mid;
							return;
						}
					} else if (ZERO(max - min)) {
						roots[0] = mid;
						return;
					}
	
					if ((atmin - atmid) == 0)
						min = mid;
					else
						max = mid;
				}
	
			if (its == MAXIT) {
					error_msg<< "sbisect: overflow min "<<min<<" max "<<max
						<<" diff "<<max - min<<" nroot "<<nroot<<" n1 "<<n1
						<<" n2 "<<n2<<"\n";
				roots[0] = mid;
			}
	
			return;
		}
	
		/*
		 * more than one root in the interval, we have to bisect...
		 */
		for (its = 0; its < MAXIT; its++) {
				log_msg(4)<<"bisect: min "<<min<<" max "<<max;
				mid = (min + max) / 2;
	
				atmid = numchanges(np, sseq, mid);
	
				n1 = atmin - atmid;
				n2 = atmid - atmax;
	
	
				if (n1 != 0 && n2 != 0) {
					sbisect(np, sseq, min, mid, atmin, atmid, roots);
					sbisect(np, sseq, mid, max, atmid, atmax, &roots[n1]);
					break;
				}
	
				if (n1 == 0)
					min = mid;
				else
					max = mid;
		}
	
		if (its == MAXIT) {
				error_msg << "sbisect: roots too close together";
				log_msg(0) << "sbisect: overflow min "<<min<<" max "<<max
						<<" diff "<<max - min<<" nroot "<<nroot<<" n1 "<<n1
						<<" n2 "<<n2<<" atmin "<<atmin<<" atmid "<<atmid<<" atmax "<<atmax;
				for (n1 = atmax; n1 < atmin; n1++)
				roots[n1 - atmax] = mid;
		}
	} 

	/*
	 * evalpoly
	 *
	 *	evaluate polynomial defined in coef returning its value.
	 */
	COEFTYPE
	evalpoly (int ord, COEFTYPE *coef, COEFTYPE x)
	{
		COEFTYPE	*fp, f;
	
		fp = &coef[ord];
		f = *fp;
	
		for (fp--; fp >= coef; fp--)
		f = x * f + *fp;
			string coeff_str="f(z)=";
		for (int i = ord; i >= 0; i--){
			if (coeff_str!="f(z)=" && coef[i] >= 0)
				coeff_str+=" +";
			else
				coeff_str+=" ";
			if (coef[i]!=1) 
				coeff_str+=(string) coef[i].ToString();
			if (i==1)
				coeff_str+="z";
			if (i>=2)
				coeff_str+=+"z^"+to_string(i);
		}
		log_msg(4)<<"evalpoly "<<coeff_str<<" at "<<x<<" return: "<<f;
		return(f);
	}
	
	
	/*
	 * modrf
	 *
	 *	uses the modified regula-falsi method to evaluate the root
	 * in interval [a,b] of the polynomial described in coef. The
	 * root is returned in *val. The routine returns zero
	 * if it can't converge.
	 */
	int
	modrf(int ord, COEFTYPE *coef, COEFTYPE a, COEFTYPE b, COEFTYPE *val)
	{
		int		its;
		COEFTYPE	fa, fb, x, fx, lfx;
		COEFTYPE	*fp, *scoef, *ecoef;
	
		scoef = coef;
		ecoef = &coef[ord];
	
		fb = fa = *ecoef;
		for (fp = ecoef - 1; fp >= scoef; fp--) {
			fa = a * fa + *fp;
			fb = b * fb + *fp;
		}
	
		/*
		 * if there is no sign difference the method won't work
		 */
		if (fa * fb > 0.0)
			return(0);
	
		if (ZERO(fa) ) {
			*val = a;
			return(1);
		}
	
		if (ZERO(fb)) {
			*val = b;
			return(1);
		}
	
		lfx = fa;
	
	
		for (its = 0; its < MAXIT; its++) {
	
			x = (fb * a - fa * b) / (fb - fa);
	
			fx = *ecoef;
			for (fp = ecoef - 1; fp >= scoef; fp--)
					fx = x * fx + *fp;
	
			if (NONZERO(x) ) {
					if (ZERO(fx / x) ) {
						*val = x;
						return(1);
					}
			} else if (ZERO(fx) ) {
					*val = x;
					return(1);
			}
	
			if ((fa * fx) < 0) {
					b = x;
					fb = fx;
					if ((lfx * fx) > 0)
						fa /= 2;
			} else {
					a = x;
					fa = fx;
					if ((lfx * fx) > 0)
						fb /= 2;
			}
	
			lfx = fx;
		}
	
		error_msg << "modrf overflow "<<a<<" "<<b<<" "<<fx;
	
		return(0);
	}
	
	/*
	Using Sturm Sequences to Bracket Real Roots of Polynomial Equations
	by D.G. Hook and P.R. McAree
	from "Graphics Gems", Academic Press, 1990
	*/

	bool find_root(poly	f, int order, vector<int> res)
	{
		poly	sseq[MAX_ORDER];
		COEFTYPE 	min, max, roots[MAX_ORDER];
		int		i, j, nroots, nchanges, np, atmin, atmax;
	
		for (i = order; i >= 0; i--) 
			sseq[0].coef[i]=f.coef[order-i];
		//
		log_msg(2)<<"Find root of Sturm sequence: order "<<order<<" np:"<<np;
		string coeff_str="f(z)=";
		for (i = order; i >= 0; i--){
			log_msg(4)<<"coeff:"<<sseq[0].coef[i].ToString()<<"z^"<<i;
			if (coeff_str!="f(z)=" && sseq[0].coef[i] >= 0)
				coeff_str+=" +";
			else
				coeff_str+=" ";
			if (sseq[0].coef[i]!=1) 
				coeff_str+=(string) sseq[0].coef[i].ToString();
			if (i==1)
				coeff_str+="z";
			if (i>=2)
				coeff_str+=+"z^"+to_string(i);
		}
		log_msg(2)<<coeff_str;

		np = buildsturm(order, sseq);

		for (i = 0; i <= np; i++) {
			coeff_str="";
			for (j = sseq[i].ord; j >= 0; j--)
				coeff_str+= sseq[i].coef[j].ToString()+" ";
			log_msg(2)<<coeff_str;
		}
	
	
		//get the number of real roots

		nroots = numroots(np, sseq, &atmin, &atmax);
	
		if (nroots == 0) {
				error_msg<<"solve: no real roots\n";
				exit(0);
		}
	
		// calculate the bracket that the roots live in

		min = COEFTYPE(-1.0);
		nchanges = numchanges(np, sseq, min);
		for (i = 0; nchanges != atmin && i != MAXPOW; i++) { 
				min *= 10.0;
				nchanges = numchanges(np, sseq, min);
		}
	
		if (nchanges != atmin) {
				error_msg<<"solve: unable to bracket all negative roots\n";
				atmin = nchanges;
		}
		max = COEFTYPE(1.0);
		nchanges = numchanges(np, sseq, max);
		for (i = 0; nchanges != atmax && i != MAXPOW; i++) { 
				max *= 10.0;
				nchanges = numchanges(np, sseq, max);
		}
	
		if (nchanges != atmax) {
				error_msg<<"solve: unable to bracket all positive roots\n";
				atmax = nchanges;
		}
	
		nroots = atmin - atmax;
	
		// perform the bisection.

		sbisect(np, sseq, min, max, atmin, atmax, roots);
	
	
		// write out the roots...

		if (nroots == 1) {
				log_msg(1)<<"1 distinct real root at x = "<<roots[0]<<endl;
		} else {
				coeff_str="";
				for (i = 0; i != nroots; i++){
					coeff_str+= roots[i].ToString()+" ";
					int rr=floor(roots[i].ToFloat()+0.001);
					res.push_back(rr);
				}
				log_msg(1)<<nroots<<" distinct real roots: " << coeff_str;
		}
		return true;
	} 
 
	int modpow(int base, int exp, int modulus) {
	  base %= modulus;
	  int result = 1;
	  while (exp > 0) {
	    if (exp & 1) result = (result * base) % modulus;
	    base = (base * base) % modulus;
	    exp >>= 1;
	  }
	  return result;
	}
	
public:
	
	CgtCodegenEGHDecode(int _d, string method_): CgtCodegenEGH (_d,method_) {
	};
	

	/// generate codes
	virtual int generate(int _n) {
		n=_n;
		return CgtCodegenEGH::generate(n);	
	}
		
	void randomItem(int ii, vector<vector<int> > &y){
		string item_str="";
		for (int i=0;i<ii;i++){
			vector<int> uj; 
			int x=rand()%n;
			item_str+=to_string(x)+" "; 
			int iii=0;
			int k=primes.size();
			for( int i=0; i<k; i++){
				int xx=x%primes[i];
				uj.push_back(x%primes[i]);
			}
			y.push_back(uj);
		}
		log_msg(1)<<"Picked "<<ii<<" items randomly: "<<item_str;
	}
		
	int decode(vector<vector<int> > y) {
		log_msg(2)<<"Decoding started";
		VI A;
		for (int j=1; j<=d-1; j++){
			vector<int> ai;
			for( int i=0; i<primes.size(); i++){
				int aij=0;
				for(int i2=0;i2<d;i2++){
					aij+=modpow(y[i2][i],j,primes[i]);
				}
				aij=aij%primes[i];
				log_msg(3)<<"The remainder for prime "<<primes[i]<<" is "<<aij;
				ai.push_back(aij);
			}
			PII ret = chinese_remainder_theorem_int(primes,ai);
			A.push_back(ret.first);
		}
		VI sigma;
		sigma.push_back(A[0]);
		log_msg(2)<<"sigma_1="<<A[0];
		log_msg(2)<<"Generate elementary symetric polynomials";
		log_msg(3)<<"A_1="<<A[0];
		for (int j=2; j<=d-1; j++){
			BIGINT sigmaj=A[j-1];
			log_msg(3)<<"A_"<<j<<"="<<A[j-1];
			BIGINT elojel=-1;
			for( int i=0; i<j-1; i++){					
				sigmaj+=elojel*A[j-i-2]*sigma[i];
				elojel*=-1;
				log_msg(4)<<A[j-i-2]<<","<<sigma[i];
				log_msg(3)<<"Coeff: -1^"<<i<<"*A_{"<<j-i-1<<"}*A_"<<i-1<<"="<<A[j-i-2]*sigma[i];
			}
			sigmaj/=j;
			if (j%2==0) sigmaj*=-1;
			log_msg(2)<<"sigma_"<<j<<"="<<sigmaj;
			sigma.push_back(sigmaj);
		}
		vector<int> prody;
		for( int i=0; i<primes.size(); i++){
			int prod_yi=1;
			for (int j=0; j<d; j++){
				prod_yi*=y[j][i];
				prod_yi=prod_yi%primes[i];
			}
			log_msg(3)<<"prod y"<<i+1<<"="<<prod_yi;
			prody.push_back(prod_yi);
		}
		PII ret = chinese_remainder_theorem_int(primes,prody);
		sigma.push_back(ret.first);
		log_msg(2)<<"sigma_"<<sigma.size()<<"="<<ret.first;
		poly sseq;
		sseq.coef[0]=1;
		int elojel=-1;
		for(int i = 1; i <= d; i++) {
			sseq.coef[i]=sigma[i-1]*elojel;
			elojel*=-1;
		}
		vector<int> res;
		return find_root(sseq,d,res);
	}
};
#endif //CGT_CODEGEN_H
