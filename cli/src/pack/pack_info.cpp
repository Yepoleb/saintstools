#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include "Saints/Packfile.hpp"

#include "../common.hpp"



int cmd_info(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Shows packfile details");
    parser.addHelpOption();
    parser.addPositionalArgument("i", "This command", "i");
    parser.addPositionalArgument("file", "The file to open", "<file>");
    parser.process(args);

    checkPositionalArgs(parser, 2, 2);

    QString filename(parser.positionalArguments().value(1));
    QFile infile(filename);
    safeOpen(infile, QIODevice::ReadOnly);

    Saints::Packfile pf(infile);

    qstdout << "Filename: " << filename << '\n';
    qstdout << "Version: " << pf.getVersion() << '\n';
    qstdout << "Number of entries: " << pf.getEntriesCount() << '\n';
    qstdout << "Flags: " << pf.getFlags() << '\n';
    qstdout << "Timestamp: " << pf.getTimestamp() << '\n';

    infile.close();

    return 0;
}
