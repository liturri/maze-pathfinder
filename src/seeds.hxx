#ifndef __SEEDS_HXX__
#define __SEEDS_HXX__
#include <vector>
#include <iostream>
#include "benchmark.hxx"

// struct SeedLength
// {
//    size_t seed;
//    short length;
// };

class ReadData
{
private:
    std::vector<short> listOfLengths;

public:

    void SumarizeLengths();
    void ShowListStatus();
    void ExportLengthSeed();
    void ExportBinToFile(std::ostream &fileFD);
    void ExportTextToFile(std::ostream &fileFD);

    size_t ImportTextFromFile(std::istream &fileFD);
    size_t ImportBinFromFile(std::istream &fileFD);
};

#endif