#include "../Include/ExchangeAL/StorageExchange.h"
#include "../Include/ExchangeAL/Exchange.h"

template<> void exchangeTable<RecordStorageType>(CConfigTable &table, RecordStorageType &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "SATA", config.SATA_as);
	exchanger.exchange(table, "USB", config.USB_as);
	exchanger.exchange(table, "SD", config.SD_as);
	exchanger.exchange(table, "DVD", config.DVD_as);
}
