/**
 * @file gui.h
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief Declaration of the main widget
 * @version 0.1
 * @date 2020-07-20
 * 
 * @copyright Copyright (c) 2020
 * 
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
