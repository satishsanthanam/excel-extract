
// standard includes
#include <cstddef>
#include <fstream>
#include <ios>
#include <ostream>
#include <exception>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

void formCsvFilesLevel1();
void formCsvFilesLevel2();
void formCsvFIlesLevel3();

const int totalKhandas = 6;
const string khandas[] = { "baala", "ayodhya", "aranya", "kishkindha", "sundara", "yuddha"};
const int totalSargas[] = { 77, 119, 75, 67, 68, 128};
const string khandaFileName[] = { "balasans", "ayodhyasans", "aranyasans", "kishkindhasans", "sundarasans", "yuddhasans"};

int main (void) {

    formCsvFilesLevel1();
    formCsvFilesLevel2();
    formCsvFIlesLevel3();

    return 0;
}

void formCsvFilesLevel1()
{
    std::string htmlTagsToBeSearched[] = {"&quot;", "&nbsp;"};
                
    for (size_t khandaNo = 0; khandaNo < totalKhandas; ++khandaNo)
    {
        std::cout << "Processing Khanda : " << khandas[khandaNo] << std:: endl;
        for (int sargaNo = 1; sargaNo <= totalSargas[khandaNo]; ++sargaNo)
        {
            try {
                std::ifstream inputStream;
                ofstream outputStream;
                string khandaPath = khandas[khandaNo] + "/" + khandaFileName[khandaNo];
                char sargaNoFmt[10];
                sprintf(sargaNoFmt, "%03d", sargaNo);
                string inputFileName = "./resources/ramayan/" + khandaPath + to_string(sargaNo) + ".htm";
                string outputFileName = "./resources/ramayan-ext-1/" + khandaPath + sargaNoFmt + ".txt";
                inputStream.open(inputFileName);
                
            
                std::string inputLine; 
                std::string fullFileAsString;
                std::string extractedString;
                std::string extractedAndTrimmedString;
                if (inputStream.is_open())
                {
                    while (inputStream)
                    {
                        std::getline (inputStream, inputLine);
                        fullFileAsString.append(inputLine);
                    }
                    inputStream.close();
                }
                else  {
                    continue;
                }
                
                outputStream.open(outputFileName, std::_S_trunc);
                std::size_t anchor = 0;
                std::size_t verseNo = 1;
                do 
                {
                    std::size_t i = fullFileAsString.find("class=\"tat\"", anchor);
                    if (i == std::string::npos)
                        break;

                    std::size_t n = fullFileAsString.find("</p>", i);
                    if (n == std::string::npos)
                        break;
        
                    std::size_t size = n - i - 12;
                    extractedString =  fullFileAsString.substr(i+12, size);

                    for (size_t noOfTags = 0; noOfTags < 2; ++noOfTags) {
                        size_t hashTagPos= 0;
                        do {
                            hashTagPos = extractedString.find(htmlTagsToBeSearched[noOfTags], hashTagPos);
                            if (hashTagPos != std::string::npos)
                            {
                                extractedString.erase(hashTagPos, 6);
                            }
                        } while (hashTagPos != std::string::npos);
                    }
                    outputStream << to_string(khandaNo+1) << "~" << to_string(sargaNo) <<  "~"  << to_string(verseNo++) << "~" << extractedString << std::endl;
                    anchor = n;
                } while (anchor != std::string::npos);
                outputStream.close();
            } catch (const std::exception &e) {
                std::cerr << e.what() << "\n";
                return;
            } //end try
        } //End of each sarga
    } //end of all khandas
}

void formCsvFilesLevel2()
{
   for (size_t khandaNo = 0; khandaNo < totalKhandas; ++khandaNo)
    {
        std::cout << "Processing Khanda : " << khandas[khandaNo] << std:: endl;
        ofstream khandaFileStream;
        string khandaFileNamePart = khandas[khandaNo] + "/" + khandaFileName[khandaNo];
        string khandaFileName = "./resources/ramayan-ext-1/" + khandaFileNamePart + ".txt";

        std::cout << "concatenating Khanda" <<  khandaFileNamePart << std::endl;       
        for (int sargaNo = 1; sargaNo <= totalSargas[khandaNo]; ++sargaNo)
        {
            try {
                char sargaNoFmt[10];
                sprintf(sargaNoFmt, "%03d", sargaNo);
                string inputFileName = "./resources/ramayan-ext-1/" + khandaFileNamePart + std::string(sargaNoFmt) + ".txt";
                string commandStr = "cat " + inputFileName + " >>" +  khandaFileName;
                //std::cout << "commadn Str is "  << commandStr << std::endl;
                system(commandStr.c_str());
            }
            catch (exception e)
            {

            }
        }
    }
}

void formCsvFIlesLevel3()
{
   std::cout << "Final concatenation" << std::endl;
   for (size_t khandaNo = 0; khandaNo < totalKhandas; ++khandaNo)
    {
        ofstream khandaFileStream;
        string khandaFileNamePart = khandas[khandaNo] + "/" + khandaFileName[khandaNo];
        string khandaFileName = "./resources/ramayan-ext-1/" + khandaFileNamePart + ".txt";
        string commandStr = "cat " + khandaFileName +  " >> ./resources/ramayan-ext-1/ramayan.txt";
        system(commandStr.c_str());
    }
}
