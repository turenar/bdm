#include <cstdio>
#include <cmath>
#include <cassert>
#include "bdm/task/position_estimation.hxx"

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

		constexpr double sound_velocity = 340;
		constexpr double sound_velocity_squared = sound_velocity * sound_velocity;
		constexpr double limit = 0.01;
	}

	vector3d position_estimation::estimate(const std::array<double, channels_count>& in) {
		//double in[0] = 0, in[1] = 0.6, in[2] = 1.5, in[3] = 1.55;  //delay
		double X = 200, Y = 200, Z = 200; //position
		//velocity
		double LIMIT = 0.01;

		int sign; //function shape
		double tx1, tx2, ty1, ty2, tz1, tz2;
		double x, y, z, t, lastt;
		double e, de, dt;

		t = 9999999;  //initialize
		lastt = 9999999;

		tx1 = in[1] + in[3] - in[0] - in[2];
		tx2 = in[1] * in[1] + in[3] * in[3] - in[0] * in[0] - in[2] * in[2];
		ty1 = in[1] + in[2] - in[0] - in[3];
		ty2 = in[1] * in[1] + in[2] * in[2] - in[0] * in[0] - in[3] * in[3];
		tz1 = in[2] + in[3] - in[0] - in[1];
		tz2 = in[2] * in[2] + in[3] * in[3] - in[0] * in[0] - in[1] * in[1];

		x = (sound_velocity_squared / (8 * X)) * (tx2 + 2 * t * tx1);
		y = (sound_velocity_squared / (8 * Y)) * (ty2 + 2 * t * ty1);
		z = (sound_velocity_squared / (8 * Z)) * (tz2 + 2 * t * tz1);

		e = (X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) -
		    sound_velocity_squared * (t + in[0]) * (t + in[0]);
		sign = get_sign(e);

		while (true) {
			//calculation
			x = (sound_velocity_squared / (8 * X)) * (tx2 + 2 * t * tx1);
			y = (sound_velocity_squared / (8 * Y)) * (ty2 + 2 * t * ty1);
			z = (sound_velocity_squared / (8 * Z)) * (tz2 + 2 * t * tz1);

			e = ((X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) -
			     sound_velocity_squared * (t + in[0]) * (t + in[0])) * sign;

			de = 2 * sound_velocity_squared *
			     ((tx1 * (x - X) / X + ty1 * (y - Y) / Y + tz1 * (z - Z) / Z) / 4 - (t + in[0])) * sign;

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
					x = (sound_velocity_squared / (8 * X)) * (tx2 + 2 * min * tx1);
					y = (sound_velocity_squared / (8 * Y)) * (ty2 + 2 * min * ty1);
					z = (sound_velocity_squared / (8 * Z)) * (tz2 + 2 * min * tz1);
					emin = ((X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) - sound_velocity *
					                                                                    sound_velocity * (min + in[0]) *
					                                                                    (min + in[0])) *
					       sign;

					x = (sound_velocity_squared / (8 * X)) * (tx2 + 2 * max * tx1);
					y = (sound_velocity_squared / (8 * Y)) * (ty2 + 2 * max * ty1);
					z = (sound_velocity_squared / (8 * Z)) * (tz2 + 2 * max * tz1);
					emax = ((X - x) * (X - x) + (Y - y) * (Y - y) + (Z - z) * (Z - z) - sound_velocity *
					                                                                    sound_velocity * (max + in[0]) *
					                                                                    (max + in[0])) *
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
		x = (sound_velocity_squared / (8 * X)) * (tx2 + 2 * t * tx1);
		y = (sound_velocity_squared / (8 * Y)) * (ty2 + 2 * t * ty1);
		z = (sound_velocity_squared / (8 * Z)) * (tz2 + 2 * t * tz1);
		return {x, y, z};
	}
}
