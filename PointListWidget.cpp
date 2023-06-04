#include "PointListWidget.h"
#include "PIEProFramework/eService.h"

CustomItem::CustomItem(const QRegExp& regExp, QObject* parent)
{
	m_regExp = regExp;
}

CustomItem::~CustomItem()
{

}

QWidget *CustomItem::createEditor(QWidget *parent,const QStyleOptionViewItem &option,const QModelIndex &index)const
{
	Q_UNUSED(option);
	Q_UNUSED(index);
	QLineEdit* editor = new QLineEdit(parent);
	editor->setValidator(new QRegExpValidator(m_regExp, parent));
	return editor;
}

void CustomItem::setEditorData(QWidget *editor, const QModelIndex &index)const
{
	QString text = index.model()->data(index, Qt::EditRole).toString();
	QLineEdit* lineedit = qobject_cast<QLineEdit*>(editor);
	lineedit->setText(text);
}

void CustomItem::setModelData(QWidget *editor,QAbstractItemModel *model,const QModelIndex &index)const
{
	QLineEdit* lineedit = qobject_cast<QLineEdit*>(editor);
	QString text = lineedit->text();
	model->setData(index, text, Qt::EditRole);
}

PointListWidget::PointListWidget(QWidget* parent)
	:eBaseWidget(parent)
{
	QVBoxLayout* mainlayout = new QVBoxLayout(this);
	m_tablewidget = new QTableWidget(this);
	//单行可选择
	m_tablewidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	//显示网格
	m_tablewidget->setShowGrid(true);

	//表头显隐
	m_tablewidget->verticalHeader()->setVisible(false);
	m_tablewidget->horizontalHeader()->setVisible(false);
	m_tablewidget->setColumnCount(4);
	m_tablewidget->horizontalHeader()->setDefaultAlignment(Qt::AlignHCenter);
	m_tablewidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	m_tablewidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
	m_tablewidget->setColumnWidth(0, 50);
	mainlayout->addWidget(m_tablewidget);
	QHBoxLayout *btnlayout = new QHBoxLayout;
	QSpacerItem* spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	btnlayout->addItem(spacer);
	QPushButton* btn=new QPushButton(this);
	btn->setText(tr("确定"));
	connect(btn, &QPushButton::clicked, this, &QDialog::accept);
	btnlayout->addWidget(btn);
	mainlayout->addLayout(btnlayout);
	this->setStyleSheet(g_appInfo->DialogStyle());
	this->resize(350, 300);
}

PointListWidget::~PointListWidget()
{

}

void PointListWidget::AddPointList(QVector<Vector3d> points)
{
	m_tablewidget->setRowCount(points.count() + 1);
	m_tablewidget->setItem(0, 0, new QTableWidgetItem(tr("序号")));
	m_tablewidget->setItem(0, 1, new QTableWidgetItem(tr("经度(°)")));
	m_tablewidget->setItem(0, 2, new QTableWidgetItem(tr("纬度(°)")));
	m_tablewidget->setItem(0, 3, new QTableWidgetItem(tr("高度(m)")));
	m_tablewidget->item(0, 0)->setTextAlignment(Qt::AlignHCenter);
	m_tablewidget->item(0, 1)->setTextAlignment(Qt::AlignHCenter);
	m_tablewidget->item(0, 2)->setTextAlignment(Qt::AlignHCenter);
	m_tablewidget->item(0, 3)->setTextAlignment(Qt::AlignHCenter);
	m_tablewidget->item(0, 0)->setFlags(Qt::ItemIsEnabled);
	m_tablewidget->item(0, 1)->setFlags(Qt::ItemIsEnabled);
	m_tablewidget->item(0, 2)->setFlags(Qt::ItemIsEnabled);
	m_tablewidget->item(0, 3)->setFlags(Qt::ItemIsEnabled);
	for (int i = 0;i < points.count();++i)
	{
		m_tablewidget->setItem(i + 1, 0, new QTableWidgetItem(QString::number(i + 1)));
		m_tablewidget->setItem(i + 1, 1, new QTableWidgetItem(QString::number(points[i].x, 'f', 6)));
		m_tablewidget->setItem(i + 1, 2, new QTableWidgetItem(QString::number(points[i].y, 'f', 6)));
		m_tablewidget->setItem(i + 1, 3, new QTableWidgetItem(QString::number(points[i].z, 'f', 2)));

		m_tablewidget->item(i + 1, 0)->setTextAlignment(Qt::AlignHCenter);
		m_tablewidget->item(i + 1, 1)->setTextAlignment(Qt::AlignHCenter);
		m_tablewidget->item(i + 1, 2)->setTextAlignment(Qt::AlignHCenter);
		m_tablewidget->item(i + 1, 3)->setTextAlignment(Qt::AlignHCenter);
		m_tablewidget->item(i + 1, 0)->setFlags(Qt::ItemIsEnabled);
		m_tablewidget->item(i + 1, 1)->setFlags(Qt::ItemIsEnabled);
		m_tablewidget->item(i + 1, 2)->setFlags(Qt::ItemIsEnabled);
		m_tablewidget->item(i + 1, 3)->setFlags(Qt::ItemIsEnabled | Qt::ItemIsEditable);
	}
	QRegExp rx("^(-?[0]|-?[1-9][0-9]{0,5})(?:\\.\\d{1,4})?$|(^\\t?$)");
	m_tablewidget->setItemDelegateForColumn(3, new CustomItem(rx, m_tablewidget));
	m_tablewidget->update();
}

QVector<Vector3d> PointListWidget::GetPointList()
{
	QVector<Vector3d> points;
	int nItemCount = m_tablewidget->rowCount();
	for (int i = 1;i < nItemCount;++i)
	{
		QString strLon = m_tablewidget->item(i, 1)->text();
		QString strLat = m_tablewidget->item(i, 2)->text();
		QString strheight = m_tablewidget->item(i, 3)->text();
		points.push_back(Vector3d(strLon.toDouble(), strLat.toDouble(), strheight.toDouble()));
	}
	return points;
}

