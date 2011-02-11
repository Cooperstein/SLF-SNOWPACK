/*
 *  SNOWPACK VERSION 9.x
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
/**
 * @file Hazard.c
 * @version 10.02
 * This module contains the hazard computation routines
*/

#include <snowpack/Hazard.h>

using namespace mio;
using namespace std;

/************************************************************
 * static section                                           *
 ************************************************************/

//Lee slope length (m) used to convert mass flux to drift index deposition depth (cm/24h)
const double Hazard::typical_slope_length = 70.0;

//Predefined snow density (kg m-3) used to convert mass flux to drift index deposition depth (cm/24h)
const double Hazard::wind_slab_density = 77.0;

//At least that mass flux (kg m-1 h-1) must have summed up for the drift index to be larger than 0.
const double Hazard::minimum_drift = 0.0;

//Largest additional accumulation allowed (cm h-1)
const double Hazard::maximum_drift = 5.0;

//Selects whether hand hardness index R is output either in N (Swiss scale) or steps
const bool Hazard::r_in_n = true;

/************************************************************
 * non-static section                                       *
 ************************************************************/

Hazard::Hazard(const mio::Config& i_cfg) : cfg(i_cfg)
{
	/**
	 * @brief Defines how the height of snow is going to be handled
	 * - 0: Depth of snowfall is determined from the water equivalent of snowfall (HNW)
	 * - 1: The measured height of snow is used to determine whether new snow has been deposited.
	 *      This setting MUST be chosen in operational mode. \n
	 *      This procedure has the disadvantage that if the snowpack settles too strongly
	 *      extra mass is added to the snowpack. \n
	 * New snow density is needed in both cases, either parameterized, measured, or fixed.
	 */
	cfg.getValue("ENFORCE_MEASURED_SNOW_HEIGHTS", "Parameters", enforce_measured_snow_heights);

	//Calculation time step in seconds as derived from CALCULATION_STEP_LENGTH
	double calculation_step_length = cfg.get("CALCULATION_STEP_LENGTH", "Parameters");
	sn_dt = M_TO_S(calculation_step_length);

	/*
	 * Hazard data interval in units of CALCULATION_STEP_LENGTH \n
	 * WARNING: In operational mode, this has to result in a 30 min interval!
	 * It is a matter of consitency. If you change this, a big mess will result!!!
	 */
	cfg.getValue("HAZARD_STEPS_BETWEEN", "Parameters", hazard_steps_between);

	/* Dew point relative to water or ice
	 * - default: 1
	 * - Antarctica: 0 */
	cfg.getValue("FORCE_RH_WATER", "Parameters", force_rh_water);

	cfg.getValue("RESEARCH", "Parameters", research_mode);

	//Density of surface hoar (-> hoar index of surface node) (kg m-3)
	cfg.getValue("HOAR_DENSITY_SURF", "Parameters", hoar_density_surf);

	//Minimum size to show surface hoar on surface (mm)
	cfg.getValue("HOAR_MIN_SIZE_SURF", "Parameters", hoar_min_size_surf);

	cfg.getValue("TIME_ZONE", "Input", time_zone);
}

/**
 * @brief Allocates and initializes Hazard data (flat field station only)
 * - Fills the snowpack version number, date of computation, user, ...
 * - Computes a zeroth order drift index for the first time step w/o shifting old_drift!
 * @author Michael Lehning
 * @version 10.03
 * @param duration of run (s)
 * @param *old_drift
 * @param cos_sl Cosine of slope angle
 */
void Hazard::initializeHazard(const double duration, double *old_drift, double slope_angle,
                              std::vector<ProcessDat>& Hdata, std::vector<ProcessInd>& Hdata_ind)
{
	int nHz = (int)floor( (duration / (hazard_steps_between * sn_dt)) ) + 2;
	if (nHz <= 0) nHz = 1;

	Hdata = vector<ProcessDat>((unsigned)nHz);
	Hdata_ind = vector<ProcessInd>((unsigned)nHz);

	memset(&Hdata[0], 0, sizeof(ProcessDat)*nHz);
	memset(&Hdata_ind[0], 0, sizeof(ProcessInd)*nHz);

	Hdata[0].nHz = (signed)nHz;
	Hdata[nHz-1].nHz = (signed)nHz;

	versionUserRuntime(time_zone, Hdata[0].sn_version, Hdata[0].sn_computation_date,
                     &Hdata[0].sn_jul_computation_date, Hdata[0].sn_compilation_date, Hdata[0].sn_user);

	Hdata[0].wind_trans = driftIndex(old_drift, 0., Hazard::wind_slab_density, 6, slope_angle, -1);
	Hdata[0].wind_trans24 = driftIndex(old_drift, 0., Hazard::wind_slab_density, 24, slope_angle, -1);
}

/**
 * @brief Computes drift index for past nHours
 * - shift
 * 	-  1 : shift old_drift, overwrite old_drift[0]
 * 	-  0 : overwrite old_drift[0] only
 * 	- -1 : no action at all
 * @author Charles Fierz, based on the original by Michael Lehning
 * @version 11.01
 * @param *old_drift double
 * @param drift double
 * @param rho double
 * @param nhour double
 * @param slope_angle
 * @param shift double
 */
double Hazard::driftIndex(double *old_drift, double drift, const double rho, const int nHours,
                          double slope_angle, const int shift)
{
	int    i, nValues;
	double sumindex = 0., flux, ero_depo = 0.;

	switch (shift) {
		case 1: // Shift drift data
			for(i=47; i>0; i--) {
				old_drift[i] = old_drift[i-1];
			}
		case 0: // Overwrite old_drift[0]
			old_drift[0] = drift;
		default:
			break;
	}

	for (i = 0, nValues = 0; i < 2*nHours; i++ ) {
		if (old_drift[i] == Constants::undefined){
			continue;
		} else {
			sumindex += old_drift[i];
			nValues++;
		}
	}
	if (nValues <= int(floor(Constants::min_percent_values * 2 * nHours))) {
		return Constants::undefined;
	} else {
		flux = H_TO_S(MAX(0,(sumindex - Hazard::minimum_drift)) / (2. * nHours)); // kg m-1 h-1
		ero_depo = M_TO_CM(flux * nHours / (Hazard::typical_slope_length * rho));
		ero_depo = MIN(ero_depo, nHours * Hazard::maximum_drift * cos(slope_angle));
		ero_depo /= cos(slope_angle);
		return ero_depo;
	}
}

void Hazard::getDriftIndex(ProcessDat& Hdata, ProcessInd& Hdata_ind,
                           double *old_drift, double& drift, double slope_angle)
{
	Hdata_ind.wind_trans = 0;
	Hdata_ind.wind_trans24 = 0;

	Hdata.wind_trans   = driftIndex(old_drift, drift, Hazard::wind_slab_density,  6, slope_angle, 1);
	Hdata.wind_trans24 = driftIndex(old_drift, drift, Hazard::wind_slab_density, 24, slope_angle, 0);

	if (Hdata.wind_trans < 0.) Hdata_ind.wind_trans = -1;
	if (Hdata.wind_trans24 < 0.) Hdata_ind.wind_trans24 = -1;
}

/**
 * @brief Compute the dew point deficit in degC
 * @param TA  Air temperature in kelvins
 * @param TSS Snow surface temperature in kelvins
 * @param RH  Relative air humidity (over water) in (percents or 1? TODO)
 */
double Hazard::compDewPointDeficit(double TA, double TSS, double RH)
{
	double b=9.5, c=265.5;

	TA = K_TO_C(TA);
	TSS = K_TO_C(TSS);

	return(TSS - (c*(log10(RH) + b*TA/(c+TA))/
			(b - log10(RH) - b*TA/(c+TA))));
}

/**
 * @brief Determines hoar mass index for last n hours in kg m-2
 * @author Michael Lehning
 * @version Y.mm
 * @param *OldHoar double
 * @param new_hoar double
 * @param nhour double
 * @param new_step double
 * @return double
 */
double Hazard::compHoarIndex(double *OldHoar, double new_hoar, int nhour, int new_step)
{
	int i;
	double hoar_ind = 0.;

	// Shift hoar data
	if (new_step)
	for (i = 47; i > 0; i-- ) {
		OldHoar[i] = OldHoar[i-1];
	}
	OldHoar[0] = new_hoar;

	// Determine hoar_ind
	for (i = 0; i < 2*nhour; i++) {
		hoar_ind += OldHoar[i];
	}
	return(hoar_ind);
}

void Hazard::compMeltFreezeCrust(const SnowStation& Xdata, ProcessDat& Hdata, ProcessInd& Hdata_ind)
{
	double crust_dep=0., crust_thick=0.;
	double cos_sl = cos(Xdata.SlopeAngle);
	int e = Xdata.getNumberOfElements()-1;

	while ((e > Xdata.SoilNode) && (crust_dep <= 0.03)) {
		if ((Xdata.Edata[e].type == 772) || (Xdata.Edata[e].type == 880)) {
			crust_thick += Xdata.Edata[e].L/cos_sl;
			if ((Xdata.Edata[e-1].type != 772) && (Xdata.Edata[e-1].type != 880)) {
				break;
			}
		} else {
			crust_dep += Xdata.Edata[e].L/cos_sl;
		}
		e--;
	}
	if ( (crust_thick >= 0.) && (crust_thick <= Xdata.cH/cos_sl) ) {
		Hdata.crust = M_TO_CM(crust_thick);
	} else {
		Hdata_ind.crust = -1;
	}
}

/**
 * @brief Given the "Zwischen" data containing the depth-hoar index, the wind-drift index and the
 * three and twenty-four newsnowfall rates, this routine computes the Hdata.
 * @param Hdata
 * @param Hdata_ind
 * @param d_hs6
 * @param d_hs24
 * @param Mdata
 * @param Sdata
 * @param Zdata
 * @param Xdata
 */
void Hazard::compHazard(ProcessDat& Hdata, ProcessInd& Hdata_ind, const double& d_hs6, const double& d_hs24,
                        const CurrentMeteo& Mdata,SurfaceFluxes& Sdata, SN_ZWISCHEN_DATA& Zdata,
                        const SnowStation& Xdata)
{
	int    nE = Xdata.getNumberOfElements();
	double cos_sl = cos(Xdata.SlopeAngle);
	double hs = Xdata.cH / cos_sl;

	const ElementData *EMS;  // Pointer to element data
	EMS = &Xdata.Edata[0];

	// Initialization
	Hdata.date = Mdata.date;

	Hdata.dewpt_def = 21.7;    Hdata_ind.dewpt_def  = 0;
	Hdata.hoar_ind6 = 21.7;    Hdata_ind.hoar_ind6 = 0;
	Hdata.hoar_ind24 = 21.7;   Hdata_ind.hoar_ind24 = 0;
	Hdata.hoar_size = 21.7;    Hdata_ind.hoar_ind24 = 0;

	Hdata.hn3 = 21.7;          Hdata_ind.hn3 = 0;
	Hdata.hn6 = 21.7;          Hdata_ind.hn6 = 0;
	Hdata.hn12 = 21.7;         Hdata_ind.hn12 = 0;
	Hdata.hn24 = 21.7;         Hdata_ind.hn24 = 0;
	Hdata.hn72 = 21.7;         Hdata_ind.hn72 = 0;
	Hdata.hn72_24 = 21.7;      Hdata_ind.hn72_24 = 0;
	Hdata.hnw3  = 21.7;        Hdata_ind.hnw3  = 0;
	Hdata.hnw6  = 21.7;        Hdata_ind.hnw6  = 0;
	Hdata.hnw12 = 21.7;        Hdata_ind.hnw12 = 0;
	Hdata.hnw24 = 21.7;        Hdata_ind.hnw24 = 0;
	Hdata.hnw72 = 21.7;        Hdata_ind.hnw72 = 0;

	Hdata.stab_class1 = 0;     Hdata_ind.stab_class1 = 0;
	Hdata.stab_class2 = 5;     Hdata_ind.stab_class2 = 0;

	Hdata.stab_index1 = 6.;    Hdata_ind.stab_index1 = 0;
	Hdata.stab_index2 = 6.;    Hdata_ind.stab_index2 = 0;
	Hdata.stab_index3 = 6.;    Hdata_ind.stab_index3 = 0;
	Hdata.stab_index4 = 6.;    Hdata_ind.stab_index4 = 0;
	Hdata.stab_index5 = 6.;    Hdata_ind.stab_index5 = 0;
	Hdata.stab_height1 = hs;   Hdata_ind.stab_height1 = 0;
	Hdata.stab_height2 = hs;   Hdata_ind.stab_height2 = 0;
	Hdata.stab_height3 = hs;   Hdata_ind.stab_height3 = 0;
	Hdata.stab_height4 = hs;   Hdata_ind.stab_height4 = 0;
	Hdata.stab_height5 = hs;   Hdata_ind.stab_height5 = 0;

	Hdata.ch = M_TO_CM(hs);    Hdata_ind.ch = 0;

	Hdata.swe     = 0.;        Hdata_ind.swe     = 0;
	Hdata.tot_lwc = 0.;        Hdata_ind.tot_lwc = 0;
	Hdata.runoff  = 0.;        Hdata_ind.runoff  = 0;

	Hdata.crust  =  0.0;       Hdata_ind.crust  = 0;
	Hdata.en_bal = 21.7;       Hdata_ind.en_bal = 0;
	Hdata.sw_net = 21.7;       Hdata_ind.sw_net = 0;
	Hdata.t_top1 = 21.7;       Hdata_ind.t_top1 = 0;
	Hdata.t_top2 = 21.7;       Hdata_ind.t_top2 = 0;

	// Compute depths of snowfall for given time intervals
	double t_hn[6] ={0.5, 3., 6., 12., 24., 72.}, hn[6], hnw[6];
	double sum_hn = 0., sum_hnw = 0.;
	int e = nE-1;
	for (unsigned int k = 0; k <= 5; k++) {
		while ((e >= Xdata.SoilNode) && ((Mdata.date - EMS[e].depositionDate).getJulianDate() < (H_TO_D(t_hn[k])))) {
			sum_hn  += EMS[e].L;
			sum_hnw += EMS[e].L * EMS[e].Rho;
			e--;
		}
		hn[k] = sum_hn;
		hnw[k] = sum_hnw;
	}
	Hdata.hn_half_hour = M_TO_CM(hn[0] / cos_sl);
	Hdata.hn3 =  M_TO_CM(hn[1] / cos_sl);
	Hdata.hn6 =  M_TO_CM(hn[2] / cos_sl);
	Hdata.hn12 =  M_TO_CM(hn[3] / cos_sl);
	Hdata.hn24 =  M_TO_CM(hn[4] / cos_sl);
	Hdata.hn72 =  M_TO_CM(hn[5] / cos_sl);
	Hdata.hnw_half_hour = hnw[0] / cos_sl;
	Hdata.hnw3 =  hnw[1] / cos_sl;
	Hdata.hnw6 =  hnw[2] / cos_sl;
	Hdata.hnw12 =  hnw[3] / cos_sl;
	Hdata.hnw24 =  hnw[4] / cos_sl;
	Hdata.hnw72 =  hnw[5] / cos_sl;

	// Compute 72h sum of 24h new snow depths for a total of 3 days
	for (int l = 143; l > 0; l--) {
		Zdata.hn24[l] = Zdata.hn24[l-1];
	}
	Zdata.hn24[0] = hn[4];
	Hdata.hn72_24 =  M_TO_CM((Zdata.hn24[0] + Zdata.hn24[48] + Zdata.hn24[96]) / cos_sl);

	if (enforce_measured_snow_heights) {
		// Check for CONSISTENCY and set indicator to -1 if necessary.
		//   The following is a cheap fix for InfoManager consistency if the deviation
		//   is small, i.e. most probably caused by the smoothing
		//   (4 cm for hns6, 10 cm for hns24).
		if (((Hdata.hn6 > 1.0)  &&  (Hdata.hn6 < M_TO_CM(d_hs6))) || ((Hdata.hn6 <= 1.0) && (M_TO_CM(d_hs6) > 3.0))) {
			if ((Hdata.hn6 - M_TO_CM(d_hs6)) < 4.0) {
				Hdata.hn6 = M_TO_CM(d_hs6) * 1.1;
				Hdata.hnw6 = Hdata.hn6 * 0.8;
				Hdata.hn3 = Hdata.hn6 * 0.7;
				Hdata.hnw3 = Hdata.hn3 * 0.8;
				Hdata.hn12 = MAX (Hdata.hn6, Hdata.hn12);
				Hdata.hnw12 = Hdata.hn12 * 0.8;
			} else {
				Hdata_ind.hn3  = -1;
				Hdata_ind.hn6  = -1;
				Hdata_ind.hn12 = -1;
				Hdata_ind.hnw3   = -1;
				Hdata_ind.hnw6   = -1;
				Hdata_ind.hnw12  = -1;
			}
		}
		if (((Hdata.hn24 > 2.0)  &&  (Hdata.hn24 < M_TO_CM(d_hs24))) || ((Hdata.hn24 <= 2.0) && (M_TO_CM(d_hs24) > 5.0))) {
			if ((Hdata.hn24 - M_TO_CM(d_hs24)) < 10.0) {
				Zdata.hn24[0] = d_hs24 * 1.1;
				Hdata.hn24 = M_TO_CM(Zdata.hn24[0]);
				Hdata.hnw24 = Hdata.hn24 * 0.8;
				Hdata.hn72 = MAX (Hdata.hn24 * 1.2, Hdata.hn72);
				Hdata.hnw72 = Hdata.hn72 * 0.8;
				Hdata.hn72_24 = M_TO_CM((Zdata.hn24[0] + Zdata.hn24[48] + Zdata.hn24[96]) / cos_sl);
			} else {
				Hdata_ind.hn24 = -1;
				Hdata_ind.hn72 = -1;
				Hdata_ind.hn72_24 = -1;
				Hdata_ind.hnw24 = -1;
				Hdata_ind.hnw72 = -1;
			}
		}
	}

	// INSTANTANEOUS DEWPOINT DEFICIT between TSS and Td(air)
	if (research_mode){
		Hdata.dewpt_def = K_TO_C(Xdata.Ndata[Xdata.getNumberOfNodes()-1].T) 
			- RhtoDewPoint(Mdata.rh, K_TO_C(Mdata.ta), force_rh_water);
	} else {
		Hdata.dewpt_def = compDewPointDeficit(Mdata.ta, Xdata.Ndata[Xdata.getNumberOfNodes()-1].T, Mdata.rh);
	}

	if (!((Hdata.dewpt_def > -50.) && (Hdata.dewpt_def < 50.))) {
		Hdata_ind.dewpt_def = -1;
	}

	// hoar size, size in mm assuming HOAR_DENSITY_SURF at surface
	Hdata.hoar_size = M_TO_MM(Xdata.Ndata[nE].hoar / hoar_density_surf);
	// Check for lower size limit
	if (Hdata.hoar_size <= hoar_min_size_surf) {
		Hdata.hoar_size = 0.;
	}
	if (!((Hdata.hoar_size >= 0.) && (Hdata.hoar_size < 100.))) {
		Hdata_ind.hoar_size = -1;
	}
	// HOAR INDEX (24h and 6h), mass in kg m-2
	Hdata.hoar_ind24 = compHoarIndex(&(Zdata.hoar24[0]), Sdata.hoar, 24, 1);
	if (!((Hdata.hoar_ind24 > -10.) && (Hdata.hoar_ind24 < 10.))) {
		Hdata_ind.hoar_ind24 = -1;
	}
	Hdata.hoar_ind6  = compHoarIndex(&(Zdata.hoar24[0]), Sdata.hoar,  6, 0);
	if (!((Hdata.hoar_ind6 > -10.) && (Hdata.hoar_ind6 < 10.))) {
		Hdata_ind.hoar_ind6 = -1;
	}

	// SWE and total liquid water content
	Hdata.swe = Sdata.mass[SurfaceFluxes::MS_SWE];
	Hdata.tot_lwc = Sdata.mass[SurfaceFluxes::MS_WATER];
	// Runoff rate (kg m-2 h-1)
	Hdata.runoff /= S_TO_H(sn_dt * hazard_steps_between);

	// Profile type
	if ((Xdata.S_class1 <= 10) && (Xdata.S_class1 >= 0)) {
		Hdata.stab_class1 = Xdata.S_class1;
	} else {
		Hdata_ind.stab_class1 = -1;
	}

	// Stability class
	if ((Xdata.S_class2 <= 5) && (Xdata.S_class2 >= 1)) {
		Hdata.stab_class2 = Xdata.S_class2;
	} else {
		Hdata_ind.stab_class2 = -1;
	}
	// Stability index: Deformation index
	if ((Xdata.S_d < (Stability::max_stability + Constants::eps)) && (Xdata.S_d > 0.)) {
		Hdata.stab_index1 = Xdata.S_d;
		Hdata.stab_height1 = M_TO_CM(Xdata.z_S_d / cos_sl);
	} else {
		Hdata_ind.stab_index1 = -1;
		Hdata_ind.stab_height1 = -1;
	}
	// Natural stability index Sn38
	if ((Xdata.S_n < (Stability::max_stability + Constants::eps)) && (Xdata.S_n > 0.)) {
		Hdata.stab_index2 = Xdata.S_n;
		Hdata.stab_height2 = M_TO_CM(Xdata.z_S_n / cos_sl);
	} else {
		Hdata_ind.stab_index2 = -1;
		Hdata_ind.stab_height2 = -1;
	}
	// Skier stability index Sk38
	if ((Xdata.S_s < (Stability::max_stability + Constants::eps)) && (Xdata.S_s > 0.)) {
		Hdata.stab_index3 = Xdata.S_s;
		Hdata.stab_height3 = M_TO_CM(Xdata.z_S_s / cos_sl);
	} else {
		Hdata_ind.stab_index3 = -1;
		Hdata_ind.stab_height3 = -1;
	}
	// Structural stability index SSI
	if ((Xdata.S_4 < (Stability::max_stability + Constants::eps)) && (Xdata.S_4 > 0.)) {
		Hdata.stab_index4 = Xdata.S_4;
		Hdata.stab_height4 = M_TO_CM(Xdata.z_S_4 / cos_sl);
	} else {
		Hdata_ind.stab_index4 = -1;
		Hdata_ind.stab_height4 = -1;
	}
	// ???Index???
	if ((Xdata.S_5 < (Stability::max_stability + Constants::eps)) && (Xdata.S_5 > 0.)) {
		Hdata.stab_index5 = Xdata.S_5;
		Hdata.stab_height5 = M_TO_CM(Xdata.z_S_5 / cos_sl);
	} else {
		Hdata_ind.stab_index5 = -1;
		Hdata_ind.stab_height5 = -1;
	}

	// Surface crust [type == 772] computed for southerly aspect outside compHazard()

	// Energy input ... (kJ m-2)
	if (nE > Xdata.SoilNode) {
		Hdata.en_bal = Sdata.dIntEnergy * hazard_steps_between / 1000.;
	} else {
		Hdata.en_bal = ( ((Sdata.qw + Sdata.lw_net + Sdata.qs + Sdata.ql + Sdata.qr))
				* sn_dt * hazard_steps_between ) / 1000.;
  }
	if (!((Hdata.en_bal > -3000.) && (Hdata.en_bal < 3000.))) {
		Hdata_ind.en_bal = -1;
	}

	// Net SW energy at surface (kJ m-2)
	if (Sdata.sw_in > 0.) {
		Hdata.sw_net = (Sdata.qw * sn_dt * hazard_steps_between) / 1000.;
		if (!((Hdata.sw_net > -3000.) && (Hdata.sw_net < 3000.))) {
			Hdata_ind.sw_net = -1;
		}
	} else {
		Hdata.sw_net = 0.;
	}

	// Snow temperatures t_top1 and t_top2 in degC at 5 cm and 10 cm below the surface, respectively
	double h_top1 = hs - 0.05;
	h_top1 = hs - 0.05;
	Hdata.t_top1 = getModelledTemperature(h_top1, Xdata);
	if ( !((Hdata.t_top1 > -50.) && (Hdata.t_top1 <= 0.)) ) {
		Hdata_ind.t_top1 = -1;
	}
	double h_top2 = hs - 0.10;
	Hdata.t_top2 = getModelledTemperature(h_top2, Xdata);
	if (!((Hdata.t_top2 > -50.) && (Hdata.t_top2 <= 0.))) {
		Hdata_ind.t_top2 = -1;
	}
}

void Hazard::getHazardData(ProcessDat& Hdata, ProcessInd& Hdata_ind,
                           const double& delta_hs6, const double& delta_hs24,
                           CurrentMeteo& Mdata, SurfaceFluxes& Sdata,
                           SN_ZWISCHEN_DATA& Zdata, SnowStation& Xdata_station, SnowStation& Xdata_south,
                           const unsigned int& nSlopes, const bool& virtual_slope)
{
	compHazard(Hdata, Hdata_ind, delta_hs6, delta_hs24, Mdata, Sdata, Zdata, Xdata_station);

	// Compute snow transport on flat fiels if needed
	if (!virtual_slope) {
		getDriftIndex(Hdata, Hdata_ind, &(Zdata.drift24[0]), Sdata.drift,
                  Xdata_station.SlopeAngle);
	}

	// ... determine vertical thickness of melt-freeze crust, not buried deeper than 3 cm ...
	if (nSlopes == 1) {
		compMeltFreezeCrust(Xdata_station, Hdata, Hdata_ind);
	} else {
		// ... but even better on southerly virtual slope
		compMeltFreezeCrust(Xdata_south, Hdata, Hdata_ind);
	}
}

/*
 * End of Hazard.cc
 */
