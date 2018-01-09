#include "mediaplayer.h"
#include <QtWidgets/QApplication>
#if 0
static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0 : (double)r.num/(double)r.den;
}
int main(int argc, char *argv[])
{
	av_register_all();//注册所有的支持的视频格式
	char *path = "video.mp4";
	AVFormatContext *ic = NULL;//视频信息的文件句柄
	int re = avformat_open_input(&ic, path, 0, 0);//打开一个视频文件流
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));//将返回的错误码，转换成具体的错误信息
		printf("open %s failed: %s\n", path, buf);
		getchar();

		return -1;
	}
	int totalSec = ic->duration / AV_TIME_BASE; //视频的总时常
	printf("file totalSec is %d-%d\n", totalSec / 60, totalSec % 60);
	/* 一个视频打开，可能会有几个流，视频流，音频流，字幕流 */
	int videoStream = 0;
	AVCodecContext *videoCtx;//保存视频流信息
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVCodecContext *enc = ic->streams[i]->codec;
		printf("i = %d, type = %d\n", i, enc->codec_type);//多媒体的类型  0表示video 1表示audio
		if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoCtx = enc;
			videoStream = i;
			AVCodec *codec = avcodec_find_decoder(enc->codec_id);//根据codec_id查找系统中是不是有此解码器
			if (!codec)
			{
				printf("video code not find\n");
				return -1;
			}
			int err = avcodec_open2(enc, codec, NULL);
			if (err != 0)
			{
				char buf[1024] = { 0 };
				av_strerror(err,buf,sizeof(buf));
				printf("call avcodec_open2 error: %s", buf);
			}
			printf("open codec success!\n");
		}
	}
	SwsContext *cCtx = NULL;//解码器容器
	int outWidth = 640;
	int outHeight = 480;
	char *rgb = new char[outWidth*outHeight*4];

	for (;;)
	{
		AVPacket pkt;
		re = av_read_frame(ic, &pkt);//读取视频一帧的信息
		if (re != 0)break;//re=0的时候表示视频帧信息读取结束
		if (pkt.stream_index != videoStream)//当前帧如果是非视频信息的话，continue
		{
			av_packet_unref(&pkt);
			continue;
		}
		/* pts是int64 以AVStream-> time_base单位显示时间戳; 在哪个时间解压后的数据包将呈现给用户。
		   dts是AVStream-> time_base单位的解压缩时间戳; 在哪个时间数据包被解压。 
		   关键帧：一帧数据的所有信息
		   p帧：针对于关键帧的，是针对上一帧的区别
		   b帧：上一帧和下一帧的变化 */
		int pts = pkt.pts * r2d(ic->streams[pkt.stream_index]->time_base)*1000;
		//printf("pts = %d\n", pts);//打印时间序列
		/* 下边是两种方式来对每一帧的数据进行解码 */
		/* 方式一： */

		AVFrame *yuv = av_frame_alloc();//alloc是为这个对象分配空间，并并不是分配的解码之后的视频信息分空间
		
#if 1
		int got_pic = 0;
		int ret = 0;
		ret = avcodec_decode_video2(videoCtx, yuv, &got_pic, &pkt);//返回值ret返回的是解码之后的帧大小，为0表示解码失败，第三个参数是出参，为1表示解码成功
		if (!got_pic)
		{
			char buf[1024] = { 0 };
			av_strerror(re, buf, sizeof(buf));
			printf("decodec failed:%s!\n", buf);
			av_packet_unref(&pkt);
			continue;
		}
		/* yuv不需要释放，我们并没有真正的申请解码视频的空间，在avcodec_decode_video2中系统保证了内存的清理 */
#endif
#if 0
		/* 方式二： */
		int re = avcodec_send_packet(videoCtx, &pkt);
		if (re != 0)
		{
			char buf[1024] = {0};
			av_strerror(re, buf, sizeof(buf));
			//printf("send failed:%s!\n", buf);
			av_packet_unref(&pkt);
			continue;
		}
		else
		{
			//printf("send ok!\n");
		}
		re = avcodec_receive_frame(videoCtx, yuv);
		if (re != 0)
		{
			char buf[1024] = { 0 };
			av_strerror(re, buf, sizeof(buf));
			//printf("receive failed:%s!\n", buf);
			av_packet_unref(&pkt);
			continue;
		}
		else
		{
			//printf("receive ok!\n");
		}

#endif		
		cCtx = sws_getCachedContext(NULL, videoCtx->width, videoCtx->height,
			videoCtx->pix_fmt, outWidth, outHeight, AV_PIX_FMT_BGRA,
			SWS_BICUBIC, //算法
			NULL, NULL, NULL);
		if (!cCtx)
		{
			printf("sws_getCachedContext failed!\n");
			break;
		}
		/* 解码完之后的数据需要进行转换 */
		uint8_t *data[AV_NUM_DATA_POINTERS] = { 0 };
		data[0] = (uint8_t*)rgb;
		int linesize[AV_NUM_DATA_POINTERS] = { 0 };
		linesize[0] = outWidth * 4;
		int h = sws_scale(cCtx, yuv->data, yuv->linesize, 0, videoCtx->height,
			data, linesize);//返回的是转码时候的height
		//printf("%d\n", h);
		av_packet_unref(&pkt);
	}
	if (cCtx)
	{
		sws_freeContext(cCtx);
		cCtx = NULL;
	}
	avformat_close_input(&ic);//关闭视频信息的我呢见句柄
#endif
	QApplication a(argc, argv);
	MediaPlayer w;
	w.show();
	return a.exec();
}
