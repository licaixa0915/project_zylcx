#include <QTextEdit>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QMimeData>
#include <QClipboard>
#include <QStandardPaths>
#include <QDir>
#include <QFileDialog>
#include <QApplication>
#include <QBuffer>
#include "screengrab_pin.h"

#define SHADOW_BORDER 10
#define SHADOW_RADIUS 10
inline qreal dpiScaled(qreal value)
{
#ifdef Q_OS_MACOS
    // On mac the DPI is always 72 so we should not scale it
    return value;
#else
    static const qreal scale = qreal(qt_defaultDpiX()) / 96.0;
    return value * scale;
#endif
}
namespace
{
	QPushButton* newMenuButton(QWidget* parent, const QString& iconName)
	{
		// QColor clBg = KDrawHelper::getColorFromTheme("KScreenGrabPin", KDrawHelper::Prop_Background);
		// QColor clBoder = KDrawHelper::getColorFromTheme("KScreenGrabPin", KDrawHelper::Prop_Border);
		// QColor clHover = KDrawHelper::getColorFromTheme("KScreenGrabPin", "background-hover");
		// QColor clDown = KDrawHelper::getColorFromTheme("KScreenGrabPin", "background-down");

		QColor clBg = QColor("#FFFFFF");
		QColor clBoder = QColor("#EEEEEE");
		QColor clHover = QColor("#F5F5F5");
		QColor clDown = QColor("#EEEEEE");

		QString strBg = QString("rgba(%1, %2, %3, %4)").arg(clBg.red()).arg(clBg.green()).arg(clBg.blue()).arg(clBg.alpha());
		QString strBoder = QString("rgba(%1, %2, %3, %4)").arg(clBoder.red()).arg(clBoder.green()).arg(clBoder.blue()).arg(clBoder.alpha());
		QString strHover = QString("rgba(%1, %2, %3, %4)").arg(clHover.red()).arg(clHover.green()).arg(clHover.blue()).arg(clHover.alpha());
		QString strDown = QString("rgba(%1, %2, %3, %4)").arg(clDown.red()).arg(clDown.green()).arg(clDown.blue()).arg(clDown.alpha());

		QPushButton* b = new QPushButton(parent);
		b->setFixedSize(dpiScaled(32), dpiScaled(32));
		b->setStyleSheet(
			QString("\
				QPushButton{\
				border:1px solid %6;\
				border-radius: %1px;\
				color:#333333;font-family:'PingFang SC';}\
				QPushButton{background-color: %2}\
				QPushButton:on{background-color: %3}\
				QPushButton:hover{background-color: %4}\
				QPushButton:pressed{background-color: %5}")
			.arg(dpiScaled(2))
			.arg(strBg)
			.arg(strDown)
			.arg(strHover)
			.arg(strDown)
			.arg(strBoder)
		);
		b->setIcon(QPixmap(iconName));
		b->setCursor(Qt::ArrowCursor);
		return b;
	}
}

KScreenGrabPinMenu::KScreenGrabPinMenu(QWidget *parent)
	: KScreenGrabMenu(parent)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setGeometry(0, 0, dpiScaled(250), 1);
}

void KScreenGrabPinMenu::hideEvent(QHideEvent * e)
{
	emit hiding();
	KScreenGrabMenu::hideEvent(e);
}

void KScreenGrabPinMenu::focusOutEvent(QFocusEvent * e)
{
	emit focusOut();
	KScreenGrabMenu::focusOutEvent(e);
}

KScreenGrabPin::KScreenGrabPin(KScreenGrabDialog::DrawShape ds)
	: m_menu(new KScreenGrabPinMenu(nullptr))
	, m_actionShadow(nullptr)
	, m_actionCopy(nullptr)
	, m_actionSave(nullptr)
	, m_actionClose(nullptr)
	, m_moving(false)
	, m_mainWidget(new QWidget(this))
	, m_shadowOn(false)
	, m_eff(new QGraphicsDropShadowEffect())
	, m_pbShadow(nullptr)
	, m_pbCopy(nullptr)
	, m_pbSave(nullptr)
	, m_pbClose(nullptr)
	, m_imageWidget(nullptr)
	, m_drawShape(ds)
	, m_invisible(false)
{
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
	setAttribute(Qt::WA_ShowModal, false);
	setAttribute(Qt::WA_TranslucentBackground);
	setMouseTracking(true);
	setCursor(Qt::SizeAllCursor);
	m_countdown.start();
	m_mainWidget->installEventFilter(this);
	m_mainWidget->setGraphicsEffect(m_eff);
	QPalette p = palette();
    p.setBrush(QPalette::Window, QColor(0xFEFEFE));
	m_mainWidget->setAutoFillBackground(true);
	m_mainWidget->setPalette(p);

	m_eff->setOffset(0);
	m_eff->setColor(Qt::black);
	m_eff->setBlurRadius(dpiScaled(30));

	m_imageWidget = new QWidget(m_mainWidget);

	m_menu->setItemCheckable(true);
	if (!m_actionShadow)
	{
		m_actionShadow = new QAction(tr("Window Shadow"), this);
		m_menu->addAction(m_actionShadow);
		m_actionShadow->setCheckable(true);
		bool b = connect(m_actionShadow, SIGNAL(triggered()), this, SLOT(onActionShadow()));
		Q_ASSERT(b);
	}

	if (!m_actionCopy)
	{
		m_actionCopy = new QAction(tr("Copy"), this);
		m_menu->addAction(m_actionCopy);
		bool b = connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
		Q_ASSERT(b);
	}

	if (!m_actionSave)
	{
		m_actionSave = new QAction(tr("Save"), this);
		m_menu->addAction(m_actionSave);
		bool b = connect(m_actionSave, SIGNAL(triggered()), this, SLOT(onActionSave()));
		Q_ASSERT(b);
	}

	if (!m_actionClose)
	{
		m_actionClose = new QAction(tr("Close"), this);
		m_menu->addAction(m_actionClose);
		bool b = connect(m_actionClose, SIGNAL(triggered()), this, SLOT(onActionClose()));
		Q_ASSERT(b);
	}

	bool b = connect(m_menu, SIGNAL(hiding()), this, SLOT(onHiding()));
	Q_ASSERT(b);
	b = connect(m_menu, SIGNAL(focusOut()), this, SLOT(onMenuFocusOut()));
	Q_ASSERT(b);
	m_menu->hide();

	QVBoxLayout* mainLayout = new QVBoxLayout(m_mainWidget);

	m_menuLayout = new QHBoxLayout();
	m_menuLayout->setParent(m_mainWidget);
	mainLayout->addLayout(m_menuLayout);


	QString strResPath = "://icons_svg/16x16/";

	{
		m_pbShadow = newMenuButton(m_mainWidget, strResPath + "pinshadow.svg");
		m_pbShadow->setToolTip(tr("Window Shadow"));
		m_pbShadow->setCheckable(true);
		bool b = connect(m_pbShadow, SIGNAL(clicked()), this, SLOT(onActionShadow()));
		Q_ASSERT(b);
		m_menuLayout->addWidget(m_pbShadow);
		m_pbShadow->setFixedSize(32, 32);
		m_menuLayout->addSpacing(8);
	}

	{
		m_pbCopy = newMenuButton(m_mainWidget, strResPath + "copy.svg");
		m_pbCopy->setToolTip(tr("Copy"));
		bool b = connect(m_pbCopy, SIGNAL(clicked()), this, SLOT(onActionCopy()));
		Q_ASSERT(b);
		m_menuLayout->addWidget(m_pbCopy);
		m_menuLayout->addSpacing(8);
		m_pbShadow->setFixedSize(32, 32);
	}

	{
		m_pbSave = newMenuButton(m_mainWidget, strResPath + "pinsave.svg");
		m_pbSave->setToolTip(tr("Save"));
		bool b = connect(m_pbSave, SIGNAL(clicked()), this, SLOT(onActionSave()));
		Q_ASSERT(b);
		m_menuLayout->addWidget(m_pbSave);
		m_menuLayout->addSpacing(8);
		m_pbShadow->setFixedSize(32, 32);
	}

	{
		m_pbClose = newMenuButton(m_mainWidget, strResPath + "pinclose.svg");
		m_pbClose->setToolTip(tr("Close"));
		bool b = connect(m_pbClose, SIGNAL(clicked()), this, SLOT(onActionClose()));
		Q_ASSERT(b);
		m_menuLayout->addWidget(m_pbClose);
		m_menuLayout->addSpacing(8);
		m_pbShadow->setFixedSize(32, 32);
	}
	{
		//钉图到桌面之后，支持二次编辑
	}

	QRect rc = m_mainWidget->geometry();
	QRect rcLayout = QRect(rc.right() - 162, rc.y() - 5, 162, 32);
	m_menuLayout->setGeometry(rcLayout);


	mainLayout->addStretch();
	switchShadow();
}

KScreenGrabPin::~KScreenGrabPin()
{
	if (m_menu)
		m_menu->deleteLater();
	if (m_eff)
		m_eff->deleteLater();

	m_menu = nullptr;
	m_eff = nullptr;
}

void KScreenGrabPin::setImage(const QImage& image)
{
	m_image = image;
	QPalette p = palette();
    p.setBrush(QPalette::Window, image);
	m_imageWidget->setAutoFillBackground(true);
	m_imageWidget->setPalette(p);
}

void KScreenGrabPin::show(const QRect& rc)
{
	QPoint pt = mapToGlobal(rc.topLeft());
	QRect r(pt, rc.size());
	m_mainWidget->setGeometry(r);
	m_imageWidget->setGeometry(QRect(QPoint(0, 0), r.size()));

	if (r.width() < (168) || r.height() < (48))
	{
		m_pbShadow->setVisible(false);
		m_pbCopy->setVisible(false);
		m_pbSave->setVisible(false);
		m_pbClose->setVisible(false);
		m_invisible = true;
	}
	else
	{
		m_invisible = false;
	}

	QWidget::show();
}

bool KScreenGrabPin::eventFilter(QObject * o, QEvent * e)
{
	if (o == m_mainWidget)
	{
		switch (e->type())
		{
		case QEvent::MouseMove:
		{
			mouseMoveEvent(static_cast<QMouseEvent*>(e));
			return true;
		}
		case QEvent::MouseButtonPress:
		{
			mousePressEvent(static_cast<QMouseEvent*>(e));
			return true;
		}
		case QEvent::MouseButtonRelease:
		{
			mouseReleaseEvent(static_cast<QMouseEvent*>(e));
			return true;
		}
		}
	}

	return QObject::eventFilter(o, e);
}

void KScreenGrabPin::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Escape)
	{
		onActionClose();
	}
}

void KScreenGrabPin::contextMenuEvent(QContextMenuEvent* event)
{
	QPoint pt = event->pos();
	pt = mapToGlobal(pt);

	m_menu->resetHoveredAction();
	m_menu->move(pt);
	m_menu->show();
	m_menu->raise();
	m_menu->setFocus();
}

void KScreenGrabPin::leaveEvent(QEvent *)
{
	m_pbShadow->setVisible(false);
	m_pbCopy->setVisible(false);
	m_pbSave->setVisible(false);
	m_pbClose->setVisible(false);
}

void KScreenGrabPin::enterEvent(QEnterEvent *)
{
	if (!m_invisible)
	{
		m_pbShadow->setVisible(true);
		m_pbCopy->setVisible(true);
		m_pbSave->setVisible(true);
		m_pbClose->setVisible(true);
	}
}

void KScreenGrabPin::mousePressEvent(QMouseEvent * event)
{
	if ((event->button() & Qt::LeftButton) && m_menu)
	{
		hideMenu();
	}

	if (m_countdown.elapsed() > 10 || !m_countdown.isValid())
	{
		if (event->button() & Qt::LeftButton)
		{
			m_moving = true;
			m_down = event->pos();
		}
	}
}

void KScreenGrabPin::mouseMoveEvent(QMouseEvent * event)
{
	if (m_moving)
	{
		QPoint pt = geometry().topLeft() + event->pos() - m_down;
		move(pt);
	}
}

void KScreenGrabPin::mouseReleaseEvent(QMouseEvent * event)
{
	if (event->button() & Qt::LeftButton)
	{
		m_down = QPoint();
		m_moving = false;
	}
}

void KScreenGrabPin::showEvent(QShowEvent *)
{
	QRect rc = m_mainWidget->geometry();
	rc.adjust(
		dpiScaled(-SHADOW_BORDER),
		dpiScaled(-SHADOW_BORDER),
		dpiScaled(SHADOW_BORDER),
		dpiScaled(SHADOW_BORDER)
	);
	rc.translate(-dpiScaled(SHADOW_BORDER), -dpiScaled(SHADOW_BORDER));
	setGeometry(rc);

	m_mainWidget->move(dpiScaled(SHADOW_BORDER), dpiScaled(SHADOW_BORDER));
	QRect rect = m_mainWidget->geometry();
	QRect rcLayout = QRect(rect.right() - 162, rect.y() - 5, 162, 32);
	m_menuLayout->setGeometry(rcLayout);
}

void KScreenGrabPin::onHiding()
{
	m_moving = false;
	m_countdown.restart();
}

void KScreenGrabPin::onMenuFocusOut()
{
	hideMenu();
}

void KScreenGrabPin::onActionCopy()
{
	QClipboard* clipboard = QApplication::clipboard();
	if (NULL != clipboard)
	{
		QMimeData *data = new QMimeData;
		QByteArray byteArray;
		QBuffer buffer(&byteArray);
		buffer.open(QIODevice::WriteOnly);
		m_image.save(&buffer, "PNG", 80);
#ifdef Q_OS_WIN
		data->setData(QLatin1String("PNG"), byteArray);
#else
		data->setData(QLatin1String("image/png"), byteArray);
#endif
		data->setImageData(QVariant(m_image));
		clipboard->setMimeData(data);
		buffer.close();

	}
}

void KScreenGrabPin::onActionShadow()
{
	m_actionShadow->setCheckable(true);

	switchShadow();
}

void KScreenGrabPin::onActionSave()
{
	QDateTime current = QDateTime::currentDateTime();
	QString strFilename = tr("Capture%1").arg(current.toString("yyyyMMddhhmmss"));
#ifdef Q_OS_UNIX
	strFilename.append(".png");
#endif

	QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
	strFilename = defaultPath + QDir::separator() + strFilename;
	QString filter;
	char szFilterPNG[] = "png";
	char szFilterBMP[] = "bmp";
	char szFilterJPG[] = "jpg";

	QString png = "PNG (*.png)";
	QString bmp = "BMP(*.bmp)";
	QString jpg = "JPEG(*.jpg *.jpeg)";
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Save As"),
		strFilename,
		tr("%1;;%2;;%3").arg(png).arg(bmp).arg(jpg),
		&filter);

	if (filename.isEmpty())
		return;

	QStringList filenameList = filename.split("/");
	if (filenameList.count() <= 2 || filenameList.at(1) == "Volumes")
	{
		KMessageBoxDialog messageDlg(this);
		messageDlg.setContent(tr("Invalid path, please select again."));
		messageDlg.exec();
		return;
	}

	char *szFilter = 0;
	if (png == filter)
		szFilter = szFilterPNG;
	else if (bmp == filter)
		szFilter = szFilterBMP;
	else if (jpg == filter)
		szFilter = szFilterJPG;
	else
		szFilter = 0;

	if (!filename.isEmpty() && !filter.isEmpty() && 0 != szFilter)
	{
		m_image.save(filename, szFilter, 80);
	}
}

void KScreenGrabPin::onActionClose()
{
	close();
}

void KScreenGrabPin::hideMenu()
{
	m_menu->resetHoveredAction();
	m_menu->hide();
	setFocus();
}

void KScreenGrabPin::switchShadow()
{
	m_shadowOn = !m_shadowOn;

	m_actionShadow->setChecked(m_shadowOn);
	m_eff->setEnabled(m_shadowOn);
	if (m_pbShadow)
		m_pbShadow->setChecked(m_shadowOn);
	update();
}
