/**
 * @file chieru_translator.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Crude cli
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

#include <iostream>
#include "chieru_translator.h"

int main() {
    ChieruTranslator *translator = new ChieruTranslator();
    while(1) {
        std::string line;
        std::cout << ">>";
        std::flush(std::cout);
        std::getline(std::cin, line);
        if (line.front() == '1')
            std::cout <<translator->toUTF8(line.substr(2)) <<std::endl;
        else if (line.front() == '0')
            std::cout <<translator->fromUTF8(line.substr(2)) <<std::endl;
        else if (line.front() == 'q')
            exit(0);
    }
}