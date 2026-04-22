#include <complex>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>

namespace sjtu {

static const int BASE = 1000000000;  // 10^9
static const int BASE_DIGITS = 9;

// Helper function to remove leading zeros
void removeLeadingZeros(std::vector<int> &a) {
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

class int2048 {
private:
    std::vector<int> digits;
    int sign;

    bool isZero() const;
    int2048 abs() const;
    static void removeLeadingZeros(std::vector<int> &a);

public:
    // Constructors
    int2048();
    int2048(long long);
    int2048(const std::string &);
    int2048(const int2048 &);

    // Read from string
    void read(const std::string &);
    // Output the stored big integer, no need for newline
    void print();

    // Add a big integer
    int2048 &add(const int2048 &);
    // Return the sum of two big integers
    friend int2048 add(int2048, const int2048 &);

    // Subtract a big integer
    int2048 &minus(const int2048 &);
    // Return the difference of two big integers
    friend int2048 minus(int2048, const int2048 &);

    int2048 operator+() const;
    int2048 operator-() const;

    int2048 &operator=(const int2048 &);

    int2048 &operator+=(const int2048 &);
    friend int2048 operator+(int2048, const int2048 &);

    int2048 &operator-=(const int2048 &);
    friend int2048 operator-(int2048, const int2048 &);

    int2048 &operator*=(const int2048 &);
    friend int2048 operator*(int2048, const int2048 &);

    int2048 &operator/=(const int2048 &);
    friend int2048 operator/(int2048, const int2048 &);

    int2048 &operator%=(const int2048 &);
    friend int2048 operator%(int2048, const int2048 &);

    friend std::istream &operator>>(std::istream &, int2048 &);
    friend std::ostream &operator<<(std::ostream &, const int2048 &);

    friend bool operator==(const int2048 &, const int2048 &);
    friend bool operator!=(const int2048 &, const int2048 &);
    friend bool operator<(const int2048 &, const int2048 &);
    friend bool operator>(const int2048 &, const int2048 &);
    friend bool operator<=(const int2048 &, const int2048 &);
    friend bool operator>=(const int2048 &, const int2048 &);
};

// Implementation of all methods...

// Constructors
int2048::int2048() : sign(1) {}

int2048::int2048(long long num) {
    if (num < 0) {
        sign = -1;
        num = -num;
    } else {
        sign = 1;
    }

    if (num == 0) {
        digits.push_back(0);
    } else {
        while (num > 0) {
            digits.push_back(num % BASE);
            num /= BASE;
        }
    }
}

int2048::int2048(const std::string &s) {
    read(s);
}

int2048::int2048(const int2048 &other) {
    sign = other.sign;
    digits = other.digits;
}

// Read from string
void int2048::read(const std::string &s) {
    digits.clear();
    sign = 1;

    int pos = 0;
    if (s[0] == '-') {
        sign = -1;
        pos = 1;
    } else if (s[0] == '+') {
        pos = 1;
    }

    // Process digits in chunks of BASE_DIGITS
    for (int i = s.length() - 1; i >= pos; i -= BASE_DIGITS) {
        int chunk = 0;
        int power = 1;
        for (int j = 0; j < BASE_DIGITS && i - j >= pos; j++) {
            chunk += (s[i - j] - '0') * power;
            power *= 10;
        }
        digits.push_back(chunk);
    }

    removeLeadingZeros(digits);
    if (digits.empty()) {
        digits.push_back(0);
        sign = 1;
    }
}

// Print the number
void int2048::print() {
    if (digits.empty() || (digits.size() == 1 && digits[0] == 0)) {
        std::cout << '0';
        return;
    }

    if (sign == -1) {
        std::cout << '-';
    }

    std::cout << digits.back();
    for (int i = digits.size() - 2; i >= 0; i--) {
        std::cout << std::string(BASE_DIGITS - std::to_string(digits[i]).length(), '0') << digits[i];
    }
}

// Addition helper
std::vector<int> addVectors(const std::vector<int> &a, const std::vector<int> &b) {
    std::vector<int> result;
    int carry = 0;

    for (int i = 0; i < std::max(a.size(), b.size()) || carry; i++) {
        int sum = carry;
        if (i < a.size()) sum += a[i];
        if (i < b.size()) sum += b[i];

        carry = sum >= BASE;
        if (carry) sum -= BASE;
        result.push_back(sum);
    }

    return result;
}

// Subtraction helper (assumes a >= b)
std::vector<int> subtractVectors(const std::vector<int> &a, const std::vector<int> &b) {
    std::vector<int> result;
    int carry = 0;

    for (int i = 0; i < a.size(); i++) {
        int diff = a[i] - carry;
        if (i < b.size()) diff -= b[i];

        if (diff < 0) {
            diff += BASE;
            carry = 1;
        } else {
            carry = 0;
        }
        result.push_back(diff);
    }

    removeLeadingZeros(result);
    return result;
}

// Compare absolute values
bool absLess(const std::vector<int> &a, const std::vector<int> &b) {
    if (a.size() != b.size()) return a.size() < b.size();
    for (int i = a.size() - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i];
    }
    return false;
}

// Add operation
int2048 &int2048::add(const int2048 &other) {
    if (sign == other.sign) {
        digits = addVectors(digits, other.digits);
    } else {
        if (absLess(digits, other.digits)) {
            std::vector<int> result = subtractVectors(other.digits, digits);
            digits = result;
            sign = other.sign;
        } else if (absLess(other.digits, digits)) {
            std::vector<int> result = subtractVectors(digits, other.digits);
            digits = result;
        } else {
            // Equal absolute values, result is 0
            digits.clear();
            digits.push_back(0);
            sign = 1;
        }
    }
    return *this;
}

// Friend add function
int2048 add(int2048 a, const int2048 &b) {
    return a.add(b);
}

// Subtract operation
int2048 &int2048::minus(const int2048 &other) {
    // a - b = a + (-b)
    int2048 temp = other;
    temp.sign *= -1;
    return add(temp);
}

// Friend minus function
int2048 minus(int2048 a, const int2048 &b) {
    return a.minus(b);
}

// Unary operators
int2048 int2048::operator+() const {
    return *this;
}

int2048 int2048::operator-() const {
    int2048 result = *this;
    if (!(digits.size() == 1 && digits[0] == 0)) {
        result.sign *= -1;
    }
    return result;
}

// Assignment operator
int2048 &int2048::operator=(const int2048 &other) {
    if (this != &other) {
        sign = other.sign;
        digits = other.digits;
    }
    return *this;
}

// Compound assignment operators
int2048 &int2048::operator+=(const int2048 &other) {
    return add(other);
}

int2048 &int2048::operator-=(const int2048 &other) {
    return minus(other);
}

// Binary operators
int2048 operator+(int2048 a, const int2048 &b) {
    return a.add(b);
}

int2048 operator-(int2048 a, const int2048 &b) {
    return a.minus(b);
}

// Multiplication
int2048 &int2048::operator*=(const int2048 &other) {
    if (isZero() || other.isZero()) {
        *this = int2048();
        return *this;
    }

    sign *= other.sign;

    // Simple O(n^2) multiplication for now
    std::vector<long long> temp(digits.size() + other.digits.size(), 0);

    for (int i = 0; i < digits.size(); i++) {
        for (int j = 0; j < other.digits.size(); j++) {
            temp[i + j] += (long long)digits[i] * other.digits[j];
        }
    }

    // Handle carries
    digits.clear();
    long long carry = 0;
    for (int i = 0; i < temp.size() || carry; i++) {
        if (i < temp.size()) carry += temp[i];
        digits.push_back(carry % BASE);
        carry /= BASE;
    }

    removeLeadingZeros(digits);
    return *this;
}

int2048 operator*(int2048 a, const int2048 &b) {
    return a *= b;
}

// Division (floor division towards negative infinity)
int2048 &int2048::operator/=(const int2048 &other) {
    if (other.isZero()) {
        // Division by zero - return 0 to avoid crash
        *this = int2048();
        return *this;
    }

    if (isZero()) {
        return *this;
    }

    int resultSign = sign * other.sign;

    // Work with absolute values
    int2048 absThis = abs();
    int2048 absOther = other.abs();

    if (absThis < absOther) {
        *this = int2048();
        return *this;
    }

    // Long division
    std::vector<int> result;
    int2048 current;
    bool hasRemainder = false;

    for (int i = digits.size() - 1; i >= 0; i--) {
        current.digits.insert(current.digits.begin(), digits[i]);
        current.removeLeadingZeros(current.digits);

        int x = 0, l = 0, r = BASE;
        while (l <= r) {
            int m = (l + r) / 2;
            int2048 temp = absOther * int2048(m);
            if (temp <= current) {
                x = m;
                l = m + 1;
            } else {
                r = m - 1;
            }
        }

        result.push_back(x);
        current = current - absOther * int2048(x);
        current.removeLeadingZeros(current.digits);
        if (!current.isZero()) hasRemainder = true;
    }

    // Reverse to get most significant digit first
    std::reverse(result.begin(), result.end());

    digits = result;
    sign = resultSign;
    removeLeadingZeros(digits);

    // Handle negative division (floor towards negative infinity)
    if (sign == -1 && hasRemainder) {
        *this = *this - int2048(1);
    }

    return *this;
}

int2048 operator/(int2048 a, const int2048 &b) {
    return a /= b;
}

// Modulo operation
int2048 &int2048::operator%=(const int2048 &other) {
    if (other.isZero()) {
        // Modulo by zero - return 0 to avoid crash
        *this = int2048();
        return *this;
    }
    int2048 quotient = *this / other;
    *this = *this - quotient * other;
    return *this;
}

int2048 operator%(int2048 a, const int2048 &b) {
    return a %= b;
}

// Stream operators
std::istream &operator>>(std::istream &in, int2048 &num) {
    std::string s;
    in >> s;
    num.read(s);
    return in;
}

std::ostream &operator<<(std::ostream &out, const int2048 &num) {
    if (num.digits.empty() || (num.digits.size() == 1 && num.digits[0] == 0)) {
        out << '0';
        return out;
    }

    if (num.sign == -1) {
        out << '-';
    }

    out << num.digits.back();
    for (int i = num.digits.size() - 2; i >= 0; i--) {
        out << std::string(BASE_DIGITS - std::to_string(num.digits[i]).length(), '0') << num.digits[i];
    }

    return out;
}

// Comparison operators
bool operator==(const int2048 &a, const int2048 &b) {
    if (a.sign != b.sign) return false;
    if (a.digits.size() != b.digits.size()) return false;
    for (int i = 0; i < a.digits.size(); i++) {
        if (a.digits[i] != b.digits[i]) return false;
    }
    return true;
}

bool operator!=(const int2048 &a, const int2048 &b) {
    return !(a == b);
}

bool operator<(const int2048 &a, const int2048 &b) {
    if (a.sign != b.sign) return a.sign < b.sign;

    if (a.digits.size() != b.digits.size()) {
        return (a.sign == 1) ? (a.digits.size() < b.digits.size()) : (a.digits.size() > b.digits.size());
    }

    for (int i = a.digits.size() - 1; i >= 0; i--) {
        if (a.digits[i] != b.digits[i]) {
            return (a.sign == 1) ? (a.digits[i] < b.digits[i]) : (a.digits[i] > b.digits[i]);
        }
    }

    return false;
}

bool operator>(const int2048 &a, const int2048 &b) {
    return b < a;
}

bool operator<=(const int2048 &a, const int2048 &b) {
    return !(b < a);
}

bool operator>=(const int2048 &a, const int2048 &b) {
    return !(a < b);
}

// Helper function to check if zero
bool int2048::isZero() const {
    return digits.size() == 1 && digits[0] == 0;
}

// Helper function to get absolute value
int2048 int2048::abs() const {
    int2048 result = *this;
    if (!isZero()) {
        result.sign = 1;
    }
    return result;
}

// Helper function to remove leading zeros (non-static version)
void int2048::removeLeadingZeros(std::vector<int> &a) {
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

} // namespace sjtu