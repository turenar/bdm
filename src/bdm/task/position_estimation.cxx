#include "bdm/task/position_estimation.hxx"
#include <cassert>
#include <cmath>
#include <cstdio>

namespace bdm::task {
	namespace {
		constexpr int get_sign(double d) {
			if (d > 0) {
				return 1;
			} else if (d < 0) {
				return -1;
			}
			return 0;
		}
	}

	void position_estimation::estimate_position(double T0, double T1, double T2, double T3, double* pos) {

		//double T0 = 0, T1 = 0.6, T2 = 1.5, T3 = 1.55;  //delay
		double X = 125, Y = 125, Z = 82; //position
		double V = 340; //velocity
		double LIMIT = 0.01;

		int sign; //function shape
		double tx1, tx2, ty1, ty2, tz1, tz2;
		double x, y, z, t, lastt;
		double e, de, dt;

		t = 9999999;  //initialize
		lastt = 9999999;

		tx1 = T1 + T3 - T0 - T2;
		tx2 = T1 * T1 + T3 * T3 - T0 * T0 - T2 * T2;
		ty1 = T1 + T2 - T0 - T3;
		ty2 = T1 * T1 + T2 * T2 - T0 * T0 - T3 * T3;
		tz1 = T2 + T3 - T0 - T1;
		tz2 = T2 * T2 + T3 * T3 - T0 * T0 - T1 * T1;

		x = (V * V / (8 * X)) * (tx2 + 2 * t * tx1);
		y = (V * V / (8 * Y)) * (ty2 + 2 * t * ty1);
		z = (V * V / (8 * Z)) * (tz2 + 2 * t * tz1);

		e = (X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) - V * V * (t + T0) * (t + T0);
		sign = get_sign(e);

		while (true) {
			//calculation
			x = (V * V / (8 * X)) * (tx2 + 2 * t * tx1);
			y = (V * V / (8 * Y)) * (ty2 + 2 * t * ty1);
			z = (V * V / (8 * Z)) * (tz2 + 2 * t * tz1);

			e = ((X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) - V * V * (t + T0) * (t + T0)) * sign;

			de = 2 * V * V * ((tx1 * (x - X) / X + ty1 * (y - Y) / Y + tz1 * (z - Z) / Z) / 4 - (t + T0)) * sign;

			dt = -e / de;
			if (fabs(dt) < LIMIT) {
				break;
			}
			if (de < 0) {
				//irregular case
				double min, max;
				double emin, emax;
				min = t;
				max = lastt;
				while (true) {
					//binary search
					x = (V * V / (8 * X)) * (tx2 + 2 * min * tx1);
					y = (V * V / (8 * Y)) * (ty2 + 2 * min * ty1);
					z = (V * V / (8 * Z)) * (tz2 + 2 * min * tz1);
					emin = ((X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) -
					        V * V * (min + T0) * (min + T0)) *
					       sign;

					x = (V * V / (8 * X)) * (tx2 + 2 * max * tx1);
					y = (V * V / (8 * Y)) * (ty2 + 2 * max * ty1);
					z = (V * V / (8 * Z)) * (tz2 + 2 * max * tz1);
					emax = ((X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) -
					        V * V * (max + T0) * (max + T0)) *
					       sign;

					if (emin < emax) {
						//min is nearer
						max = (min + max) / 2;
					} else {
						//max is nearer
						min = (min + max) / 2;
					}

					if (max - min < LIMIT) {
						//binary search end
						t = (min + max) / 2;
						break;
					}
				}
				break;
			}
			lastt = t;
			t += dt;
		}
		printf("%lf\n", t);
		x = (V * V / (8 * X)) * (tx2 + 2 * t * tx1);
		y = (V * V / (8 * Y)) * (ty2 + 2 * t * ty1);
		z = (V * V / (8 * Z)) * (tz2 + 2 * t * tz1);
		pos[0] = x;
		pos[1] = y;
		pos[2] = z;
		return;
	}
}
