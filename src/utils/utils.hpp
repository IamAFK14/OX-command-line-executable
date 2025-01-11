#include <string>
#include <fstream>

typedef struct {
    std::string inputFile;
    bool quiet;
    bool helpInfo;
    bool showAllWarnings;
    bool showExtraWarnings;
    bool unknownParam;
    std::string unknownParamString;
} Params;

class Utils
{
public:
    Params parseArguments(int argc, char** argv);
    std::string getLastParserError();
    bool checkArgs(Params* args);
    std::string readEntry(std::ifstream& inputFile);
    std::string getToken(std::string code, size_t& i);
private:
    std::string lastParserError = "";
};