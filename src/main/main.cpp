#include <iostream>
#include <utils/utils.hpp>

int main(int argc, char** argv)
{
    std::cout << "OX V1.0 command-line executable" << std::endl;
    if (argc < 2)
    {
        std::cout << "Syntax: " << argv[0] << " <Input file> [flags]. use " << argv[0] << " -h for more info" << std::endl;
        return 1;
    }
    
    Utils utils;
    
    Params args = utils.parseArguments(argc, argv);
    
    if(args.unknownParam)
    {
        std::cout << "\033[31mUnknown argument \"" << args.unknownParamString << "\"\033[0m" << std::endl;
        return 2;
    }
    
    if(args.helpInfo)
    {
        std::cout << "Help page of OX command-line executable" << std::endl
                  << "Syntax: " << argv[0] << " <Input file> [flags]" << std::endl
                  << "Flags: " << std::endl
                  << "  -h: shows this page" << std::endl
                  << "  -q: shows quiet output(all code warnings are disabled)" << std::endl
                  << "  -Wall: shows all code warnings" << std::endl
                  << "  -Wextra: shows all extra code warnings" << std::endl;
        return 0;
    }
    
    if(!utils.checkArgs(&args))
    {
        std::cerr << utils.getLastParserError() << std::endl;
        return 3;
    }
    
    std::ifstream inputFile(args.inputFile);
    
    if(!inputFile.is_open())
    {
        std::cerr << "\033[31mFailed to open input file. Please check that you specified the correct path and if the file exists\033[0m" << std::endl;
        return 4;
    }
    
    std::string entry = utils.readEntry(inputFile);
    
    if(entry == "")
    {
        std::cout << utils.getLastParserError() << std::endl;
        return 5;
    }
    
    for(size_t i = 0; i < entry.length(); i++)
    {
        std::string token = utils.getToken(entry, i);
        std::cout << "Token: " << token << std::endl;
    }
    
    inputFile.close();
    return 0;
}