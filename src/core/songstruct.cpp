#include "songstruct.h"

bool RMSong::Array2Song(const QByteArray &arr, SongClientItemStruct &song)
{
    Q_ASSERT(arr.length() == 0x33e);
    const char *data = arr.constData();

#define GETX(type, name, offset) \
    song.m_ ## name = *(reinterpret_cast<const type *>(data + offset))
#define GETSTR(name, offset) \
    song.m_ ## name = QString::fromUtf8(data + offset)

    GETX(short, ushSongID, 0x0);
    GETX(int, iVersion, 0x2);
    GETSTR(szSongName, 0x6);
    GETSTR(szPath, 0x46);
    GETSTR(szArtist, 0x86);
    GETSTR(szComposer, 0xc6);
    GETSTR(szSongTime, 0x106);
    GETX(int, iGameTime, 0x146);
    GETX(int, iRegion, 0x14a);
    GETX(int, iStyle, 0x14e);
    GETX(short, ucIsNew, 0x152);
    GETX(short, ucIsHot, 0x154);
    GETX(short, ucIsRecommend, 0x156);
    GETSTR(szBPM, 0x158);
    GETX(short, ucIsOpen, 0x198);
    GETX(bool, ucCanBuy, 0x19a);
    GETX(int, iOrderIndex, 0x19b);
    GETX(bool, bIsFree, 0x19f);
    GETX(bool, bSongPkg, 0x1a0);
    GETSTR(szFreeBeginTime, 0x1a1);
    GETSTR(szFreeEndTime, 0x1e1);
    GETX(short, ush4KeyEasy, 0x221);
    GETX(short, ush4KeyNormal, 0x223);
    GETX(short, ush4KeyHard, 0x225);
    GETX(short, ush5KeyEasy, 0x227);
    GETX(short, ush5KeyNormal, 0x229);
    GETX(short, ush5KeyHard, 0x22b);
    GETX(short, ush6KeyEasy, 0x22d);
    GETX(short, ush6KeyNormal, 0x22f);
    GETX(short, ush6KeyHard, 0x231);
    GETX(int, iPrice, 0x233);
    GETSTR(szNoteNumber, 0x237);
    GETSTR(szProductID, 0x2b7);
    GETX(int, iVipFlag, 0x337);
    GETX(bool, bIsHide, 0x33b);
    GETX(bool, bIsReward, 0x33c);
    GETX(bool, bIsLevelReward, 0x33d);

#undef GETSTR
#undef GETX

    return true;
}

bool RMSong::Song2Array(const SongClientItemStruct &song, QByteArray &arr)
{
    arr.clear();
    arr.resize(0x33e);

#define SETX(name, offset) do { \
        int length = sizeof(song.m_ ## name); \
        const char *data = reinterpret_cast<const char *>(&(song.m_ ## name)); \
        for (int i = 0; i < length; ++i) \
            arr[offset + i] = data[i]; \
    } while (false)

#define SETSTR(name, offset, len) do { \
        QByteArray dataArr = song.m_ ## name.toUtf8(); \
        const char *data = dataArr.constData(); \
        for (int i = 0; i < len; ++i) { \
            if (i < dataArr.length()) \
                arr[offset + i] = data[i]; \
            else \
                arr[offset + i] = '\0'; \
        } \
    } while(false)

    SETX(ushSongID, 0x0);
    SETX(iVersion, 0x2);
    SETSTR(szSongName, 0x6, 0x40);
    SETSTR(szPath, 0x46, 0x40);
    SETSTR(szArtist, 0x86, 0x40);
    SETSTR(szComposer, 0xc6, 0x40);
    SETSTR(szSongTime, 0x106, 0x40);
    SETX(iGameTime, 0x146);
    SETX(iRegion, 0x14a);
    SETX(iStyle, 0x14e);
    SETX(ucIsNew, 0x152);
    SETX(ucIsHot, 0x154);
    SETX(ucIsRecommend, 0x156);
    SETSTR(szBPM, 0x158, 0x40);
    SETX(ucIsOpen, 0x198);
    SETX(ucCanBuy, 0x19a);
    SETX(iOrderIndex, 0x19b);
    SETX(bIsFree, 0x19f);
    SETX(bSongPkg, 0x1a0);
    SETSTR(szFreeBeginTime, 0x1a1, 0x40);
    SETSTR(szFreeEndTime, 0x1e1, 0x40);
    SETX(ush4KeyEasy, 0x221);
    SETX(ush4KeyNormal, 0x223);
    SETX(ush4KeyHard, 0x225);
    SETX(ush5KeyEasy, 0x227);
    SETX(ush5KeyNormal, 0x229);
    SETX(ush5KeyHard, 0x22b);
    SETX(ush6KeyEasy, 0x22d);
    SETX(ush6KeyNormal, 0x22f);
    SETX(ush6KeyHard, 0x231);
    SETX(iPrice, 0x233);
    SETSTR(szNoteNumber, 0x237, 0x80);
    SETSTR(szProductID, 0x2b7, 0x80);
    SETX(iVipFlag, 0x337);
    SETX(bIsHide, 0x33b);
    SETX(bIsReward, 0x33c);
    SETX(bIsLevelReward, 0x33d);

#undef SETSTR
#undef SETX

    return true;
}

bool RMSong::IsBuy(const SongClientItemStruct &song)
{
    return song.m_ucCanBuy;
}

bool RMSong::IsDown(const SongClientItemStruct &song)
{
    return song.m_ucIsOpen == 0;
}

bool RMSong::IsFree(const SongClientItemStruct &song)
{
    return !IsBuy(song) && !IsDown(song) && !IsHidden(song) && !IsLevel(song) && !IsReward(song);
}

bool RMSong::IsHidden(const SongClientItemStruct &song)
{
    return song.m_bIsHide;
}

bool RMSong::IsLevel(const SongClientItemStruct &song)
{
    // BIG IMPORTANT HACK!!!!
    // RM used No251-262 as level reward, and m_bIsLevelReward doesn't make sense anymore.
    return song.m_ushSongID >= 251 && song.m_ushSongID <= 262;
}

bool RMSong::IsReward(const SongClientItemStruct &song)
{
    // Level reward now is also reward
    return song.m_bIsReward && !IsLevel(song);
}

bool RMSong::sortByID(const SongClientItemStruct &a, const SongClientItemStruct &b)
{
    return a.m_ushSongID < b.m_ushSongID;
}

bool RMSong::Array2Song(const QByteArray &arr, PapaSongClientItemStruct &song)
{
    Q_ASSERT(arr.length() == 0x169);
    const char *data = arr.constData();

#define GETX(type, name, offset) \
    song.m_ ## name = *(reinterpret_cast<const type *>(data + offset))
#define GETSTR(name, offset) \
    song.m_ ## name = QString::fromUtf8(data + offset)

    GETX(short, ushSongID, 0x0);
    GETX(int, iVersion, 0x2);
    GETSTR(szSongName, 0x6);
    GETX(char, cDifficulty, 0x46);
    GETX(char, cLevel, 0x47);
    GETSTR(szPath, 0x48);
    GETSTR(szArtist, 0x88);
    GETSTR(szSongTime, 0xc8);
    GETX(int, iGameTime, 0x108);
    GETSTR(szRegion, 0x10c);
    GETSTR(szStyle, 0x120);
    GETSTR(szBPM, 0x134);
    GETSTR(szNoteNumber, 0x148);
    GETX(int, iOrderIndex, 0x15c);
    GETX(char, ucIsOpen, 0x160);
    GETX(char, ucIsFree, 0x161);
    GETX(char, ucIsHide, 0x162);
    GETX(char, ucIsReward, 0x163);
    GETX(char, ucIsLevelReward, 0x164);
    GETX(int, iSongType, 0x165);

#undef GETSTR
#undef GETX

    return true;
}

bool RMSong::Song2Array(const PapaSongClientItemStruct &song, QByteArray &arr)
{
    arr.clear();
    arr.resize(0x169);

#define SETX(name, offset) do { \
        int length = sizeof(song.m_ ## name); \
        const char *data = reinterpret_cast<const char *>(&(song.m_ ## name)); \
        for (int i = 0; i < length; ++i) \
            arr[offset + i] = data[i]; \
    } while (false)

#define SETSTR(name, offset, len) do { \
        QByteArray dataArr = song.m_ ## name.toUtf8(); \
        const char *data = dataArr.constData(); \
        for (int i = 0; i < len; ++i) { \
            if (i < dataArr.length()) \
                arr[offset + i] = data[i]; \
            else \
                arr[offset + i] = '\0'; \
        } \
    } while(false)

    SETX(ushSongID, 0x0);
    SETX(iVersion, 0x2);
    SETSTR(szSongName, 0x6, 0x40);
    SETX(cDifficulty, 0x46);
    SETX(cLevel, 0x47);
    SETSTR(szPath, 0x48, 0x40);
    SETSTR(szArtist, 0x88, 0x40);
    SETSTR(szSongTime, 0xc8, 0x40);
    SETX(iGameTime, 0x108);
    SETSTR(szRegion, 0x10c, 0x14);
    SETSTR(szStyle, 0x120, 0x14);
    SETSTR(szBPM, 0x134, 0x14);
    SETSTR(szNoteNumber, 0x148, 0x14);
    SETX(iOrderIndex, 0x15c);
    SETX(ucIsOpen, 0x160);
    SETX(ucIsFree, 0x161);
    SETX(ucIsHide, 0x162);
    SETX(ucIsReward, 0x163);
    SETX(ucIsLevelReward, 0x164);
    SETX(iSongType, 0x165);

#undef SETSTR
#undef SETX

    return true;
}

bool RMSong::sortByID(const PapaSongClientItemStruct &a, const PapaSongClientItemStruct &b)
{
    return a.m_ushSongID < b.m_ushSongID;
}