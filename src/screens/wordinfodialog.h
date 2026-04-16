#ifndef WORDINFODIALOG_H
#define WORDINFODIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

class WordInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WordInfoDialog(const QString &kanji, const QStringList &readings,
                            const QString &translation, const QStringList &partOfSpeech,
                            QWidget *parent = nullptr);

private:
    void setupUI(const QString &kanji, const QStringList &readings,
                 const QString &translation, const QStringList &partOfSpeech);
};

#endif // WORDINFODIALOG_H
