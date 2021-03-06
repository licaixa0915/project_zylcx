#include "XFFmpeg.h"

#pragma comment(lib, "avformat.lib") //avformat_open_input用到
#pragma comment(lib, "avutil.lib")//工具类  av_strerror用到
#pragma comment(lib, "avcodec.lib")//av_read_frame用到，视频解码模块
#pragma comment(lib, "swscale.lib")//转码模块

static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

int XFFmpeg::Open(const char*path)
{
	Close();
	mutex.lock();
	int re = avformat_open_input(&ic, path, 0, 0);//打开一个视频文件流
	if (re != 0)
	{
		mutex.unlock();
		av_strerror(re, errorbuf, sizeof(errorbuf));//将返回的错误码，转换成具体的错误信息
		printf("open %s failed: %s\n", path, errorbuf);
		return 0;
	}
	totalMs = (ic->duration / AV_TIME_BASE * 1000);
	
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVCodecContext *enc = ic->streams[i]->codec;
		printf("i = %d, type = %d\n", i, enc->codec_type);//多媒体的类型  0表示video 1表示audio
		if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoStream = i;
			fps = r2d(ic->streams[i]->avg_frame_rate);
			AVCodec *codec = avcodec_find_decoder(enc->codec_id);//根据codec_id查找系统中是不是有此解码器
			if (!codec)
			{
				mutex.unlock();
				printf("video code not find\n");
				return 0;
			}
			int err = avcodec_open2(enc, codec, NULL);
			if (err != 0)
			{
				mutex.unlock();
				av_strerror(err, errorbuf, sizeof(errorbuf));
				return 0;
			}
			printf("open codec success!\n");
		}
	}
	fileOpenFlag = true;
	mutex.unlock();
	
	return totalMs;
}

AVFrame* XFFmpeg::Decode(const AVPacket *pkt)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		printf("ic is null\n");
		return NULL;
	}

	if (yuv == NULL)
	{
		yuv = av_frame_alloc();
	}
	if (pkt->stream_index == videoStream)
	{
		int re = avcodec_send_packet(ic->streams[pkt->stream_index]->codec, pkt);
		if (re != 0)
		{
			mutex.unlock();
			av_strerror(re, errorbuf, sizeof(errorbuf));
			printf("avcodec_send_packet error\n");
			return NULL;
		}

		re = avcodec_receive_frame(ic->streams[pkt->stream_index]->codec, yuv);
		if (re != 0)
		{
			mutex.unlock();
			av_strerror(re, errorbuf, sizeof(errorbuf));
			printf("avcodec_receive_frame error\n");
			return NULL;
		}
	}
	pts = (yuv->pts *r2d(ic->streams[pkt->stream_index]->time_base)) * 1000;

	mutex.unlock();
	return yuv;
}
void XFFmpeg::Close()
{
	mutex.lock();
	if (ic)
	{
		avformat_close_input(&ic);
	}

	if (yuv) av_frame_free(&yuv);
	if (cCtx)
	{
		sws_freeContext(cCtx);
		cCtx = NULL;
	}
	fileOpenFlag = false;
	mutex.unlock();
	return;
}
std::string XFFmpeg::GetError()
{
	mutex.lock();
	std::string re = this->errorbuf;
	mutex.unlock();
	return re;
}

AVPacket XFFmpeg::Read()
{
	AVPacket pkt;
	memset(&pkt, 0, sizeof(AVPacket));
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return pkt;
	}
	int re = av_read_frame(ic, &pkt);//读取视频一帧的信息int err = av_read_frame(ic, &pkt);
	if (re != 0)
	{
		av_strerror(re, errorbuf, sizeof(errorbuf));
	}

	if (cCtx)
	{
		sws_freeContext(cCtx);
		cCtx = NULL;
	}
	mutex.unlock();

	return pkt;
}
XFFmpeg::XFFmpeg()
{
	errorbuf[0] = '\0';
	av_register_all();//注册所有的支持的视频格式
	avformat_network_init();//初始化网络流格式,使用RTSP网络流时必须先执行
}
bool XFFmpeg::Seek(float pos)
{
	mutex.lock();
	if (!ic)
	{
		mutex.unlock();
		return false;
	}
	uint64_t timeMap = (float)pos * (float)ic->streams[videoStream]->duration;
	int ret = av_seek_frame(ic, videoStream, timeMap, AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME);
	avcodec_flush_buffers(ic->streams[videoStream]->codec);
	pts = (timeMap *r2d(ic->streams[videoStream]->time_base)) * 1000;
	mutex.unlock();
	if (ret < 0)
		return false;
	return true;
}
bool XFFmpeg::ToRGB(char *out, int outwidth, int outheight)
{
	mutex.lock();
	/*这里需要判断是非视频文件则不处理*/
	if (!ic || !yuv || yuv->linesize[0] <= 0)
	{
		mutex.unlock();
		return false;
	}
	AVCodecContext *videoCtx = ic->streams[videoStream]->codec;
	cCtx = sws_getCachedContext(cCtx, videoCtx->width, videoCtx->height,
		videoCtx->pix_fmt, outwidth, outheight, AV_PIX_FMT_BGRA,
		SWS_BICUBIC, //算法
		NULL, NULL, NULL);
	if (!cCtx)
	{
		mutex.unlock();
		printf("sws_getCachedContext failed!\n");
		return false;
	}
	/* 解码完之后的数据需要进行转换 */
	uint8_t *data[AV_NUM_DATA_POINTERS] = { 0 };
	data[0] = (uint8_t*)out;
	int linesize[AV_NUM_DATA_POINTERS] = { 0 };
	linesize[0] = outwidth * 4;
	int h = sws_scale(cCtx, yuv->data, yuv->linesize, 0, videoCtx->height,
		data, linesize);//返回的是转码时候的height
	mutex.unlock();
	return true;
}
XFFmpeg::~XFFmpeg()
{
}
