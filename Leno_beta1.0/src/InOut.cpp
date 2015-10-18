/*
 * InOut.cpp
 *
 *  Created on: Oct 15, 2015
 *      Author: oscar
 */

#include "InOut.h"

InOut::InOut() {
}

InOut::~InOut() {
	// TODO Auto-generated destructor stub
}

void InOut::readDevice(std::string filename) {
	// open file, specify the comments are within /* */
	GetPot ifile(filename.c_str(), "/*", "*/");
	std::cout << "Device File is " << filename.c_str() << std::endl;
	//Device: general
	ifile.set_prefix("Device1D/"); // enter [Device1D] section
	devName = ifile("Device_Name","No name specified");
	std::cout << "Device Name is " << devName << std::endl;
	userCom = ifile("User_Comment", "No Comment");
	std::cout << "Comments: " << userCom << std::endl;
	layerNum = ifile("Layer_Number", 0);
	std::cout << "Layer Num is " << layerNum << std::endl;

	// Boundary
	ifile.set_prefix("Device1D/Boundary/");

	if (ifile("Top_Boundary_Type", "") == "Dirichlet" )
		topBT = Dirichlet;
	else if (ifile("Top_Boundary_Type", "") == "Neumann")
		topBT = Neumann;
	else
		std::cerr << "Error: top boundary type undefined." << std::endl;

	if (ifile("Bottom_Boundary_Type", "") == "Dirichlet" )
		botBT = Dirichlet;
	else if (ifile("Bottom_Boundary_Type", "") == "Neumann")
		botBT = Neumann;
	else
		std::cerr << "Error: bottom boundary type undefined." << std::endl;

	// Workfunction
	botWF = ifile("Bottom_Gate_Workfunction", 0.0);
	topWF = ifile("Top_Gate_Workfunction", 0.0);

	// Layers
	for (int i = 1; i <= layerNum; i++) {
		std::string str = "Layer"+std::to_string(i)+"/";
		ifile.set_prefix(str.c_str());

		layerName.push_back(ifile("Material", ""));
		layerThickness.push_back(ifile("Thickness",0.0));
		layerPoint.push_back(ifile("Ny",0));
	}

	// Bias
	ifile.set_prefix("Vtg/");
	vtgArray[0] = ifile("Vtg_From", 0.0);
	vtgArray[1] = ifile("Vtg_To", 0.0);
	vtgArray[2] = ifile("Vtg_Step", 0.0);
	ifile.set_prefix("Vbg/");
	vbgArray[0] = ifile("Vbg_From", 0.0);
	vbgArray[1] = ifile("Vbg_To", 0.0);
	vbgArray[2] = ifile("Vbg_Step", 0.0);
	ifile.set_prefix("Vds/");
	vdsArray[0] = ifile("Vds_From", 0.0);
	vdsArray[1] = ifile("Vds_To", 0.0);
	vdsArray[2] = ifile("Vds_Step", 0.0);

	// Output
	ifile.set_prefix("Plot/");
	if (ifile("Plot_Band_Alignment", "") == "Yes")
		plotBA = true;
	else if (ifile("Plot_Band_Alignment", "") == "No")
			plotBA = false;

	if (ifile("Plot_I-V", "") == "Yes")
		plotIV = true;
	else if (ifile("Plot_I-V", "") == "No")
			plotIV = false;

	if (ifile("Save_Plots", "") == "Yes")
		savePl = true;
	else if (ifile("Sava_Plots", "") == "No")
			savePl = false;

	ifile.set_prefix("Plot/VRange/");
    vtgPl[0]=(ifile("Vtg", 0.0, 0));
    vtgPl[1]=(ifile("Vtg", 0.0, 1));

    vbgPl[0]=(ifile("Vbg", 0.0, 0));
    vbgPl[1]=(ifile("Vbg", 0.0, 1));

    vdsPl[0]=(ifile("Vds", 0.0, 0));
    vdsPl[1]=(ifile("Vds", 0.0, 1));


	ifile.set_prefix("Save/");
	if (ifile("Save_Band_Alignment ", "") == "Yes")
		saveBA = true;
	else if (ifile("Plot_Band_Alignment", "") == "No")
			saveBA = false;

}


void InOut::readMaterial(std::string filename) {
	// open file, specify the comments are within /* */
	GetPot ifile(filename.c_str(), "/*", "*/");
	std::cout << "Material File is " << filename.c_str() << std::endl;
	// get the materials device structure required
	for (int i = 0; i < layerName.size(); i++) {
		std::vector<double> v;
		std::string str = layerName[i] + "/";
		std::cout << "Getting " << layerName[i] << std::endl;
		ifile.set_prefix(str.c_str());

		if (ifile("Type","") == "Dielectric")
			v.push_back(Dielectric);
		else if (ifile("Type","") == "Semiconductor")
			v.push_back(Semiconductor);
		else
			std::cerr << "Error: Wrong type" << std::endl;

		v.push_back(ifile("er_y",0.0));
		v.push_back(ifile("er_x",0.0));
		v.push_back(ifile("Electron_Affinity",0.0));
		v.push_back(ifile("Bandgap",0.0));
		v.push_back(ifile("Dimension",0.0));
		v.push_back(ifile("Monolayer_Thickness",0.0));
		v.push_back(ifile("Nd",0.0));
		v.push_back(ifile("Na",0.0));
		v.push_back(ifile("mc_eff",0.0));
		v.push_back(ifile("mv_eff",0.0));
		v.push_back(ifile("gv_of_Ec",0.0));
		v.push_back(ifile("gv_of_Ev",0.0));

		matMap.insert(std::pair<std::string, std::vector<double>>(layerName[i], v));
		// Map will eliminate duplication automatically

	}

	for (std::map<std::string, std::vector<double>>::iterator it = matMap.begin();
			it  != matMap.end(); ++it) {
		std::cout << "Material: " << it->first.c_str() << " is added." << std::endl;
	}

}
