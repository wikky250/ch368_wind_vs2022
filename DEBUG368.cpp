// 2013.10.28
//****************************************
//**  Copyright  (C)  W.ch  2010-2013   **
//**  Web:  http://www.winchiphead.com  **
//****************************************
//**  EXE for PCIE interface chip CH367 **
//**  C, VC6.0                          **
//****************************************
//
// PCI总线接口芯片CH367/368的应用层演示程序 V1.0
// 南京沁恒电子有限公司  作者: W.ch 2013.10
// 运行环境: Windows 98/ME, Windows 2000/XP/7
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


/* 程序入口 */
int	APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nShowCmd)
{
	mSaveInstance = hInstance;
	return(DialogBox(hInstance, (LPCTSTR)IDD_MAIN, NULL, mDialogMain)); // 调用主对话框 
}


/* 主对话框事件 */
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
			mInitCheckBox(hDialog);//初始化按钮
			mPreVerify(hDialog);//预先扫描插在计算机上的设备
			if(LoadLibrary("CH367DLL.DLL") == NULL) // 需要使用DLL则需要先加载 
			{
				MessageBox(hDialog,TEXT( "无法加载CH367的DLL"), mCaptionInform, MB_ICONSTOP | MB_OK); // 加载DLL失败,可能未安装到系统中 
				EndDialog(hDialog, 0x81); // 关闭对话框 
				return TRUE;
			}
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_BTNIORD: // I/O空间读 
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
			case IDC_BTNIOWR: // I/O空间写 
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
			case IDC_BTNIOCL://I/O数据框清除
				{
					SetDlgItemText(hDialog,IDC_EDITIODATA,"");
				}
				return TRUE;
			case IDC_BTNMEMRD://MEM读
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
			case IDC_BTNMEMWR://MEM写
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
			case IDC_CB32://MEM32位读写支持
				{
					if(CH368==1)
					{
						UCHAR oByte;
						CH367mReadIoByte(mIndex, &mBaseAddr->mCH367Speed,&oByte);//控制IO寄存器的speed位实现32位支持
						if(IsDlgButtonChecked(hDialog,IDC_CB32)==BST_CHECKED)
						{
							CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,oByte|0x40);
							SendDlgItemMessage(hDialog,IDC_CBMEMMODEL,CB_INSERTSTRING,1, (LPARAM)"双字");//
						}
						else
						{
							CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,oByte&0x3F);
							SendDlgItemMessage(hDialog,IDC_CBMEMMODEL,CB_DELETESTRING,1,0);
						}
					}
				}
				return TRUE;
			case IDC_BTNMEMCL://MEM数据框清除
				{
					SetDlgItemText(hDialog,IDC_EDITMEMDATA,"");
				}
				return TRUE;
			case IDC_BTNI2CRD://I2C读
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
			case IDC_BTNI2CWR://I2C写
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
			case IDC_BTNPU://脉冲宽度设置
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
			case IDC_BTNCONRD: // 配置空间读 
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
			case IDC_BTNCONWR://配置空间写
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
			case IDC_BTNI2C_PROGRAM: //I2C 配置空间读写
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
			case IDC_BTNSPISET://SPI设置
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
							MessageBox(hDialog, "设置SPI成功", mCaptionInform, MB_OK);
						else
							MessageBox(hDialog, "设置SPI失败", mCaptionInform, MB_OK);
					}		
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_FLASH_READ://FLASH读
				{
					read_flash();
				}
				return TRUE;
			case IDC_FLASH_WRITE://FLASH写
				{
					load_file();
					write_flash();
				}
				return TRUE;
			case IDC_FLASH_ERASE: // 擦除FLASH 
				{
					erase_flash();
				}
				return TRUE;
			case IDC_BTNINT1: // 测试电平中断
				{		
					intType=1;
					if(!CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367IntCtr,0xe2))//设置为电平中断，低电平有效
					{
						MessageBox(hDialog,"电平中断设置失败！","提示",MB_OK|MB_ICONERROR);
						return TRUE;
					}
					if(flag_open)
					{
						MessageBox(hDialog, "电平中断已启用，请确保INT#与A15连接,否则将不会产生中断", mCaptionInform, MB_OK);
						CH367_GPO_SET;//先将int#脚置高
						CH367_GPO_CLR;//将int#脚置低
					}
					else
					{
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNINT2://测试边沿中断
				{
					intType=2;		
					if(!CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367IntCtr,0xea))//设置中断模式为边沿中断，上升沿有效
					{
						MessageBox(hDialog,"边沿中断设置失败！","提示",MB_OK|MB_ICONERROR);
						return TRUE;
					}
					if(flag_open)
					{
						MessageBox(hDialog,"电平中断已启用，请确保INT#与A15连接,否则将不会产生中断","提示",MB_OK);
						CH367_GPO_SET;//用于模拟上升沿信号
						CH367_GPO_CLR;
						CH367_GPO_SET;
					}
				}
				return TRUE;
			case IDC_BTNCLOSE: // 关闭设备 
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
						sprintf(buffer_close, "设备%d已经关闭", mIndex);
						SetDlgItemText(hDialog, IDC_STATUS, buffer_close);
						EnableWindow(GetDlgItem(hDialog, IDC_BTNPU), FALSE); //按键不可见 
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
						EndDialog(mSaveDialogI2c, 1); // 关闭I2C对话框 
						EndDialog(mSaveDialogHelp, 2); // 关闭I2C对话框 
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
						sprintf(buffer_close, "设备%d没有打开", mIndex);
						MessageBox(hDialog, buffer_close, mCaptionInform, MB_OK);
					}
				}
				return TRUE;
			case IDC_BTNOPEN: // 打开设备 
				{
					UCHAR mByte;
					mCount=0;
					mCount1=0;	
					if(!flag_open)
					{
						mIndex = (UCHAR)SendDlgItemMessage(hDialog, IDC_DEVICE_NUMBER, CB_GETCURSEL, 0, 0); // 获得要打开的设备号 
						if(CH367mOpenDevice(mIndex, FALSE, TRUE, 0x00) == INVALID_HANDLE_VALUE) //  使能中断, 电平中断模式, 低电平触发 
						{
							sprintf(buffer_open, "无法打开设备%d,请确保设备已经插入", mIndex);
							MessageBox(hDialog, buffer_open, mCaptionInform, MB_ICONSTOP | MB_OK);
							return( TRUE );
						}
						else
						{
							CH367mReadConfig(mIndex,(PVOID)0x02, &mByte);//读取设备的配置空间用以确定是什么设备
							if(mByte==0x34)////0x34表示CH368设备,0x31或者0x30表示CH367,如果您更改了设备标识,即设备的配置寄存器中的02H空间，请将程序中更改这里的标识
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
								MessageBox(hDialog,"未能识别您的设备,如果您更改了设备标识,即设备的配置寄存器中的02H空间,请将程序中相应的代码更改一下","提示",MB_OK);
								CH367mCloseDevice(mIndex);
								return FALSE;
							}
							if(CH368==1)//如果是CH368设备则重新打开支持存储器读写
							{
								CH367mCloseDevice(mIndex);
								CH367mOpenDevice(mIndex, TRUE, TRUE, 0x00);//重新打开CH368设备并设置支持Mem
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
							mShowDevVer(hDialog); // 显示驱动版本号 
							AddrRefresh(hDialog);
							CH367mSetIntRoutine(mIndex, InterruptEvent); // 设置中断服务程序
							if(CH367==1)
							{
								sprintf(buffer_open, "CH367设备%d已经打开", mIndex);
							}
							if(CH368==1)
							{
								sprintf(buffer_open, "CH368设备%d已经打开", mIndex);
							}
							SetDlgItemText(hDialog, IDC_STATUS, buffer_open);
							EnableWindow(GetDlgItem(hDialog, IDC_BTNPU), TRUE); // 按键可见 
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
							read_manufacturer_device_id(buffer);//检测FLASH是否正常
							if(buffer[0] == 0xBF && buffer[1] == 0x48 && buffer[2] == 0xBF && buffer[3] == 0x48)
							{	
								SetDlgItemText(hDialog,IDC_EDITFLASH,"FLASH状态正常");
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_READ),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_WRITE),TRUE);
								EnableWindow(GetDlgItem(hDialog,IDC_FLASH_ERASE),TRUE);	
							}
							else
							{
								SetDlgItemText(hDialog,IDC_EDITFLASH,"FLASH状态正常");
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
			case IDC_BTNHELP: // 帮助说明
				{
					DialogBox(mSaveInstance, (LPCTSTR)IDD_HELP, NULL, mDialogHelp);
				}			
			return TRUE;
			case IDC_BTNEXIT:
			case WM_DESTROY: // 退出 
				CH367mCloseDevice(mIndex);
				EndDialog(hDialog, 1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;  
}

