//
// Created by syntheticgio on 7/9/21.
//

#include "CLParser.h"


int main(int argc, char** argv) {
	CLParser clparser = CLParser();

	// Test Add Argument with const char *'s
	// Test kString type
	clparser.addArgument("input file");
	clparser.addArgumentLongName("input");
	clparser.addArgumentShortName("i");
	clparser.addArgumentType(CLParser::kString);
	clparser.addArgumentHelp("This is the help for an input file.");

	// Test Add Argument with std::strings
	std::string test_name = "output file";
	clparser.addArgument(test_name);
	std::string test_long_name = "output";
	clparser.addArgumentLongName(test_long_name);
	std::string test_short_name = "o";
	clparser.addArgumentShortName(test_short_name);
	clparser.addArgumentType(CLParser::kString);
	std::string test_help = "This is the help for an output file.";
	clparser.addArgumentHelp(test_help);

	// Test kInt type & parsing
	clparser.addArgument("number 1");
	clparser.addArgumentLongName("num1");
	clparser.addArgumentShortName("1");
	clparser.addArgumentType(CLParser::kInt);
	clparser.addArgumentHelp("This is the help for the first number.");

	clparser.addArgument("number 2");
	clparser.addArgumentLongName("num2");
	clparser.addArgumentShortName("2");
	clparser.addArgumentType(CLParser::kInt);
	clparser.addArgumentHelp("This is the help for the second number.");
	clparser.addArgumentDefaultValue(50);

	clparser.parseArguments(argc, argv);

	std::cout << "Number 1 value: " << clparser.registered_arguments_["number 1"].int_value << std::endl;
	std::cout << "Number 2 value: " << clparser.registered_arguments_["number 2"].int_value << std::endl;

	return 0;
}