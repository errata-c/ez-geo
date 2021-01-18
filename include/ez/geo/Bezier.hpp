#pragma once
#include <array>
#include <type_traits>

#include <ez/math/Complex.hpp>
#include <ez/math/MathConstants.hpp>
#include <ez/math/Polynomial.hpp>
#include <ez/math/PsuedoRandom.hpp>

#include <glm/gtc/vec1.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/norm.hpp>
#include "AABB2d.hpp"
#include "CBezier.hpp"

#define EZ_GEO_BEZIER_FUNCTIONS_INCLUDED
#include "intern/BezierTypeChecks.hpp"
#include "intern/BezierUtil.hpp"
#include "intern/BezierInterpolation.hpp"
#include "intern/BezierDerivatives.hpp"
#include "intern/BezierSplit.hpp"
#include "intern/BezierLength.hpp"
#include "intern/BezierFitting.hpp"
#include "intern/BezierOffsets.hpp"