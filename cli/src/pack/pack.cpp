#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "../common.hpp"


int cmd_extract(QStringList args);
int cmd_list(QStringList args);
int cmd_info(QStringList args);


static const QString HELP_MAIN(QStringLiteral(
R"(SaintsPack 0.0.1

Tool for working with Volition packfiles.

Usage: %1 [options] <command> [args...]

Options:

  -h, --help                        Display this help menu
  command                           Command to execute
  args                              Arguments for the command

  "--" can be used to terminate flag options and force all following
  arguments to be treated as positional options

Commands:

  x: Extract packfile
  l: List files
  i: Show details
)"));


int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("saintspack");
    app.setApplicationVersion("0.0.1");

    const QHash<QString, cmd_t> commands({
        {"x", cmd_extract},
        {"l", cmd_list},
        {"i", cmd_info}
    });

    return genericMain(app, commands, HELP_MAIN);
}
