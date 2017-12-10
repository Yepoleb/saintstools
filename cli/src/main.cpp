#include <QtCore/QCoreApplication>
#include <QtCore/QCommandLineParser>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QHash>

#include "common.hpp"



static const QString HELP_MAIN = QStringLiteral(
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
)");

using cmd_t = int (*)(QStringList);

int cmd_extract(QStringList args);
int cmd_list(QStringList args);
int cmd_info(QStringList args);



QString format_help(QStringList args)
{
    return HELP_MAIN.arg(args.first());
}

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setApplicationName("saintspack");
    app.setApplicationVersion("0.0.1");

    QStringList args = app.arguments();

    if (args.size() < 2) {
        qstderr << format_help(args) << '\n';
        return 1;
    }
    if (args[1] == "-h" || args[1] == "--help") {
        qstdout << format_help(args) << '\n';
        return 0;
    }

    const QHash<QString, cmd_t> commands({
        {"x", cmd_extract},
        {"l", cmd_list},
        {"i", cmd_info}
    });

    if (commands.contains(args[1])) {
        return commands[args[1]](args);
    } else {
        qstderr << QString("[Error] Unknown command \"%s\"\n").arg(args[1]);
        return 1;
    }
}
