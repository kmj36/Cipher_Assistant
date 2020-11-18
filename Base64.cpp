#include "pch.h"
#include "Base64.h"

// Base64 Tables
char base64tables[] =
{
	'A','B','C','D','E','F','G','H',
	'I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X',
	'Y','Z','a','b','c','d','e','f',
	'g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v',
	'w','x','y','z','0','1','2','3',
	'4','5','6','7','8','9','+','/'
};

CString Base64(const CString& pt)
{
	// CString ���� char*�� ��ȯ
	int ptlen = pt.GetLength();
	char* ptch = new char[ptlen + 1];
	WideCharToMultiByte(CP_ACP, 0, pt, -1, ptch, ptlen + 1, NULL, NULL);
	// ��ȯ ��

	//Base64 �˰���
	char result[2001] = { '\0' };
	char* ptarr = ptch; // �����迭 ������
	int ei = 0;
	for (int i = 0; i < (ptlen / 3); i++) // 0x3F == 00111111 ����ũ
	{
		result[ei++] = base64tables[ptarr[0] >> 2 & 0x3F]; // ������ 2 bit ����
		result[ei++] = base64tables[((ptarr[0] << 4) | (ptarr[1] >> 4)) & 0x3F]; // 1,2 �� ��Ʈ ���� 4��Ʈ ���� �� or�� ���� �� ����ũ
		result[ei++] = base64tables[((ptarr[1] << 2) | (ptarr[2] >> 6)) & 0x3F];
		result[ei++] = base64tables[ptarr[2] & 0x3F];
		ptarr += 3; // ���ڵ� �� 3��ŭ �Ű� ��
	}

	// ���� 1~2 ���� ó��
	int remain = ptlen % 3;
	switch (remain)
	{
	case 0: // ó���� ���ڰ� ����.
		break;
	case 1: // 1�� ó��
		result[ei++] = base64tables[ptarr[0] >> 2 & 0x3F]; // ������ 2 bit ����
		result[ei++] = base64tables[((ptarr[0] << 4) | (ptarr[1] >> 4)) & 0x3F];
		//�е� 2��
		result[ei++] = '=';
		result[ei++] = '=';
		break;
	case 2: // 2�� ó��
		result[ei++] = base64tables[ptarr[0] >> 2 & 0x3F]; // ������ 2 bit ����
		result[ei++] = base64tables[((ptarr[0] << 4) | (ptarr[1] >> 4)) & 0x3F]; // 1,2 �� ��Ʈ ���� 4��Ʈ ���� �� or�� ���� �� ����ũ
		result[ei++] = base64tables[((ptarr[1] << 2) | (ptarr[2] >> 6)) & 0x3F];
		// �е�
		result[ei++] = '=';
		break;
	default:
		CString error = _T("Base64 ���� ����\n");
		return error;
	}

	// char[] -> CString
	CString Base64enarr;
	Base64enarr.Format(_T("%S"), (LPCTSTR)result);
	return Base64enarr;
}