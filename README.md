# SmartCalc v2.0

This is a project work at my university. As per the assignment, an enhanced version of a calculator was developed using C++ and OOP principles. In addition to basic arithmetic operations such as addition/subtraction and multiplication/division, the calculator can evaluate arithmetic expressions following the order of operations and includes several mathematical functions (sine, cosine, logarithm, etc.).

Beyond expression evaluation, the calculator supports the use of the _x_ variable and can plot the corresponding function. It also features an integrated loan and deposit calculator. The project follows the MVC pattern, with a clear separation between logic and graphical interface. The Qt framework was used for the visual aspect of the project.

## Contents

1. [Installation and commands](#installation-and-commands)
2. [Calculator overview](#calculator-overview)
3. [MVC Model](#mvc-model)
4. [Technical specifications](#technical-specifications)

## Installation and commands

You will need the following utilities to install `gcc`, `make` and `cmake`

##### Makefile targets:

- **install**  
  Installs the calculator in the build folder and on the desktop

- **uninstall**  
  Uninstall the calculator


## Calculator overview
![2024-06-14 22-04-46 (online-video-cutter com) (online-video-cutter com)](https://github.com/BulatRuslanovich/SmartCalc/assets/113189826/52e01b7e-cb3c-4060-a7d2-d37c0622adfd)


## MVC Model

All program logic (model) and GUI have been separated and linked via controllers.  
![image](https://github.com/BulatRuslanovich/SmartCalc/assets/113189826/b84328f2-f4ee-44a6-b207-83eb4292a3ee)


## Technical specifications

  - **Arithmetic operators**:

    | Operator name  | Infix notation <br /> (Classic) | Prefix notation <br /> (Polish notation) | Postfix notation <br /> (Reverse Polish notation) |
    | -------------- | ------------------------------- | ---------------------------------------- | ------------------------------------------------- |
    | Brackets       | (a + b)                         | (+ a b)                                  | a b +                                             |
    | Addition       | a + b                           | + a b                                    | a b +                                             |
    | Subtraction    | a - b                           | - a b                                    | a b -                                             |
    | Multiplication | a \* b                          | \* a b                                   | a b \*                                            |
    | Division       | a / b                           | / a b                                    | a b \                                             |
    | Power          | a ^ b                           | ^ a b                                    | a b ^                                             |
    | Modulus        | a mod b                         | mod a b                                  | a b mod                                           |
    | Unary plus     | +a                              | +a                                       | a+                                                |
    | Unary minus    | -a                              | -a                                       | a-                                                |


  - **Functions**:

    | Function description       | Function |
    | -------------------------- | -------- |
    | Computes cosine            | cos(x)   |
    | Computes sine              | sin(x)   |
    | Computes tangent           | tan(x)   |
    | Computes arc cosine        | acos(x)  |
    | Computes arc sine          | asin(x)  |
    | Computes arc tangent       | atan(x)  |
    | Computes square root       | sqrt(x)  |
    | Computes natural logarithm | ln(x)    |
    | Computes common logarithm  | log(x)   |
    | Factorial                  | x!       |

### Credit calculator
![image](https://github.com/BulatRuslanovich/SmartCalc/assets/113189826/399d96a2-4b4c-4be3-aa9f-6effa73728ed)


### Deposit calculator
![image](https://github.com/BulatRuslanovich/SmartCalc/assets/113189826/70f7c138-a8c4-4924-8a4f-f5949b3b52be)

