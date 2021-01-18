#pragma once
#ifndef EZ_GEO_BEZIER_FUNCTIONS_INCLUDED
#include "BezierTypeChecks.hpp"
#include "BezierDerivatives.hpp"
#include "BezierInterpolation.hpp"
#endif

namespace ez {
	namespace Bezier {

		namespace intern {

			template<typename T, typename Iter>
			void simplePixelOffsetCurve(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, T delta, Iter output) {
				static constexpr T threshold = static_cast<T>(0.921);

				// To support negative offsets
				T compare = std::abs(delta);

				// offset the points first,
				// Check the middle, if its the right distance FIN
				// else subdivide and repeat.
				std::array<glm::tvec2<T>, 3> base{ p0, p1, p2 };

				T range = static_cast<T>(1);
				T start = static_cast<T>(0);

				while (start < range) {
					glm::tvec2<T> n1 = base[2] - base[1];
					n1 = glm::normalize(n1);

					glm::vec<2, T> n0 = base[1] - base[0];
					n0 = glm::normalize(n0);

					if (glm::dot(n0, n1) < threshold) {
						// Subdivide
						range = (start + range) * static_cast<T>(0.5);
						Bezier::leftSplit(base[0], base[1], base[2], static_cast<T>(0.5), base.begin());
						continue;
					}
					else {
						n0 = glm::vec<2, T>(-n0.y, n0.x);
						n1 = glm::vec<2, T>(-n1.y, n1.x);

						glm::vec<2, T> m = glm::normalize(n0 + n1);

						n0 = base[0] + n0 * delta;
						m = interpolate(base[0], base[1], base[2], static_cast<T>(0.5)) + m * delta;
						n1 = base[2] + n1 * delta;

						// Finalize the current offset segment, check if we are done.
						(*output) = n0;
						++output;

						Bezier::curveThrough(n0, m, n1, output);

						(*output) = n1;
						++output;

						if (range < static_cast<T>(1)) {
							// Move to next segment.
							start = range;
							range = static_cast<T>(1);
							Bezier::rightSplit(p0, p1, p2, start, base.begin());
							continue;
						}
						else {
							// Finish.
							break;
						}
					}
				} // End While
			}// End simplePixelOffsetCurve

			template<typename T, typename Iter>
			void simplePixelOffsetCurve(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, const glm::vec<2, T>& p3, T delta, Iter output) {
				static constexpr std::array<T, 4> interp{ static_cast<T>(0.0), static_cast<T>(1.0 / 3.0), static_cast<T>(2.0 / 3.0), static_cast<T>(1.0) };

				static constexpr T threshold = static_cast<T>(0.93);
				
				// offset the points first,
				// Check the middle, if its the right distance FIN
				// else subdivide and repeat.
				std::array<glm::tvec2<T>, 4> base{ p0, p1, p2, p3 };
				Bezier::leftSplit(base[0], base[1], base[2], base[3], static_cast<T>(0.5), base.begin());

				T range = static_cast<T>(0.5);
				T start = static_cast<T>(0.0);
				

				while (start < range) {
					glm::tvec2<T> n1 = base[2] - base[1];
					n1 = glm::normalize(n1);

					glm::vec<2, T> n0 = base[1] - base[0];
					n0 = glm::normalize(n0);

					if (glm::dot(n0, n1) < threshold) {
						// Subdivide
						range = (start + range) * static_cast<T>(0.5);
						Bezier::leftSplit(base[0], base[1], base[2], base[3], static_cast<T>(0.5), base.begin());
						continue;
					}
					else {
						n0 = glm::vec<2, T>(-n0.y, n0.x);
						n1 = glm::vec<2, T>(-n1.y, n1.x);

						glm::vec<2, T> n2 = base[3] - base[2];
						n2 = glm::vec<2, T>(-n2.y, n2.x);
						n2 = glm::normalize(n2);

						glm::vec<2, T> m0 = glm::normalize(n0 + n1);
						glm::vec<2, T> m1 = glm::normalize(n2 + n1);

						n0 = base[0] + n0 * delta;

						m0 = Bezier::interpolate(base[0], base[1], base[2], base[3], interp[1])
							+ m0 * delta;

						m1 = Bezier::interpolate(base[0], base[1], base[2], base[3], interp[2])
							+ m1 * delta;

						n2 = base[3] + n2 * delta;

						*output = n0;
						++output;

						Bezier::curveThrough(n0, m0, m1, n2, output);

						*output = n2;
						++output;

						if (range < static_cast<T>(1)) {
							// Move to next segment.
							start = range;
							range = static_cast<T>(1);
							Bezier::rightSplit(p0, p1, p2, p3, start, base.begin());
							continue;
						}
						else {
							// Finish.
							break;
						}
					}
				} // End While
			} // End simplePixelOffsetCurve

			template<typename T, typename Iter>
			void taperedPixelOffsetCurve(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, const glm::vec<2, T>& p3, const glm::vec<4, T>& taper, Iter output) {
				static constexpr std::array<T, 4> interp{ static_cast<T>(0.0), static_cast<T>(1.0 / 3.0), static_cast<T>(2.0 / 3.0), static_cast<T>(1.0) };
				
				// Since its tapered, use slightly tighter bounds
				static constexpr T threshold = static_cast<T>(0.97);

				// offset the points first,
				// Check the middle, if its the right distance FIN
				// else subdivide and repeat.
				std::array<T, 4> taperBase{ taper.x, taper.y, taper.z, taper.w };
				std::array<glm::tvec2<T>, 4> base{ p0, p1, p2, p3 };
				Bezier::leftSplit(base[0], base[1], base[2], base[3], static_cast<T>(0.5), base.begin());
				Bezier::leftSplit(taperBase[0], taperBase[1], taperBase[2], taperBase[3], static_cast<T>(0.5), taperBase.begin());

				T range = static_cast<T>(0.5);
				T start = static_cast<T>(0.0);

				while (start < range) {
					glm::tvec2<T> n1 = base[2] - base[1];
					n1 = glm::normalize(n1);

					glm::vec<2, T> n0 = base[1] - base[0];
					n0 = glm::normalize(n0);

					if (glm::dot(n0, n1) < threshold) {
						// Subdivide
						range = (start + range) * static_cast<T>(0.5);
						Bezier::leftSplit(base[0], base[1], base[2], base[3], T(0.5), base.begin());
						Bezier::leftSplit(taperBase[0], taperBase[1], taperBase[2], taperBase[3], T(0.5), taperBase.begin());
						continue;
					}
					else {
						n0 = glm::vec<2, T>(-n0.y, n0.x);
						n1 = glm::vec<2, T>(-n1.y, n1.x);

						glm::vec<2, T> n2 = base[3] - base[2];
						n2 = glm::vec<2, T>(-n2.y, n2.x);
						n2 = glm::normalize(n2);

						glm::vec<2, T> m0 = glm::normalize(n0 + n1);
						glm::vec<2, T> m1 = glm::normalize(n2 + n1);

						n0 = base[0] + n0 * interpolate(taperBase[0], taperBase[1], taperBase[2], taperBase[3], interp[0]);

						m0 = Bezier::interpolate(base[0], base[1], base[2], base[3], interp[1]) 
							+ m0 * interpolate(taperBase[0], taperBase[1], taperBase[2], taperBase[3], interp[1]);

						m1 = Bezier::interpolate(base[0], base[1], base[2], base[3], interp[2])
							+ m1 * interpolate(taperBase[0], taperBase[1], taperBase[2], taperBase[3], interp[2]);

						n2 = base[3] + n2 * interpolate(taperBase[0], taperBase[1], taperBase[2], taperBase[3], interp[3]);

						*output = n0;
						++output;

						Bezier::curveThrough(n0, m0, m1, n2, output);

						*output = n2;
						++output;

						// Finalize the current offset segment, check if we are done.

						if (range < static_cast<T>(1)) {
							// Move to next segment.
							start = range;
							range = static_cast<T>(1);
							Bezier::rightSplit(p0, p1, p2, p3, start, base.begin());
							Bezier::rightSplit(taper[0], taper[1], taper[2], taper[3], start, taperBase.begin());
							continue;
						}
						else {
							// Finish.
							break;
						}
					}
				} // End While
			} // End taperedPixelOffsetCurve

		}; // End namespace intern

		// Offset a quadratic bezier curve, assuming that all coordinates are in pixel scale (essentially integer precision).
		template<typename T, typename Iter>
		void pixelOffset(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, T delta, Iter output) {
			static_assert(is_output_iterator<Iter, glm::vec<2, T>>::value, "The iterator passed in is not a proper output iterator!");
			intern::simplePixelOffsetCurve(p0, p1, p2, delta, output);
		}

		// Offset a cubic bezier curve, assuming that all coordinates are in pixel scale (essentially integer precision).
		template<typename T, typename Iter>
		void pixelOffset(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, const glm::vec<2, T>& p3, T delta, Iter output) {
			static_assert(is_output_iterator<Iter, glm::vec<2, T>>::value, "The iterator passed in is not a proper output iterator!");
			intern::simplePixelOffsetCurve(p0, p1, p2, p3, delta, output);
		}
		
		template<typename T, typename Iter>
		void taperedPixelOffset(const glm::vec<2, T>& p0, const glm::vec<2, T>& p1, const glm::vec<2, T>& p2, const glm::vec<2, T>& p3, glm::vec<4, T> taper, Iter output) {
			static_assert(is_output_iterator<Iter, glm::vec<2, T>>::value, "The iterator passed in is not a proper output iterator!");
			intern::taperedPixelOffsetCurve(p0, p1, p2, p3, taper, output);
		}

	}; // End namespace Bezier
}; // End namespace ez
