#include "pch.h"
#include "RSA.h"
// ���� �׽�Ʈ ��� int������ ���� �߻� ���ɼ����� -> long long int(__int64) ���

__int64 gcd(__int64 a, __int64 b) // ��Ŭ���� ȣ���� �ο�
{
	if (b == 0) return a;
	return gcd(b, a % b);
}

void cal_pq(__int64* p, __int64* q) // �Ҽ� �迭 �˰���
{
	srand(time(NULL));
	int* temp = new int[9872];
	int i, j, cnt = 0;
	for (i = 128; i <= 10000; i++) // ascii ��+1(128) ~ 10000 ������ 
	{
		for (j = 2; j < i; j++)
		{
			if ((i % j) == 0)
				break;
		}
		if (i == j) // % 1�� i���� ����� ������ �Ҽ�
		{
			temp[cnt] = i;
			cnt++;
		}
	}
	*p = temp[rand() % cnt]; // ���� �ε���
	do
	{
		*q = temp[rand() % cnt];
	} while (*p == *q); // p�� q�� ������ �ȵ�
	delete[] temp;
}

void cal_e(__int64 phi, __int64* e) // gcd(i, phi) == 1 && 1 < e < phi ����Ű
{
	srand(time(NULL));
	__int64 i = rand() % phi + 2;
	do
		i++;
	while (gcd(i, phi) != 1 || i > phi); // �ִ������� 1 �׸��� i�� phi���� ������ ��.
	*e = i;
}

void cal_d(__int64 e, __int64 phi, __int64* d) // (e * d) % phi == 1, e�� �ɸ��� �ð��� �̿� ����Ű
{
	srand(time(NULL));
	__int64 i = rand();
	do
		i++;
	while ((e * i) % phi != 1); // ed mod N = 1 �� ����
	*d = i;
}

__int64* RSAencrypt(char* m, __int64 e, __int64 N)
{
	int arrlen = strlen(m);
	__int64* enarr = new __int64[arrlen];
	for (int i = 0; i < arrlen; i++) // ���ĺ� ���� 0~25
		enarr[i] = m[i]; // �ε��� 0 ���̽�

	for (int i = 0; i < arrlen; i++) // �迭 ��ü ����
	{
		__int64 power = enarr[i]; // ���� ���� ����
		for (int j = 0; j < e - 1; j++) // ������ŭ �ڱ� �ڽ� ���� 1~e-1 ����
		{
			enarr[i] *= power; // ���� ���� ��
			enarr[i] %= N; // ������ ����
		}
	}
	return enarr; //__int64* ��ȯ
}

CString RSA(const CString& pt, CString& input_privatekeytemp, CString& input_primeproducttemp)
{
	// CString ���� char*�� ��ȯ
	int ptlen = pt.GetLength();
	char* ptch = new char[ptlen + 1];
	WideCharToMultiByte(CP_ACP, 0, pt, -1, ptch, ptlen + 1, NULL, NULL); // �����ڵ�� ��ȯ

	// RSA �˰���
	__int64 p, q; // �Ҽ�
	__int64 N; // n = p * q
	__int64 phi; // phi = (p - 1) * (q - 1)
	__int64 e; // phi ���� �۰�, phi�� ���μ��� ����
	__int64 d; // (e*d) % phi = 1 -> d = ((1/phi)/e)
	cal_pq(&p, &q);
	N = p * q; // N ���ϱ�
	phi = (p - 1) * (q - 1); // ���Ϸ� �� �Լ�(n) ���ϱ�
	cal_e(phi, &e); // 1 < e < phi ���ϱ� 
	cal_d(e, phi, &d); // (e*d)%phi == 1 ���ϱ�

	__int64* enarr = RSAencrypt(ptch, e, N);
	delete[] ptch;

	// int * -> char* ��ȯ
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

	// char* -> CString �Է�
	CString RSAenarr;
	RSAenarr.Format(_T("%S"), (LPCTSTR)result);

	// d, n ��ȯ
	input_primeproducttemp.Format(_T("%d"), (LPCTSTR)d);
	input_privatekeytemp.Format(_T("%d"), (LPCTSTR)N);

	return RSAenarr;
}

/* // main() RSA debug
	__int64 * result = RSAencrypt(ptch, e, N);

	printf_s("p = %lld, q = %lld, N = %lld, phi = %lld, e = %lld, d = %lld\n", p, q, N, phi, e, d);
	printf_s("��:");
	for (int i = 0; i < ptlen; i++)
		printf_s("%d ", ptch[i]);
	printf_s("\n");
	printf_s("%s\n", ptch);

	printf_s("��ȣ��:");
	__int64 *testen = RSAencrypt(ptch, e, N);
	for (int i = 0; i < _msize(testen)/sizeof(__int64); i++)
		printf_s("%lld ", testen[i]);
	printf_s("\n");

	printf_s("��ȣ��:\n");
	char* testde = RSAdecrypt(testen, d, N);
	for (int i = 0; i < ptlen; i++)
		printf_s("%d ", testde[i]);
	printf_s("\n");
	printf_s("���� testde: %d\n", strlen(testde));
	printf_s("��ȣȭ ����:%s", testde);

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