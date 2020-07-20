/**
 * @file chieru_translator.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Implementation of Chieru translator
 * @version 0.1
 * @date 2020-07-20
 *
 * @copyright Copyright (c) 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "chieru_translator.h"
#include <QMutex>
#include <QDebug>

bool ChieruTranslator::s_initialized = false;
QChar ChieruTranslator::s_chieru_charactor[16];
QHash<QChar, int> ChieruTranslator::s_dict;
QString ChieruTranslator::s_symbols = QString::fromUtf16(
    u"！￥…（）—【】、；：‘’“”《》，。？～｀＃＄％＾＆＊－＿＝＋［］｛｝＼｜＇＂＜＞／"
);

ChieruTranslator::ChieruTranslator() {
    initialize();
}

void ChieruTranslator::initialize() {
    static QMutex initialize_mutex;
    if (s_initialized) return;

    initialize_mutex.lock();
    if (s_initialized) return;

    QString chieru_dict = QString::fromUtf16(u"切卟叮咧哔唎啪啰啵嘭噜噼巴拉蹦铃");
    assert(chieru_dict.length() == 16);

    for (int i = 0; i < 16; ++i) {
        s_chieru_charactor[i] = chieru_dict[i];
        s_dict.insert(chieru_dict[i], i);
    }

    s_initialized = true;

    initialize_mutex.unlock();
}

QString ChieruTranslator::word2chieru(QByteArray::const_iterator begin,
                                      QByteArray::const_iterator end) {
    QString result = QString::fromUtf16(u"切");
    assert(result.length() == 1);
    result.reserve((end - begin) * 2 + 2);

    for (; begin != end; ++begin) {
        char ch = *begin;
        result.push_back(s_chieru_charactor[ch & 15]);
        result.push_back(s_chieru_charactor[(ch & 0xF0)>> 4]);
    }

    return result;
}

QByteArray ChieruTranslator::chieru2word(QString::const_iterator begin,
                                         QString::const_iterator end) {
    QByteArray result;

    // A chieru word must start with '切', and the next charactors must be in pairs
    if ((end - begin) < 2 || !((end - begin) & 1) || *begin != QChar(L'切'))
        return "{ERROR}";

    for (auto itr = begin + 1; itr != end;) {
        char32_t code;

        auto dict_itr = s_dict.find(*itr);
        if (dict_itr == s_dict.end()) return "{ERROR}";
        code = dict_itr.value();
        ++itr;

        dict_itr = s_dict.find(*itr);
        if (dict_itr == s_dict.end()) return "{ERROR}";
        code |= dict_itr.value() << 4;
        ++itr;

        result.push_back(code);
    }

    return result;
}

QString ChieruTranslator::word2chieru(const QByteArray& word) {
    return word2chieru(word.begin(), word.end());
}

QByteArray ChieruTranslator::chieru2word(const QString& word) {
    return chieru2word(word.begin(), word.end());
}

QString ChieruTranslator::toChieru(const QString& string, QTextCodec* codec) {
    QString result = QString::fromUtf16(u"切噜～♪");
    assert(result.length() == 4);

    int i = 0, s = 0;
    for (int _end = string.length(); i < _end; ++i) {
        if (is_separator(string[i])) {
            if (s != i) {
                result.append(word2chieru(codec->fromUnicode(string.midRef(s, i - s))));
                s = i;
            }
            ++s;
            result.push_back(string[i]);
        }
    }

    if (s != string.length())
        result.append(word2chieru(codec->fromUnicode(string.midRef(s, string.length() - s))));
    return result;
}

QString ChieruTranslator::fromChieru(const QString& string, QTextCodec* codec) {
    QByteArray result;

    if (string.left(4) != QString::fromUtf16(u"切噜～♪")) return "{ERROR}";

    int i = 4, s = 4, _end = string.length();
    for (; i < _end; ++i) {
        if (is_separator(string[i])) {
            if (s != i) {
                result.append(chieru2word(string.begin() + s, string.begin() + i));
                s = i;
            }
            ++s;
            result.append(codec->fromUnicode(string[i]));
        }
    }

    if (s != _end)
        result.append(chieru2word(string.begin() + s, string.end()));
    return codec->toUnicode(result);
}
