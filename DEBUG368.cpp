// 2013.10.28
//****************************************
//**  Copyright  (C)  W.ch  2010-2013   **
//**  Web:  http://www.winchiphead.com  **
//****************************************
//**  EXE for PCIE interface chip CH367 **
//**  C, VC6.0                          **
//****************************************
//
// PCI���߽ӿ�оƬCH367/368��Ӧ�ò���ʾ���� V1.0
// �Ͼ��ߺ�������޹�˾  ����: W.ch 2013.10
// ���л���: Windows 98/ME, Windows 2000/XP/7
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <iostream>
#include <string.h>

#include "resource.h"
#include "CH367DLL.h"
#include "DEBUG368.H"
#pragma comment(lib,"CH367DLL.LIB")

#include "SUB.cpp"


/* ������� */
int	APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	mSaveInstance = hInstance;
	return(DialogBox(hInstance, (LPCTSTR)IDD_MAIN, NULL, mDialogMain)); // �������Ի��� 
}


/* ���Ի����¼� */
INT_PTR CALLBACK mDialogMain(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam)
{
	int IoModel,IoAddrType;
	int MemModel;
	int SPInum,SPIclk,SPImode;
	unsigned char buffer[4];
	switch(uMessage)
	{
	case WM_INTNOTIFY:
		{	
			if(intType==1)
			{
				sprintf(irq_buf, "%d", mCount);
				SetWindowText(GetDlgItem(hDialog, IDC_EDITINT1), irq_buf);
			}
			if(intType==2)
			{
				sprintf(irq_buf, "%d", mCount1);
				SetWindowText(GetDlgItem(hDialog, IDC_EDITINT2), irq_buf);
			}
		}
		return TRUE;
	case WM_INITDIALOG:
		{
			mSaveDialogMain = hDialog;
			mInitCheckBox(hDialog);//��ʼ����ť
			mPreVerify(hDialog);//Ԥ��ɨ����ڼ�����ϵ��豸
			if(LoadLibrary("CH367DLL.DLL") == NULL) // ��Ҫʹ��DLL����Ҫ�ȼ��� 
			{
				MessageBox(hDialog,TEXT( "�޷�����CH367��DLL"), mCaptionInform, MB_ICONSTOP | MB_OK); // ����DLLʧ��,����δ��װ��ϵͳ�� 
				EndDialog(hDialog, 0x81); // �رնԻ��� 
				return TRUE;
			}
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BTNIORD: // I/O�ռ�� 
				{
					if(flag_open)
					{
						IoModel=SendDlgItemMessage(hDialog,IDC_CBIOMODEL,CB_GETCURSEL,0,0);
						IoAddrType=SendDlgItemMessage(hDialog,IDC_CBIOADDTYPE,CB_GETCURSEL,0,0);
						mIoRead(hDialog,IoModel,IoAddrType);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNIOWR: // I/O�ռ�д 
				{
					if(flag_open)
					{
						IoModel=SendDlgItemMessage(hDialog,IDC_CBIOMODEL,CB_GETCURSEL,0,0);
						IoAddrType=SendDlgItemMessage(hDialog,IDC_CBIOADDTYPE,CB_GETCURSEL,0,0);
						mIoWrite(hDialog,IoModel,IoAddrType);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNIOCL://I/O���ݿ����
				{
					SetDlgItemText(hDialog,IDC_EDITIODATA,"");
				}
				return TRUE;
			case IDC_BTNMEMRD://MEM��
				{
					if(flag_open)
					{
						MemModel=SendDlgItemMessage(hDialog,IDC_CBMEMMODEL,CB_GETCURSEL,0,0);
						mMemRead(hDialog,MemModel);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
					return TRUE;
			case IDC_BTNMEMWR://MEMд
				{
					if(flag_open)
					{
						MemModel=SendDlgItemMessage(hDialog,IDC_CBMEMMODEL,CB_GETCURSEL,0,0);
						mMemWrite(hDialog,MemModel);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_CB32://MEM32λ��д֧��
				{
					if(CH368==1)
					{
						UCHAR oByte;
						CH367mReadIoByte(mIndex, &mBaseAddr->mCH367Speed,&oByte);//����IO�Ĵ�����speedλʵ��32λ֧��
						if(IsDlgButtonChecked(hDialog,IDC_CB32)==BST_CHECKED)
						{
							CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,oByte|0x40);
							SendDlgItemMessage(hDialog,IDC_CBMEMMODEL,CB_INSERTSTRING,1, (LPARAM)"˫��");//
						}
						else
						{
							CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,oByte&0x3F);
							SendDlgItemMessage(hDialog,IDC_CBMEMMODEL,CB_DELETESTRING,1,0);
						}
					}
				}
				return TRUE;
			case IDC_BTNMEMCL://MEM���ݿ����
				{
					SetDlgItemText(hDialog,IDC_EDITMEMDATA,"");
				}
				return TRUE;
			case IDC_BTNI2CRD://I2C��
				{
					if(flag_open)
					{
						mI2CRead(hDialog);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNI2CWR://I2Cд
				{
					if(flag_open)
					{
						 mI2CWrite(hDialog);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNPU://����������
				{
					if(flag_open)
					{
						PulwidthSetting();
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNCONRD: // ���ÿռ�� 
				{
					if(flag_open)
					{
						mConRead(hDialog);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNCONWR://���ÿռ�д
				{
					if(flag_open)
					{
						mConWrite(hDialog);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNI2C_PROGRAM: //I2C ���ÿռ��д
				{
					if(flag_open)
					{
						DialogBox(mSaveInstance, (LPCTSTR)IDD_I2C_PROGRAM, NULL, mDialogI2c);
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNSPISET://SPI����
				{
					if(flag_open)
					{
						SPInum=SendDlgItemMessage(hDialog,IDC_CBSPINUM,CB_GETCURSEL,0,0);
						SPIclk=SendDlgItemMessage(hDialog,IDC_CBSPICLK,CB_GETCURSEL,0,0);
						if(SPInum==0&&SPIclk==0)
						{
							SPImode=0x01;
						}
						else if(SPInum==0&&SPIclk==1)
						{
							SPImode=0x00;
						}
						else if(SPInum==1&&SPIclk==0)
						{
							SPImode=0x03;
						}
						else
						{
							SPImode=0x02;
						}
						if(CH367SetStream(mIndex, SPImode))
							MessageBox(hDialog, "����SPI�ɹ�", mCaptionInform, MB_OK);
						else
							MessageBox(hDialog, "����SPIʧ��", mCaptionInform, MB_OK);
					}		
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_FLASH_READ://FLASH��
				{
					read_flash();
				}
				return TRUE;
			case IDC_FLASH_WRITE://FLASHд
				{
					load_file();
					write_flash();
				}
				return TRUE;
			case IDC_FLASH_ERASE: // ����FLASH 
				{
					erase_flash();
				}
				return TRUE;
			case IDC_BTNINT1: // ���Ե�ƽ�ж�
				{		
					intType=1;
					if(!CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367IntCtr,0xe2))//����Ϊ��ƽ�жϣ��͵�ƽ��Ч
					{
						MessageBox(hDialog,"��ƽ�ж�����ʧ�ܣ�","��ʾ",MB_OK|MB_ICONERROR);
						return TRUE;
					}
					if(flag_open)
					{
						MessageBox(hDialog, "��ƽ�ж������ã���ȷ��INT#��A15����,���򽫲�������ж�", mCaptionInform, MB_OK);
						CH367_GPO_SET;//�Ƚ�int#���ø�
						CH367_GPO_CLR;//��int#���õ�
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNINT2://���Ա����ж�
				{
					intType=2;		
					if(!CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367IntCtr,0xea))//�����ж�ģʽΪ�����жϣ���������Ч
					{
						MessageBox(hDialog,"�����ж�����ʧ�ܣ�","��ʾ",MB_OK|MB_ICONERROR);
						return TRUE;
					}
					if(flag_open)
					{
						MessageBox(hDialog,"��ƽ�ж������ã���ȷ��INT#��A15����,���򽫲�������ж�","��ʾ",MB_OK);
						CH367_GPO_SET;//����ģ���������ź�
						CH367_GPO_CLR;
						CH367_GPO_SET;
					}
				}
				return TRUE;
			case IDC_BTNCLOSE: // �ر��豸 
				{
					if(flag_open)
					{
						CH367mCloseDevice(mIndex);
						flag_open=0;
						mCount=0;
						mCount1=0;
						CH367=CH368=0;
						SetDlgItemText(hDialog,IDC_EDITINT1,"");
						SetDlgItemText(hDialog,IDC_EDITINT2,"");
						sprintf(buffer_close, "�豸%d�Ѿ��ر�", mIndex);
						SetDlgItemText(hDialog, IDC_STATUS, buffer_close);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNPU), FALSE); //�������ɼ� 
						EnableWindow(GetDlgItem(hDialog, IDC_BTNIOCL), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNIORD), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNIOWR), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIRD), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIWR), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNI2CWR), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNI2CRD), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNI2C_PROGRAM), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNSPICL), FALSE); 
						EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIRD), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIWR), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNCONRD), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNCONWR), FALSE); 
						EnableWindow(GetDlgItem(hDialog, IDC_BTNINT1), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNINT2), FALSE); 
						EnableWindow(GetDlgItem(hDialog, IDC_BTNCLOSE), FALSE); 
						EnableWindow(GetDlgItem(hDialog, IDC_FLASH_LOADFILE), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_FLASH_READ), FALSE); 
						EnableWindow(GetDlgItem(hDialog, IDC_FLASH_WRITE), FALSE); 
						EnableWindow(GetDlgItem(hDialog, IDC_FLASH_ERASE), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNSPISET), FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_CB32), FALSE);
						EnableWindow(GetDlgItem(hDialog,IDC_STATICMEM),FALSE);
						EnableWindow(GetDlgItem(hDialog,IDC_EDTMEM),FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNMEMCL), FALSE);
						EnableWindow(GetDlgItem(hDialog,IDC_BTNMEMRD),FALSE);
						EnableWindow(GetDlgItem(hDialog,IDC_BTNMEMWR),FALSE);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNOPEN), TRUE);
						EnableWindow(GetDlgItem(hDialog, IDC_DEVICE_NUMBER), TRUE);
						EndDialog(mSaveDialogI2c, 1); // �ر�I2C�Ի��� 
						EndDialog(mSaveDialogHelp, 2); // �ر�I2C�Ի��� 
						SetDlgItemText(hDialog,IDC_EDTINTLINE,"");
						SetDlgItemText(hDialog,IDC_EDITFLASH,"");
						SetDlgItemText(hDialog,IDC_EDTBASEADDR,"");
						SetDlgItemText(hDialog,IDC_EDTMEM,"");
						SetDlgItemText(hDialog,IDC_EDIT,"");
						SetDlgItemText(hDialog,IDC_PULSHOW,"");
						SetDlgItemText(hDialog,IDC_EDITIOADD,"");
						SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
						SetDlgItemText(hDialog,IDC_EDITIODATA,"");
						SetDlgItemText(hDialog,IDC_EDITMEMADD,"");
						SetDlgItemText(hDialog,IDC_EDITMEMLEN,"");
						SetDlgItemText(hDialog,IDC_EDITMEMDATA,"");
						SetDlgItemText(hDialog,IDC_EDITI2CADD,"");
						SetDlgItemText(hDialog,IDC_EDITI2CDATA,"");
						SetDlgItemText(hDialog,IDC_EDITSPILEN,"");
						SetDlgItemText(hDialog,IDC_EDITCONADD,"");
						SetDlgItemText(hDialog,IDC_EDITCONDATA,"");
						SetDlgItemText(hDialog,IDC_EDITINT1,"");
						SetDlgItemText(hDialog,IDC_EDITINT2,"");
						SetDlgItemText(hDialog,IDC_EDTDEVVER,"");
						SetDlgItemText(hDialog,IDC_FILE,"");
						SendMessage(GetDlgItem(hDialog,IDC_DEVICE_NUMBER),CB_SETCURSEL,0,0);
						SendMessage(GetDlgItem(hDialog,IDC_CBPU),CB_SETCURSEL,0,0);
						SendMessage(GetDlgItem(hDialog,IDC_CBIOMODEL),CB_SETCURSEL,0,0);
						SendMessage(GetDlgItem(hDialog,IDC_CBIOADDTYPE),CB_SETCURSEL,0,0);
						SendMessage(GetDlgItem(hDialog,IDC_CBMEMMODEL),CB_SETCURSEL,0,0);
						SendMessage(GetDlgItem(hDialog,IDC_CBSPINUM),CB_SETCURSEL,0,0);
						SendMessage(GetDlgItem(hDialog,IDC_CBSPICLK),CB_SETCURSEL,0,0);
					}
					else
					{
						sprintf(buffer_close, "�豸%dû�д�", mIndex);
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNOPEN: // ���豸 
				{
					UCHAR mByte;
					mCount=0;
					mCount1=0;	
					if(!flag_open)
					{
						mIndex = (UCHAR)SendDlgItemMessage(hDialog, IDC_DEVICE_NUMBER, CB_GETCURSEL, 0, 0); // ���Ҫ�򿪵��豸�� 
						if(CH367mOpenDevice(mIndex, FALSE, TRUE, 0x00) == INVALID_HANDLE_VALUE) //  ʹ���ж�, ��ƽ�ж�ģʽ, �͵�ƽ���� 
						{
							sprintf(buffer_open, "�޷����豸%d,��ȷ���豸�Ѿ�����", mIndex);
							MessageBox(hDialog, buffer_open, mCaptionInform, MB_ICONSTOP | MB_OK);
							return( TRUE );
						}
						else
						{
							CH367mReadConfig(mIndex,(PVOID)0x02, &mByte);//��ȡ�豸�����ÿռ�����ȷ����ʲô�豸
							if(mByte==0x34)////0x34��ʾCH368�豸,0x31����0x30��ʾCH367,������������豸��ʶ,���豸�����üĴ����е�02H�ռ䣬�뽫�����и�������ı�ʶ
							{
								CH368=1;
								CH367=0;
							}
							else if(mByte==0x31||mByte==0x30)
							{
								CH367=1;
								CH368=0;
							}
							else
							{
								MessageBox(hDialog,"δ��ʶ�������豸,������������豸��ʶ,���豸�����üĴ����е�02H�ռ�,�뽫��������Ӧ�Ĵ������һ��","��ʾ",MB_OK);
								CH367mCloseDevice(mIndex);
								return FALSE;
							}
							if(CH368==1)//�����CH368�豸�����´�֧�ִ洢����д
							{
								CH367mCloseDevice(mIndex);
								CH367mOpenDevice(mIndex, TRUE, TRUE, 0x00);//���´�CH368�豸������֧��Mem
								EnableWindow(GetDlgItem(hDialog, IDC_EDTMEM), TRUE);
								EnableWindow(GetDlgItem(hDialog, IDC_BTNMEMRD), TRUE);
								EnableWindow(GetDlgItem(hDialog, IDC_BTNMEMWR), TRUE);
								EnableWindow(GetDlgItem(hDialog, IDC_BTNMEMCL), TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_STATICMEM),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_EDTMEM),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_CB32),TRUE);
								CheckDlgButton(hDialog,IDC_CB32,TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_STATICMEM),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_EDTMEM),TRUE);
							}
							flag_open = 0x01;
							mShowDevVer(hDialog); // ��ʾ�����汾�� 
							AddrRefresh(hDialog);
							CH367mSetIntRoutine(mIndex, InterruptEvent); // �����жϷ������
							if(CH367==1)
							{
								sprintf(buffer_open, "CH367�豸%d�Ѿ���", mIndex);
							}
							if(CH368==1)
							{
								sprintf(buffer_open, "CH368�豸%d�Ѿ���", mIndex);
							}
							SetDlgItemText(hDialog, IDC_STATUS, buffer_open);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNPU), TRUE); // �����ɼ� 
							EnableWindow(GetDlgItem(hDialog, IDC_BTNIOCL), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNIORD), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNIOWR), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIRD), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIWR), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNI2CWR), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNI2CRD), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNI2C_PROGRAM), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNSPICL), TRUE); 
							EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIRD), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNSPIWR), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNCONRD), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNCONWR), TRUE); 
							EnableWindow(GetDlgItem(hDialog, IDC_BTNINT1), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNINT2), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNCLOSE),TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNSPISET), TRUE);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNOPEN), FALSE);
							EnableWindow(GetDlgItem(hDialog, IDC_DEVICE_NUMBER), FALSE);
							read_manufacturer_device_id(buffer);//���FLASH�Ƿ�����
							if(buffer[0] == 0xBF && buffer[1] == 0x48 && buffer[2] == 0xBF && buffer[3] == 0x48)
							{	
								SetDlgItemText(hDialog,IDC_EDITFLASH,"FLASH״̬����");
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_READ),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_WRITE),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_ERASE),TRUE);	
							}
							else
							{
								SetDlgItemText(hDialog,IDC_EDITFLASH,"FLASH״̬����");
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_READ),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_WRITE),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_ERASE),TRUE);	
							}
						}
					
					}
					else
					{
						MessageBox(hDialog, buffer_open, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNHELP: // ����˵��
				{
					DialogBox(mSaveInstance, (LPCTSTR)IDD_HELP, NULL, mDialogHelp);
				}			
			return TRUE;
			case IDC_BTNEXIT:
			case WM_DESTROY: // �˳� 
				CH367mCloseDevice(mIndex);
				EndDialog(hDialog, 1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;  
}

