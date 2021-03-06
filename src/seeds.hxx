#ifndef __SEEDS_HXX__
#define __SEEDS_HXX__
#include <iostream>
#include <vector>

using TypeLength = int; // short;
using TypeSeed = long;

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
    bool printTimming = true;

public:
    void ExportTextToFile(std::ostream &fileFD);
    size_t ImportTextFromFile(std::istream &fileFD);

    bool IsEmpty(TypeLength length) const { return listOfLengths.size() < (size_t)length + 1 || listOfLengths[length] == -1; };
    void AddItem(TypeLength length, TypeSeed seed);
    void DisableDebug() { printTimming = false; };
};
#endif