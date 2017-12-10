#include <QtCore/QtGlobal>
#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QDir>

#include "Saints/Packfile.hpp"

#include "common.hpp"



QString toNativePath(QString windows)
{
#ifndef Q_OS_WIN
    return windows.replace("\\", "/");
#endif
}

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
        qstderr << "Error: Too few arguments\n";
        qstderr << parser.helpText() << '\n';
        return 1;
    }

    QFile infile(parser.positionalArguments().value(1));
    if (!infile.open(QIODevice::ReadOnly)) {
        qstderr << "Failed to open input file\n";
        return 1;
    }

    Saints::Packfile pf(infile);

    QDir outdir(parser.positionalArguments().value(2));

    for (Saints::PackfileEntry& entry : pf.getEntries()) {
        QString native_filepath = toNativePath(entry.getFilepath());
        outdir.mkpath(QFileInfo(native_filepath).path());
        QString outpath(outdir.filePath(native_filepath));
        QFile outfile(outpath);
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
