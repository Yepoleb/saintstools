#include <stdio.h>
#include <QtCore/QCoreApplication>
#include <QtCore/QTextStream>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>
#include <QtCore/QFileInfo>
#include <QtCore/QIODevice>
#include <QtCore/QFileDevice>

#include "common.hpp"



QTextStream qstdout(stdout);
QTextStream qstderr(stderr);


QString formatHelp(QString help, QStringList args)
{
    return help.arg(args.first());
}

int genericMain(QCoreApplication& app, const QHash<QString, cmd_t>& commands, QString help)
{
    QStringList args = app.arguments();

    if (args.size() < 2) {
        qstderr << formatHelp(help, args) << '\n';
        return 1;
    }
    if (args[1] == "-h" || args[1] == "--help") {
        qstdout << formatHelp(help, args) << '\n';
        return 0;
    }

    if (commands.contains(args[1])) {
        try {
            return commands[args[1]](args);
        } catch (const ExitError& e) {
            return e.getStatus();
        }
    } else {
        qstderr << QString("[Error] Unknown command %1\n").arg(args[1]);
        return 1;
    }
}

void safeOpen(QFileDevice& file, QIODevice::OpenMode mode)
{
    if (!file.open(mode)) {
        if (mode & QIODevice::WriteOnly) {
            qstderr << "Failed to open " << file.fileName() << " for writing\n";
        } else if (mode& QIODevice::ReadOnly) {
            qstderr << "Failed to open " << file.fileName() << " for reading\n";
        } else {
            qstderr << "Failed to open " << file.fileName() << " with flags "
                << mode << '\n';
        }
        throw ExitError(1);
    }
}

void checkPositionalArgs(QCommandLineParser& parser, int min_count, int max_count)
{
    if ((min_count != -1) && (parser.positionalArguments().count() < min_count)) {
        qstderr << "Error: Too few arguments\n";
        qstderr << parser.helpText() << '\n';
        throw ExitError(1);
    } else if ((max_count != -1) && (parser.positionalArguments().count() > max_count)) {
        qstderr << "Error: Too many arguments\n";
        qstderr << parser.helpText() << '\n';
        throw ExitError(1);
    }
}

QString getDataFilename(const QString& cpu_filename)
{
    int separator_pos = cpu_filename.lastIndexOf('.');
    QString gpu_filename = cpu_filename;
    gpu_filename[separator_pos + 1] = 'g';
    return gpu_filename;
}
