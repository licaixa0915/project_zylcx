#ifndef __KSCREENGRAB_DEFS_H__
#define __KSCREENGRAB_DEFS_H__

class KscrnGrabHelper
{
public:
	enum DlgType
	{
		Dlg_ScrnGrab,
		Dlg_ShortcutSetting,
		Dlg_ShortcutConflict,
		Dlg_ScrnGrabMainWnd,
	};

	enum UpgradeVipType
	{
		Upgrade_RoundRect,
		Upgrade_Polygon,
		Upgrade_Pin,
	};

	enum ExitCode
	{
		EC_Ocr = QDialog::Accepted + 0x10,
		EC_Save2PDF,
		EC_ExportImage,
		EC_Pin,
		EC_Copy,
		EC_Edit,
		EC_EditClarity,
		EC_EditRemove,
		EC_EditMatting,
	};

	enum ActionFlag
	{
		ACF_Rect    = 0x0001,//矩形工具
		ACF_Ellipse = 0x0002,//椭圆工具
		ACF_Arrow   = 0x0004,//箭头工具
		ACF_Brush   = 0x0008,//画笔工具
		ACF_Text    = 0x0010,//文字
		ACF_Undo    = 0x0020,//回退
		ACF_Setting = 0x0040,//设置
		ACF_Copy    = 0x0080,//复制
		ACF_Save    = 0x0100,//保存
		ACF_Cancel  = 0x0200,//取消
		ACF_Done    = 0x0400,//完成
		ACF_Mosaic  = 0x0800,//马赛克

		ACF_TranslateImage = 0x0100000,
		ACF_ExportImage    = 0x0200000,
		ACF_Save2Pdf       = 0x0400000,
		ACF_Ocr            = 0x0800000,
		ACF_Edit		   = 0x1000000,
		ACF_Pin            = 0x2000000,

		ACF_Default = ACF_Rect \
		| ACF_Ellipse \
		| ACF_Arrow \
		| ACF_Brush \
		| ACF_Mosaic \
		| ACF_Text \
		| ACF_Undo \
		| ACF_Copy \
		| ACF_Save \
		| ACF_Pin \
		| ACF_Cancel \
		| ACF_Done,
	};
	Q_DECLARE_FLAGS(ActionFlags, ActionFlag)

	typedef struct ScrnGrabArgs_tag {
		DlgType type;
		QWidget *host;
		QVariantMap extInfo;

		ScrnGrabArgs_tag()
		{
			type = Dlg_ScrnGrab;
			host = NULL;
		}
	}ScrnGrabArgs;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KscrnGrabHelper::ActionFlags)
#endif /* __KSCREENGRAB_DEFS_H__ */
