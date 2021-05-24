/*
 * MPSTSerial.h
 *
 *  Created on: May 24, 2021
 *      Author: alfy
 */

#ifndef MPSTSERIAL_H_
#define MPSTSERIAL_H_
#include <MPcore/MP.h>
#include <stm32f4xx_hal.h>

typedef uint8_t (*HAL_send)(uint8_t *buf, uint16_t len); // USB and UART return 0 on complete transmission, <0 other (no need more info)

namespace EMP {

templatePar()
class MP_ST_Serial: public MP<templateParCall()> {
	HAL_send send;

	CircularBuffer<uint8_t, cbBinSize> byteSend;
	unsigned long lastDecodeTime;
public:
	MP_ST_Serial(HAL_send send_);
	~MP_ST_Serial();
	int16_t getData_try(pIn *pack) override; // return the residual pack available after the remove
	int16_t getData_wait(pIn *pack) override; // return the residual pack available after the remove

	void updateState();

private:
	void trySend();
	void poolRead();

protected:
	int packSend_Concrete(uint8_t *stream, uint16_t len) override; // 0 on success, other wise, error code (!=-1)

	void packTimeRefresh() override; //Call by byteParsing when new pack are recived, used to measure the time
public:
	unsigned long lastPackElapsed() override; //retur micro second (10^-6 sec) elapsed since last pack recived

};

templatePar()
MP_ST_Serial<templateParCall()>::MP_ST_Serial(HAL_send send_) :
		MP<templateParCall()>() {
	send = send_;
	byteSend.memClean();
}

templatePar() MP_ST_Serial<templateParCall()>::~MP_ST_Serial() {
}

templatePar()int16_t MP_ST_Serial<templateParCall()>::getData_try(pIn *pack) {
	updateState();
	return MP<templateParCall()>::getData_try(pack);
}

templatePar()int16_t MP_ST_Serial<templateParCall()>::getData_wait(pIn *pack) {
	while (this->dataAvailable() < 1) {
		updateState();
	}
	return getData_try(pack);
}

templatePar()void MP_ST_Serial<templateParCall()>::updateState() {
	trySend();
	poolRead();
}

templatePar()void MP_ST_Serial<templateParCall()>::trySend() {
	uint16_t len;
	len = byteSend.remaningSpaceLinear();
	if (send(byteSend.getTailPtr(), len) == 0) {
		byteSend.tailAdd(len);
	}

}

templatePar()void MP_ST_Serial<templateParCall()>::poolRead() {
	//todo: rubare alla USB la call back
	//reload the byteRecive buffer form the Serial Subsystem
	/*while (com.available())
		this->byteRecive.put((uint8_t) com.read());
	//analizzo i dati
	this->byteParsing();
	*/
}

templatePar()int MP_ST_Serial<templateParCall()>::packSend_Concrete(
		uint8_t *stream, uint16_t len) {

	if (byteSend.availableSpace() < len)
		return -2;
	trySend();  // Push out the buffered byte
	uint16_t i = 0;

	if (byteSend.isEmpty()) {
		int ret = send(stream, len);
		if (ret < 0) {
			byteSend.putArray(stream, len);
		}
		return 0;
	} else {
		byteSend.putArray(stream, len);
	}

	return -2;
}

templatePar()void MP_ST_Serial<templateParCall()>::packTimeRefresh() {
	lastDecodeTime = HAL_GetTick();
}

templatePar()unsigned long MP_ST_Serial<templateParCall()>::lastPackElapsed() {
	return HAL_GetTick() - lastDecodeTime;
}
} /* namespace EMP */

#endif /* MPSTSERIAL_H_ */
