/**
 * @file gui.h
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Declaration of the main widget
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
#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class EncoderWidget; }
QT_END_NAMESPACE

class AbstractTranslator;
class QPixmap;
class QLabel;

class TranslatorWidget : public QWidget
{
    Q_OBJECT

public:
    TranslatorWidget(QWidget *parent = nullptr);
    ~TranslatorWidget();

    void resizeEvent(QResizeEvent *event) override;

private slots:
    void on_button_from_string_clicked();

    void on_button_to_string_clicked();

    void updateUI();

private:

    Ui::EncoderWidget *ui;
    AbstractTranslator *m_translator;
    QPixmap *m_horizontal_background, *m_vertical_background, *m_current_background;
    QLabel* m_background_label;
};
