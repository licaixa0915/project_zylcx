#include "windowinfo.h"

#undef slots
#define OBJC_OLD_DISPATCH_PROTOTYPES 1
#include <Cocoa/Cocoa.h>
#include <objc/objc.h>

namespace KWindowUtil {

void setupDialogTitleBar(QWidget *window)
{
	if (!window || !window->window())
		return ;
	window->setVisible(true);

	NSToolbar* customToolbar = [[[NSToolbar alloc] initWithIdentifier:@"main"] autorelease];
	customToolbar.showsBaselineSeparator = NO;

	NSView* view = (__bridge NSView *)reinterpret_cast<void *>(window->window()->winId());
	if (!view) return;
	NSWindow* nsWindow = [view window];
	if (!nsWindow) return ;
	
	nsWindow.titlebarAppearsTransparent = YES;
	nsWindow.titleVisibility = NSWindowTitleHidden;
	nsWindow.toolbar = customToolbar;

	nsWindow.appearance = [NSAppearance appearanceNamed:NSAppearanceNameVibrantLight];

	nsWindow.styleMask = NSWindowStyleMaskClosable
			| NSTitledWindowMask
			| NSFullSizeContentViewWindowMask;

	[[nsWindow standardWindowButton:NSWindowMiniaturizeButton] setEnabled:NO];
	[[nsWindow standardWindowButton:NSWindowZoomButton] setEnabled:NO];
}

void setWindowTopLevel(QWidget* w, bool bTop)
{
	if (!w) return;
	NSView* view = (NSView*)w->winId();
	if (!view) return;
	NSWindow* window = [view window];
	if (!window) return;
	if (bTop)
		[window setLevel:kCGScreenSaverWindowLevel];
	else
		[window setLevel:kCGFloatingWindowLevel];
}
}


namespace
{
	QPixmap QPixmapFromCGImageRef(const CGImageRef &image)
	{
		if(!image)
			return QPixmap();

		const std::size_t width = CGImageGetWidth(image);
		const std::size_t height = CGImageGetHeight(image);
		QImage result(static_cast<int>(width), static_cast<int>(height), QImage::Format_ARGB32_Premultiplied);
		if(result.isNull())
			return QPixmap();

		result.fill(Qt::transparent);

		CGColorSpace* colorSpace = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
		if(!colorSpace)
			return QPixmap();

		CGContext* context = CGBitmapContextCreate(static_cast<void*>(result.bits()),
				width, height, 8, width * 4, colorSpace, kCGImageAlphaPremultipliedFirst | kCGBitmapByteOrder32Host);

		CGRect rect = CGRectMake(0, 0, width, height);
		CGContextDrawImage(context, rect, image);
		QPixmap pix = QPixmap::fromImage(result);
		return pix;
	}
}


KWindowInfo::KWindowInfo(QObject* parent, QWidget* topWidget)
	: QObject(parent)
	, m_windowList(NULL)
	, m_sniffId(0)
	, m_windowId(0)
{
	getWindowList(topWidget);
}

KWindowInfo::~KWindowInfo()
{
	CFRelease(m_windowList);
}

void KWindowInfo::getWindowList(QWidget* widget)
{
	NSView* view = (__bridge NSView *)reinterpret_cast<void *>(widget->window()->winId());
	if (!view) return;
	NSWindow* nsWindow = [view window];
	if (!nsWindow) return ;
	
	CGWindowID window_id = (CGWindowID)[nsWindow windowNumber];
	m_windowList = CGWindowListCopyWindowInfo(kCGWindowListOptionOnScreenBelowWindow, window_id);
	m_windowId = (long)window_id;
}

QRect KWindowInfo::getWindowRect(const QPoint& pt)
{
	if (m_windowList == NULL)
		return QRect();

	NSMutableArray *windows = (NSMutableArray *)m_windowList;

	for (NSDictionary *window in windows)
	{
		CGRect bounds;
		CGRectMakeWithDictionaryRepresentation((CFDictionaryRef)[window objectForKey:@"kCGWindowBounds"], &bounds);
		QRect rc = QRect(bounds.origin.x, bounds.origin.y, bounds.size.width, bounds.size.height);
		NSString *name = [window objectForKey:@"kCGWindowName" ];
		bool bDock = false;
		if([name containsString:@"Dock"])
			bDock = true;
		else
			bDock = false;

		if (rc.contains(pt) && !bDock)
		{
			m_sniffId = [[window objectForKey:(id)kCGWindowNumber] intValue];
			m_rectSniff = rc;
			return rc;
		}
	}
	return QRect();
}

QPixmap KWindowInfo::getWindowPixmap(const QPoint& pt, const QRect& sniffRect, const QRect& sgRect,
									 int screenOffset, bool& bFullScreen)
{
	if (m_windowList == NULL)
		return QPixmap();

	if (sniffRect.right() - sniffRect.left() >= sgRect.right() - sgRect.left()
			&&sniffRect.bottom() - sniffRect.top() >= sgRect.bottom() - sgRect.top())
	{
		bFullScreen = true;
		return QPixmap();
	}

	qreal ratio = qApp->devicePixelRatio();

	CFArrayRef onScreenWindows = CGWindowListCreate(kCGWindowListOptionOnScreenBelowWindow, m_windowId);
	CFMutableArrayRef desktopElements = CFArrayCreateMutableCopy(NULL, 0, onScreenWindows);

	for (int i = CFArrayGetCount(desktopElements) - 1; i >= 0; i--)
	{
		CGWindowID window = (CGWindowID)(uintptr_t)CFArrayGetValueAtIndex(desktopElements, i);
		if (m_sniffId != window)
		{
			CFArrayRemoveValueAtIndex(desktopElements, i);
		}
	}

	CGImageRef screenShot = CGWindowListCreateImageFromArray(CGRectInfinite, desktopElements, kCGWindowImageBestResolution);
	CGRect rectSniff = CGRectMake(sniffRect.left() * ratio,
								  sniffRect.top() * ratio + screenOffset * ratio, sniffRect.width() * ratio, sniffRect.height() * ratio);
	CGImageRef mainCroppedCGImage = CGImageCreateWithImageInRect(screenShot, rectSniff);
	NSSize frameSize = NSMakeSize(sniffRect.width(),sniffRect.height());

	QPixmap pixmap = QPixmapFromCGImageRef(mainCroppedCGImage);

//	NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithCGImage:screenShot];
//	NSData* data = [rep representationUsingType:NSPNGFileType properties:[NSDictionary dictionary]];
//	[data writeToFile:@"/Users/liufuchen/Desktop/foo.png" atomically:YES];

	CGImageRelease(screenShot);
	CGImageRelease(mainCroppedCGImage);
	return pixmap;
}
