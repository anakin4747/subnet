# Subnetting Program

The Subnetting Program is a command-line tool written in C that allows users to perform subnetting calculations. It provides an intuitive interface for calculating subnets based on different input possibilities, such as classful and CIDR addresses, dotted decimal subnet masks, and more.

## Features

- Calculate subnets based on various input possibilities:
  - Classful & CIDR
  - Classful & Dotted Decimal
  - CIDR & Dotted Decimal
  - Classful Maximum Subnets/Hosts
  - CIDR Maximum Subnets/Hosts (coming soon)
  - Advanced subnet calculations (coming soon)

- Object-oriented programming (OOP) implementation in C, leveraging abstraction for improved code organization and maintainability.

- Thorough testing framework to ensure correct functionality and handle invalid inputs.

## Installation

To install the Subnetting Program, follow these steps:

1. Clone this repository to your local machine.
2. Navigate to the project's directory.
3. Run the following command to install the program:

   ```
   sudo make install
   ```

   This will compile the program and install it on your system.

## Usage

To use the Subnetting Program, execute the `subnet` command followed by the appropriate input.

### Examples

Here are a few examples of how to use the Subnetting Program:

```
subnet 192.168.0.0/25
```

This example calculates the subnets for the given network address and new CIDR mask. It displays information such as the number of subnets possible, number of hosts per subnet, subnet masks, network address, broadcast address, and more.

```
subnet 10.0.0.0 255.128.0.0
```

This example calculates subnets based on a classful network address and a dotted decimal subnet mask.

For more examples and detailed usage instructions, refer to the [Examples](#examples) section.

## Future Plans

The Subnetting Program has a roadmap for future enhancements and features, including:

- Implementing advanced subnet calculations.
- Adding more comprehensive testing, including validation of logically correct addresses.
- Refactoring the code to reduce code repetition.
- Providing switches for customizing output information.
- Adding an option to print binary representation.
- Enhancing documentation for better code understanding.

## Contributing

Contributions to the Subnetting Program are welcome! If you would like to contribute, please follow these steps:

1. Fork this repository and create a new branch for your feature or improvement.
2. Make your changes and ensure they pass the existing tests.
3. Submit a pull request with a clear explanation of your changes.

## Testing

The Subnetting Program includes a robust testing framework to ensure correct functionality and handle various input scenarios. To run the tests, follow these steps:

1. Navigate to the `tests` directory.
2. Execute the `test.sh` script to run the test cases.

## License

The Subnetting Program is released under the [MIT License](LICENSE).

## Contact

For any questions or inquiries, please contact [Your Name](mailto:your.email@example.com).
