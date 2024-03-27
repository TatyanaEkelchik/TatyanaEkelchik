#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>

class CBigInt {
private:
    // Represents the number as a string in a reverse order
    // The order is reversed to simplify the arithmetic operations
    // The index in the string corresponds directly to the power of 10 for that digit 
    // i.e., index 0 is the 10^0 place, index 1 is the 10^1 place, etc.
    std::string numAsRevStr; 
    bool isNegative = false;

    void removeLeadingZeros() {
        while (numAsRevStr.length() > 1 && numAsRevStr.back() == '0') {
            numAsRevStr.pop_back();
        }
        if (numAsRevStr.length() == 1 && numAsRevStr[0] == '0') isNegative = false;
    }



public:
    // Default constructor
    CBigInt() 
        : numAsRevStr("0"), isNegative(false)
    {}

    // Int constructor
    CBigInt(int n) {
        isNegative = n < 0;
        numAsRevStr = std::to_string(std::abs(n));
        std::reverse(numAsRevStr.begin(), numAsRevStr.end());
    }

    // String constructor
    CBigInt(const std::string& str) {
        if (str.empty()) throw std::invalid_argument("An empty string is not a valid number!");
        isNegative = str[0] == '-';
        for (size_t i = isNegative ? 1 : 0; i < str.length(); ++i) { // Why cycle not a reverse()?
            if (!isdigit(str.at(i))) { // why not str[i]?
                throw std::invalid_argument("The string contains non-digit characters!");
            }
            numAsRevStr = str[i] + numAsRevStr; 
        }
        removeLeadingZeros(); // Where from leading zeros?
    }

    // Copy constructor
    CBigInt(const CBigInt& other) = default;

    // Destructor
    ~CBigInt() = default;

    // Assignment operator =
    CBigInt& operator=(const CBigInt& other) = default;

    CBigInt& operator=(int n) {
        CBigInt temp(n);
        *this = temp;
        return *this;
    }

    CBigInt& operator=(const std::string& str) {
        CBigInt temp(str);
        *this = temp;
        return *this;
    }

    // Output operator <<
    friend std::ostream& operator<<(std::ostream& outStream, const CBigInt& number) {
        if (number.isNegative) 
            outStream << '-';
        for (auto iter = number.numAsRevStr.rbegin(); iter != number.numAsRevStr.rend(); ++iter) { //what is rbegin()?
            outStream << *iter;
        }
        return outStream;
    }

    // Input operator >>
    friend std::istream& operator>>(std::istream& inStream, CBigInt& number) {
        std::string userInput;
        inStream >> userInput;

        // Ignore leading whitespaces
        userInput.erase(0, userInput.find_first_not_of(" \t\n\r\f\v"));

        // Stop reading the number at the first non-digit character 
        auto lastIndex = userInput.find_last_of("0123456789");
        userInput = userInput.substr(0, lastIndex+1);

        // If the string contains any chars but from "-0123456789"
        if (userInput.find_last_not_of("-0123456789") != std::string::npos || userInput == "") { 
            inStream.setstate(std::ios::failbit);
            return inStream;
        }

        number = CBigInt(userInput);
        return inStream;
    }

   

    // Operator +
    CBigInt operator+(const CBigInt& rhs) const {
        CBigInt result;
        size_t lenLhs = this->numAsRevStr.length();
        size_t lenRhs = rhs.numAsRevStr.length();
        size_t length = std::max(lenLhs, lenRhs);
        // To make sure the string is empty before starting summation
        result.numAsRevStr = "";
        int carry = 0;
        // Same signs
        if (this->isNegative == rhs.isNegative) {
            result.isNegative = this->isNegative; 
            for (size_t i = 0; i < length || carry > 0; ++i) {
                int lhsDigit = i < lenLhs ? numAsRevStr[i] - '0' : 0;
                int rhsDigit = i < lenRhs ? rhs.numAsRevStr[i] - '0' : 0;
                int digitSum = lhsDigit + rhsDigit + carry;
                carry = digitSum / 10;
                result.numAsRevStr.push_back('0' + (digitSum % 10));
            }
            return result;
        }


        // Different signs
      bool absLhsIsLarger = (*this > rhs) ? (*this >= rhs * (-1)) : ((*this * (-1)) >= rhs);
result.isNegative = absLhsIsLarger ? this->isNegative : rhs.isNegative;


        for (size_t i = 0; i < length; ++i) {
            int lhsDigit = i < lenLhs ? this->numAsRevStr[i] - '0' : 0;
            int rhsDigit = i < lenRhs ? rhs.numAsRevStr[i] - '0' : 0;
            int digitSub = absLhsIsLarger ? lhsDigit - rhsDigit - carry : rhsDigit - lhsDigit - carry;

            // If subtraction results in a negative digit, 
            // borrow from the next higher digit
            if (digitSub < 0) {
                digitSub += 10;
                carry = 1;
            }
            else {
                carry = 0;
            }

            result.numAsRevStr.push_back('0' + digitSub);
        }

        result.removeLeadingZeros();
        return result;
    }

    CBigInt operator+(int rhs) { 
        return *this + CBigInt(rhs);
    }

    friend CBigInt operator+(int lhs, const CBigInt& rhs) { // can we do it const?
        return CBigInt(lhs) + rhs;
    }

    friend CBigInt operator+(const CBigInt& lhs, const std::string& rhs) { // How can we change const?
        return lhs + CBigInt(rhs);
    }

    friend CBigInt operator+(const std::string& lhs, const CBigInt& rhs) {
        return CBigInt(lhs) + rhs;
    }

    // Operator +=
    CBigInt operator+=(const CBigInt& rhs) {
        *this = *this + rhs;
        return *this;
    }

    CBigInt operator+=(const std::string& rhs) {
        *this = *this + rhs;
        return *this;
    }

    CBigInt operator+=(int rhs) {
        *this = *this + rhs;
        return *this;
    }

    // Operator *
    CBigInt operator*(const CBigInt& rhs) const {
        CBigInt result;
        result.numAsRevStr = std::string(this->numAsRevStr.length() + rhs.numAsRevStr.length(), '0'); // what is here?
        for (size_t i = 0; i < this->numAsRevStr.length(); ++i) {
            int carry = 0;
            for (size_t j = 0; j < rhs.numAsRevStr.length() || carry; ++j) {
                int n1 = this->numAsRevStr[i] - '0';
                int n2 = j < rhs.numAsRevStr.length() ? rhs.numAsRevStr[j] - '0' : 0;
                auto sum = (result.numAsRevStr[i + j] - '0') + n1 * n2 + carry;
                result.numAsRevStr[i + j] = '0' + sum % 10;
                carry = sum / 10;
            }
        }
        result.isNegative = this->isNegative != rhs.isNegative; 
        result.removeLeadingZeros(); 
        return result;
    }

    CBigInt operator*(int rhs) { // can we do it const?
        return *this * CBigInt(rhs);
    }

    friend CBigInt operator*(int lhs, const CBigInt& rhs) {
        return CBigInt(lhs) * rhs;
    }

    friend CBigInt operator*(const CBigInt& lhs, const std::string& rhs) { // can we write it whth one argunent (string& rhs)?
        return lhs * CBigInt(rhs);
    }

    friend CBigInt operator*(const std::string& lhs, const CBigInt& rhs) {
        return CBigInt(lhs) * rhs;
    }

    // Operator *=
    CBigInt operator*=(const CBigInt& rhs) {
        *this = *this * rhs;
        return *this;
    }

    CBigInt operator*=(const std::string& rhs) {
        *this = *this * rhs;
        return *this;
    }

    CBigInt operator*=(int rhs) {
        *this = *this * rhs;
        return *this;
    }

    // Operator ==
     bool operator==(const CBigInt& rhs) const {
        auto lenLhs = this->numAsRevStr.length();
        auto lenRhs = rhs.numAsRevStr.length();
        if (lenLhs != lenRhs || this->isNegative != rhs.isNegative)
            return false;
        for (size_t i = 0; i < lenLhs; ++i) {
            if (this->numAsRevStr[i] != rhs.numAsRevStr[i])
                return false;
        }
        return true;
    }

    bool operator==(int rhs) {
        return *this == CBigInt(rhs);
    }

   friend bool operator==(int lhs, const CBigInt & rhs) {
        return CBigInt(lhs) == rhs;
    }

    friend bool operator==(const CBigInt & lhs, const std::string & rhs) {
        return lhs == CBigInt(rhs);
    }

    friend bool operator==(const std::string & lhs, const CBigInt & rhs) {
        return CBigInt(lhs) == rhs;
    }

    // Operator !=
    bool operator!=(const CBigInt& rhs) const {
        return !(*this == rhs);
    }

    bool operator!=(int rhs) {
        return *this != CBigInt(rhs);
    }

    friend bool operator!=(int lhs, const CBigInt& rhs) {
        return CBigInt(lhs) != rhs;
    }

    friend bool operator!=(const CBigInt& lhs, const std::string& rhs) {
        return lhs != CBigInt(rhs);
    }

    friend bool operator!=(const std::string& lhs, const CBigInt& rhs) {
        return CBigInt(lhs) != rhs;
    }

    // Operator <
    bool operator<(const CBigInt & rhs) const {
        if (*this == rhs)
            return false;
        if (this->isNegative && !rhs.isNegative)
            return true;
        if (!this->isNegative && rhs.isNegative)
            return false;

        auto lenLhs = this->numAsRevStr.length();
        auto lenRhs = rhs.numAsRevStr.length();

        if (lenLhs < lenRhs)
            return !this->isNegative;
        if (lenLhs > lenRhs)
            return this->isNegative;

        // By now, lenLhs == lenRhs, and both numbers are either non-negative or negative
        for (size_t i = 1; i <= lenLhs; ++i) {
            if (this->numAsRevStr[lenLhs - i] != rhs.numAsRevStr[lenLhs - i])
                return this->numAsRevStr[lenLhs - i] < rhs.numAsRevStr[lenLhs - i] && !this->isNegative;
        }
        return false;
    }

    bool operator<(int rhs) {
        return *this < CBigInt(rhs);
    }

    friend bool operator<(int lhs, const CBigInt& rhs) {
        return CBigInt(lhs) < rhs;
    }

    friend bool operator<(const CBigInt& lhs, const std::string& rhs) {
        return lhs < CBigInt(rhs);
    }

    friend bool operator<(const std::string& lhs, const CBigInt& rhs) {
        return CBigInt(lhs) < rhs;
    }

    // Operator <=
    bool operator<=(const CBigInt& rhs) const {
        return *this == rhs || *this < rhs;
    }

    bool operator<=(int rhs) {
        return *this == CBigInt(rhs) || *this < CBigInt(rhs);
    }

    friend bool operator<=(int lhs, const CBigInt& rhs) {
        return CBigInt(lhs) == rhs || CBigInt(lhs) < rhs;
    }

    friend bool operator<=(const CBigInt& lhs, const std::string& rhs) {
        return lhs == CBigInt(rhs) || lhs < CBigInt(rhs);
    }

     friend bool operator<=(const std::string& lhs, const CBigInt& rhs) {
        return CBigInt(lhs) == rhs || CBigInt(lhs) < rhs;
    }

    // Operator >
    bool operator>(const CBigInt& rhs) const {
        return !(*this <= rhs);
    }

    bool operator>(int rhs) {
        return !(*this <= CBigInt(rhs));
    }

     friend bool operator>(int lhs, const CBigInt& rhs) {
        return !(CBigInt(lhs) <= rhs);
    }

    friend bool operator>(const CBigInt& lhs, const std::string& rhs) {
        return !(lhs <= CBigInt(rhs));
    }

     friend bool operator>(const std::string& lhs, const CBigInt& rhs) {
        return !(CBigInt(lhs) <= rhs);
    }

    // Operator >=
    bool operator>=(const CBigInt& rhs) const {
        return !(*this < rhs);
    }

    bool operator>=(int rhs) {
        return !(*this < CBigInt(rhs));
    }

    friend bool operator>=(int lhs, const CBigInt& rhs) {
        return !(CBigInt(lhs) < rhs);
    }

    friend bool operator>=(const CBigInt& lhs, const std::string& rhs) {
        return !(lhs < CBigInt(rhs));
    }

    friend bool operator>=(const std::string& lhs, const CBigInt& rhs) {
        return !(CBigInt(lhs) < rhs);
    }

};

static bool equal(const CBigInt& x, const char val[])
{
    std::ostringstream oss;
    oss << x;
    return oss.str() == val;
}

int main()
{
    CBigInt a, b;
    std::istringstream is;
    a = 10;
    a += 20;
    assert(equal(a, "30"));
    a *= 5;
    assert(equal(a, "150"));
    b = a + 3;
    assert(equal(b, "153"));
    b = a * 7;
    assert(equal(b, "1050"));
    assert(equal(a, "150"));

    a = 10;
    a += -20;
    assert(equal(a, "-10"));
    a *= 5;
    assert(equal(a, "-50"));
    b = a + 73;
    assert(equal(b, "23"));
    b = a * -7;
    assert(equal(b, "350"));
    assert(equal(a, "-50"));

    a = "12345678901234567890";
    a += "-99999999999999999999";
    assert(equal(a, "-87654321098765432109"));
    a *= "54321987654321987654";
    assert(equal(a, "-4761556948575111126880627366067073182286"));
    a *= 0;
    assert(equal(a, "0"));
    a = 10;
    b = a + "400";
    assert(equal(b, "410"));
    b = a * "15";
    assert(equal(b, "150"));
    assert(equal(a, "10"));

    is.clear();
    is.str(" 1234");
    assert(is >> b);
    assert(equal(b, "1234"));
    is.clear();
    is.str(" 12 34");
    assert(is >> b);
    assert(equal(b, "12"));
    is.clear();

    is.str("999z");
    assert(is >> b);
    assert(equal(b, "999"));
    is.clear();

    is.str("abcd");
    assert(!(is >> b));

    is.clear();
    is.str("- 758");
    assert(!(is >> b));

    a = 42;
    try
    {
        a = "-xyz";
        assert("missing an exception" == nullptr);
    }
    catch (const std::invalid_argument &e)
    {
        assert(equal(a, "42"));
    }
    
     a = "73786976294838206464";
    assert(equal(a, "73786976294838206464"));
    assert(a < "1361129467683753853853498429727072845824");
    assert(a < "99911294676837538538");
    assert(a <= "1361129467683753853853498429727072845824");
    assert(!(a > "1361129467683753853853498429727072845824"));
    assert(!(a >= "1361129467683753853853498429727072845824"));
    assert(!(a == "1361129467683753853853498429727072845824"));
    assert(a != "1361129467683753853853498429727072845824");
    assert(!(a < "73786976294838206464"));
    assert(a <= "73786976294838206464");
    assert(!(a > "73786976294838206464"));
    assert(a >= "73786976294838206464");
    assert(a == "73786976294838206464");
    assert(!(a != "73786976294838206464"));
    assert(a < "73786976294838206465");
    assert(a <= "73786976294838206465");
    assert(!(a > "73786976294838206465"));
    assert(!(a >= "73786976294838206465"));
    assert(!(a == "73786976294838206465"));
    assert(a != "73786976294838206465");
    a = "2147483648";
    assert(!(a < -2147483648));
    assert(!(a <= -2147483648));
    assert(a > -2147483648);
    assert(a >= -2147483648);
    assert(!(a == -2147483648));
    assert(a != -2147483648);
    a = "-12345678";
    assert(!(a < -87654321));
    assert(!(a <= -87654321));
    assert(a > -87654321);
    assert(a >= -87654321);
    assert(!(a == -87654321));
    assert(a != -87654321);
    a = "9999999";
    assert(a <= "9999999");
    return EXIT_SUCCESS;
}


