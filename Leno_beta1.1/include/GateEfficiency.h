/*
 * GateEfficiency.h
 *
 *  Created on: Dec 14, 2015
 *      Author: oscar
 */

#ifndef INCLUDE_GATEEFFICIENCY_H_
#define INCLUDE_GATEEFFICIENCY_H_

#include "Params.h"
#include <map>

class GateEfficiency : public Params{

private:
	std::map<std::vector<double>, std::vector<double>> gateEffMap;

public:
	GateEfficiency();
	virtual ~GateEfficiency();

	std::map<std::vector<double>, std::vector<double>> getGateEffMap();

	void calGateEfficiency(double Vbg, double Vds, double VtgStep,
			int vBxThin, int cBxThin, int vBxPin, int cBxPin,
			std::vector< std::vector< std::vector<double> > > band2DPerVtg);
};

#endif /* INCLUDE_GATEEFFICIENCY_H_ */
