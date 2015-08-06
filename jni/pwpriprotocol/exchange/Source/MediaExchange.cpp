//	Description:	
//	Revisions:		Year-Month-Day  SVN-Author  Modification
//

#include "../Include/ExchangeAL/MediaExchange.h"
#include "../Include/ExchangeAL/Exchange.h"

static ConfigPair s_videoCompressionMap[] = 
{
	{"MPEG4", CAPTURE_COMPRESS_DIVX_MPEG4},
	{"MPEG2", CAPTURE_COMPRESS_MPEG2},
	{"MPEG1", CAPTURE_COMPRESS_MPEG1},
	{"MJPG", CAPTURE_COMPRESS_MJPG},
	{"H.263", CAPTURE_COMPRESS_H263},
	{"H.264", CAPTURE_COMPRESS_H264},
	{NULL, }
};

/*static*/ ConfigPair s_videoResolutionMap[] = 
{
	{"D1", CAPTURE_IMAGE_SIZE_D1},
	{"HD1", CAPTURE_IMAGE_SIZE_HD1},
	{"BCIF", CAPTURE_IMAGE_SIZE_BCIF},
	{"CIF", CAPTURE_IMAGE_SIZE_CIF},
	{"QCIF", CAPTURE_IMAGE_SIZE_QCIF},
	{"VGA", CAPTURE_IMAGE_SIZE_VGA},
	{"QVGA", CAPTURE_IMAGE_SIZE_QVGA},
	{"SVCD", CAPTURE_IMAGE_SIZE_SVCD},
	{"QQVGA", CAPTURE_IMAGE_SIZE_QQVGA},
	{"ND1", CAPTURE_IMAGE_SIZE_ND1},
	{"650TVL", CAPTURE_IMAGE_SIZE_650TVL},
	{"720P", CAPTURE_IMAGE_SIZE_720P},
	{"1_3M", CAPTURE_IMAGE_SIZE_1_3M},
	{"UXGA", CAPTURE_IMAGE_SIZE_UXGA},
	{"1080P", CAPTURE_IMAGE_SIZE_1080P},
	{"WUXGA", CAPTURE_IMAGE_SIZE_WUXGA},
	{"2_5M", CAPTURE_IMAGE_SIZE_2_5M},
	{"3M", CAPTURE_IMAGE_SIZE_3M},
	{"5M", CAPTURE_IMAGE_SIZE_5M},
	{"5M_X", CAPTURE_IMAGE_SIZE_5M_X},
	{NULL, }
};

static ConfigPair s_videoBitrateControlMap[] = 
{
	{"CBR", CAPTURE_BITRATE_CBR},
	{"VBR",	CAPTURE_BITRATE_VBR},
	{"MBR",	CAPTURE_BITRATE_MBR},
	{NULL, }
};

void exchangeFormat(CConfigTable& table, MEDIA_FORMAT& format, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table["Video"], "Compression", format.vfFormat.iCompression, s_videoCompressionMap);
	exchanger.exchange(table["Video"], "Resolution", format.vfFormat.iResolution, s_videoResolutionMap);
	exchanger.exchange(table["Video"], "BitRateControl", format.vfFormat.iBitRateControl, s_videoBitrateControlMap);
	exchanger.exchange(table["Video"], "Quality", format.vfFormat.iQuality);
	exchanger.exchange(table["Video"], "FPS", format.vfFormat.nFPS);
	exchanger.exchange(table["Video"], "GOP", format.vfFormat.iGOP);

	exchanger.exchange(table["Video"], "BitRate", format.vfFormat.nBitRate);
	exchanger.exchange(table["Audio"], "BitRate", format.afFormat.nBitRate);
	exchanger.exchange(table["Audio"], "SampleRate", format.afFormat.nFrequency);
	exchanger.exchange(table["Audio"], "MaxVolume", format.afFormat.nMaxVolume);

	exchanger.exchange(table, "VideoEnable", format.bVideoEnable);
	exchanger.exchange(table, "AudioEnable", format.bAudioEnable);
}

template<> void exchangeTable<CONFIG_ENCODE>(CConfigTable &table, CONFIG_ENCODE &config, int state)
{
	for (int i = 0; i < ENCODE_TYPE_NUM; i++)
	{
		exchangeFormat(table["MainFormat"][i], config.dstMainFmt[i], state);
		exchangeFormat(table["SnapFormat"][i], config.dstSnapFmt[i], state);
	}
	for (int i = 0; i < MAX_EXTRA_STREAM_TYPE; i++)
	{
		exchangeFormat(table["ExtraFormat"][i], config.dstExtraFmt[i], state);
	}
}

static void exchangeRect(CConfigTable& table, Rect& rect, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, 0, rect.left);
	exchanger.exchange(table, 1, rect.top);
	exchanger.exchange(table, 2, rect.right);
	exchanger.exchange(table, 3, rect.bottom);
}

void exchangeCover(CConfigTable& table, VIDEO_WIDGET& vw, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "FrontColor", vw.rgbaFrontground);

	exchanger.exchange(table, "BackColor", vw.rgbaBackground);

	exchangeRect(table["RelativePos"], vw.rcRelativePos, state);
	exchanger.exchange(table, "PreviewBlend", vw.bShowInPreview);
	exchanger.exchange(table, "EncodeBlend", vw.bShowInEncode);
}

template<> void exchangeTable<CONFIG_VIDEOWIDGET>(CConfigTable& table, CONFIG_VIDEOWIDGET& config, int state)
{
	CKeyExchange exchanger;

	CConfigTable& tb = table["Covers"];

	for(int i = 0; i < MAX_COVER_COUNT; i++)
	{
		exchangeCover(tb[i], config.dstCovers[i], state);
	}
	exchanger.setState(state);
	exchangeCover(table["ChannelTitleAttribute"], config.ChannelTitle, state);
	exchangeCover(table["TimeTitleAttribute"], config.TimeTitle, state);
	exchanger.exchange(table, "CoversNum", config.iCoverNum);

	char serialNo[64];

	exchanger.exchange(table["ChannelTitle"],"Name", config.ChannelName.strName);
	sprintf(serialNo, "%llu", config.ChannelName.iSerialNo);
	exchanger.exchange(table["ChannelTitle"], "SerialNo", serialNo);
	sscanf(serialNo, "%llu", &config.ChannelName.iSerialNo);
}


template<> void exchangeTable<VideoWidgetConfigAll>(CConfigTable& table, VideoWidgetConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vVideoWidegetConfigAll[i], state);
	}
}

template<> void exchangeTableV2<VideoWidgetConfigAll>(CConfigTable& table, VideoWidgetConfigAll& config, int state, int nSize)
{

	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vVideoWidegetConfigAll[i], state);
	}
}

static void VideoColorExchange(CConfigTable& table, VIDEOCOLOR_PARAM& vcp, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Contrast", vcp.nContrast);
	exchanger.exchange(table, "Brightness", vcp.nBrightness);
	exchanger.exchange(table, "Saturation", vcp.nSaturation);
	exchanger.exchange(table, "Hue", vcp.nHue);
	exchanger.exchange(table, "Gain", vcp.mGain);
	exchanger.exchange(table, "Whitebalance", vcp.mWhitebalance);//默认白电平使能打开
	exchanger.exchange(table, "Acutance", vcp.nAcutance);
}

// in AlarmExchange.cpp
extern void exchangeTimeSection(CConfigTable& table, TimeSection& timesection, int state);

template<> void exchangeTable<CONFIG_VIDEOCOLOR>(CConfigTable& table, CONFIG_VIDEOCOLOR& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < N_COLOR_SECTION; i++)
	{
		CConfigTable& tb1 = table[i];		
		exchangeTimeSection(tb1["TimeSection"], config.dstVideoColor[i].tsTimeSection, state);
		VideoColorExchange(tb1["VideoColorParam"], config.dstVideoColor[i].dstColor, state);
		exchanger.exchange(tb1, "Enable", config.dstVideoColor[i].iEnable);
	}
}

template<> void exchangeTable<VideoColorConfigAll>(CConfigTable& table, VideoColorConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vVideoColorAll[i], state);
	}
}

template<> void exchangeTableV2<VideoColorConfigAll>(CConfigTable& table, VideoColorConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vVideoColorAll[i], state);
	}
}

template<> void exchangeTable<EncodeConfigAll>(CConfigTable& table, EncodeConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{		
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vEncodeConfigAll[i], state);
	}
}

template<> void exchangeTableV2<EncodeConfigAll>(CConfigTable& table, EncodeConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];
		
		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vEncodeConfigAll[i], state);
	}
}

void exchangeFormat(CConfigTable& table, MEDIA_FORMAT_SIMPLIIFY& format, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table["Video"], "Compression", format.vfFormat.iCompression, s_videoCompressionMap);
	exchanger.exchange(table["Video"], "Resolution", format.vfFormat.iResolution, s_videoResolutionMap);
	exchanger.exchange(table["Video"], "BitRateControl", format.vfFormat.iBitRateControl, s_videoBitrateControlMap);
	exchanger.exchange(table["Video"], "Quality", format.vfFormat.iQuality);
	exchanger.exchange(table["Video"], "FPS", format.vfFormat.nFPS);
	exchanger.exchange(table["Video"], "GOP", format.vfFormat.iGOP);

	exchanger.exchange(table["Video"], "BitRate", format.vfFormat.nBitRate);

	exchanger.exchange(table, "VideoEnable", format.bVideoEnable);
	exchanger.exchange(table, "AudioEnable", format.bAudioEnable);
}

template<> void exchangeTable<CONFIG_ENCODE_SIMPLIIFY>(CConfigTable &table, CONFIG_ENCODE_SIMPLIIFY &config, int state)
{
	exchangeFormat(table["MainFormat"], config.dstMainFmt, state);
	exchangeFormat(table["ExtraFormat"], config.dstExtraFmt, state);
}

template<> void exchangeTable<EncodeConfigAll_SIMPLIIFY>(CConfigTable& table, EncodeConfigAll_SIMPLIIFY& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vEncodeConfigAll[i], state);
	}
}

template<> void exchangeTableV2<EncodeConfigAll_SIMPLIIFY>(CConfigTable& table, EncodeConfigAll_SIMPLIIFY& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vEncodeConfigAll[i], state);
	}
}

template<> void exchangeTable<CombEncodeConfigAll>(CConfigTable& table, CombEncodeConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_COMB_CH; i++)
	{
		exchangeTable(table[i], config.vEncodeConfigAll[i], state);
	}
}

static ConfigPair s_combEncodeTransModeMap[] = 
{
	{"MultiReplay", COMBINE_ENCODE_MULTIREPLAY},
	{"NarrowBand",	COMBINE_ENCODE_NARROWBAND},
	{NULL, }
};

template<> void exchangeTable<CombEncodeModeAll>(CConfigTable& table, CombEncodeModeAll& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < N_SYS_COMB_CH; i++)
	{
		CConfigTable& tb1 = table[i];
		exchanger.exchange(tb1["EncodeParm"], "TransMode", config.vEncodeParam[i].iEncodeMode, s_combEncodeTransModeMap);
	}
}

template<> void exchangeTable<ChannelNameConfigAll>(CConfigTable& table, ChannelNameConfigAll& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchanger.exchange(table, i, config.channelTitle[i]);
	}
}

template<> void exchangeTableV2<ChannelNameConfigAll>(CConfigTable& table, ChannelNameConfigAll& config, int state, int nSize)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < nSize; i++)
	{
		exchanger.exchange(table, i, config.channelTitle[i]);
	}
}

static ConfigPair s_chanTypesMap[] = 
{
	{"Simulate", CHAN_TYPE_SIMULATE},
	{"Digit",	CHAN_TYPE_DIGIT},
	{NULL, }
};

template<> void exchangeTable<VideoChannel>(CConfigTable& table, VideoChannel& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Mode", config.iChnType, s_chanTypesMap);
	exchanger.exchange(table, "Channel", config.iChn);
}

template<> void exchangeTable<PlayBackConfig>(CConfigTable &table, PlayBackConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Channels", config.iChannels);

	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchanger.exchange(table["Volume"], i, config.iVolume[i]);
	}
	char searchType[64];

	if (config.iSearchMask == BITMSK(0))
	{
		strcpy(searchType, "ByTime");
	}
	else
	{
		strcpy(searchType, "ByCard");
	}

	exchanger.exchange(table, "SearchType", searchType);
	if (!strcmp(searchType, "ByTime"))
	{
		config.iSearchMask = BITMSK(0);
	}
	else 
	{
		config.iSearchMask = BITMSK(1);
	}

	exchanger.exchange(table, "Continue", config.bContinue);
}

static ConfigPair s_monitorTypeKey[] = 
{
	{"Monitor", TOUR_TYPES_MONITOR},
	{"Alarm", TOUR_TYPES_ALARM},
	{"Montion", TOUR_TYPES_MONTION},
	{NULL,}
};

template<> void exchangeTable<TourConfig>(CConfigTable &table, TourConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "Interval", config.iInterval);
	exchanger.exchange(table, "Type", config.iType, s_monitorTypeKey);
	exchanger.exchange(table, "Return", config.bReturn);

	CConfigTable &tbMask = table["Mask"];
	for(int i=0; i<N_SPLIT; i++)
	{
		exchanger.exchange(tbMask, i, config.iMask[i]);
	}
}

template<> void exchangeTable<TourConfigAll>(CConfigTable &table, TourConfigAll &config, int state)
{
	for (int i = 0; i < TOUR_TYPES_NR; i++)
	{
		exchangeTable(table[i], config.vTourConfigAll[i], state);
	}
}

static void limitValue(int& value, int min, int max)
{
	if (value < min)
	{
		value = min;
	}

	if (value > max)
	{
		value = max;
	}
}

template<> void exchangeTable<TVAdjustConfig>(CConfigTable &table, TVAdjustConfig &config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	limitValue(config.rctMargin.left, 0, 100);
	limitValue(config.rctMargin.top, 0, 100);
	limitValue(config.rctMargin.right, 0, 100);
	limitValue(config.rctMargin.bottom, 0, 100);

	exchanger.exchange(table["Margin"], 0, config.rctMargin.left);
	exchanger.exchange(table["Margin"], 1, config.rctMargin.top);
	exchanger.exchange(table["Margin"], 2, config.rctMargin.right);
	exchanger.exchange(table["Margin"], 3, config.rctMargin.bottom);

	exchanger.exchange(table["BlackMargin"], 0, config.blackMargin.left);
	exchanger.exchange(table["BlackMargin"], 1, config.blackMargin.top);
	exchanger.exchange(table["BlackMargin"], 2, config.blackMargin.right);
	exchanger.exchange(table["BlackMargin"], 3, config.blackMargin.bottom);
	exchanger.exchange(table, "Brightness", config.iBrightness);
	exchanger.exchange(table, "Contrast", config.iContrast);
	exchanger.exchange(table, "AntiDither", config.iAntiDither);
}

static ConfigPair s_audioInFormatMaps[] =
{
	{"G729_8KBIT", AUDIO_ENCODE_G729_8KBIT},
	{"G726_16KBIT", AUDIO_ENCODE_G726_16KBIT},
	{"G726_24KBIT", AUDIO_ENCODE_G726_24KBIT},
	{"G726_32KBIT", AUDIO_ENCODE_G726_32KBIT},
	{"G726_40KBIT", AUDIO_ENCODE_G726_40KBIT},
	{"PCM_8TO16BIT", AUDIO_ENCODE_PCM_8TO16BIT},
	{"PCM_ALAW", AUDIO_ENCODE_PCM_ALAW},
	{"PCM_ULAW", AUDIO_ENCODE_PCM_ULAW},
	{"ADPCM8K16BIT", AUDIO_ENCODE_ADPCM8K16BIT},
	{"ADPCM16K16BIT", AUDIO_ENCODE_ADPCM16K16BIT},
	{"G711_ALAW", AUDIO_ENCODE_G711_ALAW},
	{"MPEG2_LAYER1", AUDIO_ENCODE_MPEG2_LAYER1},
	{"AMR8K16BIT", AUDIO_ENCODE_AMR8K16BIT},
	{"G711_ULAW", AUDIO_ENCODE_G711_ULAW},
	{"IMA_ADPCM_8K16BIT", AUDIO_ENCODE_IMA_ADPCM_8K16BIT},
	{NULL},
};

void AudioInFormatExchange(CConfigTable& table, AudioInFormatConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "BitRate", config.iBitRate);
	exchanger.exchange(table, "SampleRate", config.iSampleRate);
	exchanger.exchange(table, "SampleBit", config.iSampleBit);
	exchanger.exchange(table, "EncodeType", config.iEncodeType, s_audioInFormatMaps);
}

template<> void exchangeTable<AudioInFormatConfig>(CConfigTable&table, AudioInFormatConfig& config, int state)
{
	AudioInFormatExchange(table, config, state);
}

template<> void exchangeTable<AudioInFormatConfigAll>(CConfigTable&table, AudioInFormatConfigAll& config, int state)
{
	for (int i = 0; i < AUDIO_ENCODE_TYPES_NR; i++)
	{
		AudioInFormatExchange(table[i], config.vAudioInFormatConfig[i], state);
	}
}


extern void exchangeTimeSection(CConfigTable& table, TimeSection& timesection, int state);

static ConfigPair s_recordModeTypeMaps[] = 
{
	{"ClosedRecord", RECORD_MODE_CLOSED},
	{"ManualRecord", RECORD_MODE_MANUAL},
	{"ConfigRecord", RECORD_MODE_CONFIG},
	{NULL,}
};

template<> void exchangeTable<RecordConfig>(CConfigTable& table, RecordConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PreRecord", config.iPreRecord);
	exchanger.exchange(table, "Redundancy", config.bRedundancy);
	exchanger.exchange(table, "PacketLength", config.iPacketLength);
	exchanger.exchange(table, "RecordMode", config.iRecordMode, s_recordModeTypeMaps);
	for (int i = 0; i < N_WEEKS; i++)
	{
		for (int j = 0; j < N_TSECT; j++)
		{
			exchangeTimeSection(table["TimeSection"][i][j], config.wcWorkSheet.tsSchedule[i][j], state);
		}
	}
	for (int i = 0; i < N_WEEKS; i++)
	{
		for (int j = 0; j < N_TSECT; j++)
		{
			exchanger.exchange(table["Mask"][i], j, config.typeMask[i][j]);
		}
	}
}

template<> void exchangeTable<RecordConfigAll>(CConfigTable& table, RecordConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vRecordConfigAll[i], state);
	}
}

template<> void exchangeTableV2<RecordConfigAll>(CConfigTable& table, RecordConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vRecordConfigAll[i], state);
	}
}

static ConfigPair s_snapModeTypeMaps[] = 
{
	{"ClosedSnap", SNAP_MODE_CLOSED},
	{"ManualSnap", SNAP_MODE_MANUAL},
	{"ConfigSnap", SNAP_MODE_CONFIG},
	{NULL,}
};

template<> void exchangeTable<SnapshotConfig>(CConfigTable& table, SnapshotConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "PreSnap", config.iPreSnap);
	exchanger.exchange(table, "Redundancy", config.bRedundancy);
	exchanger.exchange(table, "SnapMode", config.iSnapMode, s_snapModeTypeMaps);
	for (int i = 0; i < N_WEEKS; i++)
	{
		for (int j = 0; j < N_TSECT; j++)
		{
			exchangeTimeSection(table["TimeSection"][i][j], config.wcWorkSheet.tsSchedule[i][j], state);
		}
	}
	for (int i = 0; i < N_WEEKS; i++)
	{
		for (int j = 0; j < N_TSECT; j++)
		{
			exchanger.exchange(table["Mask"][i], j, config.typeMask[i][j]);
		}
	}
}

template<> void exchangeTable<SnapshotConfigAll>(CConfigTable& table, SnapshotConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vSnapshotConfigAll[i], state);
	}
}

template<> void exchangeTableV2<SnapshotConfigAll>(CConfigTable& table, SnapshotConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vSnapshotConfigAll[i], state);
	}
}

template<> void exchangeTable<WaterMarkConfig>(CConfigTable& table, WaterMarkConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	exchanger.exchange(table, "Enable", config.bEnable);
	exchanger.exchange(table, "Key", config.sKey);
	exchanger.exchange(table, "UserData", config.sUserData);
}

template<> void exchangeTable<WaterMarkConfigAll>(CConfigTable& table, WaterMarkConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vWaterMarkConfigAll[i], state);
	}
}

template<> void exchangeTableV2<WaterMarkConfigAll>(CConfigTable& table, WaterMarkConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vWaterMarkConfigAll[i], state);
	}
}

template<> void exchangeTableV2<EncoderPower>(CConfigTable& table, EncoderPower& config, int state, int nSize)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < nSize;i++)
	{
		exchanger.exchange(table["MaxResolution"], i, config.iMaxResolution[i], s_videoResolutionMap);
	}
}

static ConfigPair s_AudioModeTypeMaps[] = 
{
	{"Single", AUDIO_MODE_SINGLE},
	{"Double", AUDIO_MODE_DOUBLE},
	{NULL,}
};

template<> void exchangeTable<AudioVolumeAll>(CConfigTable& table, AudioVolumeAll& config, int state)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchanger.exchange(table[i], "AudioMode", config.Volume[i].AudioMode, s_AudioModeTypeMaps);
		exchanger.exchange(table[i], "RightVolume", config.Volume[i].iRInVolume);
		exchanger.exchange(table[i], "LeftVolume", config.Volume[i].iLInVolume);
	}
}

template<> void exchangeTableV2<AudioVolumeAll>(CConfigTable& table, AudioVolumeAll& config, int state, int nSize)
{
	CKeyExchange exchanger;

	exchanger.setState(state);
	for (int i = 0; i < nSize;i++)
	{
		exchanger.exchange(table[i], "AudioMode", config.Volume[i].AudioMode, s_AudioModeTypeMaps);
		exchanger.exchange(table[i], "RightVolume", config.Volume[i].iRInVolume);
		exchanger.exchange(table[i], "LeftVolume", config.Volume[i].iLInVolume);
	}
}


// OSDWidgetConfig 解析
template<> void exchangeTable<OSDWidgetConfig>(CConfigTable& table, OSDWidgetConfig& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchangeCover(table["AlarmInfo"], config.alarmInfo, state);
}

template<> void exchangeTable<OSDWidgetConfigAll>(CConfigTable& table, OSDWidgetConfigAll& config, int state)
{
	for (int i = 0; i < N_SYS_CH; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vOSDWidgetConfigAll[i], state);
	}
}

template<> void exchangeTableV2<OSDWidgetConfigAll>(CConfigTable& table, OSDWidgetConfigAll& config, int state, int nSize)
{

	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vOSDWidgetConfigAll[i], state);
	}
}

//----------------------------------Spot功能-------------------------------------------------
static ConfigPair s_spotType[] = 
{
	{"AsMain", SpotAsMain},
	{"AsAux", SpotAsAux},
	{NULL,	}
};

template<> void exchangeTable<SpotConfig>(CConfigTable& table, SpotConfig& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "TourEnable", config.bTourEnable);
	exchanger.exchange(table, "Type", config.iType, s_spotType);
	exchanger.exchange(table, "Interval", config.iInterval);

	CConfigTable &tbMask = table["Mask"];
	for(int i=0; i<N_SPLIT; i++)
	{
		exchanger.exchange(tbMask, i, config.iMask[i]);
	}

}

template<> void exchangeTable <SpotConfigAll>(CConfigTable & table, SpotConfigAll& config, int state)
{
	for (int i = 0; i < MAX_SPOT_NUMBER; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vSpotAllCfg[i], state);
	}
}

template<> void exchangeTableV2 <SpotConfigAll>(CConfigTable & table, SpotConfigAll& config, int state, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		const CConfigTable &tableValue = table[i];

		if (tableValue.type() == Json::nullValue && state == CKeyExchange::ES_LOADING)
		{
			continue;
		}
		exchangeTable(table[i], config.vSpotAllCfg[i], state);
	}
}


template<> void exchangeTable<VideoChannelSeq>(CConfigTable& table, VideoChannelSeq& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "FullScreen", config.bFullScreen);
	CConfigTable &tbChannelState = table["ChannelState"];
	CConfigTable &tbTopChSeq = table["TopChSeq"];
	CConfigTable &tbSplitChSeq = table["SplitChSeq"];
	CConfigTable &tbDecChNorSeq = table["DecChNorSeq"];
	CConfigTable &tbDecChSeq = table["DecChSeq"];
	for(int i=0; i < N_SYS_CH + N_DECORDR_CH; i++)
	{	
		exchanger.exchange(tbChannelState, i, config.bChannelState[i]);
		exchanger.exchange(tbTopChSeq, i, config.vTopChSeq[i]);
		exchanger.exchange(tbSplitChSeq, i, config.vSplitChSeq[i]);
	}
	
	for(int i = 0; i < N_PLY_CH + N_DECORDR_CH; i++)
	{
		exchanger.exchange(tbDecChNorSeq, i, config.vDecChResSeq[i]);
		exchanger.exchange(tbDecChSeq, i, config.vDecChSeq[i]);
	}
}

template<> void exchangeTableV2<VideoChannelSeq>(CConfigTable& table, VideoChannelSeq& config, int state, int nSize)
{
	CKeyExchange exchanger;
	exchanger.setState(state);
	exchanger.exchange(table, "FullScreen", config.bFullScreen);
	CConfigTable &tbChannelState = table["ChannelState"];
	CConfigTable &tbTopChSeq = table["TopChSeq"];
	CConfigTable &tbSplitChSeq = table["SplitChSeq"];
	CConfigTable &tbDecChNorSeq = table["DecChNorSeq"];
	CConfigTable &tbDecChSeq = table["DecChSeq"];
	for(int i=0; i < nSize; i++)
	{
		exchanger.exchange(tbChannelState, i, config.bChannelState[i]);
		exchanger.exchange(tbTopChSeq, i, config.vTopChSeq[i]);
		exchanger.exchange(tbSplitChSeq, i, config.vSplitChSeq[i]);
	}
	
	for(int i = 0; i < N_PLY_CH + N_DECORDR_CH; i++)
	{
		exchanger.exchange(tbDecChNorSeq, i, config.vDecChResSeq[i]);
		exchanger.exchange(tbDecChSeq, i, config.vDecChSeq[i]);
	}
}

//---------------   车载显示相关配置   -----------------
//车辆信息
void exchangeCarInfoDisplay(CConfigTable& table, CarInfoDisplay& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);

	exchangeCover(table["CarInfoWidget"], config.stCarInfoWidget, state);

}

//GPS信息
void exchangeGPSDisplay(CConfigTable& table, GPSDisplay& config, int state)
{
	CKeyExchange exchanger;
	exchanger.setState(state);

	exchangeCover(table["GPSWidget"], config.stGPSWidget, state);
}

template<> void exchangeTable<CarDisplay>(CConfigTable& table, CarDisplay& config, int state)
{
	CKeyExchange exchanger;

	exchanger.setState(state);

	exchangeCarInfoDisplay(table["CarInfoDisplay"], config.stCarInfoDisplay, state);
	exchangeGPSDisplay(table["GPSDisplay"], config.stGPSDisplay, state);
}

template<> void exchangeTable<EncodeStaticParamAll>(CConfigTable& table, EncodeStaticParamAll& config, int state)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	for (int i = 0; i < N_SYS_CH; i++)
	{
		exchanger.exchange(table[i], "Profile", config.vEncodeStaticParamAll[i].profile);
		exchanger.exchange(table[i], "Level", config.vEncodeStaticParamAll[i].level);
	}
}

template<> void exchangeTableV2<EncodeStaticParamAll>(CConfigTable& table, EncodeStaticParamAll& config, int state, int nSize)
{
	CKeyExchange exchanger;
	
	exchanger.setState(state);
	for (int i = 0; i < nSize; i++)
	{
		exchanger.exchange(table[i], "Profile", config.vEncodeStaticParamAll[i].profile);
		exchanger.exchange(table[i], "Level", config.vEncodeStaticParamAll[i].level);
	}
}

