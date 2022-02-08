/**
 * Copyright 2018 Raul Salinas-Monteagudo
 */

#include "ze08_ch2o.h"

Ze08CH2O::Ze08CH2O(Stream * stream, std::function<void(void)> onBadChecksum) : stream_(stream), onBadChecksum_(onBadChecksum) {
}

Ze08CH2O::concentration_t Ze08CH2O::getFullRange() const {
	return fullrange;
}

void Ze08CH2O::switchToQAMode()
{
	uint8_t command[] = { 0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46 };
	stream_->write(command, sizeof(command));
	delay(100);
	_mode = MODE_PASSIVE;
}

void Ze08CH2O::switchToActiveUploadMode()
{
	uint8_t command[] = { 0xFF, 0x01, 0x78, 0x40, 0x00, 0x00, 0x00, 0x00, 0x47 };
	stream_->write(command, sizeof(command));
	delay(100);
	_mode = MODE_ACTIVE;
}

bool Ze08CH2O::getGasConcentration(concentration_t& value)
{
	// Clear buffer (removes potentially old data) before read. Some data could have been also sent before switching to passive mode.
    while (stream_->available()) { stream_->read(); }
	available_ = false;
	_concentration = _ppb = 0;

	if(_mode == MODE_PASSIVE){		
		uint8_t command[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};
		for(uint8_t i = 0; i < (uint8_t)sizeof(command); i++)
		{
			stream_->write(command[i]);
			delay(50);
		}
	}

	return readUntil(value, 1500);
}

// Blocking function for parse response. Default timeout is 1s.
bool Ze08CH2O::readUntil(concentration_t& concentration, uint16_t timeout)
{
	uint32_t start = millis();
	resetState();
	do
	{
		if(_mode == MODE_PASSIVE) passiveReadLoop();
		else activeReadLoop();

		if (state == STATE_DONE) break;
	} while (millis() - start < timeout);

	concentration = _concentration;
	return state == STATE_DONE;
}

void Ze08CH2O::passiveReadLoop()
{
  if (stream_->available() > 0)
  {
    uint8_t ch = stream_->read();

	switch (state) {
	case STATE_START:
		if (ch == 0xFFu) {
			dataSum = 0;
			state = STATE_PASSIVE_COMMAND;
			return;
		}
		break;
	case STATE_PASSIVE_COMMAND:
		if (ch != 0x86) {
			resetState();
			return;
		}
		break;
	case STATE_PASSIVE_CONCENTRATION_HI:
		_concentration = ch << 8;
		break;
	case STATE_PASSIVE_CONCENTRATION_LO:
		_concentration |= ch;
		break;
	case STATE_PASSIVE_PPB_HI:
		_ppb = ch << 8;
		break;
	case STATE_PASSIVE_PPB_LO:
		_ppb |= ch;
		break;
	case STATE_CHECKSUM:
		unsigned char finalEcksum  = (~dataSum)+1 ;
		if (finalEcksum   == ch ) {
				available_ = true;
		} else {
			onBadChecksum_();
		}
		state = STATE_DONE;
		return;

	}
	dataSum += ch;
	state = (State) (state + 1);

  }
}

void Ze08CH2O::activeReadLoop()
{
  if (stream_->available())
  {
    uint8_t ch = stream_->read();

	switch (state) {
	case STATE_START:
		if (ch == 0xFFu) {
			dataSum = 0;
			state = STATE_GASNAME;
			return;
		}
		break;
	case STATE_GASNAME:
		if (ch != 0x17) {
			resetState();
			return;
		}
		break;
	case STATE_UNIT:
		if (ch != 0x04) {
			resetState();
			return;
		}
		break;
	case STATE_NODECIMAL:
		if (ch != 0) {
			resetState();
			return;
		}
		break;
	case STATE_CONCENTRATION_HI:
		_concentration = ch << 8;
		break;
	case STATE_CONCENTRATION_LO:
		_concentration |= ch;
		break;
	case STATE_FULLRANGE_HI:
		fullrange = ch << 8;
		break;
	case STATE_FULLRANGE_LO:
		fullrange |= ch;
		break;
	case STATE_CHECKSUM:
		unsigned char finalEcksum  = (~dataSum)+1 ;
		if (finalEcksum   == ch ) {
				available_ = true;
		} else {
			onBadChecksum_();
		}
		state = STATE_DONE;
		return;

	}
	dataSum += ch;
	state = (State) (state + 1);
  }
}
