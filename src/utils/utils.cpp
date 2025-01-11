#include <utils/utils.hpp>
#include <string.h>
#include <cctype>
#include <iostream>

#define elif else if

Params Utils::parseArguments(int argc, char** argv)
{
	Params args = {"", false, false, false, false, false, ""};

	args.inputFile = argv[1];

	for(int i = 1; i < argc; i++)
	{
		if(strcmp(argv[i], "-h") == 0)
		{
			args.helpInfo = true;
		}
		elif(strcmp(argv[i], "-q") == 0)
		{
			args.quiet = true;
		}
		elif(strcmp(argv[i], "-Wall") == 0)
		{
			args.showAllWarnings = true;
		}
		elif(strcmp(argv[i], "-Wextra") == 0)
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

std::string Utils::getToken(std::string code, size_t& i)
{
    bool tokenFinished = false;
    std::string token;
	for(size_t x = i; x < code.length(); x++ && i++)
	{
		switch(code[x])
		{
		case '\r':
		case '\n':
		case '\t':
			tokenFinished = true;
			continue;
		default:
			if(isspace(code[x]))
				tokenFinished = true;
			else
			{
				token += code[x];
			}
		}
		
		if(tokenFinished)
		{
		    if(!token.empty())
		        break;
		}
	}
	return token;
}