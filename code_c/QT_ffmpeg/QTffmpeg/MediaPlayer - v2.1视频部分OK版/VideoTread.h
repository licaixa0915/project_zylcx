#pragma once
#include <QThread>
class VideoTread :public QThread
{
public:
	static VideoTread *Get()
	{
		static VideoTread vt;
		return &vt;
	}
	void run();//���ػ��ຯ��
	VideoTread();
	~VideoTread();
};

