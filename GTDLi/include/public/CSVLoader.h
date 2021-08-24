#pragma once
#include "public/Core.h"
#include "private/Logger.h"
#include <fstream>

namespace GTDLi
{
    class CSVLoader
    {
    private:

        static CSVLoader* instance;

        CSVLoader() {};

    public:

        static CSVLoader* GetInstance()
        {
            if (!instance)
            {
                instance = new CSVLoader();
            }

            return instance;
        }

        std::vector<std::vector<int32_t>> LoadIntegerCSV(const std::string& path)
        {
            std::ifstream inFile(path);
            std::vector<std::vector<int32_t>> data;

            GTD_ASSERT(inFile.is_open(), "Error opening file");

            std::string line;
            int32_t val;
            // Read data, line by line
            while (std::getline(inFile, line))
            {
                std::vector<int32_t> row;
                // Create a stringstream of the current line
                std::stringstream ss(line);

                // Keep track of the current column index
                int colIdx = 0;

                // Extract each integer
                while (ss >> val)
                {
                    row.push_back(val);

                    if (ss.peek() == ',') ss.ignore();
                }

                data.push_back(row);
            }
            inFile.close();
            return data;
        }
    };
}