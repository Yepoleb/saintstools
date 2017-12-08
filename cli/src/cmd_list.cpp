#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include "Saints/Packfile.hpp"

#include "common.hpp"



int cmd_list(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Lists packfiles contents");
    parser.addHelpOption();
    parser.addPositionalArgument("l", "This command", "l");
    parser.addPositionalArgument("file", "The file to open", "<file>");
    parser.process(args);

    if (parser.positionalArguments().count() != 2) {
        qstderr << parser.helpText();
        return 1;
    }

    QFile infile(parser.positionalArguments().value(1));
    if (!infile.open(QIODevice::ReadOnly)) {
        qstderr << "Failed to open input file\n";
        return 1;
    }

    Saints::Packfile pf(infile);

    for (const Saints::PackfileEntry& entry : pf.getEntries()) {
        qstdout << entry.getSize() << " " << entry.getFilename() << "\n";
    }

    infile.close();

    return 0;
}
