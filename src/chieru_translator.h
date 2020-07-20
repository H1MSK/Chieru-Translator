/**
 * @file chieru_translator.h
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Declaration of Chieru translator
 * @version 0.1
 * @date 2020-07-18
 *
 * @warning This file should be encoded in UTF-8
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <QString>
#include <QHash>
#include <QTextCodec>

class ChieruTranslator {
 private:
    static bool s_initialized;
    static QChar s_chieru_charactor[16];
    static QHash<QChar, int> s_dict;
    static QString s_symbols;


 protected:
    static QString word2chieru(QByteArray::const_iterator begin,
                               QByteArray::const_iterator end);
    static QByteArray chieru2word(QString::const_iterator begin,
                                  QString::const_iterator end);

    static bool is_separator(QChar ch) {
        if ((ch < 0x30) ||                      // before '0'
            (ch > 0x39 && ch <= 0x40) ||        // between '9' and 'A'
            (ch > 0x5A && ch <= 0x60) ||        // between 'Z' and 'a'
            (ch > 0x7A && ch <= 0x7F))          // after 'z' in ascii
            return true;

        // is Chinese symbol
        for (QChar symbol : s_symbols)
            if (ch == symbol) return true;

        return false;
    }

 public:
    ChieruTranslator();

    void initialize();
    static QString word2chieru(const QByteArray& word);
    static QByteArray chieru2word(const QString& word);
    QString toChieru(const QString& string, QTextCodec* codec = QTextCodec::codecForName("UTF8"));
    QString fromChieru(const QString& string, QTextCodec* codec = QTextCodec::codecForName("UTF8"));
};
