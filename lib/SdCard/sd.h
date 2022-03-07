#ifndef SD_H
#define SD_H

#include <SdFat.h>
#include <SPI.h>
#include <data.h>
#include <config.h>
#include <global.h>

#include <Chrono.h>

class SDLogger
{
public:
    SDLogger(){};

    bool begin();
    bool logData();

private:
    // File system object.
    SdFat sd;
    // Log file.
    SdFile file;

    const int numDecimals = 2;
    // file Name logic
    const uint8_t BASE_NAME_SIZE = sizeof(FILE_BASE_NAME) - 1;
    char fileName[13] = FILE_BASE_NAME "00.csv";
    bool writeFileHeader();
};

#endif /* SD_H */
