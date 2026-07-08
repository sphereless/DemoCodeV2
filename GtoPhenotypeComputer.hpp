//
//  GtoPhenotypeComputer.hpp
//  Application
//
//  Created by Ashish Aring on 26.02.23.
//  Copyright © 2023 Sphereless. All rights reserved.
//

#ifndef GtoPhenotypeComputer_hpp
#define GtoPhenotypeComputer_hpp

#include "AbstractPhenotypeComputer.hpp"

/**
 *	Computes a set of maneuvers from a vector-input (integer)
 *
 *  The genotype encodes each Maneuver in 30 integer-values. This contains the
 *  julian-date (in 7+10 digits), the duration (6) , the right-ascension (4) and the declination (3).
 *
 *  Optimisers typically encode only the last N last digits of a julian DAY(!), not its full length M = 7.
 *  The amount of N digits must here be specified as argument. The unencoded part of the
 *  julian DAY gets prepended by this phenotype-computer.
 *
 *  With the partially encoding, the vector-argument to be provided to public 'computePhenotype(..)'
 *  must be of reduced length L = 30-(M-N). This shortened vector gets internally converted into a
 *  full-length vector (L=30).
 *  The partial-length gets passed to the Base phenotype-constructor, storing it as genotype
 *  unit-length.
 *
 *	[						     1234 56 78 90 12 34 567]
 *	[17 for datetime		i.e. 2023-03-21T22:00:00.000]
 *	[	 or jul.date		i.e. 2,450,000.123 456 789 0]
 *	[ 6 for duration		i.e. 1200.89				]
 *	[ 4 for right-ascension i.e. 359.1					]
 *	[ 3 for declination		i.e. 89.9					]
 */
class ManvPhenotypeComputerInt30	: public AbstractPhenotypeComputer<int>
{
private:
	// static properties
	static const unsigned int	_sFullNumIntsPerManv;
    static const unsigned int   _sNumJdDecimals;
    static const unsigned int   _sNumDurationDigits;
    static const unsigned int   _sNumRaDigits;
    static const unsigned int   _sNumDeclDigits;
    static const unsigned int   _sMinGenoLength;
    static const double         _sJd2000;
    
	static const unsigned int	_sDoublesPerManv;
    	
	// properties
	std::vector<int>*			_pGeno;
    const unsigned int          _numIntsForJDays;
    const unsigned int          _lenTotalJdEncoding;
    const unsigned int          _index0Duration;
    const unsigned int          _index0DirectionA;
    const unsigned int          _index0DirectionB;

	// methods
	double						extractJulianDate			(unsigned int index0) const;
	double						extractDuration				(unsigned int index0) const;
	double						extractRightAscension		(unsigned int index0) const;
	double						extractDeclination			(unsigned int index0) const;
	
public:
	// constructor
								ManvPhenotypeComputerInt30	();
                                ManvPhenotypeComputerInt30  (unsigned int lenEncodingJd);
								~ManvPhenotypeComputerInt30	();
	
	// overridden methods
	void						computePhenotype			(const std::vector<int>& genotype);
	
};
#endif /* GtoPhenotypeComputer_hpp */
