/*
 *
 *	Файл: 		hal_rtc.h
 *	Описание: 	HAL для модуля часов реального времени
 *
 *	История:
 *				13-Dec-2016	Zatonsky Pavel 		- 	создан
 *
 */
#ifndef __HAL_RTC_H__
#define __HAL_RTC_H__

#ifndef __cplusplus
#define inline __inline
#endif /* __cplusplus */


/* ------------------------------------ Структура периферии ------------------------------------- */
typedef volatile struct
{
	uint32_t RTC_CNT;																				// счетчик секунд
	uint32_t RTC_MR; 																				// регистр сравнения
	uint32_t WDT_CNT; 																				// текущее значение сторожевого счетчика
	uint32_t RTC_TDIV; 																				// таймера Текущее значение делителя тик-импульса
	uint32_t RTC_CR; 																				// Регистр управления
	uint32_t RTC_SDIV; 																				// Текущее значение делителя секунды
	uint32_t TIC_VAL; 																				// Период тик-импульса минус 1, max 20 bit (1048575)
	uint32_t SEC_VAL; 																				// Количество тик-импульсов в секунде
	uint32_t RTC_BUSY; 																				// Флаг занятости интерфейса
} RTC_type;

#define LX_RTC                 ( ( volatile RTC_type * ) base_RTC )

/* -------------------------------------- Специфические типы ------------------------------------ */
#define __WDT_REINIT	0x04072013

typedef enum
{
	rtcCNT, rtcMR, rtcWDT, rtcTDIV, rtcCR, rtcSDIV, rtcTICVAL, rtcSECVAL
} RTC_Reg_type;

#ifdef __cplusplus
extern "C"
{
#endif
	void 		HAL_RTC_Busy				( void );
	uint32_t 	HAL_RTC_RegRead				( RTC_Reg_type RTCReg );

	void		HAL_RTC_AlarmIntEn			( uint32_t MR_sec );
	void 		HAL_RTC_AlarmIntEnExactTime	( uint32_t MRExact_sec );
	void 		HAL_RTC_WDogIntEn			( uint32_t WDT_tic );
	void 		HAL_RTC_WDogHWResetEn		( uint32_t WDT_tic );
	void 		HAL_RTC_TicIntEn			( void );

	void 		HAL_RTC_AlarmIntDis			( void );
	void 		HAL_RTC_WDogResetDis		( void );
	void 		HAL_RTC_WDogIntDis			( void );
	void 		HAL_RTC_TicIntDis			( void );

	void 		HAL_RTC_InitTicPeriod		( uint32_t NumOfXTI );
	uint32_t 	HAL_RTC_InitTicPeriodUS		( uint32_t XTI_khz, uint32_t Period_us );
	void 		HAL_RTC_InitSecPeriod		( uint32_t NumOfTic );

	void 		HAL_RTC_WDogLock			( void );
	void 		HAL_RTC_WDogUnLock			( void );
	void 		HAL_RTC_Lock				( void );
	void 		HAL_RTC_UnLock				( void );
#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __HAL_RTC_H__ */
