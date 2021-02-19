#include <vector>
#include <iostream>
#include <cmath>

#include <fmt/core.h>
#include <fmt/ostream.h>
#include <fmt/format.h>

#include <ez/geo/Bezier.hpp>

double f(double x) {
	return std::sin(x);
}
double df(double x) {
	return std::sin(x);
}

double g(double x, double c) {
	return x - c * x * x * x;
}
double dg(double x, double c) {
	return 1.0 - 3.0 * c * x * x;
}

double error(double x, double c) {
	double tmp = f(x) - g(x, c);
	return tmp * tmp;
}

double errorSum(double low, double high, double c) {
	double range = high - low;
	double num = 99999;
	double h = range / num;

	double sum = error(low, c) + error(high, c);
	for (double i = 1.0; i < num; i += 2.0) {
		sum += 4.0 * error(low + i * h, c) + 2.0 * error(low + i * h + h, c);
	}

	return sum * (h / 3.0);
}

int main(int argc, char*  argv[]) {

	// pi -> 
	glm::vec2 p0{ 0,0 }, p1{1, 0};
	
	double low = -0.3;
	double high = 0.3;

	double c = 0.166;
	double delta = 0.7;
	double range = 0.1;
	int num = 1000;

	for (int i = 0; i < num; ++i) {
		double c0 = c - range;
		double c1 = c + range;
		range = range * delta;

		double e0 = errorSum(low, high, c0);
		double e1 = errorSum(low, high, c1);

		if (e0 < e1) {
			c = c0;
		}
		else {
			c = c1;
		}
	}

	fmt::print("The calculated coeff is: {}\n", c);
	double efinal = errorSum(low, high, c);
	fmt::print("The final error is; {}\n", efinal);

	return 0;
}