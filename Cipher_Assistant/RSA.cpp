#include "pch.h"
#include "RSA.h"
// 연산 테스트 결과 int형으로 오류 발생 가능성있음 -> long long int(__int64) 사용

__int64 gcd(__int64 a, __int64 b) // 유클리드 호제법 인용
{
	if (b == 0) return a;
	return gcd(b, a % b);
}

void cal_pq(__int64* p, __int64* q) // 소수 배열 알고리즘
{
	srand(time(NULL));
	int* temp = new int[9872];
	int i, j, cnt = 0;
	for (i = 128; i <= 10000; i++) // ascii 끝+1(128) ~ 10000 까지의 
	{
		for (j = 2; j < i; j++)
		{
			if ((i % j) == 0)
				break;
		}
		if (i == j) // % 1와 i까지 약수가 없을때 소수
		{
			temp[cnt] = i;
			cnt++;
		}
	}
	*p = temp[rand() % cnt]; // 랜덤 인덱스
	do
	{
		*q = temp[rand() % cnt];
	} while (*p == *q); // p랑 q가 같으면 안됨
	delete[] temp;
}

void cal_e(__int64 phi, __int64* e) // gcd(i, phi) == 1 && 1 < e < phi 공개키
{
	srand(time(NULL));
	__int64 i = rand() % phi + 2;
	do
		i++;
	while (gcd(i, phi) != 1 || i > phi); // 최대공약수가 1 그리고 i가 phi보다 작으면 됨.
	*e = i;
}

void cal_d(__int64 e, __int64 phi, __int64* d) // (e * d) % phi == 1, e가 걸리는 시간을 이용 개인키
{
	srand(time(NULL));
	__int64 i = rand();
	do
		i++;
	while ((e * i) % phi != 1); // ed mod N = 1 인 정수
	*d = i;
}

__int64* RSAencrypt(char* m, __int64 e, __int64 N)
{
	int arrlen = strlen(m);
	__int64* enarr = new __int64[arrlen];
	for (int i = 0; i < arrlen; i++) // 알파벳 기준 0~25
		enarr[i] = m[i]; // 인덱스 0 베이스

	for (int i = 0; i < arrlen; i++) // 배열 전체 접근
	{
		__int64 power = enarr[i]; // 원본 숫자 저장
		for (int j = 0; j < e - 1; j++) // 제곱만큼 자기 자신 제외 1~e-1 까지
		{
			enarr[i] *= power; // 원본 숫자 곱
			enarr[i] %= N; // 나머지 연산
		}
	}
	return enarr; //__int64* 반환
}

CString RSA(const CString& pt, CString& input_privatekeytemp, CString& input_primeproducttemp)
{
	// CString 에서 char*로 변환
	int ptlen = pt.GetLength();
	char* ptch = new char[ptlen + 1];
	WideCharToMultiByte(CP_ACP, 0, pt, -1, ptch, ptlen + 1, NULL, NULL); // 유니코드로 변환

	// RSA 알고리즘
	__int64 p, q; // 소수
	__int64 N; // n = p * q
	__int64 phi; // phi = (p - 1) * (q - 1)
	__int64 e; // phi 보다 작고, phi와 서로소인 정수
	__int64 d; // (e*d) % phi = 1 -> d = ((1/phi)/e)
	cal_pq(&p, &q);
	N = p * q; // N 구하기
	phi = (p - 1) * (q - 1); // 오일러 피 함수(n) 구하기
	cal_e(phi, &e); // 1 < e < phi 구하기 
	cal_d(e, phi, &d); // (e*d)%phi == 1 구하기

	__int64* enarr = RSAencrypt(ptch, e, N);
	delete[] ptch;

	// int * -> char* 변환
	char result[2001] = { '\0' };
	for (int i = 0; i < ptlen; i++)
	{
		char temp[50] = { '\0' };
		if (i == ptlen - 1)
			sprintf_s(temp, "%lld", enarr[i]);
		else
			sprintf_s(temp, "%lld ", enarr[i]);
		strcat_s(result, temp);
	}
	delete[] enarr;

	// char* -> CString 입력
	CString RSAenarr;
	RSAenarr.Format(_T("%S"), (LPCTSTR)result);

	// d, n 반환
	input_primeproducttemp.Format(_T("%d"), (LPCTSTR)d);
	input_privatekeytemp.Format(_T("%d"), (LPCTSTR)N);

	return RSAenarr;
}

/* // main() RSA debug
	__int64 * result = RSAencrypt(ptch, e, N);

	printf_s("p = %lld, q = %lld, N = %lld, phi = %lld, e = %lld, d = %lld\n", p, q, N, phi, e, d);
	printf_s("평문:");
	for (int i = 0; i < ptlen; i++)
		printf_s("%d ", ptch[i]);
	printf_s("\n");
	printf_s("%s\n", ptch);

	printf_s("암호문:");
	__int64 *testen = RSAencrypt(ptch, e, N);
	for (int i = 0; i < _msize(testen)/sizeof(__int64); i++)
		printf_s("%lld ", testen[i]);
	printf_s("\n");

	printf_s("복호문:\n");
	char* testde = RSAdecrypt(testen, d, N);
	for (int i = 0; i < ptlen; i++)
		printf_s("%d ", testde[i]);
	printf_s("\n");
	printf_s("길이 testde: %d\n", strlen(testde));
	printf_s("복호화 문장:%s", testde);

	delete[] testen;
	delete[] testde;
	delete[] ptch;
	*/

	/* // RSA decrypto debug
	char* RSAdecrypt(__int64* c, __int64 d, __int64 N)
	{
		int arrlen = _msize(c) / sizeof(__int64);
		char* dearr = new char[arrlen+1];
		__int64* enarrcpy = new __int64[arrlen];

		for (int i = 0; i < arrlen; i++)
			enarrcpy[i] = c[i];

		for (int i = 0; i < arrlen; i++)
		{
			__int64 power = enarrcpy[i];
			for (int j = 0; j < d-1; j++)
			{
				enarrcpy[i] *= power;
				enarrcpy[i] %= N;
			}
		}
		for (int i = 0; i < arrlen; i++)
			dearr[i] = (char)enarrcpy[i];

		return dearr;
	}
	*/