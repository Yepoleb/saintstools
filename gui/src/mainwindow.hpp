#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>

#include "Saints/Packfile.hpp"



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
    void loadFile(const QString& filename);

private slots:
    void contentContextMenuRequested(const QPoint& pos);
    void saveActionTriggered();
    void openActionTriggered();

private:
    void createUi();

    QWidget* central_widget;
    QHBoxLayout* central_layout;
    QListWidget* file_list;
    QTableWidget* content_table;
    QWidget* details_widget;

    QMenuBar* menubar;
    QMenu* file_menu;
    QStatusBar* statusbar;

    QAction* action_open;
    QAction* action_exit;

    QString m_filepath;
    QFile m_fstream;
    Saints::Packfile m_packfile;
};
