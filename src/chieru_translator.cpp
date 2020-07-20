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

bool ChieruTranslator::s_initialized = false;
wchar_t ChieruTranslator::s_chieru_charactor[16];
std::map<wchar_t, int> ChieruTranslator::s_dict;
std::wstring ChieruTranslator::s_symbols(
    L"！￥…（）—【】、；：‘’“”《》，。？"
);

thread_local std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>
    ChieruTranslator::s_utf8_utf16_cvt;

ChieruTranslator::ChieruTranslator() {
    initialize();
}

void ChieruTranslator::initialize() {
    static std::mutex initialize_mutex;
    if (s_initialized) return;

    initialize_mutex.lock();
    if (s_initialized) return;

    std::wstring chieru_dict = L"切卟叮咧哔唎啪啰啵嘭噜噼巴拉蹦铃";

    auto itr = chieru_dict.begin(), _end = chieru_dict.end();
    int num = 0;

    while (itr != _end) {
        s_chieru_charactor[num] = *itr;
        s_dict.insert({*itr, num});
        ++num;
        ++itr;
    }

    s_initialized = true;

    initialize_mutex.unlock();
}

std::string ChieruTranslator::word2chieru(std::wstring::iterator begin,
                                          std::wstring::iterator end) {
    std::wstring result = L"切";

    for (; begin != end; ++begin) {
        wchar_t ch = *begin;
        result.push_back(s_chieru_charactor[ch & 15]);
        result.push_back(s_chieru_charactor[(ch & 0xF0)>> 4]);
        result.push_back(s_chieru_charactor[(ch & 0xF00)>> 8]);
        result.push_back(s_chieru_charactor[(ch & 0xF000)>> 12]);
    }

    return s_utf8_utf16_cvt.to_bytes(result);
}

std::string ChieruTranslator::chieru2word(std::wstring::iterator begin,
                                          std::wstring::iterator end) {
    std::wstring result;

    // A chieru word must start with '切', and the next charactors must be in pairs
    if (end - begin < 3 || !((end - begin) & 1) || *begin != L'切')
        return "{ERROR}";

    for (auto itr = begin + 1; itr != end;) {
        wchar_t code;

        auto dict_itr = s_dict.find(*itr);
        if (dict_itr == s_dict.end()) return "{ERROR}";
        code = dict_itr->second;
        ++itr;

        dict_itr = s_dict.find(*itr);
        if (dict_itr == s_dict.end()) return "{ERROR}";
        code |= dict_itr->second << 4;
        ++itr;

        dict_itr = s_dict.find(*itr);
        if (dict_itr == s_dict.end()) return "{ERROR}";
        code |= dict_itr->second << 8;
        ++itr;

        dict_itr = s_dict.find(*itr);
        if (dict_itr == s_dict.end()) return "{ERROR}";
        code |= dict_itr->second << 12;
        ++itr;

        result.push_back(code);
    }

    return s_utf8_utf16_cvt.to_bytes(result);
}

std::string ChieruTranslator::word2chieru(const std::string& word) {
    std::wstring u32word = s_utf8_utf16_cvt.from_bytes(word);
    return word2chieru(u32word.begin(), u32word.end());
}

std::string ChieruTranslator::chieru2word(const std::string& word) {
    std::wstring u32word = s_utf8_utf16_cvt.from_bytes(word);
    return chieru2word(u32word.begin(), u32word.end());
}

std::string ChieruTranslator::fromUTF8(const std::string& string) {
    std::wstring wstring = s_utf8_utf16_cvt.from_bytes(string);
    std::string result = s_utf8_utf16_cvt.to_bytes(L"切噜～♪");

    auto itr = wstring.begin(), start_pos = wstring.begin(),
         _end = wstring.end();

    while(itr != _end) {
        if (is_separator(*itr)) {
            if (start_pos != itr) {
                result.append(word2chieru(start_pos, itr));
                start_pos = itr;
            }
            ++start_pos;
            result.append(s_utf8_utf16_cvt.to_bytes(std::wstring(1, *itr)));
        }
        ++itr;
    }

    if (start_pos != _end)
        result.append(word2chieru(start_pos, _end));
    return result;
}

std::string ChieruTranslator::toUTF8(const std::string& string) {
    std::wstring wstring = s_utf8_utf16_cvt.from_bytes(string);
    std::string result;

    if (wstring.substr(0, 4) != L"切噜～♪") return "{ERROR}";

    auto itr = wstring.begin() + 4, start_pos = wstring.begin() + 4,
         _end = wstring.end();

    while(itr != _end) {
        if (is_separator(*itr)) {
            if (start_pos != itr) {
                result.append(chieru2word(start_pos, itr));
                start_pos = itr;
            }
            ++start_pos;
            result.append(s_utf8_utf16_cvt.to_bytes(std::wstring(1, *itr)));
        }
        ++itr;
    }

    if (start_pos != _end)
        result.append(chieru2word(start_pos, _end));
    return result;
}
