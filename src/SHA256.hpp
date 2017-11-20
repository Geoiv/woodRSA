#include <vector>

class SHA256
{
  private:
  public:
    static const int blockSize = 512;
    SHA256();
    std::vector<std::vector<char*>> padParseInput(char* inputBinary, int l);
};