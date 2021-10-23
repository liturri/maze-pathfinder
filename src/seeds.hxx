#ifndef __SEEDS_HXX__
#define __SEEDS_HXX__
#include <vector>
#include <iostream>
#include "benchmark.hxx"


using TypeLength = int; //short;
using TypeSeed   = int;

// struct SeedLength
// {
//    TypeSeed seed;
//    TypeLength length;
// };

class MazeSeedsDB
{
private:
    std::vector<TypeSeed> listOfSeeds;

public:

    void SumarizeLengths();
    void ShowListStatus();
    void ExportLengthSeed();
    void ExportBinToFile(std::ostream &fileFD);
    void ExportTextToFile(std::ostream &fileFD);

    size_t ImportTextFromFile(std::istream &fileFD);
    size_t ImportBinFromFile(std::istream &fileFD);

};


class MazeLengthsDB
{
private:
    std::vector<TypeLength> listOfLengths;

public:
    void ExportTextToFile(std::ostream &fileFD);
    size_t ImportTextFromFile(std::istream &fileFD);

    bool IsEmpty(TypeLength length) const { return listOfLengths.size() < (size_t)length+1 || listOfLengths[length] == -1; };
    void AddItem(TypeLength length, TypeSeed seed);
};
#endif