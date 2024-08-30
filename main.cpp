#include "quotesmanager.h"
#include <iostream>

int main()
{
    setlocale(LC_ALL, "Russian");
    setlocale(LC_NUMERIC, "eng");

    std::string inputFilename = "ETHUSDT_1.csv";    // Нзавание исходного файла
    std::string candleFilename = "candles.csv";     // Название файла с данными о свечах
    std::string smaFilename = "sma.csv";            // Название файла с данными SMA
    int candleLength = 5;                           // Длинна свечи.
    int smaPeriod = 10;                             // Период SMA

    std::cout << "Введите длинну свечи" << std::endl;
    std::cin >> candleLength;
    std::cout << "Введите период SMA" << std::endl;
    std::cin >> smaPeriod;

    QuotesManager manager(inputFilename, candleLength, smaPeriod);
    manager.WriteCandlesToFile(candleFilename);
    manager.WriteSMAtoFile(smaFilename);

    return 0;
}
