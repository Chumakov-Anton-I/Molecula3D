#include "elementlibrary.h"
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

ElementLibrary::ElementLibrary(QWidget *parent)
    : QDialog{parent}
{
    setWindowTitle(tr("Add element from library"));

    auto *topLayout = new QVBoxLayout;
    setLayout(topLayout);

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
        e.group  = item["group"].toInt(0);
        e.period = item["period"].toInt(0);
        e.block  = item["block"].toString("--");
        e.type   = item["class"].toString("--");
        e.radius = item["radius"].toDouble(10.0);
        e.electronegativity = item["enegativity"].toDouble(0.0);
        e.valences = valences;
        m_database.insert(key, e);
    }
}

void ElementLibrary::accept()
{
    int numb = m_number->value();
    QVector3D pos(m_X->value(), m_Y->value(), m_Z->value());
    auto *atom = new Atom(m_database.value(numb), pos);
    setVisible(false);
    emit createAtom(atom);  // NB! - this dialog DOES NOT own the new atom instance!
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
