#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QSaveFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtCore/QDir>

#include "Saints/PegFile.hpp"
#include "Saints/TGAFile.hpp"

#include "../common.hpp"



int cmd_delete(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Deletes textures from a container.");
    parser.addHelpOption();
    parser.addPositionalArgument("d", "This command", "x");
    parser.addPositionalArgument("header", "Header file ending with cvbm_pc or cpeg_pc", "<header>");
    parser.addPositionalArgument("textures", "Texture names to delete", "[textures...]");
    QCommandLineOption output_option(QStringList({"o", "output"}), "Output directory", "output");
    parser.addOption(output_option);
    parser.process(args);

    checkPositionalArgs(parser, 2, -1);

    QString header_filename(parser.positionalArguments().value(1));
    QString data_filename(getDataFilename(header_filename));

    QString output_dir(parser.value(output_option));
    QString output_header;
    QString output_data;
    if (parser.isSet(output_option)) {
        output_header = QDir(output_dir).filePath(
            QFileInfo(header_filename).fileName());
        output_data = getDataFilename(output_header);
    } else {
        output_header = header_filename;
        output_data = data_filename;
    }

    QFile headerfile(header_filename);
    QFile datafile(data_filename);
    safeOpen(headerfile, QIODevice::ReadOnly);
    safeOpen(datafile, QIODevice::ReadOnly);
    Saints::PegFile pegfile(headerfile, datafile);
    headerfile.close();
    datafile.close();

    QStringList textures(parser.positionalArguments().mid(2));
    for (QString tex_name : textures) {
        int tex_index = pegfile.getEntryIndex(tex_name);
        if (tex_index >= 0) {
            pegfile.entries.removeAt(tex_index);
        } else {
            qstderr << "Could not find texture " << tex_name << '\n';
        }
    }

    if (!output_dir.isEmpty()) {
        QDir(output_dir).mkpath(".");
    }
    QSaveFile headersave(output_header);
    QSaveFile datasave(output_data);
    safeOpen(headersave, QIODevice::WriteOnly);
    safeOpen(datasave, QIODevice::WriteOnly);
    pegfile.writeHeader(headersave);
    pegfile.writeData(datasave);
    headersave.commit();
    datasave.commit();

    return 0;
}
