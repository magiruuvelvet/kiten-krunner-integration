#include "kitenrunner.hpp"

#include <QtGlobal>
#include <QStandardPaths>
#include <QClipboard>
#include <QGuiApplication>

#ifdef DEBUG_BUILD
#include <QDebug>
#endif

#include <KLocalizedString>

#include <libkiten/dictionarymanager.h>
#include <libkiten/dictquery.h>
#include <libkiten/entrylist.h>

K_EXPORT_PLASMA_RUNNER_WITH_JSON(KitenRunner, "plasma-runner-kiten.json")

KitenRunner::KitenRunner(QObject *parent, const QVariantList &args)
    : Plasma::AbstractRunner(parent, args),
      m_keyword(QStringLiteral("kiten"))
{
    this->setObjectName(QStringLiteral("Kiten"));
    this->setIgnoredTypes(
        Plasma::RunnerContext::Directory |
        Plasma::RunnerContext::File |
        Plasma::RunnerContext::NetworkLocation |
        Plasma::RunnerContext::Executable |
        Plasma::RunnerContext::ShellCommand
    );

    const QString description = i18n("Queries the Kiten dictionary");
    this->setPriority(AbstractRunner::HighestPriority);
    this->addSyntax(Plasma::RunnerSyntax(QStringLiteral("kiten :q:"), description));

    this->addAction(QStringLiteral("copyToClipboard"), QIcon::fromTheme(QStringLiteral("edit-copy")), i18n("Copy to Clipboard"));

    // initialize the Kiten dictionary
    this->_dictionaryManager = std::make_shared<DictionaryManager>();
    const QString dictType = "edict";
    const QString dictionary = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral( "kiten/" ) + dictType);
    this->_isSuccessfullyInitialized = this->_dictionaryManager->addDictionary(dictionary, dictType, dictType);
}

KitenRunner::~KitenRunner()
{
}

void KitenRunner::match(Plasma::RunnerContext &context)
{
    // do nothing if the Kiten dictionary is missing
    if (!this->_isSuccessfullyInitialized)
    {
        return;
    }

    // check if the kiten command exists, or abort
    const QString term = context.query().trimmed();
#ifdef DEBUG_BUILD
    qDebug() << "kiten command term: " << term;
#endif
    if (!term.startsWith(m_keyword))
    {
        return;
    }

    // extract the query from the command
    const QString query = term.mid(m_keyword.size()).trimmed();
#ifdef DEBUG_BUILD
    qDebug() << "kiten command query" << query;
#endif

    // don't attempt to query an empty string
    if (query.isEmpty())
    {
        return;
    }

    // do the Kiten dictionary search and add one match for each item
    DictQuery dictQuery(query);
    dictQuery.setMatchType(DictQuery::Beginning);
    auto dictMatches = this->_dictionaryManager->doSearch(dictQuery);

    // no matches, abort
    if (dictMatches->size() == 0)
    {
        return;
    }

    // iterate over all dictionary matches and create a runner item
    for (auto&& dictMatch : *dictMatches)
    {
        const QString result = dictMatch->toString();
        Plasma::QueryMatch match(this);
        match.setType(Plasma::QueryMatch::InformationalMatch);
        //match.setIconName();
        match.setText(result);
        match.setData(result);
        match.setId(term);
        context.addMatch(match);
    }
}

void KitenRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    if (match.selectedAction())
    {
        QGuiApplication::clipboard()->setText(match.text());
    }
}

QList<QAction*> KitenRunner::actionsForMatch(const Plasma::QueryMatch &match)
{
    return actions().values();
}

QMimeData *KitenRunner::mimeDataForMatch(const Plasma::QueryMatch &match)
{
    QMimeData *result = new QMimeData();
    result->setText(match.text());
    return result;
}

#include "kitenrunner.moc"
