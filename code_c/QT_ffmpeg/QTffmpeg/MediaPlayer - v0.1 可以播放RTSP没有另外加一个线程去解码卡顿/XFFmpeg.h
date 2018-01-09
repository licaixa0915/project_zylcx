#pragma once
extern "C"
{
	#include <libavformat/avformat.h>
	#include <libswscale/swscale.h>
}
#include <string>
#include <QMutex>
class XFFmpeg
{
public:
	static XFFmpeg *Get()
	{
		static XFFmpeg ff;
		return &ff;
	}
	/* 打开视频文件，如果上次已经打开会先关闭
	*  @para path 视频文件路径
	*  &return bool 成功失败 ，失败错误信息通过GetError()获取
	*/
	bool Open(const char*path);
	void Close();
	/* 这里申请的内存的释放，考虑由外部释放 */
	AVPacket Read();

	AVFrame* Decode(const AVPacket *pkt);
	bool ToRGB(char *out, int outwidth, int outheight);
	std::string GetError();
	int totalMs;//总时长
	virtual ~XFFmpeg();
	int videoStream = 0;
protected://外部不能创建，只有内部可以创建
	char errorbuf[1024];//保存错误信息
	QMutex mutex;
	AVFormatContext *ic = NULL;
	AVFrame *yuv = NULL;
	
	SwsContext *cCtx = NULL;//定义转换器
	XFFmpeg();
};

