#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "../common.hpp"


int cmd_add(QStringList args);
int cmd_check(QStringList args);
int cmd_delete(QStringList args);
int cmd_extract(QStringList args);
int cmd_list(QStringList args);


static const QString HELP_MAIN(QStringLiteral(
R"(SaintsPeg 0.0.1

Tool for converting DDS textures to and from the Volition bitmap format.

Usage: %1 [options] <command> [args...]

Options:

  -h, --help                        Display this help menu
  command                           Command to execute
  args                              Arguments for the command

  "--" can be used to terminate flag options and force all following
  arguments to be treated as positional options

Commands:

  x: Extract textures
  a: Add or update textures
  l: List header fields and entries
  d: Delete textures
  m: Modify texture properties
  c: Check texture for errors
)"));


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("saintspeg");
    app.setApplicationVersion("0.0.1");

    const QHash<QString, cmd_t> commands({
        {"a", cmd_add},
        {"c", cmd_check},
        {"d", cmd_delete},
        {"x", cmd_extract},
        {"l", cmd_list}
    });

    return genericMain(app, commands, HELP_MAIN);
}
