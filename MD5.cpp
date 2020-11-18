#include "pch.h"
#include "MD5.h"

// MD5는 리틀 엔디언 방식으로 구현을 해야하는 알고리즘.
// 위키백과, github 참조
// MD5 알고리즘:pod32g github 일부 인용

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

const uint32_t k[] = { // 값들 == floor(abs(sin(i+1)) * (2 pow 32)) 소수점삭제(절대값(삼각함수(i+1)) 곱하기 (2의 32 제곱))
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee ,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501 ,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be ,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821 ,
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa ,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8 ,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed ,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a ,
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c ,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70 ,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05 ,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665 ,
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039 ,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1 ,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1 ,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 // 계산된 값 사용
};

const uint32_t r[] = { // 한 라운드당 시프트 양
	7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
	5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
	4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
	6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

void int32_to_bytes(uint32_t value, uint8_t* bytes) // uint32_t 에서 byte 형태로 변환
{
	for (size_t i = 0; i < 4; i++)
		bytes[i] = (uint8_t)(value >> (i * 8));
}

uint32_t bytes_to_int32(uint8_t* bytes) // byte 에서 uint32_t  형태로 변환
{
	return (uint32_t)bytes[0]
		| ((uint32_t)bytes[1] << 8)
		| ((uint32_t)bytes[2] << 16)
		| ((uint32_t)bytes[3] << 24);
}


CString MD5(const CString& pt)
{
	// CString 에서 char*로 변환
	int ptlen = pt.GetLength();
	char* ptch = new char[ptlen + 1];
	WideCharToMultiByte(CP_ACP, 0, pt, -1, ptch, ptlen + 1, NULL, NULL); // 유니코드로 변환

	//MD5 알고리즘
	uint32_t h0 = 0x67452301, h1 = 0xefcdab89, h2 = 0x98badcfe, h3 = 0x10325476;
	uint32_t a = h0, b = h1, c = h2, d = h3;
	uint32_t i, f, g, temp, ptnewlen, offsetbit;;
	uint32_t w[16];

	for (ptnewlen = ptlen + 1; ptnewlen % 64 != 56; ptnewlen++); // 512 bit 에서 64 비트를 빼고 나머지 연산하였을때 0인 길이로 만듬

	uint8_t* msg = new uint8_t[ptnewlen + 8];
	memcpy(msg, ptch, ptlen); // 메모리 복사
	msg[ptlen] = 0x80;// 문자열\0 -> 문자열1으로 변환 (시작 시그니처 비트 1)

	for (offsetbit = ptlen + 1; offsetbit < ptnewlen; offsetbit++)
		msg[offsetbit] = 0; // 0으로 64bit 남기고 전부 패딩
	// 64 비트 리틀 엔디안 정수로 패딩되지 않은 메시지의 비트(바이트가 아닌 비트) 길이를 메시지에 추가
	int32_to_bytes(ptlen * 8, msg + ptnewlen); // ptlen의 8배 길이 추가
	int32_to_bytes(ptlen >> 29, msg + ptnewlen + 4); // 오버 플로우 방지

	for (offsetbit = 0; offsetbit < ptnewlen; offsetbit += 64)
	{

		// 연속적인 512 비트 청크로 메시지를 처리합니다.
		for (i = 0; i < 16; i++)
			w[i] = bytes_to_int32(msg + offsetbit + i * 4);

		// 메인 루프
		for (i = 0; i < 64; i++)
		{
			if (i < 16) {
				f = (b & c) | ((~b) & d);
				g = i;
			}
			else if (i < 32) {
				f = (d & b) | ((~d) & c);
				g = (5 * i + 1) % 16;
			}
			else if (i < 48) {
				f = b ^ c ^ d;
				g = (3 * i + 5) % 16;
			}
			else {
				f = c ^ (b | (~d));
				g = (7 * i) % 16;
			}

			temp = d;
			d = c;
			c = b;
			b = b + LEFTROTATE((a + f + k[i] + w[g]), r[i]);
			a = temp;
		}
		h0 += a;
		h1 += b;
		h2 += c;
		h3 += d;
	}

	delete[] msg;
	uint8_t result[16];
	int32_to_bytes(h0, result);
	int32_to_bytes(h1, result + 4);
	int32_to_bytes(h2, result + 8);
	int32_to_bytes(h3, result + 12);

	// uint8_t (unsigned char) -> CString 변환
	CString MD5enarr;
	MD5enarr.Format(_T("%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x%2.2x"), result[0], result[1], result[2], result[3], result[4], result[5], result[6], result[7], result[8], result[9], result[10], result[11], result[12], result[13], result[14], result[15]);
	return MD5enarr;
}

/*for (int i = 0; i < 16; i++) // debug
		printf("%2.2x", result[i]);*/