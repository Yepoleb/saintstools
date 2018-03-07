#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>
#include <QtCore/QDir>

#include "Saints/PegFile.hpp"
#include "Saints/TGAFile.hpp"

#include "../common.hpp"



void extractFile(const QDir& output_dir, const Saints::PegEntry& entry)
{
    QFile outfile(output_dir.filePath(entry.filename));
    safeOpen(outfile, QIODevice::WriteOnly);
    Saints::TGAFile tgafile(entry.toTGA());
    tgafile.write(outfile);
    outfile.close();
}

int cmd_extract(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Lists header fields and texture entries.");
    parser.addHelpOption();
    parser.addPositionalArgument("x", "This command", "x");
    parser.addPositionalArgument("header", "Header file ending with cvbm_pc or cpeg_pc", "<header>");
    parser.addPositionalArgument("textures", "Texture names if you only want to extract certain textures", "[textures...]");
    QCommandLineOption output_option(QStringList({"o", "output"}), "Directory to write the files to", "output");
    parser.addOption(output_option);
    parser.process(args);

    checkPositionalArgs(parser, 2, -1);

    QDir output_dir("");
    if (parser.isSet(output_option)) {
        output_dir.setPath(parser.value(output_option));
    }

    QString header_filename(parser.positionalArguments().value(1));
    QString data_filename(getDataFilename(header_filename));
    QFile headerfile(header_filename);
    QFile datafile(data_filename);
    safeOpen(headerfile, QIODevice::ReadOnly);
    safeOpen(datafile, QIODevice::ReadOnly);
    Saints::PegFile pegfile(headerfile, datafile);
    headerfile.close();
    datafile.close();

    QStringList textures(parser.positionalArguments().mid(2));
    if (!textures.isEmpty()) {
        for (QString tex_name : textures) {
            int tex_index = pegfile.getEntryIndex(tex_name);
            if (tex_index >= 0) {
                extractFile(output_dir, pegfile.entries[tex_index]);
            } else {
                qstderr << "Could not find texture " << tex_name << '\n';
            }
        }
    } else {
        for (Saints::PegEntry& entry : pegfile.entries) {
            extractFile(output_dir, entry);
        }
    }

    return 0;
}
