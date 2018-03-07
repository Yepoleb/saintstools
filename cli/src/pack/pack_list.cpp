#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include "Saints/Packfile.hpp"

#include "../common.hpp"



int cmd_list(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Lists packfiles contents");
    parser.addHelpOption();
    parser.addPositionalArgument("l", "This command", "l");
    parser.addPositionalArgument("file", "The file to open", "<file>");
    parser.process(args);

    checkPositionalArgs(parser, 2, 2);

    QFile infile(parser.positionalArguments().value(1));
    safeOpen(infile, QIODevice::ReadOnly);
    Saints::Packfile pf(infile);

    for (const Saints::PackfileEntry& entry : pf.getEntries()) {
        qstdout << entry.getSize();
        if (entry.getFlags() & Saints::PackfileEntry::Compressed) {
            qstdout << '*';
        }
        qstdout << " " << entry.getFilepath() << '\n';
    }

    infile.close();

    return 0;
}
