#include "messageboxdialog.h"
#include "windowinfo.h"

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

KBoxTitleBar::KBoxTitleBar(QWidget* parent)
	: QWidget(parent)
	, m_titleColor("#FFFFFF")
{
	setFixedHeight(25);
	init();
}

KBoxTitleBar::~KBoxTitleBar()
{

}

void KBoxTitleBar::init()
{
	setStyleSheet("border:none;");
	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	mainLayout->setContentsMargins(0, 0, 0, 0);
	mainLayout->setSpacing(0);

	mainLayout->addStretch();
	mainLayout->addSpacing(16);
	setLayout(mainLayout);
	// connect(kcoreApp, SIGNAL(notifyAppSkinChange(QString)), this, SLOT(onSkinChanged()));
	onSkinChanged();
}

bool KBoxTitleBar::event(QEvent* e)
{
	return QWidget::event(e);
}


void KBoxTitleBar::setTitleBarColor(const QColor& cl)
{
	m_titleColor = cl;
}

void KBoxTitleBar::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);

	//QColor clBg = KDrawHelper::getColorFromTheme("TitleBarWidget", KDrawHelper::Prop_Background);
	QColor clBg = QColor(Qt::blue);
	QPainterPath path;
	path.addRoundedRect(QRect(QPoint(0, 0), geometry().size()), dpiScaled(4), dpiScaled(4));

	painter.fillPath(path, QBrush(clBg));
}

void KBoxTitleBar::onSkinChanged()
{

}

//////////////////////////////////////////////////////////////////////////

KMessageBoxDialog::KMessageBoxDialog(QWidget *parent /* = 0 */)
	: Base(parent)
	, m_btnOk(nullptr)
	, m_labelText(nullptr)
{
	setFixedSize(350, 150);
	setWindowFlags(windowFlags() & ~(Qt::WindowFullscreenButtonHint));
	setWindowModality(Qt::ApplicationModal);
	QWidget* w = new KBoxTitleBar(this);
	m_labelText = new QLabel(this);
	m_btnOk = new QPushButton(tr("OK"), this);
	m_btnOk->setFixedSize(90, 40);
	m_btnOk->setContentsMargins(24, 0, 24, 0);
	QVBoxLayout* vLayout = new QVBoxLayout(this);
	vLayout->setSpacing(0);
	vLayout->setSpacing(0);
	vLayout->addWidget(w);
	vLayout->addSpacing(20);
	vLayout->addWidget(m_labelText);
	vLayout->addStretch();
	vLayout->addSpacing(20);
	QHBoxLayout* hLayout = new QHBoxLayout(this);
	hLayout->addStretch();
	hLayout->addWidget(m_btnOk);
	vLayout->addLayout(hLayout);
	// connect(kcoreApp, SIGNAL(notifyAppSkinChange(QString)), this, SLOT(onSkinChanged()));
	connect(m_btnOk, &QPushButton::clicked, this, &KMessageBoxDialog::accept);
	onSkinChanged();
	m_btnOk->setStyleSheet(QString("QPushButton{background-color:#4585D7;color:#ffffff;font-family:'PingFang SC';font-size:%1px;border-radius:%2px;}\
								   QPushButton:hover{background: #64A3F3;}\
								   QPushButton:pressed{background: #2B7ADF;}"));
	KWindowUtil::setupDialogTitleBar(this);
}

KMessageBoxDialog::~KMessageBoxDialog()
{
}

void KMessageBoxDialog::setContent(const QString& text)
{
	m_labelText->setText(text);
}


void KMessageBoxDialog::onSkinChanged()
{
	QColor clText = QColor(Qt::blue);
	//QColor clText = KDrawHelper::getColorFromTheme("TitleBarWidget", KDrawHelper::Prop_Text);
	QString strCl = QString("rgba(%1, %2, %3, %4)").arg(clText.red()).arg(clText.green()).arg(clText.blue()).arg(clText.alpha());
	if (m_labelText)
	m_labelText->setStyleSheet(QString("QLabel{color:%1;font-family:'PingFang SC';font-size:%2px;}")
		.arg(strCl)
		.arg(12));
}

bool KMessageBoxDialog::event(QEvent *e)
{
	return Base::event(e);
}

bool KMessageBoxDialog::eventFilter(QObject *o, QEvent *e)
{
	return Base::eventFilter(o, e);
}

void KMessageBoxDialog::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	//QColor clrBg = KDrawHelper::getColorFromTheme("KScreenGrabWindow", KDrawHelper::Prop_Background);
	QColor clrBg = QColor(Qt::white);
	QPainter p(this);
	p.setRenderHint(QPainter::Antialiasing);
	// QPainterPath path = KDrawHelper::getBoundPath(contentsRect(), 4, KDrawHelper::ROUND_ALL);
	// p.setBrush(clrBg);
	// p.setPen(Qt::NoPen);
	// p.drawPath(path);
}

void KMessageBoxDialog::keyPressEvent(QKeyEvent* e)
{
	if (e->key() == Qt::Key_Escape)
	{
		e->accept();
		this->reject();
	}
	else
	{
		Base::keyPressEvent(e);
	}
}
