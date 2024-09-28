#include <iostream>
#include <map>
#include <cmath>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>

// Structure to represent a point (x, y)
struct Point {
    int x;
    long long y;
};

// Function to decode y value from different bases
long long decodeValue(int base, const std::string& value) {
    long long result = 0;
    for (char c : value) {
        result *= base;
        if ('0' <= c && c <= '9') {
            result += c - '0';
        } else if ('A' <= c && c <= 'Z') {
            result += c - 'A' + 10;
        }
    }
    return result;
}

// Lagrange interpolation function to find the constant term
double lagrangeInterpolation(const std::vector<Point>& points) {
    int k = points.size(); // Number of points
    double constantTerm = 0; // To store the constant term (f(0))

    // Loop over each point (xi, yi)
    for (int i = 0; i < k; i++) {
        int xi = points[i].x;
        long long yi = points[i].y;

        // Lagrange basis polynomial
        double li = 1;
        for (int j = 0; j < k; j++) {
            if (i != j) {
                int xj = points[j].x;
                li *= -static_cast<double>(xj) / (xi - xj);
            }
        }

        // Add the contribution of this term to the constant term
        constantTerm += yi * li;
    }

    return constantTerm;
}

// Function to parse the input JSON-like structure and solve for the constant term
double solveSecret(std::map<std::string, std::map<std::string, std::string>>& jsonInput) {
    int n = std::stoi(jsonInput["keys"]["n"]);
    int k = std::stoi(jsonInput["keys"]["k"]);

    // Collect the first 'k' points
    std::vector<Point> points;
    int count = 0;

    for (auto& pair : jsonInput) {
        if (pair.first != "keys" && count < k) {
            int x = std::stoi(pair.first); // Use the key as the x value
            int base = std::stoi(pair.second["base"]); // Get the base
            std::string value = pair.second["value"]; // Get the encoded value
            long long y = decodeValue(base, value); // Decode the y value
            points.push_back({ x, y }); // Add the point (x, y)
            count++;
        }
    }

    // Perform Lagrange interpolation to find the constant term 'c'
    double constantTerm = lagrangeInterpolation(points);

    return constantTerm;
}

// Utility function to print double values with precision
void printResult(double value) {
    std::cout << std::fixed << std::setprecision(10) << value << std::endl;
}

int main() {
    // Example Test Case 1
    std::map<std::string, std::map<std::string, std::string>> testCase1 = {
        {"keys", {{"n", "4"}, {"k", "3"}}},
        {"1", {{"base", "10"}, {"value", "4"}}},
        {"2", {{"base", "2"}, {"value", "111"}}},
        {"3", {{"base", "10"}, {"value", "12"}}},
        {"6", {{"base", "4"}, {"value", "213"}}}
    };

    // Example Test Case 2
    std::map<std::string, std::map<std::string, std::string>> testCase2 = {
        {"keys", {{"n", "9"}, {"k", "6"}}},
        {"1", {{"base", "10"}, {"value", "28735619723837"}}},
        {"2", {{"base", "16"}, {"value", "1A228867F0CA"}}},
        {"3", {{"base", "12"}, {"value", "32811A4AA0B7B"}}},
        {"4", {{"base", "11"}, {"value", "917978721331A"}}},
        {"5", {{"base", "16"}, {"value", "1A22886782E1"}}},
        {"6", {{"base", "10"}, {"value", "28735619654702"}}},
        {"7", {{"base", "14"}, {"value",  "71AB5070CC4B"}}},
        {"8", {{"base", "9"}, {"value", "122662581541670"}}},
        {"9", {{"base", "8"}, {"value", "642121030037605"}}}
    };

    // Run the tests and print the results
    std::cout << "Test Case 1: Constant term c = ";
    printResult(solveSecret(testCase1));

    std::cout << "Test Case 2: Constant term c = ";
    printResult(solveSecret(testCase2));

    return 0;
}