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
	// CString 에서 char*로 변환
	int ptlen = pt.GetLength();
	char* ptch = new char[ptlen + 1];
	WideCharToMultiByte(CP_ACP, 0, pt, -1, ptch, ptlen + 1, NULL, NULL);
	// 변환 끝

	//Base64 알고리즘
	char result[2001] = { '\0' };
	char* ptarr = ptch; // 원본배열 포인터
	int ei = 0;
	for (int i = 0; i < (ptlen / 3); i++) // 0x3F == 00111111 마스크
	{
		result[ei++] = base64tables[ptarr[0] >> 2 & 0x3F]; // 오른쪽 2 bit 삭제
		result[ei++] = base64tables[((ptarr[0] << 4) | (ptarr[1] >> 4)) & 0x3F]; // 1,2 번 비트 각각 4비트 제거 후 or로 병합 후 마스크
		result[ei++] = base64tables[((ptarr[1] << 2) | (ptarr[2] >> 6)) & 0x3F];
		result[ei++] = base64tables[ptarr[2] & 0x3F];
		ptarr += 3; // 인코딩 후 3만큼 옮겨 감
	}

	// 남은 1~2 글자 처리
	int remain = ptlen % 3;
	switch (remain)
	{
	case 0: // 처리할 글자가 없음.
		break;
	case 1: // 1개 처리
		result[ei++] = base64tables[ptarr[0] >> 2 & 0x3F]; // 오른쪽 2 bit 삭제
		result[ei++] = base64tables[((ptarr[0] << 4) | (ptarr[1] >> 4)) & 0x3F];
		//패딩 2개
		result[ei++] = '=';
		result[ei++] = '=';
		break;
	case 2: // 2개 처리
		result[ei++] = base64tables[ptarr[0] >> 2 & 0x3F]; // 오른쪽 2 bit 삭제
		result[ei++] = base64tables[((ptarr[0] << 4) | (ptarr[1] >> 4)) & 0x3F]; // 1,2 번 비트 각각 4비트 제거 후 or로 병합 후 마스크
		result[ei++] = base64tables[((ptarr[1] << 2) | (ptarr[2] >> 6)) & 0x3F];
		// 패딩
		result[ei++] = '=';
		break;
	default:
		CString error = _T("Base64 연산 오류\n");
		return error;
	}

	// char[] -> CString
	CString Base64enarr;
	Base64enarr.Format(_T("%S"), (LPCTSTR)result);
	return Base64enarr;
}