#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QDir>

#include "Saints/Packfile.hpp"

#include "common.hpp"



int cmd_extract(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Extracts packfiles");
    parser.addHelpOption();
    parser.addPositionalArgument("x", "This command", "x");
    parser.addPositionalArgument("file", "The file to open", "<file>");
    parser.addPositionalArgument("dest", "Destination folder", "<dest>");
    parser.process(args);

    if (parser.positionalArguments().count() != 3) {
        qstderr << parser.helpText();
        return 1;
    }

    QFile infile(parser.positionalArguments().value(1));
    if (!infile.open(QIODevice::ReadOnly)) {
        qstderr << "Failed to open input file\n";
        return 1;
    }

    Saints::Packfile pf(infile);

    QDir outdir(parser.positionalArguments().value(2));
    outdir.mkpath(".");

    for (Saints::PackfileEntry& entry : pf.getEntries()) {
        QFile outfile(outdir.filePath(entry.getFilename()));
        if (!outfile.open(QIODevice::WriteOnly)) {
            qstderr << "Failed to open output file\n";
            return 1;
        }
        outfile.write(entry.getData());
        outfile.close();
    }

    infile.close();

    return 0;
}
