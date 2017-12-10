#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>

#include "detailswidget.hpp"
#include "mainwindow.hpp"



constexpr Qt::ItemFlags DEFAULT_ITEM_FLAGS = Qt::ItemIsSelectable | Qt::ItemIsEnabled;



MainWindow::MainWindow()
{
    createUi();
}

MainWindow::~MainWindow()
{
    m_fstream.close();
}

void MainWindow::createUi()
{
    resize(800, 600);
    setWindowTitle("Saints Row Packfile Viewer");

    central_widget = new QWidget(this);
    central_layout = new QHBoxLayout(central_widget);
    content_table = new QTableWidget(0, 4, central_widget);
    details_widget = new PackfileDetails(central_widget);

    setCentralWidget(central_widget);
    central_layout->addWidget(content_table);
    central_layout->addWidget(details_widget);
    QSizePolicy content_table_policy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSizePolicy details_widget_policy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    content_table->setSizePolicy(content_table_policy);
    details_widget->setSizePolicy(details_widget_policy);

    QStringList table_header({"Name", "Type", "Size", "Compressed"});
    content_table->setHorizontalHeaderLabels(table_header);
    content_table->verticalHeader()->setVisible(false);
    content_table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    content_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    content_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    content_table->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(content_table, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(contentContextMenuRequested(const QPoint&)));

    menubar = new QMenuBar(this);
    file_menu = new QMenu("File", menubar);
    action_open = new QAction("Open", this);
    action_exit = new QAction("Exit", this);
    statusbar = new QStatusBar(this);

    setMenuBar(menubar);
    menubar->addAction(file_menu->menuAction());
    file_menu->addAction(action_open);
    file_menu->addAction(action_exit);
    setStatusBar(statusbar);

    connect(action_open, SIGNAL(triggered()), this, SLOT(openActionTriggered()));
    connect(action_exit, SIGNAL(triggered()), this, SLOT(close()));
}

void MainWindow::loadFile(const QString& filepath)
{
    m_filepath = filepath;
    m_fstream.setFileName(filepath);
    m_fstream.open(QIODevice::ReadOnly);
    m_packfile.open(m_fstream);

    content_table->clearContents();
    content_table->setRowCount(m_packfile.getEntriesCount());
    for (size_t entry_i = 0; entry_i < m_packfile.getEntriesCount(); entry_i++) {
        const Saints::PackfileEntry& entry = m_packfile.getEntry(entry_i);

        QString name_str(entry.getFilename());
        QString type_str(QFileInfo(name_str).suffix());
        QString size_str(QString("%L1").arg(entry.getSize()));
        QString compressed_str(
            entry.getFlags() & Saints::Packfile::Compressed ? "Yes" : "No");

        QTableWidgetItem* name_item(new QTableWidgetItem(name_str));
        QTableWidgetItem* type_item(new QTableWidgetItem(type_str));
        QTableWidgetItem* size_item(new QTableWidgetItem(size_str));
        QTableWidgetItem* compressed_item(new QTableWidgetItem(compressed_str));

        name_item->setFlags(DEFAULT_ITEM_FLAGS | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled);
        type_item->setFlags(DEFAULT_ITEM_FLAGS);
        size_item->setFlags(DEFAULT_ITEM_FLAGS);
        compressed_item->setFlags(DEFAULT_ITEM_FLAGS);
        size_item->setTextAlignment(Qt::AlignRight | Qt::AlignCenter);
        compressed_item->setTextAlignment(Qt::AlignHCenter | Qt::AlignCenter);

        content_table->setItem(entry_i, 0, name_item);
        content_table->setItem(entry_i, 1, type_item);
        content_table->setItem(entry_i, 2, size_item);
        content_table->setItem(entry_i, 3, compressed_item);
    }

    PackfileDetails* packfile_details = dynamic_cast<PackfileDetails*>(details_widget);
    packfile_details->loadPackfile(m_packfile);
}

void MainWindow::contentContextMenuRequested(const QPoint& pos)
{
    QMenu context_menu(this);

    QAction save_action("Save File", this);
    connect(&save_action, SIGNAL(triggered()), this, SLOT(saveActionTriggered()));
    context_menu.addAction(&save_action);

    context_menu.exec(content_table->viewport()->mapToGlobal(pos));
}

void MainWindow::saveActionTriggered()
{
    QModelIndexList selected_rows(content_table->selectionModel()->selectedRows());

    QDir save_dir(QFileDialog::getExistingDirectory(this, "Select Directory"));

    for (const QModelIndex& index : selected_rows) {
        Saints::PackfileEntry& entry(m_packfile.getEntry(index.row()));

        QFile file(save_dir.filePath(entry.getFilename()));
        file.open(QIODevice::WriteOnly);
        file.write(entry.getData());
        file.close();
    }
}

void MainWindow::openActionTriggered()
{
    QString filename(QFileDialog::getOpenFileName(
        this, "Open File", QString(), "Packfiles (*.vpp_pc *.str2_pc)"));
    loadFile(filename);
}
