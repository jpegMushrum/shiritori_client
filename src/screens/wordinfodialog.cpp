#include "wordinfodialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>

WordInfoDialog::WordInfoDialog(const QString &kanji, const QStringList &readings,
                               const QString &translation, const QStringList &partOfSpeech,
                               QWidget *parent)
    : QDialog(parent)
{
    setupUI(kanji, readings, translation, partOfSpeech);
    setWindowTitle("Word Information");
    setMinimumWidth(400);
    setMinimumHeight(300);
}

void WordInfoDialog::setupUI(const QString &kanji, const QStringList &readings,
                             const QString &translation, const QStringList &partOfSpeech)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);

    // Kanji title
    auto *kanjiLabel = new QLabel(this);
    kanjiLabel->setText(kanji);
    kanjiLabel->setObjectName("wordKanji");
    kanjiLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(kanjiLabel);

    mainLayout->addSpacing(10);

    // Readings section
    auto *readingsHeaderLabel = new QLabel("Readings:", this);
    readingsHeaderLabel->setObjectName("dialogSectionHeader");
    mainLayout->addWidget(readingsHeaderLabel);

    QString readingsText = readings.join(", ");
    auto *readingsLabel = new QLabel(readingsText, this);
    readingsLabel->setObjectName("dialogContent");
    readingsLabel->setWordWrap(true);
    mainLayout->addWidget(readingsLabel);

    mainLayout->addSpacing(10);

    // Translation section
    auto *translationHeaderLabel = new QLabel("Translation:", this);
    translationHeaderLabel->setObjectName("dialogSectionHeader");
    mainLayout->addWidget(translationHeaderLabel);

    auto *translationLabel = new QLabel(translation, this);
    translationLabel->setObjectName("dialogContent");
    translationLabel->setWordWrap(true);
    mainLayout->addWidget(translationLabel);

    mainLayout->addSpacing(10);

    // Part of Speech section
    if (!partOfSpeech.isEmpty())
    {
        auto *posHeaderLabel = new QLabel("Parts of Speech:", this);
        posHeaderLabel->setObjectName("dialogSectionHeader");
        mainLayout->addWidget(posHeaderLabel);

        QString posText = partOfSpeech.join(", ");
        auto *posLabel = new QLabel(posText, this);
        posLabel->setObjectName("dialogContent");
        posLabel->setWordWrap(true);
        mainLayout->addWidget(posLabel);

        mainLayout->addSpacing(10);
    }

    mainLayout->addStretch();

    // Close button
    auto *closeButton = new QPushButton("Close", this);
    closeButton->setMinimumHeight(35);
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);
    mainLayout->addWidget(closeButton);
}
