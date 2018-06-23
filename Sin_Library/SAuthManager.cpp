#include "stdafx.h"
#include "SAuthManager.h"


// xtime is a macro that finds the product of {02} and the argument to xtime modulo {1b}
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))

// Multiplty is a macro used to multiply numbers in the field GF(2^8)
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))


AESAuth_128::AESAuth_128()
{
}

AESAuth_128::~AESAuth_128()
{

}

void AESAuth_128::Cipher(OUT unsigned char plan_text[4][4], IN unsigned char cipher_key[4][4])
{
	int i = 0;
	AddRoundKey(plan_text, cipher_key);

	for (i = 0; i<9; i++) {
		SubBytes(plan_text);
		ShiftRows(plan_text);
		MixColumns(plan_text);
		KeySchedule(cipher_key, S_BOX, i);
		AddRoundKey(plan_text, cipher_key);
	}

	SubBytes(plan_text);
	ShiftRows(plan_text);
	KeySchedule(cipher_key, S_BOX, i);
	AddRoundKey(plan_text, cipher_key);
}

void AESAuth_128::InvCipher(OUT unsigned char cipher_text[4][4], IN unsigned char cipher_key[4][4])
{
	memcpy(original_cipher_key, cipher_key, sizeof(original_cipher_key));

	InvKeySchedule(cipher_key, 9);
	AddRoundKey(cipher_text, cipher_key);

	for (int i = 8; i > -1; i--) {
		InvShiftRows(cipher_text);
		InvSubBytes(cipher_text); // S_BOX를 Inv_S_BOX로 교체. 상수값만 다르고 계산방식은 같다


		InvKeySchedule(cipher_key, i);
		AddRoundKey(cipher_text, cipher_key);

		InvMixColumns(cipher_text);
	}

	InvShiftRows(cipher_text);
	InvSubBytes(cipher_text);

	AddRoundKey(cipher_text, original_cipher_key);
}

void AESAuth_128::SubBytes(OUT unsigned char plan_text[4][4])
{
	int x, y;

	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			x = plan_text[i][j] >> 4;
			y = plan_text[i][j] - (x << 4);
			plan_text[i][j] = S_BOX[x][y];
		}
	}
}

void AESAuth_128::ShiftRows(OUT unsigned char plan_text[4][4])
{
	int tmp;

	for (int i = 0; i<4; i++) {
		for (int j = 4 - i; j < 4; j++) {
			tmp = plan_text[i][0];
			for (int k = 0; k < 4 - 1; k++) {
				plan_text[i][k] = plan_text[i][k + 1];
			}
			plan_text[i][3] = tmp;
		}
	}
}

void AESAuth_128::MixColumns(OUT unsigned char plan_text[4][4])
{
	unsigned char a[0x04], b[0x04], h;

	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			a[j] = plan_text[j][i];
			h = (unsigned char)((signed char)plan_text[j][i] >> 7);
			b[j] = plan_text[j][i] << 1;
			b[j] ^= 0x1b & h;
		}
		plan_text[0][i] = b[0] ^ a[3] ^ a[2] ^ b[1] ^ a[1];
		plan_text[1][i] = b[1] ^ a[0] ^ a[3] ^ b[2] ^ a[2];
		plan_text[2][i] = b[2] ^ a[1] ^ a[0] ^ b[3] ^ a[3];
		plan_text[3][i] = b[3] ^ a[2] ^ a[1] ^ b[0] ^ a[0];
	}
}


void AESAuth_128::InvSubBytes(OUT unsigned char cipher_text[4][4])
{
	int x, y;

	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			x = cipher_text[i][j] >> 4;
			y = cipher_text[i][j] - (x << 4);
			cipher_text[i][j] = INV_S_BOX[x][y];
		}
	}
}

void AESAuth_128::InvShiftRows(OUT unsigned char cipher_text[0x04][0x04])
{
	int k, tmp;

	for (int i = 3; i > -1; i--) {
		for (int j = i - 4; j<4; j++) {
			tmp = cipher_text[i][0];
			for (k = 0; k < 4 - 1; k++) {
				cipher_text[i][k] = cipher_text[i][k + 1];
			}
			cipher_text[i][3] = tmp;
		}
	}
}

void AESAuth_128::InvMixColumns(OUT unsigned char cipher_text[0x04][0x04])
{

	int i;
	unsigned char a, b, c, d;

	for (i = 0; i<4; i++)
	{
		a = cipher_text[0][i];
		b = cipher_text[1][i];
		c = cipher_text[2][i];
		d = cipher_text[3][i];


		cipher_text[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
		cipher_text[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
		cipher_text[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
		cipher_text[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
	}
}

void AESAuth_128::InvKeySchedule(OUT unsigned char CipherKey[4][4], IN int num)
{
	unsigned char cipherkey[0x04][0x04];

	memcpy(cipherkey, original_cipher_key, sizeof(original_cipher_key));

	for (int i = 0; i != num + 1; i++)
		KeySchedule(cipherkey, S_BOX, i);

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			CipherKey[i][j] = cipherkey[i][j];
	}
}


void AESAuth_128::KeySchedule(OUT unsigned char CipherKey[0x04][0x04], IN const unsigned char BOX[16][16], int i) {

	unsigned char tmp[0x04][0x04];
	unsigned char tmp2[0x04];
	int j, k, x, y;

	for (j = 0; j<3; j++) {// RotWord
		tmp2[j] = CipherKey[j + 1][3];
	}

	tmp2[3] = CipherKey[0][3];

	for (j = 0; j<4; j++) {// SubBytes
		x = tmp2[j] >> 4;
		y = tmp2[j] - (x << 4);
		tmp2[j] = BOX[x][y];
	}

	for (j = 0; j<4; j++) {
		tmp[j][0] = CipherKey[j][0] ^ tmp2[j] ^ R_Con[j][i];
	}
	for (j = 0; j<4; j++) {
		for (k = 1; k<4; k++) {
			tmp[j][k] = CipherKey[j][k] ^ tmp[j][k - 1];
		}
	}
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			CipherKey[i][j] = tmp[i][j];
		}
	}
}

void AESAuth_128::AddRoundKey(OUT unsigned char plan_text[4][4], unsigned char RoundKey[0x04][0x04])
{
	for (int i = 0; i<4; i++) {
		for (int j = 0; j<4; j++) {
			plan_text[i][j] ^= RoundKey[i][j];
		}
	}
}



//////////////////////////////////////

SAuth::SAuth()
{
}


SAuth::~SAuth()
{
}

void SAuth::Set_AES128_CipherKey(std::string filename)
{
	bool result = m_file(filename);

	if (result == false) return;

	m_file >> cipher_key;
}

void SAuth::AES128_Cipher(OUT unsigned char text[4][4])
{
	memcpy(this->copy_cipher_key, cipher_key, sizeof(cipher_key));

	m_aes128.Cipher(text, copy_cipher_key);
}

void SAuth::AES128_InvCipher(OUT unsigned char text[4][4])
{
	memcpy(this->copy_cipher_key, cipher_key, sizeof(copy_cipher_key));

	m_aes128.InvCipher(text, copy_cipher_key);
}
