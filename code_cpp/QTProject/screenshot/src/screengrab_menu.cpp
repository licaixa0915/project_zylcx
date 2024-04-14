#include "screengrab_menu.h"
#include <QEvent>
#include <QMouseEvent>
#include <QAction>
#include <QPainter>
namespace
{
	const int ITEM_HEIGHT_ACTION = 30;
	const int ITEM_HEIGHT_SEPARATOR = 6;
}
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

KScreenGrabMenu::KScreenGrabMenu(QWidget *parent)
	: QWidget(parent)
	, m_actionCount(0)
	, m_bMenuItemCheckable(false)
{
	this->setWindowModality(Qt::WindowModal);

	setMouseTracking(true);

	setFixedWidth(dpiScaled(250));
	setFixedHeight(dpiScaled(4));

	m_items.clear();

	m_hoverRect = QRect();
	m_hoverAction = NULL;

	this->setCursor(Qt::ArrowCursor);
}

KScreenGrabMenu::~KScreenGrabMenu()
{

}

void KScreenGrabMenu::setItemCheckable(bool bCheckable)
{
	m_bMenuItemCheckable = bCheckable;
}

bool KScreenGrabMenu::event( QEvent* event )
{
	return QWidget::event(event);
}


bool KScreenGrabMenu::eventFilter( QObject* obj, QEvent* event )
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

	default:
		break;
	}
	
	return QWidget::eventFilter(obj, event);
}

void KScreenGrabMenu::mousePressEvent( QMouseEvent* event )
{

}

void KScreenGrabMenu::mouseReleaseEvent( QMouseEvent* event )
{
	detectHoveredItem(event->pos());

	if (NULL != m_hoverAction)
	{
		m_hoverAction->setProperty("fromMenu", true);
		m_hoverAction->trigger();

		if(m_hoverAction) //这里需要判空，调用trigger后action可能会被析构。例如点击了退出截屏，或隐藏工具栏
			m_hoverAction->setProperty("fromMenu", false);

		this->move(-1000, -1000);
		this->hide();
	}
}

void KScreenGrabMenu::mouseMoveEvent( QMouseEvent* event )
{
	detectHoveredItem(event->pos());
	update();
}

void KScreenGrabMenu::keyPressEvent( QKeyEvent *event )
{
	switch (event->key())
	{
	case Qt::Key_Up:
		if (NULL == m_hoverAction)
		{
			QList<MenuItem>::Iterator iter = m_items.begin();
			iter = iter + m_items.size() - 1;
			while (iter->type == ItemSeparator)
				--iter;
			m_hoverAction = iter->action;
			m_hoverRect = iter->rect;
			update();
		}
		else
		{
			QList<MenuItem>::Iterator iter = m_items.begin();
			while (iter->action != m_hoverAction)
				++iter;
			do
			{
				if (iter != m_items.begin())
					--iter;
				else
					iter = iter + m_items.size() - 1;
			} while (iter->type == ItemSeparator);
			m_hoverAction = iter->action;
			m_hoverRect = iter->rect;
			update();
		}
		break;
	case Qt::Key_Down:
		if (NULL == m_hoverAction)
		{
			QList<MenuItem>::Iterator iter = m_items.begin();
			while(iter->type == ItemSeparator)
				++iter;
			m_hoverAction = iter->action;
			m_hoverRect = iter->rect;

			update();
		}
		else
		{
			QList<MenuItem>::Iterator iter = m_items.begin();
			while (iter->action != m_hoverAction)
				++iter;
			do 
			{
				if(iter != m_items.end())
				{
					++iter;
					if (iter == m_items.end())
						iter = m_items.begin();
				}
				else
					iter = m_items.begin();
			} while (iter->type == ItemSeparator);
			m_hoverAction = iter->action;
			m_hoverRect = iter->rect;

			update();
		}
		break;
	case Qt::Key_Enter:
	case Qt::Key_Return:
		if (NULL != m_hoverAction)
		{
			m_hoverAction->setProperty("fromMenu", true);
			m_hoverAction->trigger();

			if (m_hoverAction) //这里需要判空，调用trigger后action可能会被析构，例如点击了退出截屏，或隐藏工具栏
				m_hoverAction->setProperty("fromMenu", false);

			this->move(-1000, -1000);
			this->hide();
		}
		break;
	default:
		break;
	}
}

void KScreenGrabMenu::paintEvent( QPaintEvent* event )
{
	// QColor clBg = KDrawHelper::getColorFromTheme("KScreenGrabMenu", KDrawHelper::Prop_Background);
	// QColor clBorder = KDrawHelper::getColorFromTheme("KScreenGrabMenu", KDrawHelper::Prop_Border);
	QColor clBg = QColor("#FFFFFF");
	QColor clBorder = QColor("#E6E6E6");
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	painter.setPen(Qt::NoPen);
	painter.setBrush(Qt::white);
    painter.drawRoundedRect(QRect(0, 0, width() - 1, height() - 1), dpiScaled(4), dpiScaled(2));

	painter.setPen(clBorder);
	painter.setBrush(clBg);
    painter.drawRoundedRect(QRect(0, 0, width() - 1, height() - 1), dpiScaled(4), dpiScaled(2));
	drawMenu(painter);
}

void KScreenGrabMenu::drawSelection( QPainter& painter )
{
	// QColor clBg = KDrawHelper::getColorFromTheme("KScreenGrabMenu-Selection", KDrawHelper::Prop_Background);
	QColor clBg = QColor("#FFFFFF");
	painter.setPen(clBg);
	painter.setBrush(Qt::NoBrush);

	painter.setPen(Qt::NoPen);
}



void KScreenGrabMenu::detectHoveredItem( const QPoint& pt )
{
	foreach (MenuItem item, m_items)
	{
		if (item.type == ItemAction &&
			item.rect.contains(pt))
		{
			m_hoverAction = item.action;
			m_hoverRect = item.rect;
			break;
		}
		else
		{
			m_hoverRect = QRect();
			m_hoverAction = NULL;
		}
	}
}

void KScreenGrabMenu::addAction( QAction* action, bool bLimitFree)
{
	MenuItem item;
	item.type = ItemAction;
	item.action = action;
	item.rect = QRect(dpiScaled(2), height() - dpiScaled(2), width() - dpiScaled(2), dpiScaled(ITEM_HEIGHT_ACTION));

	item.bLimitFree = bLimitFree;
	if (bLimitFree)
		item.iconName = "://icons_svg/16x16/big.svg";

	m_items.push_back(item);
	setFixedHeight(height() + dpiScaled(ITEM_HEIGHT_ACTION));
	m_actionCount++;
}

void KScreenGrabMenu::addSeparator( void )
{
	MenuItem item;
	item.type = ItemSeparator;
	item.action = NULL;
	item.rect = QRect(1, height() - 1, width() - 1, dpiScaled(ITEM_HEIGHT_SEPARATOR));

	m_items.push_back(item);
	setFixedHeight(height() + dpiScaled(ITEM_HEIGHT_SEPARATOR));
}

void KScreenGrabMenu::drawMenu( QPainter& painter )
{
	// QColor clBg = KDrawHelper::getColorFromTheme("KScreenGrabMenu-Item", KDrawHelper::Prop_Background);
	// QColor clItemText = KDrawHelper::getColorFromTheme("KScreenGrabMenu-Item", KDrawHelper::Prop_Text);
	// QColor clItemHover = KDrawHelper::getColorFromTheme("KScreenGrabMenu-Item", "background-hover");
	// QColor clSeparator = KDrawHelper::getColorFromTheme("KScreenGrabMenu-Separator", KDrawHelper::Prop_Background);
	QColor clBg = QColor("#0D0D0D");
	clBg.setAlpha(0.12*255);
	QColor clItemText = QColor("#0D0D0D");
	clItemText.setAlpha(0.66*255);
	QColor clItemHover = QColor("#EEEEEE");
	QColor clSeparator = QColor("#EEEEEE");
	QFont font;
	font.setFamily("PingFang SC");
	font.setPixelSize(dpiScaled(12));
	foreach (MenuItem item, m_items)
	{
		QRect rcPixmap(
			item.rect.left() + dpiScaled(6),
			item.rect.top() + (dpiScaled(ITEM_HEIGHT_ACTION) - dpiScaled(16)) / 2,
			dpiScaled(16),
			dpiScaled(16));

		QRect rcItemText(dpiScaled(32),
			item.rect.top() + dpiScaled(6),
			width() - dpiScaled(32),
			dpiScaled(16));

		QRect rcItemShortcut(item.rect.right() - dpiScaled(50),
			item.rect.top() + dpiScaled(6),
			dpiScaled(50),
			dpiScaled(16));

		if (ItemAction == item.type)
		{
			if (m_hoverAction == item.action)
			{
				painter.setBrush(clItemHover);
				painter.setPen(Qt::NoPen);
				painter.drawRect(QRect(item.rect.topLeft(), item.rect.bottomRight()
					+ QPoint(dpiScaled(-2), dpiScaled(-2))));
			}

			QIcon icon = item.action->icon();
			if (!icon.isNull())
			{
				QPixmap pixmap(item.action->icon().pixmap(dpiScaled(16), dpiScaled(16)));
				painter.drawPixmap(rcPixmap, pixmap);
			}

			if (item.bLimitFree)
			{
				QFontMetrics fm(font);
				QString text = item.action->text();
				if (text.isEmpty())
					text = item.action->toolTip();

				QRect rec = fm.boundingRect(text);
				int textWidth = rec.width();
				int textHeight = rec.width();
				QRect rcLimitFree = QRect(rcItemText.left() + textWidth + 5, rcItemText.top() + 3, 25, 13);
				QIcon ic = QIcon(item.iconName);
				ic.paint(&painter, rcLimitFree);
			}

			{
				if (m_bMenuItemCheckable && icon.isNull())
				{
					if (item.action->isChecked())
					{
						QPoint p1 = QPoint(rcPixmap.left() + 2, rcPixmap.top() + 6);
						QPoint p2 = QPoint(rcPixmap.left() + 6, rcPixmap.top() + 10);
						QPoint p3 = QPoint(rcPixmap.left() + 14, rcPixmap.top() + 4);
						painter.setPen(clItemText);
						painter.setBrush(Qt::NoBrush);
						painter.drawLine(p1, p2);
						painter.drawLine(p2, p3);
					}
				}
			}

			QString text = item.action->text();
			if (text.isEmpty())
				text = item.action->toolTip();

			painter.setPen(clItemText);
			painter.setBrush(Qt::NoBrush);
			painter.setFont(font);
			painter.drawText(rcItemText, text);

			if (!item.action->shortcut().isEmpty())
			{
				painter.setPen(QColor(160, 160, 160));
				painter.setBrush(Qt::NoBrush);
				painter.setFont(font);
				painter.drawText(rcItemShortcut, item.action->shortcut().toString(QKeySequence::NativeText));
			}			
		}
		else if (ItemSeparator == item.type)
		{
			painter.setPen(clSeparator);
			painter.setBrush(Qt::NoBrush);
			QLine line(QPoint(0,rcItemText.top()), rcItemText.topRight()
				+ QPoint(dpiScaled(-10), 0));
			line.translate(dpiScaled(5),
				- dpiScaled(ITEM_HEIGHT_SEPARATOR) / 2 - dpiScaled(2));
			painter.drawLine(line);
		}
	}
}

void KScreenGrabMenu::resetHoveredAction( void )
{
	m_hoverAction = NULL;
}

int KScreenGrabMenu::actionCount()
{
	return m_actionCount;
}
