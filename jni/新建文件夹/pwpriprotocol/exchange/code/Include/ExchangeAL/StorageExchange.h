#ifndef __EXCHANGEAL_STORAGEEXCHANGE_H__
#define __EXCHANGEAL_STORAGEEXCHANGE_H__

/// 录像存储设备类型，以下类型的一种或者多种
struct RecordStorageType
{
	bool SATA_as;
	bool USB_as;
	bool SD_as;
	bool DVD_as;
};

#endif
