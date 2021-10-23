#include "seeds.hxx"


void MazeSeedsDB::SumarizeLengths()
{
    // std::sort(seedsList.begin(), seedsList.end(), compareLengths);
    Benchmark b;
    std::vector<int> seedsByLen;
    TypeLength length = 0;
    for (TypeSeed seed = 0; (size_t)seed < listOfSeeds.size(); seed++)
    {
        length = listOfSeeds[seed];
        if (length == -1)
            continue;
        if ((size_t)length + 1 > seedsByLen.size())
        {
            seedsByLen.resize(length + 1, -1);
        }
        if (seedsByLen[length] != -1)
            continue;
        seedsByLen[length] = seed;
        // std::cout << seedInfo.length << " " << seedInfo.seed << std::endl;
    }
    std::cerr << "Extract seeds by length: " << b.checkpoint() << std::endl;
    for (int cant=0,length = 0; cant < 10 && (size_t)length < seedsByLen.size(); length++)
    {
        if (seedsByLen[length] == -1)
            continue;
        cant++;
        std::cout << length << ' ' << seedsByLen[length] << std::endl;
    }
    std::cout << '\n';
    for (int cant=0,length = seedsByLen.size(); cant < 10 && length > 0 ; length--)
    {
        if (seedsByLen[length] == -1)
            continue;
        cant++;
        std::cout << length << ' ' << seedsByLen[length] << std::endl;
    }
}

// void MazeSeedsDB::ExportLengthSeed()
// {
//    Benchmark b;
//    for (auto const &item : seedsList)
//    {
//       std::cout << item.length << ' ' << item.seed << '\n';
//    }
//    std::cerr << "Time: " << b.elapsed() << std::endl;
//    b.reset();
// };

void MazeSeedsDB::ShowListStatus()
{
    Benchmark b;

    int holeStart = -1;
    int holeCount = 0;
    for (size_t cont = 0; cont < listOfSeeds.size(); cont++)
    {
        int val = listOfSeeds[cont];
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
    std::cerr << "List Size: " << listOfSeeds.size() << " Holes: " << holeCount << std::endl;
    std::cerr << " Time: " << b.elapsed() << std::endl;
    b.reset();
}

void MazeSeedsDB::ExportBinToFile(std::ostream &fileFD)
{
    Benchmark b;
    size_t seed;
    for (seed = 0; seed < listOfSeeds.size(); seed++)
    {
        fileFD.write(reinterpret_cast<char *>(&listOfSeeds[seed]), sizeof(listOfSeeds[0]));
    };
    std::cerr << "Time: " << b.elapsed() << " Export Binary items: " << seed << '\n';
}

void MazeSeedsDB::ExportTextToFile(std::ostream &fileFD)
{
    Benchmark b;
    size_t seed;
    for (seed = 0; seed < listOfSeeds.size(); seed++)
    {
        if (listOfSeeds[seed] == -1)
            continue;
        fileFD << listOfSeeds[seed] << ' ' << seed << '\n';
    };
    std::cerr << "Time: " << b.elapsed() << " Export text items: " << seed << '\n';
}

size_t MazeSeedsDB::ImportTextFromFile(std::istream &fileFD)
{
    Benchmark b;
    TypeSeed seed;
    TypeLength length;
    size_t cant = 0;

    while (fileFD >> length >> seed)
    {
        if ((size_t)seed + 1 >= listOfSeeds.size())
        {
            // std::cout << "Seed: " << data.GetSeed() << "  Len: " << data.GetLength() << "  ListSize: " << lengthWithSeeds.size() << "  List Capacity: " << lengthWithSeeds.capacity() << std::endl;
            listOfSeeds.resize(seed + 1, -1);
        }
        if (listOfSeeds[seed] != -1)
            continue;
        cant++;
        listOfSeeds[seed] = length;
    };
    std::cerr << "Time: " << b.elapsed() << " Import Text items: " << cant << '\n';
    return cant;
}

size_t MazeSeedsDB::ImportBinFromFile(std::istream &fileFD)
{
    Benchmark b;
    TypeSeed seed = 0;
    TypeLength length;
    size_t cant = 0;
    while (fileFD.read(reinterpret_cast<char *>(&length), sizeof(length)))
    {
        if (length == -1)
            continue;
        if ((size_t)seed + 1 >= listOfSeeds.size())
        {
            // std::cout << "Seed: " << data.GetSeed() << "  Len: " << data.GetLength() << "  ListSize: " << lengthWithSeeds.size() << "  List Capacity: " << lengthWithSeeds.capacity() << std::endl;
            listOfSeeds.resize(seed + 1, -1);
        }
        if (listOfSeeds[seed] == -1)
        {
            listOfSeeds[seed] = length;
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

size_t MazeLengthsDB::ImportTextFromFile(std::istream &fileFD)
{
    Benchmark b;
    TypeSeed seed;
    TypeLength length;
    size_t cant = 0;

    while (fileFD >> seed >> length)
    {
        if ((size_t)length + 1 >= listOfLengths.size())
        {
            // std::cout << "Seed: " << data.GetSeed() << "  Len: " << data.GetLength() << "  ListSize: " << lengthWithSeeds.size() << "  List Capacity: " << lengthWithSeeds.capacity() << std::endl;
            listOfLengths.resize(length + 1, -1);
        }
        if (listOfLengths[length] != -1)
            continue;
        cant++;
        listOfLengths[length] = seed;
    };
    std::cerr << "Time: " << b.elapsed() << " Import Text items: " << cant << '\n';
    return cant;
}

void MazeLengthsDB::ExportTextToFile(std::ostream &fileFD)
{
    Benchmark b;
    TypeLength length;
    for (length = 0; (size_t)length < listOfLengths.size(); length++)
    {
        if (listOfLengths[length] == -1)
            continue;
        fileFD << listOfLengths[length] << ' ' << length << '\n';
    };
    std::cerr << "Time: " << b.elapsed() << " Export text items: " << length << '\n';
}

void MazeLengthsDB::AddItem(TypeLength length, TypeSeed seed)
{
        if ((size_t)length + 1 >= listOfLengths.size())
        {
            listOfLengths.resize(length + 1, -1);
        }
        listOfLengths[length] = seed;
}
