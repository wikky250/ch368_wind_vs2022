
/*============================= 基础的字符处理函数 ==============================*/

LRESULT CALLBACK MyEditProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) //处理Edit回调,只能接收'0'-'9','a'-'f','A'-'F','del' 
{
	WNDPROC OldProc = (WNDPROC)GetWindowLong(hWnd, GWL_USERDATA);
	if(Msg == WM_CHAR)
	{
		if(!( ( wParam >= '0' && wParam <= '9') || ( wParam >= 'a' && wParam <= 'f' ) || ( wParam >= 'A' && wParam <= 'F' ) || wParam == 0x08 ) )
		{
			return 0;
		} 
	}
	return CallWindowProc(OldProc, hWnd, Msg, wParam, lParam); 
}

BOOL mCheckInput(UCHAR *input)//检查用户输入的是否是'0~9','a~f','A~F'之间的十六进制字符
{
	int i=0;
	do
	{
		if(((input[i]>='0'&&input[i]<='9')||(input[i]>='a'&&input[i]<='f')||(input[i]>='A'&&input[i]<='F')))
		{
			
		}
		else
		{
			return FALSE;
		}
		i++;
	}
	while(input[i]!='\0');
	return TRUE;
}

UCHAR mCharToBcd(UCHAR iChar) // 将ASCII字符转换为一位BCD码,有效字符是0-9,A-F,a-f,无效字符返回0FFH 
{
	UCHAR mBCD;
	if ( iChar >= '0' && iChar <= '9' ) mBCD = iChar - '0';
	else if ( iChar >= 'A' && iChar <= 'F' ) mBCD = iChar - 'A' + 0x0a;
	else if ( iChar >= 'a' && iChar <= 'f' ) mBCD = iChar - 'a' + 0x0a;
	else 
	{
		mBCD=0xFF;
	}
	return( mBCD );
}

mVAR_TYPE mCharToVar(UCHAR *sChar, UINT sLen, UINT sFlag) // 将字符串转换成数值返回 
{
	UINT i;
	mVAR_TYPE mVarType;
	if(sLen != 0)
	{
		if(sLen > 1) //字符串长度大于1 
		{
			if(sFlag == 1) //sFlag为1表示返回ULONG型 
			{
				ULONG lReturn = 0;
				for(i = 0; i < sLen; i++)	
				{
					lReturn <<= 4;
					lReturn |= mCharToBcd(sChar[i]); 
				}
				mVarType.lVar = lReturn;
			}
			else if(sFlag == 0) //sFlag为0表示返回USHORT型 
			{
				USHORT sReturn = 0;
				for(i = 0; i < sLen; i++)
				{
					sReturn <<= 4;
					sReturn |= mCharToBcd(sChar[i]);
				}
				mVarType.sVar = sReturn;
			}
			else if(sFlag == 2) //sFlag为2表示返回UINT型 
			{
				UINT iReturn = 0;
				for(i = 0; i < sLen; i++)
				{
					iReturn <<= 4;
					iReturn |= mCharToBcd(sChar[i]);
				}
				mVarType.iVar = iReturn;
			}
			else if(sFlag == 3)
			{
				UCHAR cReturn = 0;
				cReturn = (mCharToBcd(sChar[0]) << 4) | mCharToBcd(sChar[1]);
				mVarType.cVar = cReturn;
			}
		}
		else //字符串长度等于1 
		{
			mVarType.lVar = mCharToBcd(sChar[0]);
			mVarType.sVar = mCharToBcd(sChar[0]);
			mVarType.iVar = mCharToBcd(sChar[0]);
			mVarType.cVar = mCharToBcd(sChar[0]);
		}
	}
	else
	{
		mVarType.iVar = 0;
		mVarType.lVar = 0;
		mVarType.sVar = 0;
		mVarType.cVar = 0;
	}
	return mVarType;
}

PUCHAR mStrtoVal(PUCHAR str, ULONG strlen)//将一串16进制数，每两位打包成一个字节
{
	ULONG i,j;
	ULONG len;
	UCHAR strRev[mMAX_BUFFER_LENGTH];
	if( strlen % 2 != 0 )
	{  	
		str[strlen] = 0;
		strlen += 1;
	}
	len = strlen / 2;
	for(i=0,j=0;j<len;i++,j++)
	{
		strRev[j] = (UCHAR)((mCharToBcd(str[i])<<4) + mCharToBcd(str[i+1]));
		i++;
	}
	strRev[j]='\0';
	memcpy(str,strRev,len);
	return str;
}

UINT mStrToBcd(UCHAR* str)//将一串字符转化为UINT类型的数
{   
	char mlen,i=0;
	UCHAR iChar=0,Char[9]="";
	UINT mBCD=0,de=1;
	mlen=strlen((char*)str);
	memcpy(Char,str,mlen);
	for(i=mlen-1;i>=0;i--)
	{	
		iChar=Char[i];
		if ( iChar >= '0' && iChar <= '9' )
			mBCD = mBCD+(iChar -'0')*de;
		else if ( iChar >= 'A' && iChar <= 'F' ) 
			mBCD =mBCD+ (iChar - 'A' + 0x0a)*de;
		else if ( iChar >= 'a' && iChar <= 'f' )
			mBCD =mBCD+ (iChar - 'a' + 0x0a)*de;
		else return(0);
			de*=16;
	}
	return(mBCD);
}


BOOL mCheckWord(PVOID mWord, int dFlag) //检测地址是否能存储字或双字 
{
	if(dFlag == 4)
	{	
		if(((USHORT)mWord % 2)  == 0)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else if(dFlag == 8)
	{
		if(((USHORT)mWord % 4)  == 0)
		{	
			return TRUE;
		}
		else
		{	
			return FALSE;
		}
	}
	return FALSE;
}

/*============================= 主对话框初始化相关处理函数 ==============================*/
void mInitCheckBox(HWND hDialog) //初始化对话框控件
{
	unsigned char i, tmp[4],add[10];
	int j=0;
	for(i = 0; i <4; i++) // 初始化设备号,最多支持4个设备 
	{
		sprintf((char*)tmp, "%d", i);
		SendDlgItemMessage(hDialog, IDC_DEVICE_NUMBER, CB_ADDSTRING, 0 , (LPARAM)tmp);
	}
	SendDlgItemMessage(hDialog, IDC_DEVICE_NUMBER, CB_SETCURSEL, 0, 0); // 设置当前设备号为0 
	do//初始化脉冲宽度COMBOX
	{
		j=j+30;
		sprintf((char*)add,"%d ns",j);
		SendDlgItemMessage(hDialog,IDC_CBPU,CB_ADDSTRING,0,(LPARAM)add);
	}while(j<=420);
	SendDlgItemMessage(hDialog,IDC_CBPU,CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_ADDSTRING, 0 , (LPARAM)"字节");//初始化IO模式选择
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_ADDSTRING, 0 , (LPARAM)"字");
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_ADDSTRING, 0 , (LPARAM)"双字");
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBIOADDTYPE, CB_ADDSTRING, 0 , (LPARAM)"偏移地址");//初始化IO地址选择
	SendDlgItemMessage(hDialog, IDC_CBIOADDTYPE, CB_ADDSTRING, 0 , (LPARAM)"直接地址");
	SendDlgItemMessage(hDialog, IDC_CBIOADDTYPE, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBMEMMODEL, CB_ADDSTRING, 0 , (LPARAM)"字节");//初始化IO模式选择
	SendDlgItemMessage(hDialog, IDC_CBMEMMODEL, CB_ADDSTRING, 0 , (LPARAM)"双字");
	SendDlgItemMessage(hDialog, IDC_CBMEMMODEL, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBSPINUM, CB_ADDSTRING, 0 , (LPARAM)"SPI3");//初始化SPI引脚
	SendDlgItemMessage(hDialog, IDC_CBSPINUM, CB_ADDSTRING, 0 , (LPARAM)"SPI4");
	SendDlgItemMessage(hDialog, IDC_CBSPINUM, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBSPICLK, CB_ADDSTRING, 0 , (LPARAM)"15.6Mhz");//初始化SPI频率
	SendDlgItemMessage(hDialog, IDC_CBSPICLK, CB_ADDSTRING, 0 , (LPARAM)"31.3Mhz");
	SendDlgItemMessage(hDialog, IDC_CBSPICLK, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_EDITCONADD, EM_SETLIMITTEXT, 2, 0); // 限制EDIT输入数据长度 
	SendDlgItemMessage(hDialog, IDC_EDITCONDATA, EM_SETLIMITTEXT, 2, 0);
	SendDlgItemMessage(hDialog, IDC_EDITIOADD, EM_SETLIMITTEXT, 4, 0);
	SendDlgItemMessage(hDialog, IDC_EDITMEMADD, EM_SETLIMITTEXT, 4, 0);
	SendDlgItemMessage(hDialog, IDC_EDITI2CDATA, EM_SETLIMITTEXT, 2, 0);
	SendDlgItemMessage(hDialog, IDC_EDITI2CDATA, EM_SETLIMITTEXT, 2, 0);
	WindowProcedureIoAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOADD), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOADD), GWL_USERDATA, (LONG)WindowProcedureIoAdd);
	WindowProcedureIoLen = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOLEN), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOLEN), GWL_USERDATA, (LONG)WindowProcedureIoLen);
	WindowProcedureIoData = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITIODATA), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITIODATA), GWL_USERDATA, (LONG)WindowProcedureIoData);
	WindowProcedureMemAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMADD), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMADD), GWL_USERDATA, (LONG)WindowProcedureMemAdd);
	WindowProcedureMemLen = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMLEN), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMLEN), GWL_USERDATA, (LONG)WindowProcedureMemLen);
	WindowProcedureMemData = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMDATA), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMDATA), GWL_USERDATA, (LONG)WindowProcedureMemData);
	WindowProcedureI2CAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CADD), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CADD), GWL_USERDATA, (LONG)WindowProcedureI2CAdd);
	WindowProcedureI2cData= (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CDATA), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CDATA), GWL_USERDATA, (LONG)WindowProcedureI2cData);
	WindowProcedureConAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONADD), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONADD), GWL_USERDATA, (LONG)WindowProcedureConAdd);
	WindowProcedureConData= (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONDATA), GWL_WNDPROC, (LONG)MyEditProc);//限制输入的字符为16进制字符
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONDATA), GWL_USERDATA, (LONG)WindowProcedureConData);
	SetWindowText(GetDlgItem(hDialog, IDC_EDITMEMLEN),"1");
	SetWindowText(GetDlgItem(hDialog, IDC_EDITIOLEN),"1");
	EnableWindow(GetDlgItem(hDialog,IDC_BTNPU),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_STATICMEM),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_EDTMEM),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_FLASH_READ),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_FLASH_WRITE),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_FLASH_ERASE),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_FLASH_LOADFILE),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_CB32),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_EDTMEM),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNIOCL),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNIORD),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNIOWR),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNMEMCL),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNMEMRD),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNMEMWR),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNI2CRD),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNI2CWR),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNI2C_PROGRAM),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNSPISET),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNSPIRD),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNSPIWR),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNSPICL),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNCONRD),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNCONWR),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNINT1),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNINT2),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_BTNCLOSE),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_STATICMEM),FALSE);
	EnableWindow(GetDlgItem(hDialog,IDC_EDTMEM),FALSE);
}

void mShowDevVer(HWND hDialog) //获得驱动版本号
{
	ULONG ver = 0;
	char vers[8];
	ver = CH367GetDrvVersion();  
	_ltoa(ver, vers, 16);
	if(!SetDlgItemText(hDialog, IDC_EDTDEVVER, vers))
	{
		MessageBox(hDialog, "未能成功获得驱动版本号!", "提示", MB_ICONSTOP | MB_OK);
	}
}

void AddrRefresh(HWND hDialog) // 取I/O基址存储器基址和中断号 
{
	ULONG oIntLine;
	UCHAR oByte,x;
	ULONG ver = 0;
	if(CH367mGetIntLine(mIndex, &oIntLine)) 
	{
		CHAR sByte[12];
		sprintf(sByte, "%02X\x0", oIntLine);
		SetDlgItemText(hDialog, IDC_EDTINTLINE, sByte);
	}
	else
	{
		MessageBox(hDialog, "获得中断号失败!", "提示", MB_OK | MB_ICONSTOP);
		SetDlgItemText(hDialog, IDC_EDTINTLINE, "");
	}
	if(CH367mGetIoBaseAddr(mIndex, &mBaseAddr)) //取I/O基址 
	{
		CHAR sByte[12];
		sprintf(sByte, "%04X\x0", mBaseAddr);
		SetDlgItemText(hDialog, IDC_EDTBASEADDR, sByte);
	}
	else
	{
		MessageBox(hDialog, "获得I/O基址失败!", "提示", MB_OK | MB_ICONSTOP);
		SetDlgItemText(hDialog, IDC_EDTBASEADDR, "");
	}
	if(CH368==1)
	{
		if(CH368mGetMemBaseAddr(mIndex, &mMemAddr)) //取MEM基址 
		{
			CHAR sByte[128]="";
			sprintf( sByte,"%04p\x0", mMemAddr );
			SetDlgItemText(hDialog,IDC_EDTMEM,sByte);
		}
		else
		{
			MessageBox(hDialog,"获得MEM寄存器基址失败！","提示",MB_OK|MB_ICONSTOP);
			SetDlgItemText(hDialog,IDC_EDTMEM,"");
		}
		
	}
	if(CH367mReadIoByte(mIndex, &mBaseAddr->mCH367Speed,&oByte))//显示当前脉冲宽度
	{
		char input[10];
		x=oByte&0X0F;
		sprintf(input,"%d\0",(x+1)*30);
		SetDlgItemText(mSaveDialogMain,IDC_PULSHOW,input);
	}
	else
	{
		MessageBox(mSaveDialogMain,"当前脉冲读取失败","提示",MB_OK|MB_ICONSTOP);
	}
	if(CH368==1)
	{
		if(!CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,oByte|0x40))//设置CH368支持32位对IO或者存储器进行读写
			MessageBox(mSaveDialogMain,"32位IO读写设置失败","提示",MB_OK);
	}
}

void mPreVerify(HWND hwnd)//预先判别有哪些板卡插入计算机
{
	char preRead[100];
	char item[100],mError[100];
	int j=0;
	int i,m;
	UCHAR cByte;
	for( i=0;i<16;i++)
	{
		if(CH367mOpenDevice(i,FALSE,TRUE,0x00)!=INVALID_HANDLE_VALUE)
		{	
			CH367mReadConfig(i,(PVOID)0x02, &cByte);//读取设备的配置空间中的设备标识用以确定是什么设备
			if(cByte==0x34)//0x34表示CH368设备,0x31或者0x30表示CH367,如果您更改了设备标识,即设备的配置寄存器中的02H空间，请将程序中更改这里的标识
			{
				Devices[j][1]=1;
			}
			else if(cByte==0x31||cByte==0x30)
			{
				Devices[j][1]=2;
			}
			else
			{
				sprintf(mError,"未能识别您的设备%d，如果您更改了设备标识,即设备的配置寄存器中的02H空间，请将程序中相应的代码更改一下",i);
				CH367mCloseDevice(i);
				MessageBox(NULL,mError,"提示",MB_OK);
				return;
			}
			j++;
		}
		CH367mCloseDevice(i);
	}
	sprintf(preRead,"已检测到%d台设备. ",j);
	for(m=0;m<j;m++)
	{
		if(Devices[m][1]==2)
			sprintf(item,"设备%d为CH367设备. ",m);
		else if(Devices[m][1]==1)
			sprintf(item,"设备%d为CH368设备. ",m);
		strcat(preRead,item);
	}
	SetDlgItemText(mSaveDialogMain,IDC_STATUS,preRead);
}

/*============================= 脉冲宽度设置 ==============================*/
BOOL PulwidthSetting()
{
	int j,x;
	UCHAR PulShow[10];
	j=SendMessage(GetDlgItem(mSaveDialogMain,IDC_CBPU),CB_GETCURSEL,0,0);
	CurPulWidth=PulWidth[j];
	if(CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,CurPulWidth))//向CH367/CH368的速度寄存器中写入控制数据
	{
		MessageBox(mSaveDialogMain,"设置成功","提示",MB_OK);
		x=CurPulWidth&0x0F;//获得CurPulWidth的低四位
		sprintf((char*)PulShow,"%d\0",(x+1)*30);
		SetWindowText(GetDlgItem(mSaveDialogMain,IDC_PULSHOW), (char*)PulShow);//将设置的速度显示在主对话框上
	}
	else
	{
		MessageBox(mSaveDialogMain,"设置失败","提示",MB_OK|MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

/*=============================IO的读写 ==============================*/
void mIoRead(HWND hDialog,int IoModel,int IoAddrType)//IO空间读
{
	mVAR_TYPE mVarType;
	UCHAR IoAddr[5],IoLen[5];
	UCHAR mBuf[mMAX_BUFFER_LENGTH]="";
	ULONG mLen=0,mAddr;
	mAddr=GetDlgItemText(hDialog,IDC_EDITIOADD, (char*)IoAddr,5);
	if(!mCheckInput(IoAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(IoAddr,mAddr, 0);
	mAddr = mVarType.sVar;//输入地址
	mLen=GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5);
	if(!mCheckInput(IoLen))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(IoLen);//输入长度
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"请输入小于等于0x8000的长度!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	if(GetDlgItemText(hDialog,IDC_EDITIOADD, (char*)IoAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5)==0)
	{
		MessageBox(hDialog,"请输入地址和数据长度","提示",MB_OK|MB_ICONERROR);
		return;
	}
	if(IoModel==0&&IoAddrType==0)//以字节和偏移地址读写IO
	{
		if(!CH367mAccessBlock(mIndex,mFuncReadIoByte,&mBaseAddr->mCH367IoPort[mAddr],mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO读取失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
					j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==0&&IoAddrType==1)//以字节和直接地址读写IO
	{
		if(!CH367mAccessBlock(mIndex,mFuncReadIoByte,(PVOID)mAddr,mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO读取失败","提示",MB_OK);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==1&&IoAddrType==0)//以字和偏移地址读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 4)||!mCheckWord((PVOID)mLen, 4))
		{
			MessageBox(hDialog, "请输入能存储字的起始地址并且数据长度为2的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoWord,&mBaseAddr->mCH367IoPort[mAddr],mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO读取失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==1&&IoAddrType==1)//以字和直接地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 4)||!mCheckWord((PVOID)mLen, 4))
		{
			MessageBox(hDialog, "请输入能存储字的起始地址并且数据长度为2的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoWord,(PVOID)mAddr,mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO读取失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==2&&IoAddrType==0)//以双字和偏移地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 8)||!mCheckWord((PVOID)mLen, 8))
		{
			MessageBox(hDialog, "请输入能存储字的起始地址且数据长度为4的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoDword,&mBaseAddr->mCH367IoPort[mAddr],mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO读取失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else//以双字和直接地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 8)||!mCheckWord((PVOID)mLen, 8))
		{
			MessageBox(hDialog, "请输入能存储字的起始地址且数据长度为4的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoDword,(PVOID)mAddr,mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO读取失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
}

void mIoWrite(HWND hDialog,int IoModel,int IoAddrType)//IO空间写
{
	mVAR_TYPE mVarType;
	UCHAR IoAddr[5],IoLen[5];
	UCHAR mBuf[mMAX_BUFFER_LENGTH*2]="";
	UCHAR bufferFilter[mMAX_BUFFER_LENGTH*2]="";
	UCHAR buffer[mMAX_BUFFER_LENGTH]="";
	UCHAR inputStr[mMAX_BUFFER_LENGTH*2]="";
	ULONG mLen=0,mAddr,strLen,i,j=0;
	mAddr=GetDlgItemText(hDialog,IDC_EDITIOADD,(char*)IoAddr,5);
	if(!mCheckInput(IoAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(IoAddr,mAddr, 0);
	mAddr = mVarType.sVar;//获取输入地址
	mLen=GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5);
	if(!mCheckInput(IoLen))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(IoLen);//获取输入长度
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"请输入小于等于0x8000的长度!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	strLen=GetDlgItemText(hDialog,IDC_EDITIODATA, (char*)inputStr,mMAX_BUFFER_LENGTH);//得到输入框中的数据
	for(i=0;i<strLen;i++)
	{
		if(inputStr[i]!=' ')
			bufferFilter[j++]=inputStr[i];
	}
	if(GetDlgItemText(hDialog,IDC_EDITIOADD, (char*)IoAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5)==0||GetDlgItemText(hDialog,IDC_EDITIODATA, (char*)IoLen,5)==0)
	{
		MessageBox(hDialog,"请输入地址,数据长度和写入数据内容","提示",MB_OK|MB_ICONERROR);
		return;
	}
	if(mLen>j/2)//在输入长度和数据长度中取较小的值
	{
		mLen=j/2;
	}
	memcpy(mBuf,bufferFilter,mLen*2);
	memcpy(buffer,mStrtoVal(mBuf,mLen*2),mLen);
	if(IoModel==0&&IoAddrType==0)//以字节和偏移地址的方式读写IO
	{
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoByte,&mBaseAddr->mCH367IoPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
	else if(IoModel==0&&IoAddrType==1)//以字节和直接地址的方式读写IO
	{
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoByte,(PVOID)mAddr,buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
	else if(IoModel==1&&IoAddrType==0)//以字和偏移地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 4))
		{
			MessageBox(hDialog, "请输入能存储字的起始地址且数据长度为2的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoWord,&mBaseAddr->mCH367IoPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
	else if(IoModel==1&&IoAddrType==1)//以字和直接地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 4))
		{
			MessageBox(hDialog, "请输入能存储字的起始地址且数据长度为2的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoWord,(PVOID)mAddr,buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
	else if(IoModel==2&&IoAddrType==0)//以双字和偏移地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 8))
		{
			MessageBox(hDialog, "请输入能存储双字的起始地址且数据长度为4的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoDword,&mBaseAddr->mCH367IoPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
	else//以双字和直接地址的方式读写IO
	{
		if(!mCheckWord((PVOID)mAddr, 8))
		{
			MessageBox(hDialog, "请输入能存储双字的起始地址且数据长度为4的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoDword,(PVOID)mAddr,buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
}


/*============================= MEM读写 ==============================*/
void mMemRead(HWND hDialog,int MemModel)//MEM读操作
{
	mVAR_TYPE mVarType;
	UCHAR MemAddr[5],MemLen[5];
	UCHAR mBuf[mMAX_BUFFER_LENGTH]="";
	ULONG mLen=0,mAddr;
	mAddr=GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5);
	if(!mCheckInput(MemAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(MemAddr,mAddr, 0);
	mAddr = mVarType.sVar;//获得输入地址
	mLen=GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemLen,5);
	if(!mCheckInput(MemLen))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(MemLen);//获得输入长度
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"请输入小于等于0x8000的长度!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	if(GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemAddr,5)==0)
	{
		MessageBox(hDialog,"请输入地址和数据长度","提示",MB_OK|MB_ICONSTOP);
		return;
	}
	if(mAddr+mLen>0x7FFF)
	{
		MessageBox(hDialog,"您输入的地址+长度>0x7FFF发生错误,请查阅CH367mAccessBlock的相关说明","提示",MB_OK|MB_ICONSTOP);
		return;
	}
	if(MemModel==0)
	{
		if(!CH367mAccessBlock(mIndex,mFuncReadMemByte,&mMemAddr->mCH368MemPort[mAddr],mBuf,mLen))
		{
			MessageBox(hDialog,"MEM读取失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITMEMDATA,buffer);
		}
	}
	else
	{
		if(!mCheckWord((PVOID)mAddr, 8)||!mCheckWord((PVOID)mLen, 8))
		{
			MessageBox(hDialog, "请输入能存储双字的起始地址且数据长度为4的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadMemDword,&mMemAddr->mCH368MemPort[mAddr],mBuf,mLen))
		{
			MessageBox(hDialog,"MEM读取失败","提示",MB_OK);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //两位十六进制数加一个空格
				if (mBuf[i]<16 )                      //一位十六进制字符前面加0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //小写字母转为大写字母
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //小写字母转为大写字母
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITMEMDATA,buffer);
		}
	}
}

void mMemWrite(HWND hDialog,int MemModel)
{
	mVAR_TYPE mVarType;
	UCHAR MemAddr[5],MemLen[5];
	UCHAR mBuf[mMAX_BUFFER_LENGTH*2]="";
	UCHAR bufferFilter[mMAX_BUFFER_LENGTH*2]="";
	UCHAR buffer[mMAX_BUFFER_LENGTH]="";
	UCHAR inputStr[mMAX_BUFFER_LENGTH*2]="";
	ULONG mLen=0,mAddr,strLen,i,j=0;
	mAddr=GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5);
	if(!mCheckInput(MemAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(MemAddr,mAddr, 0);
	mAddr = mVarType.sVar;//获取输入地址
	mLen=GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemLen,5);
	if(!mCheckInput(MemLen))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(MemLen);//获取输入长度
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"请输入小于等于0x8000的长度!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	strLen=GetDlgItemText(hDialog,IDC_EDITMEMDATA, (char*)inputStr,mMAX_BUFFER_LENGTH);//获取输入数据
	for(i=0;i<strLen;i++)
	{
		if(inputStr[i]!=' ')
			bufferFilter[j++]=inputStr[i];
	}
	if(mAddr+mLen>0x7FFF)
	{
		MessageBox(hDialog,"您输入的地址+长度>0x7FFF发生错误,请查阅CH367mAccessBlock的相关说明","提示",MB_OK|MB_ICONERROR);
		return;
	}
	if(GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemLen,5)==0||GetDlgItemText(hDialog,IDC_EDITMEMDATA, (char*)inputStr,mMAX_BUFFER_LENGTH)==0)
	{
		MessageBox(hDialog,"请输入地址,数据长度和写入数据内容","提示",MB_OK|MB_ICONSTOP);
		return;
	}
	if(mLen>j/2)//在输入长度和数据长度中取较小的值
	{
		mLen=j/2;
	}
	memcpy(mBuf,bufferFilter,mLen*2);
	memcpy(buffer,mStrtoVal(mBuf,mLen*2),mLen);
	if(MemModel==0)//以字节的方式读写MEM
	{
		if(!CH367mAccessBlock(mIndex,mFuncWriteMemByte,&mMemAddr->mCH368MemPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
	else//以双字的方式读写MEM
	{
		if(!mCheckWord((PVOID)mAddr, 8))
		{
			MessageBox(hDialog, "请输入能存储双字的起始地址且数据长度为4的倍数", "提示", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteMemDword,&mMemAddr->mCH368MemPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"写失败","提示",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"写成功","提示",MB_OK);
		}
	}
}





/*============================= I2C相关处理　==============================*/
void mI2CRead(HWND hDialog)//I2C读
{
	UCHAR dAddr[8];
	USHORT dAddrLen;
	UCHAR idAddr; // 数据单元地址 
	UCHAR cByte;
	mVAR_TYPE mVarTyped;
	dAddrLen = GetDlgItemText(hDialog, IDC_EDITI2CADD, (char*)dAddr, 3); // 取数据单元地址 
	if(!mCheckInput(dAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarTyped = mCharToVar(dAddr, dAddrLen, 3);
	idAddr = mVarTyped.cVar;
	if(dAddrLen) // 取数据单元地址 
	{
		if(CH367mReadI2C(mIndex, 0x50, idAddr, &cByte)) // 从I2C接口读一个字节 
		{
			UCHAR sByte[8];
			sprintf((char*)sByte, "%02X\x0", cByte);
			SetDlgItemText(hDialog, IDC_EDITI2CDATA, (char*)sByte);
		}
		else
		{
			MessageBox(hDialog, "读数据失败!", "提示", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "请输入地址!", "提示", MB_OK);
	}
}

void mI2CWrite(HWND hDialog)
{
	UCHAR dAddr[8];
	USHORT dAddrLen;
	UCHAR idAddr; // 数据单元地址
	UCHAR oByte[8];
	UCHAR cByte;
	UINT oByteLen;
	mVAR_TYPE mVarByte;
	mVAR_TYPE mVarTyped;
	dAddrLen = GetDlgItemText(hDialog, IDC_EDITI2CADD, (char*)dAddr, 3); // 取数据单元地址 
	if(!mCheckInput(dAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarTyped = mCharToVar(dAddr, dAddrLen, 3);
	idAddr = mVarTyped.cVar;
	
	oByteLen = GetDlgItemText(hDialog, IDC_EDITI2CDATA, (char*)oByte, 3); // 取待写入数据 
	if(!mCheckInput(oByte))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarByte = mCharToVar(oByte, oByteLen, 0);
	cByte = mVarByte.cVar;
	if(dAddrLen && oByteLen)
	{
		if(CH367mWriteI2C(mIndex, 0x50, idAddr, cByte)) // 向I2C中写入一个字节 
		{
			MessageBox(hDialog, "写数据成功!", "提示", MB_OK);
		}
		else
		{
			MessageBox(hDialog, "写数据失败!", "提示", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "请输入地址和数据!", "提示", MB_OK);
	}
}

void eeprom_write(void) //修改VID,DID... 
{
	mVAR_TYPE mVarType;
	UCHAR i, data[8], buffer[32];
	UINT Len = 0;
	USHORT Value = 0;
	buffer[0] = 0x78;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_VID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // 取VID 
	Value = mVarType.sVar;
	buffer[4] = Value & 0xFF;
	buffer[5] = Value >> 8;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_DID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // 取VID 
	Value = mVarType.sVar;
	buffer[6] = Value & 0xFF;
	buffer[7] = Value >> 8;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_RID, (char*)data, 3);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 3); // 取RID 
	buffer[8] = mVarType.cVar;
	buffer[9] = 0x00;
	buffer[10] = 0x00;
	buffer[11] = 0x10;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_SVID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // 取SVID 
	Value = mVarType.sVar;
	buffer[12] = Value & 0xFF;
	buffer[13] = Value >> 8;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_SID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // 取SVID 
	Value = mVarType.sVar;
	buffer[14] = Value & 0xFF;
	buffer[15] = Value >> 8;
	for(i = 0x10; i < 0x20; i++)
	{
		buffer[i] = 0x00;
	}
	for(i = 0; i < 0x20; i++)
	{
		if(CH367mWriteI2C(mIndex, 0x50, i, buffer[i]))
		{
			Sleep(20); // 务必要加延时 
		}
		else
		{
			MessageBox(mSaveDialogI2c, "写EEPROM失败,可能EEPROM损坏或没有连接", mCaptionInform, MB_OK);
			return;
		}
	}
	MessageBox(mSaveDialogI2c, "写EEPROM完成", mCaptionInform, MB_OK);
}

void eeprom_read(void) // 读EEPROM 
{
	CHAR i, sByte[8];
	UCHAR data[32];
	for(i = 0; i < 0x20; i++)
	{
		if(!CH367mReadI2C(mIndex, 0x50, i, &data[i]))
		{
			MessageBox(mSaveDialogI2c, "写EEPROM失败,可能EEPROM损坏或没有连接", mCaptionInform, MB_OK);
			EndDialog(mSaveDialogI2c,2);
			return;
		}
	}
	sprintf(sByte, "%02X%02X\x0", data[5], data[4]);
	SetDlgItemText(mSaveDialogI2c, IDC_VID, sByte); // 输出VID 
	sprintf(sByte, "%02X%02X\x0", data[7], data[6]);
	SetDlgItemText(mSaveDialogI2c, IDC_DID, sByte); // 输出DID 
	sprintf(sByte, "%02X\x0", data[8]);
	SetDlgItemText(mSaveDialogI2c, IDC_RID, sByte); // 输出RID 
	sprintf(sByte, "%02X%02X\x0", data[13], data[12]);
	SetDlgItemText(mSaveDialogI2c, IDC_SVID, sByte); // 输出SVID 
	sprintf(sByte, "%02X%02X\x0", data[15], data[14]);
	SetDlgItemText(mSaveDialogI2c, IDC_SID, sByte); // 输出SID 
}

INT_PTR CALLBACK mDialogI2c(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam)//I2C对话框处理函数
{
	switch(uMessage)
	{
	case WM_INITDIALOG: // 初始化 
		{
			mSaveDialogI2c = hDialog;
			EnableWindow(GetDlgItem(mSaveDialogMain, IDC_BTNI2C_PROGRAM), FALSE);
			SendDlgItemMessage(hDialog, IDC_VID, EM_SETLIMITTEXT, 4, 0);
			SendDlgItemMessage(hDialog, IDC_DID, EM_SETLIMITTEXT, 4, 0);
			SendDlgItemMessage(hDialog, IDC_RID, EM_SETLIMITTEXT, 2, 0);
			SendDlgItemMessage(hDialog, IDC_SVID, EM_SETLIMITTEXT, 4, 0);
			SendDlgItemMessage(hDialog, IDC_SID, EM_SETLIMITTEXT, 4, 0);
			WindowProcedureI2cDid = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_DID), GWL_WNDPROC, (LONG)MyEditProc);
			SetWindowLong(GetDlgItem(hDialog, IDC_DID), GWL_USERDATA, (LONG)WindowProcedureI2cDid);
			WindowProcedureI2cVid = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_VID), GWL_WNDPROC, (LONG)MyEditProc);
			SetWindowLong(GetDlgItem(hDialog, IDC_VID), GWL_USERDATA, (LONG)WindowProcedureI2cVid);
			WindowProcedureI2cRid = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_RID), GWL_WNDPROC, (LONG)MyEditProc);
			SetWindowLong(GetDlgItem(hDialog, IDC_RID), GWL_USERDATA, (LONG)WindowProcedureI2cRid);
			WindowProcedureI2cSdid = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_SID), GWL_WNDPROC, (LONG)MyEditProc);
			SetWindowLong(GetDlgItem(hDialog, IDC_SID), GWL_USERDATA, (LONG)WindowProcedureI2cSdid);
			WindowProcedureI2cSvid = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_SVID), GWL_WNDPROC, (LONG)MyEditProc);
			SetWindowLong(GetDlgItem(hDialog, IDC_SVID), GWL_USERDATA, (LONG)WindowProcedureI2cSvid);
			eeprom_read(); // 读eeprom 
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_PROGRAM: // 写EEPROM 
				{
					eeprom_write();
				}
				return TRUE;
			case IDC_PROMREAD://读EEPROM
				{
					eeprom_read();
				}
				return TRUE;
			case WM_DESTROY: // 退出 
				EnableWindow(GetDlgItem(mSaveDialogMain, IDC_BTNI2C_PROGRAM), TRUE);
				EndDialog(hDialog, 1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;  
}


/*============================= 配置空间的读写 ==============================*/
void mConRead(HWND hDialog)//配置空间读
{
	UCHAR iAddr[8];
	USHORT mAddr;
	UCHAR cByte;
	UINT iAddrLen;
	mVAR_TYPE mVarType;
	iAddrLen = GetDlgItemText(hDialog, IDC_EDITCONADD, (char*)iAddr, 5); // 取地址 
	if(!mCheckInput(iAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(iAddr,iAddrLen,0);
	mAddr = mVarType.sVar;
	if(iAddrLen) 
	{
		if(CH367mReadConfig(mIndex, (PVOID)mAddr, &cByte)) /* 从配置空间读一个字节 */
		{
			UCHAR sByte[8];
			sprintf((char*)sByte, "%02X\x0", cByte);
			SetDlgItemText(hDialog, IDC_EDITCONDATA, (char*)sByte);
		}
		else
		{
			MessageBox(hDialog, "读数据失败!", "提示", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "请输入地址!", "提示", MB_OK);
	}
}

void mConWrite(HWND hDialog)//配置空间写
{
	UCHAR iAddr[8];
	USHORT mAddr;
	UCHAR oByte[8];
	UCHAR cByte;
	int iAddrLen;
	mVAR_TYPE mVarType;
	mVAR_TYPE mVarByte;
	int oByteLen = 0;
	iAddrLen = GetDlgItemText(hDialog, IDC_EDITCONADD, (char*)iAddr, 8); // 取地址 
	if(!mCheckInput(iAddr))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(iAddr,iAddrLen,0);
	mAddr = mVarType.sVar;	
	oByteLen=GetDlgItemText(hDialog, IDC_EDITCONDATA, (char*)oByte, 8); // 取数据 
	if(!mCheckInput(oByte))
	{
		MessageBox(hDialog,"您输入的字符有误，请输入0~9,a~f,A~F之间的十六进制数!","提示",MB_OK|MB_ICONERROR);
		return;
	}
	mVarByte=mCharToVar(oByte,oByteLen,3);
	cByte = mVarByte.cVar;
	if(iAddrLen && oByteLen)	
	{
		if(CH367mWriteConfig(mIndex, (PVOID)mAddr, cByte)) // 向配置空间写一个字节 
		{	
			MessageBox(hDialog, "写数据成功!", "提示", MB_OK);
		}
		else
		{
			MessageBox(hDialog, "写数据失败!", "提示", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "请输入地址和数据!", "提示", MB_OK);
	}
}


/*============================= SPI 25VF512 相关处理 ==============================*/

unsigned char write_enable(void) // 写使能 
{
	unsigned char buffer[8];
	buffer[0] = WREN;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char write_disable(void) // 写禁止 
{
	unsigned char buffer[8];
	buffer[0] = WRDI;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char read_status_register(unsigned char *data) //读状态寄存器 
{
	unsigned char buffer[8];
	unsigned long len = 0x01;
	buffer[0] = RDSR;
	if( CH367StreamSPI(mIndex, 0x01, buffer, &len, data) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char write_status_register_enable(void) // 写状态寄存器使能 
{
	unsigned char buffer[8];
	buffer[0] = EWSR;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char write_status_register(unsigned char status) // 写状态寄存器 
{
	unsigned char buffer[8];
	buffer[0] = WRSR;
	buffer[1] = status;
	if( CH367StreamSPI(mIndex, 0x02, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char check(void) // 检查WEL位 
{
	unsigned char data;
	int count = 10000;
	do{
		read_status_register(&data);
		if(count-- == 0) return FALSE;
	}while(data & 0x01);
	return TRUE;
}

unsigned char byte_program(unsigned long address, unsigned char data) // 字节写 
{
	unsigned char buffer[8];
	buffer[0] = BP;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF);
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	buffer[4] = data;
	if( CH367StreamSPI(mIndex, 0x05, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char sector_erase(unsigned long address) // 扇区擦除 
{
	unsigned char buffer[8];
	buffer[0] = SE;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF); 
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	if( CH367StreamSPI(mIndex, 0x04, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char bulk_erase(unsigned long address) // 块擦除 
{
	unsigned char buffer[8];
	buffer[0] = BE;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF);
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	if( CH367StreamSPI(mIndex, 0x04, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;	
}

unsigned char chip_erase(void) // 芯片擦除
{
	unsigned char buffer[8];
	buffer[0] = CE;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;		
}

unsigned char read_block(unsigned long address, unsigned long length, unsigned char *data) // 读FLASH 
{
	unsigned char buffer[8];
	if(length > 4096) return FALSE;
	buffer[0] = READ;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF);
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	if( CH367StreamSPI(mIndex, 0x04, buffer, &length, data) == FALSE ) return FALSE;
	return TRUE;	
}

unsigned char read_block_fast(unsigned long address, unsigned long length, unsigned char *data) // 快速读FLASH 
{
	unsigned char buffer[8];
	if(length > 4096) return FALSE;
	buffer[0] = FAST_READ;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF);
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	buffer[4] = 0x00;
	if( CH367StreamSPI(mIndex, 0x05, buffer, &length, data) == FALSE ) return FALSE;
	return TRUE;
}

BOOL read_manufacturer_device_id(unsigned char *data) // 读厂商/设备ID 
{
	unsigned char buffer[8];
	unsigned long len = 0x04;
	buffer[0] = RMDI;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	if( CH367StreamSPI(mIndex, 0x04, buffer, &len, data) == FALSE ) return FALSE;
	return TRUE;	
}

void load_file(void) // 选择将发送的文件 
{
	TCHAR szFile[260] = "\0";
	OPENFILENAME openfilename;
	TCHAR szFilter[] = __TEXT( "Images(*.TXT)\0*.txt; \0All Files(*.*)\0*.*\0\0" );
	openfilename.lStructSize       = sizeof ( OPENFILENAME );
	openfilename.hwndOwner         = mSaveDialogMain;
	openfilename.hInstance         = mSaveInstance;
	openfilename.lpstrFilter       = szFilter;
	openfilename.lpstrCustomFilter = NULL;
	openfilename.nMaxCustFilter    = 0;
	openfilename.nFilterIndex      = 0;
	openfilename.lpstrFile         = szFile;
	openfilename.nMaxFile          = sizeof(szFile);
	openfilename.lpstrFileTitle    = NULL;
	openfilename.nMaxFileTitle     = 0;
	openfilename.lpstrInitialDir   = NULL;
	openfilename.lpstrTitle		   = "选择即将写入的目标文件";
	openfilename.nFileOffset       = 0;
	openfilename.nFileExtension    = 0;
	openfilename.lpstrDefExt       = NULL;
	openfilename.lCustData         = 0;
	openfilename.lpfnHook 		   = NULL;
	openfilename.lpTemplateName    = NULL;
	openfilename.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_READONLY | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&openfilename) ) // 获取文件路径并保存到指定的缓冲区 
	{
		memset(SpiFilePath, 0x00, sizeof(SpiFilePath));
		memcpy(SpiFilePath, openfilename.lpstrFile, strlen(openfilename.lpstrFile)); 
	}
}

void read_flash(void) // 读FLASH数据写到文件 
{
	OPENFILENAME openfilename;  
	CHAR szFile[260] = "\0";
	HANDLE hFile;
	char DirName[256];
	unsigned long address, mTotal = 0;
	unsigned char *p, buffer[1024 * 64];
	DWORD Num;
	p = buffer;
	GetCurrentDirectory(sizeof(DirName), DirName); // 获取当前的路径 
	openfilename.lStructSize       = sizeof(OPENFILENAME);
	openfilename.hwndOwner         = NULL;
	openfilename.hInstance         = NULL;
	openfilename.lpstrFilter       = __TEXT("Images(*.txt)\0*.txt;\0All Files(*.*)\0*.*\0\0");
	openfilename.lpstrCustomFilter = NULL;
	openfilename.nMaxCustFilter    = 0;
	openfilename.nFilterIndex      = 0;
	openfilename.lpstrFile         = szFile;
	openfilename.nMaxFile          = sizeof(szFile);
	openfilename.lpstrFileTitle    = NULL;
	openfilename.nMaxFileTitle     = 0;
	openfilename.lpstrInitialDir   = DirName;
	openfilename.lpstrTitle        = "保存文件";
	openfilename.nFileOffset       = 0;
	openfilename.nFileExtension    = 0;
	openfilename.lpstrDefExt       = __TEXT("txt\0");
	openfilename.lCustData         = 0;
	openfilename.lpfnHook 		   = NULL;
	openfilename.lpTemplateName    = NULL;
	openfilename.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT;
	
	if (GetSaveFileName(&openfilename)) // 创建指定名字的文件
	{  
		hFile = CreateFile ( szFile,  
			GENERIC_WRITE | GENERIC_READ,  
			FILE_SHARE_READ | FILE_SHARE_WRITE,  
			NULL,  
			CREATE_ALWAYS,  
			FILE_ATTRIBUTE_NORMAL,  
			NULL);  
		if (hFile != INVALID_HANDLE_VALUE) // 创建指定名字的文件成功 
		{  
			for(address = 0; address < 0x10000; address += SPI_BUFFER_LENGTH)
			{
				// 读FLASH 每次读256字节 
				if(CH364mReadFlash(0,address,p, SPI_BUFFER_LENGTH))
				{
					mTotal = mTotal + SPI_BUFFER_LENGTH;
					p += SPI_BUFFER_LENGTH;
				}
				else
				{
					MessageBox(mSaveDialogMain, "读FLASH失败", mCaptionInform, MB_ICONSTOP | MB_OK);
					CloseHandle(hFile);
					return;
				}
			}
			if ( ! WriteFile(hFile, buffer, mTotal, &Num, NULL) ) // 把接收缓冲区中的数据写到创建的文件中,发生错误时,关闭创建的文件句柄 
			{ 
				MessageBox(mSaveDialogMain, "写文件失败", mCaptionInform, MB_ICONSTOP | MB_OK);
				CloseHandle(hFile);
				return;
			}
			MessageBox(mSaveDialogMain, "读FLASH成功", mCaptionInform, MB_OK);
			CloseHandle(hFile);
		}
		else // 发生错误时, 关闭创建的文件句柄  
		{  
			CloseHandle(hFile);
			return;
		}
	}
}

void write_flash(void) // 把打开的文件写到FLASH 
{
	CHAR pFileN[100] = "";
	ULONG i, address;
	HANDLE hFile;
	DWORD mFileSize = 0;
	UCHAR *mBuffer;
	
	for(i = 0; i < 100; i++)
	{
		pFileN[i] = '\0';
	}
	strcpy(pFileN, "");
	memcpy(pFileN, SpiFilePath, strlen(SpiFilePath)); // 保存文件路径到pFileN缓冲区中
	pFileN[strlen(SpiFilePath)] = '\0';
	if(pFileN[0] == '\0') // 若没有选择打开文件,提示 
	{  
		MessageBox(mSaveDialogMain, "请打开文件", mCaptionInform, MB_ICONSTOP);
	}
	else 
	{  
		if ( ( hFile = CreateFile (  // 打开选择的文件，获取文件句柄 
			( LPCTSTR ) pFileN,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			( HANDLE ) NULL ) ) == ( HANDLE ) -1 )
		{ 
			MessageBox(mSaveDialogMain, "创建/打开文件出错", mCaptionInform, MB_ICONSTOP);
			return;
		}
		mFileSize = GetFileSize (hFile, NULL); // 获取文件的大小 
		if(mFileSize > 1024 * 64)
		{
			MessageBox(mSaveDialogMain, "只能打开小于64K的文件", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile); 
			return;
		}
		if(mFileSize == 0xFFFFFFFF)
		{  
			MessageBox(mSaveDialogMain, "获取文件的大小错误", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile);
			return;
		}
		mBuffer = (UCHAR *)malloc(mFileSize); // 分配放文件的缓冲区 
		if(mBuffer == NULL)
		{ 
			MessageBox(mSaveDialogMain, "分配缓冲区错误", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile);
			free(mBuffer);
			return;
		}
		memset(mBuffer, '\0', mFileSize); // 清空缓冲区 
		if (!(ReadFile(hFile, mBuffer, mFileSize, &i, NULL))) // 读取文件数据到分配的缓冲区中,发生错误时,关闭文件句柄,释放缓冲区
		{  
			MessageBox(mSaveDialogMain, "读文件出错", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile);
			free(mBuffer);
			return;
		}
		CloseHandle(hFile);
		/*
		write_enable(); // 写使能 
		write_status_register_enable();
		write_status_register(0x00);
		//write_disable(); // 写禁止 

		for(address = 0; address < mFileSize; address++)
		{
			write_enable(); // 写使能 
			byte_program(address, *mBuffer); // 字节写 
			//write_disable(); // 写禁止 
			if(check()) // 检查WEL位 
			{
				mBuffer++;
			}
			else
			{
				MessageBox(mSaveDialogMain, "写FLASH失败,可能FLASH已经损坏,或者没有连接", mCaptionInform, MB_OK); // 可能flash已经损坏 
				return;
			}
		}
		*/
		CH364mWriteFlash(0,0,mBuffer,mFileSize);
		MessageBox(mSaveDialogMain, "写FLASH完成", mCaptionInform, MB_OK);
	}
}

void erase_flash(void) // 擦除FLASH
{
	/*
	write_enable(); // 写使能 
	if(chip_erase())
	{
		MessageBox(mSaveDialogMain, "擦除FLASH成功", mCaptionInform, MB_OK);
	}
	else
	{
		MessageBox(mSaveDialogMain, "擦除FLASH失败", mCaptionInform, MB_ICONSTOP | MB_OK);
	}
	//write_disable(); // 写禁止 
	*/
	
	CH364mLockFlash(0,0);
	if( CH364mEarseFlash(0,0,64*1024))
	{
		MessageBox(mSaveDialogMain, "擦除FLASH成功", mCaptionInform, MB_OK);
	}
	else
	{
		MessageBox(mSaveDialogMain, "擦除FLASH失败", mCaptionInform, MB_ICONSTOP | MB_OK);
	}
}




/*============================= 中断相关处理 ==============================*/

void CALLBACK InterruptEvent(void) // 设备中断通知消息 
{	
	UCHAR oByte;
	if(intType==1)
	{
		mCount++; // 中断次数记数 
		CH367_GPO_SET;//将中断电平设置无效
	}
	if(intType==2)
	{
		mCount1++;
		CH367mReadIoByte(mIndex, &mBaseAddr->mCH367Status,&oByte);
		CH367mWriteIoByte(mIndex, &mBaseAddr->mCH367Status,oByte&0xFB);//将杂项控制和状态寄存器中的激活状态设备无中断
	}
	PostMessage(mSaveDialogMain, WM_INTNOTIFY, 0, 0); // 发送设备中断消息 
}

/*============================= HELP相关处理 ==============================*/
LPCTSTR help_buf = "1-支持4个设备 \r\n\
  通过设备号选择操作哪个设备 \r\n\
  0为第一个设备(靠CPU最近的设备) \r\n\
  1为第二个设备 \r\n\
  2为第三个设备 \r\n\
  3为第四个设备 \r\n\
2-CH367,CH368设备通用,该程序自动判断设备类型\r\n\
  注意：程序通过读取设备标识来识别CH367，CH368设备，如果您更改了设备标\r\n\
  识,即设备的配置寄存器中的02H空间,请将程序中相应的代码更改一下,请在\r\n\
  程序中搜索CH367mReadConfig(mIndex,(PVOID)0x02, &mByte)和\r\n\
  CH367mReadConfig(mIndex,(PVOID)0x02, &cByte)中找到相应的代码更改一下\r\n\
3-I/O读写 \r\n\
  CH367,CH368支持以字节/字/双字模式对I/O端口访问,默认为字节方式访问 \r\n\
  支持直接地址/偏移地址访问I/O端口 \r\n\
      直接地址可以访问其他设备的I/O空间 \r\n\
      偏移地址只能访问CH367/CH368的I/O空间 \r\n\
  支持块读/写访问 \r\n\
  注意：当选择字,双字模式进行读写IO时,输入的地址和长度需分别为2和4的倍数,\r\n\
  块读写的最大长度为32KB\r\n\
4-MEM读写\r\n\
  仅CH368支持以字节/双字模式对MEM进行读写,程序中的输入地址为偏移地址,当\r\n\
  选择双字进行读写双字进行读写时,输入的地址和数据长度需为4的倍数,输入的地\r\n\
  址和数据长度的和应小于等于0x7FFF\r\n\
5-支持CH367,CH368的脉冲宽度设置\r\n\
6-I2C读写 \r\n\
  CH367/CH368支持I2C接口与外设通讯,CH367/CH368评估板演示操作24C02,支\r\n\
  持读/写EEPROM\r\n\
7-SPI接口FLASH操作 \r\n\
  CH367/CH368支持SPI接口与外设通讯,CH367/CH368评估板演示操作25VF512,\r\n\
  支持读/写/擦除FLASH \r\n\
8-配置空间读写 \r\n\
  支持字节读/写访问,详细说明请查阅CH367/CH368芯片手册 \r\n\
9-中断测试 \r\n\
  CH367/CH368支持电平/边沿中断,通过打开设备时操作中断寄存器,可以选择\r\n\
  中断类型,详细说明请查阅CH367/CH368芯片手册中IO基址寄存器和硬件中断的 \r\n\
  说明:\r\n\
  a)电平中断\r\n\
  ☉该程序通过输出引脚GP0来控制INT#触发中断,在测试时请把INT#与A15短接 \r\n\
  ☉该程序使用低电平有效的电平触发方式,低电平有效 \r\n\
  ☉该程序通过\"测试电平中断\"按键来控制GP0产生一个高电平信号和一个低\r\n\
    电平信号从而触发中断,进入中断以后产生一个高电平取消中断请求\r\n\
  b)边沿中断\r\n\
  ☉该程序通过输出引脚GP0来控制INT#触发中断,在测试时请把INT#与A15短接 \r\n\
  ☉该程序使用上升沿有效的边沿触发方式\r\n\
  ☉该程序通过\"测试边沿中断\"按键来控制GP0来模拟产生上升沿信号,当INT#脚\r\n\
    收到上升沿信号后进入中断计数，进入中断以后设置CH367/CH368的中断激活\r\n\
    状态位为0,从而取消中断\r\n\
";
  
  

void help(void) // 显示help 
{
	SetWindowText(GetDlgItem(mSaveDialogHelp, IDC_EDIT_HELP), help_buf);
}

INT_PTR CALLBACK mDialogHelp(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam) /* HELP对话框事件 */
{
	switch(uMessage)
	{
	case WM_INITDIALOG: // 初始化 
		{
			mSaveDialogHelp = hDialog;
			help();
			SetFocus(NULL);
		}
		return FALSE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case WM_DESTROY: // 退出 
				EndDialog(hDialog, 1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;  
}


