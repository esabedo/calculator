#include "NumberConverter.hpp"
#include <QRegularExpression>
#include <stdexcept>

namespace calc {

QString NumberConverter::toString(int64_t value, NumberBase base) {
    if (value == 0) {
        return "0";
    }
    
    bool isNegative = value < 0;
    uint64_t absValue = isNegative ? -value : value;
    
    QString result;
    int baseValue = static_cast<int>(base);
    
    while (absValue > 0) {
        int digit = absValue % baseValue;
        if (digit < 10) {
            result.prepend(QChar('0' + digit));
        } else {
            result.prepend(QChar('A' + (digit - 10)));
        }
        absValue /= baseValue;
    }
    
    if (isNegative) {
        result.prepend('-');
    }
    
    return result;
}

int64_t NumberConverter::fromString(const QString& str, NumberBase base) {
    if (str.isEmpty()) {
        return 0;
    }
    
    QString cleanStr = str.trimmed().toUpper();
    bool isNegative = cleanStr.startsWith('-');
    if (isNegative) {
        cleanStr = cleanStr.mid(1);
    }
    
    int64_t result = 0;
    int baseValue = static_cast<int>(base);
    
    for (const QChar& c : cleanStr) {
        if (!isValidDigit(c, base)) {
            throw std::invalid_argument("Invalid digit for the given base");
        }
        
        int digit;
        if (c.isDigit()) {
            digit = c.digitValue();
        } else {
            digit = c.toLatin1() - 'A' + 10;
        }
        
        result = result * baseValue + digit;
    }
    
    return isNegative ? -result : result;
}

bool NumberConverter::isValidDigit(QChar c, NumberBase base) {
    c = c.toUpper();
    int baseValue = static_cast<int>(base);
    
    if (c.isDigit()) {
        int digit = c.digitValue();
        return digit < baseValue;
    }
    
    if (base == NumberBase::Hexadecimal && c >= 'A' && c <= 'F') {
        return true;
    }
    
    return false;
}

int NumberConverter::getMaxDigit(NumberBase base) {
    return static_cast<int>(base) - 1;
}

} // namespace calc
