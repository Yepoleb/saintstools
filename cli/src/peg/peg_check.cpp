#include <QtCore/QCommandLineParser>
#include <QtCore/QStringList>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QIODevice>

#include "Saints/PegFile.hpp"

#include "../common.hpp"



#define CHECK_FIELD(cond) \
    if (!(cond)) { \
        qstdout << "Failed check: " << #cond << '\n'; \
        failed = true; \
    }

constexpr bool is_pow2(int x)
{
    return (x != 0) && !(x & (x - 1));
}

int cmd_check(QStringList args)
{
    QCommandLineParser parser;
    parser.setApplicationDescription("Checks a texture file for format errors. No output means the file is probably ok.");
    parser.addHelpOption();
    parser.addPositionalArgument("c", "This command", "c");
    parser.addPositionalArgument("header", "Header files ending with cvbm_pc or cpeg_pc", "<header...>");
    parser.process(args);

    checkPositionalArgs(parser, 2, 2);

    QStringList headers(parser.positionalArguments().mid(1));
    for (QString header_path : headers) {
        QString data_path(getDataFilename(header_path));

        QFile headerfile(header_path);
        safeOpen(headerfile, QIODevice::ReadOnly);
        Saints::PegFile pegfile(headerfile);
        qint64 data_size = QFileInfo(data_path).size();

        qint64 data_size_predict = 0;
        for (const Saints::PegEntry& entry : pegfile.entries) {
            data_size_predict = alignAddress(data_size_predict, 16) + entry.data_size;
        }

        bool failed = false;
        CHECK_FIELD(pegfile.data_size == data_size_predict);
        CHECK_FIELD(pegfile.data_size == data_size);
        CHECK_FIELD(pegfile.flags == 0);
        CHECK_FIELD(pegfile.alignment == 16);
        CHECK_FIELD(pegfile.entries.size() > 0);

        for (const Saints::PegEntry& entry : pegfile.entries) {
            CHECK_FIELD(entry.offset >= 0);
            CHECK_FIELD(entry.offset + entry.data_size <= pegfile.data_size);
            CHECK_FIELD(entry.width > 0);
            CHECK_FIELD(entry.height > 0);
            if (pegfile.version < 19 && !(entry.flags & Saints::BM_F_NONPOW2)) {
                CHECK_FIELD(is_pow2(entry.width));
                CHECK_FIELD(is_pow2(entry.height));
            }
            CHECK_FIELD(entry.bm_fmt >= Saints::TextureFormat::PC_BC1);
            CHECK_FIELD(entry.bm_fmt <= Saints::TextureFormat::PC_32323232);
            CHECK_FIELD(entry.pal_fmt == 0);
            CHECK_FIELD(entry.num_frames == 1);
            CHECK_FIELD(entry.pal_size == 0);
            CHECK_FIELD(entry.fps == 1);
            CHECK_FIELD(entry.mip_levels >= 1);
            CHECK_FIELD(entry.data_size >= 0);

            CHECK_FIELD(!entry.filename.isEmpty());
        }

        if (failed) {
            qstdout << "Failed: " << header_path << '\n';
        }
    }

    return 0;
}
