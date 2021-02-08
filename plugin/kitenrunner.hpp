#ifndef KITENRUNNER_HPP
#define KITENRUNNER_HPP

#include <QMimeData>

#include <krunner/abstractrunner.h>

#include <memory>

class DictionaryManager;

class KitenRunner : public Plasma::AbstractRunner
{
Q_OBJECT

public:
    KitenRunner(QObject *parent, const QVariantList &args);
    ~KitenRunner() override;

    void match(Plasma::RunnerContext &context) override;
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match) override;

protected:
    QList<QAction*> actionsForMatch(const Plasma::QueryMatch &match) override;
    QMimeData *mimeDataForMatch(const Plasma::QueryMatch &match) override;

private:
    bool _isSuccessfullyInitialized = false;
    std::shared_ptr<DictionaryManager> _dictionaryManager;

    const QString m_keyword;
};

#endif // KITENRUNNER_HPP
