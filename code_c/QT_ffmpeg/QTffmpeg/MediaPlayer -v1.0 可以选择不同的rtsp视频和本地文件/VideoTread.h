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
	void run();//重载基类函数
	VideoTread();
	~VideoTread();
};

