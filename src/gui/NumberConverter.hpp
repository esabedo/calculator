#pragma once

#include <QString>
#include <cstdint>

namespace calc {

/**
 * @brief Система счисления для программистского режима
 */
enum class NumberBase {
    Hexadecimal = 16,  // HEX
    Decimal = 10,      // DEC
    Octal = 8,         // OCT
    Binary = 2         // BIN
};

/**
 * @brief Утилита для конвертации между системами счисления
 */
class NumberConverter {
public:
    /**
     * @brief Конвертировать число в строку в заданной системе счисления
     */
    static QString toString(int64_t value, NumberBase base);
    
    /**
     * @brief Конвертировать строку из заданной системы счисления в число
     */
    static int64_t fromString(const QString& str, NumberBase base);
    
    /**
     * @brief Проверить, является ли символ допустимым для данной системы счисления
     */
    static bool isValidDigit(QChar c, NumberBase base);
    
    /**
     * @brief Получить максимальное значение цифры для системы счисления
     */
    static int getMaxDigit(NumberBase base);
};

} // namespace calc
