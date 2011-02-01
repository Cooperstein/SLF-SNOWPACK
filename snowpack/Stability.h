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
#ifndef __STABILITY_H__
#define __STABILITY_H__

#define CALL_MEMBER_FN(object,ptrToMember)  ((object).*(ptrToMember)) 

#include <snowpack/Constants.h>
#include <snowpack/DataClasses.h>
#include <snowpack/Laws_sn.h>
#include <snowpack/Laws.h>
#include <vector>
#include <map>
#include <string>

/**
 * @class InstabilityData
 * @brief double and integer values to pinpoint structural instability
 */
class InstabilityData {
	public:
		InstabilityData() : n_lemon(0), dhard(0.0), dgsz(0.0), ssi(0.0) {}

		int    n_lemon;  ///< Number of "lemons" found
		double dhard;    ///< Difference in hardness
		double dgsz;     ///< Difference in grain size
		double ssi;      ///< Sk38 + structural instabilities (dhard & dgsz)
};

/**
 * @class StabilityData
 * @brief Structure of double and integer for shear strength evaluation
 */
class StabilityData {
	public:
 		StabilityData() : cH(0.0), Sig_c2(0.0), strength_up(0.0), phi(0.0), sig_n(0.0), sig_s(0.0),
		                  alpha_max(0.0), psi_ref(0.0), cos_psi_ref(0.0), sin_psi_ref(0.0) {}

		double cH;          ///< Calculated height of snow (m)
		double Sig_c2;      ///< Element shear strength (kPa)
		double strength_up; ///< Shear strength of adjacent upper element
		double phi;         ///< Correction to normal load
		double sig_n;       ///< Normal load on upper element node, perpendicular to slope
		double sig_s;       ///< Shear stress on upper element node, parallel to slope
		double alpha_max;   ///< Angle from snow surface to peak shear stress, 54.3 at 38 deg
		double psi_ref;     ///< Slope angle of reference slope, usually 38 deg
		double cos_psi_ref; ///< Cosine of psi_ref
		double sin_psi_ref; ///< Sine of psi_ref
};

class Stability;

typedef double (Stability::*StabMemFn)(const ElementData&);
typedef bool (Stability::*StabFnShearStrength)(const double&, const double&, const mio::Date&, 
                                               ElementData&, NodeData&, StabilityData&);

/**
 * @class Stability
 * @version -
 * @date    -
 * @bug     -
 * @author Michael Lehning \n Charles Fierz \n Sascha Bellaire
 * @brief This class contains the stability routines for the snowpack model.
 * TODO: update description 2009-10-20 \n
 * Stability is found for each LAYER (i.e. finite element) and INTERFACE (i.e. node).
 * Subsequently, the element data contains a variable S_dr (layer stability for
 * direct action avalanches) and the nodal data contains the varialble S_i
 * (interface stability). The station data contain S_class an overall stability
 * estimation for the profile based on hand hardness, grain class and a slab
 * characterization.  At the end, the stability index approach is compared to this
 * profile characterization to check consistency/reliability. This is a first "shot"
 * and it would be a miracle if we got it right at the very beginning.
 */
class Stability {
	public:
		Stability (const mio::Config& i_cfg);

		void checkStability(const SN_MET_DATA& Mdata, SnowStation& Xdata);

		static const double psi_ref, max_stability, minimum_slab, ground_rough;
		static const double min_depth_ssi, skier_depth, min_thick_crust;
		static const int sh_mod, nmax_lemon, prof_classi; 

	private:

		void initStability(const double& psi_ref, StabilityData& STpar, 
		                   SnowStation& Xdata, std::vector<InstabilityData>& SIdata);

		double st_HandHardnessDEFAULT(const ElementData& Edata);
		double st_HandHardnessASARC(const ElementData& Edata);

		double st_CriticalStress(const double& epDotn, const double& T_s);

		double st_DeformationRateIndex(ElementData& Edata);

		double st_PenetrationDepth(const SnowStation& Xdata);

		void compReducedStresses(const double& stress, const double& cos_sl, StabilityData& STpar);
		
		bool st_ShearStrengthDEFAULT(const double& cH, const double& cos_sl, const mio::Date& date,
		                             ElementData& Edata, NodeData& Ndata, StabilityData& STpar);
		bool st_ShearStrengthSTRENGTH_NIED(const double& cH, const double& cos_sl, const mio::Date& date,
		                             ElementData& Edata, NodeData& Ndata, StabilityData& STpar);

		double st_NaturalStabilityIndex(const StabilityData& STpar);

		double st_SkierStabilityIndex(const double& depth_lay, const StabilityData& STpar);

		void setStructuralStabilityIndex(const ElementData& Edata_low, const ElementData& Edata_up,
		                                 const double& Sk, InstabilityData& SIdata);

		bool classifyProfileStability(SnowStation& Xdata);
		
		bool recognizeProfileType(const mio::Date& date, SnowStation& Xdata);
 
	private:
		static const bool __init;    ///<helper variable to enable the init of static collection data
		static bool initStaticData();///<initialize the static containers
		static std::map<std::string, StabMemFn> mapHandHardness;
		static std::map<std::string, StabFnShearStrength> mapShearStrength;
		
		const mio::Config& cfg;
		std::string strength_model, hardness_model;
		bool plastic;
		double hoar_density_buried;
};



#endif //End of Stability.h
