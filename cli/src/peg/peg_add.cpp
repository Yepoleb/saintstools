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



int cmd_add(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "Adds textures to a container or updates them if they already exist.");
    parser.addHelpOption();
    parser.addPositionalArgument("a",
        "This command", "a");
    parser.addPositionalArgument("header",
        "Header file ending with cvbm_pc or cpeg_pc", "<header>");
    parser.addPositionalArgument("textures",
        "Textures to add or update", "[textures...]");
    QCommandLineOption output_option(QStringList({"o", "output"}),
        "Output directory", "output");
    parser.addOption(output_option);
    QCommandLineOption input_option(QStringList({"i", "input"}),
        "Directory to update all existing textures from", "input");
    parser.addOption(input_option);
    QCommandLineOption version_option(QStringList({"v", "version"}),
        "Format version when creating a new file", "version");
    parser.addOption(version_option);
    QCommandLineOption format_option(QStringList({"f", "format"}),
        "Texture format used for compression", "format");
    parser.addOption(format_option);
    parser.process(args);

    checkPositionalArgs(parser, 2, -1);

    QString header_filename(parser.positionalArguments().value(1));
    QString data_filename(getDataFilename(header_filename));

    QString output_header;
    QString output_data;
    if (parser.isSet(output_option)) {
        output_header = parser.value(output_option);
        output_data = getDataFilename(output_header);
    } else {
        output_header = header_filename;
        output_data = data_filename;
    }

    Saints::TextureFormat default_format = Saints::TextureFormat::NONE;
    if (parser.isSet(format_option)) {
        default_format = Saints::getFormatId(parser.value(format_option));
        if (default_format == Saints::TextureFormat::NONE) {
            qstderr << "Failed to parse compression format\n";
            return 1;
        }
    }

    QFile headerfile(header_filename);
    QFile datafile(data_filename);
    Saints::PegFile pegfile;
    if (headerfile.exists()) {
        safeOpen(headerfile, QIODevice::ReadOnly);
        safeOpen(datafile, QIODevice::ReadOnly);
        pegfile.open(headerfile, datafile);
        headerfile.close();
        datafile.close();
    } else {
        if (!parser.isSet(version_option)) {
            qstderr << "Version option needs to be set when creating a new file\n";
            return 1;
        }
        QString version_str = parser.value(version_option);
        bool success;
        int version = version_str.toInt(&success, 0);
        if (success) {
            pegfile.version = version;
        } else {
            qstderr << "Invalid version number\n";
            return 1;
        }
    }

    QStringList textures(parser.positionalArguments().mid(2));
    for (QString tex_path : textures) {
        QString tex_name(QFileInfo(tex_path).fileName());
        QFile tex_file(tex_path);
        safeOpen(tex_file, QIODevice::ReadOnly);
        Saints::TGAFile tga_file(tex_file);

        Saints::PegEntry* entry;
        int tex_index = pegfile.getEntryIndex(tex_name);
        if (tex_index != -1) {
            entry = &pegfile.entries[tex_index];
        } else {
            Saints::PegEntry new_entry;
            new_entry.filename = tex_name;
            pegfile.entries.append(new_entry);
            entry = &pegfile.entries.back();
        }

        Saints::TextureFormat format = default_format;
        if (default_format == Saints::TextureFormat::NONE) {
            if (entry->bm_fmt == Saints::TextureFormat::NONE) {
                qstderr << "Unable to determine compression format\n";
                qstderr << "Check the texture names or specify a format with -f\n";
                return 1;
            } else {
                format = entry->bm_fmt;
            }
        }
        entry->fromTGA(tga_file, format);
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
