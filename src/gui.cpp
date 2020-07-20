/**
 * @file gui.cpp
 * @author H1MSK (ksda47832338@outlook.com)
 * @brief GUI wrapper for the translator
 * @version 0.1
 * @date 2020-07-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "gui.h"
#include "ui_gui.h"

#include <QTextCodec>
#include <QResizeEvent>
#include <QDateTime>
#include <QPixmap>
#include <QLabel>
#include <QGraphicsOpacityEffect>
#include <QDebug>

#include <QFontDatabase>

#include <cstdlib>

#include "chieru_translator.h"
#include "singleton.h"

TranslatorWidget::TranslatorWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::EncoderWidget)
    , m_translator(Instance<ChieruTranslator>())
    , m_horizontal_background(nullptr)
    , m_vertical_background(nullptr)
    , m_current_background(nullptr)
    , m_background_label(new QLabel(this)) {
    ui->setupUi(this);

    srand(QDateTime::currentMSecsSinceEpoch());
    int total;
    for (total = 0; QFile::exists(QString(":/imgs/chieruh%1.png").arg(total)); ++total) {
        qDebug() <<"Found: " <<QString(":/imgs/chieruh%1.png").arg(total);
    }
    m_horizontal_background = new QPixmap(QString(":/imgs/chieruh%1.png").arg(rand() % total));

    for (total = 0; QFile::exists(QString(":/imgs/chieruv%1.png").arg(total)); ++total) {
        qDebug() <<"Found: " <<QString(":/imgs/chieruv%1.png").arg(total);
    }
    m_vertical_background = new QPixmap(QString(":/imgs/chieruv%1.png").arg(rand() % total));

    m_current_background = m_horizontal_background;

    m_background_label->setAttribute(Qt::WA_TransparentForMouseEvents);
    m_background_label->resize(size());
    m_background_label->show();
    m_background_label->setPixmap(*m_horizontal_background);

    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(m_background_label);
    effect->setEnabled(true);
    effect->setOpacity(0.2);
    m_background_label->setGraphicsEffect(effect);

#ifdef ANDROID
    QFont f = ui->textedit_original->font();
    f.setPointSize(18);
    ui->textedit_original->setFont(f);
    ui->textedit_translated->setFont(f);
#endif

    updateUI();
}

TranslatorWidget::~TranslatorWidget()
{
    delete ui;
}

void TranslatorWidget::resizeEvent(QResizeEvent *event) {
    m_background_label->resize(event->size());
    updateUI();
}

void TranslatorWidget::updateUI() {
    do {
        if ((ui->central_layout->direction() == QBoxLayout::LeftToRight)) {
            if (width() > height()) break;

            // Change direction to vertical
            ui->central_layout->setDirection(QBoxLayout::TopToBottom);
            ui->layout_control->setDirection(QBoxLayout::LeftToRight);
            ui->button_to_string->setText(QString("\xE2\x86\x91")); // ↑
            ui->button_from_string->setText(QString("\xE2\x86\x93")); // ↓

            m_current_background = m_vertical_background;
        } else {
            if (width() < height()) break;

            // Change direction to horizontal
            ui->central_layout->setDirection(QBoxLayout::LeftToRight);
            ui->layout_control->setDirection(QBoxLayout::TopToBottom);
            ui->button_to_string->setText(QString("\xE2\x86\x90")); // ←
            ui->button_from_string->setText(QString("\xE2\x86\x92")); // →

            m_current_background = m_horizontal_background;
        }
    } while(0);

    // Change background and center it
    QPixmap background = m_current_background->scaled(size(), Qt::KeepAspectRatioByExpanding);

    m_background_label->resize(background.size());
    if (background.width() > width()) {
        m_background_label->move((width() - background.width()) / 2, 0);
    } else {
        m_background_label->move(0, (height() - background.height()) / 2);
    }
    m_background_label->setPixmap(background);

    m_background_label->raise();
}

void TranslatorWidget::on_button_from_string_clicked() {
    std::string original_string = ui->textedit_original->toPlainText()
                                        .toUtf8().toStdString();

    QString translated_string = QString::fromUtf8(
                QByteArray::fromStdString(
                    m_translator->fromUTF8(original_string)));

    ui->textedit_translated->setPlainText(translated_string);
}

void TranslatorWidget::on_button_to_string_clicked() {
    std::string translated_string = ui->textedit_translated->toPlainText()
                                        .toUtf8().toStdString();

    QString origianl_string = QString::fromUtf8(
                QByteArray::fromStdString(
                    m_translator->toUTF8(translated_string)));

    ui->textedit_original->setPlainText(origianl_string);
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    int font_id = QFontDatabase::addApplicationFont(":/fonts/FeiGunGun.ttf");
    if (font_id != -1) {
        QFont font(QFontDatabase::applicationFontFamilies(font_id).at(0));
        QGuiApplication::setFont(font);
    }

    Instance<TranslatorWidget>()->show();
    return a.exec();
}
