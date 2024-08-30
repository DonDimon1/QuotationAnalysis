#ifndef QUOTESMANAGER_H
#define QUOTESMANAGER_H

#include <iostream>
#include <string>
#include <vector>

class QuotesManager
{
public:
    struct Candle;  // Хранение свечи
    struct Quote;   // Хранение котировки
    enum FieldQuote { // Перечисление полей котировки
        Time,
        Price,
        Volume
    };

    std::vector<Quote> quotes;
    std::vector<Candle> candles;
    int candleLength{};
    int smaPeriod{};

    // Конструктор
    QuotesManager(const std::string& filename, const int candleLength, const int smaPeriod);
    // Проверка валидности данных
    bool QuoteVerification(const std::string& token, Quote& quote, const FieldQuote& field);
    // Считывание исходного файла .csv
    void LoadQuotes(const std::string& filename);
    // Преобразуем секунды в ISO 8601
    std::string ConvertToISO8601(const std::string& timestamp_str);
    // Создание свечей
    void CreateCandles();
    // Подсчёт SMA
    std::vector<double> CalculateSMA() const;
    // Запись данных свеч в новый файл
    void WriteCandlesToFile(const std::string& filename) const;
    // Запись данных SMA в новый файл
    void WriteSMAtoFile(const std::string& filename) const;

};

// Хранение свечи
struct QuotesManager::Candle {
    std::string time{};
    double open{};
    double high{};
    double low{};
    double close{};
    double volume{};
};

// Хранение котировки
struct QuotesManager::Quote {
    std::string time{};
    double price{};
    double volume{};
};


#endif // QUOTESMANAGER_H
