#ifndef		_DEBUG368_H
#define		_DEBUG368_H

#ifdef __cplusplus
extern "C" {
#endif

HINSTANCE mSaveInstance; /* ʵ�� */
HWND mSaveDialogMain; /* ���Ի��� */
HWND mSaveDialogI2c; /* I2C�Ի��� */
HWND mSaveDialogHelp; /* HELP�Ի��� */

CHAR mCaptionInform[] = "��ʾ";
UINT mIndex = 0; /* �豸��� */
CHAR flag_open = 0; /* �豸���ر�־ */
mPCH367_IO_REG mBaseAddr; /* I/O�ռ�Ĵ��� */
mPCH368_MEM_REG mMemAddr;/*Mem�ռ�Ĵ���*/

char buffer_open[16];
char buffer_close[16];
int CH367=0;
int CH368=0;
int intType;//�ж��ǵ�ƽ�жϣ�1�����Ǳ����жϣ�2��
int Devices[5][2];//���ڴ洢�����е������豸

WNDPROC WindowProcedureI2cDid,WindowProcedureI2cVid,WindowProcedureI2cRid,WindowProcedureI2cSdid,\
		WindowProcedureI2cSvid,WindowProcedureIoAdd,WindowProcedureIoLen,WindowProcedureIoData,\
		WindowProcedureMemAdd,WindowProcedureMemLen,WindowProcedureMemData,WindowProcedureI2CAdd,\
		WindowProcedureI2cData,WindowProcedureConAdd,WindowProcedureConData;

typedef struct _VAR_TYPE {
	union {
		UCHAR cVar;
		UINT iVar;
		USHORT sVar;
		ULONG lVar;
	};
}mVAR_TYPE, *mpVAR_TYPE;

/* �����볣������,�����25F512оƬ˵���� */
#define WREN		0x06 /* дʹ��ָ�� */
#define WRDI		0x04 /* д��ָֹ�� */
#define EWSR		0x50 /* д״̬�Ĵ���ʹ��ָ�� */
#define RDSR		0x05 /* ��״̬�Ĵ���ָ�� */
#define WRSR		0x01 /* д״̬�Ĵ���ָ��,WELΪ0������д */
#define READ		0x03 /* ���ֽ�ָ�� */
#define FAST_READ	0x0B /* ���ָ�� */
#define BP			0x02 /* �ֽ�дָ�� */
#define AAI			0xAF /* ����дָ�� */
#define SE			0x20 /* ��������ָ�� */
#define BE			0xD8 /* �����ָ�� */
//#define CE			0xC7 /* оƬ����ָ��*/
#define CE			0x60 /* оƬ����ָ��*/
#define RMDI        0x90 /* ������/�豸IDָ�� */

#define SPI_BUFFER_LENGTH 0x1000 /* ÿ�ζ�4096���ֽ� */
CHAR SpiFilePath[256] = ""; /* ������ļ�·�� */


unsigned char qbyte = 0;
#define CH367_GPO_SET do{CH367mReadIoByte(mIndex, &mBaseAddr->mCH367IoPort[0xF1], &qbyte); CH367mWriteIoByte(mIndex, &mBaseAddr->mCH367IoPort[0xF1], qbyte |= 0x80);}while(0)
#define CH367_GPO_CLR do{CH367mReadIoByte(mIndex, &mBaseAddr->mCH367IoPort[0xF1], &qbyte); CH367mWriteIoByte(mIndex, &mBaseAddr->mCH367IoPort[0xF1], qbyte &= 0x7F);}while(0)
#define WM_INTNOTIFY (WM_USER + 1)
char irq_buf[100] = "";
ULONG mCount = 0;//��ƽ�жϼ���
ULONG mCount1=0;//�����жϼ���
UCHAR PulWidth[]={0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a,0x4b,0x4c,0x4d,0x4e};//����������
UCHAR CurPulWidth;//��ǰ������

INT_PTR CALLBACK mDialogMain(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam); // ���Ի����¼� 
INT_PTR CALLBACK mDialogI2c(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam); // I2C�Ի����¼� 
INT_PTR CALLBACK mDialogHelp(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam); // HELP�Ի����¼� 
void CALLBACK InterruptEvent(void); // �豸�ж�֪ͨ��Ϣ 
void AddrRefresh(HWND hDialog); // ����ȡ��ַ
void mSetI2C(HWND hDialog); // I2C��д
void mIoWrite(HWND hDialog,int IoModel,int IoAddr);//IO������
void mIoRead(HWND hDialog,int IoModel,int IoAddr);//IOд����
void mMemRead(HWND hDialog,int MemModel);//MEM������
void mMemWrite(HWND hDialog,int MemModel);//MEM������
void mConRead(HWND hDialog);//���ÿռ��ֽڶ�
void mConWrite(HWND hDialog);//���ÿռ��ֽ�д
void mI2CRead(HWND hDialog);//I2C��
void mI2CWrite(HWND hDialog);//I2Cд
void mInitCheckBox(HWND hDialog); // ��ʼ��Ĭ��ѡ�е�ѡ�� 
void mShowDevVer(HWND hDialog); //��ʾ�����汾��


void mPreVerify(HWND hwnd);

#ifdef __cplusplus
}
#endif

#endif // _DEBUG367_H