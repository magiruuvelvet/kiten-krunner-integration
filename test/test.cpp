#include <QApplication>
#include <QtGlobal>
#include <QStandardPaths>
#include <cstdio>
#include <libkiten/dictquery.h>
#include <libkiten/dictionarymanager.h>
#include <libkiten/entrylist.h>

static void silenceKitenLibraryOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    // send output from libkiten to void
}

static inline void printfq(const QString &str)
{
    std::printf("%s\n", str.toStdString().c_str());
}

int main(int argc, char **argv)
{
    qInstallMessageHandler(silenceKitenLibraryOutput);
    QApplication a(argc, argv);

    DictionaryManager manager;
    const QString dictType = "edict";
    const QString dictionary = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral( "kiten/" ) + dictType);
    const bool success = manager.addDictionary(dictionary, dictType, dictType);

    if (!success)
    {
        return 1;
    }

    QString queryString;

    if (argc > 1)
    {
        queryString = QString::fromUtf8(argv[1]);
    }

    DictQuery query(queryString);
    query.setMatchType(DictQuery::Anywhere);
    auto list = manager.doSearch(query);

    for (auto&& entry : *list)
    {
        //printfq(entry->getWord());
        //printfq(entry->getReadings());
        //printfq(entry->getMeanings());
        printfq(entry->toString());
    }

    return 0;
}
