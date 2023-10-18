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
   * @brief   ����һ�����ʵ����������
   *           ���δ����ʵ���������ڲ�����ʵ��
   *
   * @return  ��ʵ��
   */
public:
	 static LAppInterface* GetInstance();
    /**
   * @brief   �ͷ���ʵ��
   *
   */
    static void ReleaseInstance();

	/*
	* @brief ���ö���
	*/
	 void SetMotion(MotionType motiontype);
	/*
	* @brief	��ʼ��������
	*/
	void InitializeDelegate();
private:
	MotionType current_motiontype = BIGSMILE;
};

