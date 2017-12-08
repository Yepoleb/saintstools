#include "QtWidgets/QApplication"
#include "QtCore/QCommandLineParser"

#include "mainwindow.hpp"



int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QCoreApplication::setApplicationName("viewer");
    QCoreApplication::setApplicationVersion("0.0.1");
    QCommandLineParser parser;
    parser.setApplicationDescription("VPP viewer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow window;
    if (!parser.positionalArguments().isEmpty()) {
        window.loadFile(parser.positionalArguments().first());
    }
    window.show();
    return app.exec();
}
