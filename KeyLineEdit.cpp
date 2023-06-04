#include "AppCore/KeyLineEdit/KeyLineEdit.h"

KeyLineEdit::KeyLineEdit(QWidget *parent /*= nullptr*/)
	: QLineEdit(parent)
{
	//设置setReadOnly能解决和输入法冲突的问题，但需要重新实现selected方法，否则选中状态不明显
	//setReadOnly(true);
	setText("无");
}

KeyLineEdit::~KeyLineEdit()
{
}

void KeyLineEdit::setKey(int key)
{
	Qt::Key qkey = static_cast<Qt::Key>(key);
	if (qkey != Qt::Key_unknown)
	{
		m_key = qkey;
		if (m_altModifier)
		{
			m_key += Qt::ALT;
		}
		if (m_ctrlModifier)
		{
			m_key += Qt::CTRL;
		}
		setText(QKeySequence(m_key).toString());
	}
	else
	{
		m_key = Qt::Key_unknown;
		setText("无");
	}
}

int KeyLineEdit::getKey()
{
	if (m_altModifier)
	{
		m_key += Qt::ALT;
	}
	if (m_ctrlModifier)
	{
		m_key += Qt::CTRL;
	}
	return m_key;
}

void KeyLineEdit::setText(QString qsText)
{
	QLineEdit::setText(qsText);
}

void KeyLineEdit::keyPressEvent(QKeyEvent *e)
{
	int uKey = e->key();
	Qt::Key key = static_cast<Qt::Key>(uKey);
	if (key == Qt::Key_unknown)
	{
		return;
	}
	//除去单个的特殊按键
	if (key == Qt::Key_Control
		|| key == Qt::Key_Shift
		|| key == Qt::Key_Alt
		|| key == Qt::Key_Enter
		|| key == Qt::Key_Return
		|| key == Qt::Key_Tab
		|| key == Qt::Key_CapsLock
		|| key == Qt::Key_Escape)
	{
		return;
	}
	if (key == Qt::Key_Backspace)
	{
		sigKeyChange(Qt::Key_unknown);
		m_key = Qt::Key_unknown;
		setText("无");
		return;
	}
	bool bModifiers = false;
	Qt::KeyboardModifiers modifiers = e->modifiers();
	if (modifiers & Qt::ShiftModifier)
	{
		uKey += Qt::SHIFT;
		bModifiers = true;
		setText("无");
		m_key = Qt::Key_unknown;
		sigKeyChange(Qt::Key_unknown);
		return;
	}
	if (modifiers & Qt::ControlModifier)
	{
		uKey += Qt::CTRL;
		bModifiers = true;
		m_ctrlModifier = true;
	}
	if (modifiers & Qt::AltModifier)
	{
		uKey += Qt::ALT;
		bModifiers = true;
		m_altModifier = true;
	}

	//不支持单个数字 字母
	bool bNum = (e->key() >= Qt::Key_0 && e->key() <= Qt::Key_9);
	//bool bChar = (e->key() >= Qt::Key_A && e->key() <= Qt::Key_Z);
	if ((bNum /*|| bChar*/) && !bModifiers)
	{
		QMessageBox::warning(NULL, QObject::tr("提示"), QObject::tr("快捷键不支持单独数字!"), QMessageBox::Yes);
		setText("无");
		m_key = Qt::Key_unknown;
		sigKeyChange(Qt::Key_unknown);
		return;
	}
	QString qsKey = QKeySequence(uKey).toString();
	//除去Windows常用快捷键
	QStringList blackList;
	blackList << "CTRL+S" << "CTRL+C" << "CTRL+V" << "CTRL+A" << "CTRL+D" << "CTRL+Z" << "CTRL+X";
	if (blackList.contains(qsKey, Qt::CaseInsensitive))
	{
		return;
	}
	setText(qsKey);
	m_key = key;
	emit sigKeyChange(uKey);
}

