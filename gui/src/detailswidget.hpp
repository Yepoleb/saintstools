#include <QtWidgets/QtWidgets>

#include "Saints/Packfile.hpp"



class PackfileDetails: public QWidget
{
    Q_OBJECT

public:
    PackfileDetails(QWidget* parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());

    void loadPackfile(const Saints::Packfile& file);

private:
    QFormLayout* m_layout;

    QLabel* m_version_label;
    QLabel* m_flags_label;
    QLabel* m_num_files_label;

    QLabel* m_version_value;
    QLabel* m_flags_value;
    QLabel* m_num_files_value;
};
