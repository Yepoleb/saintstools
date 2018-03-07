#include <QtCore/QtGlobal>
#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QDir>

#include "Saints/Packfile.hpp"

#include "../common.hpp"



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

    checkPositionalArgs(parser, 3, 3);

    QFile infile(parser.positionalArguments().value(1));
    safeOpen(infile, QIODevice::ReadOnly);

    Saints::Packfile pf(infile);

    QDir outdir(parser.positionalArguments().value(2));

    for (Saints::PackfileEntry& entry : pf.getEntries()) {
        QString native_filepath = toNativePath(entry.getFilepath());
        if (!outdir.mkpath(QFileInfo(native_filepath).path())) {
            qstderr << "Failed to create directory "
                << QFileInfo(native_filepath).path() << '\n';
            return 1;
        }
        QFile outfile(outdir.filePath(native_filepath));
        safeOpen(outfile, QIODevice::WriteOnly);

        outfile.write(entry.getData());
        outfile.close();
    }

    infile.close();

    return 0;
}
