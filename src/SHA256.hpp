#include <vector>

class SHA256
{
  private:

    std::vector<std::vector<char*>> padParseInput(char* inputBinary, int l);
  public:
    static const int blockSize = 512;
    SHA256();
    void hash(char* inputBinary, int l);

};