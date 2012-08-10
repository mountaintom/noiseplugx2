#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <Windows.h>
#include <MMSystem.h>
#include "binary.h"

static inline uint8_t THREEQUARTERS(uint8_t x)
{
	return (x >> 2) + (x >> 1);
}

const int notes[37] = { 134, 142, 150, 159, 169, 179, 189, 201, 213, 225, 239, 253, 268, 284, 301, 319, 338, 358, 379, 401, 425, 451, 477, 506, 536, 568, 601, 637, 675, 715, 758, 803, 851, 901, 955, 1011, 1072 };

const uint8_t arpnotes[] = {
	8, 10, 12, 14, 15, 17, 19, 20, 21, 22, 24, 26, 27, 29, 
};

const uint16_t arpeggio[][2] = {
	{ 0x24, 0x6A },
	{ 0x46, 0x9C },
	{ 0x13, 0x59 },
	{ 0x02, 0x47 },
	{ 0x24, 0x59 },
	{ 0x24, 0x58 },
	{ 0x57, 0xAD },
	{ 0x35, 0x9B }
};

#define ARPSIZE 76

#if 0
uint8_t arpseq1[4][8] = {
	{ 0, 0, 1, 2, 0, 0, 6, 2, },
	{ 0, 0, 1, 2, 0, 0, 1, 7, },
	{ 0, 0, 1, 2, 0, 0, 1, 2, },
	{ 3, 3, 2, 2, 0, 0, 4, 5, },
};
#else
const uint8_t arpseq1[4][4] = {
	{ 0x00, 0x12, 0x00, 0x62 },
	{ 0x00, 0x12, 0x00, 0x17 },
	{ 0x00, 0x12, 0x00, 0x12 },
	{ 0x33, 0x22, 0x00, 0x45 },
};
#endif
const int arpseq2[] = { 0, 1, 0, 1, 0, 1, 0, 2, 3, 3 };
//int arptiming[32] = { 4, 2, 4, 2, 4, 2, 4, 5, 1, 2, 2 }
const uint32_t arptiming = B32(00001100,00110000,11111011,00001100);

#define BASSSIZE ARPSIZE

const int bassbeat[8] = { 0, 0, 1, 0, 0, 1, 0, 1 };
const int bassline[BASSSIZE] = {
	12, 12, 15, 10, 12, 12, 17, 10, 12, 12, 15, 7, 8, 8, 3, 7,
	8, 8, 10, 10, 12, 12, 5, 5,	8, 8, 10, 10,
};

const uint8_t leadnotes[] = {
	0xFF, 0, 3, 5, 7, 8, 10, 12, 14, 15, 17, 19, 20, 22, 24, 26, 27
};

const uint8_t leadtimes[] = {
	1, 2, 3, 4, 5, 6, 28, 56
};

#define LEADSIZE 180
const int leadmelody[LEADSIZE] = {
	12, 7, 0, 12, 0, 14, 15, 0, 14, 0, 12, 0, 14, 15, 0, 14, 0, 12, 0, 14, 10, 0, 7, 0, 5, 7, 3, 1, 0, 
	12, 7, 0, 12, 0, 14, 15, 0, 14, 0, 12, 0, 14, 15, 0, 14, 0, 15, 0, 17, 19, 0, 22, 0, 24, 26, 27, 24, 0,
	12, 7, 0, 12, 0, 14, 15, 0, 14, 0, 12, 0, 14, 15, 0, 14, 0, 12, 0, 14, 10, 0, 7, 0, 5, 7, 3, 1, 0, 
	12, 7, 0, 12, 0, 14, 15, 0, 14, 0, 12, 0, 14, 15, 0, 14, 0, 15, 0, 17, 19, 0, 22, 0, 24, 26, 24, 20, 0,
	8, 3, 0, 8, 10, 12, 0, 14, 15, 19, 17, 0, 12, 7, 0, 12, 14, 15, 0, 14, 15, 19, 17, 0, 
	8, 3, 0, 8, 10, 12, 14, 0, 15, 19, 17, 15, 0, 14, 15, 17, 19, 0, 15, 14, 15, 12,
};
const int leadtiming[LEADSIZE] = {
	4, 2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1, 3, 1, 4, 2, 4, 6, 56,
	4, 2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1, 3, 1, 4, 2, 4, 6, 56,
	4, 2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1, 3, 1, 4, 2, 4, 6, 56,
	4, 2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1,  2, 2,  2, 2,  2,  5, 1, 3, 1, 4, 2, 4, 6, 56,
	4, 2, 2, 4, 2, 5, 1, 4, 4, 2, 6, 28, 4, 2, 2, 4, 2, 5, 1, 4, 4, 2, 6, 28, 
	4, 2, 2, 4, 2,  6,  2,  2, 4,  2,  6,  2,  2,  4,  2,  6,  2, 2,  4,  2,  4,  4,
};

struct leadvoice_s {
	uint8_t ptr, timer;
	uint16_t osc;
} leads[3] = {
	{ LEADSIZE, 1, 0 },
	{ LEADSIZE, 3, 1601 },
	{ LEADSIZE, 5, 3571 },
};

uint8_t boosts;

static unsigned char voice_lead(unsigned long i, int voice_nr)
{
#define leadptr leads[voice_nr].ptr
#define lead_osc leads[voice_nr].osc
#define leadtimer leads[voice_nr].timer

	if (leadptr == LEADSIZE)
	{
		if (i == (0x40000 + 0x400 * voice_nr))
		{
			leadptr = -1;
			leadtimer = 1;
		}
		else
			return 0;
	}

	if (0 == (i & 0x0FF))
		boosts &= ~(1 << voice_nr);
	if (0 == (i & 0x1FF))
		leadtimer--;
	if (0 == leadtimer)
	{
		leadptr++;
		leadtimer = leadtiming[leadptr];
		boosts |= 1 << voice_nr;
	}


	uint8_t melody = leadmelody[leadptr];
	int note = notes[melody == 1 ? 0 : melody]; // TODO remove this hack by using note table
	lead_osc += note;
//	lead_flange += note + (i & 1);
	uint8_t sample = ((lead_osc >> 6) & 0x7F) + ((lead_osc >> 6) & 0x3F);// + ((lead_flange >> 6) & 0x3F);  // xor also sounds cool
	//return (!melody) ? 0 : ((lead_osc >> 5) & 0x80) + ((lead_flange >> 6) & 0x3F);
	//return (!melody) ? 0 : ((lead_osc & 0x1000) ? ((lead_osc >> 6) & 0x3F) | 0xC0 : 0x40 - ((lead_osc >> 6) & 0x3F));
	return (!melody) ? 0 : ((boosts & (1 << voice_nr)) ? sample : THREEQUARTERS(sample));
}
                     
static inline unsigned char voice_arp(unsigned long i)
{
	static uint16_t arp_osc = 0;
	uint8_t arpptr = i >> 13;
	uint8_t arpptr2 = arpseq1[arpseq2[arpptr >> 3]][(arpptr >> 1) & 3];
	if (!(arpptr & 1))
		arpptr2 >>= 4;
	arpptr = arpeggio[arpptr2 & 0xF][(i >> 8) & 1];
	if (!(i & 0x80))
		arpptr >>= 4;

	int note = notes[arpnotes[arpptr & 0xF]];
	arp_osc += note;
	return ((arptiming & (1 << (31 - (i >> 9)))) && (arp_osc & (1 << 12)) && ((i >> 13) > 15)) ? 0 : 140;
	//return ((arp_osc >> 5) & 128) - 1;
}

static inline unsigned char voice_bass(unsigned long i)
{
	static uint16_t bassosc = 0, flangeosc = 0;
	uint8_t bassptr = (i >> 13) & 0xF;
	if (i >> 19)
		bassptr |= 0x10;
	int note = notes[bassline[bassptr]];
	if (bassbeat[(i >> 10) & 7])
		note <<= 1;
	bassosc += note;
	flangeosc += note + 1;
	unsigned char ret = ((bassosc >> 8) & 0x7F) + ((flangeosc >> 8) & 0x7F);
	return ((i >> 6) & 0xF) == 0xF ? 0 : ret;
}

static inline uint8_t next_sample()
{
	static unsigned long i = 0;//x40000;
	uint8_t ret = (voice_lead(i, 0) >> 1) + THREEQUARTERS(voice_lead(i, 1) >> 2) + (voice_lead(i, 2) >> 3) + (voice_bass(i) >> 2) + (voice_arp(i) >> 2);
	i++;
	if ((i >> 13) == ARPSIZE)
		i = 16 << 13;
	return ret;
}

void fill(uint8_t *data)
{
	static uint8_t max = 0;

	for (int j = 0; j < 4096; j++)
	{
		data[j] = next_sample();
		if (data[j] > max)
		{
			max = data[j];
			printf("%x\n", max);
		}
	}
}

static const WAVEFORMATEX fmt = {
	WAVE_FORMAT_PCM, 1, 8000, 8000, 1, 8, 0
};

int main(int argc, char *argv[])
{
	HWAVEOUT out;
	HRESULT rc = waveOutOpen(&out, WAVE_MAPPER, &fmt, NULL, NULL, CALLBACK_NULL);
	if (rc != MMSYSERR_NOERROR)
	{
		printf("error %d on open\n");
		exit(1);
	}

	WAVEHDR bufs[2] = {
		{ (char *)malloc(4096), 4096 },
		{ (char *)malloc(4096), 4096 },
	};
	int i = 0;

	fill((uint8_t *)bufs[i].lpData);
	bufs[i].dwFlags = WHDR_PREPARED;
	waveOutPrepareHeader(out, bufs + i, sizeof(WAVEHDR));
	waveOutWrite(out, bufs + i, sizeof(WAVEHDR));
	i ^= 1;

	while (!(GetAsyncKeyState(VK_ESCAPE) & 1))
	{
		fill((uint8_t *)bufs[i].lpData);
		bufs[i].dwFlags = WHDR_PREPARED;
		waveOutPrepareHeader(out, bufs + i, sizeof(WAVEHDR));
		waveOutWrite(out, bufs + i, sizeof(WAVEHDR));
		i ^= 1;
		while (waveOutUnprepareHeader(out, bufs + i, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);
	}
}