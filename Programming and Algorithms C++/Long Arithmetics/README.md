Implementation of C++ class CBigInt representing integers (both positive and negative) with virtually unlimited range.

Built-in integer data types like int and long long int have a fixed size, thus the range is fixed too. If a computation requires a bigger range, a custom data type must be used to represent the numbers. Our class representing big integers will use dynamically allocated memory to store the numbers. Thus the range will be unlimited (only the size of the available memory is the limit). Your task is to implement the class. To simplify the task, we adopted certain limitations:

Only integers are considered. The class will not have to care for decimal/fractional numbers.
The class will only support basic math operations: addition, multiplication, and comparison.
The class shall conform to the following interface:

default constructor
the constructor will prepare an object representing number 0.
constructor with an integer parameter
the constructor will initialize the object with the value given by the integer parameter.
constructor with string (ASCIIZ) parameter
the constructor will initialize the object with the number passed as the parameter. The number parameter is in the decimal notation. If the string does not represent a valid decimal number, the constructor must throw std::invalid_argument exception. This exception is declared in the standard library (header stdexcept). The constructor of invalid_argument takes one parameter - a textual description of the problem. The description string may be just anything in this homework.
copy constructor
the constructor will be implemented if your internal representation requires it.
destructor
destructor will be present if your internal representation requires it.
overloaded operator=
the operator will allow assignments from integer, ASCIIZ string and CBigInt instance (right-hand side) to a CBigInt instance (left-hand side).
operator <<
will output an instance of CBigInt to an output stream. In the mandatory tests, the output will be in decimal, without unnecessary leading zeros. Moreover, bonus tests switch output to either decimal or hexadecimal. Your implementation must check stream settings and provide correct output. The hexadecimal output also suppresses unnecessary leading zeros and uses lowercase letters for digits 10 to 15.
operator >>
will read an instance of CBigInt from an input stream (the input will be in decimal).
operator +
this operator adds two instances of:
CBigInt + CBigInt,
CBigInt + int,
CBigInt + ASCIIZ string,
int + CBigInt, and
ASCIIZ string + CBigInt.
operator +=
this operator will add the right-hand operand to the left-hand operand. Valid right-hand operands are CBigInt, integer, and ASCIIZ string (representing a decimal number).
operator *
this operator multiplies two integers, operands are the same as in the case of the addition operator,
operator *=
this operator will multiply the left hand operand with the value of the right-hand operand. Valid right-hand operands are be CBigInt, integer, and ASCIIZ string (representing a decimal number).
relational operators (< <=, > >=, == and !=)
are designated to compare two integers, the types of operands are the same as in the case of the addition and multiplication.
