#include <string>
#include <fstream>
#include <stdint.h>

typedef struct {
    std::string inputFile;
    bool quiet;
    bool version;
    bool helpInfo;
    bool showAllWarnings;
    bool showExtraWarnings;
    bool unknownParam;
    std::string unknownParamString;
} Params;

typedef enum IFunc{
    NON_INTERNAL            = 0x00,
    EXIT                    = 0x01,
    PRINT                   = 0x02,
} IFunc;

#define MAX_ARG_COUNT 32

typedef struct {
    int ArgType;
    std::string ArgStr;
} FuncArg;

typedef struct {
    int ArgCount;
    FuncArg Args[MAX_ARG_COUNT];
} FuncArgs;

typedef enum ValueTypes {
    INTEGER,
    STRING
} ValueTypes;

class Utils
{
public:
    Params parseArguments(int argc, char** argv);
    std::string getLastParserError();
    bool checkArgs(Params* args);
    std::string readEntry(std::ifstream& inputFile);
    std::string getToken(const std::string& input, size_t& pos);
    uint32_t checkForIFunctions(std::string Token);
    FuncArgs getFuncArgs(std::string token);
    bool isInteger(const std::string& str);
    void handlePrint(FuncArgs args);
private:
    std::string lastParserError = "";
};