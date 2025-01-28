// ****************************************
// *** �`�c�|�[�g�̓���m�F             ***
// ***    produced by Y. Mori           ***
// ***       special thanks to A. Ruike ***
// ****************************************
// �� H8�{�[�h�� JP2, JP3��z�����邱��

#include "r3069.h"
#include <mes2.h>

/* ************ */
/* *** main *** */
/* ************ */
int 
main(void)
{
	int  ad_data, ad_volt;

	// AD�|�[�g�̏�����
	AD.ADCSR.BYTE = 0x08;  
	// 00001000
	// ||||||||_AN0
	// |||||_	�ϊ����� �������[�h
	// ||||_	�P�ꃂ�[�h
	// |||_		A/D�ϊ���~
	// ||_		A/D�ϊ����荞�݋֎~
	// |_		A/D�ϊ��I���t���O
	
	while (1){
		AD.ADCSR.BIT.ADST = 1;			// AD�ϊ��X�^�[�g
		while (AD.ADCSR.BIT.ADF == 0);	// �ϊ��I���܂ő҂�
		ad_data = AD.ADDRA >> 6;		// �r�b�g�V�t�g
		AD.ADCSR.BIT.ADF = 0;

		ad_volt = (int)((5.13f/1024.0f)*1000.0f*ad_data + 0.5f);  //5.13[V]�͓��͓d���i����)

		printf(" %4d   %4d [mV]\r", ad_data, ad_volt);
	}
}
