#pragma once
#include <QtCore/QtGlobal>
#include <QtCore/QTextStream>
#include <QtCore/QCoreApplication>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QCommandLineParser>



extern QTextStream qstdout;
extern QTextStream qstderr;

using cmd_t = int (*)(QStringList);

QString formatHelp(QString help, QStringList args);
int genericMain(QCoreApplication& app, const QHash<QString, cmd_t>& commands, QString help);
void safeOpen(QFileDevice& file, QIODevice::OpenMode mode);
void checkPositionalArgs(QCommandLineParser& parser, int min_count, int max_count);
QString getDataFilename(const QString& header_filename);

constexpr qint64 alignAddress(qint64 address, qint64 alignment)
{
    return (address + alignment - 1) / alignment * alignment;
}

// Used for terminating the program inside shared functions
// Does not inherit from exception because it should not get caught
class ExitError
{
public:
    explicit ExitError(int status) : m_status(status) { }
    int getStatus() const noexcept {return m_status;}

private:
    int m_status;
};
