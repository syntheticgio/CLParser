//
// Created by syntheticgio on 7/9/21.
//

#ifndef CLPARSER_CLPARSER_H
#define CLPARSER_CLPARSER_H

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <cstring>
#include <iostream>
#include <cstring>
#include <algorithm>

class CLParser {
public:
	enum ArgType {
		kNone = 0,
		kInt = 1,
		kString = 2,
		kBool = 3,
		kFloat = 4,
		kArray = 5
	};

	struct Argument {
		Argument() : required(false), help("Default help message."), arg_type(kString), default_value(false), flag_value(false), int_value(-1), float_value(-1), string_value(
				nullptr), array_value(nullptr) {}
		std::vector <std::string> long_name;
		std::vector <std::string> short_name;
		ArgType arg_type;
		std::string help;
//		std::string action;
		bool default_value;
		bool required;
		// This didn't work in a union because of the string value for some reason...
		bool flag_value;
		int int_value;
		float float_value;
		char *string_value;
		char **array_value;
	};

	CLParser() {
		current_arg_number_ = 0;
		current_arg_name_ = "";

		// Add Help Argument by default
		addArgument("help");
		addArgumentLongName("help");
		addArgumentShortName("h");
		addArgumentType(CLParser::kBool);
		addArgumentHelp("This is the built in help.  Running with this flag will print out help for all possible arguments registered.");
	}

	/**
	 * @brief Add a full argument
	 *
	 * This will add an entire argument when given the following
	 * @param long_name This is the argument long name (without the '--')
	 * @param short_name This is the argument short name (without the '-')
	 * @param arg_type
	 * @param default_val
	 * @param help
	 */
//	void addFullArgument(std::string &long_name, std::string &short_name, ArgType arg_type, void * default_val, std::string &help);
	/**
	 * @brief Add a full argument (sans short name)
	 *
	 * This adds a full argument without the short name.
	 * @param long_name This is the argument long name (without the '--')
	 * @param arg_type
	 * @param default_val
	 * @param help
	 */
//	void addFullArgument(std::string &long_name, ArgType arg_type, void * default_val, std::string &help);

	/**
	 * @brief Add a full argument (with vectors of long and short names)
	 *
	 * @param long_name
	 * @param short_name
	 * @param arg_type
	 * @param default_val
	 * @param help
	 */
//	void addFullArgument(std::vector<std::string> &long_name, std::vector<std::string> &short_name, ArgType arg_type, void * default_val, std::string &help);

	/**
	 * @brief Register argument.
	 * Register an argument with a generic argument name.
	 * @return false on failure, true on success.
	 */
	bool addArgument() {
		std::stringstream arg_name;
		arg_name << "Arg" << current_arg_number_;
		if (registered_arguments_.find(arg_name.str()) != registered_arguments_.end()) {
			std::cerr << "The argument " << arg_name.str() << " is currently in use.  NOT registering argument.";
			return false;
		}
		Argument new_arg = Argument();
		registered_arguments_[arg_name.str()] = new_arg;
		current_arg_number_++;
		current_arg_name_ = arg_name.str();
		return true;
	}

	/**
	 * @brief Registers argument.
	 * Registers a new argument and names it for reference.
	 * @param argument_name General name for the argument for future referencing
	 * @return false on failure, true on success.
	 */
	bool addArgument(const std::string& argument_name) {
		if (registered_arguments_.find(argument_name) != registered_arguments_.end()) {
			std::cerr << "The argument " << argument_name << " is currently in use.  NOT registering argument.";
			return false;
		}
		Argument new_arg = Argument();
		registered_arguments_[argument_name] = new_arg;
		current_arg_name_ = argument_name;
		return true;
	}

	/**
	 * @brief Add a long name
	 *
	 * Add an argument long name (without the '--' included).
	 * @param long_name
	 */
	bool addArgumentLongName(const std::string &long_name) {
		if (std::find(registered_arguments_[current_arg_name_].long_name.begin(),
					  registered_arguments_[current_arg_name_].long_name.end(), long_name)
					  != registered_arguments_[current_arg_name_].long_name.end()) {
			// Already present.
			std::cerr << "Long name --" << long_name << " is already present in this argument (" << current_arg_name_ << ") long name parameters.  Skipping.\n";
			return false;
		}
		registered_arguments_[current_arg_name_].long_name.push_back(long_name);
		return true;
	}

 	/**
	 *
	 * @param long_name
	 */
	bool addArgumentLongName(std::vector<std::string> &long_name) {
		bool success = true;
		for (auto &it : long_name) {
			if (std::find(registered_arguments_[current_arg_name_].long_name.begin(),
						  registered_arguments_[current_arg_name_].long_name.end(), it)
				!= registered_arguments_[current_arg_name_].long_name.end()) {
				// Already present.
				std::cerr << "Long name --" << it << " is already present in this argument (" << current_arg_name_ << ") long name parameters.  Skipping.\n";
				success = false;
			}
			registered_arguments_[current_arg_name_].long_name.push_back(it);
		}
		return success;
	}
	/**
	 *
	 * @param short_name
	 */
	bool addArgumentShortName(const std::string &short_name) {
		if (std::find(registered_arguments_[current_arg_name_].long_name.begin(),
					  registered_arguments_[current_arg_name_].long_name.end(), short_name)
			!= registered_arguments_[current_arg_name_].long_name.end()) {
			// Already present.
			std::cerr << "Short name --" << short_name << " is already present in this argument (" << current_arg_name_ << ") short name parameters.  Skipping.\n";
			return false;
		}
		registered_arguments_[current_arg_name_].short_name.push_back(short_name);
		return true;
	}

	/**
	 *
	 * @param short_name
	 */
	bool addArgumentShortName(std::vector<std::string> &short_name) {
		bool success = true;
		for (auto &it : short_name) {
			if (std::find(registered_arguments_[current_arg_name_].long_name.begin(),
						  registered_arguments_[current_arg_name_].long_name.end(), it)
				!= registered_arguments_[current_arg_name_].long_name.end()) {
				// Already present.
				std::cerr << "Short name --" << it << " is already present in this argument (" << current_arg_name_ << ") short name parameters.  Skipping.\n";
				success = false;
			}
			registered_arguments_[current_arg_name_].short_name.push_back(it);
		}
		return success;
	}
	/**
	 *
	 * @param argument_type
	 */
	void addArgumentType(ArgType argument_type) {
		registered_arguments_[current_arg_name_].arg_type = argument_type;
		if (argument_type == kString) {
			// TODO: Should we set something here?  Default doesn't make much sense
		} else if (argument_type == kBool) {
			// Default flag value is false (or OFF) if not pre-set
			if (!registered_arguments_[current_arg_name_].default_value) {
				registered_arguments_[current_arg_name_].flag_value = false;
				registered_arguments_[current_arg_name_].default_value = true;
			}
		} else if (argument_type == kArray) {
			// TODO: Set something here?  Default doesn't make much sense
		} else if (argument_type == kFloat) {
			// Set default value if not already set for FLOAT as 0.0
			if (!registered_arguments_[current_arg_name_].default_value) {
				registered_arguments_[current_arg_name_].float_value = 0.0;
				registered_arguments_[current_arg_name_].default_value = true;
			}
		} else if (argument_type == kInt) {
			// Set default value if not already set for INT as 0
			if (!registered_arguments_[current_arg_name_].default_value) {
				registered_arguments_[current_arg_name_].int_value = 0;
				registered_arguments_[current_arg_name_].default_value = true;
			}
		} else {
			// TODO: Would be an error state here I guess.  I.e. a NoneType
		}
	}

	/**
	 * @brief Set Default Value
	 *
	 * Set default value (int override).
	 * @param default_value
	 */
	void addArgumentDefaultValue(int default_value) {
		registered_arguments_[current_arg_name_].int_value = default_value;
		registered_arguments_[current_arg_name_].default_value = true;
	}
	/**
	 * @brief Set Default Value
	 *
	 * Set default value (float override).
	 * @param default_value
	 */
	void addArgumentDefaultValue(float default_value) {
		registered_arguments_[current_arg_name_].float_value = default_value;
		registered_arguments_[current_arg_name_].default_value = true;
	}

	/**
	 * @brief Set Default Value
	 *
	 * Set default value (bool override).
	 * @param default_value
	 */
	void addArgumentDefaultValue(bool default_value) {
		registered_arguments_[current_arg_name_].flag_value = default_value;
		registered_arguments_[current_arg_name_].default_value = true;
	}

	/**
	 * @brief Set Default Value
	 *
	 * Set default value (string override).
	 * @param default_value
	 */
	void addArgumentDefaultValue(char * default_value) {
		registered_arguments_[current_arg_name_].string_value = default_value;
		registered_arguments_[current_arg_name_].default_value = true;
	}

	/**
	 *
	 * @param help
	 */
	void addArgumentHelp(const std::string &help) {
		registered_arguments_[current_arg_name_].help = help;
	}

	void addArgumentRequired(bool required) {
		registered_arguments_[current_arg_name_].required = required;
	}

	/**
	 *
	 * @param set_arg_name
	 * @return True if successfully set, false if failed.
	 */
	bool setCurrentArgument(const std::string &set_arg_name) {
		if (registered_arguments_.count(set_arg_name) > 0) {
			// Found the argument, can set to current.
			current_arg_name_ = set_arg_name;
			return true;
		}
		return false;
	}

	void printArgumentHelp() {
		std::cout << "Command Line Parameter Help\n";
		std::cout << "===========================\n";
		std::cout << "Usage:\n";
		std::cout << "\t"

		for (auto const& [key, val] : registered_arguments_)
		{

			std::cout << key        // string (key)
					  << ':'
					  << val        // string's value
					  << std::endl;
		}
	}


	/**
	 * @brief Parse given arguments given argument template designed.
	 *
	 * @param argc Count of the arguments from the command line.
	 * @param argv Vector of arguments from command line.
	 */
	bool parseArguments(int argc, char** argv);

	std::map <const std::string, Argument> registered_arguments_;
	int current_arg_number_;
	std::string current_arg_name_;


};


#endif //CLPARSER_CLPARSER_H
