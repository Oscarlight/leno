/*
 * GateEfficiency.cpp
 *
 *  Created on: Dec 14, 2015
 *      Author: oscar
 */

#include "GateEfficiency.h"

GateEfficiency::GateEfficiency() {
	// TODO Auto-generated constructor stub

}

GateEfficiency::~GateEfficiency() {
	// TODO Auto-generated destructor stub
}


std::map<std::vector<double>, std::vector<double>> GateEfficiency::getGateEffMap() {
	return ( gateEffMap );
}

// TODO: generalize it, now design only for pin/thin tfet
void GateEfficiency::calGateEfficiency( double Vbg, double Vds, double VtgStep,
		int vBxThin, int cBxThin, int vBxPin, int cBxPin,
		std::vector<std::vector<std::vector<double> > > band2DPerVtg) {

	// is Thin or pin?
	bool isThin = true;
	/* true: is ThinTFET, false: is pinTFET
	 * in ThinTFET, the source and drain are in the different layers,
	 * so the size of bandPerLayer (which is a element in band2DPerVtg)
	 * is 2.
	*/
	if (band2DPerVtg[0].size() == 2) {
		isThin = false;
	}

	std::vector<double> tunnelWindows;
	// TODO: need to add for p-type
	for (int i = 0; i < band2DPerVtg.size(); i++) {  // for each Vtg
		if (isThin) {
		// for n-Thin
		// valence band in block 2 in bottom layer - conduction band in block 2 in top layer, < 0 no tunnel, > 0 there is tunnelwindow
		tunnelWindows.push_back(band2DPerVtg[i][3][vBxThin] - band2DPerVtg[i][0][cBxThin]);
		} else {
		// for n-pin
		// valence band in block 1 - conduction band in block 2, < 0 no tunnel, > 0 there is tunnelwindow
		tunnelWindows.push_back(band2DPerVtg[i][1][vBxPin] - band2DPerVtg[i][0][cBxPin]);
		}
	}

	// get first, last, sloop
	std::vector<double> triple;
	triple.push_back(tunnelWindows.front());
	triple.push_back(tunnelWindows.back());
	// find the two points closest to 0, one is negative, another is positive
	// we know it most likly in the middel
	double neg, pos;
	double j = tunnelWindows.size() - 1;
	double i = 0;
	while ( j  >=  i) {
		if (tunnelWindows[i] < 0) {
			neg = tunnelWindows[i];
			i++;
		}
		if (tunnelWindows[j] > 0) {
			pos = tunnelWindows[j];
			j--;
		}
	}
	triple.push_back((pos - neg)/VtgStep);

	std::vector<double> bias; bias.push_back(Vbg); bias.push_back(Vds);
	gateEffMap.insert(std::pair<std::vector<double>, std::vector<double>>(bias, triple));

	std::cout << "***** GateEfficiency map is generated. " << std::endl;
}
