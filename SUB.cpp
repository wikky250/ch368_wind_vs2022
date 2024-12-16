
/*============================= �������ַ������� ==============================*/

LRESULT CALLBACK MyEditProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) //����Edit�ص�,ֻ�ܽ���'0'-'9','a'-'f','A'-'F','del' 
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

BOOL mCheckInput(UCHAR *input)//����û�������Ƿ���'0~9','a~f','A~F'֮���ʮ�������ַ�
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

UCHAR mCharToBcd(UCHAR iChar) // ��ASCII�ַ�ת��ΪһλBCD��,��Ч�ַ���0-9,A-F,a-f,��Ч�ַ�����0FFH 
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

mVAR_TYPE mCharToVar(UCHAR *sChar, UINT sLen, UINT sFlag) // ���ַ���ת������ֵ���� 
{
	UINT i;
	mVAR_TYPE mVarType;
	if(sLen != 0)
	{
		if(sLen > 1) //�ַ������ȴ���1 
		{
			if(sFlag == 1) //sFlagΪ1��ʾ����ULONG�� 
			{
				ULONG lReturn = 0;
				for(i = 0; i < sLen; i++)	
				{
					lReturn <<= 4;
					lReturn |= mCharToBcd(sChar[i]); 
				}
				mVarType.lVar = lReturn;
			}
			else if(sFlag == 0) //sFlagΪ0��ʾ����USHORT�� 
			{
				USHORT sReturn = 0;
				for(i = 0; i < sLen; i++)
				{
					sReturn <<= 4;
					sReturn |= mCharToBcd(sChar[i]);
				}
				mVarType.sVar = sReturn;
			}
			else if(sFlag == 2) //sFlagΪ2��ʾ����UINT�� 
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
		else //�ַ������ȵ���1 
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

PUCHAR mStrtoVal(PUCHAR str, ULONG strlen)//��һ��16��������ÿ��λ�����һ���ֽ�
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

UINT mStrToBcd(UCHAR* str)//��һ���ַ�ת��ΪUINT���͵���
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


BOOL mCheckWord(PVOID mWord, int dFlag) //����ַ�Ƿ��ܴ洢�ֻ�˫�� 
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

/*============================= ���Ի����ʼ����ش����� ==============================*/
void mInitCheckBox(HWND hDialog) //��ʼ���Ի���ؼ�
{
	unsigned char i, tmp[4],add[10];
	int j=0;
	for(i = 0; i <4; i++) // ��ʼ���豸��,���֧��4���豸 
	{
		sprintf((char*)tmp, "%d", i);
		SendDlgItemMessage(hDialog, IDC_DEVICE_NUMBER, CB_ADDSTRING, 0 , (LPARAM)tmp);
	}
	SendDlgItemMessage(hDialog, IDC_DEVICE_NUMBER, CB_SETCURSEL, 0, 0); // ���õ�ǰ�豸��Ϊ0 
	do//��ʼ��������COMBOX
	{
		j=j+30;
		sprintf((char*)add,"%d ns",j);
		SendDlgItemMessage(hDialog,IDC_CBPU,CB_ADDSTRING,0,(LPARAM)add);
	}while(j<=420);
	SendDlgItemMessage(hDialog,IDC_CBPU,CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_ADDSTRING, 0 , (LPARAM)"�ֽ�");//��ʼ��IOģʽѡ��
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_ADDSTRING, 0 , (LPARAM)"��");
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_ADDSTRING, 0 , (LPARAM)"˫��");
	SendDlgItemMessage(hDialog, IDC_CBIOMODEL, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBIOADDTYPE, CB_ADDSTRING, 0 , (LPARAM)"ƫ�Ƶ�ַ");//��ʼ��IO��ַѡ��
	SendDlgItemMessage(hDialog, IDC_CBIOADDTYPE, CB_ADDSTRING, 0 , (LPARAM)"ֱ�ӵ�ַ");
	SendDlgItemMessage(hDialog, IDC_CBIOADDTYPE, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBMEMMODEL, CB_ADDSTRING, 0 , (LPARAM)"�ֽ�");//��ʼ��IOģʽѡ��
	SendDlgItemMessage(hDialog, IDC_CBMEMMODEL, CB_ADDSTRING, 0 , (LPARAM)"˫��");
	SendDlgItemMessage(hDialog, IDC_CBMEMMODEL, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBSPINUM, CB_ADDSTRING, 0 , (LPARAM)"SPI3");//��ʼ��SPI����
	SendDlgItemMessage(hDialog, IDC_CBSPINUM, CB_ADDSTRING, 0 , (LPARAM)"SPI4");
	SendDlgItemMessage(hDialog, IDC_CBSPINUM, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_CBSPICLK, CB_ADDSTRING, 0 , (LPARAM)"15.6Mhz");//��ʼ��SPIƵ��
	SendDlgItemMessage(hDialog, IDC_CBSPICLK, CB_ADDSTRING, 0 , (LPARAM)"31.3Mhz");
	SendDlgItemMessage(hDialog, IDC_CBSPICLK, CB_SETCURSEL,0,0);
	SendDlgItemMessage(hDialog, IDC_EDITCONADD, EM_SETLIMITTEXT, 2, 0); // ����EDIT�������ݳ��� 
	SendDlgItemMessage(hDialog, IDC_EDITCONDATA, EM_SETLIMITTEXT, 2, 0);
	SendDlgItemMessage(hDialog, IDC_EDITIOADD, EM_SETLIMITTEXT, 4, 0);
	SendDlgItemMessage(hDialog, IDC_EDITMEMADD, EM_SETLIMITTEXT, 4, 0);
	SendDlgItemMessage(hDialog, IDC_EDITI2CDATA, EM_SETLIMITTEXT, 2, 0);
	SendDlgItemMessage(hDialog, IDC_EDITI2CDATA, EM_SETLIMITTEXT, 2, 0);
	WindowProcedureIoAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOADD), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOADD), GWL_USERDATA, (LONG)WindowProcedureIoAdd);
	WindowProcedureIoLen = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOLEN), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITIOLEN), GWL_USERDATA, (LONG)WindowProcedureIoLen);
	WindowProcedureIoData = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITIODATA), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITIODATA), GWL_USERDATA, (LONG)WindowProcedureIoData);
	WindowProcedureMemAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMADD), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMADD), GWL_USERDATA, (LONG)WindowProcedureMemAdd);
	WindowProcedureMemLen = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMLEN), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMLEN), GWL_USERDATA, (LONG)WindowProcedureMemLen);
	WindowProcedureMemData = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMDATA), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITMEMDATA), GWL_USERDATA, (LONG)WindowProcedureMemData);
	WindowProcedureI2CAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CADD), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CADD), GWL_USERDATA, (LONG)WindowProcedureI2CAdd);
	WindowProcedureI2cData= (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CDATA), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITI2CDATA), GWL_USERDATA, (LONG)WindowProcedureI2cData);
	WindowProcedureConAdd = (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONADD), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
	SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONADD), GWL_USERDATA, (LONG)WindowProcedureConAdd);
	WindowProcedureConData= (WNDPROC)SetWindowLong(GetDlgItem(hDialog, IDC_EDITCONDATA), GWL_WNDPROC, (LONG)MyEditProc);//����������ַ�Ϊ16�����ַ�
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

void mShowDevVer(HWND hDialog) //��������汾��
{
	ULONG ver = 0;
	char vers[8];
	ver = CH367GetDrvVersion();  
	_ltoa(ver, vers, 16);
	if(!SetDlgItemText(hDialog, IDC_EDTDEVVER, vers))
	{
		MessageBox(hDialog, "δ�ܳɹ���������汾��!", "��ʾ", MB_ICONSTOP | MB_OK);
	}
}

void AddrRefresh(HWND hDialog) // ȡI/O��ַ�洢����ַ���жϺ� 
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
		MessageBox(hDialog, "����жϺ�ʧ��!", "��ʾ", MB_OK | MB_ICONSTOP);
		SetDlgItemText(hDialog, IDC_EDTINTLINE, "");
	}
	if(CH367mGetIoBaseAddr(mIndex, &mBaseAddr)) //ȡI/O��ַ 
	{
		CHAR sByte[12];
		sprintf(sByte, "%04X\x0", mBaseAddr);
		SetDlgItemText(hDialog, IDC_EDTBASEADDR, sByte);
	}
	else
	{
		MessageBox(hDialog, "���I/O��ַʧ��!", "��ʾ", MB_OK | MB_ICONSTOP);
		SetDlgItemText(hDialog, IDC_EDTBASEADDR, "");
	}
	if(CH368==1)
	{
		if(CH368mGetMemBaseAddr(mIndex, &mMemAddr)) //ȡMEM��ַ 
		{
			CHAR sByte[128]="";
			sprintf( sByte,"%04p\x0", mMemAddr );
			SetDlgItemText(hDialog,IDC_EDTMEM,sByte);
		}
		else
		{
			MessageBox(hDialog,"���MEM�Ĵ�����ַʧ�ܣ�","��ʾ",MB_OK|MB_ICONSTOP);
			SetDlgItemText(hDialog,IDC_EDTMEM,"");
		}
		
	}
	if(CH367mReadIoByte(mIndex, &mBaseAddr->mCH367Speed,&oByte))//��ʾ��ǰ������
	{
		char input[10];
		x=oByte&0X0F;
		sprintf(input,"%d\0",(x+1)*30);
		SetDlgItemText(mSaveDialogMain,IDC_PULSHOW,input);
	}
	else
	{
		MessageBox(mSaveDialogMain,"��ǰ�����ȡʧ��","��ʾ",MB_OK|MB_ICONSTOP);
	}
	if(CH368==1)
	{
		if(!CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,oByte|0x40))//����CH368֧��32λ��IO���ߴ洢�����ж�д
			MessageBox(mSaveDialogMain,"32λIO��д����ʧ��","��ʾ",MB_OK);
	}
}

void mPreVerify(HWND hwnd)//Ԥ���б�����Щ�忨��������
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
			CH367mReadConfig(i,(PVOID)0x02, &cByte);//��ȡ�豸�����ÿռ��е��豸��ʶ����ȷ����ʲô�豸
			if(cByte==0x34)//0x34��ʾCH368�豸,0x31����0x30��ʾCH367,������������豸��ʶ,���豸�����üĴ����е�02H�ռ䣬�뽫�����и�������ı�ʶ
			{
				Devices[j][1]=1;
			}
			else if(cByte==0x31||cByte==0x30)
			{
				Devices[j][1]=2;
			}
			else
			{
				sprintf(mError,"δ��ʶ�������豸%d��������������豸��ʶ,���豸�����üĴ����е�02H�ռ䣬�뽫��������Ӧ�Ĵ������һ��",i);
				CH367mCloseDevice(i);
				MessageBox(NULL,mError,"��ʾ",MB_OK);
				return;
			}
			j++;
		}
		CH367mCloseDevice(i);
	}
	sprintf(preRead,"�Ѽ�⵽%d̨�豸. ",j);
	for(m=0;m<j;m++)
	{
		if(Devices[m][1]==2)
			sprintf(item,"�豸%dΪCH367�豸. ",m);
		else if(Devices[m][1]==1)
			sprintf(item,"�豸%dΪCH368�豸. ",m);
		strcat(preRead,item);
	}
	SetDlgItemText(mSaveDialogMain,IDC_STATUS,preRead);
}

/*============================= ���������� ==============================*/
BOOL PulwidthSetting()
{
	int j,x;
	UCHAR PulShow[10];
	j=SendMessage(GetDlgItem(mSaveDialogMain,IDC_CBPU),CB_GETCURSEL,0,0);
	CurPulWidth=PulWidth[j];
	if(CH367mWriteIoByte(mIndex,&mBaseAddr->mCH367Speed,CurPulWidth))//��CH367/CH368���ٶȼĴ�����д���������
	{
		MessageBox(mSaveDialogMain,"���óɹ�","��ʾ",MB_OK);
		x=CurPulWidth&0x0F;//���CurPulWidth�ĵ���λ
		sprintf((char*)PulShow,"%d\0",(x+1)*30);
		SetWindowText(GetDlgItem(mSaveDialogMain,IDC_PULSHOW), (char*)PulShow);//�����õ��ٶ���ʾ�����Ի�����
	}
	else
	{
		MessageBox(mSaveDialogMain,"����ʧ��","��ʾ",MB_OK|MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

/*=============================IO�Ķ�д ==============================*/
void mIoRead(HWND hDialog,int IoModel,int IoAddrType)//IO�ռ��
{
	mVAR_TYPE mVarType;
	UCHAR IoAddr[5],IoLen[5];
	UCHAR mBuf[mMAX_BUFFER_LENGTH]="";
	ULONG mLen=0,mAddr;
	mAddr=GetDlgItemText(hDialog,IDC_EDITIOADD, (char*)IoAddr,5);
	if(!mCheckInput(IoAddr))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(IoAddr,mAddr, 0);
	mAddr = mVarType.sVar;//�����ַ
	mLen=GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5);
	if(!mCheckInput(IoLen))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(IoLen);//���볤��
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"������С�ڵ���0x8000�ĳ���!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	if(GetDlgItemText(hDialog,IDC_EDITIOADD, (char*)IoAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5)==0)
	{
		MessageBox(hDialog,"�������ַ�����ݳ���","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	if(IoModel==0&&IoAddrType==0)//���ֽں�ƫ�Ƶ�ַ��дIO
	{
		if(!CH367mAccessBlock(mIndex,mFuncReadIoByte,&mBaseAddr->mCH367IoPort[mAddr],mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO��ȡʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j+1]=buffer[j+1]-32;
					j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==0&&IoAddrType==1)//���ֽں�ֱ�ӵ�ַ��дIO
	{
		if(!CH367mAccessBlock(mIndex,mFuncReadIoByte,(PVOID)mAddr,mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO��ȡʧ��","��ʾ",MB_OK);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==1&&IoAddrType==0)//���ֺ�ƫ�Ƶ�ַ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 4)||!mCheckWord((PVOID)mLen, 4))
		{
			MessageBox(hDialog, "�������ܴ洢�ֵ���ʼ��ַ�������ݳ���Ϊ2�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoWord,&mBaseAddr->mCH367IoPort[mAddr],mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO��ȡʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==1&&IoAddrType==1)//���ֺ�ֱ�ӵ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 4)||!mCheckWord((PVOID)mLen, 4))
		{
			MessageBox(hDialog, "�������ܴ洢�ֵ���ʼ��ַ�������ݳ���Ϊ2�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoWord,(PVOID)mAddr,mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO��ȡʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else if(IoModel==2&&IoAddrType==0)//��˫�ֺ�ƫ�Ƶ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 8)||!mCheckWord((PVOID)mLen, 8))
		{
			MessageBox(hDialog, "�������ܴ洢�ֵ���ʼ��ַ�����ݳ���Ϊ4�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoDword,&mBaseAddr->mCH367IoPort[mAddr],mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO��ȡʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
	else//��˫�ֺ�ֱ�ӵ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 8)||!mCheckWord((PVOID)mLen, 8))
		{
			MessageBox(hDialog, "�������ܴ洢�ֵ���ʼ��ַ�����ݳ���Ϊ4�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadIoDword,(PVOID)mAddr,mBuf,mLen))
		{
			SetDlgItemText(hDialog,IDC_EDITIOLEN,"");
			MessageBox(hDialog,"IO��ȡʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j+1]=buffer[j+1]-32;
				j += 3;
			}
			buffer[j]='\0';
			SetDlgItemText(hDialog,IDC_EDITIODATA,buffer);
		}
	}
}

void mIoWrite(HWND hDialog,int IoModel,int IoAddrType)//IO�ռ�д
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
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(IoAddr,mAddr, 0);
	mAddr = mVarType.sVar;//��ȡ�����ַ
	mLen=GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5);
	if(!mCheckInput(IoLen))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(IoLen);//��ȡ���볤��
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"������С�ڵ���0x8000�ĳ���!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	strLen=GetDlgItemText(hDialog,IDC_EDITIODATA, (char*)inputStr,mMAX_BUFFER_LENGTH);//�õ�������е�����
	for(i=0;i<strLen;i++)
	{
		if(inputStr[i]!=' ')
			bufferFilter[j++]=inputStr[i];
	}
	if(GetDlgItemText(hDialog,IDC_EDITIOADD, (char*)IoAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITIOLEN, (char*)IoLen,5)==0||GetDlgItemText(hDialog,IDC_EDITIODATA, (char*)IoLen,5)==0)
	{
		MessageBox(hDialog,"�������ַ,���ݳ��Ⱥ�д����������","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	if(mLen>j/2)//�����볤�Ⱥ����ݳ�����ȡ��С��ֵ
	{
		mLen=j/2;
	}
	memcpy(mBuf,bufferFilter,mLen*2);
	memcpy(buffer,mStrtoVal(mBuf,mLen*2),mLen);
	if(IoModel==0&&IoAddrType==0)//���ֽں�ƫ�Ƶ�ַ�ķ�ʽ��дIO
	{
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoByte,&mBaseAddr->mCH367IoPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
	else if(IoModel==0&&IoAddrType==1)//���ֽں�ֱ�ӵ�ַ�ķ�ʽ��дIO
	{
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoByte,(PVOID)mAddr,buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
	else if(IoModel==1&&IoAddrType==0)//���ֺ�ƫ�Ƶ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 4))
		{
			MessageBox(hDialog, "�������ܴ洢�ֵ���ʼ��ַ�����ݳ���Ϊ2�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoWord,&mBaseAddr->mCH367IoPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
	else if(IoModel==1&&IoAddrType==1)//���ֺ�ֱ�ӵ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 4))
		{
			MessageBox(hDialog, "�������ܴ洢�ֵ���ʼ��ַ�����ݳ���Ϊ2�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoWord,(PVOID)mAddr,buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
	else if(IoModel==2&&IoAddrType==0)//��˫�ֺ�ƫ�Ƶ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 8))
		{
			MessageBox(hDialog, "�������ܴ洢˫�ֵ���ʼ��ַ�����ݳ���Ϊ4�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoDword,&mBaseAddr->mCH367IoPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
	else//��˫�ֺ�ֱ�ӵ�ַ�ķ�ʽ��дIO
	{
		if(!mCheckWord((PVOID)mAddr, 8))
		{
			MessageBox(hDialog, "�������ܴ洢˫�ֵ���ʼ��ַ�����ݳ���Ϊ4�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteIoDword,(PVOID)mAddr,buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
}


/*============================= MEM��д ==============================*/
void mMemRead(HWND hDialog,int MemModel)//MEM������
{
	mVAR_TYPE mVarType;
	UCHAR MemAddr[5],MemLen[5];
	UCHAR mBuf[mMAX_BUFFER_LENGTH]="";
	ULONG mLen=0,mAddr;
	mAddr=GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5);
	if(!mCheckInput(MemAddr))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(MemAddr,mAddr, 0);
	mAddr = mVarType.sVar;//��������ַ
	mLen=GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemLen,5);
	if(!mCheckInput(MemLen))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(MemLen);//������볤��
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"������С�ڵ���0x8000�ĳ���!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	if(GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemAddr,5)==0)
	{
		MessageBox(hDialog,"�������ַ�����ݳ���","��ʾ",MB_OK|MB_ICONSTOP);
		return;
	}
	if(mAddr+mLen>0x7FFF)
	{
		MessageBox(hDialog,"������ĵ�ַ+����>0x7FFF��������,�����CH367mAccessBlock�����˵��","��ʾ",MB_OK|MB_ICONSTOP);
		return;
	}
	if(MemModel==0)
	{
		if(!CH367mAccessBlock(mIndex,mFuncReadMemByte,&mMemAddr->mCH368MemPort[mAddr],mBuf,mLen))
		{
			MessageBox(hDialog,"MEM��ȡʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
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
			MessageBox(hDialog, "�������ܴ洢˫�ֵ���ʼ��ַ�����ݳ���Ϊ4�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncReadMemDword,&mMemAddr->mCH368MemPort[mAddr],mBuf,mLen))
		{
			MessageBox(hDialog,"MEM��ȡʧ��","��ʾ",MB_OK);
			return;
		}
		else
		{
			char buffer[mMAX_BUFFER_LENGTH*3];
			ULONG i,j=0;
			for(i=0;i<mLen;i++)
			{
				sprintf(&buffer[j],"%2x ",mBuf[i]);    //��λʮ����������һ���ո�
				if (mBuf[i]<16 )                      //һλʮ�������ַ�ǰ���0
					buffer[j]=48;    				
				if (buffer[j]>=97 && buffer[j]<=122) //Сд��ĸתΪ��д��ĸ
					buffer[j]=buffer[j]-32;
				if (buffer[j+1]>=97 && buffer[j+1]<=122) //Сд��ĸתΪ��д��ĸ
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
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(MemAddr,mAddr, 0);
	mAddr = mVarType.sVar;//��ȡ�����ַ
	mLen=GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemLen,5);
	if(!mCheckInput(MemLen))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mLen=mStrToBcd(MemLen);//��ȡ���볤��
	if(mLen>=0x8000)
	{
		MessageBox(hDialog,"������С�ڵ���0x8000�ĳ���!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	strLen=GetDlgItemText(hDialog,IDC_EDITMEMDATA, (char*)inputStr,mMAX_BUFFER_LENGTH);//��ȡ��������
	for(i=0;i<strLen;i++)
	{
		if(inputStr[i]!=' ')
			bufferFilter[j++]=inputStr[i];
	}
	if(mAddr+mLen>0x7FFF)
	{
		MessageBox(hDialog,"������ĵ�ַ+����>0x7FFF��������,�����CH367mAccessBlock�����˵��","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	if(GetDlgItemText(hDialog,IDC_EDITMEMADD, (char*)MemAddr,5)==0||GetDlgItemText(hDialog,IDC_EDITMEMLEN, (char*)MemLen,5)==0||GetDlgItemText(hDialog,IDC_EDITMEMDATA, (char*)inputStr,mMAX_BUFFER_LENGTH)==0)
	{
		MessageBox(hDialog,"�������ַ,���ݳ��Ⱥ�д����������","��ʾ",MB_OK|MB_ICONSTOP);
		return;
	}
	if(mLen>j/2)//�����볤�Ⱥ����ݳ�����ȡ��С��ֵ
	{
		mLen=j/2;
	}
	memcpy(mBuf,bufferFilter,mLen*2);
	memcpy(buffer,mStrtoVal(mBuf,mLen*2),mLen);
	if(MemModel==0)//���ֽڵķ�ʽ��дMEM
	{
		if(!CH367mAccessBlock(mIndex,mFuncWriteMemByte,&mMemAddr->mCH368MemPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
	else//��˫�ֵķ�ʽ��дMEM
	{
		if(!mCheckWord((PVOID)mAddr, 8))
		{
			MessageBox(hDialog, "�������ܴ洢˫�ֵ���ʼ��ַ�����ݳ���Ϊ4�ı���", "��ʾ", MB_OK | MB_ICONSTOP);
			return;
		}
		if(!CH367mAccessBlock(mIndex,mFuncWriteMemDword,&mMemAddr->mCH368MemPort[mAddr],buffer,mLen))
		{
			MessageBox(NULL,"дʧ��","��ʾ",MB_OK|MB_ICONERROR);
			return;
		}
		else
		{
			MessageBox(NULL,"д�ɹ�","��ʾ",MB_OK);
		}
	}
}





/*============================= I2C��ش���==============================*/
void mI2CRead(HWND hDialog)//I2C��
{
	UCHAR dAddr[8];
	USHORT dAddrLen;
	UCHAR idAddr; // ���ݵ�Ԫ��ַ 
	UCHAR cByte;
	mVAR_TYPE mVarTyped;
	dAddrLen = GetDlgItemText(hDialog, IDC_EDITI2CADD, (char*)dAddr, 3); // ȡ���ݵ�Ԫ��ַ 
	if(!mCheckInput(dAddr))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarTyped = mCharToVar(dAddr, dAddrLen, 3);
	idAddr = mVarTyped.cVar;
	if(dAddrLen) // ȡ���ݵ�Ԫ��ַ 
	{
		if(CH367mReadI2C(mIndex, 0x50, idAddr, &cByte)) // ��I2C�ӿڶ�һ���ֽ� 
		{
			UCHAR sByte[8];
			sprintf((char*)sByte, "%02X\x0", cByte);
			SetDlgItemText(hDialog, IDC_EDITI2CDATA, (char*)sByte);
		}
		else
		{
			MessageBox(hDialog, "������ʧ��!", "��ʾ", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "�������ַ!", "��ʾ", MB_OK);
	}
}

void mI2CWrite(HWND hDialog)
{
	UCHAR dAddr[8];
	USHORT dAddrLen;
	UCHAR idAddr; // ���ݵ�Ԫ��ַ
	UCHAR oByte[8];
	UCHAR cByte;
	UINT oByteLen;
	mVAR_TYPE mVarByte;
	mVAR_TYPE mVarTyped;
	dAddrLen = GetDlgItemText(hDialog, IDC_EDITI2CADD, (char*)dAddr, 3); // ȡ���ݵ�Ԫ��ַ 
	if(!mCheckInput(dAddr))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarTyped = mCharToVar(dAddr, dAddrLen, 3);
	idAddr = mVarTyped.cVar;
	
	oByteLen = GetDlgItemText(hDialog, IDC_EDITI2CDATA, (char*)oByte, 3); // ȡ��д������ 
	if(!mCheckInput(oByte))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarByte = mCharToVar(oByte, oByteLen, 0);
	cByte = mVarByte.cVar;
	if(dAddrLen && oByteLen)
	{
		if(CH367mWriteI2C(mIndex, 0x50, idAddr, cByte)) // ��I2C��д��һ���ֽ� 
		{
			MessageBox(hDialog, "д���ݳɹ�!", "��ʾ", MB_OK);
		}
		else
		{
			MessageBox(hDialog, "д����ʧ��!", "��ʾ", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "�������ַ������!", "��ʾ", MB_OK);
	}
}

void eeprom_write(void) //�޸�VID,DID... 
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
		MessageBox(NULL,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // ȡVID 
	Value = mVarType.sVar;
	buffer[4] = Value & 0xFF;
	buffer[5] = Value >> 8;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_DID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // ȡVID 
	Value = mVarType.sVar;
	buffer[6] = Value & 0xFF;
	buffer[7] = Value >> 8;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_RID, (char*)data, 3);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 3); // ȡRID 
	buffer[8] = mVarType.cVar;
	buffer[9] = 0x00;
	buffer[10] = 0x00;
	buffer[11] = 0x10;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_SVID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // ȡSVID 
	Value = mVarType.sVar;
	buffer[12] = Value & 0xFF;
	buffer[13] = Value >> 8;
	Len = GetDlgItemText(mSaveDialogI2c, IDC_SID, (char*)data, 5);
	if(!mCheckInput(data))
	{
		MessageBox(NULL,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(data, Len, 0); // ȡSVID 
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
			Sleep(20); // ���Ҫ����ʱ 
		}
		else
		{
			MessageBox(mSaveDialogI2c, "дEEPROMʧ��,����EEPROM�𻵻�û������", mCaptionInform, MB_OK);
			return;
		}
	}
	MessageBox(mSaveDialogI2c, "дEEPROM���", mCaptionInform, MB_OK);
}

void eeprom_read(void) // ��EEPROM 
{
	CHAR i, sByte[8];
	UCHAR data[32];
	for(i = 0; i < 0x20; i++)
	{
		if(!CH367mReadI2C(mIndex, 0x50, i, &data[i]))
		{
			MessageBox(mSaveDialogI2c, "дEEPROMʧ��,����EEPROM�𻵻�û������", mCaptionInform, MB_OK);
			EndDialog(mSaveDialogI2c,2);
			return;
		}
	}
	sprintf(sByte, "%02X%02X\x0", data[5], data[4]);
	SetDlgItemText(mSaveDialogI2c, IDC_VID, sByte); // ���VID 
	sprintf(sByte, "%02X%02X\x0", data[7], data[6]);
	SetDlgItemText(mSaveDialogI2c, IDC_DID, sByte); // ���DID 
	sprintf(sByte, "%02X\x0", data[8]);
	SetDlgItemText(mSaveDialogI2c, IDC_RID, sByte); // ���RID 
	sprintf(sByte, "%02X%02X\x0", data[13], data[12]);
	SetDlgItemText(mSaveDialogI2c, IDC_SVID, sByte); // ���SVID 
	sprintf(sByte, "%02X%02X\x0", data[15], data[14]);
	SetDlgItemText(mSaveDialogI2c, IDC_SID, sByte); // ���SID 
}

INT_PTR CALLBACK mDialogI2c(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam)//I2C�Ի�������
{
	switch(uMessage)
	{
	case WM_INITDIALOG: // ��ʼ�� 
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
			eeprom_read(); // ��eeprom 
		}
		return TRUE;
	case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
			case IDC_PROGRAM: // дEEPROM 
				{
					eeprom_write();
				}
				return TRUE;
			case IDC_PROMREAD://��EEPROM
				{
					eeprom_read();
				}
				return TRUE;
			case WM_DESTROY: // �˳� 
				EnableWindow(GetDlgItem(mSaveDialogMain, IDC_BTNI2C_PROGRAM), TRUE);
				EndDialog(hDialog, 1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;  
}


/*============================= ���ÿռ�Ķ�д ==============================*/
void mConRead(HWND hDialog)//���ÿռ��
{
	UCHAR iAddr[8];
	USHORT mAddr;
	UCHAR cByte;
	UINT iAddrLen;
	mVAR_TYPE mVarType;
	iAddrLen = GetDlgItemText(hDialog, IDC_EDITCONADD, (char*)iAddr, 5); // ȡ��ַ 
	if(!mCheckInput(iAddr))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(iAddr,iAddrLen,0);
	mAddr = mVarType.sVar;
	if(iAddrLen) 
	{
		if(CH367mReadConfig(mIndex, (PVOID)mAddr, &cByte)) /* �����ÿռ��һ���ֽ� */
		{
			UCHAR sByte[8];
			sprintf((char*)sByte, "%02X\x0", cByte);
			SetDlgItemText(hDialog, IDC_EDITCONDATA, (char*)sByte);
		}
		else
		{
			MessageBox(hDialog, "������ʧ��!", "��ʾ", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "�������ַ!", "��ʾ", MB_OK);
	}
}

void mConWrite(HWND hDialog)//���ÿռ�д
{
	UCHAR iAddr[8];
	USHORT mAddr;
	UCHAR oByte[8];
	UCHAR cByte;
	int iAddrLen;
	mVAR_TYPE mVarType;
	mVAR_TYPE mVarByte;
	int oByteLen = 0;
	iAddrLen = GetDlgItemText(hDialog, IDC_EDITCONADD, (char*)iAddr, 8); // ȡ��ַ 
	if(!mCheckInput(iAddr))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarType = mCharToVar(iAddr,iAddrLen,0);
	mAddr = mVarType.sVar;	
	oByteLen=GetDlgItemText(hDialog, IDC_EDITCONDATA, (char*)oByte, 8); // ȡ���� 
	if(!mCheckInput(oByte))
	{
		MessageBox(hDialog,"��������ַ�����������0~9,a~f,A~F֮���ʮ��������!","��ʾ",MB_OK|MB_ICONERROR);
		return;
	}
	mVarByte=mCharToVar(oByte,oByteLen,3);
	cByte = mVarByte.cVar;
	if(iAddrLen && oByteLen)	
	{
		if(CH367mWriteConfig(mIndex, (PVOID)mAddr, cByte)) // �����ÿռ�дһ���ֽ� 
		{	
			MessageBox(hDialog, "д���ݳɹ�!", "��ʾ", MB_OK);
		}
		else
		{
			MessageBox(hDialog, "д����ʧ��!", "��ʾ", MB_OK | MB_ICONSTOP);
		}
	}
	else
	{
		MessageBox(hDialog, "�������ַ������!", "��ʾ", MB_OK);
	}
}


/*============================= SPI 25VF512 ��ش��� ==============================*/

unsigned char write_enable(void) // дʹ�� 
{
	unsigned char buffer[8];
	buffer[0] = WREN;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char write_disable(void) // д��ֹ 
{
	unsigned char buffer[8];
	buffer[0] = WRDI;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char read_status_register(unsigned char *data) //��״̬�Ĵ��� 
{
	unsigned char buffer[8];
	unsigned long len = 0x01;
	buffer[0] = RDSR;
	if( CH367StreamSPI(mIndex, 0x01, buffer, &len, data) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char write_status_register_enable(void) // д״̬�Ĵ���ʹ�� 
{
	unsigned char buffer[8];
	buffer[0] = EWSR;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char write_status_register(unsigned char status) // д״̬�Ĵ��� 
{
	unsigned char buffer[8];
	buffer[0] = WRSR;
	buffer[1] = status;
	if( CH367StreamSPI(mIndex, 0x02, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char check(void) // ���WELλ 
{
	unsigned char data;
	int count = 10000;
	do{
		read_status_register(&data);
		if(count-- == 0) return FALSE;
	}while(data & 0x01);
	return TRUE;
}

unsigned char byte_program(unsigned long address, unsigned char data) // �ֽ�д 
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

unsigned char sector_erase(unsigned long address) // �������� 
{
	unsigned char buffer[8];
	buffer[0] = SE;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF); 
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	if( CH367StreamSPI(mIndex, 0x04, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;
}

unsigned char bulk_erase(unsigned long address) // ����� 
{
	unsigned char buffer[8];
	buffer[0] = BE;
	buffer[1] = (unsigned char)(address >> 16 & 0xFF);
	buffer[2] = (unsigned char)(address >> 8 & 0xFF);
	buffer[3] = (unsigned char)(address & 0xFF);
	if( CH367StreamSPI(mIndex, 0x04, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;	
}

unsigned char chip_erase(void) // оƬ����
{
	unsigned char buffer[8];
	buffer[0] = CE;
	if( CH367StreamSPI(mIndex, 0x01, buffer, NULL, NULL) == FALSE ) return FALSE;
	return TRUE;		
}

unsigned char read_block(unsigned long address, unsigned long length, unsigned char *data) // ��FLASH 
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

unsigned char read_block_fast(unsigned long address, unsigned long length, unsigned char *data) // ���ٶ�FLASH 
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

BOOL read_manufacturer_device_id(unsigned char *data) // ������/�豸ID 
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

void load_file(void) // ѡ�񽫷��͵��ļ� 
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
	openfilename.lpstrTitle		   = "ѡ�񼴽�д���Ŀ���ļ�";
	openfilename.nFileOffset       = 0;
	openfilename.nFileExtension    = 0;
	openfilename.lpstrDefExt       = NULL;
	openfilename.lCustData         = 0;
	openfilename.lpfnHook 		   = NULL;
	openfilename.lpTemplateName    = NULL;
	openfilename.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_READONLY | OFN_FILEMUSTEXIST;
	if (GetOpenFileName(&openfilename) ) // ��ȡ�ļ�·�������浽ָ���Ļ����� 
	{
		memset(SpiFilePath, 0x00, sizeof(SpiFilePath));
		memcpy(SpiFilePath, openfilename.lpstrFile, strlen(openfilename.lpstrFile)); 
	}
}

void read_flash(void) // ��FLASH����д���ļ� 
{
	OPENFILENAME openfilename;  
	CHAR szFile[260] = "\0";
	HANDLE hFile;
	char DirName[256];
	unsigned long address, mTotal = 0;
	unsigned char *p, buffer[1024 * 64];
	DWORD Num;
	p = buffer;
	GetCurrentDirectory(sizeof(DirName), DirName); // ��ȡ��ǰ��·�� 
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
	openfilename.lpstrTitle        = "�����ļ�";
	openfilename.nFileOffset       = 0;
	openfilename.nFileExtension    = 0;
	openfilename.lpstrDefExt       = __TEXT("txt\0");
	openfilename.lCustData         = 0;
	openfilename.lpfnHook 		   = NULL;
	openfilename.lpTemplateName    = NULL;
	openfilename.Flags             = OFN_SHOWHELP | OFN_EXPLORER | OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT;
	
	if (GetSaveFileName(&openfilename)) // ����ָ�����ֵ��ļ�
	{  
		hFile = CreateFile ( szFile,  
			GENERIC_WRITE | GENERIC_READ,  
			FILE_SHARE_READ | FILE_SHARE_WRITE,  
			NULL,  
			CREATE_ALWAYS,  
			FILE_ATTRIBUTE_NORMAL,  
			NULL);  
		if (hFile != INVALID_HANDLE_VALUE) // ����ָ�����ֵ��ļ��ɹ� 
		{  
			for(address = 0; address < 0x10000; address += SPI_BUFFER_LENGTH)
			{
				// ��FLASH ÿ�ζ�256�ֽ� 
				if(CH364mReadFlash(0,address,p, SPI_BUFFER_LENGTH))
				{
					mTotal = mTotal + SPI_BUFFER_LENGTH;
					p += SPI_BUFFER_LENGTH;
				}
				else
				{
					MessageBox(mSaveDialogMain, "��FLASHʧ��", mCaptionInform, MB_ICONSTOP | MB_OK);
					CloseHandle(hFile);
					return;
				}
			}
			if ( ! WriteFile(hFile, buffer, mTotal, &Num, NULL) ) // �ѽ��ջ������е�����д���������ļ���,��������ʱ,�رմ������ļ���� 
			{ 
				MessageBox(mSaveDialogMain, "д�ļ�ʧ��", mCaptionInform, MB_ICONSTOP | MB_OK);
				CloseHandle(hFile);
				return;
			}
			MessageBox(mSaveDialogMain, "��FLASH�ɹ�", mCaptionInform, MB_OK);
			CloseHandle(hFile);
		}
		else // ��������ʱ, �رմ������ļ����  
		{  
			CloseHandle(hFile);
			return;
		}
	}
}

void write_flash(void) // �Ѵ򿪵��ļ�д��FLASH 
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
	memcpy(pFileN, SpiFilePath, strlen(SpiFilePath)); // �����ļ�·����pFileN��������
	pFileN[strlen(SpiFilePath)] = '\0';
	if(pFileN[0] == '\0') // ��û��ѡ����ļ�,��ʾ 
	{  
		MessageBox(mSaveDialogMain, "����ļ�", mCaptionInform, MB_ICONSTOP);
	}
	else 
	{  
		if ( ( hFile = CreateFile (  // ��ѡ����ļ�����ȡ�ļ���� 
			( LPCTSTR ) pFileN,
			GENERIC_READ,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			( HANDLE ) NULL ) ) == ( HANDLE ) -1 )
		{ 
			MessageBox(mSaveDialogMain, "����/���ļ�����", mCaptionInform, MB_ICONSTOP);
			return;
		}
		mFileSize = GetFileSize (hFile, NULL); // ��ȡ�ļ��Ĵ�С 
		if(mFileSize > 1024 * 64)
		{
			MessageBox(mSaveDialogMain, "ֻ�ܴ�С��64K���ļ�", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile); 
			return;
		}
		if(mFileSize == 0xFFFFFFFF)
		{  
			MessageBox(mSaveDialogMain, "��ȡ�ļ��Ĵ�С����", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile);
			return;
		}
		mBuffer = (UCHAR *)malloc(mFileSize); // ������ļ��Ļ����� 
		if(mBuffer == NULL)
		{ 
			MessageBox(mSaveDialogMain, "���仺��������", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile);
			free(mBuffer);
			return;
		}
		memset(mBuffer, '\0', mFileSize); // ��ջ����� 
		if (!(ReadFile(hFile, mBuffer, mFileSize, &i, NULL))) // ��ȡ�ļ����ݵ�����Ļ�������,��������ʱ,�ر��ļ����,�ͷŻ�����
		{  
			MessageBox(mSaveDialogMain, "���ļ�����", mCaptionInform, MB_ICONSTOP);
			CloseHandle(hFile);
			free(mBuffer);
			return;
		}
		CloseHandle(hFile);
		/*
		write_enable(); // дʹ�� 
		write_status_register_enable();
		write_status_register(0x00);
		//write_disable(); // д��ֹ 

		for(address = 0; address < mFileSize; address++)
		{
			write_enable(); // дʹ�� 
			byte_program(address, *mBuffer); // �ֽ�д 
			//write_disable(); // д��ֹ 
			if(check()) // ���WELλ 
			{
				mBuffer++;
			}
			else
			{
				MessageBox(mSaveDialogMain, "дFLASHʧ��,����FLASH�Ѿ���,����û������", mCaptionInform, MB_OK); // ����flash�Ѿ��� 
				return;
			}
		}
		*/
		CH364mWriteFlash(0,0,mBuffer,mFileSize);
		MessageBox(mSaveDialogMain, "дFLASH���", mCaptionInform, MB_OK);
	}
}

void erase_flash(void) // ����FLASH
{
	/*
	write_enable(); // дʹ�� 
	if(chip_erase())
	{
		MessageBox(mSaveDialogMain, "����FLASH�ɹ�", mCaptionInform, MB_OK);
	}
	else
	{
		MessageBox(mSaveDialogMain, "����FLASHʧ��", mCaptionInform, MB_ICONSTOP | MB_OK);
	}
	//write_disable(); // д��ֹ 
	*/
	
	CH364mLockFlash(0,0);
	if( CH364mEarseFlash(0,0,64*1024))
	{
		MessageBox(mSaveDialogMain, "����FLASH�ɹ�", mCaptionInform, MB_OK);
	}
	else
	{
		MessageBox(mSaveDialogMain, "����FLASHʧ��", mCaptionInform, MB_ICONSTOP | MB_OK);
	}
}




/*============================= �ж���ش��� ==============================*/

void CALLBACK InterruptEvent(void) // �豸�ж�֪ͨ��Ϣ 
{	
	UCHAR oByte;
	if(intType==1)
	{
		mCount++; // �жϴ������� 
		CH367_GPO_SET;//���жϵ�ƽ������Ч
	}
	if(intType==2)
	{
		mCount1++;
		CH367mReadIoByte(mIndex, &mBaseAddr->mCH367Status,&oByte);
		CH367mWriteIoByte(mIndex, &mBaseAddr->mCH367Status,oByte&0xFB);//��������ƺ�״̬�Ĵ����еļ���״̬�豸���ж�
	}
	PostMessage(mSaveDialogMain, WM_INTNOTIFY, 0, 0); // �����豸�ж���Ϣ 
}

/*============================= HELP��ش��� ==============================*/
LPCTSTR help_buf = "1-֧��4���豸 \r\n\
  ͨ���豸��ѡ������ĸ��豸 \r\n\
  0Ϊ��һ���豸(��CPU������豸) \r\n\
  1Ϊ�ڶ����豸 \r\n\
  2Ϊ�������豸 \r\n\
  3Ϊ���ĸ��豸 \r\n\
2-CH367,CH368�豸ͨ��,�ó����Զ��ж��豸����\r\n\
  ע�⣺����ͨ����ȡ�豸��ʶ��ʶ��CH367��CH368�豸��������������豸��\r\n\
  ʶ,���豸�����üĴ����е�02H�ռ�,�뽫��������Ӧ�Ĵ������һ��,����\r\n\
  ����������CH367mReadConfig(mIndex,(PVOID)0x02, &mByte)��\r\n\
  CH367mReadConfig(mIndex,(PVOID)0x02, &cByte)���ҵ���Ӧ�Ĵ������һ��\r\n\
3-I/O��д \r\n\
  CH367,CH368֧�����ֽ�/��/˫��ģʽ��I/O�˿ڷ���,Ĭ��Ϊ�ֽڷ�ʽ���� \r\n\
  ֧��ֱ�ӵ�ַ/ƫ�Ƶ�ַ����I/O�˿� \r\n\
      ֱ�ӵ�ַ���Է��������豸��I/O�ռ� \r\n\
      ƫ�Ƶ�ַֻ�ܷ���CH367/CH368��I/O�ռ� \r\n\
  ֧�ֿ��/д���� \r\n\
  ע�⣺��ѡ����,˫��ģʽ���ж�дIOʱ,����ĵ�ַ�ͳ�����ֱ�Ϊ2��4�ı���,\r\n\
  ���д����󳤶�Ϊ32KB\r\n\
4-MEM��д\r\n\
  ��CH368֧�����ֽ�/˫��ģʽ��MEM���ж�д,�����е������ַΪƫ�Ƶ�ַ,��\r\n\
  ѡ��˫�ֽ��ж�д˫�ֽ��ж�дʱ,����ĵ�ַ�����ݳ�����Ϊ4�ı���,����ĵ�\r\n\
  ַ�����ݳ��ȵĺ�ӦС�ڵ���0x7FFF\r\n\
5-֧��CH367,CH368������������\r\n\
6-I2C��д \r\n\
  CH367/CH368֧��I2C�ӿ�������ͨѶ,CH367/CH368��������ʾ����24C02,֧\r\n\
  �ֶ�/дEEPROM\r\n\
7-SPI�ӿ�FLASH���� \r\n\
  CH367/CH368֧��SPI�ӿ�������ͨѶ,CH367/CH368��������ʾ����25VF512,\r\n\
  ֧�ֶ�/д/����FLASH \r\n\
8-���ÿռ��д \r\n\
  ֧���ֽڶ�/д����,��ϸ˵�������CH367/CH368оƬ�ֲ� \r\n\
9-�жϲ��� \r\n\
  CH367/CH368֧�ֵ�ƽ/�����ж�,ͨ�����豸ʱ�����жϼĴ���,����ѡ��\r\n\
  �ж�����,��ϸ˵�������CH367/CH368оƬ�ֲ���IO��ַ�Ĵ�����Ӳ���жϵ� \r\n\
  ˵��:\r\n\
  a)��ƽ�ж�\r\n\
  ���ó���ͨ���������GP0������INT#�����ж�,�ڲ���ʱ���INT#��A15�̽� \r\n\
  ���ó���ʹ�õ͵�ƽ��Ч�ĵ�ƽ������ʽ,�͵�ƽ��Ч \r\n\
  ���ó���ͨ��\"���Ե�ƽ�ж�\"����������GP0����һ���ߵ�ƽ�źź�һ����\r\n\
    ��ƽ�źŴӶ������ж�,�����ж��Ժ����һ���ߵ�ƽȡ���ж�����\r\n\
  b)�����ж�\r\n\
  ���ó���ͨ���������GP0������INT#�����ж�,�ڲ���ʱ���INT#��A15�̽� \r\n\
  ���ó���ʹ����������Ч�ı��ش�����ʽ\r\n\
  ���ó���ͨ��\"���Ա����ж�\"����������GP0��ģ������������ź�,��INT#��\r\n\
    �յ��������źź�����жϼ����������ж��Ժ�����CH367/CH368���жϼ���\r\n\
    ״̬λΪ0,�Ӷ�ȡ���ж�\r\n\
";
  
  

void help(void) // ��ʾhelp 
{
	SetWindowText(GetDlgItem(mSaveDialogHelp, IDC_EDIT_HELP), help_buf);
}

INT_PTR CALLBACK mDialogHelp(HWND hDialog, UINT uMessage, WPARAM wParam, LPARAM lParam) /* HELP�Ի����¼� */
{
	switch(uMessage)
	{
	case WM_INITDIALOG: // ��ʼ�� 
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
			case WM_DESTROY: // �˳� 
				EndDialog(hDialog, 1);
				return TRUE;
			}
		}
		break;
	}
	return FALSE;  
}


