#include <map>
#include <vector>
#include <iostream>

#define DEFAULTSTRING "ASJGDKASJDGKAJSDGKLASJDKLGJ"

class CommandLine
{
private:
   std::string Self;
   std::map<std::string, std::string> Arguments;
   std::vector<std::string> ExtraArguments;
public:
   CommandLine(int argc, char *argv[]);
   ~CommandLine();
   void Initialize(int argc, char *argv[]);
   std::string operator [](int Index);
   std::string operator [](std::string Key);
   std::string Get(int Index);
   std::string Get(std::string Key);
   std::string Get(int Index, std::string Default);
   std::string Get(std::string Key, std::string Default);
   std::vector<std::string> GetStringVector(int Index, char Delimiter = ',');
   std::vector<std::string> GetStringVector(std::string Key, char Delimiter = ',');
   std::vector<std::string> GetStringVector(int Index, std::string Default, char Delimiter = ',');
   std::vector<std::string> GetStringVector(std::string Key, std::string Default, char Delimiter = ',');
   std::vector<std::string> GetStringVector(int Index, std::vector<std::string> Default, char Delimiter = ',');
   std::vector<std::string> GetStringVector(std::string Key, std::vector<std::string> Default, char Delimiter = ',');
   int GetInt(int Index);
   int GetInt(std::string Key);
   int GetInt(int Index, int Default);
   int GetInt(std::string Key, int Default);
   std::vector<int> GetIntVector(int Index, char Delimiter = ',');
   std::vector<int> GetIntVector(std::string Key, char Delimiter = ',');
   std::vector<int> GetIntVector(int Index, std::string Default, char Delimiter = ',');
   std::vector<int> GetIntVector(std::string Key, std::string Default, char Delimiter = ',');
   std::vector<int> GetIntVector(int Index, std::vector<int> Default, char Delimiter = ',');
   std::vector<int> GetIntVector(std::string Key, std::vector<int> Default, char Delimiter = ',');
   double GetDouble(int Index);
   double GetDouble(std::string Key);
   double GetDouble(int Index, double Default);
   double GetDouble(std::string Key, double Default);
   std::vector<double> GetDoubleVector(int Index, char Delimiter = ',');
   std::vector<double> GetDoubleVector(std::string Key, char Delimiter = ',');
   std::vector<double> GetDoubleVector(int Index, std::string Default, char Delimiter = ',');
   std::vector<double> GetDoubleVector(std::string Key, std::string Default, char Delimiter = ',');
   std::vector<double> GetDoubleVector(int Index, std::vector<double> Default, char Delimiter = ',');
   std::vector<double> GetDoubleVector(std::string Key, std::vector<double> Default, char Delimiter = ',');
   bool GetBool(int Index);
   bool GetBool(std::string Key);
   bool GetBool(int Index, bool Default);
   bool GetBool(std::string Key, bool Default);
   bool StringToBool(string String);
   std::string GetSelf();
   std::vector<std::string> Parse(std::string Input, char Delimiter = ',');
   std::vector<int> ParseInt(std::string Input, char Delimiter = ',');
   std::vector<double> ParseDouble(std::string Input, char Delimiter = ',');
};

CommandLine::CommandLine(int argc, char *argv[])
{
   Initialize(argc, argv);
}

CommandLine::~CommandLine()
{
}

void CommandLine::Initialize(int argc, char *argv[])
{
   Self = "";
   Arguments.clear();
   ExtraArguments.clear();

   if(argc < 1)
      return;

   Self = argv[0];

   for(int i = 1; i < argc; i++)
   {
      if(argv[i][0] != '-')
         ExtraArguments.push_back(argv[i]);
      else
      {
         bool ForceNext = false;
         if(argv[i][1] == '-')   // note since [0] is '-', there must be a [1]: '\0' for short string
            ForceNext = true;

         std::string Key = argv[i];
         while(Key.size() > 0 && Key[0] == '-')
            Key.erase(Key.begin());

         if(ForceNext == false && (i + 1 >= argc || argv[i+1][0] == '-'))
            Arguments.insert(std::pair<std::string, std::string>(Key, "1"));
         else
         {
            Arguments.insert(std::pair<std::string, std::string>(Key, argv[i+1]));
            i = i + 1;
         }
      }
   }
}

std::string CommandLine::operator [](int Index)
{
   return Get(Index);
}

std::string CommandLine::operator [](std::string Key)
{
   return Get(Key);
}

std::string CommandLine::Get(int Index)
{
   if(Index < 0 || Index >= ExtraArguments.size())
   {
      std::cerr << "Exception: argument index out of range" << std::endl;
      throw;
   }
   
   return ExtraArguments[Index];
}
   
std::string CommandLine::Get(std::string Key)
{
   if(Arguments.find(Key) == Arguments.end())
   {
      std::cerr << "Exception: Key \"" << Key << "\" not found in arguments" << std::endl;
      throw;
   }

   return Arguments[Key];
}

std::string CommandLine::Get(int Index, std::string Default)
{
   if(Index < 0 || Index >= ExtraArguments.size())
      return Default;
   
   return ExtraArguments[Index];
}
   
std::string CommandLine::Get(std::string Key, std::string Default)
{
   if(Arguments.find(Key) == Arguments.end())
      return Default;

   return Arguments[Key];
}

std::vector<std::string> CommandLine::GetStringVector(int Index, char Delimiter)
{
   return Parse(Get(Index), Delimiter);
}

std::vector<std::string> CommandLine::GetStringVector(std::string Key, char Delimiter)
{
   return Parse(Get(Key), Delimiter);
}

std::vector<std::string> CommandLine::GetStringVector(int Index, std::string Default, char Delimiter)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Parse(Default, Delimiter);
   return Parse(Result, Delimiter);
}

std::vector<std::string> CommandLine::GetStringVector(std::string Key, std::string Default, char Delimiter)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Parse(Default, Delimiter);
   return Parse(Result, Delimiter);
}

std::vector<std::string> CommandLine::GetStringVector(int Index, std::vector<std::string> Default, char Delimiter)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return Parse(Result, Delimiter);
}

std::vector<std::string> CommandLine::GetStringVector(std::string Key, std::vector<std::string> Default, char Delimiter)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return Parse(Result, Delimiter);
}

int CommandLine::GetInt(int Index)
{
   return atoi(Get(Index).c_str());
}

int CommandLine::GetInt(std::string Key)
{
   return atoi(Get(Key).c_str());
}

int CommandLine::GetInt(int Index, int Default)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return atoi(Result.c_str());
}

int CommandLine::GetInt(std::string Key, int Default)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return atoi(Result.c_str());
}

std::vector<int> CommandLine::GetIntVector(int Index, char Delimiter)
{
   return ParseInt(Get(Index), Delimiter);
}

std::vector<int> CommandLine::GetIntVector(std::string Key, char Delimiter)
{
   return ParseInt(Get(Key), Delimiter);
}

std::vector<int> CommandLine::GetIntVector(int Index, std::string Default, char Delimiter)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return ParseInt(Default, Delimiter);
   return ParseInt(Result, Delimiter);
}

std::vector<int> CommandLine::GetIntVector(std::string Key, std::string Default, char Delimiter)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return ParseInt(Default, Delimiter);
   return ParseInt(Result, Delimiter);
}

std::vector<int> CommandLine::GetIntVector(int Index, std::vector<int> Default, char Delimiter)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return ParseInt(Result, Delimiter);
}

std::vector<int> CommandLine::GetIntVector(std::string Key, std::vector<int> Default, char Delimiter)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return ParseInt(Result, Delimiter);
}

double CommandLine::GetDouble(int Index)
{
   return atof(Get(Index).c_str());
}

double CommandLine::GetDouble(std::string Key)
{
   return atof(Get(Key).c_str());
}

double CommandLine::GetDouble(int Index, double Default)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return atof(Result.c_str());
}

double CommandLine::GetDouble(std::string Key, double Default)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return atof(Result.c_str());
}

std::vector<double> CommandLine::GetDoubleVector(int Index, char Delimiter)
{
   return ParseDouble(Get(Index), Delimiter);
}

std::vector<double> CommandLine::GetDoubleVector(std::string Key, char Delimiter)
{
   return ParseDouble(Get(Key), Delimiter);
}

std::vector<double> CommandLine::GetDoubleVector(int Index, std::string Default, char Delimiter)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return ParseDouble(Default, Delimiter);
   return ParseDouble(Result, Delimiter);
}

std::vector<double> CommandLine::GetDoubleVector(std::string Key, std::string Default, char Delimiter)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return ParseDouble(Default, Delimiter);
   return ParseDouble(Result, Delimiter);
}

std::vector<double> CommandLine::GetDoubleVector(int Index, std::vector<double> Default, char Delimiter)
{
   std::string Result = Get(Index, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return ParseDouble(Result, Delimiter);
}

std::vector<double> CommandLine::GetDoubleVector(std::string Key, std::vector<double> Default, char Delimiter)
{
   std::string Result = Get(Key, DEFAULTSTRING);
   if(Result == DEFAULTSTRING)
      return Default;
   return ParseDouble(Result, Delimiter);
}

bool CommandLine::GetBool(int Index)
{
   string String = Get(Index);
   return StringToBool(String);
}

bool CommandLine::GetBool(std::string Key)
{
   string String = Get(Key);
   return StringToBool(String);
}

bool CommandLine::GetBool(int Index, bool Default)
{
   std::string String = Get(Index, (Default ? "true" : "false"));
   return StringToBool(String);
}

bool CommandLine::GetBool(std::string Key, bool Default)
{
   std::string String = Get(Key, (Default ? "true" : "false"));
   return StringToBool(String);
}

bool CommandLine::StringToBool(string String)
{
   if(String == "1")   return true;
   if(String == "0")   return false;

   if(String == "true")    return true;
   if(String == "false")   return false;

   if(String == "True")    return true;
   if(String == "False")   return false;

   if(String == "TRUE")    return true;
   if(String == "FALSE")   return false;

   std::cerr << "Exception: boolean value \"" << String << "\" not recognized." << std::endl;
   throw;
   
   return false;
}

std::string CommandLine::GetSelf()
{
   return Self;
}

std::vector<std::string> CommandLine::Parse(std::string  Input, char Delimiter)
{
   std::vector<std::string> Result;

   int Start = 0;
   for(int i = 0; i < (int)Input.size(); i++)
   {
      if(Input[i] == Delimiter)
      {
         Result.push_back(Input.substr(Start, i - Start));
         Start = i + 1;
      }
   }
   if(Start != Input.size())
      Result.push_back(Input.substr(Start));

   return Result;
}

std::vector<int> CommandLine::ParseInt(std::string Input, char Delimiter)
{
   std::vector<std::string> ResultString = Parse(Input, Delimiter);

   std::vector<int> Result;

   for(std::string Item : ResultString)
      Result.push_back(atoi(Item.c_str()));

   return Result;
}

std::vector<double> CommandLine::ParseDouble(std::string Input, char Delimiter)
{
   std::vector<std::string> ResultString = Parse(Input, Delimiter);

   std::vector<double> Result;

   for(std::string Item : ResultString)
      Result.push_back(atof(Item.c_str()));

   return Result;
}

