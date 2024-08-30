#include "quotesmanager.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

QuotesManager::QuotesManager(const std::string &filename, const int candleLength, const int smaPeriod)
        : candleLength(candleLength)
        , smaPeriod(smaPeriod) {
    LoadQuotes(filename);
    CreateCandles();
}

// Проверка валидности данных
bool QuotesManager::QuoteVerification(const std::string& token, Quote& quote, const FieldQuote& field) {
    if(token.empty()) {                         // Если цена пустая
        if(quotes.empty())                      // Если ещё нет непустых котировок
            return false;

        switch (field) {                        // Сверяем тип поля котировки и заполняем её предыдущим значением
        case Time:
            quote.time = quotes.back().time;
            break;
        case Price:
            quote.price = quotes.back().price;
            break;
        case Volume:
            quote.volume = quotes.back().volume;
        default:
            break;
        }
    }
    else{                                       // Если цена не пустая
        try{
            switch (field) {                    // Cверяем тип поля котировки
            case Time:
                quote.time = token;
                break;
            case Price: {
                double temp = std::stod(token); // Пробуем преобразовать std::string в double
                quote.price = temp;             // Если удачно, заполняем её новым значением
                break;
            }
            case Volume: {
                double temp = std::stod(token);
                quote.volume = temp;
                break;
            }
            default:
                break;
            }
        } catch(...) {                      // если token не преобразуется в double
            return false;
        }
    }
    return true;
};

// Считывание исходного файла .csv
void QuotesManager::LoadQuotes(const std::string& filename) {
    std::ifstream inputFile(filename);
    std::string line;

    if (!inputFile.is_open()) {
        std::cerr << "Не удалось открыть файл для чтения." << std::endl;
        std::abort();
    }


    while (std::getline(inputFile, line)) {  // В исходном файле все параметры в одной ячейке
        std::istringstream iss(line);
        std::string token;
        Quote quote;

        // Извлекаем и преобразуем числа из строки
        std::getline(iss, token, ',');          // Вычисляем время
        if(!QuoteVerification(token, quote, Time))
            continue;

        std::getline(iss, token, ',');          // Вычисляем цену
        if(!QuoteVerification(token, quote, Price))
            continue;

        std::getline(iss, token);               // Вычисляем значение
        if(!QuoteVerification(token, quote, Volume))
            continue;

        quotes.push_back(quote);                // Сохраняем котировку
    }
    inputFile.close();
}

// Преобразуем секунды в ISO 8601
std::string QuotesManager::ConvertToISO8601(const std::string& timestamp_str) {
    // Преобразуем строку в число (time_t)
    time_t timestamp = std::stoll(timestamp_str);

    // Преобразуем timestamp в структуру времени
    std::tm* tm = std::gmtime(&timestamp);

    // Форматируем в строку ISO 8601
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%dT%H:%M:%S");
    return ss.str();
}

// Создание свечей
void QuotesManager::CreateCandles() {
    // Проходим по всем котировкам
    for (size_t i = 0; i < quotes.size(); i += candleLength) {
        Candle candle;
        candle.time = ConvertToISO8601(quotes[i].time);
        candle.open = quotes[i].price;
        candle.high = quotes[i].price;
        candle.low = quotes[i].price;
        candle.close = quotes[i].price;
        candle.volume = quotes[i].volume;

        // Проходим по длине свечи
        for (size_t j = i + 1; j < i + candleLength && j < quotes.size(); ++j) {
            candle.high = std::max(candle.high, quotes[j].price);
            candle.low = std::min(candle.low, quotes[j].price);
            candle.close = quotes[j].price;
            candle.volume += quotes[j].volume;
        }
        candles.push_back(candle);
    }
}

// Подсчёт SMA
std::vector<double> QuotesManager::CalculateSMA() const {
    std::vector<double> smaValues;
    double sum = 0.0;   // Cуммарное значение цен закрытия

    for (size_t i = 0; i < candles.size(); ++i) {       // Проходим по всем свечам
        sum += candles[i].close;                        // Добавляем цену закрытия текущей свечи
        if (i >= smaPeriod - 1) {                       // Если достигнут индекс, достаточный для начала расчета SMA
            smaValues.push_back(sum / smaPeriod);       // Вычисляем и сохраняем SMA
            sum -= candles[i - smaPeriod + 1].close;    // Вычитаем из суммы цену свечи, которая выходит за пределы окна SMA
        }
    }
    return smaValues;
}

void QuotesManager::WriteCandlesToFile(const std::string& filename) const {
    std::ofstream outFile(filename);
    outFile << "Time,Open,High,Low,Close,Volume\n";
    for (const auto& candle : candles) {
        outFile << candle.time << "," << candle.open << "," << candle.high << ","
                << candle.low << "," << candle.close << "," << candle.volume << "\n";
    }
}

void QuotesManager::WriteSMAtoFile(const std::string& filename) const {
    std::vector<double> smaValues = CalculateSMA();
    std::ofstream outFile(filename);
    outFile << "Time,SMA\n";
    for (size_t i = 0; i < smaValues.size(); ++i) {
        outFile << candles[i + smaPeriod - 1].time << "," << smaValues[i] << "\n";
    }
}
