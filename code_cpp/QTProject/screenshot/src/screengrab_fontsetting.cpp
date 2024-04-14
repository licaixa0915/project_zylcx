#include "screengrab_fontsetting.h"
#include <QEvent>
#include <QMouseEvent>
#include <QPainter>
KScreenGrabFontSetting::KScreenGrabFontSetting(QWidget *parent)
	: QWidget(parent)
{
// 	m_comboBoxSize = new KLiteComboBox(this);
// 	m_comboBoxSize->setProperty("qtspyName", "FontSizeList");
// 	QIcon ic;
// 	ic.addFile("://res/16x16/arrow_down.svg", QSize(12, 12), QIcon::Disabled);
// 	ic.addFile("://res/16x16/arrow_up.svg", QSize(12, 12), QIcon::Active);
// 	m_comboBoxSize->setArrowIcon(ic, QSize(12, 12));
// 	m_comboBoxSize->addItem("8");
// 	m_comboBoxSize->addItem("9");
// 	m_comboBoxSize->addItem("10");
// 	m_comboBoxSize->addItem("11");
// 	m_comboBoxSize->addItem("12");
// 	m_comboBoxSize->addItem("14");
// 	m_comboBoxSize->addItem("16");
// 	m_comboBoxSize->addItem("18");
// 	m_comboBoxSize->addItem("20");
// 	m_comboBoxSize->addItem("22");

// 	m_comboBoxSize->setCurrentIndex(4);

// 	connect(m_comboBoxSize, SIGNAL(currentIndexChanged(const QString&)),
// 		this, SLOT(currentIndexChanged(const QString&)));

// 	setFixedWidth(dpiScaled(87));
// 	setFixedHeight(dpiScaled(30));
// 	m_comboBoxSize->setMaxVisibleItems(6);
// 	connect(kcoreApp, SIGNAL(notifyAppSkinChange(QString)), this, SLOT(onSkinChanged()));
// 	onSkinChanged();
// #ifdef Q_OS_MACOS
// 	m_comboBoxSize->setFixedSize(56, 24);
// 	m_comboBoxSize->move(dpiScaled(28), dpiScaled(2));
// #else
// 	m_comboBoxSize->setFixedSize(50, 24);
// 	m_comboBoxSize->move(dpiScaled(30), dpiScaled(2));
// #endif	
// 	m_comboBoxSize->show();
}

KScreenGrabFontSetting::~KScreenGrabFontSetting()
{

}

void KScreenGrabFontSetting::onSkinChanged()
{
	m_icon = QIcon("://icons_svg/16x16/text.svg");
}

bool KScreenGrabFontSetting::event( QEvent* event )
{
	return QWidget::event(event);
}

bool KScreenGrabFontSetting::eventFilter( QObject* obj, QEvent* event )
{
	switch (event->type())
	{
	case QEvent::MouseMove:
		mouseMoveEvent(static_cast<QMouseEvent*>(event));
		break;

	case QEvent::MouseButtonPress:
		mousePressEvent(static_cast<QMouseEvent*>(event));
		break;

	case QEvent::MouseButtonRelease:
		mouseReleaseEvent(static_cast<QMouseEvent*>(event));
		break;

	default:
		break;
	}

	return QWidget::event(event);
}

void KScreenGrabFontSetting::paintEvent( QPaintEvent* event )
{
	QPainter painter(this);
	m_icon.paint(&painter, (4), (6), (16), (16));
}

void KScreenGrabFontSetting::mousePressEvent( QMouseEvent* event )
{

}

void KScreenGrabFontSetting::mouseReleaseEvent( QMouseEvent* event )
{

}

void KScreenGrabFontSetting::mouseMoveEvent( QMouseEvent* event )
{

}

void KScreenGrabFontSetting::currentIndexChanged(const QString& text)
{
	bool ok = false;
	m_nSize = text.toInt(&ok);
	if (ok)
	{
		emit(fontSizeChanged(m_nSize));
	}
}
