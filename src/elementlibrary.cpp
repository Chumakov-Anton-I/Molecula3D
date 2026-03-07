#include "elementlibrary.h"
#include "elementtile.h"
#include "atom.h"

#include <QCloseEvent>
#include <QKeyEvent>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>

ElementLibrary::ElementLibrary(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle(tr("Add element from library"));

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

    auto *elementsLayout = new QHBoxLayout;
    topLayout->addLayout(elementsLayout);

    m_PTable = new QListWidget;
    //m_PTable->set
    elementsLayout->addWidget(m_PTable);

    m_eTile = new ElementTile;
    elementsLayout->addWidget(m_eTile);

    auto *groupPos = new QGroupBox(tr("Position"));
    topLayout->addWidget(groupPos);
    auto *form = new QFormLayout;
    groupPos->setLayout(form);
    m_X = new QDoubleSpinBox;
    m_Y = new QDoubleSpinBox;
    m_Z = new QDoubleSpinBox;
    double m = std::numeric_limits<double>::max();
    m_X->setRange(-m, m);
    m_Y->setRange(-m, m);
    m_Z->setRange(-m, m);

    form->addRow(" X ", m_X);
    form->addRow(" Y ", m_Y);
    form->addRow(" Z ", m_Z);

    auto *groupRadius = new QGroupBox(tr("Element"));
    topLayout->addWidget(groupRadius);
    auto *radLayout = new QVBoxLayout;
    groupRadius->setLayout(radLayout);
    m_number = new QSpinBox;
    m_number->setRange(1, 118);
    radLayout->addWidget(m_number);

    auto *bbox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal);
    topLayout->addWidget(bbox);

    connect(bbox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(bbox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    connect(m_PTable, &QListWidget::itemClicked, this, &ElementLibrary::selectElement);
}

void ElementLibrary::init()
{
    QFile ptableFile(":/data/elements.json");
    if (!ptableFile.open(QIODevice::Text | QIODevice::ReadOnly)) {
        qDebug() << "Can not open file 'data/elements.json'";
        return;
    }
    QByteArray fdata = ptableFile.readAll();
    ptableFile.close();

    QJsonParseError parse_errors;
    auto json = QJsonDocument::fromJson(fdata, &parse_errors);
    if (parse_errors.error != QJsonParseError::NoError) {
        qDebug() << "Parse error:\n" << parse_errors.errorString();
        return;
    }
    m_database.reserve(118);   // Nowadays there are just 118 chemical elements
    auto obj = json.object();
    auto elems = obj["elements"].toArray();
    for (auto it = elems.constBegin(); it != elems.constEnd(); ++it) {
        auto item = it->toObject();
        auto valence = item["valence"].toArray();
        QList<int> valences;
        for (auto vi = valence.constBegin(); vi != valence.constEnd(); ++vi)
            valences.append(vi->toInt(0));
        int key = item["number"].toInt();
        Element e;
        e.number = key;
        e.name   = item["name"].toString("Unknown");
        e.sign   = item["sign"].toString("--");
        e.group  = item["group"].toInt();
        e.period = item["period"].toInt();
        e.block  = item["block"].toString("--");
        e.type   = item["class"].toString("--");
        e.radius = item["radius"].toDouble(10.0);
        e.electronegativity = item["enegativity"].toDouble();
        e.valences = valences;
        m_database.insert(key, e);
    }

    for (int i = 1; i <= 118; ++i) {
        auto e = m_database.value(i);
        /*auto *eitem =*/ new QListWidgetItem(e.name, m_PTable);
        //m_PTable->setItem(i - 1, 0, eitem);
    }

    QFile colorFile(":/data/colors_cpk.json");
    if (!colorFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Can not open file 'data/colors_cpk.json'";
        return;
    }
    QByteArray cfdata = colorFile.readAll();
    colorFile.close();
    json = QJsonDocument::fromJson(cfdata, &parse_errors);
    if (parse_errors.error != QJsonParseError::NoError) {
        qDebug() << "Parse error:\n" << parse_errors.errorString();
        return;
    }
    m_colorbase.clear();
    m_colorbase.reserve(118);
    obj = json.object();
    auto colors = obj["colors"].toArray();
    for (auto it = colors.constBegin(); it != colors.constEnd(); ++it) {
        auto item = it->toObject();
        m_colorbase.insert(item["numb"].toInt(), item["color"].toString());    // "#1F2209"
    }
}

void ElementLibrary::accept()
{
    int numb = m_number->value();
    QVector3D pos(m_X->value(), m_Y->value(), m_Z->value());
    auto *atom = new Atom(m_database.value(numb), pos);
    QColor color(m_colorbase.value(numb));
    atom->setColor(QVector3D(color.redF(), color.greenF(), color.blueF()));
    setVisible(false);
    emit createAtom(atom);  // NB! - this dialog DOES NOT own the new atom instance!
}

void ElementLibrary::selectElement(QListWidgetItem *item)
{
    Q_UNUSED(item)
    int key = m_PTable->currentRow() + 1;
    m_eTile->setElement(m_database.value(key));
}

void ElementLibrary::closeEvent(QCloseEvent *event)
{
    setVisible(false);
    event->ignore();
}

void ElementLibrary::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
        setVisible(false);
    event->ignore();
}
