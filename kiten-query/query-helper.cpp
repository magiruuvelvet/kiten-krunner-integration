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

static inline void print_helper(const QString &str)
{
    std::printf("%s\n", str.toStdString().c_str());
}

int main(int argc, char **argv)
{
    qInstallMessageHandler(silenceKitenLibraryOutput);

    // parse dictionary
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
        queryString = QString::fromUtf8(argv[1]).trimmed();
    }

    if (queryString.isEmpty())
    {
        return 2;
    }

    DictQuery query(queryString);
    query.setMatchType(DictQuery::Anywhere);
    auto list = manager.doSearch(query);

    if (list->size() == 0)
    {
        return 5;
    }

    for (auto&& entry : *list)
    {
        print_helper(entry->toString());
    }

    return 0;
}
