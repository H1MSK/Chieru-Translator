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

#include <locale>
#include <codecvt>
#include <exception>
#include <map>
#include <mutex>
#include <string>

#include "abstract_translator.h"

class ChieruTranslator : public AbstractTranslator {
 private:
    static bool s_initialized;
    static wchar_t s_chieru_charactor[16];
    static std::map<wchar_t, int> s_dict;
    static std::wstring s_symbols;

    // UCS4-UTF8 converter
    static thread_local
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>
        s_utf8_utf16_cvt;

 protected:
    static std::string word2chieru(std::wstring::iterator begin,
                                   std::wstring::iterator end);
    static std::string chieru2word(std::wstring::iterator begin,
                                   std::wstring::iterator end);

    static bool is_separator(wchar_t ch) {
        return  (ch < 0x30) ||                      // before '0'
                (ch > 0x39 && ch <= 0x40) ||        // between '9' and 'A'
                (ch > 0x5A && ch <= 0x60) ||        // between 'Z' and 'a'
                (ch > 0x7A && ch <= 0x7F) ||        // after 'z' in ascii
                s_symbols.find(ch) != std::wstring::npos; // is Chinese symbol
    }

 public:
    ChieruTranslator();

    void initialize() override;
    static std::string word2chieru(const std::string& word);
    static std::string chieru2word(const std::string& word);
    std::string fromUTF8(const std::string& string) override;
    std::string toUTF8(const std::string& string) override;
};
