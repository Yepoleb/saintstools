#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QTextStream>

#include "Saints/PegFile.hpp"

#include "../common.hpp"



int cmd_list(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Lists header fields and texture entries.");
    parser.addHelpOption();
    parser.addPositionalArgument("l", "This command", "l");
    parser.addPositionalArgument("header", "Header file ending with cvbm_pc or cpeg_pc", "<header>");
    parser.process(args);

    checkPositionalArgs(parser, 2, 2);

    QFile infile(parser.positionalArguments().value(1));
    safeOpen(infile, QIODevice::ReadOnly);
    Saints::PegFile pegfile(infile);

    qstdout << showbase;
    qstdout << "Version: " << pegfile.version << '\n';
    qstdout << "Platform: " << pegfile.platform << '\n';
    qstdout << "Dir block (cpeg) size: " << pegfile.header_size << '\n';
    qstdout << "Data block (gpeg) size: " << pegfile.data_size << '\n';
    qstdout << "Entries count: " << pegfile.entries.size() << '\n';
    qstdout << "Flags: " << hex << pegfile.flags << dec << '\n';
    qstdout << "Alignment: " << pegfile.alignment << '\n';
    qstdout << '\n';

    qstdout << "Entries: " << '\n';
    for (const Saints::PegEntry& entry : pegfile.entries) {
        qstdout << "Name: " << entry.filename << '\n';
        qstdout << "Dimensions: " << entry.width << "x" << entry.height << '\n';
        qstdout << "Format: " << Saints::getFormatName(entry.bm_fmt) << '\n';
        if (entry.flags) {
            qstdout << "Flags: " <<
                Saints::getEntryFlagNames(entry.flags).join(", ") <<
                " (" << hex << entry.flags << dec << ")" << '\n';
        }
        if (entry.mip_levels > 1) {
            qstdout << "Mip levels: " << entry.mip_levels << '\n';
        }
        if (entry.flags & Saints::BM_F_ANIM_SHEET) {
            qstdout << "Animation dimensions: "
                << entry.anim_tiles_width << "x"
                << entry.anim_tiles_height << '\n';
        }
        qstdout << "Offset: " << hex << entry.offset << dec << '\n';
        qstdout << "Texture size: " << entry.data_size << '\n';
        if (pegfile.version == 19) {
            if (entry.depth != 1) {
                qstdout << "Depth: " << entry.depth << '\n';
            }
            qstdout << hex << "Average color red: " << entry.avg_color.r << '\n';
            qstdout << "Average color green: " << entry.avg_color.g << '\n';
            qstdout << "Average color blue: " << entry.avg_color.b << '\n';
            qstdout << "Average color alpha: " << entry.avg_color.a << dec << '\n';
            if (entry.num_mips_split > 0) {
                qstdout << "Num mips split: " << entry.num_mips_split << '\n';
            }
            if (entry.data_max_size > 0) {
                qstdout << "Data max size: " << entry.data_max_size << '\n';
            }
        }
        qstdout << '\n';
    }

    infile.close();

    return 0;
}
