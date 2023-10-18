#pragma once
#include  "LAppDelegate.hpp"
enum  MotionType
{
	BIGSMILE =0,
	BIGSAD =1,
	BIGAFARID =2,
	BIGBLINKLEFT =3,
	BIGBLINKRIGHT =4,
	BIGSERIOUS = 5
};
class LAppInterface
{
public:
	LAppInterface();
	~LAppInterface();
    /**
   * @brief   返回一个类的实例（单个）
   *           如果未生成实例，则在内部生成实例
   *
   * @return  类实例
   */
public:
	 static LAppInterface* GetInstance();
    /**
   * @brief   释放类实例
   *
   */
    static void ReleaseInstance();

	/*
	* @brief 设置动画
	*/
	 void SetMotion(MotionType motiontype);
	/*
	* @brief	初始化分配器
	*/
	void InitializeDelegate();
private:
	MotionType current_motiontype = BIGSMILE;
};

