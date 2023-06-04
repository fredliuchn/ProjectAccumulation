#pragma once
#include <QtWidgets>
#include "PIEWrapper/PIEDef.h"
#include "PIEProFramework/eBaseWidget.h"

class CustomItem :public QStyledItemDelegate
{
	Q_OBJECT
public:
	CustomItem(const QRegExp& regExp,QObject* parent =nullptr);
	~CustomItem(void);

	// editing
	QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;

	void setEditorData(QWidget *editor, const QModelIndex &index)const;

	void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index)const;
private:
	QRegExp m_regExp;
};

class PointListWidget : public PIEProFramework::eBaseWidget
{
	Q_OBJECT
public:
	PointListWidget(QWidget* parent = Q_NULLPTR);
	~PointListWidget();
public:
	void AddPointList(QVector<Vector3d> points);

	QVector<Vector3d> GetPointList();
private:
	QTableWidget *m_tablewidget;
};