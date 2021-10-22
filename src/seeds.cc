#include "seeds.hxx"

void ReadData::SumarizeLengths()
{
    // std::sort(seedsList.begin(), seedsList.end(), compareLengths);
    Benchmark b;
    std::vector<int> seedsByLen;
    short length = 0;
    for (size_t seed = 0; seed < listOfLengths.size(); seed++)
    {
        length = listOfLengths[seed];
        if (length == -1)
            continue;
        if (length + 1 > seedsByLen.size())
        {
            seedsByLen.resize(length + 1, -1);
        }
        if (seedsByLen[length] != -1)
            continue;
        seedsByLen[length] = seed;
        // std::cout << seedInfo.length << " " << seedInfo.seed << std::endl;
    }
    std::cerr << "Extract seeds by length: " << b.checkpoint() << std::endl;
    for (length = 0; length < seedsByLen.size(); length++)
    {
        if (seedsByLen[length] == -1)
            continue;
        std::cout << length << ' ' << seedsByLen[length] << std::endl;
    }
}

// void ReadData::ExportLengthSeed()
// {
//    Benchmark b;
//    for (auto const &item : seedsList)
//    {
//       std::cout << item.length << ' ' << item.seed << '\n';
//    }
//    std::cerr << "Time: " << b.elapsed() << std::endl;
//    b.reset();
// };

void ReadData::ShowListStatus()
{
    Benchmark b;

    int holeStart = -1;
    int holeCount = 0;
    for (size_t cont = 0; cont < listOfLengths.size(); cont++)
    {
        int val = listOfLengths[cont];
        if (holeStart == -1 && val != -1)
        {
            continue;
        };
        if (holeStart != -1 && val != -1)
        {
            std::cerr << "Missing from: " << holeStart << " to " << cont - 1 << '\n';
            holeStart = -1;
            continue;
        }
        if (holeStart == -1 && val == -1)
        {
            holeCount++;
            holeStart = cont;
            continue;
        }
        if (holeStart != -1 && val == -1)
        {
            holeCount++;
        }
    }
    std::cerr << "List Size: " << listOfLengths.size() << " Holes: " << holeCount << std::endl;
    std::cerr << " Time: " << b.elapsed() << std::endl;
    b.reset();
}

void ReadData::ExportBinToFile(std::ostream &fileFD)
{
    Benchmark b;
    size_t seed;
    for (seed = 0; seed < listOfLengths.size(); seed++)
    {
        fileFD.write(reinterpret_cast<char *>(&listOfLengths[seed]), sizeof(listOfLengths[0]));
    };
    std::cerr << "Time: " << b.elapsed() << " Export Binary items: " << seed << '\n';
}

void ReadData::ExportTextToFile(std::ostream &fileFD)
{
    Benchmark b;
    size_t seed;
    for (seed = 0; seed < listOfLengths.size(); seed++)
    {
        if (listOfLengths[seed] == -1)
            continue;
        fileFD << listOfLengths[seed] << ' ' << seed << '\n';
    };
    std::cerr << "Time: " << b.elapsed() << " Export text items: " << seed << '\n';
}

size_t ReadData::ImportTextFromFile(std::istream &fileFD)
{
    Benchmark b;
    size_t seed;
    short length;
    size_t cant = 0;

    while (fileFD >> length >> seed)
    {
        if (seed + 1 >= listOfLengths.size())
        {
            // std::cout << "Seed: " << data.GetSeed() << "  Len: " << data.GetLength() << "  ListSize: " << lengthWithSeeds.size() << "  List Capacity: " << lengthWithSeeds.capacity() << std::endl;
            listOfLengths.resize(seed + 1, -1);
        }
        if (listOfLengths[seed] != -1)
            continue;
        cant++;
        listOfLengths[seed] = length;
    };
    std::cerr << "Time: " << b.elapsed() << " Import Text items: " << cant << '\n';
    return cant;
}

size_t ReadData::ImportBinFromFile(std::istream &fileFD)
{
    Benchmark b;
    size_t seed = 0;
    short length;
    size_t cant = 0;
    while (fileFD.read(reinterpret_cast<char *>(&length), sizeof(length)))
    {
        if (length == -1)
            continue;
        if (seed + 1 >= listOfLengths.size())
        {
            // std::cout << "Seed: " << data.GetSeed() << "  Len: " << data.GetLength() << "  ListSize: " << lengthWithSeeds.size() << "  List Capacity: " << lengthWithSeeds.capacity() << std::endl;
            listOfLengths.resize(seed + 1, -1);
        }
        if (listOfLengths[seed] == -1)
        {
            listOfLengths[seed] = length;
            cant++;
        };
        seed++;
    };
    std::cerr << "Time: " << b.elapsed() << " Import binary items: " << cant << '\n';
    return cant;
}

// bool compareSeeds(const SeedLength &a, const SeedLength &b)
// {
//    return a.seed < b.seed;
// }

// bool compareLengths(const SeedLength &a, const SeedLength &b)
// {
//    return a.length < b.length;
// }
