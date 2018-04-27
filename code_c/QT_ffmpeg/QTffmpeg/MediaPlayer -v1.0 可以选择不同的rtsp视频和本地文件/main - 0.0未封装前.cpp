#include "mediaplayer.h"
#include <QtWidgets/QApplication>
#if 0
static double r2d(AVRational r)
{
	return r.num == 0 || r.den == 0 ? 0 : (double)r.num/(double)r.den;
}
int main(int argc, char *argv[])
{
	av_register_all();//ע�����е�֧�ֵ���Ƶ��ʽ
	char *path = "video.mp4";
	AVFormatContext *ic = NULL;//��Ƶ��Ϣ���ļ����
	int re = avformat_open_input(&ic, path, 0, 0);//��һ����Ƶ�ļ���
	if (re != 0)
	{
		char buf[1024] = { 0 };
		av_strerror(re, buf, sizeof(buf));//�����صĴ����룬ת���ɾ���Ĵ�����Ϣ
		printf("open %s failed: %s\n", path, buf);
		getchar();

		return -1;
	}
	int totalSec = ic->duration / AV_TIME_BASE; //��Ƶ����ʱ��
	printf("file totalSec is %d-%d\n", totalSec / 60, totalSec % 60);
	/* һ����Ƶ�򿪣����ܻ��м���������Ƶ������Ƶ������Ļ�� */
	int videoStream = 0;
	AVCodecContext *videoCtx;//������Ƶ����Ϣ
	for (int i = 0; i < ic->nb_streams; i++)
	{
		AVCodecContext *enc = ic->streams[i]->codec;
		printf("i = %d, type = %d\n", i, enc->codec_type);//��ý�������  0��ʾvideo 1��ʾaudio
		if (enc->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			videoCtx = enc;
			videoStream = i;
			AVCodec *codec = avcodec_find_decoder(enc->codec_id);//����codec_id����ϵͳ���ǲ����д˽�����
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
	SwsContext *cCtx = NULL;//����������
	int outWidth = 640;
	int outHeight = 480;
	char *rgb = new char[outWidth*outHeight*4];

	for (;;)
	{
		AVPacket pkt;
		re = av_read_frame(ic, &pkt);//��ȡ��Ƶһ֡����Ϣ
		if (re != 0)break;//re=0��ʱ���ʾ��Ƶ֡��Ϣ��ȡ����
		if (pkt.stream_index != videoStream)//��ǰ֡����Ƿ���Ƶ��Ϣ�Ļ���continue
		{
			av_packet_unref(&pkt);
			continue;
		}
		/* pts��int64 ��AVStream-> time_base��λ��ʾʱ���; ���ĸ�ʱ���ѹ������ݰ������ָ��û���
		   dts��AVStream-> time_base��λ�Ľ�ѹ��ʱ���; ���ĸ�ʱ�����ݰ�����ѹ�� 
		   �ؼ�֡��һ֡���ݵ�������Ϣ
		   p֡������ڹؼ�֡�ģ��������һ֡������
		   b֡����һ֡����һ֡�ı仯 */
		int pts = pkt.pts * r2d(ic->streams[pkt.stream_index]->time_base)*1000;
		//printf("pts = %d\n", pts);//��ӡʱ������
		/* �±������ַ�ʽ����ÿһ֡�����ݽ��н��� */
		/* ��ʽһ�� */

		AVFrame *yuv = av_frame_alloc();//alloc��Ϊ����������ռ䣬�������Ƿ���Ľ���֮�����Ƶ��Ϣ�ֿռ�
		
#if 1
		int got_pic = 0;
		int ret = 0;
		ret = avcodec_decode_video2(videoCtx, yuv, &got_pic, &pkt);//����ֵret���ص��ǽ���֮���֡��С��Ϊ0��ʾ����ʧ�ܣ������������ǳ��Σ�Ϊ1��ʾ����ɹ�
		if (!got_pic)
		{
			char buf[1024] = { 0 };
			av_strerror(re, buf, sizeof(buf));
			printf("decodec failed:%s!\n", buf);
			av_packet_unref(&pkt);
			continue;
		}
		/* yuv����Ҫ�ͷţ����ǲ�û�����������������Ƶ�Ŀռ䣬��avcodec_decode_video2��ϵͳ��֤���ڴ������ */
#endif
#if 0
		/* ��ʽ���� */
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
			SWS_BICUBIC, //�㷨
			NULL, NULL, NULL);
		if (!cCtx)
		{
			printf("sws_getCachedContext failed!\n");
			break;
		}
		/* ������֮���������Ҫ����ת�� */
		uint8_t *data[AV_NUM_DATA_POINTERS] = { 0 };
		data[0] = (uint8_t*)rgb;
		int linesize[AV_NUM_DATA_POINTERS] = { 0 };
		linesize[0] = outWidth * 4;
		int h = sws_scale(cCtx, yuv->data, yuv->linesize, 0, videoCtx->height,
			data, linesize);//���ص���ת��ʱ���height
		//printf("%d\n", h);
		av_packet_unref(&pkt);
	}
	if (cCtx)
	{
		sws_freeContext(cCtx);
		cCtx = NULL;
	}
	avformat_close_input(&ic);//�ر���Ƶ��Ϣ�����ؼ����
#endif
	QApplication a(argc, argv);
	MediaPlayer w;
	w.show();
	return a.exec();
}
