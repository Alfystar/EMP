/*
 * MPSTSerial.h
 *	Must be call only once time!!!!
 *
 *  Created on: May 24, 2021
 *      Author: alfy
 */
//todo: change to singleton logic
#ifndef MP_ST_USB_H_
#define MP_ST_USB_H_
#ifdef STM32F407xx		//todo: Found more general define for STM32 family
#include <MPcore/MP.h>
#include <stm32f4xx_hal.h>

// STM32 USB include
#include <usbd_cdc_if.h>
extern USBD_HandleTypeDef hUsbDeviceFS;

//extern USBD_CDC_ItfTypeDef USBD_Interface_fops_FS; // Included from: #include <usbd_cdc_if.h>

//typedef uint8_t (*HAL_send)(uint8_t *buf, uint16_t len); // USB and UART return 0 on complete transmission, <0 other (no need more info)

namespace EMP {


templatePar()
class MP_ST_usb: public MP<templateParCall()> {
	//HAL_send send;
	static MP_ST_usb<templateParCall()> *instance;
	CircularBuffer<uint8_t, cbBinSize> byteSend;
	unsigned long lastDecodeTime;
public:
	MP_ST_usb();
	~MP_ST_usb();

	int16_t getData_wait(pIn *pack) override; // return the residual pack available after the remove

private:
	static int8_t isrSendNotify(uint8_t *Buf, uint32_t *Len, uint8_t epnum);
	static int8_t isrRead(uint8_t *Buf, uint32_t *Len);

protected:
	int packSend_Concrete(uint8_t *stream, uint16_t len) override; // 0 on success, other wise, error code (!=-1)

	void packTimeRefresh() override; //Call by byteParsing when new pack are recived, used to measure the time
public:
	unsigned long lastPackElapsed() override; //retur micro second (10^-6 sec) elapsed since last pack recived

};

templatePar() MP_ST_usb<templateParCall()> * MP_ST_usb<templateParCall()>::instance = nullptr;	//Istanza statica dinamica in base alla classe compilata


templatePar()
MP_ST_usb<templateParCall()>::MP_ST_usb() :
		MP<templateParCall()>() {
	byteSend.memClean();
	instance = this;
	USBD_Interface_fops_FS.Receive = MP_ST_usb<templateParCall()>::isrRead;
	USBD_Interface_fops_FS.TransmitCplt =
			MP_ST_usb<templateParCall()>::isrSendNotify;
}

templatePar() MP_ST_usb<templateParCall()>::~MP_ST_usb() {
}

templatePar()int16_t MP_ST_usb<templateParCall()>::getData_wait(pIn *pack) {
	while (this->dataAvailable() < 1) {
	}
	return this->getData_try(pack);
}

/** #### FROM CDC_TransmitCplt_FS Mock Function ####
 * @brief  CDC_TransmitCplt_FS
 *         Data transmited callback
 *
 *         @note
 *         This function is IN transfer complete callback used to inform user that
 *         the submitted Data is successfully sent over USB.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
templatePar()int8_t MP_ST_usb<templateParCall()>::isrSendNotify(uint8_t *Buf,
		uint32_t *Len, uint8_t epnum) {
	MP_ST_usb<templateParCall()> *inst = MP_ST_usb<templateParCall()>::instance;

	if (inst->byteSend.usedSpace()) {
		uint16_t len;
		len = inst->byteSend.usedSpaceLinear();
		if (CDC_Transmit_FS(inst->byteSend.getTailPtr(), len) == 0) {
			inst->byteSend.tailAdd(len);
		}
	}
	return USBD_OK;
}

/** #### FROM CDC_Receive_FS Mock Function ####
 * @brief  Data received over USB OUT endpoint are sent over CDC interface
 *         through this function.
 *
 *         @note
 *         This function will issue a NAK packet on any OUT packet received on
 *         USB endpoint until exiting this function. If you exit this function
 *         before transfer is complete on CDC interface (ie. using DMA controller)
 *         it will result in receiving more data while previous ones are still
 *         not sent.
 *
 * @param  Buf: Buffer of data to be received
 * @param  Len: Number of data received (in bytes)
 * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
 */
templatePar()int8_t MP_ST_usb<templateParCall()>::isrRead(uint8_t *Buf,
		uint32_t *Len) {
	MP_ST_usb<templateParCall()> *inst = MP_ST_usb<templateParCall()>::instance;
	inst->byteRecive.putArray(Buf, *Len);
	USBD_CDC_ReceivePacket(&hUsbDeviceFS);
	inst->byteParsing();
	return USBD_OK;
}

templatePar()int MP_ST_usb<templateParCall()>::packSend_Concrete(
		uint8_t *stream, uint16_t len) {
	//return 0;
	if (byteSend.availableSpace() < len){
		return -2;
	}

	byteSend.putArray(stream, len);
	len = byteSend.usedSpaceLinear();
	if (CDC_Transmit_FS(byteSend.getTailPtr(), len) == 0) {
		byteSend.tailAdd(len);
		return 0;
	} else{
		return -3;
	}
}

templatePar()void MP_ST_usb<templateParCall()>::packTimeRefresh() {
	lastDecodeTime = HAL_GetTick();
}

templatePar()unsigned long MP_ST_usb<templateParCall()>::lastPackElapsed() {
	return HAL_GetTick() - lastDecodeTime;
}
} /* namespace EMP */

#endif /*#ifdef STM32F407xx */
#endif /* MP_ST_USB_H_ */
