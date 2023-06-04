#pragma once

#include <QtWidgets>
#include "AppCore/AppCore_global.h"

class APPCORE_EXPORT KeyLineEdit : public QLineEdit
{
	Q_OBJECT
public:
	KeyLineEdit(QWidget *parent = nullptr);
	~KeyLineEdit();

public:
	void setKey(int key);
	int getKey();
	void setText(QString qsText);

signals:
	void sigKeyChange(int key);

protected:
	void keyPressEvent(QKeyEvent *e) override;

private:
	int m_key = Qt::Key_unknown;
	bool m_ctrlModifier = false;
	bool m_altModifier = false;
};