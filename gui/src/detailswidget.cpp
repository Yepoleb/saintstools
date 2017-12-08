#include <QtWidgets/QtWidgets>

#include "Saints/Packfile.hpp"

#include "detailswidget.hpp"



PackfileDetails::PackfileDetails(QWidget* parent, Qt::WindowFlags f):
    QWidget(parent, f)
{
    m_layout = new QFormLayout(this);
    m_version_label = new QLabel("Version:", this);
    m_flags_label = new QLabel("Flags:", this);
    m_num_files_label = new QLabel("Number of files:", this);

    m_version_value = new QLabel(this);
    m_flags_value = new QLabel(this);
    m_num_files_value = new QLabel(this);

    m_layout->setWidget(0, QFormLayout::LabelRole, m_version_label);
    m_layout->setWidget(0, QFormLayout::FieldRole, m_version_value);
    m_layout->setWidget(1, QFormLayout::LabelRole, m_flags_label);
    m_layout->setWidget(1, QFormLayout::FieldRole, m_flags_value);
    m_layout->setWidget(2, QFormLayout::LabelRole, m_num_files_label);
    m_layout->setWidget(2, QFormLayout::FieldRole, m_num_files_value);
}

void PackfileDetails::loadPackfile(const Saints::Packfile& file)
{
    QString flags_str;
    if ((file.getFlags() & Saints::Packfile::Compressed) && (file.getFlags() & Saints::Packfile::Condensed)) {
        flags_str = "Compressed, Condensed";
    } else if (file.getFlags() & Saints::Packfile::Compressed) {
        flags_str = "Compressed";
    } else if (file.getFlags() & Saints::Packfile::Condensed) {
        flags_str = "Condensed";
    } else {
        flags_str = "None";
    }

    m_version_value->setText(QString::number(file.getVersion()));
    m_flags_value->setText(flags_str);
    m_num_files_value->setText(QString::number(file.getEntriesCount()));
}
