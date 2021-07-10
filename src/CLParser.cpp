//
// Created by syntheticgio on 7/9/21.
//

#include "CLParser.h"

bool CLParser::parseArguments(int argc, char **argv) {
	// Loop through each argument given and match with the potential template.
	for (int i = 1; i < argc; i++) {
		CLParser::Argument *arg = nullptr;
		std::string matched_name;

		if (strncmp("--", argv[i], 2) == 0) {
			// It is a long type argument name
			// TODO: should probably make sure no additional '-' are included by user

			for (auto &it : registered_arguments_) {
				for (auto &it2 : it.second.long_name) {
					char str[it2.length() + 2];
					strcpy(str, "--");
					strcat(str, it2.c_str());
					if (strncmp(argv[i], str, it2.length()) == 0) {
						// Match
						std::cout << "MATCH - " << it2 << std::endl;
						matched_name = it2;
						arg = &registered_arguments_[it.first];
						break;
					}
				}
				if (arg) break;
			}
		} else if (strncmp("-", argv[i], 1) == 0) {
			// It is a short type argument name (long type has been excluded by above)

			for (auto &it : registered_arguments_) {
				for (auto &it2 : it.second.short_name) {
					char str[it2.length() + 2];
					strcpy(str, "--");
					strcat(str, it2.c_str());
					if (strncmp(argv[i], str, it2.length()) == 0) {
						// Match
						std::cout << "MATCH - " << it2 << std::endl;
						matched_name = it2;
						arg = &registered_arguments_[it.first];
						break;
					}
				}
				if (arg) break;
			}
		}
		if (!arg) continue; // Something odd; skip

		if (arg->arg_type == kBool) {
			// Type is a boolean so just set the flag and move on to next argument.
			arg->flag_value = true;
			continue;
		} else if (arg->arg_type == kArray) {
			i++;
			if (i >= argc) {
				std::cerr << "ERROR: Missing parameter following the " << matched_name << " argument." << std::endl
						  << std::endl;
				return false;
			}
			while (strncmp(argv[i], "-", 1) != 0) {
				// TODO: Need to add argv[i] to arg->array_value array here
				i++;
				if (i >= argc) {
					i--;
					break;
				}
			}
		} else {
			// Any other ones will require additional arguments.
			i++;
			if (i >= argc) {
				std::cerr << "ERROR: Missing parameter following the " << matched_name << " argument." << std::endl
						  << std::endl;
				return false;
			} else if (strncmp(argv[i], "-", 1) == 0) {
				std::cerr << "ERROR: Missing parameter following the " << matched_name << " argument." << std::endl
						  << std::endl;
				return false;
			} else {
				if (arg->arg_type == kString) {
					arg->string_value = argv[i];
				} else if (arg->arg_type == kInt) {
					// TODO: This doesn't catch errors here
					arg->int_value = atoi(argv[i]);
				} else if (arg->arg_type == kFloat) {
					// TODO: This doesn't catch errors here
					arg->float_value = atof(argv[i]);
				}
			}
		}
	}
	return true;
}


//void
//CLParser::addFullArgument(std::string &long_name, std::string &short_name, CLParser::ArgType arg_type, void *default_val,
//					  std::string &help) {
//
//}

