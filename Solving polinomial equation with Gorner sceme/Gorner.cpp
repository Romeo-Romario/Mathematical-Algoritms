#include <iostream>
#include<vector>
#include <unordered_set>
#include <cmath>
using namespace std;


vector<float> mergeAndDivide(const vector<float>& v1, const vector<float>& v2);
vector<float> mergeCommon(const vector<float>& v1, const vector<float>& v2);
float bigger(float first, float second);
float smaller(float first, float second);
double findLinearRoot(const vector<float>& coeffs);
vector<float> divisioners(vector<float>& mainVector);
vector<float> findQuadraticRoots(const vector<float>& coeffs);

float bigger(float first, float second) {
	if (first >= second) return first;
	else {
		return second;
	}
}
float smaller(float first, float second) {
	if (first >= second) return second;
	else {
		return first;
	}
}

double findLinearRoot(const vector<float>& coeffs) {
	return -coeffs[1] / coeffs[0];
}


vector<float> divisioners(vector<float>& mainVector) {
	vector<float>  dividersForFirst = {};
	vector<float>  dividersForLast = {};

	int firstCoefficient = mainVector[0];
	int freeCoefficient = mainVector[mainVector.size() - 1];
	int i = 1;

	for (int i = 1; i - 1 < bigger(abs(firstCoefficient), abs(freeCoefficient)); i++)
	{

		if (firstCoefficient % i == 0) {
			dividersForFirst.push_back(i);
			dividersForFirst.push_back(-i);

		}
		if (freeCoefficient % i == 0) {
			dividersForLast.push_back(i);
			dividersForLast.push_back(-i);
		}

	}


	return mergeAndDivide(dividersForFirst, dividersForLast);

}


vector<float> mergeAndDivide(const vector<float>& v1, const vector<float>& v2) {
	vector<float> result;
	unordered_set<double> seen;

	// Merge the two vectors into a single vector
	vector<float> merged(v1.size() + v2.size());
	copy(v1.begin(), v1.end(), merged.begin());
	copy(v2.begin(), v2.end(), merged.begin() + v1.size());

	// Compute the division of each pair of elements, without repetition
	for (int i = 0; i < merged.size(); i++) {
		for (int j = i + 1; j < merged.size(); j++) {
			if (merged[j] != 0 && seen.find((double)merged[i] / merged[j]) == seen.end()) {
				result.push_back((double)merged[i] / merged[j]);
				seen.insert((double)merged[i] / merged[j]);
			}
			if (merged[i] != 0 && seen.find((double)merged[j] / merged[i]) == seen.end()) {
				result.push_back((double)merged[j] / merged[i]);
				seen.insert((double)merged[j] / merged[i]);
			}
		}
	}
	return result;
}

vector<float> findQuadraticRoots(const vector<float>& coeffs) {
	vector<float> roots;

	double a = coeffs[0];
	double b = coeffs[1];
	double c = coeffs[2];

	double discriminant = b * b - 4 * a * c;

	if (discriminant > 0) {
		double root1 = (-b + sqrt(discriminant)) / (2 * a);
		double root2 = (-b - sqrt(discriminant)) / (2 * a);
		roots.push_back(root1);
		roots.push_back(root2);
	}
	else if (discriminant == 0) {
		double root = -b / (2 * a);
		roots.push_back(root);
	}
	else {
		// No real roots, do nothing
	}

	return roots;
}

vector<float> mergeCommon(const vector<float>& v1, const vector<float>& v2) {
	vector<float> result;
	unordered_set<float> seen(v1.begin(), v1.end());

	for (int i = 0; i < v2.size(); i++) {
		if (seen.find(v2[i]) != seen.end()) {
			result.push_back(v2[i]);
			seen.erase(v2[i]);
		}
	}

	return result;
}

vector<float> rootsOfGorner(vector<float>& mainVector, vector<float>& divisioners) {
	vector<float> storage(mainVector.size(), 0);
	vector<float> roots = {};
	storage[0] = mainVector[0];
	storage[1] = mainVector[1];

	// Check
	if (mainVector.size() == 3) {

		for (int y = 0; y < 2; y++)
		{
			roots.push_back(findQuadraticRoots(mainVector)[y]);
		}
	}
	else if (mainVector.size() == 2) {

		roots.push_back(findLinearRoot(mainVector));
	}

	else {
		int index = 0;
		while (true) {

			if (index == divisioners.size()) {
				break;
				vector<float> nothing = {};
				return nothing;
			}
			for (int i = 1; i < mainVector.size(); i++)
			{
				storage[i] = mainVector[i] + storage[i - 1] * divisioners[index];
			}
			if (storage[storage.size() - 1] == 0) {
				roots.push_back(divisioners[index]);
				divisioners.erase(divisioners.begin() + index);
				storage.pop_back();
				mainVector.clear();
				for (int g = 0; g < storage.size(); g++)
				{
					mainVector.push_back(storage[g]);
				}
			}
			if (storage.size() == 3) {

				for (int y = 0; y < 2; y++)
				{
					roots.push_back(findQuadraticRoots(storage)[y]);
				}
				break;
			}
			if (mainVector.size() == 2) {

				roots.push_back(findLinearRoot(storage));
				break;
			}
			if (storage[storage.size() - 1] != 0) {
				storage[0] = mainVector[0];
				storage[1] = mainVector[1];
				index++;
			}
		}
	}
	return roots;
}


int main() {
	// 1, -1, -17, 21, 36
	// 1, 2, -12, -6, 19, 8
	// 1, 2, -13, -38, -24
	// 5, -36, 62, -36, 5
	// -2, 17, -34, -19, 76, -30
	// 1, -7, 13, -7, 12
	// 2, -7, -3, 5, -1
	// -1, 11, 6
	// 1, -7, 13, -7, 12
	// 1, 2, -12, -6, 19, 8
	// -2, 17, -34, -19 , 76, -30
	vector<float> poly = { 1, -1, -17, 21, 36 }; // This is the place where you input coefficients near each x
	vector<float> dividers = divisioners(poly);
	vector<float> roots = rootsOfGorner(poly, dividers);



	cout << "The roots are: ";
	for (int i = 0; i < roots.size(); i++) {
		cout << roots[i];
		if (i != roots.size() - 1) {
			cout << ", ";
		}

	}
	/*cout << "The resulting vector is: ";
	for (int i = 0; i < dividers.size(); i++) {
		cout << dividers[i];
		if (i != dividers.size() - 1) {
			cout << ", ";
		}
	}*/


}