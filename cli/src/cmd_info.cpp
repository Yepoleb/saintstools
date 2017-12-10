#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include "Saints/Packfile.hpp"

#include "common.hpp"



int cmd_info(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Shows packfile details");
    parser.addHelpOption();
    parser.addPositionalArgument("i", "This command", "i");
    parser.addPositionalArgument("file", "The file to open", "<file>");
    parser.process(args);

    if (parser.positionalArguments().count() < 2) {
        qstderr << "Error: Too few arguments\n";
        qstderr << parser.helpText() << '\n';
        return 1;
    } else if (parser.positionalArguments().count() > 2) {
        qstderr << "Error: Too many arguments\n";
        qstderr << parser.helpText() << '\n';
        return 1;
    }

    QString filename(parser.positionalArguments().value(1));
    QFile infile(filename);
    if (!infile.open(QIODevice::ReadOnly)) {
        qstderr << "Failed to open input file\n";
        return 1;
    }

    Saints::Packfile pf(infile);

    qstdout << "Filename: " << filename << '\n';
    qstdout << "Version: " << pf.getVersion() << '\n';
    qstdout << "Number of entries: " << pf.getEntriesCount() << '\n';
    qstdout << "Flags: " << pf.getFlags() << '\n';
    qstdout << "Timestamp: " << pf.getTimestamp() << '\n';

    infile.close();

    return 0;
}
