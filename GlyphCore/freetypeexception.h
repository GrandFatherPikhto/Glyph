#ifndef __FREETYPEEXCEPTION_H__
#define __FREETYPEEXCEPTION_H__

#include <QException>
#include <QString>

#include "GlyphCore_global.h"

class GLYPHCORE_EXPORT FreetypeException : public QException
{
public:
    explicit FreetypeException(const QString& message) 
        : m_message(message) {}
    
    // Клонирование исключения (необходимо для многопоточности)
    FreetypeException* clone() const override {
        return new FreetypeException(*this);
    }
    
    // Переопределение метода what() из std::exception
    const char* what() const noexcept override {
        return m_message.toUtf8().constData();
    }
    
    // Переопределение метода raise() из QException
    void raise() const override {
        throw *this;
    }
    
    const QString& message() const { return m_message; }

private:
    QString m_message;
};

#endif // __FREETYPEEXCEPTION_H__