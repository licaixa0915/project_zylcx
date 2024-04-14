#include <QPushButton>
#include <QDebug>
#include <QApplication>
#include <QScreen>
#include <QPainter>
#include "windowinfo.h"
#include "screengrab_colorpicker.h"
#include "screengrab_fontsetting.h"
#include "screengrab_pin.h"
#include "screengrab_pen.h"
#include "screengrab_hint.h"
#include "kscreengrabdialog.h"

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

#define __STRINGIFY( _x ) # _x
#define STRINGIFY( _x ) __STRINGIFY( _x )
namespace
{
	const QString STR_GRAB_SCREEN = "GrabScreen";
	const QString STR_HIDE_WHILE_GRABBING = "HideWhileGrabbingScreen";
	const int LOCAL_SCREEN_GRAB_GAP_SELECTION_TOOLBAR = 5;
	Qt::CursorShape cursorHit[] =
	{
	// WARNING, "corsorHit[]" is defined associated with "enum HitTest"
	// SEE ALSO "enum HitTestResult" IN "screengrab_dialog.h"
		Qt::ArrowCursor,
		Qt::SizeAllCursor,
		Qt::SizeVerCursor,
		Qt::SizeVerCursor,
		Qt::SizeHorCursor,
		Qt::SizeHorCursor,
		Qt::SizeFDiagCursor,
		Qt::SizeBDiagCursor,
		Qt::SizeBDiagCursor,
		Qt::SizeFDiagCursor,
	};

	const int NUM_POINTS_INIT = 100;

	const int NUM_EDIT_CIRCLE = 8;

	const QString STR_DEFAULT_FONT_FAMILY = "PingFang SC";

	const QColor colorSelectionEdge = QColor(79, 133, 255);

	const QPoint ptNullPressedPoint(-1, -1);

	const int roundedRadius = 20;

	const QSize szToolbarBtn = QSize(dpiScaled(26), dpiScaled(26));
}

namespace KScreenGrabHelper
{
const QString strType_ScrnGrab = "kscrngrab";
static const char* Reason = "reason";
const int NoReason = 0;
const int NeedLoginOrVIP = -1;
}
class Settings : public QSettings
{
public:
	Settings();
	~Settings();

	static QSettings::Format XmlFormat;

private:
	QString _organizationName() const;
};
QSettings::Format Settings::XmlFormat = QSettings::InvalidFormat;

Settings::Settings()
	: QSettings(QSettings::NativeFormat, QSettings::UserScope, _organizationName(), "Office\\6.0\\plugins")
{

}

Settings::~Settings()
{

}

QString Settings::_organizationName() const
{
	return QApplication::instance()->organizationName();
}
static QRect calcCenterRect(QRect rc, QSize sz)
{
	int x = (rc.width() - sz.width()) / 2 + rc.x();
	int y = (rc.height() - sz.height()) / 2 + rc.y();
	return QRect(QPoint(x, y), sz);
}

struct _LineColorResult
{
	int nTotal;
	int nNeed;
	QRgb rgb;
};
static _LineColorResult getLineColor(const QImage& img, const QLine& line)
{
	bool isHoriz = line.y1() == line.y2();
	int from = isHoriz ? line.x1() : line.y1();
	int to = isHoriz ? line.x2() : line.y2();

	int length = to - from;
	int nIgnoreMax = length / 6, nIgnore = 0;
	int nTotalNeed = length / 2, nTotal = 0;

	_LineColorResult result;
	result.nNeed = nTotalNeed;

	QRgb rgb = RGB_MASK;
	for (int i = from; i <= to; ++ i)
	{
		QRgb rgbTemp;
		if (isHoriz)
			rgbTemp = img.pixel(i, line.y1());
		else
			rgbTemp = img.pixel(line.x1(), i);
		if (rgb == rgbTemp)
		{
			if (++ nTotal > nTotalNeed)
			{
				result.rgb = rgbTemp;
				break;
			}
		}
		else if (nIgnore < nIgnoreMax)
			++ nIgnore;
		else
		{
			rgb = rgbTemp;
			nTotal = nIgnore = 0;
		}
	}
	result.nTotal = nTotal;
	return result;
}

static QPair<bool, int> findLine(const QImage& img, const QLine& line, int step)
{
	bool isHoriz = line.y1() == line.y2();
	int direction = step > 0 ? 1 : -1;
	QPoint offset = QPoint((!isHoriz) * direction, (isHoriz) * direction);
	QLine l = line;
	_LineColorResult result = getLineColor(img, l);
	for (int i = 1; i <= qAbs(step); ++ i)
	{
		l.translate(offset);

		_LineColorResult tmp = getLineColor(img, l);
		if (tmp.nNeed < tmp.nTotal)
		{
			if (i > 1 && (tmp.nTotal < tmp.nNeed * 0.6 ||
				tmp.rgb != result.rgb))
			{
				return qMakePair(true,
					(isHoriz) ? l.y1() - offset.y() : l.x1() - offset.x());
			}
			result = tmp;
			continue;
		}

		if (result.nTotal < result.nNeed * 0.6 ||
			result.rgb != tmp.rgb)
			return qMakePair(true, (isHoriz) ? l.y1() : l.x1());
	}
	return qMakePair(false, 0);
}

void setTextFontColor(
	QTextEdit* edit,
	const QString& family,
	const int size,
	const QColor& color)
{
	QTextCursor cursor = edit->textCursor();
	int start = cursor.selectionStart();
	int end = cursor.selectionEnd();

	QTextCharFormat format = edit->currentCharFormat();
	format.setFont(QFont(family, size));
	format.setForeground(QBrush(color));

	cursor.setCharFormat(format);
	edit->setTextCursor(cursor);

	if (!edit->toPlainText().isEmpty())
	{
		edit->setText(edit->toPlainText());

		cursor.setPosition(start);
		edit->setTextCursor(cursor);
		cursor.setPosition(end, QTextCursor::KeepAnchor);
		edit->setTextCursor(cursor);
	}
}

// -----------------------------------------------------------------------
#define TRI_Y 8 // 三角形Y坐标

KScreenGrabTip::KScreenGrabTip(KScreenGrabDialog* parent /*= nullptr*/)
	: QWidget(parent)
	, m_main(nullptr)
	, m_label(nullptr)
	, m_dlg(parent)
	, m_title(nullptr)
{
	setAttribute(Qt::WA_TranslucentBackground);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	// bool b = connect(kcoreApp, SIGNAL(notifyAppSkinChange(QString)), this, SLOT(onChangeSkinMode()));
	// Q_ASSERT(b);
}

void KScreenGrabTip::reset(QWidget* btn)
{
	const QSize* szPtr = m_dlg->getTooltipSizeHint(btn);
	if (szPtr)
		setGeometry(QRect(geometry().topLeft(), *szPtr));

	QRect rc = geometry();
	rc.adjust(dpiScaled(2), dpiScaled(TRI_Y), dpiScaled(-2), dpiScaled(-2));

	if (!m_main)
	{
		m_main = new QWidget(this);
		m_main->installEventFilter(this);
		QVBoxLayout* vl = new QVBoxLayout(m_main);
		vl->setSpacing(dpiScaled(8));

		m_title = new QLabel(m_main);
		m_title->setStyleSheet(" background-color: white; color: black");
		QFont font;
		font.setPixelSize(14);
		font.setFamily("PingFang SC");
		m_title->setFont(font);
		m_title->setText(tr("Intro:"));
		vl->addSpacing(dpiScaled(6));
		vl->addWidget(m_title);

		m_label = new QLabel(m_main);
		vl->addWidget(m_label);
		vl->addStretch();
	}

	if (!m_main->graphicsEffect())
	{
		QGraphicsDropShadowEffect* eff = new QGraphicsDropShadowEffect();
		eff->setOffset(0);
		eff->setBlurRadius(dpiScaled(8));
		m_main->setGraphicsEffect(eff);
	}

	m_main->setGeometry(QRect(QPoint(dpiScaled(2), dpiScaled(TRI_Y)), rc.size()));
	m_main->setGeometry(QRect(QPoint(0, 0), rc.size()));
	m_label->setAlignment(Qt::AlignTop);
	m_label->setText(m_text);
	onChangeSkinMode();
}

bool KScreenGrabTip::eventFilter(QObject * o, QEvent * e)
{
	if (o == m_main)
	{
		QWidget* w = qobject_cast<QWidget*>(o);
		if (w)
		{
			paintEvent(w, static_cast<QPaintEvent*>(e));
			return true;
		}
	}
	return QWidget::eventFilter(o, e);
}

void KScreenGrabTip::onChangeSkinMode()
{
	// QColor cl = KDrawHelper::getColorFromTheme("KScreenGrabTip", KDrawHelper::Prop_Text);
	QColor cl = QColor(Qt::black);
	QString strText = QString("rgba(%1, %2, %3, %4)").arg(cl.red()).arg(cl.green()).arg(cl.blue()).arg(cl.alpha());
	if (m_title)
		m_title->setStyleSheet(QString("QLabel{background-color: transparent;font-family: 'PingFang SC' ;color: %1;font-size:14px;}").arg(strText));

	if (m_label)
		m_label->setStyleSheet(QString("QLabel{background-color: transparent;font-family: 'PingFang SC' ;color: %1;font-size:12px;}").arg(strText));
}

void KScreenGrabTip::paintEvent(QWidget* w, QPaintEvent *)
{
	// QColor btnCl = KDrawHelper::getColorFromTheme("KScreenGrabTip", KDrawHelper::Prop_Background);
	QColor btnCl = QColor(Qt::black);
	QPainter painter(w);
	painter.setRenderHint(QPainter::Antialiasing);
	QPainterPath pathT, pathR;
	pathR.moveTo(dpiScaled(16), dpiScaled(TRI_Y));
	pathR.lineTo(dpiScaled(16 + 6), 0);
	pathR.lineTo(dpiScaled(16 + 6 + 6), dpiScaled(TRI_Y));

	QSize sz = w->geometry().size();
	sz.rwidth() -= 0;
	sz.rheight() -= dpiScaled(TRI_Y);
	QRect rrRc(QPoint(0, dpiScaled(TRI_Y)), sz);
	pathT.addRoundedRect(rrRc, dpiScaled(4), dpiScaled(4));

	painter.fillPath(pathT, btnCl);
	painter.fillPath(pathR, btnCl);

	painter.setPen(QColor(54,66,90,50));
	painter.drawLine(rrRc.topLeft().x(), rrRc.topLeft().y() + 2, rrRc.bottomLeft().x(), rrRc.bottomLeft().y() - 2);
}

void KScreenGrabTip::setText(const QString& text)
{
	m_text = text;
}

// -----------------------------------------------------------------------

KScreenGrabToolBarHeader::KScreenGrabToolBarHeader(KScreenGrabDialog* parent)
	: KScreenGrabToolBar(parent)
	, m_moveWidget(nullptr)
	, m_moving(false)
	, m_dialog(parent)
	, m_tooltip(nullptr)
{
	QRect rc(0, 0, dpiScaled(450), dpiScaled(88));
    QRect screenGeo = QApplication::primaryScreen()->geometry();
	rc.translate(screenGeo.x() + (screenGeo.width() - rc.width()) / 2, screenGeo.y() + 20);
	setGeometry(rc);

	m_moveWidget = new QWidget(this);
	QSize fixedSize = QSize(dpiScaled(22), dpiScaled(88));
	rc = QRect(size().width() - fixedSize.width(), 0, fixedSize.width(), fixedSize.height());
	rc.adjust(dpiScaled(1), dpiScaled(1), dpiScaled(-1), dpiScaled(-1));
	m_moveWidget->setGeometry(rc);
	m_moveWidget->installEventFilter(this);
	m_moveWidget->setCursor(Qt::SizeAllCursor);
}

bool KScreenGrabToolBarHeader::eventFilter(QObject * obj, QEvent * e)
{
	if (obj == m_moveWidget)
	{
		if (e->type() == QEvent::MouseMove)
		{
			QMouseEvent* me = static_cast<QMouseEvent*>(e);
			QPoint pt = geometry().topLeft() + me->pos() - m_down;
			move(pt);
			m_dialog->update();
			return true;
		}
		else if (e->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent* me = static_cast<QMouseEvent*>(e);
			m_moving = true;
			m_down = me->pos();
			return true;
		}
		else if (e->type() == QEvent::MouseButtonRelease)
		{
			m_down = QPoint();
			m_moving = false;
			return true;
		}
		else if (e->type() == QEvent::Paint)
		{
			QPaintEvent* pe = static_cast<QPaintEvent*>(e);
			QPainter painter(m_moveWidget);
			painter.drawPixmap(QRect(QPoint(0, 0), m_moveWidget->size()), QPixmap(":icons_svg/other/move.svg"));
		}
	}
	else
	{
		QWidget* control = qobject_cast<QWidget*>(obj);
		if (control)
		{
			if (e->type() == QEvent::Enter)
			{
				if (!m_tooltip)
				{
					m_tooltip = new KScreenGrabTip(m_dialog);
				}
				show();
				QRect rc = control->geometry();
				QPoint bl = mapTo(m_dialog, rc.bottomLeft());
				bl.rx() += rc.width() / 2.f - dpiScaled(16);
				bl.ry() += dpiScaled(8);
				m_tooltip->setGeometry(QRect(bl, QSize(dpiScaled(226), dpiScaled(116))));
				m_tooltip->setText(control->toolTip());
				m_tooltip->reset(control);
				m_tooltip->show();
				m_dialog->update();
				return true;
			}
			else if (e->type() == QEvent::Leave)
			{
				if (m_tooltip)
					m_tooltip->hide();
				m_dialog->update();
				return true;
			}
			else if (e->type() == QEvent::ToolTip)
			{
				return true;
			}
		}
	}
	return KScreenGrabToolBar::eventFilter(obj, e);
}

void KScreenGrabToolBarHeader::hideTooltip()
{
	if (m_tooltip)
		m_tooltip->hide();
}
//------------------------------------------------------------------------
KScreenGrabDoneHint::KScreenGrabDoneHint(QWidget *parent, int timeout /*= -1*/)
	: QWidget(parent)
	, m_timeout(timeout)
{

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	this->setFixedWidth(dpiScaled(102));
	this->setFixedHeight(dpiScaled(90));
	m_timer = new QTimer(this);
	m_icon = QIcon("://res/16x16/success.svg");
}

KScreenGrabDoneHint::~KScreenGrabDoneHint()
{
	if (NULL != m_timer)
	{
		delete m_timer;
		m_timer = NULL;
	}
}

void KScreenGrabDoneHint::display()
{
	m_startTime = QDateTime::currentDateTime();
	if (NULL != m_timer)
	{
		connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
		m_timer->start(50);
	}
	else
	{
	}
}

void KScreenGrabDoneHint::setText(const QString& text)
{
	m_strHintText = text;
}

void KScreenGrabDoneHint::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	// QColor bgCl = KDrawHelper::getColorFromTheme("KScreenGrabDoneHint", KDrawHelper::Prop_Background);
	// QColor textCl = KDrawHelper::getColorFromTheme("KScreenGrabDoneHint", KDrawHelper::Prop_Text, Qt::white);
	QColor bgCl = QColor("#0D0D0D");
	QColor textCl = QColor("#FFFFFF");

	painter.setPen(Qt::NoPen);
	painter.setBrush(bgCl);
    painter.drawRoundedRect(rect(), 2, 2);

	QRect rcIcon = QRect(rect().left() + 35, rect().top() + 14, 32, 32);
	QRect rcText = QRect(rect().left() + 9, rect().top() + 54, 84, 22);
	if (!m_icon.isNull())
	{
		m_icon.paint(&painter, rcIcon);
	}

	QFont ft("PingFang SC");
	ft.setPixelSize(12);
	QTextOption option;
	painter.setRenderHint(QPainter::TextAntialiasing, true);
	painter.setPen(textCl);
	painter.setFont(ft);
	painter.setBrush(Qt::NoBrush);
	painter.drawText(rcText, m_strHintText);
}

void KScreenGrabDoneHint::onTimer()
{
	int ms = m_startTime.msecsTo(QDateTime::currentDateTime());
	int time = m_timeout > 0 ? m_timeout : 1500;

	if (ms <= 0)
	{
		; // do nothing
	}
	else if (ms <= time)
	{

	}
	else if (ms <= (time + 500) )
	{

	}
	else
	{
		this->hide();
		this->move(-1000, -1000);
		if (NULL != m_timer)
		{
			m_timer->stop();
		}
	}
	update();
}

// -----------------------------------------------------------------------
int KScreenGrabDialog::s_nPenWidthRect = penWidthSmall;
int KScreenGrabDialog::s_nPenWidthEllipse = penWidthSmall;
int KScreenGrabDialog::s_nArrowSize = penWidthSmall;
int KScreenGrabDialog::s_nPenWidthBrush = penWidthSmall;

KScreenGrabDialog::KScreenGrabDialog(QWidget* parent, const WId wpsWId,
	bool isHide, KscrnGrabHelper::ActionFlags flags, DrawShape drawShape)
	: QDialog(parent),
	m_Menu(NULL),
	m_toolbar(NULL),
	m_tooleditbar(NULL),
	m_toolbarSettings(NULL),
	m_colorPicker(NULL),
	m_penSmall(NULL),
	m_penMiddle(NULL),
	m_penLarge(NULL),
	m_fontSetting(NULL),
	m_hint(NULL),
	m_settingHint(NULL),
	m_rcSelection(QRect()),
	m_ptPressed(ptNullPressedPoint),
	m_pointsBrush(NULL),
	m_nNumPoints(0),
	m_nPtCurrent(0),
	m_nNumPtIncre(NUM_POINTS_INIT),
	m_textEdit(NULL),
	m_fInited(false),
	m_nEditWidth(30),
	m_nEditHeight(20),
	m_bEnableEscCancelEvent(false),
	m_fToolbarToggleOn(true),
	m_GlobalShortCutEsc(NULL),
	m_flags(flags),
	m_bSingleItemOnly(false),
	m_exitType(exitOther),
	m_toolBarActionCount(0),
	m_drawShape(drawShape),
	m_polygonState(dpsEnd),
	m_toolbarHeader(nullptr),
	m_actionDrawRect(nullptr),
	m_actionDrawRoundedRect(nullptr),
	m_actionDrawEllipse(nullptr),
	m_actionDrawPolygon(nullptr),
	m_isOriginPoint(false),
	m_badSelection(true),
	m_disableZoomBox(false),
	m_windowInfo(nullptr),
	m_bMousePressed(false),
	m_bSniffSelectDone(false),
	m_itemDrawType(itemInsert),
	m_nShapeChangeItemID(-1),
	m_pointsBrushBackup(nullptr),
	m_nPointsBackup(0),
	m_bTextHasFocus(false),
	m_bTextEditHasSelected(false),
	m_bTextDoubleClicked(false),
	m_bBrushHasDraw(false),
	m_itemOperation(optNone),
	m_nPenWidthBackup(0),
	m_nFontSizeBackup(0),
	m_bSniffWindowGrab(false),
	m_widgetTextEditMask(nullptr)
{
	// static bool bResInited(false);
	// if (!bResInited)
	// {
	// 	Q_INIT_RESOURCE(screengrab);
	// 	bResInited = true;
	// }

	m_widgetTextEditMask = new QWidget(this);
	m_widgetTextEditMask->hide();

	setProperty("qtspyName", QString("KScreenGrabDialog"));

	m_bIsHideWPS = isHide;
	if (NULL != wpsWId)
	{
		m_widWPS = wpsWId;
#ifdef Q_OS_WIN
		RECT rect;
		GetWindowRect(m_widWPS, &rect);
		m_rcWPS = QRect(rect.left, rect.top, rect.right - rect.left + 1, rect.bottom - rect.top + 1);
#else
		QWidget* pWindow= QWidget::find(WId(wpsWId));
		m_rcWPS = pWindow->rect();
#endif
	}
	else
	{
		m_widWPS = NULL;
		m_rcWPS = QRect();
	}

	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	setAttribute(Qt::WA_ShowModal, false);
	setMouseTracking(true);

    QRect rect = QApplication::primaryScreen()->geometry();
	m_rcDesktop = rect;

	QScreen *screen = QGuiApplication::primaryScreen();
    m_pixmapDesktop = screen->grabWindow(0);
	setGeometry(m_rcDesktop);
	this->move(m_rcDesktop.topLeft());
	m_screenOffset = QPoint(0, m_rcDesktop.y());
	m_rcDesktop.adjust(-m_screenOffset.x(), -m_screenOffset.y(),
		-m_screenOffset.x(), -m_screenOffset.y());

	QPixmap pixmap;
	pixmap.load(resSvgPath("cursorSelect.svg"));
	m_cursorBitmapSelect = QCursor(pixmap, -1, -1);
	pixmap.load(resSvgPath("cursorRestore.svg"));
	m_cursorBitmapRestore = QCursor(pixmap, 0, 0);
	initToolbarHeader();
	reset();

	sniffingWindow(QCursor::pos());
	connect(this, SIGNAL(finished(int)), this, SLOT(onFinished(int)));
	connect(this, SIGNAL(sigChangeItemAction(int)), this, SLOT(changeActionSelect(int)));

	int count = 0;
	int num = m_flags & ~KscrnGrabHelper::ACF_Cancel & ~KscrnGrabHelper::ACF_Setting;
	while (num)
	{
		count++;
		num = num & (num - 1);
	}
	m_bSingleItemOnly = (count == 1);
	m_toolBarActionCount = count + (m_flags & KscrnGrabHelper::ACF_Cancel ? 1 : 0) + (m_flags & KscrnGrabHelper::ACF_Setting ? 1 : 0);
}

KScreenGrabDialog::~KScreenGrabDialog()
{

}

void KScreenGrabDialog::initCursors( void )
{
	QPixmap pixmap;

	pixmap.load(resSvgPath("cursorEdit.svg"));
	m_cursorBitmapEdit = QCursor(pixmap, -1, -1);

	pixmap.load(resSvgPath("cursorSizeAll.svg"));
	m_cursorBitmapSizeAll = QCursor(pixmap, -1, -1);

	pixmap.load(resSvgPath("cursorSizeHor.svg"));
	m_cursorBitmapSizeHor = QCursor(pixmap, -1, -1);

	pixmap.load(resSvgPath("cursorSizeVer.svg"));
	m_cursorBitmapSizeVer = QCursor(pixmap, -1, -1);

	pixmap.load(resSvgPath("cursorSizeBDiag.svg"));
	m_cursorBitmapSizeBDiag = QCursor(pixmap, -1, -1);

	pixmap.load(resSvgPath("cursorSizeFDiag.svg"));
	m_cursorBitmapSizeFDiag = QCursor(pixmap, -1, -1);
}

void KScreenGrabDialog::initControls( void )
{
	initCursors();
	initActions();
	initToolbar();
	initToolbarSetting();
	initContextMenu();
		m_nTextFontSize = dpiScaled(12);

	m_colorRect = Qt::red;
	m_colorEllipse = Qt::red;
	m_colorArrow = Qt::red;
	m_colorBrush = Qt::red;
	m_colorText = Qt::red;

	m_strTextFontFamily = STR_DEFAULT_FONT_FAMILY;
	if (NULL == m_textEdit)
	{
		m_textEdit = new QTextEdit(this);
		m_textEdit->setProperty("qtspyName", QString("GrabTextEdit"));
	}
	if (NULL != m_textEdit)
	{
		m_textEdit->setAcceptRichText(false);
		m_textEdit->setContextMenuPolicy(Qt::NoContextMenu);
		connect(m_textEdit, SIGNAL(textChanged()), this, SLOT(textChanged()));
	}

	if (NULL == m_hint)
	{
		m_hint = new KScreenGrabHint(this);
	}
	onChangeSkinMode();
}

void KScreenGrabDialog::onChangeSkinMode()
{
	if(m_toolbar)
	{
		QColor bgCl = QColor("#FFFFFF");
		QColor borderCl = QColor("#EEEEEE");
		QColor textCl = QColor("#0D0D0D");
		QColor btnCl = QColor("#FFFFFF");
		QColor btnHover = QColor("#EEEEEE");
		QColor btnDown = QColor("#EEEEEE");
		QString strBg = QString("rgba(%1, %2, %3, %4)").arg(bgCl.red()).arg(bgCl.green()).arg(bgCl.blue()).arg(bgCl.alpha());
		QString strBtn = QString("rgba(%1, %2, %3, %4)").arg(btnCl.red()).arg(btnCl.green()).arg(btnCl.blue()).arg(btnCl.alpha());
		QString strHover = QString("rgba(%1, %2, %3, %4)").arg(btnHover.red()).arg(btnHover.green()).arg(btnHover.blue()).arg(btnHover.alpha());
		QString strDown = QString("rgba(%1, %2, %3, %4)").arg(btnDown.red()).arg(btnDown.green()).arg(btnDown.blue()).arg(btnDown.alpha());
		QString strText = QString("rgba(%1, %2, %3, %4)").arg(textCl.red()).arg(textCl.green()).arg(textCl.blue()).arg(textCl.alpha());
		QString strBorder = QString("rgba(%1, %2, %3, %4)").arg(borderCl.red()).arg(borderCl.green()).arg(borderCl.blue()).arg(borderCl.alpha());
		QString style =	QString("QToolBar{background-color: %8; border:1px solid %13;border-radius:%1px;padding:%6px;}\
								QToolButton{margin-left:%2px;margin-right:%2px;margin-top:%3px;margin-bottom:%3px;\
								border-style:flat;border-radius:%4px;height:%7px;width:%7px;\
								color:%9;font-family:'PingFang SC';font-size:%5px;}\
								QToolButton:on{background-color: %10}\
								QToolButton:hover{background-color: %11}\
								QToolButton:pressed{background-color: %12}")
						.arg(dpiScaled(4))
						.arg(dpiScaled(2))
						.arg(dpiScaled(0))
						.arg(dpiScaled(2))
						.arg(dpiScaled(12))
						.arg(dpiScaled(8))
						.arg(dpiScaled(23))
						.arg(strBg)
						.arg(strText)
						.arg(strDown)
						.arg(strHover)
						.arg(strDown)
						.arg(strBorder);
		m_toolbar->setStyleSheet(style);

#ifdef Q_OS_MACOS
		if (m_tooleditbar)
			m_tooleditbar->setStyleSheet(style);
#endif
	}
	if (m_toolbarSettings)
	{
		QColor bgCl = QColor("#FFFFFF");
		QColor bgBorder = QColor("#EEEEEE");
		QString strBg = QString("rgba(%1, %2, %3, %4)").arg(bgCl.red()).arg(bgCl.green()).arg(bgCl.blue()).arg(bgCl.alpha());
		QString strBorder = QString("rgba(%1, %2, %3, %4)").arg(bgBorder.red()).arg(bgBorder.green()).arg(bgBorder.blue()).arg(bgBorder.alpha());
		m_toolbarSettings->setStyleSheet(
			QString("QToolBar{background-color:%4;border:1px solid %5;border-radius:%1px;padding:%2px;}"
				"QToolButton{border-style:flat;margin-top:%3px;margin-bottom:%3px;")
			.arg(dpiScaled(4))
			.arg(dpiScaled(0))
			.arg(dpiScaled(0))
			.arg(strBg)
			.arg(strBorder));
	}
}

void KScreenGrabDialog::initToolbarHeader(void)
{
	if (m_toolbarHeader)
		return;

	QColor btnCl = QColor("#FFFFFF");
	QColor btnHover = QColor("#EEEEEE");
	QColor btnDown = QColor("#EEEEEE");
	QColor bgCl = QColor("#FFFFFF");
	QColor clText = QColor("#0D0D0D");

	QString strBtn = QString("rgba(%1, %2, %3, %4)").arg(btnCl.red()).arg(btnCl.green()).arg(btnCl.blue()).arg(btnCl.alpha());
	QString strHover = QString("rgba(%1, %2, %3, %4)").arg(btnHover.red()).arg(btnHover.green()).arg(btnHover.blue()).arg(btnHover.alpha());
	QString strDown = QString("rgba(%1, %2, %3, %4)").arg(btnDown.red()).arg(btnDown.green()).arg(btnDown.blue()).arg(btnDown.alpha());
	QString strbg = QString("rgba(%1, %2, %3, %4)").arg(bgCl.red()).arg(bgCl.green()).arg(bgCl.blue()).arg(bgCl.alpha());
	QString strText = QString("rgba(%1, %2, %3, %4)").arg(clText.red()).arg(clText.green()).arg(clText.blue()).arg(clText.alpha());
	m_toolbarHeader = new KScreenGrabToolBarHeader(this);
	m_toolbarHeader->setMouseTracking(true);
	m_toolbarHeader->installEventFilter(this);
	m_toolbarHeader->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	QString strStyle = QString("QToolBar{background-color: %1; border:1px solid rgba(0,0,0,38);border-radius:4px;padding:8px;}\
							   QToolButton{margin-left:2px;margin-right:2px;margin-top:0px;margin-bottom:0px;\
							   border-style:flat;border-radius:4px;height:23px;width:8px;\
							   color:%2;font-family:'PingFang SC';font-size:12px; padding-top:8px;}\
							   QToolButton:on{background-color: %3}\
							   QToolButton:hover{background-color: %4}\
							   QToolButton:pressed{background-color: %5}")
					   .arg(strbg)
					   .arg(strText)
					   .arg(strDown)
					   .arg(strHover)
					   .arg(strDown);
	m_toolbarHeader->setStyleSheet(strStyle);

	{
		m_actionDrawRect = new QAction(this);
		m_actionDrawRect->setText(tr("Grab Rect"));
		m_actionDrawRect->setIcon(QIcon(("://icons_svg/other/TopRect.svg")));
		bool b = connect(m_actionDrawRect, SIGNAL(triggered()), this, SLOT(onActionDrawRect()));
		Q_ASSERT(b);
		m_toolbarHeader->addAction(m_actionDrawRect);
		QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRect));
		if (button)
		{
			button->setFixedSize(QSize(dpiScaled(100), dpiScaled(72)));
			button->setCheckable(true);
			button->setCursor(Qt::ArrowCursor);
			button->setProperty("qtspyName", QString("DrawRect"));
			button->installEventFilter(m_toolbarHeader);
			button->setToolTip(QString::fromLocal8Bit(
				STRINGIFY(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"><html><head><meta name="qrichtext" content="1" /><style type="text/css">p { white-space: pre-wrap; } </style></head><body style=" font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;"><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#4f85ff;">鼠标左键拖动</span><span style="color:#999999;">选取屏幕区域; </span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#4f85ff;">Esc键</span><span style="color:#999999;">可退出截屏</span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'PingFang SC'; font-size:12px; color:#999999;">注: 自动选取全屏或窗口截屏</span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" color:#999999;"></span></p></body></html>
				)
			));
			
			m_tooltipSizeHints[button] = QSize(dpiScaled(206), dpiScaled(116));
		}
	}
	{
		m_actionDrawEllipse = new QAction(this);
		m_actionDrawEllipse->setText(tr("Grab Ellipse"));
		m_actionDrawEllipse->setIcon(QIcon(("://icons_svg/other/TopEllipse.svg")));
		bool b = connect(m_actionDrawEllipse, SIGNAL(triggered()), this, SLOT(onActionDrawEllipse()));
		Q_ASSERT(b);
		m_toolbarHeader->addAction(m_actionDrawEllipse);
		QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawEllipse));
		if (button)
		{
			button->setFixedSize(QSize(dpiScaled(100), dpiScaled(72)));
			button->setCheckable(true);
			button->setCursor(Qt::ArrowCursor);
			button->setProperty("qtspyName", QString("DrawEllipse"));
			button->installEventFilter(m_toolbarHeader);
				button->setToolTip(QString::fromLocal8Bit(
					STRINGIFY(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"><html><head><meta name = "qrichtext" content="1"/><style type = "text/css">p { white-space: pre-wrap; } </style></head><body style = " font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;"><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">鼠标左键拖动</span><span style = "color:#999999;">选取屏幕区域; </span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'PingFang SC'; font-size:12px; color:#999999;"> </span></p><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">Esc键</span><span style = "color:#999999;">可退出截屏</span></p>
					)
				));
				m_tooltipSizeHints[button] = QSize(dpiScaled(206), dpiScaled(116));
		}
	}
	{
		m_actionDrawRoundedRect = new QAction(this);
		m_actionDrawRoundedRect->setText(tr("Grab RoundedRect"));
		m_actionDrawRoundedRect->setIcon(QIcon(("://icons_svg/other/TopRoundedRect.svg")));
		bool b = connect(m_actionDrawRoundedRect, SIGNAL(triggered()), this, SLOT(onActionDrawRoundedRect()));
		Q_ASSERT(b);
		m_toolbarHeader->addAction(m_actionDrawRoundedRect);
		QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRoundedRect));
		if (button)
		{
				button->setFixedSize(QSize(dpiScaled(100), dpiScaled(72)));
			button->setCheckable(true);
			button->setCursor(Qt::ArrowCursor);
			button->setProperty("qtspyName", QString("DrawRoundedRect"));
			button->installEventFilter(m_toolbarHeader);
				button->setToolTip(QString::fromLocal8Bit(
					STRINGIFY(<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0//EN" "http://www.w3.org/TR/REC-html40/strict.dtd"><html><head><meta name = "qrichtext" content="1"/><style type = "text/css">p{ white-space: pre-wrap; }</style></head><body style = " font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;"><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">鼠标左键拖动</span><span style = "color:#999999;">选取屏幕区域; </span></p><p style=" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style=" font-family:'PingFang SC'; font-size:12px; color:#999999;"> </span></p><p style = " margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;"><span style = " color:#4f85ff;">Esc键</span><span style = "color:#999999;">可退出截屏</span></p>
					)
				));
				m_tooltipSizeHints[button] = QSize(dpiScaled(206), dpiScaled(116));
		}
	}

	{
		m_actionDrawPolygon = new QAction(this);
		m_actionDrawPolygon->setText(tr("Grab Polygon"));
		m_actionDrawPolygon->setIcon(QIcon(("://icons_svg/other/TopPolygon.svg")));
		bool b = connect(m_actionDrawPolygon, SIGNAL(triggered()), this, SLOT(onActionDrawPolygon()));
		Q_ASSERT(b);
		m_toolbarHeader->addAction(m_actionDrawPolygon);
		QAbstractButton* button = qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawPolygon));
		if (button)
		{
				button->setFixedSize(QSize(dpiScaled(100), dpiScaled(72)));
			button->setCheckable(true);
			button->setCursor(Qt::ArrowCursor);
			button->setProperty("qtspyName", QString("DrawPolygon"));
			button->installEventFilter(m_toolbarHeader);
				button->setToolTip(QString::fromLocal8Bit(
					STRINGIFY(<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\"><html><head><meta name = \"qrichtext\" content=\"1\"/><style type = \"text/css\">p{ white-space: pre-wrap; }</style></head><body style = \" font-family:'PingFang SC'; font-size:12px; font-weight:400; font-style:normal;\"><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Left mouse click </span><span style = \"color:#999999;\">to select drawing points, Get polygon area.</span></p><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Left mouse button drag </span><span style = \"color:#999999;\">will record the movement path, Get arbitrary shape area.</span></p><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Click the origin </span><span style = \"color:#999999;\">or </span><span style = \" color:#4f85ff;\">double-click with the mouse anywhere </span><span style = \"color:#999999;\">get the current screenshot area.</span></p><p style = \" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style = \" color:#4f85ff;\">Click ESC </span><span style = \"color:#999999;\">to exit editing.</span></p>
					)
				));
				m_tooltipSizeHints[button] = QSize(dpiScaled(320), dpiScaled(136));
		}
	}
	toggleDrawAction();
	showToolBarHeader();
}
void KScreenGrabDialog::initActions( void )
{
	if (NULL == m_actionRect
		&& m_flags & KscrnGrabHelper::ACF_Rect)
	{
		m_actionRect = new QAction(tr("Rect Tool"), this);
		m_actionRect->setCheckable(true);
		m_actionRect->setIcon(QIcon(resSvgPath("rect.svg")));
		connect(m_actionRect, SIGNAL(triggered()), this, SLOT(onActionRect()));
	}

	if (NULL == m_toolbtnRect
		&& m_flags & KscrnGrabHelper::ACF_Rect)
	{
		m_toolbtnRect = new KScrnGrabToolButton(this, "", true);
		m_toolbtnRect->setIcon(resSvgPath("rect.svg"));
		connect(m_toolbtnRect, SIGNAL(clicked()), this, SLOT(onActionRect()));
		m_toolbtnRect->setToolTip(tr("Rect Tool"));
	}

	if (NULL == m_actionEllipse
		&& m_flags & KscrnGrabHelper::ACF_Ellipse)
	{
		m_actionEllipse = new QAction(tr("Oval Tool"), this);
		m_actionEllipse->setCheckable(true);
		m_actionEllipse->setIcon(QIcon(resSvgPath("circle.svg")));
		connect(m_actionEllipse, SIGNAL(triggered()), this, SLOT(onActionEllipse()));
	}

	if (NULL == m_toolbtnEllipse
		&& m_flags & KscrnGrabHelper::ACF_Ellipse)
	{
		m_toolbtnEllipse = new KScrnGrabToolButton(this, "", true);
		m_toolbtnEllipse->setIcon(resSvgPath("circle.svg"));
		connect(m_toolbtnEllipse, SIGNAL(clicked()), this, SLOT(onActionEllipse()));
		m_toolbtnEllipse->setToolTip(tr("Oval Tool"));
	}

	if (NULL == m_actionArrow
		&& m_flags & KscrnGrabHelper::ACF_Arrow)
	{
		m_actionArrow = new QAction(tr("Arrow Tool"), this);
		m_actionArrow->setCheckable(true);
		m_actionArrow->setIcon(QIcon(resSvgPath("arrow.svg")));
		connect(m_actionArrow, SIGNAL(triggered()), this, SLOT(onActionArrow()));
	}

	if (NULL == m_toolbtnArrow
		&& m_flags & KscrnGrabHelper::ACF_Arrow)
	{
		m_toolbtnArrow = new KScrnGrabToolButton(this, "", true);
		m_toolbtnArrow->setIcon(resSvgPath("arrow.svg"));
		connect(m_toolbtnArrow, SIGNAL(clicked()), this, SLOT(onActionArrow()));
		m_toolbtnArrow->setToolTip(tr("Arrow Tool"));
	}

	if (NULL == m_actionBrush
		&& m_flags & KscrnGrabHelper::ACF_Brush)
	{
		m_actionBrush = new QAction(tr("Pen Tool"), this);
		m_actionBrush->setCheckable(true);
		m_actionBrush->setIcon(QIcon(resSvgPath("pen.svg")));
		connect(m_actionBrush, SIGNAL(triggered()), this, SLOT(onActionBrush()));
	}

	if (NULL == m_toolbtnBrush
		&& m_flags & KscrnGrabHelper::ACF_Brush)
	{
		m_toolbtnBrush = new KScrnGrabToolButton(this, "", true);
		m_toolbtnBrush->setIcon(resSvgPath("pen.svg"));
		connect(m_toolbtnBrush, SIGNAL(clicked()), this, SLOT(onActionBrush()));
		m_toolbtnBrush->setToolTip(tr("Pen Tool"));
	}

	if (NULL == m_actionMosaic
		&& m_flags & KscrnGrabHelper::ACF_Mosaic)
	{
		m_actionMosaic = new QAction(tr("MoSaic"), this);
		m_actionMosaic->setCheckable(true);
		m_actionMosaic->setIcon(QIcon(resSvgPath("masaike.svg")));
		connect(m_actionMosaic, SIGNAL(triggered()), this, SLOT(onActionMosaic()));
	}

	if (NULL == m_toolbtnMosaic
		&& m_flags & KscrnGrabHelper::ACF_Mosaic)
	{
		m_toolbtnMosaic = new KScrnGrabToolButton(this, "", true);
		m_toolbtnMosaic->setIcon(resSvgPath("masaike.svg"));
		connect(m_toolbtnMosaic, SIGNAL(clicked()), this, SLOT(onActionMosaic()));
		m_toolbtnMosaic->setToolTip(tr("Mosaic"));
	}

	if (NULL == m_actionText
		&& m_flags & KscrnGrabHelper::ACF_Text)
	{
		m_actionText = new QAction(tr("Text Tool"), this);
		m_actionText->setCheckable(true);
		m_actionText->setIcon(QIcon(resSvgPath("text.svg")));
		connect(m_actionText, SIGNAL(triggered()), this, SLOT(onActionText()));
	}

	if (NULL == m_toolbtnText
		&& m_flags & KscrnGrabHelper::ACF_Text)
	{
		m_toolbtnText = new KScrnGrabToolButton(this, "", true);
		m_toolbtnText->setIcon(resSvgPath("text.svg"));
		connect(m_toolbtnText, SIGNAL(clicked()), this, SLOT(onActionText()));
		m_toolbtnText->setToolTip(tr("Text Tool"));
	}

	if (NULL == m_actionUndo
		&& m_flags & KscrnGrabHelper::ACF_Undo)
	{
		m_actionUndo = new QAction(tr("Undo Editor"), this);
		m_actionUndo->setIcon(QIcon(resSvgPath("undo.svg")));
		m_actionUndo->setShortcut(QKeySequence::Undo);
		connect(m_actionUndo, SIGNAL(triggered()), this, SLOT(onActionUndo()));
	}

	if (NULL == m_toolbtnUndo
		&& m_flags & KscrnGrabHelper::ACF_Undo)
	{
		m_toolbtnUndo = new KScrnGrabToolButton(this, "");
		m_toolbtnUndo->setIcon(resSvgPath("undo.svg"));
		connect(m_toolbtnUndo, SIGNAL(clicked()), this, SLOT(onActionUndo()));
		m_toolbtnUndo->setToolTip(tr("Undo Editor"));
	}

	if (NULL == m_actionCopy
		&& m_flags & KscrnGrabHelper::ACF_Copy)
	{
		m_actionCopy = new QAction(tr("Copy"), this);
		m_actionCopy->setIcon(QIcon(resSvgPath("copy.svg")));
		m_actionCopy->setShortcut(QKeySequence::Copy);
		connect(m_actionCopy, SIGNAL(triggered()), this, SLOT(onActionCopy()));
	}

	if (NULL == m_toolbtnCopy
		&& m_flags & KscrnGrabHelper::ACF_Copy)
	{
		m_toolbtnCopy = new KScrnGrabToolButton(this, "");
		m_toolbtnCopy->setIcon(resSvgPath("copy.svg"));
		connect(m_toolbtnCopy, SIGNAL(clicked()), this, SLOT(onActionCopy()));
		m_toolbtnCopy->setToolTip(tr("Copy"));
	}

	if (NULL == m_actionSave
		&& m_flags & KscrnGrabHelper::ACF_Save)
	{
		m_actionSave = new QAction(tr("Save"), this);
		m_actionSave->setIcon(QIcon(resSvgPath("pinsave.svg")));
		m_actionSave->setShortcut(QKeySequence::Save);
		connect(m_actionSave, SIGNAL(triggered()), this, SLOT(onActionSave()));
	}

	if (NULL == m_toolbtnSave
		&& m_flags & KscrnGrabHelper::ACF_Save)
	{
		m_toolbtnSave = new KScrnGrabToolButton(this, "");
		m_toolbtnSave->setIcon(resSvgPath("pinsave.svg"));
		connect(m_toolbtnSave, SIGNAL(clicked()), this, SLOT(onActionSave()));
		m_toolbtnSave->setToolTip(tr("Save"));
	}

	if (NULL == m_actionPin
		&& m_flags & KscrnGrabHelper::ACF_Pin)
	{
		m_actionPin = new QAction(this);
		m_actionPin->setToolTip(tr("Screenshot nailed to screen"));
		m_actionPin->setIcon(QIcon(resSvgPath("pin.svg")));
		connect(m_actionPin, SIGNAL(triggered()), this, SLOT(onActionPin()));
	}

	if (NULL == m_toolbtnPin
		&& m_flags & KscrnGrabHelper::ACF_Pin)
	{
		m_toolbtnPin = new KScrnGrabToolButton(this, "");
		m_toolbtnPin->setLimitFree(true);
		m_toolbtnPin->setIcon(resSvgPath("pin.svg"));
		connect(m_toolbtnPin, SIGNAL(clicked()), this, SLOT(onActionPin()));
		m_toolbtnPin->setToolTip(tr("Screenshot nailed to screen"));
	}

	if (NULL == m_actionCancel
		&& m_flags & KscrnGrabHelper::ACF_Cancel)
	{
		m_actionCancel = new QAction(tr("Exit"), this);
		m_actionCancel->setIcon(QIcon(resSvgPath("cancel.svg")));
		connect(m_actionCancel, SIGNAL(triggered()), this, SLOT(onActionCancel()));
	}

	if (NULL == m_toolbtnCancel
		&& m_flags & KscrnGrabHelper::ACF_Cancel)
	{
		m_toolbtnCancel = new KScrnGrabToolButton(this, "", true);
		m_toolbtnCancel->setIcon(resSvgPath("cancel.svg"));
		connect(m_toolbtnCancel, SIGNAL(clicked()), this, SLOT(onActionCancel()));
		m_toolbtnCancel->setToolTip(tr("Exit"));
	}

	if (NULL == m_actionDone
		&& m_flags & KscrnGrabHelper::ACF_Done)
	{
		m_actionDone = new QAction(tr("Finish"), this);
		m_actionDone->setIcon(QIcon(resSvgPath("complate.svg")));
		connect(m_actionDone, SIGNAL(triggered()), this, SLOT(onActionDone()));
	}

	if (NULL == m_actionOcr
		&& m_flags & KscrnGrabHelper::ACF_Ocr)
	{
		m_actionOcr = new QAction(tr("Picture to Text"), this);
		m_actionOcr->setIcon(QIcon(resSvgPath("scrngrabocr.svg")));
		connect(m_actionOcr, SIGNAL(triggered()), this, SLOT(onActionOcr()));
	}

	if (NULL == m_actionSave2Pdf
		&& m_flags & KscrnGrabHelper::ACF_Save2Pdf)
	{
		m_actionSave2Pdf = new QAction(tr("Save As PDF"), this);
		m_actionSave2Pdf->setIcon(QIcon(resSvgPath("save2pdf.svg")));
		connect(m_actionSave2Pdf, SIGNAL(triggered()), this, SLOT(onActionSave2Pdf()));
	}

	if (NULL == m_actionSetting
		&& m_flags & KscrnGrabHelper::ACF_Setting)
	{
		m_actionSetting = new QAction(this);
		m_actionSetting->setToolTip(tr("Setting"));
		m_actionSetting->setIcon(QIcon(resSvgPath("setting.svg")));
		connect(m_actionSetting, SIGNAL(triggered()), this, SLOT(onActionSetting()));
	}

	if (NULL == m_actionExportImage
		&& m_flags & KscrnGrabHelper::ACF_ExportImage)
	{
		m_actionExportImage = new QAction(tr("Export Image"),this);
		m_actionExportImage->setIcon(QIcon(pngPath("export2picture.png")));
		connect(m_actionExportImage, SIGNAL(triggered()), this, SLOT(onActionExportImage()));
	}

	if (NULL == m_actionImgTranslate
		&& m_flags & KscrnGrabHelper::ACF_TranslateImage)
	{
		m_actionImgTranslate = new QAction(tr("Translate Text"), this);
		m_actionImgTranslate->setIcon(QIcon(pngPath("translateimage.png")));
		connect(m_actionImgTranslate, SIGNAL(triggered()), this, SLOT(onActionTranslateImage()));
	}
}


void KScreenGrabDialog::initToolbarSetting( void )
{
	if (NULL != m_toolbarSettings)
	{
		return;
	}

	m_toolbarSettings = new KScreenGrabToolBar(this);
	m_toolbarSettings->setFixedHeight(dpiScaled(30));
	m_toolbarSettings->setFixedWidth(dpiScaled(245));


	// pen width [small, middle, large]
		m_penSmall = new KScreenGrabPen(dpiScaled(penWidthSmall), this);
		m_penMiddle = new KScreenGrabPen(dpiScaled(penWidthMiddle), this);
		m_penLarge = new KScreenGrabPen(dpiScaled(penWidthLarge), this);
	m_penSmall->setCursor(Qt::ArrowCursor);
	m_penMiddle->setCursor(Qt::ArrowCursor);
	m_penLarge->setCursor(Qt::ArrowCursor);
	m_penSmall->setProperty("qtspyName", QString("GrabPenSmall"));
	m_penMiddle->setProperty("qtspyName", QString("GrabPenMiddle"));
	m_penLarge->setProperty("qtspyName", QString("GrabPenLarge"));
	connect(m_penSmall, SIGNAL(penSizeChanged(int)), this, SLOT(penSizeChanged(int)));
	connect(m_penMiddle, SIGNAL(penSizeChanged(int)), this, SLOT(penSizeChanged(int)));
	connect(m_penLarge, SIGNAL(penSizeChanged(int)), this, SLOT(penSizeChanged(int)));

	QWidget* paddingWidget = new QWidget(this);
	paddingWidget->setFixedWidth(1);
	m_toolbarSettings->addWidget(paddingWidget);

	m_actionPenSmall = m_toolbarSettings->addWidget(m_penSmall);
	m_actionPenSmall->setVisible(true);
	m_actionPenSmall->setCheckable(true);

	m_actionPenMiddle = m_toolbarSettings->addWidget(m_penMiddle);
	m_actionPenMiddle->setVisible(true);
	m_actionPenMiddle->setCheckable(true);

	m_actionPenLarge = m_toolbarSettings->addWidget(m_penLarge);
	m_actionPenLarge->setVisible(true);
	m_actionPenLarge->setCheckable(true);

	// font setting
	m_fontSetting = new KScreenGrabFontSetting(this);
	m_fontSetting->setProperty("qtspyName", "GrabFontSetting");
	m_fontSetting->setCursor(Qt::ArrowCursor);
	connect(m_fontSetting, SIGNAL(fontSizeChanged(int)),
		this, SLOT(fontSizeChanged(int)));

	m_actionFont = m_toolbarSettings->addWidget(m_fontSetting);
	m_actionFont->setVisible(false);

	// separator
	m_actionSeparator = m_toolbarSettings->addSeparator();
	m_toolbarSettings->widgetForAction(m_actionSeparator)->setCursor(Qt::ArrowCursor);
	m_toolbarSettings->widgetForAction(m_actionSeparator)->setFixedSize(QSize(10, 24));

	// color picker
	m_colorPicker = new KScreenGrabColorPicker(this);
	m_colorPicker->setCursor(Qt::ArrowCursor);
	m_colorPicker->setProperty("qtspyName", QString("GrabColorPicker"));
	connect(m_colorPicker, SIGNAL(colorChanged(const QColor&)),
		this, SLOT(colorChanged(const QColor&)));

	m_actionColor = m_toolbarSettings->addWidget(m_colorPicker);
	m_actionColor->setVisible(true);
}

void KScreenGrabDialog::initToolbar( void )
{
	if (NULL != m_toolbar)
	{
		return;
	}

	QAction* seperator;
	m_toolbar = new KScreenGrabToolBar(this);
	m_toolbar->setIconSize(QSize(24, 24));
	m_toolbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);

	if (m_bSingleItemOnly)
	{
		//setting
		if (m_actionSetting && (m_flags & KscrnGrabHelper::ACF_Setting))
		{
			m_toolbar->addAction(m_actionSetting);
			m_toolbar->widgetForAction(m_actionSetting)->setCursor(Qt::ArrowCursor);
			m_toolbar->widgetForAction(m_actionSetting)->setProperty("qtspyName", QString("GrabSetting"));
		}

		//cancel
		if (m_toolbtnCancel && (m_flags & KscrnGrabHelper::ACF_Cancel))
		{
			m_toolbtnCancel->setFixedSize(szToolbarBtn);
			m_toolbar->addWidget(m_toolbtnCancel);
			m_toolbtnCancel->setProperty("qtspyName", QString("GrabCancel"));
			m_toolbtnCancel->setCursor(Qt::ArrowCursor);
		}

		seperator = m_toolbar->addSeparator();
		m_toolbar->widgetForAction(seperator)->setCursor(Qt::ArrowCursor);
		m_toolbar->widgetForAction(seperator)->setFixedSize((QSize(10, 22)));

		if (m_flags & KscrnGrabHelper::ACF_Ocr)
		{
			KScrnGrabToolButton *btnOcr = new KScrnGrabToolButton(this, tr("Picture to Text"));
			btnOcr->setProperty("qtspyName", QString("GrabOcr"));
			btnOcr->setIcon(resSvgPath("scrngrabocr.svg"));
			{
				btnOcr->setFixedSize(78, 26);
			}
			connect(btnOcr, SIGNAL(clicked()), this, SLOT(onActionOcr()));
			m_toolbar->addWidget(btnOcr);
		}

		return;
	}

	bool bNeedSeparator = false;
	//exportimage
	if (m_flags & KscrnGrabHelper::ACF_ExportImage)
	{
		KScrnGrabToolButton *btnOcr = new KScrnGrabToolButton(this, tr("Picture to Text"));
		btnOcr->setProperty("qtspyName", QString("GrabOcr"));
		btnOcr->setIcon(resSvgPath("scrngrabocr.svg"));
		btnOcr->setFixedSize(78, 26);
		connect(btnOcr, SIGNAL(clicked()), this, SLOT(onActionOcr()));
		m_toolbar->addWidget(btnOcr);

		bNeedSeparator = true;
	}

	//save2pdf
	if (m_flags & KscrnGrabHelper::ACF_Save2Pdf)
	{
		KScrnGrabToolButton *btn = new KScrnGrabToolButton(this, tr("Save As PDF"));
		btn->setProperty("qtspyName", QString("GrabSaveAsPdf"));
		btn->setIcon("save2pdf");
		btn->setFixedSize(76, 26);
		connect(btn, SIGNAL(clicked()), this, SLOT(onActionSave2Pdf()));
		m_toolbar->addWidget(btn);

		bNeedSeparator = true;
	}

	//translate image
	if (m_flags & KscrnGrabHelper::ACF_TranslateImage)
	{
		KScrnGrabToolButton *btn = new KScrnGrabToolButton(this, tr("Translate Text"));
		btn->setProperty("qtspyName", QString("TranslateImage"));
		btn->setIcon("translateimage");
		btn->setFixedSize(QSize(78, 26));
		connect(btn, SIGNAL(clicked()), this, SLOT(onActionTranslateImage()));
		m_toolbar->addWidget(btn);

		bNeedSeparator = true;
	}

	//ocr
	if (m_flags & KscrnGrabHelper::ACF_Ocr)
	{
		KScrnGrabToolButton *btn = new KScrnGrabToolButton(this, tr("Picture to Text"));
		btn->setProperty("qtspyName", QString("GrabOcr"));
		btn->setIcon(resSvgPath("scrngrabocr.svg"));
		{
			btn->setFixedSize(78, 26);
		}
		connect(btn, SIGNAL(clicked()), this, SLOT(onActionOcr()));
		m_toolbar->addWidget(btn);

		bNeedSeparator = true;
	}

#ifdef Q_OS_MACOS
	//edit
    if ((m_flags & KscrnGrabHelper::ACF_Edit))
	{
		KScrnGrabToolButton *btn = new KScrnGrabToolButton(this, tr("Image Edit"));
		btn->setIcon(resSvgPath("image_edit.svg"));
			btn->setFixedSize(78, 26);
		connect(btn, &KScrnGrabToolButton::clicked, this, &KScreenGrabDialog::onActionEdit);
		m_toolbar->addWidget(btn);
		
		//“更多”按钮
		KScrnGrabToolButton* btnMore = new KScrnGrabToolButton(this, "");
		btnMore->setIcon(resSvgPath("more.svg"));
		btnMore->setFixedSize(26, 26);
		btnMore->setCursor(Qt::ArrowCursor);
		connect(btnMore, &KScrnGrabToolButton::clicked, this, &KScreenGrabDialog::onMoreEditClicked);
		m_toolbar->addWidget(btnMore);

		bNeedSeparator = true;

		//“更多”悬浮栏
		m_tooleditbar = new KScreenGrabToolBar(this);
		m_tooleditbar->setIconSize(QSize(24, 24));
		m_tooleditbar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		
		KScrnGrabToolButton *btnClarity = new KScrnGrabToolButton(this, tr("Restore"));
		btnClarity->setIcon(resSvgPath("clarity.svg"));
			btnClarity->setFixedSize(78, 26);
		connect(btnClarity, &KScrnGrabToolButton::clicked, this, &KScreenGrabDialog::onActionEditClarity);
		m_tooleditbar->addWidget(btnClarity);
		
		KScrnGrabToolButton *btnRemove = new KScrnGrabToolButton(this, tr("Remove"));
		btnRemove->setIcon(resSvgPath("remove.svg"));
			btnRemove->setFixedSize(78, 26);
		connect(btnRemove, &KScrnGrabToolButton::clicked, this, &KScreenGrabDialog::onActionEditRemove);
		m_tooleditbar->addWidget(btnRemove);
		
		KScrnGrabToolButton *btnMatting = new KScrnGrabToolButton(this, tr("Cutout"));
		btnMatting->setIcon(resSvgPath("matting.svg"));
			btnMatting->setFixedSize(78, 26);
		connect(btnMatting, &KScrnGrabToolButton::clicked, this, &KScreenGrabDialog::onActionEditMatting);
		m_tooleditbar->addWidget(btnMatting);
	}
#endif

	if (bNeedSeparator)
	{
		seperator = m_toolbar->addSeparator();
		m_toolbar->widgetForAction(seperator)->setCursor(Qt::ArrowCursor);
		m_toolbar->widgetForAction(seperator)->setFixedSize(QSize(QSize(10,22)));
		bNeedSeparator = false;
	}

	//rect
	if (m_toolbtnRect && (m_flags & KscrnGrabHelper::ACF_Rect))
	{
		m_toolbtnRect->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnRect);
		m_toolbtnRect->setProperty("qtspyName", QString("GrabRect"));
		m_toolbtnRect->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//ellipse
	if (m_toolbtnEllipse && ( m_flags & KscrnGrabHelper::ACF_Ellipse))
	{
		m_toolbtnEllipse->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnEllipse);
		m_toolbtnEllipse->setProperty("qtspyName", QString("GrabEllipse"));
		m_toolbtnEllipse->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//arrow
	if (m_toolbtnArrow && (m_flags & KscrnGrabHelper::ACF_Arrow))
	{
		m_toolbtnArrow->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnArrow);
		m_toolbtnArrow->setProperty("qtspyName", QString("GrabArrow"));
		m_toolbtnArrow->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//brush
	if (m_toolbtnBrush && (m_flags & KscrnGrabHelper::ACF_Brush))
	{
		m_toolbtnBrush->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnBrush);
		m_toolbtnBrush->setProperty("qtspyName", QString("GrabBrush"));
		m_toolbtnBrush->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}
	//mosaic
	if (m_toolbtnMosaic && (m_flags & KscrnGrabHelper::ACF_Mosaic))
	{
		m_toolbtnMosaic->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnMosaic);
		m_toolbtnMosaic->setProperty("qtspyName", QString("Mosaic"));
		m_toolbtnMosaic->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}
	
	//text
	if (m_toolbtnText && (m_flags & KscrnGrabHelper::ACF_Text))
	{
		m_toolbtnText->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnText);
		m_toolbtnText->setProperty("qtspyName", QString("GrabText"));
		m_toolbtnText->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//---
	if (bNeedSeparator)
	{
		seperator = m_toolbar->addSeparator();
		m_toolbar->widgetForAction(seperator)->setCursor(Qt::ArrowCursor);
		m_toolbar->widgetForAction(seperator)->setFixedSize(QSize(QSize(10, 22)));
		bNeedSeparator = false;
	}

	//actionUndo
	if (m_toolbtnUndo && (m_flags & KscrnGrabHelper::ACF_Undo))
	{
		m_toolbtnUndo->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnUndo);
		m_toolbtnUndo->setProperty("qtspyName", QString("GrabUndo"));
		m_toolbtnUndo->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//setting
	if (m_actionSetting && (m_flags & KscrnGrabHelper::ACF_Setting))
	{
		m_toolbar->addAction(m_actionSetting);
		m_toolbar->widgetForAction(m_actionSetting)->setCursor(Qt::ArrowCursor);
		m_toolbar->widgetForAction(m_actionSetting)->setProperty("qtspyName", QString("GrabSetting"));
		bNeedSeparator = true;
	}

	//copy
	if (m_toolbtnCopy && (m_flags & KscrnGrabHelper::ACF_Copy))
	{
		m_toolbtnCopy->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnCopy);
		m_toolbtnCopy->setProperty("qtspyName", QString("GrabCopy"));
		m_toolbtnCopy->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//save
	if (m_toolbtnSave && (m_flags & KscrnGrabHelper::ACF_Save))
	{
		m_toolbtnSave->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnSave);
		m_toolbtnSave->setProperty("qtspyName", QString("GrabSave"));
		m_toolbtnSave->setCursor(Qt::ArrowCursor);
		bNeedSeparator = true;
	}

	//---
	if (bNeedSeparator)
	{
		seperator = m_toolbar->addSeparator();
		m_toolbar->widgetForAction(seperator)->setCursor(Qt::ArrowCursor);
		m_toolbar->widgetForAction(seperator)->setFixedSize(QSize(QSize(10, 22)));
		bNeedSeparator = false;
	}

	//pin
	if (m_toolbtnPin && (m_flags & KscrnGrabHelper::ACF_Pin))
	{
		m_toolbtnPin->setFixedSize(QSize(40, 26));
		m_toolbar->addWidget(m_toolbtnPin);
		m_toolbtnPin->setProperty("qtspyName", QString("GrabPin"));
		m_toolbtnPin->setCursor(Qt::ArrowCursor);
	}

	//cancel
	if (m_toolbtnCancel && (m_flags & KscrnGrabHelper::ACF_Cancel))
	{
		m_toolbtnCancel->setFixedSize(szToolbarBtn);
		m_toolbar->addWidget(m_toolbtnCancel);
		m_toolbtnCancel->setProperty("qtspyName", QString("GrabCancel"));
		m_toolbtnCancel->setCursor(Qt::ArrowCursor);
	}

	//done
	if (m_flags & KscrnGrabHelper::ACF_Done)
	{
		KScrnGrabToolButton *btnDone = new KScrnGrabToolButton(this, tr("Finish"));
		btnDone->setProperty("qtspyName", QString("GrabDone"));
		btnDone->setIcon(resSvgPath("complate.svg"));
		btnDone->setTextColor("#269956");
		btnDone->setFixedSize(QSize(56, 26));

		connect(btnDone, SIGNAL(clicked()), this, SLOT(onActionDone()));
		m_toolbar->addWidget(btnDone);
	}
}

void KScreenGrabDialog::initContextMenu( void )
{
	if (m_bSingleItemOnly || NULL != m_Menu)
	{
		return;
	}

	m_Menu = new KScreenGrabMenu(this);

	hideMenu();

	bool bNeedSeparator = false;

	if (m_actionExportImage && (m_flags & KscrnGrabHelper::ACF_ExportImage))
	{
		m_Menu->addAction(m_actionExportImage);
		bNeedSeparator = true;
	}

	//save2pdf
	if (m_actionSave2Pdf && (m_flags & KscrnGrabHelper::ACF_Save2Pdf))
	{
		m_Menu->addAction(m_actionSave2Pdf);
		bNeedSeparator = true;
	}

	//translate image
	if (m_actionImgTranslate && (m_flags & KscrnGrabHelper::ACF_TranslateImage))
	{
		m_Menu->addAction(m_actionImgTranslate);
		bNeedSeparator = true;
	}

	//ocr
	if (m_actionOcr && (m_flags & KscrnGrabHelper::ACF_Ocr))
	{
		m_Menu->addAction(m_actionOcr);
		bNeedSeparator = true;
	}

	if (bNeedSeparator)
	{
		m_Menu->addSeparator();
		bNeedSeparator = false;
	}

	if (m_actionRect && (m_flags & KscrnGrabHelper::ACF_Rect))
	{
		m_Menu->addAction(m_actionRect);
		bNeedSeparator = true;
	}

	if (m_actionEllipse && (m_flags & KscrnGrabHelper::ACF_Ellipse))
	{
		m_Menu->addAction(m_actionEllipse);
		bNeedSeparator = true;
	}

	if (m_actionArrow && (m_flags & KscrnGrabHelper::ACF_Arrow))
	{
		m_Menu->addAction(m_actionArrow);
		bNeedSeparator = true;
	}

	if (m_actionBrush && (m_flags & KscrnGrabHelper::ACF_Brush))
	{
		m_Menu->addAction(m_actionBrush);
		bNeedSeparator = true;
	}

	if (m_actionMosaic && (m_flags & KscrnGrabHelper::ACF_Mosaic))
	{
		m_Menu->addAction(m_actionMosaic);
		bNeedSeparator = true;
	}

	if (m_actionText && (m_flags & KscrnGrabHelper::ACF_Text))
	{
		m_Menu->addAction(m_actionText);
		bNeedSeparator = true;
	}

	if (bNeedSeparator)
	{
		m_Menu->addSeparator();
		bNeedSeparator = false;
	}

	QAction *actionReSelect = new QAction(tr("Reselect the screenshot area"), this);
	connect(actionReSelect, SIGNAL(triggered()), this, SLOT(onActionReSelect()));
	m_Menu->addAction(actionReSelect);

	if (m_actionUndo && (m_flags & KscrnGrabHelper::ACF_Undo))
	{
		m_Menu->addAction(m_actionUndo);
		bNeedSeparator = true;
	}

	//setting
	if (m_actionSetting && (m_flags & KscrnGrabHelper::ACF_Setting))
	{
		m_Menu->addAction(m_actionSetting);
		bNeedSeparator = true;
	}

	if (m_actionCopy && (m_flags & KscrnGrabHelper::ACF_Copy))
	{
		m_Menu->addAction(m_actionCopy);
		bNeedSeparator = true;
	}

	if (m_actionSave && (m_flags & KscrnGrabHelper::ACF_Save))
	{
		m_Menu->addAction(m_actionSave);
		bNeedSeparator = true;
	}

	if (m_actionPin && (m_flags & KscrnGrabHelper::ACF_Pin))
	{
		m_Menu->addAction(m_actionPin, true);
		bNeedSeparator = true;
	}

	if (m_actionDone && (m_flags & KscrnGrabHelper::ACF_Done))
	{
		m_Menu->addAction(m_actionDone);
		bNeedSeparator = true;
	}

	if (bNeedSeparator)
	{
		m_Menu->addSeparator();
		bNeedSeparator = false;
	}

	if (NULL == m_actionToggleToolbar)
	{
		m_actionToggleToolbar = new QAction(tr("Hide Edit Tool"), this);
		connect(m_actionToggleToolbar, SIGNAL(triggered()), this, SLOT(onActionToggleToolbar()));

		m_Menu->addAction(m_actionToggleToolbar);
	}

	if (m_actionCancel && (m_flags & KscrnGrabHelper::ACF_Cancel))
	{
		m_Menu->addAction(m_actionCancel);
		bNeedSeparator = true;
	}
}

bool KScreenGrabDialog::event(QEvent* event)
{
	return QDialog::event(event);
}

bool KScreenGrabDialog::eventFilter(QObject* obj, QEvent* event)
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

	case QEvent::KeyPress:
		keyPressEvent(static_cast<QKeyEvent*>(event));
		break;

	case QEvent::KeyRelease:
		keyReleaseEvent(static_cast<QKeyEvent*>(event));
		break;

	default:
		break;
	}

	return QDialog::eventFilter(obj, event);
}

void KScreenGrabDialog::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);

	painter.drawPixmap(0, 0, m_pixmapDesktop);

	if (!m_pixmapSniff.isNull() && smSelection == m_selectmode)
	{
		m_pixmapSniff.scaled(m_rcSelection.width(), m_rcSelection.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
		painter.drawPixmap(m_rcSelection, m_pixmapSniff);
		m_bSniffSelectDone = true;
	}

	drawEditHistory(painter);

	if (m_selectmode == smSniffingWindow)
	{
		if (m_drawShape == dsPolygon && (m_polygonState == dpsStart || m_polygonState == dpsHolding))
		{
			drawOverlay(painter, QColor(0x3, 0x9, 0xF, 80));
			drawSelection(painter, Qt::NoBrush, 1);
			drawSizeTooptip(painter);
			drawPixelZoomBox(painter);
			drawPolygonStartPoint(painter);
		}
		else
		{
			drawOverlay(painter, QColor(0x3, 0x9, 0xF, 50));
			drawSelection(painter, Qt::NoBrush, 3);
			drawSizeTooptip(painter);
			drawPixelZoomBox(painter);
		}
	}
	else if (m_selectmode == smSelection)
	{
		if (m_fEditing && m_itemDrawType == itemInsert)
		{
			drawEditing(painter);
		}

		if (m_fEditing &&
				m_bTextDoubleClicked && m_editType == editText)
		{
			drawTextAreaRect(painter);
		}

		drawOverlay(painter, QColor(0x3, 0x9, 0xF, 80));
		drawSelection(painter, Qt::NoBrush, 1);
		if (!m_fEditing && !m_bSniffSelectDone)
			drawResizePoint(painter, m_rcSelection);

		drawSizeTooptip(painter);
		drawPolygonStartPoint(painter);

		if (m_fResizing && Qt::SizeAllCursor != m_cursorShape)
		{
			drawPixelZoomBox(painter);
		}
	}
	else if (m_selectmode == smDraging)
	{
		drawOverlay(painter, QColor(0x3, 0x9, 0xF, 80));
		drawSelection(painter, Qt::NoBrush, 1);
		drawResizePoint(painter, m_rcSelection);
		drawSizeTooptip(painter);
		drawPixelZoomBox(painter);
		drawPolygonStartPoint(painter);
	}
}


void KScreenGrabDialog::drawEditItem(QPainter& painter, EditItem& item)
{
	QPen pen;
	QBrush brush;
	QFont font;

	// solve problem: the edges of drawing edit item overflow m_rcSelection
	painter.setClipping(true);
	painter.setClipRegion(QRegion(m_rcSelection));

	switch (item.editType)
	{
	case editNone:
		// do nothing
		break;
	case editRect:
		pen.setWidth(item.width);
		pen.setColor(item.color);
		pen.setJoinStyle(Qt::MiterJoin);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setRenderHint(QPainter::Antialiasing, false);
		drawRectShape(painter, *item.rcPaint, item.drawType != itemNone);
		break;
	case editEllipse:
		pen.setWidth(item.width);
		pen.setColor(item.color);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setRenderHint(QPainter::Antialiasing, true);
		drawEllipseShape(painter, *item.rcPaint, item.drawType != itemNone);
		break;
	case editArrow:
		painter.setPen(Qt::NoPen);
		painter.setBrush(item.color);
		painter.setRenderHint(QPainter::Antialiasing, true);
		drawArrowShape(painter, item.width, *item.ptStart, *item.ptEnd, item.drawType != itemNone);
		break;
	case editBrush:
		pen.setWidth(item.width);
		pen.setColor(item.color);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setRenderHint(QPainter::Antialiasing, true);
		drawBrushShape(painter, item.points, item.nPoints, item.drawType != itemNone);
		break;
	case editText:
		{
		QFont font = QFont(*item.fontFamily, item.fontSize);
		drawTextShape(painter, item.color, font,
					  *item.rcText, *item.text, item.drawType != itemNone);
		}
		break;
	case editFilter:
		// do nothing
		break;
	default:
		break;
	}
}


void KScreenGrabDialog::drawPolygonStartPoint(QPainter& painter)
{
	if (m_drawShape == dsPolygon)
	{
		if (!m_polySelection.isEmpty())
		{
			painter.save();
			QPainterPath startPointCircle;
			startPointCircle.addEllipse(QRect(dpiScaled(-8), dpiScaled(-8), dpiScaled(16), dpiScaled(16)));
			painter.setBrush(colorSelectionEdge);

			QTransform transform;
			transform.translate(m_polySelection[0].x(), m_polySelection[0].y());
			painter.setTransform(transform);
			painter.fillPath(startPointCircle, colorSelectionEdge);
			painter.restore();
		}
	}
}

void KScreenGrabDialog::drawEditHistory( QPainter& painter )
{
	if (m_listEdit.isEmpty())
	{
		return;
	}

	painter.save();

	QList<EditItem>::iterator iter;
	for (iter = m_listEdit.begin();
		iter != m_listEdit.end();
		++iter)
	{
		EditItem item = *iter;
		drawEditItem(painter, item);
	}

	painter.restore();
}

void KScreenGrabDialog::drawOverlay(QPainter& painter, const QBrush& brush)
{
	if (m_rcSelection.isEmpty())
		return;

	painter.save();
	painter.setClipping(true);
	painter.setPen(Qt::NoPen);
	painter.setBrush(brush);
	switch (m_drawShape)
	{
	case dsRect:
		painter.setClipRegion(QRegion(m_rcDesktop) - QRegion(m_rcSelection));
		break;
	case dsEllipse:
		painter.setClipRegion(QRegion(m_rcDesktop) - QRegion(m_rcSelection, QRegion::Ellipse));
		break;
	case dsRoundedRect:
	{
		QPainterPath pathDesktop, pathSelection;
		pathDesktop.addRect(m_rcDesktop);
		pathSelection.addRoundedRect(m_rcSelection, roundedRadius, roundedRadius);
		painter.setClipPath(pathDesktop - pathSelection);
		break;
	}
	case dsPolygon:
	{
		const QPolygon& targetPolygon = (m_polygonState == dpsStart) ? m_polyRubberSelection : m_polySelection;
		QPainterPath pathDesktop, pathSelection;
		pathDesktop.addRect(m_rcDesktop);
		pathSelection.addPolygon(targetPolygon);
		painter.setClipPath(pathDesktop - pathSelection);
		break;
	}
	default:
		Q_ASSERT(false);
	}
	painter.drawRect(m_rcDesktop);
	painter.setClipping(false);

	painter.restore();
}

void KScreenGrabDialog::drawSelection(QPainter& painter, const QBrush& brush, int width)
{
	QPen pen(colorSelectionEdge);
	pen.setWidth(width);
	pen.setJoinStyle(Qt::MiterJoin);
	painter.save();
	painter.setPen(pen);
	painter.setBrush(brush);
	painter.setRenderHint(QPainter::Antialiasing);
	switch (m_drawShape)
	{
	case dsRect:
		painter.drawRect(m_rcSelection);
		break;
	case dsEllipse:
		if (!m_rcSelection.isEmpty())
			painter.drawEllipse(m_rcSelection);
		break;
	case dsRoundedRect:
		if (!m_rcSelection.isEmpty())
			painter.drawRoundedRect(m_rcSelection, roundedRadius, roundedRadius);
		break;
	case dsPolygon:
	{
		const QPolygon& targetPolygon = (m_polygonState == dpsStart) ? m_polyRubberSelection : m_polySelection;
		if (!m_rcSelection.isEmpty())
		{
			painter.drawPolygon(targetPolygon);
		}
		break;
	}
	default:
		Q_ASSERT(false);
	}
	painter.restore();

	if (m_drawShape != dsRect)
	{
		painter.save();
		painter.setBrush(brush);
		painter.setPen(pen);
		QRect rc = m_rcSelection.adjusted(dpiScaled(2), dpiScaled(2), dpiScaled(-2), dpiScaled(-2));
		painter.drawRect(m_rcSelection);
		painter.restore();
	}
}

bool KScreenGrabDialog::lineContainsPos(QPoint* points, int nPoints, int nWidth, const QPoint& pos)
{
	if (!points)
		return false;

	for (int var = 0; var < nPoints; ++var)
	{
		qreal radius = (qreal)nWidth / 2.0;
		QPainterPath path;

		path.addEllipse(points[var], radius, radius);

		if (path.contains(pos))
			return true;

		if (var + 1 < nPoints)
		{
			if (points[var].x() == points[var + 1].x() ||
					points[var].y() == points[var + 1].y())
			{
				QPointF topLeft = QPointF(points[var]) - QPointF(radius, radius);
				QPointF bottomRight = QPointF(points[var + 1]) + QPointF(radius, radius);
				if (QRectF(topLeft, bottomRight).contains(pos))
					return true;
			}
			else
			{
				QPointF ptVar1 = QPointF(points[var]);
				QPointF ptVar2 = QPointF(points[var + 1]);
				QVector<QPointF> vec1;
				QPointF p1 = QPointF(ptVar1.x() - radius, ptVar1.y() + radius);
				QPointF p2 = QPointF(ptVar2.x() - radius, ptVar2.y() + radius);
				QPointF p3 = QPointF(ptVar1.x() + radius, ptVar1.y() - radius);
				QPointF p4 = QPointF(ptVar2.x() + radius, ptVar2.y() + radius);
				vec1.push_back(p1);
				vec1.push_back(p3);
				vec1.push_back(p4);
				vec1.push_back(p2);
				QPainterPath path1;
				path1.addPolygon(vec1);
				if (path1.contains(QPointF(pos)))
					return true;
			}
		}

	}
	return false;
}
bool KScreenGrabDialog::arrowContaisPos(const QPoint& ptStart, const QPoint& ptEnd, int nSize, const QPoint& pos)
{
	if (!ptStart.isNull() &&
		!ptEnd.isNull())
	{
		QLineF lineArrow(ptStart, ptEnd);
		qreal rArrowLength = lineArrow.length();
		qreal rArrowHeadLength = 0;
		qreal rArrowHeadWidth = 0;
		qreal rArrowSideWidth = 0;

		switch (nSize)
		{
		case penWidthSmall:
			rArrowHeadLength = 10;
			rArrowHeadWidth = 10;
			rArrowSideWidth = 3;
			break;
		case penWidthMiddle:
			rArrowHeadLength = 16;
			rArrowHeadWidth = 16;
			rArrowSideWidth = 4;
			break;
		case penWidthLarge:
			rArrowHeadLength = 20;
			rArrowHeadWidth = 20;
			rArrowSideWidth = 5;
			break;
		default:
			break;
		}

		QPointF pt = lineArrow.pointAt(
			(rArrowLength - rArrowHeadLength) / rArrowLength);
		QLineF lineArrowHead(pt, lineArrow.p2());
		lineArrowHead.setAngle(lineArrow.angle() + 90);

		QPointF ptMid = lineArrowHead.pointAt(0.5);
		lineArrowHead.translate(pt - ptMid);

		QPointF pt1 = lineArrowHead.pointAt(0);
		QPointF pt2 = lineArrowHead.pointAt(1);

		QPointF pts[3] = {ptEnd, pt1, pt2};

		QVector<QPointF> vec;
		vec.push_back(pts[0]);
		vec.push_back(pts[1]);
		vec.push_back(pts[2]);
		QPolygonF polygon1(vec);
		QPainterPath painterPath;
		painterPath.addPolygon(polygon1);
		if (painterPath.contains(pos))
			return true;

		pt1 = lineArrowHead.pointAt(
			(rArrowHeadWidth - rArrowSideWidth) / rArrowHeadWidth);
		pt2 = lineArrowHead.pointAt(
			1 - (rArrowHeadWidth - rArrowSideWidth) / rArrowHeadWidth);
		QPointF ptOffset = lineArrow.pointAt(
			(rArrowLength - rArrowHeadLength + 2) / rArrowLength);
		QLineF lineSide(pt1, pt2);
		ptMid = lineSide.pointAt(0.5);
		lineSide.translate(ptOffset - ptMid);

		pts[0] = ptStart;
		pts[1] = lineSide.p1();
		pts[2] = lineSide.p2();

		QVector<QPointF> vec2;
		vec2.push_back(pts[0]);
		vec2.push_back(pts[1]);
		vec2.push_back(pts[2]);
		QPolygonF polygon2(vec2);
		QPainterPath painterPath2;
		painterPath2.addPolygon(polygon2);
		if (painterPath2.contains(pos))
			return true;
	}
	return false;
}

bool KScreenGrabDialog::textItemDoubleClicked(const QPoint& pos)
{
	if (m_listEdit.isEmpty())
		return false;

	bool bHitEditItem = false;
	for (int i = m_listEdit.count() - 1; i >=0 ; --i)
	{
		m_listEdit[i].drawType = itemNone;
		EditItem item =  m_listEdit.at(i);
		if (item.editType == editText)
		{
			if (item.rcText->contains(pos))
			{
				if (m_editType != editText)
					onActionText();

				itemSelectTest(bHitEditItem, i, itemTextEdit);
				if (bHitEditItem)
				{
					showTextEdit(*m_listEdit[i].rcText, *m_listEdit[i].text);
					if (m_listEdit[i].text)
						m_listEdit[i].text->clear();

					m_itemDrawType == itemTextEdit;
					m_bTextDoubleClicked = true;
					return true;
				}
			}
		}

	}

	return false;
}

void KScreenGrabDialog::showTextEdit(const QRect& rc, const QString& text)
{
	if (NULL == m_textEdit)
	{
		return;
	}

	m_rcTextEdit = QRect();
	m_rcTextEditBackup = QRect();
	m_fMoving = false;

	m_textEdit->clear();
	m_strText.clear();

	QRect rcEdit(rc.adjusted(-4, -5, -4, -5));

	rcEdit.moveTop(qMax(m_rcSelection.top() + 1, rcEdit.top()));
	rcEdit.moveLeft(qMax(m_rcSelection.left() + 1, rcEdit.left()));
	rcEdit.moveBottom(qMin(m_rcSelection.bottom() - 1, rcEdit.bottom()));
	rcEdit.moveRight(qMin(m_rcSelection.right() - 1, rcEdit.right()));

	m_textEdit->setGeometry(rcEdit.adjusted(0, 1, 0, 0));
	m_textEdit->setMaximumHeight(m_rcSelection.bottom() - rcEdit.top() - 1);
	m_textEdit->setMaximumWidth(m_rcSelection.right() - rcEdit.left() - 1);
	m_textEdit->setMinimumHeight(dpiScaled(20));
	m_textEdit->setMinimumWidth(dpiScaled(30));

	m_rcTextEdit = rcEdit;
	m_textEdit->show();

	showTextEditMaskWidget();
	m_textEdit->setEnabled(true);
	m_textEdit->setFocus();
	m_textEdit->ensureCursorVisible();

	m_nEditWidth = m_rcTextEdit.width();
	m_nEditHeight = m_rcTextEdit.height();


	QPalette pl = m_textEdit->palette();
	pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
	m_textEdit->setPalette(pl);
	m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_textEdit->setWordWrapMode(QTextOption::ManualWrap);

	setTextFontColor(m_textEdit, m_strTextFontFamily, m_nTextFontSize, m_colorText);

	m_textEdit->setFrameShape(QFrame::NoFrame);
	m_textEdit->setText(text);
	QTextCursor cursor = m_textEdit->textCursor();
	cursor.movePosition(QTextCursor::End);
	m_textEdit->setTextCursor(cursor);
}

KScreenGrabDialog::ItemDrawType KScreenGrabDialog::getItemDrawType(const QPoint& pos)
{
	if (m_listEdit.isEmpty())
		return itemInsert;

	bool bHitEditItem = false;
	ItemDrawType tempDrawType = itemInsert;
	for (int i = m_listEdit.count() - 1; i >=0 ; --i)
	{
		m_listEdit[i].drawType = itemNone;
		EditItem item =  m_listEdit.at(i);
		if (item.editType == editRect || item.editType == editEllipse)
		{
			if (item.editType == editRect)
			{
				if (posInRect(*item.rcPaint, item.width, pos))
					itemSelectTest(bHitEditItem, i, itemMove);
			}
			else if (item.editType == editEllipse)
			{
				if (posInEllipse(*item.rcPaint, item.width, pos))
					itemSelectTest(bHitEditItem, i, itemMove);
			}
			if (item.rcTopleft->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeTopLeft);
			else if (item.rcTopRight->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeTopRight);
			else if (item.rcBottomLeft->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeBottomLeft);
			else if (item.rcBottomRight->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeBottomRight);
			else if (item.rcLeftMid->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeLeftMid);
			else if (item.rcRightMid->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeRightMid);
			else if (item.rcTopMid->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeTopMid);
			else if (item.rcBottomMid->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeBottomMid);
		}
		else if (item.editType == editArrow)
		{
			if (arrowContaisPos(*item.ptStart, *item.ptEnd, item.width, pos))
				itemSelectTest(bHitEditItem, i, itemMove);

			if (item.rcBegin->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeBegin);
			else if (item.rcEnd->contains(pos))
				itemSelectTest(bHitEditItem, i, itemResizeEnd);
		}
		else if (item.editType == editBrush)
		{
			if (lineContainsPos(item.points, item.nPoints, item.width, pos))
				itemSelectTest(bHitEditItem, i, itemMove);
		}
		else if (item.editType == editText)
		{
			if (item.rcText->contains(pos))
				itemSelectTest(bHitEditItem, i, itemMove);
		}
	}
	return bHitEditItem ? m_listEdit[m_nShapeChangeItemID].drawType : itemInsert;
}

void KScreenGrabDialog::changeActionSelect(int nItemId)
{
	if (nItemId < 0 || nItemId >= m_listEdit.count())
		return;

	if (m_listEdit[nItemId].editType != m_editType)
	{
		switch (m_listEdit[nItemId].editType)
		{
		case editNone:
			// do nothing
			break;
		case editRect:
			if (m_toolbtnRect && m_toolbarSettings)
			{
				m_toolbtnRect->actionClick();
				m_toolbarSettings->show();
			}
			break;
		case editEllipse:
			if (m_toolbtnEllipse && m_toolbarSettings)
			{
				m_toolbtnEllipse->actionClick();
				m_toolbarSettings->show();
			}
			break;
		case editArrow:
			if (m_toolbtnArrow && m_toolbarSettings)
			{
				m_toolbtnArrow->actionClick();
				m_toolbarSettings->show();
			}
			break;
		case editBrush:
			if (m_toolbtnBrush && m_toolbarSettings)
			{
				m_toolbtnBrush->actionClick();
				m_toolbarSettings->show();
			}
			break;
		case editMosaic:
			if (m_toolbtnMosaic && m_toolbarSettings)
			{
				m_toolbtnMosaic->actionClick();
				m_toolbtnMosaic->show();
			}
			break;
		case editText:
			if (m_toolbtnText && m_toolbarSettings)
			{
				m_toolbtnText->actionClick();
				m_toolbarSettings->show();
			}
			break;
		case editFilter:
			// do nothing
			break;
		default:
			break;
		}
	}
}

void KScreenGrabDialog::itemSelectTest(bool& bHitEditItem, int nId, KScreenGrabDialog::ItemDrawType type)
{
	if (!bHitEditItem)
	{
		emit sigChangeItemAction(nId);
		m_listEdit[nId].drawType = type;
		bHitEditItem = true;
		m_nShapeChangeItemID = nId;
		if (m_editType == editRect || m_editType == editEllipse)
		{
			m_rectEditBackup = *m_listEdit[nId].rcPaint;
			backupRestzeCircle(nId);
		}
		else if (m_editType == editArrow)
		{
			m_ptStartEditBackup = *m_listEdit[nId].ptStart;
			m_ptEndEditBackup = *m_listEdit[nId].ptEnd;
		}
		else if (m_editType == editBrush)
		{
			if (m_nPointsBackup < m_listEdit[nId].nPoints)
			{
				if (m_pointsBrushBackup)
				{
					delete[] m_pointsBrushBackup;
					m_pointsBrushBackup = nullptr;
				}
				m_pointsBrushBackup = new QPoint[m_listEdit[nId].nPoints];
			}
			m_nPointsBackup = m_listEdit[nId].nPoints;
			memcpy(m_pointsBrushBackup, m_listEdit[nId].points, sizeof(QPoint) * m_listEdit[nId].nPoints);
		}
		else if (m_editType == editText)
		{
			m_textEditBackup = *m_listEdit[nId].rcText;
		}
	}
	else
	{
		if (m_listEdit[nId].drawType == itemMove)
		{
			m_listEdit[nId].drawType = type;
			if (m_editType == editRect || m_editType == editEllipse)
			{
				m_rectEditBackup = *m_listEdit[nId].rcPaint;
				backupRestzeCircle(nId);
			}
			else if (m_editType == editArrow)
			{
				m_ptStartEditBackup = *m_listEdit[nId].ptStart;
				m_ptEndEditBackup = *m_listEdit[nId].ptEnd;
			}
			else if (m_editType == editBrush)
			{
				if (m_nPointsBackup < m_listEdit[nId].nPoints)
				{
					if (m_pointsBrushBackup)
					{
						delete[] m_pointsBrushBackup;
						m_pointsBrushBackup = nullptr;
					}
					m_pointsBrushBackup = new QPoint[m_listEdit[nId].nPoints];		
				}
				m_nPointsBackup = m_listEdit[nId].nPoints;
				memcpy(m_pointsBrushBackup, m_listEdit[nId].points, sizeof(QPoint) * m_listEdit[nId].nPoints);
			}
			else if (m_editType == editText)
			{
				m_textEditBackup = *m_listEdit[nId].rcText;
			}
			m_nShapeChangeItemID = nId;
		}
	}
}

void KScreenGrabDialog::clearEditState()
{
	if (m_listEdit.isEmpty())
		return;

	for (int var = 0; var < m_listEdit.count(); ++var)
	{
		m_listEdit[var].drawType = itemNone;
	}

	update();
}

void KScreenGrabDialog::backupRestzeCircle(int nId)
{
	if (nId < 0 || nId >= m_listEdit.count())
		return;

	if (m_listEdit[nId].editType == editRect ||
			m_listEdit[nId].editType == editEllipse)
	{
		m_rcTopleftBackup = *m_listEdit[nId].rcTopleft;
		m_rcTopMidBackup = *m_listEdit[nId].rcTopMid;
		m_rcTopRightBackup = *m_listEdit[nId].rcTopRight;
		m_rcLeftMidBackup = *m_listEdit[nId].rcLeftMid;
		m_rcRightMidBackup = *m_listEdit[nId].rcRightMid;
		m_rcBottomLeftBackup = *m_listEdit[nId].rcBottomLeft;
		m_rcBottomMidBackup = *m_listEdit[nId].rcBottomMid;
		m_rcBottomRightBackup = *m_listEdit[nId].rcBottomRight;
	}
	else if (m_listEdit[nId].editType == editArrow)
	{
		m_rcBeginBackup = *m_listEdit[nId].rcBegin;
		m_rcEndBackup = *m_listEdit[nId].rcEnd;
	}

}

bool KScreenGrabDialog::popOperation(bool bClear)
{
	if (m_editType == editText && m_itemOperation == optColorChange && !bClear)
	{
		if (m_textEdit->isVisible())
		{
			setTextFontColor(m_textEdit, m_strTextFontFamily, m_nTextFontSize, m_colorBackup);
			return true;
		}
	}
	if (m_stackOpt.isEmpty())
		return false;

	ItemOperation opt = m_stackOpt.pop();
	if (opt.optType == optAdd)
		return false;

	if (opt.optType == optDel)
	{
		if (!bClear)
		{
			if (!m_stackDelete.isEmpty())
			{
				EditItem item = m_stackDelete.pop();
				item.drawType = itemNone;
				m_listEdit.push_back(item);
				update();
			}
		}
		return true;
	}

	if (opt.itemId > 0 && opt.itemId < m_listEdit.count() || bClear)
	{
		if (opt.optType == optResize || opt.optType == optMove)
		{
			if (opt.editType == editRect || opt.editType == editEllipse)
			{
				if (!bClear)
				{
					memcpy(m_listEdit[opt.itemId].rcPaint, opt.rcPaintBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcTopleft, opt.rcTopleftBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcTopMid, opt.rcTopMidBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcTopRight, opt.rcTopRightBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcLeftMid, opt.rcLeftMidBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcRightMid, opt.rcRightMidBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcBottomLeft, opt.rcBottomLeftBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcBottomMid, opt.rcBottomMidBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcBottomRight, opt.rcBottomRightBack, sizeof(QRect));
				}
				if (opt.rcPaintBack)
					delete opt.rcPaintBack;
				if (opt.rcTopleftBack)
					delete opt.rcTopleftBack;
				if (opt.rcTopMidBack)
					delete opt.rcTopMidBack;
				if (opt.rcTopRightBack)
					delete opt.rcTopRightBack;
				if (opt.rcLeftMidBack)
					delete opt.rcLeftMidBack;
				if (opt.rcRightMidBack)
					delete opt.rcRightMidBack;
				if (opt.rcBottomLeftBack)
					delete opt.rcBottomLeftBack;
				if (opt.rcBottomMidBack)
					delete opt.rcBottomMidBack;
				if (opt.rcBottomRightBack)
					delete opt.rcBottomRightBack;

				opt.rcPaintBack = nullptr;
				opt.rcTopleftBack = nullptr;
				opt.rcTopMidBack = nullptr;
				opt.rcTopRightBack = nullptr;
				opt.rcLeftMidBack = nullptr;
				opt.rcRightMidBack = nullptr;
				opt.rcBottomLeftBack = nullptr;
				opt.rcBottomMidBack = nullptr;
				opt.rcBottomRightBack = nullptr;
			}
			else if (opt.editType == editArrow)
			{
				if (!bClear)
				{
					memcpy(m_listEdit[opt.itemId].ptStart, opt.ptStartBack, sizeof(QPoint));
					memcpy(m_listEdit[opt.itemId].ptEnd, opt.ptEndBack, sizeof(QPoint));
					memcpy(m_listEdit[opt.itemId].rcBegin, opt.rcBeginBack, sizeof(QRect));
					memcpy(m_listEdit[opt.itemId].rcEnd, opt.rcEndBack, sizeof(QRect));
				}

				if (opt.ptStartBack)
					delete opt.ptStartBack;
				if (opt.ptEndBack)
					delete opt.ptEndBack;
				if (opt.rcBeginBack)
					delete opt.rcBeginBack;
				if (opt.rcEndBack)
					delete opt.rcEndBack;

				opt.ptStartBack = nullptr;
				opt.ptEndBack = nullptr;
				opt.rcBeginBack = nullptr;
				opt.rcEndBack = nullptr;
			}
			else if (opt.editType == editBrush)
			{
				if (!bClear)
				{
					if (opt.nPoints <= m_listEdit[opt.itemId].nPoints)
						memcpy(m_listEdit[opt.itemId].points, opt.pointsBack, sizeof(QPoint) * opt.nPoints);
				}

				if (opt.pointsBack)
					delete[] opt.pointsBack;

				opt.pointsBack = nullptr;
			}
			else if (opt.editType == editText)
			{
				if (!bClear)
					memcpy(m_listEdit[opt.itemId].rcText, opt.rcTextBack, sizeof(QRect));

				if (opt.rcTextBack)
					delete opt.rcTextBack;

				opt.rcTextBack = nullptr;
			}

		}
		else if (opt.optType == optTextEdit)
		{
			if (!bClear)
			{
				if (m_listEdit[opt.itemId].text)
				{
					delete m_listEdit[opt.itemId].text;
					m_listEdit[opt.itemId].text = nullptr;
					m_listEdit[opt.itemId].text = new QString(*opt.textBack);
				}
				memcpy(m_listEdit[opt.itemId].rcText, opt.rcTextBack, sizeof(QRect));
			}
			if (opt.textBack)
				delete opt.textBack;
			if (opt.rcTextBack)
				delete opt.rcTextBack;

			opt.textBack = nullptr;
			opt.rcTextBack = nullptr;
		}
		else if (opt.optType == optPenWidthChange)
		{
			if (!bClear)
				m_listEdit[opt.itemId].width = opt.widthBack;
		}
		else if (opt.optType == optColorChange)
		{
			if (!bClear)
				m_listEdit[opt.itemId].color = opt.colorBack;
		}
		else if (opt.optType == optFontSizeChange)
		{
			if (!bClear)
				m_listEdit[opt.itemId].fontSize = m_nFontSizeBackup;
		}
		if (!bClear)
			update();
	}
	return true;
}

void KScreenGrabDialog::pushOperation()
{
	if (m_nShapeChangeItemID < 0 || m_nShapeChangeItemID >= m_listEdit.count())
		return;

	EditItem editItem = m_listEdit[m_nShapeChangeItemID];

	ItemOperation itemOperation;
	itemOperation.editType = editItem.editType;
	itemOperation.optType = m_itemOperation;
	itemOperation.itemId = m_nShapeChangeItemID;
	if (m_itemOperation == optMove || m_itemOperation == optResize)
	{
		switch (editItem.editType)
		{
		case editRect:
		case editEllipse:
		{
			itemOperation.rcPaintBack = new QRect(m_rectEditBackup);
			itemOperation.rcTopleftBack = new QRect(m_rcTopleftBackup);
			itemOperation.rcTopMidBack = new QRect(m_rcTopMidBackup);
			itemOperation.rcTopRightBack = new QRect(m_rcTopRightBackup);
			itemOperation.rcLeftMidBack = new QRect(m_rcLeftMidBackup);
			itemOperation.rcRightMidBack = new QRect(m_rcRightMidBackup);
			itemOperation.rcBottomLeftBack = new QRect(m_rcBottomLeftBackup);
			itemOperation.rcBottomMidBack = new QRect(m_rcBottomMidBackup);
			itemOperation.rcBottomRightBack = new QRect(m_rcBottomRightBackup);
		}
			break;
		case editArrow:
		{
			itemOperation.ptStartBack = new QPoint(m_ptStartEditBackup);
			itemOperation.ptEndBack = new QPoint(m_ptEndEditBackup);
			itemOperation.rcBeginBack = new QRect(m_rcBeginBackup);
			itemOperation.rcEndBack = new QRect(m_rcEndBackup);
		}
			break;
		case editBrush:
		{
			itemOperation.nPoints = m_nPointsBackup;
			itemOperation.pointsBack = new QPoint[itemOperation.nPoints];
			if (m_pointsBrushBackup)
				memcpy(itemOperation.pointsBack, m_pointsBrushBackup, sizeof(QPoint) * itemOperation.nPoints);
		}
			break;
		case editText:
		{
			itemOperation.rcTextBack = new QRect(m_textEditBackup);
		}
			break;
		default:
			break;
		}
	}
	else if (m_itemOperation == optTextEdit)
	{
		itemOperation.textBack = new QString(m_strTextEditBackup);
		itemOperation.rcTextBack = new QRect(m_textEditBackup);
	}
	else if (m_itemOperation == optPenWidthChange)
	{
		itemOperation.widthBack = m_nPenWidthBackup;
	}
	else if (m_itemOperation == optFontSizeChange)
	{
		itemOperation.fontSizeBack = m_nFontSizeBackup;
	}
	else if (m_itemOperation == optColorChange)
	{
		itemOperation.colorBack = m_colorBackup;
	}
	m_stackOpt.push(itemOperation);
}

void KScreenGrabDialog::changeItemShape(const QPoint& pos)
{
	if (m_nShapeChangeItemID < 0 || m_nShapeChangeItemID >= m_listEdit.count())
		return;

	QRect rcTemp = QRect();
	bool bShapeChanged = true;
	if (m_itemDrawType == itemResizeTopLeft)
	{
		rcTemp.setTopLeft(pos);
		rcTemp.setBottomRight(m_rectEditBackup.bottomRight());
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeTopMid)
	{
		rcTemp.setBottomLeft(m_rectEditBackup.bottomLeft());
		rcTemp.setTopRight(QPoint(m_rectEditBackup.right(), pos.y()));
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeTopRight)
	{
		rcTemp.setTopRight(pos);
		rcTemp.setBottomLeft(m_rectEditBackup.bottomLeft());
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeLeftMid)
	{
		rcTemp.setTopRight(m_rectEditBackup.topRight());
		rcTemp.setBottomLeft(QPoint(pos.x(), m_rectEditBackup.bottom()));
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeRightMid)
	{
		rcTemp.setTopLeft(m_rectEditBackup.topLeft());
		rcTemp.setBottomRight(QPoint(pos.x(), m_rectEditBackup.bottom()));
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeBottomLeft)
	{
		rcTemp.setBottomLeft(pos);
		rcTemp.setTopRight(m_rectEditBackup.topRight());
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeBottomRight)
	{
		rcTemp.setBottomRight(pos);
		rcTemp.setTopLeft(m_rectEditBackup.topLeft());
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeBottomMid)
	{
		rcTemp.setTopLeft(m_rectEditBackup.topLeft());
		rcTemp.setBottomRight(QPoint(m_rectEditBackup.right(), pos.y()));
		rcTemp = rcTemp.normalized();
		memcpy(m_listEdit[m_nShapeChangeItemID].rcPaint, &rcTemp, sizeof(QRect));
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeBegin)
	{
		m_listEdit[m_nShapeChangeItemID].ptStart->setX(pos.x());
		m_listEdit[m_nShapeChangeItemID].ptStart->setY(pos.y());
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemResizeEnd)
	{
		m_listEdit[m_nShapeChangeItemID].ptEnd->setX(pos.x());
		m_listEdit[m_nShapeChangeItemID].ptEnd->setY(pos.y());
		m_itemOperation = optResize;
	}
	else if (m_itemDrawType == itemMove)
	{
		QPoint offset = pos - m_ptPressed;
		if (m_listEdit[m_nShapeChangeItemID].editType == editRect ||
				m_listEdit[m_nShapeChangeItemID].editType == editEllipse)
		{
			if (m_rectEditBackup.isValid())
			{
				QRect rect = m_rectEditBackup.adjusted(
							offset.x(), offset.y(), offset.x(), offset.y());
				m_listEdit[m_nShapeChangeItemID].rcPaint->setTopLeft(rect.topLeft());
				m_listEdit[m_nShapeChangeItemID].rcPaint->setBottomRight(rect.bottomRight());
				m_itemOperation = optMove;
			}
		}
		else if (m_listEdit[m_nShapeChangeItemID].editType == editArrow)
		{
			if (!m_ptStartEditBackup.isNull() && !m_ptStartEditBackup.isNull())
			{
				QPoint p1 = m_ptStartEditBackup + offset;
				QPoint p2 = m_ptEndEditBackup + offset;
				m_listEdit[m_nShapeChangeItemID].ptStart->setX(p1.x());
				m_listEdit[m_nShapeChangeItemID].ptStart->setY(p1.y());
				m_listEdit[m_nShapeChangeItemID].ptEnd->setX(p2.x());
				m_listEdit[m_nShapeChangeItemID].ptEnd->setY(p2.y());
				m_itemOperation = optMove;
			}
		}
		else if (m_listEdit[m_nShapeChangeItemID].editType == editBrush)
		{
			for (int var = 0; var < m_listEdit[m_nShapeChangeItemID].nPoints; ++var)
			{
				QPoint pt = m_pointsBrushBackup[var] + offset;
				m_listEdit[m_nShapeChangeItemID].points[var].setX(pt.x());
				m_listEdit[m_nShapeChangeItemID].points[var].setY(pt.y());
			}
			m_itemOperation = optMove;
		}
		else if (m_listEdit[m_nShapeChangeItemID].editType == editText)
		{
			QRect rect = m_textEditBackup.adjusted(
						offset.x(), offset.y(), offset.x(), offset.y());
			m_listEdit[m_nShapeChangeItemID].rcText->setTopLeft(rect.topLeft());
			m_listEdit[m_nShapeChangeItemID].rcText->setBottomRight(rect.bottomRight());
			m_itemOperation = optMove;
		}
	}
	else
	{
		m_itemOperation = optNone;
		bShapeChanged = false;
	}

	updateItemResizeCircle();
}

void KScreenGrabDialog::updateItemResizeCircle()
{
	if (m_nShapeChangeItemID < 0 || m_nShapeChangeItemID >= m_listEdit.count())
		return;

	if (m_listEdit[m_nShapeChangeItemID].editType == editRect ||
			m_listEdit[m_nShapeChangeItemID].editType == editEllipse)
	{
		memcpy(m_listEdit[m_nShapeChangeItemID].rcTopleft, &m_rcTopleft, sizeof(m_rcTopleft));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcTopRight, &m_rcTopRight, sizeof(m_rcTopRight));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcTopMid, &m_rcTopMid, sizeof(m_rcTopMid));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcLeftMid, &m_rcLeftMid, sizeof(m_rcLeftMid));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcRightMid, &m_rcRightMid, sizeof(m_rcRightMid));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcBottomLeft, &m_rcBottomLeft, sizeof(m_rcBottomLeft));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcBottomMid, &m_rcBottomMid, sizeof(m_rcBottomMid));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcBottomRight, &m_rcBottomRight, sizeof(m_rcBottomRight));
	}
	else if (m_listEdit[m_nShapeChangeItemID].editType == editArrow)
	{
		memcpy(m_listEdit[m_nShapeChangeItemID].rcBegin, &m_rcBegin, sizeof(m_rcBegin));
		memcpy(m_listEdit[m_nShapeChangeItemID].rcEnd, &m_rcEnd, sizeof(m_rcEnd));
	}
}

bool KScreenGrabDialog::posInRect(const QRect& rc, int nPenWidth, const QPoint& pos)
{
	QRectF rcF = QRectF(rc);
	QPointF posF = QPointF(pos);
	qreal offset = (qreal)nPenWidth / 2.0;
	QRectF rectInside = QRectF(rcF.topLeft() + QPointF(offset + 1, offset + 1), rcF.bottomRight() - QPointF(offset + 1, offset + 1));
	QRectF rectOutside = QRectF(rcF.topLeft() - QPointF(offset + 1 , offset + 1), rcF.bottomRight() + QPointF(offset + 1, offset + 1));
	return rectOutside.contains(posF) && !rectInside.contains(posF);
}

bool KScreenGrabDialog::posInEllipse(const QRect& rc, int nPenWidth, const QPoint& pos)
{
	QRectF rcF = QRectF(rc);
	QPointF posF = QPointF(pos);
	qreal offset = (qreal)nPenWidth / 2.0;
	QRectF rectInside = QRectF(rcF.topLeft() + QPointF(offset, offset), rcF.bottomRight() - QPointF(offset, offset));
	QPainterPath path;
	path.addEllipse(rectInside);
	return rcF.contains(posF) && !path.contains(posF);
}

QRect KScreenGrabDialog::drawResizePoint(QPainter& painter, const QPoint& center, bool bCircle)
{
	painter.setRenderHint(QPainter::Antialiasing);
	int nSize = dpiScaled(6);
	QRect rect = QRect(center.x() - nSize / 2, center.y() - nSize / 2, nSize, nSize);
	if (bCircle)
	{
		painter.drawEllipse(rect);
	}
	else
	{
		painter.drawRect(rect);
	}
	return rect;
}

void KScreenGrabDialog::drawResizeCircle(QPainter& painter, const QPoint& p1, const QPoint& p2)
{
	QColor cirColor;
	if (m_nShapeChangeItemID >= 0 && m_nShapeChangeItemID < m_listEdit.count())
		cirColor = m_listEdit[m_nShapeChangeItemID].color;
	else
		cirColor = m_colorRect;

	cirColor = cirColor.isValid() ? cirColor : m_colorRect;

	painter.setPen(cirColor);
	painter.setBrush(Qt::white);
	m_rcBegin = drawResizePoint(painter, p1, true);
	m_rcEnd = drawResizePoint(painter, p2, true);
}

void KScreenGrabDialog::drawResizeCircle(QPainter& painter, const QRect& rc)
{
	QColor cirColor;
	if (m_nShapeChangeItemID >= 0 && m_nShapeChangeItemID < m_listEdit.count())
		cirColor = m_listEdit[m_nShapeChangeItemID].color;
	else
		cirColor = m_colorRect;

	cirColor = cirColor.isValid() ? cirColor : m_colorRect;
	if (m_editType == editEllipse)
	{
		painter.setPen(QPen(cirColor, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawRect(rc);
	}
	painter.setPen(cirColor);
	painter.setBrush(Qt::white);

	m_rcTopleft = drawResizePoint(painter, rc.topLeft(), true);
	m_rcTopRight = drawResizePoint(painter, rc.topRight(), true);
	m_rcBottomLeft = drawResizePoint(painter, rc.bottomLeft(), true);
	m_rcBottomRight = drawResizePoint(painter, rc.bottomRight(), true);

	QPoint center = rc.center();
	m_rcLeftMid = drawResizePoint(painter, QPoint(rc.left(), center.y()), true);
	m_rcRightMid = drawResizePoint(painter, QPoint(rc.right(), center.y()) + QPoint(1, 0), true);
	m_rcTopMid = drawResizePoint(painter, QPoint(center.x(), rc.top()), true);
	m_rcBottomMid = drawResizePoint(painter, QPoint(center.x(), rc.bottom()) + QPoint(0, 1), true);


}

void KScreenGrabDialog::drawResizePoint(QPainter& painter, const QRect& rc)
{
	painter.setPen(colorSelectionEdge);
	painter.setBrush(colorSelectionEdge);

	drawResizePoint(painter, rc.topLeft());
	drawResizePoint(painter, rc.topRight());
	drawResizePoint(painter, rc.bottomLeft());
	drawResizePoint(painter, rc.bottomRight());

	QPoint center = rc.center();
	drawResizePoint(painter, QPoint(rc.left(), center.y()));
	drawResizePoint(painter, QPoint(rc.right(), center.y()) + QPoint(1, 0));
	drawResizePoint(painter, QPoint(center.x(), rc.top()));
	drawResizePoint(painter, QPoint(center.x(), rc.bottom()) + QPoint(0, 1));
}

void KScreenGrabDialog::drawSizeTooptip(QPainter& painter)
{
	QRect rcSelection = m_rcSelection & m_rcDesktop;
	QSize sz = rcSelection.size();
	QString text = QString(tr("%1 * %2")).arg(sz.width()).arg(sz.height());

	QFont font = qApp->font("QToolTip");
		font.setPointSize(dpiScaled(12));

    QFontMetrics fm(font);
        QSize szText = QSize(dpiScaled(fm.horizontalAdvance(text)), dpiScaled((fm.height())));
		QSize szTip = QSize(szText.width() + dpiScaled(5), szText.height() + dpiScaled(5));
	QRect rc = QRect(QPoint(rcSelection.left(), qMax(0,
		rcSelection.top() - szTip.height() - 4)), szTip);

	QRect rcText = calcCenterRect(rc, szText);

	painter.save();
	painter.setPen(Qt::NoPen);
	painter.setBrush(QColor(0, 0, 0, 153));
	painter.drawRect(rc);

	//圆角
	painter.setPen(QColor(108, 108, 108));
	painter.drawPoint(rc.topLeft());
	painter.drawPoint(rc.topRight());
	painter.drawPoint(rc.bottomLeft());
	painter.drawPoint(rc.bottomRight());

	painter.setPen(Qt::white);
	painter.setFont(font);
	painter.drawText(rcText, Qt::AlignCenter, text);
	painter.restore();
}

void KScreenGrabDialog::drawPixelZoomBox(QPainter& painter)
{
	if (m_disableZoomBox)
		return;

	if (m_fResizing &&
		(cursorSizeAll == m_cursorBitmapType ||
		 cursorSelect == m_cursorBitmapType))
	{
		return;
	}

#ifdef Q_OS_MACOS
	qreal ratio = qApp->devicePixelRatio();
#endif

	QPoint pt = QCursor::pos() - m_screenOffset;
	QSize szFont = QSize(painter.device()->logicalDpiX() * 8 / 72 + 1,
		painter.device()->logicalDpiY() * 8 / 72 + 1);
	QRect rc = QRect(pt + QPoint(5, 25), szFont * 11);
	rc.adjust(0, 0, 2, 2);
	if (rc.bottom() > m_rcDesktop.bottom())
		rc.moveTop(pt.y() - rc.height() - 10);
	if (rc.right() > m_rcDesktop.right())
		rc.moveLeft(pt.x() - rc.width() - 2);

	painter.save();
	painter.setPen(QColor(125, 125, 125, 125));
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(rc.adjusted(0, 0, 1, 1));
	rc.adjust(1, 1, 0, 0);
	painter.setPen(Qt::white);
	painter.drawRect(rc);
	rc.adjust(1, 1, 0, 0);

	QFont font = qApp->font("QToolTip");
	font.setPointSize(8);

	QString text = tr("(%1, %2)").arg(pt.x()).arg(pt.y());
	QSize szText = QSize(szFont.width() * text.length() / 1.8, szFont.height() * 1.1);

	QRect rcLabel = rc;
	rcLabel.setTop(rc.bottom() - szText.height() * 3);
	rc.setHeight(rc.height() - rcLabel.height());
	rcLabel.setLeft(rcLabel.left() - 1);

	painter.setBrush(QColor("#2F2F2F"));
	painter.drawRect(rcLabel);

	rcLabel.setHeight(rcLabel.height() / 2);

	QRect rcText = calcCenterRect(rcLabel, szText);
	painter.setPen(Qt::white);
	painter.setFont(font);
	painter.drawText(rcText, Qt::AlignCenter, text);

#ifdef Q_OS_MACOS
	QRgb rgb = m_pixmapDesktop.toImage().pixel(QPoint(pt.x() * ratio, pt.y() * ratio));
#else
	QRgb rgb = m_pixmapDesktop.toImage().pixel(pt);
#endif
	text = tr("RGB:(%1, %2, %3)").arg(qRed(rgb)).arg(qGreen(rgb)).arg(qBlue(rgb));
	szText = QSize(szFont.width() * text.length() / 1.8, szFont.height() * 1.1);
	rcLabel.moveBottom(rcLabel.bottom() + rcLabel.height());
	rcText = calcCenterRect(rcLabel, szText);
	painter.drawText(rcText, Qt::AlignCenter, text);

	QSize szPixmap = QSize(rc.width() / 4 + 1, rc.height() / 4 + 1);
	QRect rcPixmap = calcCenterRect(rc, szPixmap);
	rcPixmap.moveCenter(pt);
#ifdef Q_OS_MACOS
	rcPixmap.setTopLeft(QPoint(rcPixmap.x() * ratio,rcPixmap.y() * ratio));
	rcPixmap.setBottomRight(QPoint(rcPixmap.right() * ratio,rcPixmap.bottom() * ratio));
#endif
	QPixmap pixmap = m_pixmapDesktop.copy(rcPixmap);
	painter.drawPixmap(rc, pixmap);

	QPen pen(QColor("#3399FF"));
	pen.setWidth(4);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);

	QPoint center = rc.center();
	painter.drawLine(center.x(), rc.top() + 2, center.x(), rc.bottom() - 1);
	painter.drawLine(rc.left() + 2, center.y(), rc.right() - 1, center.y());

	if (m_fResizing && cursorSizeAll != m_cursorBitmapType)
	{
		QRgb rgbBackgd = m_pixmapDesktop.toImage().pixel(pt + QPoint(0, 7));
		QColor color;
		color.setRed(0xFF - qRed(rgbBackgd));
		color.setGreen(0xFF - qGreen(rgbBackgd));
		color.setBlue(0xFF - qBlue(rgbBackgd));
		painter.setPen(color);
		painter.setBrush(Qt::NoBrush);
		QFont font("Arial", 10, QFont::Bold);
		painter.setFont(font);
#ifdef Q_OS_WIN
		text = tr("Press Alt\nto Adsorb Line");
#else
		text = tr(" ");
#endif
		szText = QSize(szFont.width() * text.length() / 1.1, szFont.height() * 4);
		rcText = calcCenterRect(rc, szText);
		rcText.setLeft(rc.left());
		rcText.setRight(rc.right());
		rcText.moveBottom(rcText.bottom() + 20);
		painter.drawText(rcText, Qt::AlignCenter, text);
	}

	painter.restore();
}

void KScreenGrabDialog::mousePressEvent(QMouseEvent* event)
{
	m_toolbarHeader->hideTooltip();
	if (m_toolbarHeader->isVisible() && m_toolbarHeader->geometry().contains(event->pos()))
	{
		m_badSelection = true;
		return;
	}
	else
	{
		m_badSelection = false;
	}

	if (event->buttons() & Qt::LeftButton)
	{
		m_bMousePressed = true;
		m_ptPressed = event->pos();
		if (NULL != m_Menu && m_Menu->isVisible())
		{
			hideMenu();
		}

		if (smSelection == m_selectmode)
		{
			if (!m_fEditing)
			{
				m_hitTestResizing = hitTest(m_ptPressed);
				m_rcSelectionBackup = m_rcSelection;
				if (m_drawShape == dsPolygon)
					m_polySelectionBackup = m_polySelection;
			}
			else
			{
				m_itemDrawType = getItemDrawType(m_ptPressed);
				if (m_editType == editText)
				{
					if (m_itemDrawType == itemInsert)
					{
						if (!m_bTextHasFocus && !m_bTextEditHasSelected)
						{
							resetTextEdit(event->pos());
						}
						else
						{
							if (m_bTextDoubleClicked)
								updateEditItemText();
							else
								saveText();
						}

						m_bTextEditHasSelected = false;
						m_bTextDoubleClicked = false;
					}
					else if (m_itemDrawType == itemMove)
					{
						m_bTextEditHasSelected = true;
						saveText();
					}
					update();
				}

				m_rcTextEditBackup = m_rcTextEdit;
				m_fMoving = isPtAtRectEdges(m_rcTextEdit, event->pos());

				if (!m_rcSelection.contains(m_ptPressed))
				{
					m_ptPressed = ptNullPressedPoint;
				}
			}
		}
		else // m_selectmode != smSelection
		{
			if (m_drawShape == dsPolygon)
			{
				hideToolBarHeader();
				m_toolbarHeader->hideTooltip();

				if (m_polygonState == dpsEnd)
				{
					m_polySelection.clear();
					m_polySelection << m_ptPressed;
					m_polygonState = dpsStart;
				}
				else if (m_polygonState == dpsStart)
				{
					QPoint nearest = nearestPoint(m_ptPressed, m_isOriginPoint);
					if (m_polySelection.size() > 0 && m_polySelection[0] == nearest)
					{
						endDrawPolygon();
						m_bSniffWindowGrab = false;
					}
					else
					{
						// record one point
						m_polySelection << m_ptPressed;
						m_polygonState = dpsHolding;
					}
				}
				m_rcSelection = m_polySelection.boundingRect();
				update();
			}
			else if (m_drawShape == dsRect)
			{
				// dsRect mode will sniff windows
				hideToolBarHeader();
				m_toolbarHeader->hideTooltip();
				if (m_windowInfo)
				{
					bool bFullScreen = false;
					m_pixmapSniff = m_windowInfo->getWindowPixmap(m_ptPressed, m_rcSelection, m_rcDesktop, m_screenOffset.y(), bFullScreen);
					if (bFullScreen && m_pixmapSniff.isNull())
						m_pixmapSniff = m_pixmapDesktop;

					m_bSniffWindowGrab = true;
					update();
				}
			}
		} // end of if (smSelection == m_selectmode)
	}
	else
	{
		m_ptPressed = ptNullPressedPoint;
	} // end of if (event->buttons() & Qt::LeftButton)

	QDialog::mousePressEvent(event);
}

void KScreenGrabDialog::mouseReleaseEvent(QMouseEvent* event)
{
	if (m_badSelection)
		return;

	if (event->button() & Qt::RightButton)
	{
		if (smSelection != m_selectmode)
		{
			m_exitType = exitMouseButtonR;
			return actionCanecl();
		}

		if (!m_rcSelection.contains(event->pos()))
		{
			reset();
			sniffingWindow(QCursor::pos());
		}
	}

	if (m_ptPressed != ptNullPressedPoint)
	{
		if (m_selectmode != smSelection)
		{
			if (!m_fInited)
			{
				initControls();
				m_fInited = true;
			}

			if (m_rcSelection.isEmpty())
				return;

			if (m_drawShape != dsPolygon)
			{
				m_rcSelection &= m_rcDesktop;
				m_widSelection = NULL;
				m_selectmode = smSelection;
				setToolBarVisible(m_fToolbarToggleOn);
			}
			else
			{
				if (m_polygonState == dpsHolding)
				{
					m_polygonState = dpsStart;
					drawRubber(event->pos());
				}
			}

			update();
		}
		else
		{
			if (m_fEditing)
			{
				if (m_itemDrawType == itemInsert)
				{
					switch (m_editType)
					{
					case editNone:
						break;
					case editRect:
						pushUndoStack();
						m_itemOperation = optAdd;
						pushOperation();
						break;
					case editEllipse:
						pushUndoStack();
						m_itemOperation = optAdd;
						pushOperation();
						break;
					case editArrow:
						pushUndoStack();
						m_itemOperation = optAdd;
						pushOperation();
						break;
					case editBrush:
					{
						if (m_bBrushHasDraw)
						{
							pushUndoStack();
							m_itemOperation = optAdd;
							pushOperation();
						}

						m_bBrushHasDraw = false;
					}
						break;
					case editText:
						if (m_fMoving)
						{
							m_fMoving = false;
						}
						else
						{

						}
						break;
					case editFilter:
						break;
					default:
						break;
					}
				}
				else if (m_itemDrawType != itemNone)
				{
					pushOperation();
				}
			}
			update();
		}
		m_bMousePressed = false;
	}
	else
	{
		if (m_selectmode != smSelection)
		{

		}
		else if (!m_rcSelection.contains(event->pos()))
		{
			update();
		}
	} // end of if (!m_ptPressed.isNull())

	m_fResizing = false;

	QPoint point= QPoint(event->pos() - m_screenOffset);
	resetCursor(point);
}

void KScreenGrabDialog::mouseMoveEvent(QMouseEvent* event)
{
	m_disableZoomBox = (m_toolbarHeader->isVisible() && m_toolbarHeader->geometry().contains(event->pos()));

	m_isOriginPoint = false;
	QPoint pt = mapToGlobal(event->pos());
	if (event->buttons() & Qt::LeftButton)
	{
		if (m_badSelection)
			return;

		if (!m_rcSelection.isEmpty())
		{
			hideToolBarHeader();
			m_toolbarHeader->hideTooltip();
		}

		if (m_ptPressed != ptNullPressedPoint)
		{
			if (m_selectmode == smSelection)
			{
				if (m_fEditing)
				{
					if (m_itemDrawType == itemInsert)
					{
						switch (m_editType)
						{
						case editNone:
							// do nothing
							break;
						case editRect:
							setPaintRect(event->pos());
							break;
						case editEllipse:
							setPaintRect(event->pos());
							break;
						case editArrow:
							setArrow(event->pos());
							break;
						case editBrush:
							setBrush(event->pos());
							break;
						case editMosaic:
							setMosaic(event->pos());
							break;
						case editText:
							if (m_fMoving || cursorSizeAll == m_cursorBitmapType)
							{
//								moveTextEditRect(event->pos());
							}
							break;
						case editFilter:
							// do nothing
							break;
						default:
							// do nothing
							break;
						}
					}
					else
					{
						changeItemShape(event->pos());
					}
				}
				else
				{
					m_fResizing = true;
					if (!m_bSniffSelectDone)
						moveSelection(event->pos());
				}
			}
			else
			{
				if (m_bMousePressed)
				{
					m_bSniffSelectDone = false;
					m_bSniffWindowGrab = false;
					m_pixmapSniff = QPixmap();
				}

				m_selectmode = smDraging;
				dragRegion(event->pos());
			}
		}
	}
	else if (!(event->buttons() & Qt::RightButton))
	{
		if (m_selectmode == smSniffingWindow)
			sniffingWindow(pt);

		drawRubber(event->pos());
	}

	QPoint point = QPoint(pt - m_screenOffset);
	resetCursor(point);

	update();

	QDialog::mouseMoveEvent(event);
}

void KScreenGrabDialog::mouseDoubleClickEvent(QMouseEvent* event)
{
	if (m_toolbarHeader->isVisible() && m_toolbarHeader->geometry().contains(event->pos()))
		return;

	if (event->button() == Qt::LeftButton)
	{
		if (m_rcSelection.isEmpty())
			return;

		if (m_drawShape == dsPolygon && m_polySelection.size() <= 1)
			return;

		if (m_drawShape == dsPolygon && m_polygonState == dpsStart)
		{
			m_polySelection << event->pos();
			endDrawPolygon();
		}
		else
		{
			if (!textItemDoubleClicked(event->pos()))
			{
				m_rcSelection.normalized();
				actionDone();

			}
		}
	}
	else
	{
		; //do nothing
	}
}

void KScreenGrabDialog::EscKeyHandler()
{
	if (!m_fInited || smSelection != m_selectmode)
	{
		this->setProperty(KScreenGrabHelper::Reason, KScreenGrabHelper::NoReason);
		reject();
	}
	else
	{
		actionUndo();
	}
}

const QSize* KScreenGrabDialog::getTooltipSizeHint(QWidget* w)
{
	auto iter = m_tooltipSizeHints.constFind(w);
	if (iter == m_tooltipSizeHints.end())
		return nullptr;
	return &*iter;
}
void KScreenGrabDialog::contextMenuEvent(QContextMenuEvent* event)
{
	if (m_selectmode == smSelection &&
		m_Menu != NULL &&
		m_rcSelection.contains(event->pos()) &&
		!m_rcTextEdit.contains(event->pos()))
	{
		QPoint pt = event->pos();
		if (m_rcDesktop.right() - pt.x() <= m_Menu->width())
		{
			pt.setX(m_rcDesktop.right() - m_Menu->width());
		}

		if (m_rcDesktop.bottom() - pt.y() <= m_Menu->height())
		{
			pt.setY(pt.y() - m_Menu->height());
			if (pt.y() < m_rcDesktop.top())
			{
				pt.setY(pt.y() + m_Menu->height() / 2);
			}
		}

		showMenu(pt);
	}
	else
		event->ignore();
}
void KScreenGrabDialog::showEvent(QShowEvent *)
{
	activateWindow();
	if (!m_windowInfo)
	{
		m_windowInfo = new KWindowInfo(this, this);
		KWindowUtil::setWindowTopLevel(this);
	}
}

void KScreenGrabDialog::reset()
{
	m_selectmode = smSniffingWindow;
	m_polyRubberSelection.clear();
	m_polySelection.clear();
	m_polySelectionBackup.clear();
	m_polygonState = dpsEnd;
	showToolBarHeader();
	toggleDrawAction();

	this->setCursor(Qt::ArrowCursor);
	m_cursorBitmapType = cursorSelect;
	m_cursorShape = Qt::BitmapCursor;

	m_fResizing = false;
	m_fAltPressed = false;
	m_fEditing = false;
	m_editType = editNone;
	clearUndoStack();
	m_fShowSetting = false;

	hideToolBar();

	m_ptMouseOld = QPoint();

	m_hitTestResizing = htNone;
	m_rcSelection = QRect();
	m_widSelection = NULL;
	m_ptPressed = ptNullPressedPoint;
	m_lineHoriz = qMakePair(0, 0);
	m_lineVert = qMakePair(0, 0);

	if (NULL != m_toolbtnRect)
	{
		toggleAction(m_toolbtnRect);
		m_toolbtnRect->setChecked(false);
	}

	m_fToolbarToggleOn = true;
	if (NULL != m_actionToggleToolbar)
	{
		m_actionToggleToolbar->setText(tr("Hide Edit Tool"));
	}

	resetTextEdit(QPoint());

	if (NULL != m_hint)
	{
		m_hint->move(-1000, -1000);
		m_hint->hide();
	}
	hideMenu();
}

void KScreenGrabDialog::sniffingWindow(const QPoint& pt)
{
	if (m_drawShape != dsRect)
		return;

	if (!m_windowInfo)
		return;

	QRect rc = m_windowInfo->getWindowRect(pt);
	m_rcSelection.setRect(rc.left(), rc.top(), rc.right() - rc.left() + 1,
		rc.bottom() - rc.top() + 1);
	m_rcSelection.adjust(-m_screenOffset.x(), -m_screenOffset.y(), -m_screenOffset.x(), -m_screenOffset.y());
	update();
}

void KScreenGrabDialog::dragRegion(const QPoint& pt)
{
	QPoint ptNew = pt;
	if (m_fAltPressed)
	{
		ptNew = adsorbLine(pt, m_ptMouseOld);
		m_ptMouseOld = pt;
	}

	if (m_drawShape != dsPolygon)
	{
		QRect rcNewSelection = QRect(m_ptPressed, ptNew).normalized();
		if (rcNewSelection != m_rcSelection)
		{
			m_rcSelection = rcNewSelection;
			update();
		}
	}
	else
	{
		if (!m_polySelection.contains(ptNew))
			m_polySelection << ptNew;

		m_polyRubberSelection = m_polySelection;
		m_rcSelection = m_polySelection.boundingRect();
		update();
	}
}

void KScreenGrabDialog::moveSelection(const QPoint& pt)
{
	m_toolbarSettings->hide();
	m_hint->hide();

	QPoint offset = pt - m_ptPressed;

	QPoint ptAdsorb = pt;
	if (m_fAltPressed)
	{
		ptAdsorb = adsorbLine(pt, m_ptMouseOld);
		m_ptMouseOld = pt;
	}

	switch (m_hitTestResizing)
	{
	case htNone:
		break;
	case htSizeAll:
		m_rcSelection = m_rcSelectionBackup.adjusted(
			offset.x(), offset.y(), offset.x(), offset.y());

		m_rcSelection.moveTop(qMax(m_rcSelection.top(), 0));
		m_rcSelection.moveLeft(qMax(m_rcSelection.left(), 0));
		m_rcSelection.moveBottom(qMin(m_rcSelection.bottom(), m_rcDesktop.bottom()));
		m_rcSelection.moveRight(qMin(m_rcSelection.right(), m_rcDesktop.right()));
		break;
	case htSizeLeft:
		m_rcSelection.setTopRight(m_rcSelectionBackup.topRight());
		m_rcSelection.setBottomLeft(QPoint(ptAdsorb.x(), m_rcSelectionBackup.bottom()));
		break;
	case htSizeRight:
		m_rcSelection.setTopLeft(m_rcSelectionBackup.topLeft());
		m_rcSelection.setBottomRight(QPoint(ptAdsorb.x(), m_rcSelectionBackup.bottom()));
		break;
	case htSizeTop:
		m_rcSelection.setBottomLeft(m_rcSelectionBackup.bottomLeft());
		m_rcSelection.setTopRight(QPoint(m_rcSelectionBackup.right(), ptAdsorb.y()));
		break;
	case htSizeBottom:
		m_rcSelection.setTopLeft(m_rcSelectionBackup.topLeft());
		m_rcSelection.setBottomRight(QPoint(m_rcSelectionBackup.right(), ptAdsorb.y()));
		break;
	case htSizeTopLeft:
		m_rcSelection.setTopLeft(ptAdsorb);
		m_rcSelection.setBottomRight(m_rcSelectionBackup.bottomRight());
		break;
	case htSizeTopRight:
		m_rcSelection.setTopRight(ptAdsorb);
		m_rcSelection.setBottomLeft(m_rcSelectionBackup.bottomLeft());
		break;
	case htSizeBottomLeft:
		m_rcSelection.setBottomLeft(ptAdsorb);
		m_rcSelection.setTopRight(m_rcSelectionBackup.topRight());
		break;
	case htSizeBottomRight:
		m_rcSelection.setBottomRight(ptAdsorb);
		m_rcSelection.setTopLeft(m_rcSelectionBackup.topLeft());
		break;
	default:
		break;
	}

	movePolygon(pt);
	update();
	relocateToolBar();
}

void KScreenGrabDialog::movePolygon(const QPoint& pt)
{
	if (m_drawShape != dsPolygon)
		return;

	QPoint offset = pt - m_ptPressed;
	switch (m_hitTestResizing)
	{
	case htNone:
		break;
	case htSizeAll:
	{
		QPolygon temp = m_polySelectionBackup.translated(offset);
		QRect aabb = temp.boundingRect();
		int offsetX = aabb.left() - m_rcDesktop.left();
		if (offsetX < 0)
			temp = temp.translated(-offsetX, 0);
		int offsetY = aabb.top() - m_rcDesktop.top();
		if (offsetY < 0)
			temp = temp.translated(0, -offsetY);

		m_polySelection = temp;
		break;
	}
	case htSizeLeft:
	case htSizeRight:
	case htSizeTop:
	case htSizeBottom:
	case htSizeTopLeft:
	case htSizeTopRight:
	case htSizeBottomLeft:
	case htSizeBottomRight:
	{
		QPoint anchor;
		if (m_hitTestResizing == htSizeRight || m_hitTestResizing == htSizeBottomRight || m_hitTestResizing == htSizeBottom)
			anchor = m_polySelectionBackup.boundingRect().topLeft();
		else if (m_hitTestResizing == htSizeTopLeft || m_hitTestResizing == htSizeLeft || m_hitTestResizing == htSizeTop)
			anchor = m_polySelectionBackup.boundingRect().bottomRight();
		else if (m_hitTestResizing == htSizeTopRight)
			anchor = m_polySelectionBackup.boundingRect().bottomLeft();
		else if (m_hitTestResizing == htSizeBottomLeft)
			anchor = m_polySelectionBackup.boundingRect().topRight();

		if (m_hitTestResizing == htSizeLeft || m_hitTestResizing == htSizeTopLeft || m_hitTestResizing == htSizeBottomLeft)
			offset.rx() *= -1;
		if (m_hitTestResizing == htSizeTop || m_hitTestResizing == htSizeTopLeft || m_hitTestResizing == htSizeTopRight)
			offset.ry() *= -1;

		float scaleX = (offset.x() + m_rcSelectionBackup.width()) * 1.f / m_rcSelectionBackup.width();
		float scaleY = (offset.y() + m_rcSelectionBackup.height()) * 1.f / m_rcSelectionBackup.height();

		if (m_hitTestResizing == htSizeLeft || m_hitTestResizing == htSizeRight)
			scaleY = 1;
		if (m_hitTestResizing == htSizeTop || m_hitTestResizing == htSizeBottom)
			scaleX = 1;

		QTransform transformScale = QTransform::fromScale(scaleX, scaleY);
		QPolygon temp = m_polySelectionBackup;
		for (auto iter = temp.begin(); iter != temp.end(); ++iter)
		{
			QTransform toOrigin = QTransform::fromTranslate(-anchor.x(), -anchor.y());
			QTransform toRestore = QTransform::fromTranslate(anchor.x(), anchor.y());
			(*iter) = (*iter) * toOrigin * transformScale * toRestore;
		}
		m_polySelection = std::move(temp);
		break;
	}
	default:
		break;
	}

}

void KScreenGrabDialog::drawRubber(const QPoint& pt)
{
	if (m_drawShape == dsPolygon && m_polygonState == dpsStart)
	{
		m_polyRubberSelection = m_polySelection;
		m_polyRubberSelection << nearestPoint(pt, m_isOriginPoint);

		m_rcSelection = m_polyRubberSelection.boundingRect();
	}
}

QPoint KScreenGrabDialog::nearestPoint(const QPoint& pt, bool& isOriginPoint)
{
	if (m_polySelection.size() > 0)
	{
		const int Range = dpiScaled(7);
		float distance = qSqrt(qPow(pt.x() - m_polySelection[0].x(), 2) + qPow(pt.y() - m_polySelection[0].y(), 2));
		if (distance < Range)
		{
			isOriginPoint = true;
			return m_polySelection[0];
		}
	}
	isOriginPoint = false;
	return pt;
}

void KScreenGrabDialog::endDrawPolygon()
{
	m_rcSelection = m_polySelection.boundingRect();
	m_rcSelection &= m_rcDesktop;
	m_widSelection = NULL;
	m_selectmode = smSelection;
	m_polygonState = dpsEnd;
	setToolBarVisible(m_fToolbarToggleOn);
}

QPair<int, int> KScreenGrabDialog::adsorbLine(const QPoint& pt, bool isHoriz, int nDirection)
{
	const int kAdsorbSensitiveDistance = 10;

	QPair<int, int> lineNone = qMakePair(0, 0);
	QPair<int, int> line = isHoriz ? m_lineHoriz : m_lineVert;
	int pos = isHoriz ? pt.x() : pt.y();

	QPair<int, int> lineX = m_lineHoriz;
	QPair<int, int> lineY = m_lineVert;
	if (line.first)
	{
		if (line.first < 0)
		{
			if (pos <= line.second)
				return lineNone;
			else if (pos - line.second > kAdsorbSensitiveDistance)
				return lineNone;
		}
		else
		{
			if (pos >= line.second)
				return lineNone;
			else if (line.second - pos > kAdsorbSensitiveDistance)
				return lineNone;
		}
		return line;
	}

	QRect rc = QRect(m_ptPressed, pt).normalized();
	int lineLen = (isHoriz) ? rc.height() : rc.width();
	if ((QApplication::keyboardModifiers() & Qt::AltModifier) &&
		lineLen > 2 * kAdsorbSensitiveDistance)
	{
		QImage img = m_pixmapDesktop.toImage();

		QLine line;
		if (isHoriz)
		{
			if (pos == rc.left())
				line = QLine(rc.topLeft(), rc.bottomLeft());
			else if (pos == rc.right())
				line = QLine(rc.topRight(), rc.bottomRight());
		}
		else
		{
			if (pos == rc.top())
				line = QLine(rc.topLeft(), rc.topRight());
			else if (pos == rc.bottom())
				line = QLine(rc.bottomLeft(), rc.bottomRight());
		}

		int step;
		if (nDirection < 0)
			step = - qMin(kAdsorbSensitiveDistance, pos);
		else
			step = qMin(kAdsorbSensitiveDistance,
				(isHoriz ? img.width() : img.height()) - pos);

		QPair<bool, int> result = findLine(img, line, step);
		return result.first ? qMakePair(nDirection, result.second) : lineNone;
	}
	return lineNone;
}

QPoint KScreenGrabDialog::adsorbLine(const QPoint& pt, const QPoint& ptOld)
{
	if (pt.x() != ptOld.x())
		m_lineHoriz = adsorbLine(pt, true, pt.x() > ptOld.x() ? 1 : -1);
	if (pt.y() != ptOld.y())
		m_lineVert = adsorbLine(pt, false, pt.y() > ptOld.y() ? 1 : -1);

	QPoint ptNew = pt;
	if (m_lineHoriz.first)
		ptNew.setX(m_lineHoriz.second);
	if (m_lineVert.first)
		ptNew.setY(m_lineVert.second);
	return ptNew;
}
void KScreenGrabDialog::relocateToolBar()
{
	if (m_toolbar->isHidden())
	{
		return;
	}

		int nGapToolbar = dpiScaled(LOCAL_SCREEN_GRAB_GAP_SELECTION_TOOLBAR);
	int nGapBottom = qAbs(m_rcDesktop.bottom() - m_rcSelection.bottom()) + nGapToolbar;
	int nGapTop = qAbs(m_rcDesktop.top() - m_rcSelection.top()) + nGapToolbar;

	int h = m_toolbar->height();
	QPoint ptTarget;

	// adjust y
	bool bSelectionBottom = false;
		if (nGapBottom - nGapToolbar - dpiScaled(5) >
		m_toolbar->height() + m_toolbarSettings->height())
	{
		ptTarget.setY(m_rcSelection.bottom() + nGapToolbar);
		bSelectionBottom = true;
		// selection bottom
	}
	else if (nGapTop > m_toolbar->height())
	{
		ptTarget.setY(m_rcSelection.top() - m_toolbar->height() - nGapToolbar);
		// selection top
	}
	else
	{
		ptTarget.setY(1);
		// screen top
	}

	if (ptTarget.y() <= 0)
	{
		ptTarget.setY(1);
	}

	// adjust x
	if (m_rcSelection.width() >= m_toolbar->width())
	{
		ptTarget.setX(m_rcSelection.right() - m_toolbar->width() + 1);
	}
	else if (m_rcSelection.right() >= m_toolbar->width())
	{
		ptTarget.setX(m_rcSelection.right() - m_toolbar->width() + 1);
	}
	else
	{
		ptTarget.setX(2);
	}

	m_toolbar->move(ptTarget);

#ifdef Q_OS_MACOS
	if (m_tooleditbar)
		m_tooleditbar->move(ptTarget + QPoint(0, m_toolbar->height() + 4));
#endif

	if (m_fShowSetting)
	{
		QPoint ptSetting = ptTarget;
		int nGap = dpiScaled(4);
		int nOffset = dpiScaled(45);
		if (bSelectionBottom)
		{
			ptSetting.setY(ptTarget.y() + nOffset);
		}
		else if (ptTarget.y() < m_toolbar->height() + nGap / 2)
		{
			ptSetting.setY(ptTarget.y() + nOffset); // do nothing
		}
		else if (ptTarget.y() + m_toolbar->height() + m_toolbarSettings->height() > m_rcSelection.top() + nGap)
		{
			ptSetting.setY(ptTarget.y() - m_toolbar->height() - m_toolbarSettings->height() -  nGap + nOffset);
		}
		m_toolbarSettings->show();
		m_toolbarSettings->move(ptSetting);
	}
	else
	{
		m_toolbarSettings->hide();
	}
}

void KScreenGrabDialog::resetCursor( QPoint &pt )
{
	m_rcSelection &= m_rcDesktop;

	QCursor cursor;
	CursorType bitmapCursorType = cursorNone;


	switch (m_selectmode)
	{
	case smSniffingWindow:
	{
		if (m_isOriginPoint)
		{
			bitmapCursorType = cursorRestore;
			this->setCursor(Qt::ArrowCursor);
		}
		else
		{
			bitmapCursorType = cursorSelect;
			this->setCursor(Qt::ArrowCursor);
		}
		break;
	}

	case smDraging:
		if (m_isOriginPoint)
		{
			bitmapCursorType = cursorRestore;
			this->setCursor(Qt::ArrowCursor);
		}
		else
		{
			bitmapCursorType = cursorSelect;
			this->setCursor(Qt::ArrowCursor);
		}
		break;

	case smSelection:
		{
			if (m_fEditing)
			{
				// editing
				QPoint pos = pt;
				if (m_nShapeChangeItemID < 0 || m_nShapeChangeItemID >= m_listEdit.count())
					return;

				EditItem item = m_listEdit[m_nShapeChangeItemID];
				switch (item.editType)
				{
				case editRect:
					if (posInRect(*item.rcPaint, item.width, pos))
					{
						this->setCursor(Qt::OpenHandCursor);
					}
					else if (item.rcTopleft->contains(pos) || item.rcBottomRight->contains(pos))
					{
						cursor = m_cursorBitmapSizeFDiag;
						bitmapCursorType = cursorSizeFDiag;
					}
					else if (item.rcTopRight->contains(pos) || item.rcBottomLeft->contains(pos))
					{
						cursor = m_cursorBitmapSizeBDiag;
						bitmapCursorType = cursorSizeBDiag;
					}
					else if (item.rcLeftMid->contains(pos) || item.rcRightMid->contains(pos))
					{
						cursor = m_cursorBitmapSizeHor;
						bitmapCursorType = cursorSizeHor;
					}
					else if (item.rcTopMid->contains(pos) || item.rcBottomMid->contains(pos))
					{
						cursor = m_cursorBitmapSizeVer;
						bitmapCursorType = cursorSizeVer;
					}
					else
					{
						this->setCursor(Qt::ArrowCursor);
					}
					break;
				case editEllipse:
					if (posInEllipse(*item.rcPaint, item.width, pos))
					{
						this->setCursor(Qt::OpenHandCursor);
					}
					else if (item.rcTopleft->contains(pos) || item.rcBottomRight->contains(pos))
					{
						cursor = m_cursorBitmapSizeFDiag;
						bitmapCursorType = cursorSizeFDiag;
					}
					else if (item.rcTopRight->contains(pos) || item.rcBottomLeft->contains(pos))
					{
						cursor = m_cursorBitmapSizeBDiag;
						bitmapCursorType = cursorSizeBDiag;
					}
					else if (item.rcLeftMid->contains(pos) || item.rcRightMid->contains(pos))
					{
						cursor = m_cursorBitmapSizeHor;
						bitmapCursorType = cursorSizeHor;
					}
					else if (item.rcTopMid->contains(pos) || item.rcBottomMid->contains(pos))
					{
						cursor = m_cursorBitmapSizeVer;
						bitmapCursorType = cursorSizeVer;
					}
					else
					{
						this->setCursor(Qt::ArrowCursor);
					}
					break;
				case editArrow:
					if (arrowContaisPos(*item.ptStart, *item.ptEnd, item.width, pos))
					{
						this->setCursor(Qt::OpenHandCursor);
					}
					else if (item.rcBegin->contains(pos) || item.rcEnd->contains(pos))
					{
						this->setCursor(Qt::ArrowCursor);
					}
					else
					{
						this->setCursor(Qt::ArrowCursor);
					}
					break;
				case editBrush:
					if (lineContainsPos(item.points, item.nPoints, item.width, pos))
					{
						this->setCursor(Qt::OpenHandCursor);
					}
					else
					{
						this->setCursor(Qt::ArrowCursor);
					}
					break;
				case editText:
					if (item.rcText->contains(pos))
						this->setCursor(Qt::OpenHandCursor);
					else
						this->setCursor(Qt::ArrowCursor);

					break;
				default:
					break;
				}
			}
			else
			{
				// not editing
				HitTestResult hit = hitTest(pt);
				if (htSizeAll == hit)
				{
					this->setCursor(Qt::OpenHandCursor);
					bitmapCursorType = cursorSizeAll;
				}
				else if (htSizeLeft == hit || htSizeRight == hit)
				{
					cursor = m_cursorBitmapSizeHor;
					bitmapCursorType = cursorSizeHor;
				}
				else if (htSizeTop == hit || htSizeBottom == hit)
				{
					cursor = m_cursorBitmapSizeVer;
					bitmapCursorType = cursorSizeVer;
				}
				else if (htSizeTopLeft == hit || htSizeBottomRight == hit)
				{
					cursor = m_cursorBitmapSizeFDiag;
					bitmapCursorType = cursorSizeFDiag;
				}
				else if (htSizeTopRight == hit || htSizeBottomLeft == hit)
				{
					cursor = m_cursorBitmapSizeBDiag;
					bitmapCursorType = cursorSizeBDiag;
				}
				else if (!m_rcSelection.contains(pt, true))
				{
					// pt @ area OUT OF selection
					bitmapCursorType = cursorSelect;
					this->setCursor(Qt::ArrowCursor);
				}
			} // end of if (!m_fEditing) ...
		} // end of case smSelection
		break;
	default:
		break;
	}

	// setCursor only when cursor changed
	if (!m_fResizing)
	{
		if (cursor.shape() != m_cursorShape)
		{
			this->setCursor(cursor);
			m_cursorShape = cursor.shape();
		}
		else if (Qt::BitmapCursor == cursor.shape() &&
			m_cursorBitmapType != bitmapCursorType)
		{
			this->setCursor(cursor);
			m_cursorShape = cursor.shape();
			m_cursorBitmapType = bitmapCursorType;
		}
	}
}

KScreenGrabDialog::HitTestResult KScreenGrabDialog::hitTest( QPoint &pt )
{
	HitTestResult ht = htNone;

		QSize szResize = QSize(dpiScaled(4), dpiScaled(4));
	QPoint center = m_rcSelection.center();

	// resize region @ 4 corners
	if (QRect(m_rcSelection.topLeft(), szResize).contains(pt))
	{
		ht = htSizeTopLeft;
	}
	else if (QRect(m_rcSelection.bottomRight(), szResize).contains(pt))
	{
		ht = htSizeBottomRight;
	}
	else if (QRect(m_rcSelection.topRight(), szResize).contains(pt))
	{
		ht = htSizeTopRight;
	}
	else if (QRect(m_rcSelection.bottomLeft(), szResize).contains(pt))
	{
		ht = htSizeBottomLeft;
	}
	// resize region @ 4 edges
	else if (QRect(QPoint(m_rcSelection.left(), center.y()), szResize).contains(pt))
	{
		ht = htSizeLeft;
	}
	else if (QRect(QPoint(m_rcSelection.right(), center.y()), szResize).contains(pt))
	{
		ht = htSizeRight;
	}
	else if (QRect(QPoint(center.x(), m_rcSelection.top()), szResize).contains(pt))
	{
		ht = htSizeTop;
	}
	else if (QRect(QPoint(center.x(), m_rcSelection.bottom()), szResize).contains(pt))
	{
		ht = htSizeBottom;
	}
	//  4 edges
	else if (pt.x() == m_rcSelection.left() &&
		pt.y() > m_rcSelection.top() &&
		pt.y() < m_rcSelection.bottom())
	{
		ht = htSizeLeft;
	}
	else if (pt.x() == m_rcSelection.right() &&
		pt.y() > m_rcSelection.top() &&
		pt.y() < m_rcSelection.bottom())
	{
		ht = htSizeRight;
	}
	else if (pt.y() == m_rcSelection.top() &&
		pt.x() > m_rcSelection.left() &&
		pt.x() < m_rcSelection.right())
	{
		ht = htSizeTop;
	}
	else if (pt.y() == m_rcSelection.bottom() &&
		pt.x() > m_rcSelection.left() &&
		pt.x() < m_rcSelection.right())
	{
		ht = htSizeBottom;
	}
	else if (m_rcSelection.contains(pt, true))
	{
		ht = htSizeAll;
	}
	else
	{
		;
	}

	return ht;
}
void KScreenGrabDialog::setEditing()
{
	if (!m_fEditing)
	{
		pushUndoStack();
		m_fEditing = true;
	}
	else
	{
		if (m_listEdit.size() <= 1 &&
			!m_fShowSetting)
		{
			clearUndoStack();
			m_fEditing = false;
			m_editType = editNone;
			pushUndoStack();
		}
	}
}

void KScreenGrabDialog::onActionRect()
{
	if (m_Menu->isVisible())
	{
		hideMenu();
	}
	if (m_editType != editRect)
		clearEditState();

	saveText();
	resetTextEdit(QPoint());

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obRect);

	m_toolbtnRect->setChecked(m_toolbtnRect->isChecked());
	toggleAction(m_toolbtnRect);

	selectPenWidth(s_nPenWidthRect);

	m_fShowSetting = m_toolbtnRect->isChecked();

	setEditing();

	m_editType = editRect;

	m_colorPicker->setColor(m_colorRect);

	setupToolbarSetting(editRect);
	relocateToolBar();
}
void KScreenGrabDialog::onActionEllipse()
{
	if (m_Menu->isVisible())
	{
		hideMenu();
	}
	if (m_editType != editEllipse)
		clearEditState();

	saveText();
	resetTextEdit(QPoint());

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obEllipse);

	m_toolbtnEllipse->setChecked(m_toolbtnEllipse->isChecked());
	toggleAction(m_toolbtnEllipse);

	selectPenWidth(s_nPenWidthEllipse);

	m_fShowSetting = m_toolbtnEllipse->isChecked();

	setEditing();

	m_editType = editEllipse;

	m_colorPicker->setColor(m_colorEllipse);

	setupToolbarSetting(editEllipse);
	relocateToolBar();
}

void KScreenGrabDialog::onActionArrow()
{
	if (m_Menu->isVisible())
	{
		hideMenu();
	}
	if (m_editType != editArrow)
		clearEditState();

	saveText();
	resetTextEdit(QPoint());

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obArrow);

	m_toolbtnArrow->setChecked(m_toolbtnArrow->isChecked());
	toggleAction(m_toolbtnArrow);

	selectPenWidth(s_nArrowSize);

	m_fShowSetting = m_toolbtnArrow->isChecked();

	setEditing();

	m_editType = editArrow;

	m_colorPicker->setColor(m_colorArrow);

	setupToolbarSetting(editArrow);
	relocateToolBar();
}

void KScreenGrabDialog::onActionBrush()
{
	if (m_Menu->isVisible())
	{
		hideMenu();
	}
	if (m_editType != editBrush)
		clearEditState();

	saveText();
	resetTextEdit(QPoint());

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obBrush);

	m_toolbtnBrush->setChecked(m_toolbtnBrush->isChecked());
	toggleAction(m_toolbtnBrush);

	selectPenWidth(s_nPenWidthBrush);

	m_fShowSetting = m_toolbtnBrush->isChecked();

	setEditing();

	m_editType = editBrush;

	m_colorPicker->setColor(m_colorBrush);

	setupToolbarSetting(editBrush);
	relocateToolBar();

	m_nPtCurrent = 0;
	m_vecBrushPoint.clear();
}

QPixmap createMosaic(const QPixmap &originalPixmap, int mosaicSize = 10) {
    QPixmap mosaicPixmap(originalPixmap.size());
    mosaicPixmap.fill(Qt::white);
 
    QPainter painter(&mosaicPixmap);
    painter.setRenderHint(QPainter::Antialiasing, false);
 
    int imgWidth = originalPixmap.width();
    int imgHeight = originalPixmap.height();
 
    for (int y = 0; y < imgHeight; y += mosaicSize) {
        for (int x = 0; x < imgWidth; x += mosaicSize) {
            QRect rect(x, y, qMin(mosaicSize, imgWidth - x), qMin(mosaicSize, imgHeight - y));
            QColor color = originalPixmap.toImage().pixel(x, y);
 
            painter.fillRect(rect, color);
        }
    }
 
    return mosaicPixmap;
}

void KScreenGrabDialog::onActionMosaic()
{
    qDebug()<<"&&& onActionMosaic";
	if (m_Menu->isVisible())
	{
		hideMenu();
	}
	if (m_editType != editMosaic)
		clearEditState();

	saveText();
	resetTextEdit(QPoint());

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obMosaic);

	m_toolbtnMosaic->setChecked(m_toolbtnMosaic->isChecked());
	toggleAction(m_toolbtnMosaic);

	selectPenWidth(s_nPenWidthBrush);

	m_fShowSetting = m_toolbtnMosaic->isChecked();

	setEditing();
	m_editType = editMosaic;

	setupToolbarSetting(editMosaic);
	relocateToolBar();
	//根据当前配置生成一张马赛克备用图片
	m_pixmapMosaic = createMosaic(m_pixmapDesktop);
    QString strPixmapSniff = "/Users/mac/Downloads/temp/PixmapSniff.png";
    m_pixmapMosaic.save(strPixmapSniff);
	m_vecMosaicPoint.clear();
}
 
void KScreenGrabDialog::onActionText()
{
	if (m_Menu->isVisible())
	{
		hideMenu();
	}
	if (m_editType != editText)
		clearEditState();

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obText);

	m_toolbtnText->setChecked(m_toolbtnText->isChecked());
	toggleAction(m_toolbtnText);

	m_fShowSetting = m_toolbtnText->isChecked();

	setEditing();
	m_editType = editText;

	m_colorPicker->setColor(m_colorText);

	setupToolbarSetting(editText);
	relocateToolBar();
#ifdef Q_OS_MACOS
	closeMoreToolBar();
#endif
}

#ifdef Q_OS_MACOS
void KScreenGrabDialog::closeMoreToolBar()
{
	if (!m_tooleditbar)
		return;
	if (!m_tooleditbar->isHidden())
		m_tooleditbar->hide();
}
#endif

void KScreenGrabDialog::onActionUndo()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obUndo);
	actionUndo();
}
void KScreenGrabDialog::onActionSave()
{

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obSave);
	actionSave();
}
void KScreenGrabDialog::onActionDone()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obDone);
	actionDone();

}
void KScreenGrabDialog::onActionPin()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obPin);

	actionPin();
}

void KScreenGrabDialog::onActionCancel()
{
	m_exitType = exitCancel;
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obCancel);
	actionCanecl();
}
void KScreenGrabDialog::onActionReSelect()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obReselect);

	reset();
	sniffingWindow(QCursor::pos());
}

void KScreenGrabDialog::onActionToggleToolbar()
{
	hideMenu();

	saveText();
	resetTextEdit(QPoint());
	setToolBarVisible(!m_fToolbarToggleOn);

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obSwitchToolbar);
}

void KScreenGrabDialog::onActionOcr()
{
	bool bMenu = false;

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obOcr);

	actionCopy();
	reset();
	int nRet = bMenu ? KscrnGrabHelper::EC_Ocr + 100 : KscrnGrabHelper::EC_Ocr;
	done(nRet);
}
							   
void KScreenGrabDialog::onMoreEditClicked()
{
	if (!m_tooleditbar)
		return;
	if (m_toolbarSettings && m_toolbarSettings->isVisible())
		m_toolbarSettings->hide();
	if (m_tooleditbar->isHidden())
		m_tooleditbar->show();
	else
		m_tooleditbar->hide();
}

void KScreenGrabDialog::onActionEdit()
{
	adjustActionClickedInfo(obToolBar, obEdit);

	actionCopy();
	reset();
	done(KscrnGrabHelper::EC_Edit);
}

void KScreenGrabDialog::onActionEditClarity()
{
	adjustActionClickedInfo(obToolBar, obEdit);

	actionCopy();
	reset();
	done(KscrnGrabHelper::EC_EditClarity);
}

void KScreenGrabDialog::onActionEditRemove()
{
	adjustActionClickedInfo(obToolBar, obEdit);

	actionCopy();
	reset();
	done(KscrnGrabHelper::EC_EditRemove);
}

void KScreenGrabDialog::onActionEditMatting()
{
	adjustActionClickedInfo(obToolBar, obEdit);

	actionCopy();
	reset();
	done(KscrnGrabHelper::EC_EditMatting);
}

void KScreenGrabDialog::onActionTranslateImage()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obTranslateImage);

	actionCopy();
	reset();

	//这里先暂时写死 +0x100, 后续跟客户端版本发布再做调整
	done(QDialog::Accepted + 0x100);
}

void KScreenGrabDialog::onActionSave2Pdf()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obSave2Pdf);

	actionCopy();
	reset();
	done(KscrnGrabHelper::EC_Save2PDF);
}

void KScreenGrabDialog::onActionSetting()
{
	hideMenu();

	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obSetting);
}

void KScreenGrabDialog::onActionExportImage()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obExportImage);

	done(KscrnGrabHelper::EC_ExportImage);
}

void KScreenGrabDialog::onActionDrawRect()
{
	emit drawRectSignal();
	m_drawShape = dsRect;

	toggleDrawAction();

}
void KScreenGrabDialog::onActionDrawRoundedRect()
{
	emit drawRoundedSignal();
	if (!m_login)
	{
		emit loginSignal();
		return reject();
	}
	if (!m_upgradeVip) // 需要会员, 但是不是会员的情况
	{
		emit upgradeVipSignal(KscrnGrabHelper::Upgrade_RoundRect);
		return reject();
	}
	m_drawShape = dsRoundedRect;
	toggleDrawAction();

}

void KScreenGrabDialog::onActionDrawEllipse()
{
	emit drawEllipseSignal();
	m_drawShape = dsEllipse;
	toggleDrawAction();

}

void KScreenGrabDialog::onActionDrawPolygon()
{
	emit drawPolygonSignal();
	if (!m_login)
	{
		emit loginSignal();
		return reject();
	}
	if (!m_upgradeVip) // 需要会员, 但是不是会员的情况
	{
		emit upgradeVipSignal(KscrnGrabHelper::Upgrade_Polygon);
		return reject();
	}
	m_drawShape = dsPolygon;
	toggleDrawAction();

}

void KScreenGrabDialog::toggleDrawAction()
{
	if (m_toolbarHeader)
	{
		QAbstractButton* widgets[] = {
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRect)),
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRoundedRect)),
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawEllipse)),
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawPolygon)),
		};

		for (int i = 0; i < sizeof(widgets) / sizeof(QAbstractButton*); ++i)
		{
			if (widgets[i])
				widgets[i]->setChecked(false);
		}

		widgets[m_drawShape]->setChecked(true);
	}
	onDrawShapeChanged();
}

void KScreenGrabDialog::onDrawShapeChanged()
{
	if (m_drawShape != dsRect)
	{
		m_rcSelection = QRect();
		m_polySelection.clear();
		m_polySelectionBackup.clear();
	}
	update();

	const QString STR_GRAB_SCREEN = "GrabScreen";
	const QString STR_GRAB_SCREEN_SHAPE_USED = "GrabScreen_ShapeUsed";
	Settings s;
	s.beginGroup(STR_GRAB_SCREEN);
	int used = s.value(STR_GRAB_SCREEN_SHAPE_USED).toInt();
	if (used & (1 << m_drawShape))
	{
		// 用过这个形状, 什么也不做
	}
	else
	{
		// 显示对应的tooltip
		QAbstractButton* widgets[] = {
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRect)),
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawRoundedRect)),
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawEllipse)),
			qobject_cast<QAbstractButton*>(m_toolbarHeader->widgetForAction(m_actionDrawPolygon)),
		};
		QHelpEvent e(QEvent::Enter, QPoint(), QPoint());
		if (widgets[m_drawShape])
			m_toolbarHeader->eventFilter(widgets[m_drawShape], &e);

		used |= (1 << m_drawShape);
		s.setValue(STR_GRAB_SCREEN_SHAPE_USED, used);
	}
}

void KScreenGrabDialog::onFinished(int)
{
	sendInfoColl();
}




void KScreenGrabDialog::setPaintRect( const QPoint& pt )
{
	m_rcPaint = QRect(m_ptPressed, pt).normalized();

	m_rcPaint.setTop(qMax(m_rcPaint.top(), m_rcSelection.top()));
	m_rcPaint.setLeft(qMax(m_rcPaint.left(), m_rcSelection.left()));
	m_rcPaint.setBottom(qMin(m_rcPaint.bottom(), m_rcSelection.bottom()));
	m_rcPaint.setRight(qMin(m_rcPaint.right(), m_rcSelection.right()));
}

void KScreenGrabDialog::setupPainter( QPainter& painter )
{
	QPen pen;
	QBrush brush;

	switch (m_editType)
	{
	case editNone:
		// do nothing
		break;
	case editRect:
		pen.setWidth(s_nPenWidthRect);
		pen.setColor(m_colorRect);
		pen.setJoinStyle(Qt::MiterJoin);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setRenderHint(QPainter::Antialiasing, false);
		break;
	case editEllipse:
		pen.setWidth(s_nPenWidthEllipse);
		pen.setColor(m_colorEllipse);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setRenderHint(QPainter::Antialiasing, true);
		break;
	case editArrow:
		painter.setPen(Qt::NoPen);
		painter.setBrush(m_colorArrow);
		painter.setRenderHint(QPainter::Antialiasing, true);
		break;
	case editBrush:
		pen.setWidth(s_nPenWidthBrush);
		pen.setColor(m_colorBrush);
		pen.setJoinStyle(Qt::RoundJoin);
		pen.setCapStyle(Qt::RoundCap);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		painter.setRenderHint(QPainter::Antialiasing, true);
		break;
	case editMosaic:
		painter.setPen(Qt::NoPen);
		painter.setRenderHint(QPainter::Antialiasing, true);
		break;
	case editText:
		// do nothing
		break;
	case editFilter:
		// do nothing
		break;
	default:
		break;
	}
}

void KScreenGrabDialog::setArrow( const QPoint& pt )
{
	m_ptArrowStart = m_ptPressed;
	m_ptArrowEnd = pt;
	if (pt.x() < m_rcSelection.left())
	{
		m_ptArrowEnd.setX(m_rcSelection.left());
	}
	else if (pt.x() > m_rcSelection.right())
	{
		m_ptArrowEnd.setX(m_rcSelection.right());
	}
	else
	{
		m_ptArrowEnd.setX(pt.x());
	}

	if (pt.y() < m_rcSelection.top())
	{
		m_ptArrowEnd.setY(m_rcSelection.top());
	}
	else if (pt.y() > m_rcSelection.bottom())
	{
		m_ptArrowEnd.setY(m_rcSelection.bottom());
	}
	else
	{
		m_ptArrowEnd.setY(pt.y());
	}

	if (m_ptArrowStart.x() <= 0 &&
		m_ptArrowStart.y() <= 0)
	{
		m_ptPressed = m_ptArrowStart;
		m_ptArrowStart.setX(1);
		m_ptArrowStart.setY(1);
	}

	if (m_ptArrowEnd.x() <= 0 &&
		m_ptArrowEnd.y() <= 0)
	{
		m_ptArrowEnd.setX(1);
		m_ptArrowEnd.setY(1);
	}
}

void KScreenGrabDialog::drawArrow( QPainter& painter, int nSize, QPoint& ptStart, QPoint& ptEnd )
{
	if (!ptStart.isNull() &&
		!ptEnd.isNull())
	{
		QLineF lineArrow(ptStart, ptEnd);
		qreal rArrowLength = lineArrow.length();
		qreal rArrowHeadLength = 0;
		qreal rArrowHeadWidth = 0;
		qreal rArrowSideWidth = 0;

		switch (nSize)
		{
		case penWidthSmall:
			rArrowHeadLength = 10;
			rArrowHeadWidth = 10;
			rArrowSideWidth = 3;
			break;
		case penWidthMiddle:
			rArrowHeadLength = 16;
			rArrowHeadWidth = 16;
			rArrowSideWidth = 4;
			break;
		case penWidthLarge:
			rArrowHeadLength = 20;
			rArrowHeadWidth = 20;
			rArrowSideWidth = 5;
			break;
		default:
			break;
		}

		QPointF pt = lineArrow.pointAt(
			(rArrowLength - rArrowHeadLength) / rArrowLength);
		QLineF lineArrowHead(pt, lineArrow.p2());
		lineArrowHead.setAngle(lineArrow.angle() + 90);

		QPointF ptMid = lineArrowHead.pointAt(0.5);
		lineArrowHead.translate(pt - ptMid);

		QPointF pt1 = lineArrowHead.pointAt(0);
		QPointF pt2 = lineArrowHead.pointAt(1);

		QPointF pts[3] = {ptEnd, pt1, pt2};
		painter.drawPolygon(pts, 3);


		pt1 = lineArrowHead.pointAt(
			(rArrowHeadWidth - rArrowSideWidth) / rArrowHeadWidth);
		pt2 = lineArrowHead.pointAt(
			1 - (rArrowHeadWidth - rArrowSideWidth) / rArrowHeadWidth);
		QPointF ptOffset = lineArrow.pointAt(
			(rArrowLength - rArrowHeadLength + 2) / rArrowLength);
		QLineF lineSide(pt1, pt2);
		ptMid = lineSide.pointAt(0.5);
		lineSide.translate(ptOffset - ptMid);

		pts[0] = ptStart;
		pts[1] = lineSide.p1();
		pts[2] = lineSide.p2();
		painter.drawPolygon(pts, 3);
	}
}

void KScreenGrabDialog::drawBrush( QPainter& painter, QPoint* pts, int nPts )
{
	if (NULL == pts)
	{
		return;
	}

	painter.setClipping(true);
	painter.setClipRegion(QRegion(m_rcSelection));

	painter.drawPolyline(pts, nPts);
}

void KScreenGrabDialog::drawEditing( QPainter& painter )
{
	painter.save();
	this->setupPainter(painter);

	// solve problem: the edges of drawing edit item overflow m_rcSelection
	painter.setClipping(true);
	painter.setClipRegion(QRegion(m_rcSelection));

	switch (m_editType)
	{
	case editNone:
		// do nothing
		break;
	case editRect:
		drawRectShape(painter, m_rcPaint, true);
		break;
	case editEllipse:
		drawEllipseShape(painter, m_rcPaint, true);
		break;
	case editArrow:
		drawArrowShape(painter, s_nArrowSize, m_ptArrowStart, m_ptArrowEnd, true);
		break;
	case editBrush:
		drawBrushMouseMove(painter);
		break;
	case editMosaic:
		drawMosaicMove(painter);
		break;
	case editText:
		drawTextAreaRect(painter);
		break;
	case editFilter:
		// do nothing
		break;
	default:
		// do nothing
		break;
	}
	painter.restore();
}

void KScreenGrabDialog::drawTextShape(QPainter& painter, QColor& color, QFont& font, QRect& rc, QString text, bool bEdit)
{
	drawText(painter, color, font, rc, text);
	if (bEdit)
	{
		if (m_itemDrawType == itemMove && !m_bTextDoubleClicked)
			drawTextEditRect(painter, rc);
	}
}

void KScreenGrabDialog::drawTextEditRect(QPainter& painter, const QRect& rect)
{
	QPen penBlack(QPen(m_colorText, 0.5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

	painter.setBrush(Qt::NoBrush);
	painter.setPen(penBlack);
	QRect rc = rect.adjusted(-4, -5, -4, -5);

	painter.drawRect(rc);
}

void KScreenGrabDialog::updateEditItemText()
{
	if (m_nShapeChangeItemID >= 0 &&
			m_nShapeChangeItemID < m_listEdit.count())
	{
		this->setFocus();
		m_strText = m_textEdit->toPlainText();

		QString strPreText = *m_listEdit[m_nShapeChangeItemID].text;
		QRect rectPre = *m_listEdit[m_nShapeChangeItemID].rcText;
		if (strPreText != m_strText)
		{
			QRect rect = m_rcTextEdit.adjusted(4, 5, 4, 5);
			m_listEdit[m_nShapeChangeItemID].rcText->setTopLeft(rect.topLeft());
			m_listEdit[m_nShapeChangeItemID].rcText->setBottomRight(rect.bottomRight());

			if (m_listEdit[m_nShapeChangeItemID].text)
			{
				delete m_listEdit[m_nShapeChangeItemID].text;
				m_listEdit[m_nShapeChangeItemID].text = nullptr;
				m_listEdit[m_nShapeChangeItemID].text  = new QString(m_strText);
			}
			m_strTextEditBackup = strPreText;
			m_textEditBackup = rectPre;
			m_itemOperation = optTextEdit;
			pushOperation();
		}
		m_textEdit->setGeometry(-100, -100, 30, 20);
		m_rcTextEdit = m_textEdit->geometry();
		m_rcTextEditBackup = m_rcTextEdit;
		m_textEdit->hide();
		hideTextEditMaskWidget();
		m_textEdit->setDisabled(true);
		m_nEditWidth = 30;
		m_nEditHeight = 20;
	}
}

void KScreenGrabDialog::drawRectShape(QPainter& painter, QRect& rc, bool bEdit)
{
	painter.setRenderHint(QPainter::Antialiasing, true);
	if (rc.isValid())
        painter.drawRoundedRect(rc, 2, 2);

	if (bEdit)
		drawResizeCircle(painter, rc);
}

void KScreenGrabDialog::drawEllipseShape(QPainter& painter, QRect& rc, bool bEdit)
{
	if (rc.isValid())
		painter.drawEllipse(rc);

	if (bEdit)
		drawResizeCircle(painter, rc);
}

void KScreenGrabDialog::drawArrowShape(QPainter& painter, int nSize, QPoint& ptStart, QPoint& ptEnd, bool bEdit)
{
	drawArrow(painter, nSize, ptStart, ptEnd);

	if (bEdit)
		drawResizeCircle(painter, ptStart, ptEnd);
}

void KScreenGrabDialog::drawBrushMouseMove(QPainter& painter)
{
	if (m_vecBrushPoint.count() <= 1)
		return;

	painter.setClipping(true);
	painter.setClipRegion(QRegion(m_rcSelection));

	painter.drawPolyline(m_vecBrushPoint);
}

void KScreenGrabDialog::drawMosaicMove(QPainter& painter)
{
    if (m_vecMosaicPoint.isEmpty()) {
        return;
    }

    // painter.save();
    // painter.setPen(Qt::NoPen);
    // for (auto m : m_vecMosaicPoint) {
    //     painter.setBrush(std::get<1>(m));
    //     painter.drawRect(std::get<0>(m));
    // }
    // painter.restore();

    QPainterPath painterpath;

    QPoint prePoint = m_vecMosaicPoint[0];
    for (auto m : m_vecMosaicPoint) {
        if (m == prePoint)
            continue;
        painterpath.cubicTo(prePoint, m, m);
        prePoint = m;
    }
    painter.setClipping(true);
    QPainterPathStroker pathStroker;
    pathStroker.setWidth(5);
    QPainterPath strokPath = pathStroker.createStroke(painterpath);
    painter.setClipPath(strokPath);
    painter.drawPixmap(0, 0, m_pixmapMosaic);
    painter.setClipping(false);
}

void KScreenGrabDialog::drawBrushShape(QPainter& painter, QPoint* pts, int nPts, bool bEdit)
{
	drawBrush(painter, pts, nPts);
	if (bEdit)
	{
		int nSize = dpiScaled(6);
		QPoint p1 = pts[0];
		QPoint p2 = pts[nPts - 1];
		QRect rect1 = QRect(p1.x() - nSize / 2, p1.y() - nSize / 2, nSize, nSize);
		QRect rect2 = QRect(p2.x() - nSize / 2, p2.y() - nSize / 2, nSize, nSize);
		painter.setPen(m_colorBrush);
		painter.setBrush(Qt::white);
		painter.drawEllipse(rect1);
		painter.drawEllipse(rect2);
	}
}

void KScreenGrabDialog::pushUndoStack()
{
	bool fPush = true;
	EditItem item;
	memset(&item, 0, sizeof(item));
	item.editType = m_editType;
	item.drawType = itemInsert;
	switch (item.editType)
	{
	case editNone:
		break;
	case editRect:
		if (m_rcPaint.isEmpty())
		{
			fPush = false;
		}
		else
		{
			item.width = s_nPenWidthRect;
			item.color = m_colorRect;
			item.rcPaint  = new QRect(m_rcPaint);
			item.rcTopleft = new QRect(m_rcTopleft);
			item.rcTopRight = new QRect(m_rcTopRight);
			item.rcTopMid = new QRect(m_rcTopMid);
			item.rcLeftMid = new QRect(m_rcLeftMid);
			item.rcRightMid = new QRect(m_rcRightMid);
			item.rcBottomLeft = new QRect(m_rcBottomLeft);
			item.rcBottomMid = new QRect(m_rcBottomMid);
			item.rcBottomRight = new QRect(m_rcBottomRight);
		}
		break;
	case editEllipse:
		if (m_rcPaint.isEmpty())
		{
			fPush = false;
		}
		else
		{
			item.width = s_nPenWidthEllipse;
			item.color = m_colorEllipse;
			item.rcPaint  = new QRect(m_rcPaint);
			item.rcTopleft = new QRect(m_rcTopleft);
			item.rcTopRight = new QRect(m_rcTopRight);
			item.rcTopMid = new QRect(m_rcTopMid);
			item.rcLeftMid = new QRect(m_rcLeftMid);
			item.rcRightMid = new QRect(m_rcRightMid);
			item.rcBottomLeft = new QRect(m_rcBottomLeft);
			item.rcBottomMid = new QRect(m_rcBottomMid);
			item.rcBottomRight = new QRect(m_rcBottomRight);
		}
		break;
	case editArrow:
		if (m_ptArrowStart == m_ptArrowEnd)
		{
			fPush = false;
		}
		else
		{
			item.width = s_nArrowSize;
			item.color = m_colorArrow;
			item.ptStart = new QPoint(m_ptArrowStart);
			item.ptEnd = new QPoint(m_ptArrowEnd);
			item.rcBegin = new QRect(m_rcBegin);
			item.rcEnd = new QRect(m_rcEnd);
		}
		break;
	case editBrush:
		if (1 >= m_nPtCurrent)
		{
			fPush = false;
		}
		else
		{
			int nPtNum = m_vecBrushPoint.count();
			if (nPtNum > 1)
			{
				item.width = s_nPenWidthBrush;
				item.color = m_colorBrush;
				item.points = new QPoint[nPtNum];
				item.nPoints = m_nPtCurrent;
				if (NULL != item.points)
				{
//					memcpy(item.points, m_pointsBrush, sizeof(QPoint) * m_nPtCurrent);
					for (int var = 0; var < nPtNum; ++var)
					{
						item.points[var].setX(m_vecBrushPoint.at(var).x());
						item.points[var].setY(m_vecBrushPoint.at(var).y());
					}
				}
				else
				{
					fPush = false;
				}
			}
			else
			{
				fPush = false;
			}
		}
		break;
	case editText:
		if (m_strText.isEmpty())
		{
			fPush = false;
		}
		else
		{
			item.color = m_colorText;
			item.fontFamily = new QString(m_strTextFontFamily);
			item.fontSize = m_nTextFontSize;
			item.rcText = new QRect(m_rcTextEdit.adjusted(4, 5, 4, 5));
			item.text = new QString(m_strText);
			item.drawType = itemNone;
			if (NULL == item.fontFamily ||
				NULL == item.text)
			{
				fPush = false;
			}
			m_strText = QString();
		}
		break;
	case editFilter:
		break;
	default:
		break;
	}
	if (fPush)
	{
		m_listEdit.push_back(item);
	}

	m_rcPaint = QRect();
	m_ptArrowStart = QPoint();
	m_ptArrowEnd = QPoint();
	m_nPtCurrent = 0;
	m_vecBrushPoint.clear();
	m_vecMosaicPoint.clear();
	m_ptPressed = ptNullPressedPoint;
	m_nShapeChangeItemID = m_listEdit.count() - 1;
}

void KScreenGrabDialog::popUndoStack()
{
	if (m_listEdit.isEmpty())
	{
		return;
	}

	EditItem item = m_listEdit.takeLast();

	switch (item.editType)
	{
	case editRect:
	case editEllipse:
		if (NULL != item.rcPaint)
		{
			delete item.rcPaint;
			item.rcPaint = NULL;
		}
		if (item.rcTopleft)
		{
			delete item.rcTopleft;
			item.rcTopleft = NULL;
		}
		if (item.rcTopMid)
		{
			delete item.rcTopMid;
			item.rcTopMid = NULL;
		}
		if (item.rcTopRight)
		{
			delete item.rcTopRight;
			item.rcTopRight = NULL;
		}
		if (item.rcLeftMid)
		{
			delete item.rcLeftMid;
			item.rcLeftMid = NULL;
		}
		if (item.rcRightMid)
		{
			delete item.rcRightMid;
			item.rcRightMid = NULL;
		}
		if (item.rcBottomLeft)
		{
			delete item.rcBottomLeft;
			item.rcBottomLeft = NULL;
		}
		if (item.rcBottomMid)
		{
			delete item.rcBottomMid;
			item.rcBottomMid = NULL;
		}
		if (item.rcBottomRight)
		{
			delete item.rcBottomRight;
			item.rcBottomRight = NULL;
		}
		break;
	case editArrow:
		if (NULL != item.ptStart)
		{
			delete item.ptStart;
			item.ptStart = NULL;
		}
		if (NULL != item.ptEnd)
		{
			delete item.ptEnd;
			item.ptEnd = NULL;
		}
		if (NULL != item.rcBegin)
		{
			delete item.rcBegin;
			item.rcBegin = NULL;
		}
		if (NULL != item.rcEnd)
		{
			delete item.rcEnd;
			item.rcEnd = NULL;
		}
		break;
	case editBrush:
		if (NULL != item.points)
		{
			delete[] item.points;
			item.points = NULL;
		}
		break;
	case editText:
		if (NULL != item.fontFamily)
		{
			delete item.fontFamily;
			item.fontFamily = NULL;
		}
		if (NULL != item.rcText)
		{
			delete item.rcText;
			item.rcText = NULL;
		}
		if (NULL != item.text)
		{
			delete item.text;
			item.text = NULL;
		}
		break;
	}
}

void KScreenGrabDialog::clearUndoStack()
{
	while (!m_listEdit.isEmpty())
	{
		popUndoStack();
	}
	m_stackDelete.clear();
}

void KScreenGrabDialog::clearOperationStack()
{
	while (!m_listEdit.isEmpty())
	{
		popOperation(true);
	}
}

void KScreenGrabDialog::setBrush( const QPoint& pt )
{
	QPoint point = pt;

	if (point.x() < m_rcSelection.left())
	{
		point.setX(m_rcSelection.left());
	}
	else if (point.x() > m_rcSelection.right())
	{
		point.setX(m_rcSelection.right());
	}

	if (point.y() < m_rcSelection.top())
	{
		point.setY(m_rcSelection.top());
	}
	else if (point.y() > m_rcSelection.bottom())
	{
		point.setY(m_rcSelection.bottom());
	}

	m_vecBrushPoint.push_back(point);
	m_nPtCurrent++;
	m_bBrushHasDraw = true;
}

void KScreenGrabDialog::setMosaic( const QPoint& pt )
{
	QPoint point = pt;

	if (point.x() < m_rcSelection.left())
	{
		point.setX(m_rcSelection.left());
	}
	else if (point.x() > m_rcSelection.right())
	{
		point.setX(m_rcSelection.right());
	}

	if (point.y() < m_rcSelection.top())
	{
		point.setY(m_rcSelection.top());
	}
	else if (point.y() > m_rcSelection.bottom())
	{
		point.setY(m_rcSelection.bottom());
	}

    m_vecMosaicPoint.push_back(point);

	m_nPtCurrent++;
}

void KScreenGrabDialog::showTextEditMaskWidget()
{
	bool bShow = m_rcDesktop.right() - m_rcSelection.right() > 1;
	if (m_widgetTextEditMask && bShow)
	{
		QRect rc  = QRect(m_rcSelection.right(),
						  m_rcSelection.top(), m_rcDesktop.right() - m_rcSelection.right(), m_rcDesktop.height());
        QPixmap pix = QApplication::primaryScreen()->grabWindow(this->winId(), m_rcSelection.right(),
                                                  m_rcSelection.top(), m_rcDesktop.right() - m_rcSelection.right(), m_rcDesktop.height());
		m_widgetTextEditMask->setGeometry(rc);
		m_widgetTextEditMask->setAutoFillBackground(true);
		QPalette pal = m_widgetTextEditMask->palette();
        pal.setBrush(QPalette::Window, QBrush(pix));
		m_widgetTextEditMask->setPalette(pal);
		m_widgetTextEditMask->show();
		m_widgetTextEditMask->raise();
	}
}

void KScreenGrabDialog::hideTextEditMaskWidget()
{
	if (m_widgetTextEditMask)
		m_widgetTextEditMask->hide();
}

void KScreenGrabDialog::resetTextEdit(const QPoint& pt)
{
	if (NULL == m_textEdit)
	{
		return;
	}

	m_rcTextEdit = QRect();
	m_rcTextEditBackup = QRect();
	m_fMoving = false;

	m_textEdit->clear();
	m_strText.clear();

	if (pt.isNull() || m_textEdit->isVisible())
	{
		m_textEdit->setGeometry(-100, -100, 30, 20);
		m_rcTextEdit = m_textEdit->geometry();
		m_rcTextEditBackup = m_rcTextEdit;
		m_textEdit->hide();
		hideTextEditMaskWidget();
		m_textEdit->setDisabled(true);
		m_nEditWidth = 30;
		m_nEditHeight = 20;
		m_bTextHasFocus = false;
		m_bTextEditHasSelected = false;
		m_bTextDoubleClicked = false;
	}
	else if (!pt.isNull())
	{
				QRect rcEdit(pt, QSize(dpiScaled(30), dpiScaled(20)));

		rcEdit.moveTop(qMax(m_rcSelection.top() + 1, rcEdit.top()));
		rcEdit.moveLeft(qMax(m_rcSelection.left() + 1, rcEdit.left()));
		rcEdit.moveBottom(qMin(m_rcSelection.bottom() - 1, rcEdit.bottom()));
		rcEdit.moveRight(qMin(m_rcSelection.right() - 1, rcEdit.right()));

		m_textEdit->setGeometry(rcEdit);
		m_textEdit->setMaximumHeight(m_rcSelection.bottom() - rcEdit.top() - 1);
		m_textEdit->setMaximumWidth(m_rcSelection.right() - rcEdit.left() - 1);
				m_textEdit->setMinimumHeight(dpiScaled(20));
				m_textEdit->setMinimumWidth(dpiScaled(30));

		m_rcTextEdit = rcEdit;
		m_textEdit->show();

		showTextEditMaskWidget();
		m_textEdit->setEnabled(true);
		m_textEdit->setFocus();
		m_textEdit->ensureCursorVisible();

		m_nEditWidth = m_rcTextEdit.width();
		m_nEditHeight = m_rcTextEdit.height();
	}

	QPalette pl = m_textEdit->palette();
	pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
	m_textEdit->setPalette(pl);
	m_textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_textEdit->setWordWrapMode(QTextOption::ManualWrap);

	setTextFontColor(m_textEdit, m_strTextFontFamily, m_nTextFontSize, m_colorText);

	m_textEdit->setFrameShape(QFrame::NoFrame);
	m_bTextHasFocus = true;
}

void KScreenGrabDialog::moveTextEditRect( const QPoint& pt )
{
	QPoint offset = pt - m_ptPressed;

	m_rcTextEdit = m_rcTextEditBackup.adjusted(
		offset.x(), offset.y(), offset.x(), offset.y());

	m_rcTextEdit.moveTop(qMax(m_rcSelection.top(), m_rcTextEdit.top()) + 2);
	m_rcTextEdit.moveLeft(qMax(m_rcSelection.left(), m_rcTextEdit.left()) + 2);
	m_rcTextEdit.moveBottom(qMin(m_rcSelection.bottom(), m_rcTextEdit.bottom()) - 1);
	m_rcTextEdit.moveRight(qMin(m_rcSelection.right(), m_rcTextEdit.right()) - 1);

	m_textEdit->move(m_rcTextEdit.topLeft() + QPoint(1, 1));
}

void KScreenGrabDialog::textChanged()
{
	QTextEdit* edit = qobject_cast<QTextEdit*>(sender());

	if (edit)
	{
		if (edit->toPlainText().isEmpty())
		{
			setTextFontColor(edit, m_strTextFontFamily, m_nTextFontSize, m_colorText);
		}

		QTextDocument* document = edit->document();

		if (document)
		{
			document->adjustSize();

			int height = document->size().height();
			int width = document->size().width();

			QFont font = document->defaultFont();
			QFontMetrics fontmetrics(font);
			int mh = fontmetrics.height();
			height += mh - 15;
			width += mh - 15;

			m_nEditHeight = height;
			m_nEditWidth = width;

			document->setTextWidth(m_nEditWidth);
			document->adjustSize();
			edit->setDocument(document);

			QRect rcEdit = m_rcTextEdit;

			rcEdit.setHeight(m_nEditHeight);
			rcEdit.setWidth(m_nEditWidth);

			edit->setGeometry(rcEdit.adjusted(0, 1, 0, 0));
			edit->setFixedHeight(rcEdit.height());
			edit->setFixedWidth(rcEdit.width());

			m_rcTextEdit = rcEdit;

			update();
		}
	}
}

bool KScreenGrabDialog::isPtAtRectEdges( const QRect& rc, const QPoint& pt )
{
	QPoint pt1 = pt + QPoint(0, 1);
	QPoint pt2 = pt + QPoint(0, -1);
	QPoint pt3 = pt + QPoint(1, 0);
	QPoint pt4 = pt + QPoint(-1, 0);

	if (!rc.contains(pt, true) && rc.contains(pt, false))
	{
		return true;
	}
	else if (!rc.contains(pt1, true) &&	rc.contains(pt1, false))
	{
		return true;
	}
	else if (!rc.contains(pt2, true) &&	rc.contains(pt2, false))
	{
		return true;
	}
	else if (!rc.contains(pt3, true) &&	rc.contains(pt3, false))
	{
		return true;
	}
	else if (!rc.contains(pt4, true) &&	rc.contains(pt4, false))
	{
		return true;
	}

	return false;
}

void KScreenGrabDialog::drawTextAreaRect(QPainter& painter )
{
	if (m_textEdit->isVisible())
	{
		QPen penBlack(QPen(m_colorText, 0.5, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

		painter.setBrush(Qt::NoBrush);
		painter.setPen(penBlack);
		painter.drawRect(m_rcTextEdit);
	}
}
void KScreenGrabDialog::drawText( QPainter& painter, QColor& color, QFont& font,
	QRect& rc, QString text )
{
	QPen pen;
	pen.setColor(color);
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.setFont(font);

	painter.setRenderHint(QPainter::Antialiasing, false);

	if (!text.isEmpty() &&
		!rc.isEmpty())
	{
		painter.drawText(rc, Qt::TextWrapAnywhere, text);
	}
}

void KScreenGrabDialog::clearEdit()
{
	m_textEdit->setText("");
	m_strText.clear();
	if (m_strText.isEmpty())
	{
		m_textEdit->setGeometry(-100, -100, 30, 20);
		m_rcTextEdit = m_textEdit->geometry();
		m_rcTextEditBackup = m_rcTextEdit;
		m_textEdit->hide();
		hideTextEditMaskWidget();
		m_textEdit->setDisabled(true);
		m_nEditWidth = 30;
		m_nEditHeight = 20;
	}

}

void KScreenGrabDialog::saveText()
{
	this->setFocus(); // 解决问题: TextEdit失去焦点之前, 不会将输入法的commitString输入到TextEdit

	m_strText = m_textEdit->toPlainText();
	if (!m_strText.isEmpty())
	{
		pushUndoStack();
		m_itemOperation = optAdd;
		pushOperation();
		update();
	}
	clearEdit();
	m_bTextHasFocus = false;
}

void KScreenGrabDialog::toggleAction( KScrnGrabToolButton* action)
{
	if (NULL == action)
	{
		return;
	}

	if (m_toolbtnRect != action)
	{
		m_toolbtnRect->setChecked(false);
	}

	if (m_toolbtnEllipse != action)
	{
		m_toolbtnEllipse->setChecked(false);
	}

	if (m_toolbtnArrow != action)
	{
		m_toolbtnArrow->setChecked(false);
	}

	if (m_toolbtnBrush != action)
	{
		m_toolbtnBrush->setChecked(false);
	}

	if (m_toolbtnMosaic != action)
	{
		m_toolbtnMosaic->setChecked(false);
	}

	if (m_toolbtnText != action)
	{
		m_toolbtnText->setChecked(false);
	}
}

void KScreenGrabDialog::colorChanged(const QColor& color)
{
	switch (m_editType)
	{
	case editNone:
		break;
	case editRect:
		m_colorRect = color;
		updateItemColor(m_colorRect);
		break;
	case editEllipse:
		m_colorEllipse = color;
		updateItemColor(m_colorEllipse);
		break;
	case editArrow:
		m_colorArrow = color;
		updateItemColor(m_colorArrow);
		break;
	case editBrush:
		m_colorBrush = color;
		updateItemColor(m_colorBrush);
		break;
	case editText:
		m_itemOperation = optColorChange;
		updateItemColor(color);
		m_colorText = color;
		setTextFontColor(m_textEdit, m_strTextFontFamily, m_nTextFontSize, m_colorText);
		break;
	case editFilter:
		// do nothing
		break;
	default:
		break;
	}
}
void KScreenGrabDialog::updateItemColor(const QColor& cl)
{
	if (m_editType == editText && m_itemOperation == optColorChange)
	{
		if (m_textEdit->isVisible())
		{
			m_colorBackup = m_colorText;
			return;
		}
	}

	if (m_nShapeChangeItemID >= 0 && m_nShapeChangeItemID < m_listEdit.count())
	{
		if (m_listEdit[m_nShapeChangeItemID].drawType != itemNone)
		{
			m_colorBackup = m_listEdit[m_nShapeChangeItemID].color;
			m_listEdit[m_nShapeChangeItemID].color = cl;
			m_itemOperation = optColorChange;
			pushOperation();
			update();
		}
	}
}

void KScreenGrabDialog::updateItemPenWidth(int nSize)
{
	if (m_nShapeChangeItemID >= 0 && m_nShapeChangeItemID < m_listEdit.count())
	{
		if (m_listEdit[m_nShapeChangeItemID].drawType != itemNone)
		{
			m_nPenWidthBackup = m_listEdit[m_nShapeChangeItemID].width;
			m_listEdit[m_nShapeChangeItemID].width = nSize;
			m_itemOperation = optPenWidthChange;
			pushOperation();
			update();
		}
	}
}

void KScreenGrabDialog::penSizeChanged( int size )
{
	switch (m_editType)
	{
	case editNone:
		break;
	case editRect:
		s_nPenWidthRect = size;
		updateItemPenWidth(s_nPenWidthRect);
		break;
	case editEllipse:
		s_nPenWidthEllipse = size;
		updateItemPenWidth(s_nPenWidthEllipse);
		break;
	case editArrow:
		s_nArrowSize = size;
		updateItemPenWidth(s_nArrowSize);
		break;
	case editBrush:
		s_nPenWidthBrush = size;
		updateItemPenWidth(s_nPenWidthBrush);
		break;
	case editMosaic:
		s_nPenWidthBrush = size;
		updateItemPenWidth(s_nPenWidthBrush);
		break;
	case editText:
		// do nothing
		break;
	case editFilter:
		// do nothing
		break;
	default:
		break;
	}

	selectPenWidth(size);
}


void KScreenGrabDialog::fontSizeChanged(int size)
{
	switch (m_editType)
	{
	case editNone:
		// do nothing
		break;
	case editRect:
		// do nothing
		break;
	case editEllipse:
		// do nothing
		break;
	case editArrow:
		// do nothing
		break;
	case editBrush:
		// do nothing
		break;
	case editMosaic:
		// do nothing
		break;
	case editText:
	{
		m_nTextFontSize = size;
		setTextFontColor(m_textEdit, m_strTextFontFamily, m_nTextFontSize, m_colorText);
		m_textEdit->setFocus();
		if (m_nShapeChangeItemID >= 0 && m_nShapeChangeItemID < m_listEdit.count())
		{
			if (m_listEdit[m_nShapeChangeItemID].drawType != itemNone)
			{
				m_nFontSizeBackup = m_listEdit[m_nShapeChangeItemID].fontSize;
				m_listEdit[m_nShapeChangeItemID].fontSize = m_nTextFontSize;
				m_itemOperation = optFontSizeChange;
				pushOperation();
				update();
			}
		}
	}
		break;
	case editFilter:
		// do nothing
		break;
	default:
		break;
	}
}

void KScreenGrabDialog::setupToolbarSetting(EditType editMode)
{
	switch (editMode)
	{
	case editNone:
		// do nothing
		break;
	case editRect:
		m_actionPenSmall->setVisible(true);
		m_actionPenMiddle->setVisible(true);
		m_actionPenLarge->setVisible(true);
		m_actionSeparator->setVisible(true);
		m_actionFont->setVisible(false);
		m_actionColor->setVisible(true);
		break;
	case editEllipse:
		m_actionPenSmall->setVisible(true);
		m_actionPenMiddle->setVisible(true);
		m_actionPenLarge->setVisible(true);
		m_actionSeparator->setVisible(true);
		m_actionFont->setVisible(false);
		m_actionColor->setVisible(true);
		break;
	case editArrow:
		m_actionPenSmall->setVisible(true);
		m_actionPenMiddle->setVisible(true);
		m_actionPenLarge->setVisible(true);
		m_actionSeparator->setVisible(true);
		m_actionFont->setVisible(false);
		m_actionColor->setVisible(true);
		break;
	case editBrush:
		m_actionPenSmall->setVisible(true);
		m_actionPenMiddle->setVisible(true);
		m_actionPenLarge->setVisible(true);
		m_actionSeparator->setVisible(true);
		m_actionFont->setVisible(false);
		m_actionColor->setVisible(true);
		break;
	case editMosaic:
		m_actionPenSmall->setVisible(true);
		m_actionPenMiddle->setVisible(true);
		m_actionPenLarge->setVisible(true);
		m_actionSeparator->setVisible(false);
		m_actionFont->setVisible(false);
		m_actionColor->setVisible(false);
		break;
	case editText:
		m_actionPenSmall->setVisible(false);
		m_actionPenMiddle->setVisible(false);
		m_actionPenLarge->setVisible(false);
		m_actionSeparator->setVisible(false);
		m_actionFont->setVisible(true);
		m_actionColor->setVisible(true);
		break;
	case editFilter:
		// do nothing
		break;
	default:
		break;
	}
}

void KScreenGrabDialog::selectPenWidth( int width )
{
	switch (width)
	{
	case penWidthSmall:
		m_penSmall->selected(true);
		m_penMiddle->selected(false);
		m_penLarge->selected(false);
		break;
	case penWidthMiddle:
		m_penSmall->selected(false);
		m_penMiddle->selected(true);
		m_penLarge->selected(false);
		break;
	case penWidthLarge:
		m_penSmall->selected(false);
		m_penMiddle->selected(false);
		m_penLarge->selected(true);
		break;
	default:
		break;
	}
}
void KScreenGrabDialog::onActionCopy()
{
	bool fromMenu = isTriggerFromMenu(qobject_cast<QAction*>(sender()));
	adjustActionClickedInfo(fromMenu ? obMenu : obToolBar, obCopy);

	actionCopy();
	done(KscrnGrabHelper::EC_Copy);
}
void KScreenGrabDialog::showMenu( QPoint pt )
{
	if (NULL != m_Menu && !pt.isNull())
	{
		m_Menu->resetHoveredAction();
		m_Menu->move(pt);
		m_Menu->show();
		m_Menu->raise();
		m_Menu->setFocus();

		adjustToolBarShowInfo(obMenu);
	}
}
void KScreenGrabDialog::hideMenu()
{
	if (NULL == m_Menu || !m_Menu->isVisible())
		return;

	m_Menu->resetHoveredAction();
	m_Menu->move(-1000, -1000);
	m_Menu->hide();

	this->setFocus();
}

void KScreenGrabDialog::showToolBar()
{
	if (m_toolbar)
	{
		m_toolbar->show();
		relocateToolBar();

		adjustToolBarShowInfo(obToolBar);
	}

	QString key = isSingleItem(KscrnGrabHelper::ACF_Ocr) ? "settinghint_ocr_shown_flag" : "settinghint_shown_flag";
	if (m_flags & KscrnGrabHelper::ACF_Setting && !isSettingHintShown(key))
	{
		int wHint = isSingleItem(KscrnGrabHelper::ACF_Ocr) ? dpiScaled(200) : dpiScaled(180);
		if (NULL == m_settingHint)
		{
			m_settingHint = new KScreenGrabHint(this, 3000);
			m_settingHint->setFixedWidth(wHint);
		}

		QString tips = tr("you can set screen grab global shortcut here");
		QPoint pt;
		if (m_toolbar->isVisible())
		{
			if (isSingleItem(KscrnGrabHelper::ACF_Ocr))
			{
				tips = tr("you can set screen grab ocr global shortcut here");
				pt = m_toolbar->geometry().bottomLeft();
				pt.setX(pt.rx() + dpiScaled(24) - m_settingHint->width() / 2);
				pt.setY(pt.ry() + dpiScaled(2));
			}
			else
			{
				pt = m_toolbar->geometry().bottomRight();
				pt.setX(pt.rx() - dpiScaled(178) - m_settingHint->width() / 2);
				pt.setY(pt.ry() + dpiScaled(2));
			}
		}
		else
		{
			pt = m_rcSelection.bottomLeft();
			pt += QPoint(0, dpiScaled(2));
		}

		if (pt.x() + m_settingHint->width() > m_rcSelection.right())
		{
			pt.setX(qMax(pt.x(), m_rcSelection.right() - m_settingHint->width()));
		}

		if (pt.x() + m_settingHint->width() > m_rcDesktop.right())
		{
			pt.setX(m_rcDesktop.right() - m_settingHint->width());
		}

		if (pt.y() + m_settingHint->height() > m_rcDesktop.bottom())
		{
			pt.setY(m_rcDesktop.bottom() - m_settingHint->height());
		}


		m_settingHint->setText(tips);
		m_settingHint->move(pt);

		m_settingHint->show();
		m_settingHint->raise();

		m_settingHint->display();

		setSettingHintShownFlag(key, true);
	}
}

void KScreenGrabDialog::hideToolBar()
{
	if(NULL != m_toolbar)
	{
		m_toolbar->hide();
#ifdef Q_OS_MACOS
		if (m_tooleditbar)
			m_tooleditbar->hide();
#endif
	}

	if(NULL != m_toolbarSettings)
		m_toolbarSettings->hide();
}

void KScreenGrabDialog::setToolBarVisible(bool bVisible)
{
	if (m_toolbar && m_toolbar->isVisible() == bVisible)
		return;

	QString text;
	if (bVisible)
	{
		text = tr("Hide Edit Tool");
		showToolBar();
	}
	else
	{
		text = tr("Show Toolbar");
		hideToolBar();
	}

	if (m_actionToggleToolbar)
		m_actionToggleToolbar->setText(text);

	m_fToolbarToggleOn = bVisible;
}

void KScreenGrabDialog::showToolBarHeader()
{
	if (m_toolbarHeader)
		m_toolbarHeader->show();
}

void KScreenGrabDialog::hideToolBarHeader()
{
	if (m_toolbarHeader)
		m_toolbarHeader->hide();
}

QString KScreenGrabDialog::pngPath(const QString& name)
{
	QString resPathStr = "://icons_png/";
	return resPathStr + name;
}
QString KScreenGrabDialog::resSvgPath(const QString& name, bool bOldPath)
{
	QString resPathStr = "://icons_svg/16x16/";
	// if (KCocoaCommon::applicationIsInDarkMode())
	// 	resPathStr = "://res/dark/";

	return resPathStr + name;
}
bool KScreenGrabDialog::isTriggerFromMenu(QAction *action)
{
	if (!action)
		return false;

	return action->property("fromMenu").toBool();
}
void KScreenGrabDialog::adjustActionClickedInfo(OperObjectType toolBarType, OperObjectType itemType)
{
	if (obMenu == toolBarType)
	{
		addObjOperTimes(m_lstMenuActClickedInfo, itemType);
	}
	else if (obToolBar == toolBarType)
	{
		addObjOperTimes(m_lstToolBarActClickedInfo, itemType);
	}
}
void KScreenGrabDialog::adjustToolBarShowInfo(OperObjectType toolBarType)
{
	addObjOperTimes(m_lstToolBarAndMenuShownInfo, toolBarType);

	QList<OperObjectType> lstItemType;
	if (m_flags & KscrnGrabHelper::ACF_ExportImage)
	{
		lstItemType.append(obExportImage);
	}

	if (m_flags & KscrnGrabHelper::ACF_Save2Pdf)
	{
		lstItemType.append(obSave2Pdf);
	}

	if (m_flags & KscrnGrabHelper::ACF_Ocr)
	{
		lstItemType.append(obOcr);
	}

#ifdef Q_OS_MACOS
	if (m_flags & KscrnGrabHelper::ACF_Ocr)
		lstItemType.append(obEdit);
#endif

	if (m_flags & KscrnGrabHelper::ACF_Pin)
	{
		lstItemType.append(obPin);
	}

	foreach(auto itemType, lstItemType)
	{
		if (obMenu == toolBarType)
		{
			addObjOperTimes(m_lstMenuActShownInfo, itemType);
		}
		else if (obToolBar == toolBarType)
		{
			addObjOperTimes(m_lstToolBarActShownInfo, itemType);
		}
	}
}

void KScreenGrabDialog::addObjOperTimes(QList<OperationInfo> &lstInfo, OperObjectType objType)
{
	bool bHasItem = false;
	foreach(auto item, lstInfo)
	{
		if (item.type != objType)
			continue;

		bHasItem = true;
		break;
	}

	if (!bHasItem)
	{
		OperationInfo info;
		info.type = objType;
		info.count = 0;

		lstInfo.append(info);
	}

	for (int i = 0; i < lstInfo.size(); i++)
	{
		OperationInfo &item = lstInfo[i];
		if (item.type != objType)
			continue;

		switch (item.type)
		{
		case obDone:
		case obOcr:
		case obEdit:
		case obExportImage:
		case obSave2Pdf:
			item.szSelection = QRect(m_rcSelection & m_rcDesktop).size();
			break;
		default:
			break;
		}

		item.count++;
	}
}

void KScreenGrabDialog::onActionDelete()
{
	if (!m_fInited)
	{
		return;
	}

	hideMenu();

	if (m_fInited && !m_fEditing)
	{
		reset();
		sniffingWindow(QCursor::pos());
	}
	else
	{
		if (m_textEdit->isVisible())
		{
			resetTextEdit(QPoint());
			return;
		}

		if (m_listEdit.isEmpty())
		{
			reset();
			sniffingWindow(QCursor::pos());
		}
		else
		{
			for (auto it = m_listEdit.begin(); it != m_listEdit.end(); it++)
			{
				EditItem item = *it;
				if (item.drawType != itemNone)
				{
					if (item.editType == editText)
					{
						resetTextEdit(QPoint());
					}
					m_itemOperation = optDel;
					pushOperation();
					m_stackDelete.push(item);
					m_listEdit.erase(it);
					break;
				}
			}
		}
	}
}

void KScreenGrabDialog::actionUndo()
{
	if (!m_fInited)
	{
		return;
	}

	hideMenu();

	if (m_fInited && !m_fEditing)
	{
		reset();
		sniffingWindow(QCursor::pos());
	}
	else
	{
		if (m_textEdit->isVisible())
		{
			resetTextEdit(QPoint());
			return;
		}

		if (m_listEdit.isEmpty())
		{
			reset();
			sniffingWindow(QCursor::pos());
		}
		else
		{
			if (!popOperation())
			{
				popUndoStack();
				if (!m_listEdit.isEmpty())
				{
					update();
				}
				else
				{
					reset();
					sniffingWindow(QCursor::pos());
				}
			}
		}
	}
}

void KScreenGrabDialog::actionSave()
{
	if (!m_fInited)
	{
		return;
	}

	clearEditState();
	hideMenu();

	saveText();
	resetTextEdit(QPoint());

	QPixmap pixmapDesktop = m_pixmapDesktop;

	int DestHight = m_rcDesktop.height();
	int PixmapDeskHight = m_pixmapDesktop.height();
	QRect rcSelectionTemp = QRect(m_rcSelection.topLeft()*(PixmapDeskHight/DestHight), m_rcSelection.bottomRight()*(PixmapDeskHight/DestHight));

	QImage img;
	if (m_bSniffWindowGrab && !m_pixmapSniff.isNull())
	{
		QPainter painter(&pixmapDesktop);
		painter.drawPixmap(m_rcSelection, m_pixmapSniff);
		drawEditHistory(painter);
		img = pixmapDesktop.copy(rcSelectionTemp).toImage();
	}
	else
	{
		QPainter painter(&pixmapDesktop);
		drawEditHistory(painter);
		switch (m_drawShape)
		{
		case dsRect:
			img = pixmapDesktop.copy(rcSelectionTemp).toImage();
			break;
		case dsEllipse:
		{
			img = drawEllipse(pixmapDesktop, rcSelectionTemp, false);
			break;
		}
		case dsRoundedRect:
		{
			img = drawRoundedRect(pixmapDesktop, rcSelectionTemp, roundedRadius, false);
			break;
		}
		case dsPolygon:
		{
			img = drawPolygon(pixmapDesktop, rcSelectionTemp, m_polySelection, false);
			break;
		}
		default:
			Q_ASSERT(false);
		}
	}

	KWindowUtil::setWindowTopLevel(this, false);
	QDateTime current = QDateTime::currentDateTime();
	QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
	QString strFilename = tr("Capture%1").arg(current.toString("yyyyMMddhhmmss"));
#ifdef Q_OS_UNIX
	strFilename.append(".png");
#endif
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
	{
		KWindowUtil::setWindowTopLevel(this);
		return;
	}

	QStringList filenameList = filename.split("/");
	if (filenameList.count() <= 2 || filenameList.at(1) == "Volumes")
	{
		KMessageBoxDialog messageDlg(this);
		messageDlg.setContent(tr("Invalid path, please select again."));
		messageDlg.exec();
		KWindowUtil::setWindowTopLevel(this);
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
		img.save(filename, szFilter, 80);
		actionCanecl();
	}
	else
	{
		KWindowUtil::setWindowTopLevel(this);
	}
}

void KScreenGrabDialog::actionCopy()
{
	if (!m_fInited)
	{
		return;
	}

	clearEditState();

	saveText();
	resetTextEdit(QPoint());

	hideMenu();
	int DestHight = m_rcDesktop.height();
	int PixmapDeskHight = m_pixmapDesktop.height();
	QRect rcSelectionTemp = QRect(m_rcSelection.topLeft()*(PixmapDeskHight/DestHight), m_rcSelection.bottomRight()*(PixmapDeskHight/DestHight));
	if (smSelection == m_selectmode &&
		!rcSelectionTemp.isEmpty())
	{
		QPixmap pixmapDesktop = m_pixmapDesktop;
		if (m_bSniffWindowGrab && !m_pixmapSniff.isNull())
		{
			QPainter painter(&pixmapDesktop);
			painter.drawPixmap(m_rcSelection, m_pixmapSniff);
			drawEditHistory(painter);
			m_selectionImage = pixmapDesktop.copy(rcSelectionTemp).toImage();
		}
		else
		{
			QPainter painter(&pixmapDesktop);
			drawEditHistory(painter);
			switch (m_drawShape)
			{
			case dsRect:
				m_selectionImage = pixmapDesktop.copy(rcSelectionTemp).toImage();
				break;
			case dsEllipse:
			{
				m_selectionImage = drawEllipse(pixmapDesktop, rcSelectionTemp, true);
				break;
			}
			case dsRoundedRect:
			{
				m_selectionImage = drawRoundedRect(pixmapDesktop, rcSelectionTemp, roundedRadius, true);
				break;
			}
			case dsPolygon:
			{
				for (int var = 0; var < m_polySelection.count(); ++var)
				{
					QPoint pt = m_polySelection.point(var);
					m_polySelection.setPoint(var, pt.x() *(PixmapDeskHight/DestHight), pt.y() *(PixmapDeskHight/DestHight));
				}
				m_selectionImage = drawPolygon(pixmapDesktop, rcSelectionTemp, m_polySelection, true);
				break;
			}
			default:
				Q_ASSERT(false);
			}
		}

		QClipboard* clipboard = QApplication::clipboard();
		if (NULL != clipboard)
		{
			QMimeData *data = new QMimeData;
			QByteArray byteArray;
			QBuffer buffer(&byteArray);
			buffer.open(QIODevice::WriteOnly);
			m_selectionImage.save(&buffer, "PNG", 80);
#ifdef Q_OS_WIN
			data->setData(QLatin1String("PNG"), byteArray);
#else
			data->setData(QLatin1String("image/png"), byteArray);
#endif
			data->setImageData(QVariant(m_selectionImage));
			clipboard->setMimeData(data);
			buffer.close();

			/*QPoint pt;
			if (m_toolbar->isVisible())
			{
				pt = m_toolbar->geometry().bottomRight();
				pt.setX(pt.rx() - dpiScaled(203) - m_hint->width()/2);
				pt.setY(pt.ry() + dpiScaled(2));


			}
			else
			{
				pt = m_rcSelection.bottomLeft();
				pt += QPoint(0, dpiScaled(2));
			}

			if (pt.x() + m_hint->width() > m_rcSelection.right())
			{
				pt.setX(qMax(pt.x(), m_rcSelection.right() - m_hint->width()));
			}

			if (pt.x() + m_hint->width() > m_rcDesktop.right())
			{
				pt.setX(m_rcDesktop.right() - m_hint->width());
			}

			if (pt.y() + m_hint->height() > m_rcDesktop.bottom())
			{
				pt.setY(m_rcDesktop.bottom() - m_hint->height());
			}
			m_hint->setText(tr("Successfully copied to clipboard!"));
			m_hint->move(pt);
			m_hint->show();
			m_hint->raise();

			m_hint->display();*/
		}
	}
#ifdef Q_OS_MACOS
	sendEditItemCollInfo();
#endif
}

void KScreenGrabDialog::actionCanecl()
{
	reset();
	this->setProperty(KScreenGrabHelper::Reason, KScreenGrabHelper::NoReason);
	reject();
}

void KScreenGrabDialog::actionDone()
{
	actionCopy();
	reset();
	accept();
}

void KScreenGrabDialog::actionPin()
{
	QClipboard* clipboard = QApplication::clipboard();
	if (clipboard)
	{
		actionCopy();
		KScreenGrabPin* pin = new KScreenGrabPin(m_drawShape);
		pin->setImage(clipboard->image());
		pin->show(m_rcSelection.adjusted(m_screenOffset.x(), m_screenOffset.y(), m_screenOffset.x(), m_screenOffset.y()));
		reset();
		this->setProperty(KScreenGrabHelper::Reason, KScreenGrabHelper::NoReason);
		reject();
	}
}

bool KScreenGrabDialog::isSingleItem(KscrnGrabHelper::ActionFlag flag)
{
	return m_bSingleItemOnly && m_flags & flag;
}
bool KScreenGrabDialog::isSettingHintShown(const QString& key)
{
	QSettings settings;
	settings.beginGroup("kscreengrab");

	return settings.value(key, false).toBool();
}

void KScreenGrabDialog::setSettingHintShownFlag(const QString& key, bool flag)
{
	QSettings settings;
	settings.beginGroup("kscreengrab");
	settings.setValue(key, flag);
}
QString KScreenGrabDialog::parseOperObjectType(OperObjectType type)
{
	QString text;

	switch (type)
	{
	case obRect:
		text = "rectangle";
		break;
	case obEllipse:
		text = "ellipse";
		break;
	case obArrow:
		text = "arrow";
		break;
	case obBrush:
		text = "brush";
		break;
	case obText:
		text = "text";
		break;
	case obReselect:
		text = "reselect";
		break;
	case obUndo:
		text = "undo";
		break;
	case obSetting:
		text = "shortcut_setting";
		break;
	case obCopy:
		text = "copy";
		break;
	case obSave:
		text = "save";
		break;
	case obSwitchToolbar:
		text = "switch_toolbar";
		break;
	case obCancel:
		text = "cancel";
		break;
	case obDone:
		text = "done";
		break;
	case obPin:
		text = "fix";
		break;
	case obExportImage:
		text = "export_image";
		break;
	case obSave2Pdf:
		text = "save_as_pdf";
		break;
	case obOcr:
		text = "character_capture";
		break;
	case obEdit:
		text = "picture_edit";
		break;
	case obToolBar:
		text = "toolbar";
		break;
	case obMenu:
		text = "menu";
		break;

	case obInvalid:
	default:
		break;
	}

	return text;
}
void KScreenGrabDialog::sendInfoColl()
{
	QHash<QString, QString> args;

	//toolbar&menubar shown
	QString toolbarType = m_bSingleItemOnly ? "singleton" : "complete";
	foreach(auto item, m_lstToolBarAndMenuShownInfo)
	{
		QString eventName;
		int actionCount = 0;
		switch (item.type)
		{
		case obMenu:
			actionCount = m_Menu ? m_Menu->actionCount() : 0;
			eventName = "kscreencapture_menu_show";
			break;
		case obToolBar:
			actionCount = m_toolBarActionCount;
			eventName = "kscreencapture_toolbar_show";
			break;
		default:
			break;
		}

		if (!eventName.isEmpty())
		{
			args.clear();
			args.insert("type", toolbarType);
			args.insert("item_count", QString::number(actionCount));
			args.insert("operation_count", QString::number(item.count));
		}
	}

	//toolbar action shown
	QString toolBarActEvent = "kscreencapture_toolbar_button";
	foreach(auto item, m_lstToolBarActShownInfo)
	{
		args.clear();
		args.insert("name", parseOperObjectType(item.type));
		args.insert("operation_count", QString::number(item.count));
		args.insert("action", "shown");

		if (item.szSelection.isValid())
			args.insert("selection_size", QString("%1*%2").arg(item.szSelection.width()).arg(item.szSelection.height()));
	}

	//toolbar action clicked
	foreach(auto item, m_lstToolBarActClickedInfo)
	{
		args.clear();
		args.insert("name", parseOperObjectType(item.type));
		args.insert("operation_count", QString::number(item.count));
		args.insert("action", "clicked");

		if (item.szSelection.isValid())
			args.insert("selection_size", QString("%1*%2").arg(item.szSelection.width()).arg(item.szSelection.height()));
	}

	//menu action shown
	QString menuActEvent = "kscreencapture_menu_button";
	foreach(auto item, m_lstMenuActShownInfo)
	{
		args.clear();
		args.insert("name", parseOperObjectType(item.type));
		args.insert("operation_count", QString::number(item.count));
		args.insert("action", "shown");
	}

	//menu action clicked
	foreach(auto item, m_lstMenuActClickedInfo)
	{
		args.clear();
		args.insert("name", parseOperObjectType(item.type));
		args.insert("operation_count", QString::number(item.count));
		args.insert("action", "clicked");
	}
}

QBitmap KScreenGrabDialog::drawMask(const QPixmap& src, const QRect& rc, std::function<void(QPainter&)> drawFunc, bool antialiasing)
{
    QBitmap mask(src.width(), src.height());
	QPainter painter(&mask);
	QPainter::RenderHints hints = QPainter::SmoothPixmapTransform;
	if (antialiasing)
        hints |= QPainter::Antialiasing;

	painter.setRenderHints(hints);
	painter.fillRect(0, 0, mask.width(), mask.height(), Qt::white);
	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::black);
	if (drawFunc)
		drawFunc(painter);
	return mask;
}

QImage KScreenGrabDialog::drawPath(const QPixmap& src, const QRect& rc, std::function<void(QPainter&)> drawFunc, bool antialiasing)
{
    QBitmap mask = drawMask(src, rc, drawFunc, antialiasing);
	QPixmap result = src;
	result.setMask(mask);
	return result.copy(rc).toImage();
}
QImage KScreenGrabDialog::drawEllipse(const QPixmap& src, const QRect& rc, bool antialiasing)
{
	return drawPath(src, rc, [rc](QPainter& painter) {
		painter.drawEllipse(rc);
	}, antialiasing);
}
QImage KScreenGrabDialog::drawRoundedRect(const QPixmap& src, const QRect& rc, int radius, bool antialiasing)
{
	return drawPath(src, rc, [rc, radius](QPainter& painter) {
		painter.drawRoundedRect(rc, radius, radius);
	}, antialiasing);
}

QImage KScreenGrabDialog::drawPolygon(const QPixmap& src, const QRect& rc, const QPolygon& polygon, bool antialiasing)
{
	return drawPath(src, rc, [rc, polygon](QPainter& painter) {
		painter.drawPolygon(polygon);
	}, antialiasing);
}

#ifdef Q_OS_MACOS
void KScreenGrabDialog::sendEditItemCollInfo()
{

}
#endif
