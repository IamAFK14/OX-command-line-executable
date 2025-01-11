#include <utils/utils.hpp>
#include <string.h>
#include <cctype>
#include <iostream>
#include <sstream>
#include <algorithm>

Params Utils::parseArguments(int argc, char** argv)
{
	Params args = {"", false, false, false, false, false, false, ""};

	args.inputFile = argv[1];

	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-h") == 0)
		{
			args.helpInfo = true;
            break;
		}
        else if(strcmp(argv[i], "-v") == 0)
        {
            args.version = true;
            break;
        }
		else if(strcmp(argv[i], "-q") == 0)
		{
			args.quiet = true;
		}
		else if(strcmp(argv[i], "-Wall") == 0)
		{
			args.showAllWarnings = true;
		}
		else if(strcmp(argv[i], "-Wextra") == 0)
		{
			args.showExtraWarnings = true;
		}
		else if(i != 1)
		{
			args.unknownParam = true;
			args.unknownParamString = argv[i];
			break;
		}
	}

	return args;
}

std::string Utils::getLastParserError()
{
	return lastParserError;
}

bool Utils::checkArgs(Params* args)
{
	if(args->quiet)
	{
		if(args->showAllWarnings)
		{
			lastParserError = "\033[31mParser error: invalid argument combination.\r\nCannot combine -q with -Wall\033[0m";
			return false;
		}
		if(args->showExtraWarnings)
		{
			lastParserError = "\033[31mParser error: invalid argument combination.\r\nCannot combine -q with -Wextra\033[0m";
			return false;
		}
	}

	return true;
}

std::string Utils::readEntry(std::ifstream& inputFile)
{
	std::string entryFunction;
	std::string line;
	std::string token = "";
	bool tokenFinished = false;
	bool entryFound = false;
	int bracesOpened = 0;
	bool atLeastOneBraceFound = false;
	bool entryValid = false;
	while(std::getline(inputFile, line))
	{
		for(size_t i = 0; i < line.length(); i++)
		{
			if(!entryFound)
			{
				switch(line[i])
				{
				case '\r':
				case '\n':
				case '\t':
					tokenFinished = true;
					continue;
				default:
					if(isspace(line[i]))
						tokenFinished = true;
					else
					{
						token += line[i];
					}
				}
			}
			if(!entryFound && tokenFinished)
			{
				if(!token.empty())
				{
					if(token == "entry")
					{
						entryFound = true;
					}
				}
				token = "";
				tokenFinished = false;
			}
			if(entryFound)
			{
				switch(line[i])
				{
				case '{':
				{
					bracesOpened++;
					if(!atLeastOneBraceFound)
					{
						atLeastOneBraceFound = true;
					}
					break;
				}
				case '}':
					bracesOpened--;
				}

				if(atLeastOneBraceFound && !entryValid)
				{
					entryValid = true;
					continue;
				}
				if(bracesOpened == 0 && atLeastOneBraceFound)
				{
					return entryFunction;
				}

				if(bracesOpened != 0)
				{
					entryFunction += line[i];
				}
			}
		}

	}
	if(!entryValid)
	{
		lastParserError = "\033[31mIncorrect entry function syntax. please check if your entry function is correct\033[0m";
		return "";
	}

	if(!entryFound)
	{
		lastParserError = "\033[31mFailed to find the entry function. please check if the entry function exists in the specified input file\033[0m";
		return "";
	}

	return entryFunction;
}

std::string Utils::getToken(const std::string& input, size_t& pos) {
    std::string token;
    bool inQuotes = false;
    bool inParentheses = false;
    
    while (pos < input.size()) {
        char currentChar = input[pos];
        
        // Handle start of a string literal
        if (currentChar == '"' && !inParentheses) {
            if (inQuotes) {
                token += currentChar;  // Add the closing quote
                pos++;
                break;
            } else {
                inQuotes = true;
                token += currentChar;  // Add the opening quote
                pos++;
                continue;
            }
        }

        // Handle parentheses
        if (currentChar == '(') {
            inParentheses = true;
            token += currentChar;
            pos++;
            continue;
        }

        if (currentChar == ')') {
            inParentheses = false;
            token += currentChar;
            pos++;
            continue;
        }

        // If inside parentheses or quotes, keep adding characters
        if (inParentheses || inQuotes) {
            token += currentChar;
            pos++;
        } else if (!isspace(currentChar)) {  // Skip spaces outside parentheses
            token += currentChar;
            pos++;
        } else {  // Break on space outside quotes or parentheses
            if (!token.empty()) {
                break;
            } else {
                pos++;
            }
        }
    }

    return token;
}

#define NON_INTERNAL 0

uint32_t INTERNAL_FUNCTIONS[] = {
    0, // not internal function
    1, // EXIT function
    2, // PRINT function
    
};

std::string InternalFunctions[] = {
    "exit",
    "print"
};

uint32_t Utils::checkForIFunctions(std::string Token)
{
    std::string Name;
    bool validName = false;
    for(size_t i = 0; i < Token.length(); i++)
    {
        if(Token[i] == '(')
        {
            validName = true;
            break;
        }
        Name += Token[i];
    }

    if(!validName) return INTERNAL_FUNCTIONS[NON_INTERNAL];

    size_t size = sizeof(InternalFunctions) / sizeof(InternalFunctions[0]);

    for(size_t i = 0; i < size; i++)
    {
        if(InternalFunctions[i] == Name)
        {
            return INTERNAL_FUNCTIONS[i + 1];
        }
    }

    return INTERNAL_FUNCTIONS[NON_INTERNAL];
}

bool Utils::isInteger(const std::string& str)
{
    std::string trimmed = str;
    trimmed.erase(trimmed.find_last_not_of(" \t\r\n") + 1);
    try {
        std::stoi(trimmed);  // Attempt to convert to integer
        return true;          // Conversion successful
    } catch (const std::invalid_argument& e) {
        return false;         // Not a valid integer
    } catch (const std::out_of_range& e) {
        return false;         // Value out of range
    }
}


FuncArgs Utils::getFuncArgs(std::string token)
{
    FuncArgs args = {};
    size_t x = 0;
    bool finished = false;
    bool inQuotes = false;
    std::string currentArg;

    // Skip function name part until '('
    while(x < token.length() && token[x] != '(')
    {
        x++;
    }

    if(x == token.length()) return args; // No arguments if no '(' is found

    x++; // Skip the '(' character
    while(x < token.length() && !finished)
    {
        // Handle spaces and commas
        if((isspace(token[x]) || token[x] == ',') && !inQuotes)
        {
            if(!currentArg.empty())
            {
                args.Args[args.ArgCount].ArgStr = currentArg;
                args.Args[args.ArgCount].ArgType = isInteger(currentArg) ? INTEGER : STRING;
                args.ArgCount++;
                currentArg.clear();
            }
        }
        // Handle closing parenthesis
        else if(token[x] == ')')
        {
            finished = true;
        }
        // Handle quotes (strings)
        else if(token[x] == '"')
        {
            if(inQuotes) {
                currentArg += token[x];  // Add closing quote
                inQuotes = false;        // Close the string
            }
            else {
                inQuotes = true;         // Open the string
                currentArg += token[x];  // Add opening quote
            }
        }
        else
        {
            // Add character to current argument (normal characters)
            currentArg += token[x];
        }

        x++;
    }

    // Add the last argument if any
    if(!currentArg.empty())
    {
        args.Args[args.ArgCount].ArgStr = currentArg;
        args.Args[args.ArgCount].ArgType = isInteger(currentArg) ? INTEGER : STRING;
        args.ArgCount++;
    }

    return args;
}

#include <iostream>
#include <string>
#include <algorithm>

void Utils::handlePrint(FuncArgs args)
{
    // Check if there are arguments to print
    if (args.ArgCount > 0) {
        for (int i = 0; i < args.ArgCount; i++) {
            std::string argument = args.Args[i].ArgStr;

            // Remove surrounding quotes (if any)
            if (argument.size() >= 2 && 
               ((argument.front() == '"' && argument.back() == '"') || 
                (argument.front() == '\'' && argument.back() == '\''))) {
                argument = argument.substr(1, argument.size() - 2);
            }

            // Process escape sequences
            std::string processed;
            for (size_t j = 0; j < argument.size(); ++j) {
                if (argument[j] == '\\' && j + 1 < argument.size()) {
                    switch (argument[j + 1]) {
                        case 'n': processed += '\n'; break;
                        case 'r': processed += '\r'; break;
                        case 't': processed += '\t'; break;
                        case '\\': processed += '\\'; break;
                        case '"': processed += '"'; break;
                        case '\'': processed += '\''; break;
                        default: processed += '\\'; processed += argument[j + 1]; break;
                    }
                    ++j; // Skip the escaped character
                } else {
                    processed += argument[j];
                }
            }

            // Print the processed argument
            std::cout << processed;

            // Add a space after each argument, but not after the last one
            if (i < args.ArgCount - 1) {
                std::cout << " ";
            }
        }
    }
}
