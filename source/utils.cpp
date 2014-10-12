#include "utils.h"

using namespace std;

void ReadAllBytes(istream& in, vector<char>& bytes){
    if (in.fail())
        return;
    
    in.seekg(0, in.end);
    size_t pos = static_cast<size_t>(in.tellg());
    bytes.resize(pos + 2); // double null terminate because flex requires it
    in.seekg(0, ios::beg);
    in.read(bytes.data(), pos);
}