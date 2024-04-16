#ifndef __KSCREENGRABDIALOG_H__
#define __KSCREENGRABDIALOG_H__


#include <QDialog>
#include <QStack>
#include "screengrab_toolbar.h"
#include "kscrngrabdefs.h"
#include "screengrab_menu.h"
#include "messageboxdialog.h"
#include <QDateTime>
QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE
class KScreenGrabToolBar;
class KScreenGrabColorPicker;
class KScreenGrabPen;
class KScreenGrabFontSetting;
class KScreenGrabHint;
class KScreenGrabMenu;
class KGlobalShortcut;
class KScreenGrabDialog;
class KWindowInfo;
class KScreenGrabTip;

class KScreenGrabDialog : public QDialog
{
    Q_OBJECT
    enum SelectMode 
	{ 
		smSniffingWindow,	// sniffing to select
		smDraging,			// dragging to select
		smSelection,		// selection done (include resizing & editing)
	};

	enum OperationType
	{
		optNone,
		optPenWidthChange,
		optColorChange,
		optFontSizeChange,
		optMove,
		optResize,
		optTextEdit,
		optAdd,
		optDel,
	};

	enum ItemDrawType
	{
		itemNone,
		itemInsert,
		itemMove,
		itemResizeTopLeft,
		itemResizeTopMid,
		itemResizeTopRight,
		itemResizeLeftMid,
		itemResizeRightMid,
		itemResizeBottomLeft,
		itemResizeBottomRight,
		itemResizeBottomMid,
		itemResizeBegin,
		itemResizeEnd,
		itemTextEdit,
	};

	enum HitTestResult
	{
	// WARNING, "corsorHit[]" is defined associated with "enum HitTest"
	// SEE ALSO "cursorHit[]" IN "screengrab_dialog.cpp"
		htNone,
		htSizeAll,
		htSizeTop,
		htSizeBottom,
		htSizeLeft,
		htSizeRight,
		htSizeTopLeft,
		htSizeTopRight,
		htSizeBottomLeft,
		htSizeBottomRight
	};

	enum CursorType
	{
		cursorNone,
		cursorSelect,
		cursorEdit,
		cursorSizeAll,
		cursorSizeHor,
		cursorSizeVer,
		cursorSizeBDiag,
		cursorSizeFDiag,
		cursorRestore,
	};

	enum EditType
	{
		editNone,
		editRect,
		editEllipse,
		editArrow,
		editBrush,
		editMosaic,
		editText,
		editFilter
	};

	enum PenWidth
	{
		penWidthSmall = 1,
		penWidthMiddle = 3,
		penWidthLarge = 7,
	};

	enum exitType
	{
		exitOther,
		exitEsc,
		exitCancel,
		exitMouseButtonR,
	};

	enum OperObjectType
	{
		//action item
		obInvalid = -1,
		obRect = 0,
		obEllipse,
		obArrow,
		obBrush,
		obMosaic,
		obText,
		obReselect,
		obUndo,
		obSetting,
		obCopy,
		obSave,
		obSwitchToolbar,
		obCancel,
		obDone,
		obExportImage,
		obSave2Pdf,
		obOcr,
		obEdit,
		obTranslateImage,

		//toolbar
		obToolBar,
		obMenu,

		// new added
		obPin,
	};

public:
	enum DrawShape
	{
		dsRect,
		dsRoundedRect,
		dsEllipse,
		dsPolygon,
	};

private:
	enum DrawPolygonState
	{
		dpsStart,
		dpsHolding,
		dpsEnd,
	};

	typedef struct OperationInfo_tag
	{
		OperObjectType type;
		int count;
		QSize szSelection;
		OperationInfo_tag()
		{
			type = obInvalid;
			count = 0;
		}
	}OperationInfo;

	struct ItemOperation
	{
		EditType editType;
		OperationType optType;
		int itemId;
		int      widthBack;
		QColor   colorBack;
		union
		{
			struct
			{
				QRect *rcPaintBack;
				QRect *rcTopleftBack;
				QRect *rcTopMidBack;
				QRect *rcTopRightBack;
				QRect *rcLeftMidBack;
				QRect *rcRightMidBack;
				QRect *rcBottomLeftBack;
				QRect *rcBottomMidBack;
				QRect *rcBottomRightBack;
			};
			struct
			{
				QPoint *ptStartBack;
				QPoint *ptEndBack;
				QRect *rcBeginBack;
				QRect *rcEndBack;
			};
			struct
			{
				QPoint* pointsBack;
				int		nPoints;
			};
			struct
			{
				int     fontSizeBack;
				QRect *rcTextBack;
				QString* textBack;
			};
		};
	};

#pragma pack(push, 1)
	struct EditItem
	{
		EditType editType;
		int      width;
		QColor   color;
		ItemDrawType drawType;
		union
		{
			struct  
			{
				QRect *rcPaint;
				QRect *rcTopleft;
				QRect *rcTopMid;
				QRect *rcTopRight;
				QRect *rcLeftMid;
				QRect *rcRightMid;
				QRect *rcBottomLeft;
				QRect *rcBottomMid;
				QRect *rcBottomRight;
			};
			struct
			{
				QPoint *ptStart;
				QPoint *ptEnd;
				QRect *rcBegin;
				QRect *rcEnd;
			};
			struct 
			{
				QPoint* points;
				int		nPoints;
			};
			struct
			{
				QString* fontFamily;
				int     fontSize;
				QRect *rcText;
				QString* text;
			};
		};
	};
#pragma pack(pop)
public:
	explicit KScreenGrabDialog(QWidget* parent = NULL, const WId wpsWId = NULL,
	bool isHide = true, KscrnGrabHelper::ActionFlags flags = KscrnGrabHelper::ACF_Default, DrawShape drawShape = dsRect);
    ~KScreenGrabDialog();
	const QSize* getTooltipSizeHint(QWidget*);
protected:
	virtual bool event(QEvent* event) override;
	virtual bool eventFilter(QObject* obj, QEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;

	virtual void contextMenuEvent(QContextMenuEvent* event) override;
	virtual void showEvent(QShowEvent *) override;
private:
    void drawOverlay(QPainter& painter, const QBrush& brush);
    void drawSelection(QPainter& painter, const QBrush& brush, int width);
	QRect drawResizePoint(QPainter& painter, const QPoint& center, bool bCircle = false);
	void drawResizePoint(QPainter& painter, const QRect& rc);
	void drawResizeCircle(QPainter& painter, const QRect& rc);
	void drawResizeCircle(QPainter& painter, const QPoint& p1, const QPoint& p2);
    void drawSizeTooptip(QPainter& painter);
    void drawPixelZoomBox(QPainter& painter);
	void drawEditHistory(QPainter& painter);
	void drawEditing(QPainter& painter);
	void drawArrow(QPainter& painter, int nSize, QPoint& ptStart, QPoint& ptEnd);
	void drawBrush(QPainter& painter, QPoint* pts, int nPts);
	void setupPainter(QPainter& painter);
	void drawEditItem(QPainter& painter, EditItem& item);
	void drawPolygonStartPoint(QPainter& painter);
	void drawRectShape(QPainter& painter, QRect& rc, bool bEdit);
	void drawEllipseShape(QPainter& painter, QRect& rc, bool bEdit);
	void drawArrowShape(QPainter& painter, int nSize, QPoint& ptStart, QPoint& ptEnd, bool bEdit);
	void drawBrushShape(QPainter& painter, QPoint* pts, int nPts, bool bEdit);
	void drawBrushMouseMove(QPainter& painter);
	void drawMosaicMove(QPainter& painter);
	void drawTextShape(QPainter& painter, QColor& color, QFont& font, QRect& rc, QString text, bool bEdit);

	void initCursors(void);
	void initActions(void);
	void initControls(void);
	void initToolbarHeader(void);
	void initToolbar(void);
	void initToolbarSetting(void);
	void initContextMenu(void);
	void setupToolbarSetting(EditType editMode);
	void reset();
	void sniffingWindow(const QPoint& pt);
	void dragRegion(const QPoint& pt);
	void moveSelection(const QPoint& pt);
	void movePolygon(const QPoint& pt);
	void drawRubber(const QPoint& pt);
	QPoint nearestPoint(const QPoint& pt, bool&);
	void endDrawPolygon();

	void relocateToolBar();
	void toggleAction(KScrnGrabToolButton* action);
	void resetCursor(QPoint &pt);
	HitTestResult hitTest(QPoint &pt);
	QPair<int, int> adsorbLine(const QPoint& pt, bool isHoriz, int nDirection);
	QPoint adsorbLine(const QPoint& pt, const QPoint& ptOld);
	void setEditing();
	void setPaintRect(const QPoint& pt);
	void setArrow(const QPoint& pt);
	void setBrush(const QPoint& pt);
	void setMosaic(const QPoint& pt);
	void pushUndoStack();
	void popUndoStack();
	void clearUndoStack();
	void clearOperationStack();

	void moveTextEditRect(const QPoint& pt);
	void resetTextEdit(const QPoint& pt);
	bool isPtAtRectEdges(const QRect& rc, const QPoint& pt);

	void drawTextAreaRect(QPainter& painter);
	void drawText(QPainter& painter, QColor& color, QFont& font, QRect& rc, QString text);
	void saveText();
	void selectPenWidth(int width);

	void showMenu(QPoint pt);
	void hideMenu();
	void showToolBar();
	void hideToolBar();
	void setToolBarVisible(bool bVisible);
	void showToolBarHeader();
	void hideToolBarHeader();
	QString pngPath(const QString& name);
	QString resSvgPath(const QString& name, bool bOldPath = false);
	void setEscGlobalShortcutEnable(bool bEnable);
	bool isTriggerFromMenu(QAction *action);

	void adjustActionClickedInfo(OperObjectType toolBarType, OperObjectType itemType);
	void adjustToolBarShowInfo(OperObjectType toolBarType);
	void addObjOperTimes(QList<OperationInfo> &lstInfo, OperObjectType objType);

	void actionUndo();
	void actionSave();
	void actionCopy();
	void actionCanecl();
	void actionDone();
	void actionPin();
	bool isSingleItem(KscrnGrabHelper::ActionFlag);
	bool isSettingHintShown(const QString& key);
	void setSettingHintShownFlag(const QString& key, bool flag);

	QString parseOperObjectType(OperObjectType);
	QString parseExitType(exitType);
	void sendInfoColl();

    QBitmap drawMask(const QPixmap& src, const QRect& rc, std::function<void(QPainter&)> drawFunc, bool antialiasing);
	QImage drawPath(const QPixmap& src, const QRect& rc, std::function<void(QPainter&)> drawFunc, bool antialiasing);
	QImage drawEllipse(const QPixmap& src, const QRect& rc, bool antialiasing);
	QImage drawRoundedRect(const QPixmap& src, const QRect& rc, int radius, bool antialiasing);
	QImage drawPolygon(const QPixmap& src, const QRect& rc, const QPolygon& polygon, bool antialiasing);

	KScreenGrabDialog::ItemDrawType getItemDrawType(const QPoint& pos);
	bool textItemDoubleClicked(const QPoint& pos);
	void showTextEdit(const QRect& rc, const QString& text);
	bool posInRect(const QRect& rc, int nPenWidth, const QPoint& pos);
	bool posInEllipse(const QRect& rc, int nPenWidth, const QPoint& pos);
	void changeItemShape(const QPoint& pos);
	void itemSelectTest(bool& bHitEditItem, int nId, KScreenGrabDialog::ItemDrawType type);
	void updateItemResizeCircle();
	bool arrowContaisPos(const QPoint& ptStart, const QPoint& ptEnd, int nSize, const QPoint& pos);
	bool lineContainsPos(QPoint* points, int nPoints, int nWidth, const QPoint& pos);
	void drawTextEditRect(QPainter& painter, const QRect& rect);
	void updateEditItemText();
	void clearEdit();
	void updateItemPenWidth(int nSize);
	void updateItemColor(const QColor& cl);
	void pushOperation();
	bool popOperation(bool bClear = false);
	void backupRestzeCircle(int nId);
	void clearEditState();
	void showTextEditMaskWidget();
	void hideTextEditMaskWidget();
	void sendEditItemCollInfo();
	void closeMoreToolBar();
signals:
	void loginSignal();
	void upgradeVipSignal(int);

	void drawRectSignal();
	void drawEllipseSignal();
	void drawRoundedSignal();
	void drawPolygonSignal();

	void sigDlgDone(int);
	void sigChangeItemAction(int);
	void sigActionPin(const QImage&, const QRect&, bool);
private slots:
	void onActionRect();
	void onActionEllipse();
	void onActionArrow();
	void onActionBrush();
	void onActionMosaic();
	void onActionText();
	void onActionUndo();
	void onActionSave();
	void onActionCopy();
	void onActionCancel();
	void onActionDone();
	void onActionPin();
	void onActionReSelect();
	void onActionToggleToolbar();
	void onActionOcr();
	void onActionEdit();
	void onMoreEditClicked();
	void onActionEditClarity();
	void onActionEditRemove();
	void onActionEditMatting();
	void onActionTranslateImage();
	void onActionSave2Pdf();
	void onActionSetting();
	void onActionExportImage();

	void onActionDrawRect();
	void onActionDrawRoundedRect();
	void onActionDrawEllipse();
	void onActionDrawPolygon();
	void toggleDrawAction();
	void onDrawShapeChanged();

	void onFinished(int);
	void textChanged();
	void colorChanged(const QColor& color);
	void penSizeChanged(int size);
	void fontSizeChanged(int size);
	void EscKeyHandler();
	void onChangeSkinMode();
	void changeActionSelect(int nItemId);
	void onActionDelete();
private:
	KScrnGrabToolButton* m_toolbtnRect = nullptr;
	KScrnGrabToolButton* m_toolbtnEllipse = nullptr;
	KScrnGrabToolButton* m_toolbtnArrow = nullptr;
	KScrnGrabToolButton* m_toolbtnBrush = nullptr;
	KScrnGrabToolButton* m_toolbtnMosaic = nullptr;
	KScrnGrabToolButton* m_toolbtnText = nullptr;
	KScrnGrabToolButton* m_toolbtnUndo = nullptr;
	KScrnGrabToolButton* m_toolbtnPin = nullptr;
	KScrnGrabToolButton* m_toolbtnCancel = nullptr;
	KScrnGrabToolButton* m_toolbtnCopy = nullptr;
	KScrnGrabToolButton* m_toolbtnSave = nullptr;
	QAction*		m_actionRect = nullptr;
	QAction*		m_actionEllipse = nullptr;
	QAction*		m_actionArrow = nullptr;
	QAction*		m_actionBrush = nullptr;
	QAction*		m_actionMosaic = nullptr;
	QAction*		m_actionText = nullptr;
	QAction*		m_actionUndo = nullptr;
	QAction*		m_actionCopy = nullptr;
	QAction*		m_actionSave = nullptr;
	QAction*		m_actionCancel = nullptr;
	QAction*		m_actionDone = nullptr;
	QAction*		m_actionPin = nullptr;
	QAction*		m_actionReSelect = nullptr;
	QAction*		m_actionToggleToolbar = nullptr;
	QAction*		m_actionPenSmall = nullptr;
	QAction*		m_actionPenMiddle = nullptr;
	QAction*		m_actionPenLarge = nullptr;
	QAction*		m_actionFont = nullptr;
	QAction*		m_actionSeparator = nullptr;
	QAction*		m_actionColor = nullptr;
	QAction*		m_actionOcr = nullptr;
	QAction*		m_actionSave2Pdf = nullptr;
	QAction*		m_actionSetting = nullptr;
	QAction*		m_actionExportImage = nullptr;
	QAction*		m_actionImgTranslate = nullptr;

	QAction*		m_actionDrawRect = nullptr;
	QAction*		m_actionDrawRoundedRect = nullptr;
	QAction*		m_actionDrawEllipse = nullptr;
	QAction*		m_actionDrawPolygon = nullptr;
	QAction*		m_actionRoundValue = nullptr;
    QAction*		m_actionFixedRect = nullptr;

	KScreenGrabMenu*		m_Menu;
	KScreenGrabToolBar*		m_toolbar = nullptr;
	KScreenGrabToolBar*		m_tooleditbar = nullptr;
	KScreenGrabToolBar*		m_tooleditmore = nullptr;
	KScreenGrabToolBar*		m_toolbarSettings = nullptr;
	KScreenGrabToolBarHeader*	m_toolbarHeader = nullptr;
	KScreenGrabColorPicker* m_colorPicker = nullptr;
	KScreenGrabPen*			m_penSmall = nullptr;
	KScreenGrabPen*			m_penMiddle = nullptr;
	KScreenGrabPen*			m_penLarge = nullptr;
	KScreenGrabFontSetting* m_fontSetting = nullptr;
	KScreenGrabHint*		m_hint = nullptr;
    KScreenGrabHint*        m_settingHint = nullptr;
    KScreenGrabPen*         m_roundValueBtn = nullptr;
    KScreenGrabPen*         m_fixedRectBtn = nullptr;

	QPoint			m_screenOffset;

	QPixmap			m_pixmapDesktop;
	QPixmap			m_pixmapSniff;
	QPixmap         m_pixmapMosaic;
	QRect			m_rcDesktop;
	WId				m_widSelection;
	QRect			m_rcSelection;
	QRect			m_rcSelectionBackup;
	QPolygon		m_polySelection;
	QPolygon		m_polySelectionBackup;
	QPolygon		m_polyRubberSelection;
	SelectMode		m_selectmode;
	QPoint			m_ptPressed;
	QPoint			m_ptMouseOld;
	
	QPair<int, int> m_lineHoriz;
	QPair<int, int> m_lineVert;

	QCursor			m_cursorBitmapSelect;
	QCursor			m_cursorBitmapRestore;
	QCursor			m_cursorBitmapEdit;
	QCursor			m_cursorBitmapSizeAll;
	QCursor			m_cursorBitmapSizeHor;
	QCursor			m_cursorBitmapSizeVer;
	QCursor			m_cursorBitmapSizeBDiag;
	QCursor			m_cursorBitmapSizeFDiag;
	CursorType		m_cursorBitmapType;
	Qt::CursorShape	m_cursorShape;

	HitTestResult	m_hitTestResizing;

	bool			m_fResizing;
	bool			m_fAltPressed;

	bool			m_fInited;
	
	QList<EditItem> m_listEdit;
	EditType		m_editType;
	bool			m_fEditing;
	static int		s_nPenWidthRect;
	static int		s_nPenWidthEllipse;
	static int		s_nArrowSize;
	static int		s_nPenWidthBrush;
	int				m_toolBarActionCount;
	int				m_nTextFontSize;
	QString			m_strTextFontFamily;

	QColor			m_colorRect;
	QColor			m_colorEllipse;
	QColor			m_colorArrow;
	QColor			m_colorBrush;
	QColor			m_colorText;

	QRect			m_rcPaint;

	QPoint			m_ptArrowStart;
	QPoint			m_ptArrowEnd;

	QPoint*			m_pointsBrush;
	int				m_nNumPoints;
	int				m_nPtCurrent;
	int				m_nNumPtIncre;

	QTextEdit*		m_textEdit;
	QRect			m_rcTextEdit;
	QRect			m_rcTextEditBackup;
	QString			m_strText;
	int				m_nEditHeight;
	int				m_nEditWidth;
	bool			m_fMoving;

	bool			m_fToolbarToggleOn;
	bool			m_fShowSetting;

	WId				m_widWPS;
	QRect			m_rcWPS;

    KGlobalShortcut* m_GlobalShortCutEsc;
	bool			m_bIsHideWPS;

	bool            m_bEnableEscCancelEvent;
	bool			m_bSingleItemOnly;
	QImage			m_selectionImage;

	KscrnGrabHelper::ActionFlags m_flags;

	//记录工具栏和菜单栏的展示和操作信息，用于发送信息收集
	QList<OperationInfo> m_lstToolBarAndMenuShownInfo;
	QList<OperationInfo> m_lstToolBarActShownInfo;
	QList<OperationInfo> m_lstToolBarActClickedInfo;
	QList<OperationInfo> m_lstMenuActShownInfo;
	QList<OperationInfo> m_lstMenuActClickedInfo;

	exitType m_exitType;
	DrawShape m_drawShape;
	DrawPolygonState m_polygonState;
	bool m_isOriginPoint;
	bool m_badSelection;
	bool m_disableZoomBox;
	QMap<QWidget*, QSize> m_tooltipSizeHints;
	bool m_login;
	bool m_upgradeVip;
    KWindowInfo*    m_windowInfo;
	int m_result;
	bool m_bMousePressed;
	bool m_bSniffSelectDone;
	QRect m_rcTopleft;
	QRect m_rcTopMid;
	QRect m_rcTopRight;
	QRect m_rcLeftMid;
	QRect m_rcRightMid;
	QRect m_rcBottomLeft;
	QRect m_rcBottomMid;
	QRect m_rcBottomRight;
	QRect m_rcBegin;
	QRect m_rcEnd;
	ItemDrawType m_itemDrawType;
	int m_nShapeChangeItemID;
	QRect m_rcTopleftBackup;
	QRect m_rcTopMidBackup;
	QRect m_rcTopRightBackup;
	QRect m_rcLeftMidBackup;
	QRect m_rcRightMidBackup;
	QRect m_rcBottomLeftBackup;
	QRect m_rcBottomMidBackup;
	QRect m_rcBottomRightBackup;
	QRect m_rcBeginBackup;
	QRect m_rcEndBackup;
	QRect m_textEditBackup;
	QRect m_rectEditBackup;
	QPoint m_ptStartEditBackup;
	QPoint m_ptEndEditBackup;
	QPoint*	m_pointsBrushBackup;
	QString m_strTextEditBackup;
	QColor m_colorBackup;
	int m_nPenWidthBackup;
	int m_nFontSizeBackup;
	int m_nPointsBackup;
	bool m_bTextHasFocus;
	bool m_bTextEditHasSelected;
	bool m_bTextDoubleClicked;
	bool m_bBrushHasDraw;
	QStack<ItemOperation> m_stackOpt;
	QStack<EditItem> m_stackDelete;
	OperationType m_itemOperation;

	bool m_bSniffWindowGrab;
	QVector<QPoint> m_vecBrushPoint;
    QVector<QPoint>m_vecMosaicPoint;
	QWidget* m_widgetTextEditMask;
};

#endif // __KSCREENGRABDIALOG_H__
