

#include "AIS.h"

#include <QStringList>

/* Copied from util.h */

#define htonl(x) ( ( ((x)<<24) & 0xFF000000UL) | \
    (((x)<< 8) & 0x00FF0000UL) | \
    (((x)>> 8) & 0x0000FF00UL) | \
    (((x)>>24) & 0x000000FFUL) )
#define htons(x) ( ((x)<<8) | (((x)>>8)&0xFF) )
AIS_object_t::AIS_object_t()
{

}

QString AIS_object_t::printData()
{
    QString output;
    if(mMMSI)output.append(QString::fromUtf8("MMSI:")+QString::number(mMMSI)+"\n");
    if(mImo)output.append(QString::fromUtf8("IMO:")+QString::number(mImo)+"\n");
    if(mName.size())output.append(QString::fromUtf8("Tên:")+mName+"\n");
    if(mType)
    {
        output.append(QString::fromUtf8("Loại:"));
        switch(mType/10)
        {
        case 2:
            output.append(QString::fromUtf8("Thủy phi cơ"));
            break;
        case 3:
            if(mType==30)output.append(QString::fromUtf8("Tàu cá"));
            else if(mType==31||mType==32)output.append(QString::fromUtf8("Tàu kéo"));
            else output.append(QString::fromUtf8("Thuyền"));
            break;
        case 4:
            output.append(QString::fromUtf8("Tàu cao tốc"));
            break;
        case 5:
            output.append(QString::fromUtf8("Tàu chuyên dụng"));
            break;
        case 6:
            output.append(QString::fromUtf8("Tàu chở khách"));
            break;
        case 7:
            output.append(QString::fromUtf8("Tàu hàng"));
            break;
        case 8:
            output.append(QString::fromUtf8("Tàu chở dầu"));
            break;
        default:
            output.append(QString::fromUtf8("Không xác định"));
            break;
        }

        output.append("("+ QString::number(mType)+")");output.append("\n");
    }
    output.append(QString::fromUtf8("Kinh độ:")
                  +QString::number(mLong)
                  +"\n");
    output.append(QString::fromUtf8("Vĩ độ:")
                  +QString::number(mLat)
                  +"\n");
    output.append(QString::fromUtf8("Hướng di chuyển:")
                  + QString::number(mCog)
                  + "\n");
    output.append(QString::fromUtf8("Tốc độ:")
                  + QString::number(mSog)
                  +"Kn\n");
    output.append(QString::fromUtf8("Chiều dài:")
                  + QString::number(mBow+mStern)
                  + "m\n");
    output.append(QString::fromUtf8("Chiều rộng:")
                  + QString::number(mPort+mStarboard)
                  +"m\n");
    output.append(QString::fromUtf8("Điểm đến:")
                  + mDst
                  +"\n");
    return output;
}

const uint16_t AIS::AisParamLength[] = {
    2,   // AIS_PARAM_U8_REPEAT
    30,  // AIS_PARAM_U32_MMSI,
    4,   // AIS_PARAM_E_STATUS,
    8,   // AIS_PARAM_I8_TURN,
    10,  // AIS_PARAM_U16_SOG,
    1,   // AIS_PARAM_B_ACCURACY,
    28,  // AIS_PARAM_I32_LONG,
    27,  // AIS_PARAM_I32_LAT,
    12,  // AIS_PARAM_U16_COG,
    9,   // AIS_PARAM_U16_HEADING,
    6,   // AIS_PARAM_U8_SECOND,
    2,   // AIS_PARAM_E_MANEUVER,
    1,   // AIS_PARAM_B_RAIM,
    19,  // AIS_PARAM_U32_RADIO,
    2,   // AIS_PARAM_U8_AIS_VERSION,
    30,  // AIS_PARAM_U32_IMO,
    42,  // AIS_PARAM_T_CALLSIGN,
    120, // AIS_PARAM_T_SHIPNAME,
    8,   // AIS_PARAM_E_SHIPTYPE,
    9,   // AIS_PARAM_U16_TO_BOW,
    9,   // AIS_PARAM_U16_TO_STERN,
    6,   // AIS_PARAM_U8_TO_PORT,
    6,   // AIS_PARAM_U8_TO_STARBOARD,
    4,   // AIS_PARAM_E_EPFD,
    4,   // AIS_PARAM_U8_MONTH,
    5,   // AIS_PARAM_U8_DAY,
    5,   // AIS_PARAM_U8_HOUR,
    6,   // AIS_PARAM_U8_MINUTE,
    8,   // AIS_PARAM_U8_DRAUGHT,
    120, // AIS_PARAM_T_DESTINATION,
    1,   // AIS_PARAM_B_DTE,
    1,   // AIS_PARAM_B_CS,
    1,   // AIS_PARAM_B_DISPLAY,
    1,   // AIS_PARAM_B_DSC,
    1,   // AIS_PARAM_B_BAND,
    1,   // AIS_PARAM_B_MSG22,
    1,   // AIS_PARAM_B_ASSIGNED,
    2,   // AIS_PARAM_U8_PARTNO,
    18,  // AIS_PARAM_T_VENDORID,
    4,   // AIS_PARAM_U8_MODEL,
    20,  // AIS_PARAM_U32_SERIAL,
    30,  // AIS_PARAM_U32_MOTHERSHIP_MMSI,
};

const struct AIS::AisTypeMsgPair AIS::AisMsgTypes[] = {
{ AIS_MSG_1_2_3_POS_REPORT_CLASS_A,     1 },
{ AIS_MSG_1_2_3_POS_REPORT_CLASS_A,     2 },
{ AIS_MSG_1_2_3_POS_REPORT_CLASS_A,     3 },
{ AIS_MSG_4_BASE_STATION_REPORT,        4 },
{ AIS_MSG_5_STATIC_AND_VOYAGE,          5 },
{ AIS_MSG_18_CS_POS_REPORT_CLASS_B,     18 },
{ AIS_MSG_19_CS_POS_REPORT_EXT_CLASS_B, 19 },
{ AIS_MSG_24_STATIC_DATA_REPORT,        24 },
{ AIS_MSG_21_AID_TO_NAVIGATION,         21 },
{ AIS_MSG_MAX, 0 } // Must be last
};

const struct AIS::AisParamPosPair AIS::AisMsgPosReportClassA[] = {
{ AIS_PARAM_E_STATUS,      38 },
{ AIS_PARAM_I8_TURN,       42 },
{ AIS_PARAM_U16_SOG,       50 },
{ AIS_PARAM_B_ACCURACY,    60 },
{ AIS_PARAM_I32_LONG,      61 },
{ AIS_PARAM_I32_LAT,       89 },
{ AIS_PARAM_U16_COG,      116 },
{ AIS_PARAM_U16_HEADING,  128 },
{ AIS_PARAM_U8_SECOND,    137 },
{ AIS_PARAM_E_MANEUVER,   143 },
{ AIS_PARAM_B_RAIM,       148 },
{ AIS_PARAM_U32_RADIO,    149 },
{ AIS_PARAM_MAX, 0 } // Must be last
};

const struct AIS::AisParamPosPair AIS::AisMsgBaseStationReport[] = {
{ AIS_PARAM_MAX, 0 } // Must be last
};

const struct AIS::AisParamPosPair AIS::AisMsgStaticAndVoyage[] = {
{ AIS_PARAM_U8_AIS_VERSION,   38 },
{ AIS_PARAM_U32_IMO,          40 },
{ AIS_PARAM_T_CALLSIGN,       70 },
{ AIS_PARAM_T_SHIPNAME,      112 },
{ AIS_PARAM_E_SHIPTYPE,      232 },
{ AIS_PARAM_U16_TO_BOW,      240 },
{ AIS_PARAM_U16_TO_STERN,    249 },
{ AIS_PARAM_U8_TO_PORT,      258 },
{ AIS_PARAM_U8_TO_STARBOARD, 264 },
{ AIS_PARAM_E_EPFD,          270 },
{ AIS_PARAM_U8_MONTH,        274 },
{ AIS_PARAM_U8_DAY,          278 },
{ AIS_PARAM_U8_HOUR,         283 },
{ AIS_PARAM_U8_MINUTE,       288 },
{ AIS_PARAM_U8_DRAUGHT,      294 },
{ AIS_PARAM_T_DESTINATION,   302 },
{ AIS_PARAM_B_DTE,           422 },
{ AIS_PARAM_MAX, 0 } // Must be last
};

const struct AIS::AisParamPosPair AIS::AisMsgCsPosReportClassB[] = {
{ AIS_PARAM_U16_SOG,          46 },
{ AIS_PARAM_B_ACCURACY,       56 },
{ AIS_PARAM_I32_LONG,         57 },
{ AIS_PARAM_I32_LAT,          85 },
{ AIS_PARAM_U16_COG,         112 },
{ AIS_PARAM_U16_HEADING,     124 },
{ AIS_PARAM_U8_SECOND,       133 },
{ AIS_PARAM_B_CS,            141 },
{ AIS_PARAM_B_DISPLAY,       142 },
{ AIS_PARAM_B_DSC,           143 },
{ AIS_PARAM_B_BAND,          144 },
{ AIS_PARAM_B_MSG22,         145 },
{ AIS_PARAM_B_ASSIGNED,      146 },
{ AIS_PARAM_B_RAIM,          147 },
{AIS_PARAM_U32_RADIO,        148 },
{ AIS_PARAM_MAX, 0 } // Must be last
};
const struct AIS::AisParamPosPair AIS::AisMsgCsPosReportExtClassB[] = {
{ AIS_PARAM_U16_SOG,          46 },
{ AIS_PARAM_B_ACCURACY,       56 },
{ AIS_PARAM_I32_LONG,         57 },
{ AIS_PARAM_I32_LAT,          85 },
{ AIS_PARAM_U16_COG,         112 },
{ AIS_PARAM_U16_HEADING,     124 },
{ AIS_PARAM_U8_SECOND,       133 },
{ AIS_PARAM_T_SHIPNAME,      143 },
{ AIS_PARAM_E_SHIPTYPE,      263 },
{ AIS_PARAM_U16_TO_BOW,      271 },
{ AIS_PARAM_U16_TO_STERN,    280 },
{ AIS_PARAM_U8_TO_PORT,      289 },
{ AIS_PARAM_U8_TO_STARBOARD, 295 },
{ AIS_PARAM_MAX, 0 } // Must be last
};

const struct AIS::AisParamPosPair AIS::AisMsgStaticDataRaport[] = {
{ AIS_PARAM_U8_PARTNO,        38 }, // A-B switch
{ AIS_PARAM_T_SHIPNAME,       40 }, // A
{ AIS_PARAM_E_SHIPTYPE,       40 }, // B
{ AIS_PARAM_T_VENDORID,       48 },
{ AIS_PARAM_U8_MODEL,         66 },
{ AIS_PARAM_U32_SERIAL,       70 },
{ AIS_PARAM_T_CALLSIGN,       90 },
{ AIS_PARAM_U16_TO_BOW,      132 },
{ AIS_PARAM_U16_TO_STERN,    141 },
{ AIS_PARAM_U8_TO_PORT,      150 },
{ AIS_PARAM_U8_TO_STARBOARD, 156 },
{ AIS_PARAM_U32_MOTHERSHIP_MMSI, 132 }, // Either this or bow,stern, port, starboard
{ AIS_PARAM_MAX, 0 } // Must be last
};

const struct AIS::AisParamPosPair AIS::AisAidsToNavReport[] = {
{ AIS_PARAM_T_SHIPNAME,       42 }, // A
{ AIS_PARAM_I32_LONG,         164 },
{ AIS_PARAM_I32_LAT,          192 },
{ AIS_PARAM_MAX, 0 } // Must be last
};
const struct AIS::AisParamPosPair* AIS::AisMsgParams[AIS_MSG_MAX] = {
    &AisMsgPosReportClassA[0],
    &AisMsgBaseStationReport[0],
    &AisMsgStaticAndVoyage[0],
    &AisMsgCsPosReportClassB[0],
    &AisMsgCsPosReportExtClassB[0],
    &AisMsgStaticDataRaport[0],
    &AisAidsToNavReport[0]
};

AIS::AIS()
{
    //msglen = 0;
    lastMesID = -1;
}
bool AIS::ProcessPayload(const char *AISbitstream, unsigned int fillBits)
{
    //msglen = 0;
    uint8_t* tmp = (uint8_t*)AISbitstream;
    int i = 0;
    while (*tmp != '\0') {
        msg[i] = *tmp;
        i++;
        tmp++;
    }
    msg[i] = *tmp; // Terminate msg

    // Time to decode the AIS data
    decode(fillBits);
    uint8_t msgNum;
    getdata(0,6, &msgNum);  // Will be used a lot

    msgType = numericToMessage(msgNum);
    if(msgType==AIS_MSG_MAX)
    {
        return false;// message not supported
    }
    msgNumeric = msgNum;
    return true;
}




/*
 * Decode msg into it self. 4 bytes becomes 3, i.e. it is OK to do it
 * cnt    byte
 *  0   --qqqqqq
 *  1   --yyyyyy
 *  2   --zzzzzz
 *  3   --wwwwww
 *
 * becomes | qq qq qq yy | yy yy zz zz | zz ww ww ww |
 */
void AIS::decode(unsigned int fillBits)
{
    unsigned int srcIdx=0;

    /* First convert to binary */
    while (msg[srcIdx] != '\0') {
        msg[srcIdx] -= '0';
        if (msg[srcIdx] > 40) {
            msg[srcIdx] -= 8;
        }
        srcIdx++;
    }
    msgLen = srcIdx; /* For now in bytes - later in bits */

    /* Now compress resulting 6bits values */
    unsigned int dstIdx=0;
    unsigned int cnt=0;
    uint8_t src;

    srcIdx = 0;
    while (srcIdx != msgLen) {
        src = msg[srcIdx];
        switch (cnt) {
        case 0:
            msg[dstIdx] = (src << 2);
            break;
        case 1:
            msg[dstIdx] |= (src >> 4);
            dstIdx++;
            msg[dstIdx] = (src << 4);
            break;
        case 2:
            msg[dstIdx] |= (src >> 2);
            dstIdx++;
            msg[dstIdx] = (src << 6);
            break;
        case 3:
            msg[dstIdx] |= src;
            dstIdx++;
            break;
        }
        srcIdx++;
        cnt++;
        if (cnt==4) {
            cnt = 0;
        }
    }
    /* Store msgLen as bit count */
    msgLen = srcIdx*6 - fillBits;
}

int AIS::getbit(unsigned int idx)
{
    int byteIdx = idx / 8;
    int bitIdx = 7 - (idx % 8);

    return ((msg[byteIdx] >> bitIdx) & 0x01);
}

/*
 * getdata return in data cnt bits from decoded AIS data, starting from begin
 * If not enough data function returns false, otherwise true
 */
bool AIS::getdata(unsigned int begin, unsigned int cnt, uint8_t *data, bool isSigned)
{
    if (begin+cnt > msgLen)
        return false;

    unsigned int srcIdx = begin;
    unsigned int dstIdx = 0;
    unsigned int dstBitIdx = cnt % 8;

    data[dstIdx] = 0;
    if (isSigned) {
        if (getbit(srcIdx) == 1) {
            /* Pre-pend with 1's */
            const static uint8_t signmask[8] =
            { 0x00, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80};

            data[dstIdx] = signmask[dstBitIdx];
        }
    }

    if (dstBitIdx == 0)
        dstBitIdx = 8;

    while (cnt--) {
        dstBitIdx--;
        data[dstIdx] |= (getbit(srcIdx) << dstBitIdx);
        srcIdx++;
        if (cnt && (0 == dstBitIdx)) {
            dstBitIdx = 8;
            dstIdx++;
            data[dstIdx] = 0;
        }
    }

    return true;
}

AIS_object_t AIS::GetAisObject()
{
    AIS_object_t obj;
    obj.mMMSI = get_mmsi();

    if(get_type()==AIS::AIS_MSG_24_STATIC_DATA_REPORT)
    {
        if(get_partno()==0)
        {
            obj.mName = QString::fromLatin1(get_shipname());
            obj.mType = 0;
        }
        else
        {
            obj.mType = get_shiptype();

        }
    }
    else
    {
        obj.mName = QString::fromLatin1(get_shipname());
        obj.mType = get_shiptype();
    }
    obj.mDst = QString(get_destination());
    obj.mImo = get_imo();
    obj.mNavStat = get_navStatus();
    obj.mBow = get_to_bow();
    obj.mStern = get_to_stern();
    obj.mStarboard = get_to_starboard();
    obj.mPort = get_to_port();
    obj.mSog = get_SOG()/10.0;
    obj.mCog = get_COG()/10.0;
    obj.mLat = get_latitude()/600000.0;
    obj.mLong = get_longitude()/600000.0;
    obj.mLut = QDateTime::currentMSecsSinceEpoch();
    obj.isNewest = true;
    obj.isSelected = false;
    obj.mName.replace('@',"");

    return  obj;
}

bool AIS::getParamStart(enum AIS::Nmea0183AisParams param, unsigned& start)
{
    if(msgType==AIS_MSG_MAX)return false;
    const struct AisParamPosPair* pparam = AisMsgParams[msgType];

    while (pparam->param != AIS_PARAM_MAX) {
        if (pparam->param == param) {
            start = pparam->start;
            return true;
        }
        pparam++;
    }

    return false;

}

enum AIS::Nmea0183AisMessages AIS::numericToMessage(uint8_t msgNumeric)
{
    unsigned i = 0;
    while (AisMsgTypes[i].msgEnum != AIS_MSG_MAX) {
        if (AisMsgTypes[i].msgNumeric == msgNumeric) {
            return AisMsgTypes[i].msgEnum;
        }
        i++;
    }
    printf("\nAIS unsupported type:%d",msgNumeric);
    return AIS_MSG_MAX;
}

uint32_t AIS::get_u32(unsigned start, unsigned len)
{
    uint32_t val;

    if (start + len > msgLen) return 0;
    getdata(start, len, reinterpret_cast<uint8_t*>(&val));
    return htonl(val);
}

uint32_t AIS::get_u32(enum AIS::Nmea0183AisParams param)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        return 0;
    }
    unsigned int len = AisParamLength[param];
    return get_u32(start,len);
}


int32_t AIS::get_i32(unsigned start, unsigned len)
{
    int32_t val;

    if (start + len > msgLen) return 0;

    getdata(start, len, reinterpret_cast<uint8_t*>(&val), true);
    return htonl(val);
}

int32_t AIS::get_i32(enum AIS::Nmea0183AisParams param)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        return 0;
    }
    unsigned int len = AisParamLength[param];
    return get_i32(start,len);
}


uint16_t AIS::get_u16(unsigned start, unsigned len)
{
    uint16_t val;

    if (start + len > msgLen) return 0;
    getdata(start, len, reinterpret_cast<uint8_t*>(&val));
    return htons(val);
}

uint16_t AIS::get_u16(enum AIS::Nmea0183AisParams param)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        return 0;
    }
    unsigned int len = AisParamLength[param];
    return get_u16(start,len);
}


uint8_t AIS::get_u8(unsigned start, unsigned len)
{
    uint8_t val;

    if (start + len > msgLen) return 0;
    getdata(start,len, &val);
    return val;
}

uint8_t AIS::get_u8(enum AIS::Nmea0183AisParams param)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        return 0;
    }
    unsigned int len = AisParamLength[param];
    return get_u8(start,len);
}


int8_t AIS::get_i8(unsigned start, unsigned len)
{
    int8_t val;

    if (start + len > msgLen) return 0;
    getdata(start,len, reinterpret_cast<uint8_t*>(&val), true);
    return val;
}

int8_t AIS::get_i8(enum AIS::Nmea0183AisParams param)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        return 0;
    }
    unsigned int len = AisParamLength[param];
    return get_i8(start,len);
}


void AIS::get_string(char* str, unsigned start, unsigned cnt)
{
    const unsigned charlen = 6; // Number of bits per char
    unsigned i;
    for (i = 0; i < cnt; i++) {
        uint8_t sixBitChar = get_u8(start, charlen);
        if (sixBitChar == '\0') {
            break;
        }
        if (sixBitChar < ' ') {
            str[i] = '@' + sixBitChar;
        } else {
            str[i] = sixBitChar;
        }
        start += charlen;
    }
    str[i] = '\0';
}



const char* AIS::get_string(enum AIS::Nmea0183AisParams param, char* str)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        str[0] = '\0';
        return 0;
    }
    unsigned int len = AisParamLength[param];

    get_string(str, start, len/bits_pr_char);
    return str;
}
bool AIS::ProcessNMEA(QString data)
{
    //if(!data.contains('*'))return false;
    //int checkSum = data.split('*').at(1).toInt();
    QStringList fieldList = data.split(',');
    if(fieldList.size()<7)return false;
    if(!(fieldList.at(0).contains("AIVD")))return false;
    int numOfSentence = fieldList.at(1).toInt();
    int senNum = fieldList.at(2).toInt();
    int padding = fieldList.at(6).split('*').at(0).toInt();
    if(numOfSentence==1)return ProcessPayload(fieldList.at(5).toStdString().data(),padding);
    else if(numOfSentence==2)
    {
        if(senNum==1)
        {
            payloadFirstHalf = fieldList.at(5);
            lastMesID =fieldList.at(3).toInt();
        }
        else if(senNum==2)
        {
            if(lastMesID==fieldList.at(3).toInt())
            {
                payloadFirstHalf += fieldList.at(5);
                return ProcessPayload(payloadFirstHalf.toStdString().data(),padding);
                lastMesID=-1;
            }

        }

    }
    else
    {
        return false;
    }


    return true;
}

bool AIS::get_flag(enum AIS::Nmea0183AisParams param)
{
    unsigned int start;
    if (! getParamStart(param, start)) {
        return false;
    }

    return (get_u8(start,1) == 1);
}



