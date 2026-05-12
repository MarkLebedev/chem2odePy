#ifndef _chemistry_hpp_
#define _chemistry_hpp_
/*******************************************************************/
#define LIMITER		1.0e-5	// 10 percent from electron

void chemistry(	/* Time interval in seconds */
				double time_interval,
				/* Initial concentration in cm^{-3} */
				double oldnH,		// n_{H}
				double oldnHm,		// n_{H-}
				double oldnHp,		// n_{H+}
				double oldnH2,		// n_{H2}
				double oldnH2p,		// n_{H2+}
				double oldnHe,		// n_{He}
				double oldnHep,		// n_{He+}
				double oldnHepp,	// n_{He++} 
				double oldne,		// n_{e}
				/* Final concentration in cm^{-3} */
				double &nH, 
				double &nHm, 
				double &nHp, 
				double &nH2, 
				double &nH2p, 
				double &nHe, 
				double &nHep, 
				double &nHepp, 
				double &ne,
				/* Temparature in Kelvins */
				double T, 
				/* Rate coefficient in in cm^{-3} s^{-1} */
				double k1,      // (1) H + e -> H^{+} + 2e -- collision ionisation                         
				double k2,      // (2) H^{+} + e -> H + gamma -- collision radiative recombination        
				double k3,      // (3) H + e -> H^{-} + gamma -- collision radiative ionisation           
				double k4,      // (4) H^{-} + H -> H_{2} + e -- associative electron detachment          
				double k5,      // (5) H + H^{+} -> H_{2}^{+} + gamma -- radiative association            
				double k6,      // (6) H_{2}^{+} + H -> H_{2} + H^{+} -- hydrogen recombination           
				double k7,      // (7) H_{2} + H^{+} -> H_{2}^{+} + H -- molecular hydrogen recombination 
				double k8,      // (8) H_{2} + e -> 2H + e -- collision dissociation                      
				double k9,      // (9) H^{-} + e -> H + 2e -- first collision electron detachment         
				double k10,     // (10) H^{-} + H -> 2H + e -- second collision electron detachment       
				double k11,     // (11) H^{-} + H^{+} -> 2H + gamma -- neutralize                         
				double k12,     // (12) H^{-} + H^{+} -> H_{2}^{+} + e -- collision association           
				double k13,     // (13) H_{2}^{+} + e -> 2H + gamma -- dissociative recombination         
				double k14,     // (14) H_{2}^{+} + H^{-} -> H + H_{2} -- molecular neutralize            
				double k15,     // (15) 3H -> H_{2} + H -- collision molecular association                
				double k16,     // (16) H_{2} + H -> 3H -- collision molecular dissociation               
				double k17,     // (17) He + e -> He^{+} + 2e -- collision ionisation                     
				double k18,     // (18) He^{+} + e -> He + gamma -- collision radiative recombination     
				double k19,     // (19) He^{+} + e -> He^{++} + 2e -- collision ionisation                
				double k20)     // (20) He^{++} + e -> He^{+} + gamma -- collision radiative recombination
{
	double flnH, flnHm, flnHp, flnH2, flnH2p, flnHe, flnHep, flnHepp, flne;
	double fdnH, fdnHm, fdnHp, fdnH2, fdnH2p, fdnHe, fdnHep, fdnHepp, fdne;
	double fluxnH, fluxnHm, fluxnHp, fluxnH2, fluxnH2p, fluxnHe, fluxnHep, fluxnHepp, fluxne;
	
	/* C - Dn */
	flnH =	- k1 * oldnH * oldne 
			+ k2 * oldnHp * oldne
			- k7 * oldnH * oldne 
			- k8 * oldnHm * oldnH
			- k9 * oldnH * oldnHp
			- k10 * oldnH2p * oldnH
			+ k11 * oldnH2 * oldnHp
			+ k12 * 2.0 * oldnH2 * oldne
			+ k13 * oldnHm * oldne
			- k14 * oldnHm * oldnH
			+ k14 * 2.0 * oldnHm * oldnH
			+ k15 * 2.0 * oldnHm * oldnHp
			+ k17 * 2.0 * oldnH2p * oldne
			+ k18 * oldnH2p * oldnHm
			- k19 * 3.0 * oldnH * oldnH * oldnH
			+ k19 * oldnH * oldnH * oldnH
			- k20 * oldnH2 * oldnH
			+ k20 * 3.0 * oldnH2 * oldnH;

	flne =	- k1 * oldnH * oldne
			+ k1 * 2.0 *  oldnH * oldne
			- k2 * oldnHp * oldne
			- k3 * oldnHe * oldne
			+ k3 * 2.0 *  oldnHe * oldne
			- k4 * oldnHep * oldne
			- k5 * oldnHep * oldne
			+ k5 * 2.0 *  oldnHep * oldne
			- k6 * oldnHepp * oldne
			- k7 * oldnH * oldne
			+ k8 * oldnHm * oldnH
			- k12 * oldnH2 * oldne
			+ k12 * oldnH2 * oldne
			- k13 * oldnHm * oldne
			+ k13 * 2.0 *  oldnHm * oldne
			+ k14 * oldnHm * oldnH
			+ k16 * oldnHm * oldnHp
			- k17 * oldnH2p * oldne;

	flnHp =	  k1 * oldnH * oldne
			- k2 * oldnHp * oldne
			- k9 * oldnH * oldnHp
			+ k10 * oldnH2p * oldnH
			- k11 * oldnH2 * oldnHp
			- k15 * oldnHm * oldnHp
			- k16 * oldnHm * oldnHp;

	flnHe = - k3 * oldnHe * oldne
			+ k4 * oldnHep * oldne;

	flnHep =  k3 * oldnHe * oldne 
			- k4 * oldnHep * oldne 
			- k5 * oldnHep * oldne
			+ k6 * oldnHepp * oldne;

	flnHepp = k5 * oldnHep * oldne
			- k6 * oldnHepp * oldne;

	flnHm =   k7 * oldnH * oldne
			- k8 * oldnHm * oldnH
			- k13 * oldnHm * oldne
			- k14 * oldnHm * oldnH
			- k15 * oldnHm * oldnHp
			- k16 * oldnHm * oldnHp
			- k18 * oldnH2p * oldnHm;

	flnH2 =   k8 * oldnHm * oldnH
			+ k10 * oldnH2p * oldnH
			- k11 * oldnH2 * oldnHp
			- k12 * oldnH2 * oldne
			+ k18 * oldnH2p * oldnHm
			+ k19 * oldnH * oldnH * oldnH
			- k20 * oldnH2 * oldnH;

	flnH2p =  k9 * oldnH * oldnHp
			- k10 * oldnH2p * oldnH
			+ k11 * oldnH2 * oldnHp
			+ k16 * oldnHm * oldnHp
			- k17 * oldnH2p * oldne
			- k18 * oldnH2p * oldnHm;

	/* D */
	fdnH =	k1 * oldne +
			k7 * oldne +
			k8 * oldnHm +
			k9 * oldnHp +
			k10 * oldnH2p +
			k14 * oldnHm - k14 * 2.0 * oldnHm +
			k19 * 3.0 * oldnH * oldnH - k19 * oldnH * oldnH +
			k20 * oldnH2 - k20 * 3.0 * oldnH2;

	fdne =	k1 * oldnH - k1 * 2.0 *  oldnH +
			k2 * oldnHp +
			k3 * oldnHe - k3 * 2.0 *  oldnHe +
			k4 * oldnHep +
			k5 * oldnHep - k5 * 2.0 *  oldnHep +
			k6 * oldnHepp +
			k7 * oldnH +
			k13 * oldnHm - k13 * 2.0 *  oldnHm +
			k17 * oldnH2p;

	fdnHp =	k2 * oldne +
			k9 * oldnH +
			k11 * oldnH2 + 
			k15 * oldnHm +
			k16 * oldnHm;

	fdnHe = k3 * oldne;

	fdnHep = k4 * oldne +
			 k5 * oldne;
			
	fdnHepp = k6 * oldne;

	fdnHm =  k8 * oldnH +
			 k13 * oldne +
			 k14 * oldnH +
			 k15 * oldnHp +
			 k16 * oldnHp +
			 k18 * oldnHm;

	fdnH2 =  k11 * oldnHp +
			 k12 * oldne +
			 k20 * oldnH;

	fdnH2p = k10 * oldnH +
			 k17 * oldne +
			 k18 * oldnHm;

	/* flux computing */
	fluxnH     = LIMITER * time_interval * flnH    / (1.0 + time_interval * fdnH);
	fluxnHm    = LIMITER * time_interval * flnHm   / (1.0 + time_interval * fdnHm);
	fluxnHp    = LIMITER * time_interval * flnHp   / (1.0 + time_interval * fdnHp);
	fluxnH2    = LIMITER * time_interval * flnH2   / (1.0 + time_interval * fdnH2);
	fluxnH2p   = LIMITER * time_interval * flnH2p  / (1.0 + time_interval * fdnH2p);
	fluxnHe    = LIMITER * time_interval * flnHe   / (1.0 + time_interval * fdnHe);
	fluxnHep   = LIMITER * time_interval * flnHep  / (1.0 + time_interval * fdnHep);
	fluxnHepp  = LIMITER * time_interval * flnHepp / (1.0 + time_interval * fdnHepp);
	fluxne     = LIMITER * time_interval * flne    / (1.0 + time_interval * fdne);

	nH = oldnH + fluxnH;  
	nHm = oldnHm + fluxnHm; 
	nHp = oldnHp + fluxnHp; 
	nH2 = oldnH2 + fluxnH2; 
	nH2p = oldnH2p + fluxnH2p;
	nHe = oldnHe + fluxnHe; 
	nHep = oldnHep + fluxnHep;
	nHepp = oldnHepp + fluxnHepp;
	ne = oldne + fluxne; 
}

/*******************************************************************/
#endif