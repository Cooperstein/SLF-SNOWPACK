/*
 *  SNOWPACK stand-alone
 *
 *  Copyright WSL Institute for Snow and Avalanche Research SLF, DAVOS, SWITZERLAND
*/
/*  This file is part of Snowpack.
    Snowpack is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Snowpack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Snowpack.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <snowpack/SnowpackConfig.h>

using namespace mio;
using namespace std;

/************************************************************
 * static section                                           *
 ************************************************************/
map<string,string> SnowpackConfig::advancedConfig;
map<string,string> SnowpackConfig::inputConfig;
map<string,string> SnowpackConfig::outputConfig;

const bool SnowpackConfig::__init = SnowpackConfig::initStaticData();

bool SnowpackConfig::initStaticData()
{
	//[SnowpackAdvanced] section
	advancedConfig["ALPINE3D"] = "false";
	advancedConfig["DOORSCHOT"] = "false";
	advancedConfig["DETECT_GRASS"] = "false";
	advancedConfig["FIXED_ALBEDO"] = "-999.";
	advancedConfig["FIXED_POSITIONS"] = "";
	advancedConfig["FORCE_RH_WATER"] = "true";
	advancedConfig["HARDNESS_MODEL"] = "MONTI";
	advancedConfig["HEIGHT_NEW_ELEM"] = "0.02";
	advancedConfig["HN_DENSITY"] = "PARAMETERIZED";
	advancedConfig["HN_DENSITY_MODEL"] = "LEHNING_NEW";
	advancedConfig["HOAR_DENSITY_BURIED"] = "125.";
	advancedConfig["HOAR_DENSITY_SURF"] = "100.";
	advancedConfig["HOAR_MIN_SIZE_BURIED"] = "2.";
	advancedConfig["HOAR_MIN_SIZE_SURF"] = "0.5";
	advancedConfig["HOAR_THRESH_RH"] = "0.97";
	advancedConfig["HOAR_THRESH_VW"] = "3.5";
	advancedConfig["JAM"] = "false";
	advancedConfig["COMBINE_ELEMENTS"] = "true";
	advancedConfig["MASS_BALANCE"] = "false";
	advancedConfig["MAX_NUMBER_MEAS_TEMPERATURES"] = "5";
	advancedConfig["METAMORPHISM_MODEL"] = "DEFAULT";
	advancedConfig["MIN_DEPTH_SUBSURF"] = "0.07";
	advancedConfig["MULTISTREAM"] = "true";
	advancedConfig["NEW_SNOW_GRAIN_RAD"] = "0.15";
	advancedConfig["NUMBER_FIXED_RATES"] = "0";
	advancedConfig["PERP_TO_SLOPE"] = "false";
	advancedConfig["PLASTIC"] = "false";
	advancedConfig["PREVAILING_WIND_DIR"] = "0.";
	advancedConfig["RESEARCH"] = "true";
	advancedConfig["STATION_NAME"] = "station"; //TODO: we should suppress this (not needed anymore)
	advancedConfig["STRENGTH_MODEL"] = "DEFAULT";
	advancedConfig["SW_MODE_CHANGE"] = "false";
	advancedConfig["THRESH_RAIN"] = "1.2";
	advancedConfig["THRESH_RH"] = "0.5";
	advancedConfig["THRESH_DT_AIR_SNOW"] = "3.0";
	advancedConfig["T_CRAZY_MAX"] = "340.";
	advancedConfig["T_CRAZY_MIN"] = "210.";
	advancedConfig["VARIANT"] = "DEFAULT";
	advancedConfig["VISCOSITY_MODEL"] = "DEFAULT";
	advancedConfig["WATER_LAYER"] = "false";
	advancedConfig["WATERTRANSPORTMODEL_SNOW"]="BUCKET";
	advancedConfig["WATERTRANSPORTMODEL_SOIL"]="BUCKET";
	advancedConfig["WIND_SCALING_FACTOR"] = "1.0";

	//[Input] section
	inputConfig["METEOPATH"] = "./DATA/input";
	inputConfig["NUMBER_OF_SOLUTES"] = "0";
	inputConfig["SNOW"] = "SMET";
	inputConfig["SOLUTE_NAMES"] = "NITRATE";

	//[Output] section
	outputConfig["AVGSUM_TIME_SERIES"] = "true";
	outputConfig["BACKUP_DAYS_BETWEEN"] = "365.";
	outputConfig["CUMSUM_MASS"] = "false";
	outputConfig["EXPERIMENT"] = "NO_EXP";
	outputConfig["FIRST_BACKUP"] = "400.";
	outputConfig["METEOPATH"] = "./DATA";
	outputConfig["OUT_CANOPY"] = "false";
	outputConfig["OUT_HAZ"] = "true";
	outputConfig["OUT_HEAT"] = "true";
	outputConfig["OUT_LOAD"] = "false";
	outputConfig["OUT_LW"] = "true";
	outputConfig["OUT_MASS"] = "true";
	outputConfig["OUT_METEO"] = "true";
	outputConfig["OUT_STAB"] = "true";
	outputConfig["OUT_SW"] = "true";
	outputConfig["OUT_T"] = "true";
	outputConfig["PRECIP_RATES"] = "true";
	outputConfig["HARDNESS_IN_NEWTON"] = "true";
	outputConfig["CLASSIFY_PROFILE"] = "false";
	outputConfig["SNOW"] = "SMET";

	return true;
}

/************************************************************
 * non-static section                                       *
 ************************************************************/

SnowpackConfig::~SnowpackConfig() {}

SnowpackConfig::SnowpackConfig(const mio::Config& i_cfg) : Config(i_cfg),
                               enforce_measured_snow_heights(false)
{
	setDefaults();
}

SnowpackConfig::SnowpackConfig(const std::string& i_filename) : Config(i_filename),
                               enforce_measured_snow_heights(false)
{
	setDefaults();
}

void SnowpackConfig::setDefaults()
{
	string variant; getValue("VARIANT", "SnowpackAdvanced", variant, Config::nothrow);

	getValue("ENFORCE_MEASURED_SNOW_HEIGHTS", "Snowpack", enforce_measured_snow_heights);

	addKey("MINIMUM_L_ELEMENT", "SnowpackAdvanced", "0.0025"); //Minimum element length (m)
	double minimum_l_element = get("MINIMUM_L_ELEMENT", "SnowpackAdvanced");
	if (enforce_measured_snow_heights) {
		addKey("HEIGHT_NEW_ELEM", "SnowpackAdvanced", "0.02");
	} else {
		stringstream ss;
		double tmp = 2.0 * minimum_l_element;
		ss << tmp;
		addKey("HEIGHT_NEW_ELEM", "SnowpackAdvanced", ss.str());
	}

	string hn_density;  getValue("HN_DENSITY", "SnowpackAdvanced", hn_density, Config::nothrow);
	string hn_density_model; getValue("HN_DENSITY_MODEL", "SnowpackAdvanced", hn_density_model, Config::nothrow);
	string metamorphism_model; getValue("METAMORPHISM_MODEL", "SnowpackAdvanced", metamorphism_model, Config::nothrow);
	string strength_model; getValue("STRENGTH_MODEL", "SnowpackAdvanced", strength_model, Config::nothrow);
	string viscosity_model; getValue("VISCOSITY_MODEL", "SnowpackAdvanced", viscosity_model, Config::nothrow);
	string watertransportmodel_snow; getValue("WATERTRANSPORTMODEL_SNOW", "SnowpackAdvanced", watertransportmodel_snow, Config::nothrow);
	string watertransportmodel_soil; getValue("WATERTRANSPORTMODEL_SOIL", "SnowpackAdvanced", watertransportmodel_soil, Config::nothrow);


	if ((variant == "") || (variant == "DEFAULT")) {

		// Use default settings and ...

	} else if (variant == "JAPAN") {

		if (metamorphism_model == "") addKey("METAMORPHISM_MODEL", "SnowpackAdvanced", "NIED");
		if (strength_model == "") addKey("STRENGTH_MODEL", "SnowpackAdvanced", "NIED");
		if (viscosity_model == "") addKey("VISCOSITY_MODEL", "SnowpackAdvanced", "KOJIMA");
		if (watertransportmodel_snow == "") addKey("WATERTRANSPORTMODEL_SNOW", "SnowpackAdvanced", "NIED");
		if (watertransportmodel_soil == "") addKey("WATERTRANSPORTMODEL_SOIL", "SnowpackAdvanced", "NIED");

	} else if (variant == "ANTARCTICA") {

		if (hn_density == "") addKey("HN_DENSITY", "SnowpackAdvanced", "EVENT");

		addKey("MINIMUM_L_ELEMENT", "SnowpackAdvanced", "0.0001"); //Minimum element length (m)
		minimum_l_element = get("MINIMUM_L_ELEMENT", "SnowpackAdvanced");

		string hoar_density_buried; getValue("HOAR_DENSITY_BURIED", "SnowpackAdvanced",
		                                     hoar_density_buried, Config::nothrow);
		if (hoar_density_buried == "") addKey("HOAR_DENSITY_BURIED", "SnowpackAdvanced", "200.0");

		string force_rh_water; getValue("FORCE_RH_WATER", "SnowpackAdvanced", force_rh_water, Config::nothrow);
		if (force_rh_water == "") addKey("FORCE_RH_WATER", "SnowpackAdvanced", "false");

		string thresh_rh; getValue("THRESH_RH", "SnowpackAdvanced", thresh_rh, Config::nothrow);
		if (thresh_rh == "") addKey("THRESH_RH", "SnowpackAdvanced", "0.7");

		if ( !enforce_measured_snow_heights) {
			stringstream ss;
			double tmp = 1.1 * minimum_l_element;
			ss << tmp;
			addKey("HEIGHT_NEW_ELEM", "SnowpackAdvanced", ss.str());
		}

		addKey("FIRST_BACKUP", "Output", "1500.");
		addKey("FIXED_POSITIONS", "SnowpackAdvanced", "7");
		addKey("FIXED_RATES", "SnowpackAdvanced", "false");
		addKey("NUMBER_FIXED_RATES", "SnowpackAdvanced", "0");
		addKey("MAX_NUMBER_MEAS_TEMPERATURES", "SnowpackAdvanced", "7");
		addKey("MIN_DEPTH_SUBSURF", "SnowpackAdvanced", "0.");
		addKey("T_CRAZY_MIN", "SnowpackAdvanced", "165.");
		addKey("T_CRAZY_MAX", "SnowpackAdvanced", "300.");
		addKey("NEW_SNOW_GRAIN_RAD", "SnowpackAdvanced", "0.1");

	} else if (variant == "CALIBRATION") {

		if (hn_density_model == "") addKey("HN_DENSITY_MODEL", "SnowpackAdvanced", "ZWART");
		if (viscosity_model == "") addKey("VISCOSITY_MODEL", "SnowpackAdvanced", "CALIBRATION");

		string fixed_positions; getValue("FIXED_POSITIONS", "SnowpackAdvanced", fixed_positions, Config::nothrow);
		if (fixed_positions == "") addKey("FIXED_POSITIONS", "SnowpackAdvanced", "5");
		string number_fixed_rates; getValue("NUMBER_FIXED_RATES", "SnowpackAdvanced", number_fixed_rates, Config::nothrow);
		if (number_fixed_rates == "") addKey("NUMBER_FIXED_RATES", "SnowpackAdvanced", "0");
		string max_number_meas_temperatures;
		getValue("MAX_NUMBER_MEAS_TEMPERATURES", "SnowpackAdvanced", max_number_meas_temperatures, Config::nothrow);
		if (max_number_meas_temperatures == "") addKey("MAX_NUMBER_MEAS_TEMPERATURES", "SnowpackAdvanced", "5");
		string min_depth_subsurf; getValue("MIN_DEPTH_SUBSURF", "SnowpackAdvanced", min_depth_subsurf, Config::nothrow);
		if (min_depth_subsurf == "") addKey("MIN_DEPTH_SUBSURF", "SnowpackAdvanced", "0.0");

	} else {
		throw UnknownValueException("Unknown variant " + variant, AT);
	}

	/* For all parameters not set by the user or by the initialization above, the default values apply
	 * That is, loop through advancedConfig (then inputConfig & outputConfig) and check whether user has set
	 * the parameter in the corresponding section, if not add default value
	 */
	for(map<string,string>::const_iterator it = advancedConfig.begin(); it != advancedConfig.end(); ++it){
		//[SnowpackAdvanced] section
		string value; getValue(it->first, "SnowpackAdvanced", value, Config::nothrow);
		if (value == "") addKey(it->first, "SnowpackAdvanced", it->second);
	}

	for(map<string,string>::const_iterator it = inputConfig.begin(); it != inputConfig.end(); ++it){
		//[Input] section
		string value; getValue(it->first, "Input", value, Config::nothrow);
		if (value == "") addKey(it->first, "Input", it->second);
	}

	for(map<string,string>::const_iterator it = outputConfig.begin(); it != outputConfig.end(); ++it){
		//[Output] section
		string value; getValue(it->first, "Output", value, Config::nothrow);
		if (value == "") addKey(it->first, "Output", it->second);
	}

	/**
	 * @brief Defines how energy and mass balance are output \n
	 * - AVGSUM_TIME_SERIES == 1 \n
	 *   Energy and mass fluxes are averaged and cumulated over TS_DAYS_BETWEEN, respectively. \n
	 *   Otherwise, instantaneous energy fluxes and mass fluxes cumulated over the last computation
	 *   time step (CALCULATION_STEP_LENGTH) are dumped. \n
	 *   @note Precipitations and Erosion are always given in rates per TS_DAYS_BETWEEN interval (h-1)
	 * - CUMSUM_MASS == 1 \n
	 *   Mass fluxes are cumulated over whole run period.
	 * - WARNING: In operational mode and if NUMBER_SLOPES > 1, the above two values are always unset!
	 */
	unsigned int nSlopes = get("NUMBER_SLOPES", "Snowpack");
	if (nSlopes > 1) {
		addKey("AVGSUM_TIME_SERIES", "Output", "false");
		addKey("CUMSUM_MASS", "Output", "false");
	}

	/**
	 * @brief Hazard data interval in units of CALCULATION_STEP_LENGTH \n
	 * WARNING: In operational mode, this has to result in a 30 min interval!
	 * It is a matter of consitency. If you change this, a big mess will result!!!
	 */
	double calculation_step_length = get("CALCULATION_STEP_LENGTH", "Snowpack");

	string hazard_steps_between;
	getValue("HAZARD_STEPS_BETWEEN", "Output", hazard_steps_between, Config::nothrow);
	if (hazard_steps_between == "") {
		stringstream ss;
		int tmp = (int)(30./calculation_step_length + 0.5);
		ss << tmp;
		addKey("HAZARD_STEPS_BETWEEN", "Output", ss.str());
	}
}
