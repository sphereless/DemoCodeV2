//
//  GtoPhenotypeComputer.cpp
//  Application
//
//  Created by Ashish Aring on 26.02.23.
//  Copyright © 2023 Sphereless. All rights reserved.
//

#include "GtoPhenotypeComputer.hpp"
#include <sstream>
#include "Enums.hpp"
#include "Constants.hpp"

// static properties

const unsigned int	ManvPhenotypeComputerInt30::_sFullNumIntsPerManv	= 30;
const unsigned int  ManvPhenotypeComputerInt30::_sNumJdDecimals         = 10;
const unsigned int  ManvPhenotypeComputerInt30::_sNumDurationDigits     = 6;
const unsigned int  ManvPhenotypeComputerInt30::_sNumRaDigits           = 4;
const unsigned int  ManvPhenotypeComputerInt30::_sNumDeclDigits         = 3;
const unsigned int  ManvPhenotypeComputerInt30::_sMinGenoLength         = _sNumJdDecimals
                                                                            + _sNumDurationDigits
                                                                            + _sNumRaDigits
                                                                            + _sNumDeclDigits;
const double        ManvPhenotypeComputerInt30::_sJd2000                = Constants::Times::JD2000;
const unsigned int	ManvPhenotypeComputerInt30::_sDoublesPerManv		= 4;


// constructor

ManvPhenotypeComputerInt30::ManvPhenotypeComputerInt30		()
						: ManvPhenotypeComputerInt30        (_sFullNumIntsPerManv-_sMinGenoLength)   {	/* empty */ }

ManvPhenotypeComputerInt30::ManvPhenotypeComputerInt30      (unsigned int lenEncodingJDays)
                        : AbstractPhenotypeComputer<int>    (typeid(this).name(),
                                                             _sMinGenoLength+lenEncodingJDays,
                                                             _sDoublesPerManv),
                            _pGeno                          (nullptr),
                            _numIntsForJDays                (lenEncodingJDays),
                            _lenTotalJdEncoding             (_numIntsForJDays+_sNumJdDecimals),
                            _index0Duration                 (_numIntsForJDays+_sNumJdDecimals),
                            _index0DirectionA               (_index0Duration+_sNumDurationDigits),
                            _index0DirectionB               (_index0DirectionA+_sNumRaDigits)
{
    /* empty */
}

ManvPhenotypeComputerInt30::~ManvPhenotypeComputerInt30		()
{
	_pGeno = nullptr;
}


// gets


// methods

double			ManvPhenotypeComputerInt30::extractJulianDate	(unsigned int index0)	const {
    // add encoded julian day
    std::ostringstream os;
    for (unsigned int i=0; i < _numIntsForJDays; i++)
        os << (*_pGeno)[index0+i];
    
    // update first index
    index0 += _numIntsForJDays;
	
    // fraction of day
	os  << "."
        << (*_pGeno)[index0] % 10
        << (*_pGeno)[index0+1] % 10
        << (*_pGeno)[index0+2] % 10
        << (*_pGeno)[index0+3] % 10
	    << (*_pGeno)[index0+4] % 10
        << (*_pGeno)[index0+5] % 10
        << (*_pGeno)[index0+6] % 10
        << (*_pGeno)[index0+7] % 10
		<< (*_pGeno)[index0+8] % 10
        << (*_pGeno)[index0+9] % 10;
    
	_logger->deep(_name, "Time: "+os.str());
	return ManvPhenotypeComputerInt30::_sJd2000+std::stod(os.str());
}

double			ManvPhenotypeComputerInt30::extractDuration 	(unsigned int index0)	const {
    std::ostringstream os;
    unsigned int index = index0+_index0Duration;
    
	os	<< (*_pGeno)[index]   % 10
        << (*_pGeno)[index+1] % 10
        << (*_pGeno)[index+2] % 10
        << (*_pGeno)[index+3] % 10
        << "."
	    << (*_pGeno)[index+4] % 10
        << (*_pGeno)[index+5] % 10;
	
	_logger->deep(_name, "Duration: "+os.str());
	return std::stod(os.str());
}

double			ManvPhenotypeComputerInt30::extractRightAscension(unsigned int index0) const {
	std::ostringstream os;
    unsigned int index = index0+_index0DirectionA;
    
	os	<< (*_pGeno)[index]   % 10
        << (*_pGeno)[index+1] % 10
        << (*_pGeno)[index+2] % 10;
    int confinedRA = std::stoi(os.str()) % 360;
    
    os.str("");
    os  << "."
        << (*_pGeno)[index+3] % 10;
	
	_logger->deep(_name, "RightAsc: "+std::to_string(confinedRA)+os.str());
	return confinedRA + std::stod(os.str());
}

double			ManvPhenotypeComputerInt30::extractDeclination  (unsigned int index0) const {
	std::ostringstream os;
    unsigned int index = index0+_index0DirectionB;
    
	os	<< (*_pGeno)[index]   % 10
        << (*_pGeno)[index+1] % 10;
    int confinedDec = std::stoi(os.str()) % 90;
    
    os.str("");
    os  << "."
        << (*_pGeno)[index+2] % 10;
	
	_logger->deep(_name, "Declination: "+std::to_string(confinedDec)+os.str());
	return confinedDec + std::stod(os.str());
}

void			ManvPhenotypeComputerInt30::computePhenotype	(const std::vector<int> &genotype) {
	
	if (genotype.size() % _unitLenGeno != 0) {
		std::ostringstream os;
		os << "Incompatible arg-length " << genotype.size() << ", expect multiples of " << _unitLenGeno;
		this->_logger->error(_name, os.str());
		
		_pGeno = nullptr;
		throw std::invalid_argument(os.str());
	}
	
	unsigned int numManvs = (unsigned int)genotype.size()/_unitLenGeno;
	unsigned int reqVecSize = numManvs*_sDoublesPerManv;
	_pGeno = &(std::vector<int>&) genotype;
	
	this->adjustPhenoVector(reqVecSize);
	
	// extract maneuvers
	unsigned int index0Geno;
	unsigned int index0Pheno;
	for (unsigned int i=0; i < numManvs; i++) {
		index0Geno  = i*_unitLenGeno;
		index0Pheno = i*_unitLenPheno;
		_phenotype[index0Pheno]   = this->extractJulianDate(index0Geno);
		_phenotype[index0Pheno+1] = this->extractDuration(index0Geno);
		_phenotype[index0Pheno+2] = this->extractRightAscension(index0Geno);
		_phenotype[index0Pheno+3] = this->extractDeclination(index0Geno);
	}
}

