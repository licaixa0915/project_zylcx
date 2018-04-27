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
	/* ����Ƶ�ļ�������ϴ��Ѿ��򿪻��ȹر�
	*  @para path ��Ƶ�ļ�·��
	*  &return bool �ɹ�ʧ�� ��ʧ�ܴ�����Ϣͨ��GetError()��ȡ
	*/
	int Open(const char*path);
	void Close();
	/* ����������ڴ���ͷţ��������ⲿ�ͷ� */
	AVPacket Read();

	AVFrame* Decode(const AVPacket *pkt);
	bool ToRGB(char *out, int outwidth, int outheight);
	bool Seek(float pos);//���ƽ��ȵ��϶�
	std::string GetError();
	int totalMs;//��ʱ��
	virtual ~XFFmpeg();
	int videoStream = 0;
	int fps = 0;
	int pts = 0;
	bool fileOpenFlag = false;
	bool isPlay = false;
protected://�ⲿ���ܴ�����ֻ���ڲ����Դ���
	char errorbuf[1024];//���������Ϣ
	QMutex mutex;
	AVFormatContext *ic = NULL;
	AVFrame *yuv = NULL;
	SwsContext *cCtx = NULL;//����ת����
	XFFmpeg();
};
