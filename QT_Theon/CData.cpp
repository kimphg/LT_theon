//=============================================================================
//
// CData.cpp: implementation of the C2_Rada class.
//
//=============================================================================

#include "StdAfx.h"
#include "CData.h"

#include <math.h>



//=============================================================================
// C2_Mark Class
//=============================================================================
C2_Mark::C2_Mark(void)
{
	m_TypSub = 0;
}

C2_Mark::~C2_Mark(void)
{}

void C2_Mark::Copy(C2_Mark *pMark)
{
    C2_Place::Update(pMark);
	
    m_TypSub = pMark->m_TypSub;	
}

CString C2_Mark::GetStrType()
{
	return GetStrType(m_Type);
}

CString C2_Mark::GetStrType(int nId)
{
    _TCHAR *szType[5] = {
		_T("Kiểu chữ X"),	_T("Kiểu chữ O"),	_T("Kiểu dấu *"),		
		_T("Kiểu dấu +"), 	_T("Kiểu dấu #")	};
		
	return (nId < 5)? szType[nId] : _T("...");
}


//=============================================================================
// RADA class
//=============================================================================
C2_Rada::C2_Rada()
{
	m_Status = 0x00;		// b0 = View
}

C2_Rada::~C2_Rada()
{}

void C2_Rada::Copy(C2_Rada *pRada)
{
    C2_Sector::Copy(pRada);

	m_Status = pRada->m_Status;
//	m_IsView = pRada->m_IsView;
}

CString C2_Rada::GetStrType()
{
	return GetStrType(m_Type);
}

CString C2_Rada::GetStrType(int nId)
{
    _TCHAR *szType[5] = {
		_T("ra đa tầm xa"),		_T("ra đa tầm trung"),	_T("ra đa ven bờ"),		
		_T("ra đa thụ động"), 	_T("ra đa khác")	};
		
	return (nId < 5)? szType[nId] : _T("...");
}

bool C2_Rada::CheckSelect (C2_Point *pPoint, UINT nScale)
{
	int nDrg;		// [pixel]
	
	nDrg = GetDistPoint(pPoint);
	if (nDrg <= 5)
		return true;
	
	UINT	nAzi;
	long	nRge;
	
	pPoint->ConvWGSToPol(this, nRge, nAzi);
	
	// Check near line 1-2
	if (nDrg > int(m_RgMax * LBS_RGE/nScale) + 5)
		return false;
	
	C2_Point	nPoint;
	
	nPoint.x = x + int(nDrg * sin(m_AzBeg*M_PI/(1<<15)));
	nPoint.y = y - int(nDrg * cos(m_AzBeg*M_PI/(1<<15)));	
	if (pPoint->GetDistLine(&nPoint, this) <= 5)
		return true;
	
	nPoint.x = x + int(nDrg * sin(m_AzEnd*M_PI/(1<<15)));
	nPoint.y = y - int(nDrg * cos(m_AzEnd*M_PI/(1<<15)));	
	if (pPoint->GetDistLine(&nPoint, this) <= 5)
		return true;
	
	// Check near Arc
	if ((nDrg < int(m_RgMax * LBS_RGE / nScale - 4)) || 
		(nDrg > int(m_RgMax * LBS_RGE / nScale + 4)))
		return false;
	
	if		(m_AzBeg == m_AzEnd)	// Circle
		return true;
	else if (m_AzBeg <  m_AzEnd)
	{
		if ((nAzi <= m_AzEnd) && (nAzi >= m_AzBeg))
			return true;
		else
			return false;
	}
	else
	{
		if ((nAzi > m_AzEnd) && (nAzi < m_AzBeg))
			return false;
		else
			return true;
	}
}


//=============================================================================
// BATE class
//=============================================================================
C2_Bate::C2_Bate()
{}

C2_Bate::~C2_Bate()
{}

void C2_Bate::Copy(C2_Bate *pBate)
{
    C2_Rada::Copy(pBate);
}

CString C2_Bate::GetStrType()
{
	return GetStrType(m_Type);
}

CString C2_Bate::GetStrType(int nId)
{
    _TCHAR *szType[9] = {
		_T("Sở chỉ huy lữ"),	_T("SCH trung đòan"),	_T("SCH tiểu đoàn"),
		_T("Tên lửa bờ"),		_T("Pháo bờ biển"),		_T("Sân bay quân sự"),
		_T("Đội tàu pháo"),		_T("Đội tàu hộ vệ"),	_T("Đội tàu tên lửa")};
		
	return (nId < 9)? szType[nId] : _T("...");
}


//=============================================================================
// C2_Unit Class
//=============================================================================
C2_Unit::C2_Unit()
{
}

C2_Unit::~C2_Unit()
{}

void C2_Unit::Copy(C2_Unit *pUnit)
{
    C2_Mark::Copy(pUnit);
}

CString C2_Unit::GetStrType()
{
	return GetStrType(m_Type);
}

CString C2_Unit::GetStrType(int nId)
{
    _TCHAR *szType[79] = {
		_T("SCH quân chủng"),			_T("SCH quân đoàn"),			_T("Bộ CH QS tỉnh"),			_T("SCH sư đoàn"),
		_T("SCH lữ đoàn"),				_T("SCH trung đoàn"),			_T("SCH trung đoàn đp"),		_T("Xe tăng chiến đấu"),
		_T("Xe thiếp giáp"),			_T("Xe ôtô cơ giới"),			_T("Pháo lựu 122mm"),			_T("Pháo lựu 155mm"),
		_T("Pháo nòng dài 122mm"),		_T("Pháo nòng dài 155mm"),		_T("Pháo nòng dài 122mm"),		_T("Pháo nòng dài 155mm"),
		_T("Trận địa cối 81-82"),		_T("Tên lửa chiến dịch"),		_T("Trạm TT tiếp sức"),			_T("Trạm TT viba"),				
		_T("Trạm VSAT vệ tinh"),		_T("Trạm tổng đài TT"),			_T("Trạm TT vô tuyến"),			_T("Trinh sát vô tuyến"),		
		_T("Định vị sóng ngắn"),		_T("Gây nhiễu sóng ngắn"),		_T("Đài gây nhiễu rađa"),		_T("Hầm ngầm công binh"),		
		_T("Cầu bê tông - gỗ"),			_T("Cầu sắt qua sông"),			_T("Bến phà vượt sông"),		_T("P.đội tên lửa vác vai"),
		_T("Tên lửa PK tầm gần"),		_T("Tên lửa PK tầm trung"),		_T("Tên lửa PK tầm xa"),		_T("Pháo cao xạ cỡ nhỏ"),		
		_T("Pháo cao xạ cỡ trung"),		_T("Pháo PK tự hành"),			_T("Trận địa súng máy PK"),		_T("Rađa cảnh giới sóng m"),
		_T("Rađa c.giới sóng dm"),		_T("Rađa c.giới sóng cm"),		_T("Đài rađa đo cao"),			_T("Đài dẫn đường t.không"),
		_T("Trạm chỉ huy d.đường"),		_T("Rađa cảnh giới biển"),		_T("Máy bay tiêm kích"),		_T("May bay tiêm kích bom"),	
		_T("M.bay ném bom p.lực"),		_T("Máy bay trinh sát"),		_T("Trực thăng vũ trang"),		_T("Trực thăng vận tải"),
		_T("Đài quan sát mắt"),			_T("Đài dẫn đường KQ"),			_T("Sân bay cấp 1"),			_T("Sân bay cấp 2"),
		_T("Sân bay cấp 3"),			_T("Căn cứ hải quân"),			_T("Tàu sân bay"),				_T("Tàu tuần dương"),
		_T("Tàu khu trục"),				_T("Tàu hậu vệ"),				_T("Tàu pháo"),					_T("Tàu tên lửa"),				
		_T("Tàu tuần tiễu"),			_T("Tàu ngầm Diêzn"),			_T("Tàu quét lôi"),				_T("Tàu đổ bộ"),
		_T("Vận tải quâ sự"),			_T("Tàu cá"),					_T("Vị trí neo tàu"),			_T("Kho quân chủng"),
		_T("Nhà máy sản xuất"),			_T("Xưởng sản xuất"),			_T("Bệnh viện quân chủng"),		_T("Bệnh viện sư đoàn"),
		_T("Trạm quân y tiểu đoàn"),	_T("Trạm cấp xăng dầu"),		_T("Trạm x.dầu dã chiến")};

		return (nId < 79)? szType[nId] : _T("...");
}


//=============================================================================
// HOME Class
//=============================================================================
C2_Home::C2_Home()
{}

C2_Home::~C2_Home()
{}

void C2_Home::Copy(C2_Home *pHome)
{
    C2_Mark::Copy(pHome);
}

CString C2_Home::GetStrType()
{
	return GetStrType(m_Type);
}

CString C2_Home::GetStrType(int nId)
{
    _TCHAR *szType[10] = {
		_T("Dàn khoan"),	_T("Nhà giàn"),		_T("Nhà bê tông"),		_T("Đốc nổi"), 
		_T("Phao biển"),	_T("Khoan thăm dò"),_T("Trạm khí tượng"), 	_T("Đèn biển"),
		_T("Đảo chìm"),		_T("Đảo nổi")	};
		
	return (nId < 10)? szType[nId] : _T("...");
}


//=============================================================================
// CC_Data Class
//=============================================================================
CC_Data::CC_Data()
{
	m_szRoot = "";
}

CC_Data::~CC_Data()
{
	Clear();
}

//-----------------------------------------------------------------------------
void CC_Data::Clear()
{
    Clear(TYPE_DATA_MARK);
	Clear(TYPE_DATA_RADA);
	Clear(TYPE_DATA_BATE);
	Clear(TYPE_DATA_UNIT);
    Clear(TYPE_DATA_HOME);
}

void CC_Data::Clear(BYTE nType)
{
	POSITION	nPos;
	
	if ((nType == TYPE_DATA_MARK) && !m_ListMark.IsEmpty())
	{
		nPos = m_ListMark.GetHeadPosition();
		while (nPos)
			delete m_ListMark.GetNext(nPos);
		m_ListMark.RemoveAll();
	}
	
	if ((nType == TYPE_DATA_RADA) && !m_ListRada.IsEmpty())
	{
		nPos = m_ListRada.GetHeadPosition();
		while (nPos)
			delete m_ListRada.GetNext(nPos);
		m_ListRada.RemoveAll();
	}	

    if ((nType == TYPE_DATA_BATE) && !m_ListBate.IsEmpty())
	{
		nPos = m_ListBate.GetHeadPosition();
		while (nPos)
			delete m_ListBate.GetNext(nPos);
		m_ListBate.RemoveAll();
	}

    if ((nType == TYPE_DATA_UNIT) && !m_ListUnit.IsEmpty())
	{
		nPos = m_ListUnit.GetHeadPosition();
		while (nPos)
			delete m_ListUnit.GetNext(nPos);
		m_ListUnit.RemoveAll();
	}

	if ((nType == TYPE_DATA_HOME) && !m_ListHome.IsEmpty())
	{
		nPos = m_ListHome.GetHeadPosition();
		while (nPos)
			delete m_ListHome.GetNext(nPos);
		m_ListHome.RemoveAll();
	}
}

//-----------------------------------------------------------------------------
//	Get new ID for data
//-----------------------------------------------------------------------------
BYTE CC_Data::GetUid(BYTE nType)
{
	POSITION	nPos;
	BYTE		nUid = 0;
	bool		bFind= false;

	switch (nType)
	{
		case TYPE_DATA_MARK:
			C2_Mark	*pMark;
			
			while (!bFind)
			{
				nUid  ++;
				bFind = true;
				nPos  = m_ListMark.GetHeadPosition();
				while (nPos && bFind)
				{
					pMark = m_ListMark.GetNext(nPos);
					if (pMark->m_Uid == nUid)	
						bFind = false;
				}
			}
			break;

		case TYPE_DATA_RADA:
			C2_Rada *pRada;

			while (!bFind)
			{
				nUid  ++;
				bFind = true;
				nPos  = m_ListRada.GetHeadPosition();
				while (nPos && bFind)
				{
					pRada = m_ListRada.GetNext(nPos);
					if (pRada->m_Uid == nUid)
						bFind = false;
				}
			}
			break;

		case TYPE_DATA_BATE:
			C2_Bate *pBate;
			
			while (!bFind)
			{
				nUid  ++;
				bFind = true;
				nPos  = m_ListBate.GetHeadPosition();
				while (nPos && bFind)
				{
					pBate = m_ListBate.GetNext(nPos);
					if (pBate->m_Uid == nUid)
						bFind = false;
				}
			}
			break;

		case TYPE_DATA_UNIT:
			C2_Unit	*pUnit;
			
			while (!bFind)
			{
				nUid  ++;
				bFind = true;
				nPos  = m_ListUnit.GetHeadPosition();
				while (nPos && bFind)
				{
					pUnit = m_ListUnit.GetNext(nPos);
					if (pUnit->m_Uid == nUid)
						bFind = false;
				}
			}
			break;

		case TYPE_DATA_HOME:
			C2_Home *pHome;
			
			while (!bFind)
			{
				nUid  ++;
				bFind = true;
				nPos  = m_ListHome.GetHeadPosition();
				while (nPos && bFind)
				{
					pHome = m_ListHome.GetNext(nPos);
					if (pHome->m_Uid == nUid)	
						bFind = false;
				}
			}
			break;

		default:
			break;
	}
	return nUid;
}

//-----------------------------------------------------------------------------
//	Find data: by Type, Uid;	Return: Pointer of Data, Index
//-----------------------------------------------------------------------------
void * CC_Data::Find(BYTE nType, BYTE nUid)
{
	int		nIndex;
	return  Find(nType, nUid, nIndex);
}

void * CC_Data::Find(BYTE nType, BYTE nUid, int &nIndex)
{
	POSITION	nPos;

	switch (nType)
	{
		case TYPE_DATA_MARK:
			C2_Mark	*pMark;
			
			nPos	= m_ListMark.GetHeadPosition();
			nIndex	= 0;
			while (nPos)
			{
				pMark = m_ListMark.GetNext(nPos);
				if (pMark->m_Uid == nUid)
					return pMark;
				
				nIndex ++;
			}
			return NULL;
		
		case TYPE_DATA_RADA:
			C2_Rada *pRada;

			nPos	= m_ListRada.GetHeadPosition();
			nIndex	= 0;
			while (nPos)
			{
				pRada = m_ListRada.GetNext(nPos);
				if (pRada->m_Uid == nUid)
					return pRada;
				
				nIndex ++;
			}
			return NULL;

		case TYPE_DATA_BATE:
			C2_Bate	*pBate;
			
			nPos	= m_ListBate.GetHeadPosition();
			nIndex	= 0;
			while (nPos)
			{
				pBate = m_ListBate.GetNext(nPos);
				if (pBate->m_Uid == nUid)
					return pBate;
				
				nIndex ++;
			}
			return NULL;
			
		case TYPE_DATA_UNIT:
			C2_Unit	*pUnit;

			nPos	= m_ListUnit.GetHeadPosition();
			nIndex	= 0;
			while (nPos)
			{
				pUnit = m_ListUnit.GetNext(nPos);
				if (pUnit->m_Uid == nUid)
					return pUnit;
				
				nIndex ++;
			}
			return NULL;

		case TYPE_DATA_HOME:
			C2_Home *pHome;

			nPos	= m_ListHome.GetHeadPosition();
			nIndex	= 0;
			while (nPos)
			{
				pHome = m_ListHome.GetNext(nPos);
				if (pHome->m_Uid == nUid)
					return pHome;

				nIndex ++;
			}
			return NULL;

		default:
			return NULL;
	}
}

//-----------------------------------------------------------------------------
//	Number of data
int CC_Data::Count(BYTE nType)
{
	switch (nType)
	{
		case TYPE_DATA_MARK: return (int)m_ListMark.GetCount();
		case TYPE_DATA_RADA: return (int)m_ListRada.GetCount();
		case TYPE_DATA_BATE: return (int)m_ListBate.GetCount();
		case TYPE_DATA_UNIT: return (int)m_ListUnit.GetCount();
		case TYPE_DATA_HOME: return (int)m_ListHome.GetCount();
		default:			 return 0;
	}
}

bool CC_Data::IsEmpty(BYTE nType)
{
	switch (nType)
	{
		case TYPE_DATA_MARK: return (m_ListMark.IsEmpty() != 0);
		case TYPE_DATA_RADA: return (m_ListRada.IsEmpty() != 0);
		case TYPE_DATA_BATE: return (m_ListBate.IsEmpty() != 0);
		case TYPE_DATA_UNIT: return (m_ListUnit.IsEmpty() != 0);
		case TYPE_DATA_HOME: return (m_ListHome.IsEmpty() != 0);
		default:			 return true;
	}
}

//-----------------------------------------------------------------------------
//  Add / Get some
//-----------------------------------------------------------------------------
void CC_Data::AddHead(BYTE nType, void *pData)
{
    switch (nType)
	{
		case TYPE_DATA_MARK: m_ListMark.AddHead((C2_Mark *)pData); break;
		case TYPE_DATA_RADA: m_ListRada.AddHead((C2_Rada *)pData); break;
		case TYPE_DATA_BATE: m_ListBate.AddHead((C2_Bate *)pData); break;
		case TYPE_DATA_UNIT: m_ListUnit.AddHead((C2_Unit *)pData); break;
		case TYPE_DATA_HOME: m_ListHome.AddHead((C2_Home *)pData); break;
		default: break;
	}
}

void CC_Data::AddTail(BYTE nType, void *pData)
{
    switch (nType)
	{
		case TYPE_DATA_MARK: m_ListMark.AddTail((C2_Mark *)pData); break;
		case TYPE_DATA_RADA: m_ListRada.AddTail((C2_Rada *)pData); break;
		case TYPE_DATA_BATE: m_ListBate.AddTail((C2_Bate *)pData); break;
		case TYPE_DATA_UNIT: m_ListUnit.AddTail((C2_Unit *)pData); break;
		case TYPE_DATA_HOME: m_ListHome.AddTail((C2_Home *)pData); break;
		default:			 break;
	}
}

void * CC_Data::GetHead(BYTE nType)
{
    switch (nType)
	{
		case TYPE_DATA_MARK: return (void *)m_ListMark.GetHead();
		case TYPE_DATA_RADA: return (void *)m_ListRada.GetHead();
		case TYPE_DATA_BATE: return (void *)m_ListBate.GetHead();
		case TYPE_DATA_UNIT: return (void *)m_ListUnit.GetHead();
		case TYPE_DATA_HOME: return (void *)m_ListHome.GetHead();
		default:			 return NULL;
	}
}

void * CC_Data::GetTail(BYTE nType)
{
    switch (nType)
	{
		case TYPE_DATA_MARK: return (void *)m_ListMark.GetTail();
		case TYPE_DATA_RADA: return (void *)m_ListRada.GetTail();
		case TYPE_DATA_BATE: return (void *)m_ListBate.GetTail();
		case TYPE_DATA_UNIT: return (void *)m_ListUnit.GetTail();
		case TYPE_DATA_HOME: return (void *)m_ListHome.GetTail();
		default:			 return NULL;
	}
}

void CC_Data::RemvHead(BYTE nType)
{
    switch (nType)
	{
		case TYPE_DATA_MARK: if (!m_ListMark.IsEmpty()) m_ListMark.RemoveHead(); break;
		case TYPE_DATA_RADA: if (!m_ListRada.IsEmpty()) m_ListRada.RemoveHead(); break;
		case TYPE_DATA_BATE: if (!m_ListBate.IsEmpty()) m_ListBate.RemoveHead(); break;
		case TYPE_DATA_UNIT: if (!m_ListUnit.IsEmpty()) m_ListUnit.RemoveHead(); break;
		case TYPE_DATA_HOME: if (!m_ListHome.IsEmpty()) m_ListHome.RemoveHead(); break;		
	}
}

void CC_Data::RemvTail(BYTE nType)
{
	switch (nType)
	{
		case TYPE_DATA_MARK: if (!m_ListMark.IsEmpty()) m_ListMark.RemoveTail(); break;
		case TYPE_DATA_RADA: if (!m_ListRada.IsEmpty()) m_ListRada.RemoveTail(); break;
		case TYPE_DATA_BATE: if (!m_ListBate.IsEmpty()) m_ListBate.RemoveTail(); break;
		case TYPE_DATA_UNIT: if (!m_ListUnit.IsEmpty()) m_ListUnit.RemoveTail(); break;
		case TYPE_DATA_HOME: if (!m_ListHome.IsEmpty()) m_ListHome.RemoveTail(); break;		
	}
}

//-----------------------------------------------------------------------------
//	Get a data: by Type, index
//-----------------------------------------------------------------------------
POSITION CC_Data::GetPosH(BYTE nType)
{
	switch (nType)
	{
		case TYPE_DATA_MARK: return m_ListMark.GetHeadPosition();
		case TYPE_DATA_RADA: return m_ListRada.GetHeadPosition();
		case TYPE_DATA_BATE: return m_ListBate.GetHeadPosition();
		case TYPE_DATA_UNIT: return m_ListUnit.GetHeadPosition();
		case TYPE_DATA_HOME: return m_ListHome.GetHeadPosition();
		default:			 return NULL;
	}
}	

void * CC_Data::GetNext(BYTE nType, POSITION &nPos)
{
	switch (nType)
	{
		case TYPE_DATA_MARK: return m_ListMark.GetNext(nPos);
		case TYPE_DATA_RADA: return m_ListRada.GetNext(nPos);
		case TYPE_DATA_BATE: return m_ListBate.GetNext(nPos);
		case TYPE_DATA_UNIT: return m_ListUnit.GetNext(nPos);
		case TYPE_DATA_HOME: return m_ListHome.GetNext(nPos);
		default:			 return NULL;
	}
}

POSITION CC_Data::FindPos(BYTE nType, int nIndex)
{
	switch (nType)
	{
		case TYPE_DATA_MARK: return m_ListMark.FindIndex(nIndex);
		case TYPE_DATA_RADA: return m_ListRada.FindIndex(nIndex);
		case TYPE_DATA_BATE: return m_ListBate.FindIndex(nIndex);
		case TYPE_DATA_UNIT: return m_ListUnit.FindIndex(nIndex);
		case TYPE_DATA_HOME: return m_ListHome.FindIndex(nIndex);
		default:			 return NULL;
	}
}

bool CC_Data::RemvPos(BYTE nType, POSITION nPos)
{
	if (!nPos)
		return false;

	switch (nType)
	{
		case TYPE_DATA_MARK: m_ListMark.RemoveAt(nPos); return true;
		case TYPE_DATA_RADA: m_ListRada.RemoveAt(nPos); return true;
		case TYPE_DATA_BATE: m_ListBate.RemoveAt(nPos); return true;
		case TYPE_DATA_UNIT: m_ListUnit.RemoveAt(nPos); return true;
		case TYPE_DATA_HOME: m_ListHome.RemoveAt(nPos); return true;
		default:			 return false;
	}	
}

void * CC_Data::GetAt(BYTE nType, int nIndex)
{
	POSITION nPos;

    switch (nType)
	{
		case TYPE_DATA_MARK : 
			nPos = m_ListMark.FindIndex(nIndex);
			return (nPos == NULL)? NULL : (void *)m_ListMark.GetAt(nPos);

		case TYPE_DATA_RADA : 
			nPos = m_ListRada.FindIndex(nIndex);
			return (nPos == NULL)? NULL : (void *)m_ListRada.GetAt(nPos);
			
		case TYPE_DATA_BATE : 
			nPos = m_ListBate.FindIndex(nIndex);
			return (nPos == NULL)? NULL : (void *)m_ListBate.GetAt(nPos);
			
		case TYPE_DATA_UNIT : 
			nPos = m_ListUnit.FindIndex(nIndex);
			return (nPos == NULL)? NULL : (void *)m_ListUnit.GetAt(nPos);

		case TYPE_DATA_HOME: 
			nPos = m_ListHome.FindIndex(nIndex);
			return (nPos == NULL)? NULL : (void *)m_ListHome.GetAt(nPos);

		default:
			return NULL;
	}
}

bool CC_Data::RemvAt(BYTE nType, int nIndex)
{
	POSITION	nPos;

    switch (nType)
	{
		case TYPE_DATA_MARK: 
			nPos = m_ListMark.FindIndex(nIndex);
			if (!nPos)
				return false;
			
			m_ListMark.RemoveAt(nPos);				
			return true;
			
		case TYPE_DATA_RADA: 
			nPos = m_ListRada.FindIndex(nIndex);
			if (!nPos)
				return false;
			
			m_ListRada.RemoveAt(nPos);
			return true;

		case TYPE_DATA_BATE: 
			nPos = m_ListBate.FindIndex(nIndex);
			if (!nPos)
				return false;
			
			m_ListBate.RemoveAt(nPos);
			return true;
			
		case TYPE_DATA_UNIT: 
			nPos = m_ListUnit.FindIndex(nIndex);
			if (!nPos)
				return false;
			
			m_ListUnit.RemoveAt(nPos);
			return true;
			
		case TYPE_DATA_HOME: 
			nPos = m_ListHome.FindIndex(nIndex);
			if (!nPos)			
				return false;

			m_ListHome.RemoveAt(nPos);
			return true;

		default:			  
			return false;
	}
}

//-----------------------------------------------------------------------------
//	Input: Type, Command (1.2.3), Iden, pData
//	Return result: (0:None, Comm), Index,
//-----------------------------------------------------------------------------
BYTE CC_Data::Command(BYTE nType, BYTE nComm, void *pData)
{
    int    nIndex;
    return  Command(nType, nComm, pData, nIndex);
}

// Overload
BYTE CC_Data::Command(BYTE nType, BYTE nComm, void *pData, int &nIndex)
{
	// 1: MARK point
	if (nType == TYPE_DATA_MARK)
	{
        C2_Mark *pNewM = (C2_Mark *)pData;
		C2_Mark	*pMark;

		if		(nComm == COMM_NEW)		// New data
		{
			nIndex = (int)m_ListMark.GetCount();
            if (nIndex >= MAX_DATA_MARK)
                return COMM_NON;		// Too much

			pMark = new C2_Mark();
            if (!pMark)
                return COMM_NON;		// Error memory

            pNewM->m_Uid = GetUid(nType);
            pMark->Copy(pNewM);
			m_ListMark.AddTail(pMark);
    		return nComm;
		}
		else if (nComm == COMM_UPD)		// Update data
		{
			pMark = (C2_Mark *)Find(nType, pNewM->m_Uid, nIndex);
			if (!pMark)
				return COMM_NON;		// Not find ID

            pMark->Copy(pNewM);
			return nComm;
		}
		else if (nComm == COMM_DEL)		// Delete data
		{
			pMark = (C2_Mark *)Find(nType, pNewM->m_Uid, nIndex);
			if (!pMark)
				return COMM_NON;		// Not find ID

			delete pMark;
			RemvAt(nType, nIndex);
			return nComm;
		}
		else
			return COMM_NON;			// Error Command
	}
	
	// 2: RADAR site
	else	 
	if (nType == TYPE_DATA_RADA)
	{
		C2_Rada *pNewR = (C2_Rada *)pData;
		C2_Rada *pRada;

		if		(nComm == COMM_NEW)		// New data
		{
			nIndex = (int)m_ListRada.GetCount();
            if (nIndex >= MAX_DATA_RADA)
                return COMM_NON;		// Too much

			pRada = new C2_Rada();
            if (!pRada)
                return COMM_NON;		// Error memory

            pNewR->m_Uid = GetUid(nType);
            pRada->Copy(pNewR);
			m_ListRada.AddTail(pRada);
    		return nComm;
		}
		else if (nComm == COMM_UPD)		// Update data
		{
			pRada = (C2_Rada *)Find(nType, pNewR->m_Uid, nIndex);
			if (!pRada)
				return COMM_NON;		// Not find ID

            pRada->Copy(pNewR);
            return nComm;
		}
		else if (nComm == COMM_DEL)		// Delete data
		{
			pRada = (C2_Rada *)Find(nType, pNewR->m_Uid, nIndex);
			if (!pRada)
				return COMM_NON;		// Not find ID

			delete pRada;
            RemvAt(nType, nIndex);
			return nComm;
		}
		else
			return COMM_NON;			// Error Command
	}

	// 3: BATE site
	else 
	if (nType == TYPE_DATA_BATE)
	{
		C2_Bate *pNewB = (C2_Bate *)pData;
		C2_Bate *pBate;
		
		if		(nComm == COMM_NEW)     // New data
		{
			nIndex = (int)m_ListBate.GetCount();
			if (nIndex >= MAX_DATA_BATE)
				return COMM_NON;		// Too much
			
			pBate = new C2_Bate();
			if (!pBate)
				return COMM_NON;		// Error memory
			
			pNewB->m_Uid = GetUid(nType);
			pBate->Copy(pNewB);
			m_ListBate.AddTail(pBate);
			return nComm;
		}
		else if (nComm == COMM_UPD)		// Update data
		{
			pBate = (C2_Bate *)Find(nType, pNewB->m_Uid, nIndex);
			if (!pBate)
				return COMM_NON;		// Not find ID
			
			pBate->Copy(pNewB);
			return nComm;
		}
		else if (nComm == COMM_DEL)		// Delete data
		{
			pBate = (C2_Bate *)Find(nType, pNewB->m_Uid, nIndex);
			if (!pBate)
				return COMM_NON;		// Not find ID
			
			delete pBate;
			RemvAt(nType, nIndex);	
			return nComm;
		}
		else
			return COMM_NON;			// Error Command
	}
	
	// 4: UNIT site
	else 
	if (nType == TYPE_DATA_UNIT)
	{
		C2_Unit *pNewU = (C2_Unit *)pData;
		C2_Unit *pUnit;
		
		if		(nComm == COMM_NEW)     // New data
		{
			nIndex = (int)m_ListUnit.GetCount();
			if (nIndex >= MAX_DATA_UNIT)
				return COMM_NON;		// Too much
			
			pUnit = new C2_Unit();
			if (!pUnit)
				return COMM_NON;		// Error memory
			
			pNewU->m_Uid = GetUid(nType);
			pUnit->Copy(pNewU);
			m_ListUnit.AddTail(pUnit);
			return nComm;
		}
		else if (nComm == COMM_UPD)		// Update data
		{
			pUnit = (C2_Unit *)Find(nType, pNewU->m_Uid, nIndex);
			if (!pUnit)
				return COMM_NON;		// Not find ID
			
			pUnit->Copy(pNewU);
			return nComm;
		}
		else if (nComm == COMM_DEL)		// Delete data
		{
			pUnit = (C2_Unit *)Find(nType, pNewU->m_Uid, nIndex);
			if (!pUnit)
				return COMM_NON;		// Not find ID
			
			delete pUnit;
			RemvAt(nType, nIndex);	
			return nComm;
		}
		else
			return COMM_NON;			// Error Command
	}
	
	// 5: HOME site (building/platform in sea)
	else 	
	if (nType == TYPE_DATA_HOME)
	{
		C2_Home *pNewA = (C2_Home *)pData;
		C2_Home *pHome;

		if		(nComm == COMM_NEW)		// New data
		{
			nIndex = (int)m_ListHome.GetCount();
            if (nIndex >= MAX_DATA_HOME)
                return COMM_NON;		// Too much

			pHome = new C2_Home();
            if (!pHome)
                return COMM_NON;		// Error memory

            pNewA->m_Uid = GetUid(nType);
            pHome->Copy(pNewA);
			m_ListHome.AddTail(pHome);
    		return nComm;
		}
		else if (nComm == COMM_UPD)		// Update data
		{
			pHome = (C2_Home *)Find(nType, pNewA->m_Uid, nIndex);
			if (!pHome)
				return COMM_NON;		// Not find ID

            pHome->Copy(pNewA);
			return nComm;
		}
		else if (nComm == COMM_DEL)		// Delete data
		{
			pHome = (C2_Home *)Find(nType, pNewA->m_Uid, nIndex);
			if (!pHome)
				return COMM_NON;		// Not find ID

			delete pHome;
            RemvAt(nType, nIndex);
			return nComm;
		}
		else
			return COMM_NON;			// Error Command
    }
    else
		return COMM_NON;	// Error
}

//-----------------------------------------------------------------------------
//	Input: Radar, Command(1.2.3), Fspec
//	Return:Result(0,1.2), Index, 
//-----------------------------------------------------------------------------
BYTE CC_Data::Update(BYTE nType, BYTE nComm, void *pData)
{
	int		nIndex;
	return	Update(nType, nComm, pData, nIndex);
}

BYTE CC_Data::Update(BYTE nType, BYTE nComm, void *pData, int &nIndex)
{
	// 1: MARK point
	if (nType == TYPE_DATA_MARK)
	{
		C2_Mark	*pMark, *pNewM = (C2_Mark *)pData;
		
		if ((nComm == COMM_NEW) || (nComm == COMM_UPD))
		{
			pMark = (C2_Mark *)Find(nType, pNewM->m_Uid, nIndex);
			if (!pMark)					// New
			{
				nIndex = (int)m_ListMark.GetCount();
				if (nIndex >= MAX_DATA_MARK)
					return COMM_NON;	// Too much 
				
				pMark = new C2_Mark();
				if (!pMark)
					return COMM_NON;
				
				pNewM->m_Uid = GetUid(nType);
				pMark->Copy(pNewM);				
				m_ListMark.AddTail(pMark);
				return COMM_NEW;		// Ack
			}
			else						// Update
			{	
				pMark->Copy(pNewM);
				return COMM_UPD;
			}
		}
		else if (nComm == COMM_DEL)		// Delete
		{
			pMark = (C2_Mark *)Find(nType, pNewM->m_Uid, nIndex);
			if (!pMark)
				return COMM_NON;		// Not find ID
			else
			{
				delete pMark;
				RemvAt(nType, nIndex);	
				return COMM_DEL;
			}
		}
		else
			return COMM_NON;			// Error Command
	}
		
	// 2: RADA site
	else
	if (nType == TYPE_DATA_RADA)
	{
		C2_Rada *pRada, *pNewR = (C2_Rada *)pData;
	
		if ((nComm == COMM_NEW) || (nComm == COMM_UPD))
		{
			pRada = (C2_Rada *)Find(nType, pNewR->m_Uid, nIndex);
			if (!pRada)					// New
			{
				nIndex = (int)m_ListRada.GetCount();
				if (nIndex >= MAX_DATA_RADA)
					return COMM_NON;	// Too much 
				
				pRada = new C2_Rada();
				if (!pRada)
					return COMM_NON;
				
				pNewR->m_Uid = GetUid(nType);
				pRada->Copy(pNewR);				
				m_ListRada.AddTail(pRada);
				return COMM_NEW;		// Ack
			}
			else						// Update
			{	
				pRada->Copy(pNewR);
				return COMM_UPD;
			}
		}
		else if (nComm == COMM_DEL)		// Delete
		{
			pRada = (C2_Rada *)Find(nType, pNewR->m_Uid, nIndex);
			if (!pRada)
				return COMM_NON;		// Not find ID
			else
			{
				delete pRada;
				RemvAt(nType, nIndex);	
				return COMM_DEL;
			}
		}
		else
			return COMM_NON;			// Error Command
	}

	// 3: BATE site
	else
	if (nType == TYPE_DATA_BATE)
	{
		C2_Bate *pBate, *pNewB = (C2_Bate *)pData;
		
		if ((nComm == COMM_NEW) || (nComm == COMM_UPD))
		{
			pBate = (C2_Bate *)Find(nType, pNewB->m_Uid, nIndex);
			if (!pBate)					// New
			{
				nIndex = (int)m_ListBate.GetCount();
				if (nIndex >= MAX_DATA_BATE)
					return COMM_NON;	// Too much 
				
				pBate = new C2_Bate();
				if (!pBate)
					return COMM_NON;
				
				pNewB->m_Uid = GetUid(nType);
				pBate->Copy(pNewB);				
				m_ListBate.AddTail(pBate);
				return COMM_NEW;		// Ack
			}
			else						// Update
			{	
				pBate->Copy(pNewB);
				return COMM_UPD;
			}
		}
		else if (nComm == COMM_DEL)		// Delete
		{
			pBate = (C2_Bate *)Find(nType, pNewB->m_Uid, nIndex);
			if (!pBate)
				return COMM_NON;		// Not find ID
			else
			{
				delete pBate;
				RemvAt(nType, nIndex);	
				return COMM_DEL;
			}
		}
		else
			return COMM_NON;			// Error Command
	}
		
	// 4: UNIT site
	else	
	if	(nType == TYPE_DATA_UNIT)
	{
		C2_Unit *pUnit, *pNewU = (C2_Unit *)pData;
		
		if ((nComm == COMM_NEW) || (nComm == COMM_UPD))
		{
			pUnit = (C2_Unit *)Find(nType, pNewU->m_Uid, nIndex);
			if (!pUnit)					// New
			{
				nIndex = (int)m_ListUnit.GetCount();
				if (nIndex >= MAX_DATA_UNIT)
					return COMM_NON;	// Too much 
				
				pUnit = new C2_Unit();
				if (!pUnit)
					return COMM_NON;
				
				pNewU->m_Uid = GetUid(nType);
				pUnit->Copy(pNewU);				
				m_ListUnit.AddTail(pUnit);
				return COMM_NEW;		// Ack
			}
			else						// Update
			{	
				pUnit->Copy(pNewU);
				return COMM_UPD;
			}
		}
		else if (nComm == COMM_DEL)		// Delete
		{
			pUnit = (C2_Unit *)Find(nType, pNewU->m_Uid, nIndex);
			if (!pUnit)
				return COMM_NON;		// Not find ID
			else
			{
				delete pUnit;
				RemvAt(nType, nIndex);	
				return COMM_DEL;
			}
		}
		else
			return COMM_NON;			// Error Command
	}

	// 5: HOME site
	else 		
	if (nType == TYPE_DATA_HOME)
	{
		C2_Home *pHome, *pNewA = (C2_Home *)pData;
	
		if ((nComm == COMM_NEW) || (nComm == COMM_UPD))
		{
			pHome = (C2_Home *)Find(nType, pNewA->m_Uid, nIndex);
			if (!pHome)					// New
			{
				nIndex = (int)m_ListHome.GetCount();
				if (nIndex >= MAX_DATA_HOME)
					return COMM_NON;	// Too much 
				
				pHome = new C2_Home();
				if (!pHome)
					return COMM_NON;
				
				pNewA->m_Uid = GetUid(nType);
				pHome->Copy(pNewA);				
				m_ListHome.AddTail(pHome);
				return COMM_NEW;		// Ack
			}
			else						// Update
			{	
				pHome->Copy(pNewA);
				return COMM_UPD;
			}
		}
		else if (nComm == COMM_DEL)		// Delete
		{
			pHome = (C2_Home *)Find(nType, pNewA->m_Uid, nIndex);
			if (!pHome)
				return COMM_NON;		// Not find ID
			else
			{
				delete pHome;
				RemvAt(nType, nIndex);	
				return COMM_DEL;
			}
		}
		else
			return COMM_NON;			// Error Command
	}
	else
		return COMM_NON;
}


//-----------------------------------------------------------------------------
//	Graph: Rect, Circle, Sector, Polygon, Ellipe 
//-----------------------------------------------------------------------------
BYTE CC_Data::LoadFile()
{
    BYTE  nErro = 0;
	
    if (!LoadFile(TYPE_DATA_MARK)) nErro |= 0x01;
    if (!LoadFile(TYPE_DATA_RADA)) nErro |= 0x02;
    if (!LoadFile(TYPE_DATA_BATE)) nErro |= 0x04;
    if (!LoadFile(TYPE_DATA_UNIT)) nErro |= 0x08;
    if (!LoadFile(TYPE_DATA_HOME)) nErro |= 0x10;
	
    return nErro;
}

bool CC_Data::LoadFile(BYTE nType)
{
	CString	szFile;
	
    switch (nType)
    {
		case TYPE_DATA_MARK : 	
			szFile = m_szRoot + _T("\\") + _T("DataMark.bin");
			return LoadMark(szFile);
		
		case TYPE_DATA_RADA: 	
			szFile = m_szRoot + _T("\\") + _T("DataRada.bin");
			return LoadRada(szFile);
			
		case TYPE_DATA_BATE: 	
			szFile = m_szRoot + _T("\\") + _T("DataBate.bin");
			return LoadBate(szFile);
			
		case TYPE_DATA_UNIT:	
			szFile = m_szRoot + _T("\\") + _T("DataUnit.bin");
			return LoadUnit(szFile);
			
		case TYPE_DATA_HOME: 	
			szFile = m_szRoot + _T("\\") + _T("DataHome.bin");
			return LoadHome(szFile);
			
		default:              	
			return false;
    }
}

//-----------------------------------------------------------------------------
//  Save all of data to files
//-----------------------------------------------------------------------------
BYTE CC_Data::SaveFile()
{
    BYTE nErro = 0;
	
    if (!SaveFile(TYPE_DATA_MARK)) nErro |= 0x01;
    if (!SaveFile(TYPE_DATA_RADA)) nErro |= 0x02;
    if (!SaveFile(TYPE_DATA_BATE)) nErro |= 0x04;
    if (!SaveFile(TYPE_DATA_UNIT)) nErro |= 0x08;
    if (!SaveFile(TYPE_DATA_HOME)) nErro |= 0x10;
	
    return nErro;
}

bool CC_Data::SaveFile(BYTE nType)
{
    CString	szFile;
	
    switch (nType)
    {
		case TYPE_DATA_MARK : 	
			szFile = m_szRoot + _T("\\") + _T("DataMark.bin");
			return SaveMark(szFile);
			
		case TYPE_DATA_RADA: 	
			szFile = m_szRoot + _T("\\") + _T("DataRada.bin");
			return SaveRada(szFile);
			
		case TYPE_DATA_BATE: 	
			szFile = m_szRoot + _T("\\") + _T("DataBate.bin");
			return SaveBate(szFile);
			
		case TYPE_DATA_UNIT :	
			szFile = m_szRoot + _T("\\") + _T("DataUnit.bin");
			return SaveUnit(szFile);
			
		case TYPE_DATA_HOME: 	
			szFile = m_szRoot + _T("\\") + _T("DataHome.bin");
			return SaveHome(szFile);
			
		default:
			return false;
    }
}

//-----------------------------------------------------------------------------
//  Check header of file: 16 byte
//-----------------------------------------------------------------------------
bool CC_Data::LoadHead(BYTE nType, BYTE *szBuff)
{
    bool	bOpen;	    // Open file
    int     nPos, i;
	
    // Check header (8 byte = 0xFF)
    bOpen = true;
	nPos  = 0;
    i     = 0;
	while (bOpen && (i < 8))
	{
		if (szBuff[nPos+i] != 0xFF)
			bOpen = false;  			// Error of Header
        else
            i ++;
    }
    nPos += 8;
	
    if (!bOpen)
        return false;
	
    // Check CAT-TYPE-LEN (4 byte)
	if ((szBuff[nPos  ] != 0x03 ) ||    // File DATA = 3
        (szBuff[nPos+1] != nType))
        bOpen = false;					// Error of CAT
    nPos += 2;
	
    nPos += 2;							// LEN of file
	
    // Check end of header
    i = 0;
	while (bOpen && (i < 4))
	{
		if (szBuff[nPos+i] != 0)
			bOpen = false;				// Error of End header
		else
			i ++;
	}
	nPos += 4;
	
    return bOpen;
}

//-----------------------------------------------------------------------------
//  Save header of file
//-----------------------------------------------------------------------------
bool CC_Data::SaveHead(BYTE nType, BYTE *szBuff)
{
    int		nPos = 0;
	
    memset(&szBuff[nPos], 0xFF, 8);
    nPos += 8;
	
	szBuff[nPos++] = 0x03 ;			// CAT = 3 file DATA
    szBuff[nPos++] = nType;			// Type of Data
    nPos += 2;						// Leng of file
	
	memset(&szBuff[nPos], 0x00, 4);
    nPos += 4;
	
	return true;
}


//-----------------------------------------------------------------------------
//	Load file MARK
//-----------------------------------------------------------------------------
bool CC_Data::LoadMark(CString szFile)
{
	CFile	hFile ;
	if (!hFile.Open(szFile, CFile::modeRead))	// No file / Not load
		return false;
	
    BYTE	szBuff[MAX_BUFF_READ];
	int		nRead;                  // Read byte
	
    // Get block for check header of file
	nRead = hFile.Read(szBuff, 16);
    if (nRead < 16)     // = header len
    {
        hFile.Close();
        return false;
    }
	
    if (!LoadHead(TYPE_DATA_MARK, szBuff))
    {
        hFile.Close();
        return false;
    }
	
    // Get Mark list from file
    C2_Mark	*	pMark;			//
	BYTE		szName[21];		// Name of Mark
    BYTE    	nLen;           // Record length
    int     	nPos, nUid;
	bool		bEnd;		    // End of file
	
	bEnd = false;
    nLen = 34;
    nUid = 0;
 	while (!bEnd)
 	{
 		nRead = hFile.Read(szBuff, MAX_BUFF_READ);
 		if (nRead < MAX_BUFF_READ)
 			bEnd = true;

 		nPos = 0;
 		while (nPos <= (nRead - nLen))
 		{
            // Begin of Record
			if ((szBuff[nPos  ] != 0xFF) || (szBuff[nPos+1] != 0xFF) || (szBuff[nPos+2] != 0xFF))
            {
                nPos ++;
                continue;
            }

            pMark = new C2_Mark();
            if (!pMark)
			{
				bEnd = true;
                break;
			}

            // Head - ID    - 3
            nPos += 3;
            pMark->m_Uid	= (++nUid);

 		    // 1: Lat/Long	- 6
 		    pMark->m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
 		    pMark->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
 		    nPos += 6;

			// 2: Type/Colo	- 2
            pMark->m_Type   = szBuff[nPos++];
            pMark->m_Color  = szBuff[nPos++];

 		    // 3: Name mark	- 20
 		    memcpy(szName, &szBuff[nPos], 20);
 		    szName[20] = 0x00;
 		    pMark->m_szName = szName;
 		    nPos += 20;

 		    // 4: Backup	- 3
 		    nPos += 3;
 		    m_ListMark.AddTail(pMark);
        }
 	}

    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//	Load file RADA
//-----------------------------------------------------------------------------
bool CC_Data::LoadRada(CString szFile)
{
	CFile	hFile ;
	if (!hFile.Open(szFile, CFile::modeRead))	// No file / Not load
		return false;
	
    BYTE	szBuff[MAX_BUFF_READ];
	int		nRead;                  // Read byte
	
    // Get block for check header of file
	nRead = hFile.Read(szBuff, 16);
    if (nRead < 16)     // = header len
    {
        hFile.Close();
        return false;
    }
	
	if (!LoadHead(TYPE_DATA_RADA, szBuff))
    {
        hFile.Close();
        return false;
    }

    // Get Radar list from file
	C2_Rada	*	pRada;			// Pointer for new C2_Rada
	char		szName[21];		// Name of Rada
    int			nPos, nUid;
	bool		bEnd;		    // End of file
    BYTE		nLen;           // Record length

	bEnd = false;
    nLen = 38;
    nUid = 0;
	while (!bEnd)
	{
		nRead = hFile.Read(szBuff, MAX_BUFF_READ);
		if (nRead < MAX_BUFF_READ)
			bEnd = true;

		nPos = 0;
		while (nPos <= (nRead - nLen))
		{
            // Begin of Record
			if ((szBuff[nPos  ] != 0xFF) || (szBuff[nPos+1] != 0xFF) || (szBuff[nPos+2] != 0xFF))
            {
                nPos ++;
                continue;
            }

            pRada = new C2_Rada();
            if (!pRada)
			{
				bEnd = true;
                break;
			}

            // Head - ID    - 3
            nPos += 3;
            pRada->m_Uid	= (++nUid);

			// 1: Lat/Long	- 6
			pRada->m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pRada->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;

			// 2: Type/Stat	- 1
			pRada->m_Type	= szBuff[nPos] & 0x0F;
			pRada->m_Status =(szBuff[nPos] >> 4);
			nPos += 1;

            // 3: Rge	    - 6			
			pRada->m_RgMax	= (szBuff[nPos  ] << 8) | szBuff[nPos+1];
		    pRada->m_AzBeg	= (szBuff[nPos+2] << 8) | szBuff[nPos+3];
	        pRada->m_AzEnd	= (szBuff[nPos+4] << 8) | szBuff[nPos+5];
	        nPos += 6;

			// 4: Name      - 20 byte
			memcpy(szName, &szBuff[nPos], 20);
			szName[20]      = 0x00;
			pRada->m_szName = szName;
			nPos += 20;

			// 5: Backup	- 2
			nPos += 2;
			m_ListRada.AddTail(pRada);
        }
	}

    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//	Load file BATE
//-----------------------------------------------------------------------------
bool CC_Data::LoadBate(CString szFile)
{
	CFile	hFile ;
	if (!hFile.Open(szFile, CFile::modeRead))	// No file / Not load
		return false;
	
    BYTE	szBuff[MAX_BUFF_READ];
	int		nRead;                  // Read byte
	
    // Get block for check header of file
	nRead = hFile.Read(szBuff, 16);
    if (nRead < 16)     // = header len
    {
        hFile.Close();
        return false;
    }
		
    if (!LoadHead(TYPE_DATA_BATE, szBuff))
    {
        hFile.Close();
        return false;
    }
	
    // Get BaTe list from file
	C2_Bate	*	pBate;			//
	char		szName[21];		// Name of BaTe
    int			nPos, nUid;
    BYTE		nLen;           // Record length: min=40, max=46/47
	bool		bEnd;		    // End of file
	
	bEnd = false;
    nLen = 40;
    nUid = 0;
	while (!bEnd)
	{
		nRead = hFile.Read(szBuff, MAX_BUFF_READ);
		if (nRead < MAX_BUFF_READ)
			bEnd = true;
		
		nPos = 0;
		while (nPos <= (nRead - nLen))
		{
            // Begin of Record
			if ((szBuff[nPos  ] != 0xFF) || (szBuff[nPos+1] != 0xFF) || (szBuff[nPos+2] != 0xFF))
            {
                nPos ++;
                continue;
            }
			
            pBate = new C2_Bate();
            if (!pBate)
			{
				bEnd = true;
                break;
			}
			
            // Head-ID      - 3
            nPos += 3;
			pBate->m_Uid	= (++nUid);
			
			// 1: Lat/Long	- 6
			pBate->m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pBate->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;
			
            // 2: Type/Stat	- 2
            pBate->m_Type   = szBuff[nPos  ] & 0x0F;
			pBate->m_Status =(szBuff[nPos  ] >> 4); 
            pBate->m_Color	= szBuff[nPos+1];
			nPos += 2;
			
            // 4: Fire data - 6; 
            pBate->m_RgMax  = (szBuff[nPos  ] << 8) | szBuff[nPos+1];
            pBate->m_AzBeg  = (szBuff[nPos+2] << 8) | szBuff[nPos+3];
            pBate->m_AzEnd  = (szBuff[nPos+4] << 8) | szBuff[nPos+5];
            nPos += 6;
			
            // 6: Name  	- 20
			memcpy(szName, &szBuff[nPos], 20);
			szName[20] = 0x00;
			pBate->m_szName = szName;
			nPos += 20;
			
			// 7: Backup	- 3
			nPos += 3;
			
			m_ListBate.AddTail(pBate);
        }
	}
	
    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//	Load file UNIT
//-----------------------------------------------------------------------------
bool CC_Data::LoadUnit(CString szFile)
{
	CFile	hFile ;
	if (!hFile.Open(szFile, CFile::modeRead))	// No file / Not load
		return false;
	
    BYTE	szBuff[MAX_BUFF_READ];
	int		nRead;                  // Read byte
	
    // Get block for check header of file
	nRead = hFile.Read(szBuff, 16);
    if (nRead < 16)     // = header len
    {
        hFile.Close();
        return false;
    }
	
    if (!LoadHead(TYPE_DATA_UNIT, szBuff))
    {
        hFile.Close();
        return false;
    }
	
    // Get Unit list from file
	C2_Unit	*	pUnit;			//
	BYTE		szName[21];		// Name of Rect
    int     	nPos, nUid;
	bool		bEnd;		    // End of file
    BYTE    	nLen;           // Record length
	
	bEnd = false;
    nLen = 34;
    nUid = 0;
	while (!bEnd)
	{
		nRead = hFile.Read(szBuff, MAX_BUFF_READ);
		if (nRead < MAX_BUFF_READ)
			bEnd = true;
		
		nPos = 0;
		while (nPos <= (nRead - nLen))
		{
            // Begin of Record
			if ((szBuff[nPos  ] != 0xFF) || (szBuff[nPos+1] != 0xFF) || (szBuff[nPos+2] != 0xFF))
            {
                nPos ++;
                continue;
            }
			
            pUnit = new C2_Unit();
            if (!pUnit)
			{
				bEnd = true;
                break;
			}
			
			// Head-ID      - 3
            nPos += 3;
            pUnit->m_Uid	= (++nUid);
			
			// 1: Lat/Long	- 6
			pUnit->m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pUnit->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;
			
			// 2: Type/Colo	- 2
			pUnit->m_Type	= szBuff[nPos++];
			pUnit->m_Color	= szBuff[nPos++];
			
			// 3: Name unit	- 20
			memcpy(szName, &szBuff[nPos], 20);
			szName[20]      = 0x00;
			pUnit->m_szName = szName;
			nPos += 20;
			
			// 4: Backup	- 3
			nPos += 3;
			m_ListUnit.AddTail(pUnit);
        }
	}
	
    hFile.Close();
    return true;
}

//-----------------------------------------------------------------------------
//	Load file Homease
//-----------------------------------------------------------------------------
bool CC_Data::LoadHome(CString szFile)
{
	CFile	hFile ;
	if (!hFile.Open(szFile, CFile::modeRead))	// No file / Not load
		return false;
	
    BYTE	szBuff[MAX_BUFF_READ];
	int		nRead;                  // Read byte
	
    // Get block for check header of file
	nRead = hFile.Read(szBuff, 16);
    if (nRead < 16)     // = header len
    {
        hFile.Close();
        return false;
    }
	   
	if (!LoadHead(TYPE_DATA_HOME, szBuff))
    {
        hFile.Close();
        return false;
    }

    // Get Radar list from file
	C2_Home	*	pHome;			// Pointer for new C2_Home
	char		szName[21];		// Name of Homease
    int			nPos, nUid;
	bool		bEnd;		    // End of file
    BYTE		nLen;           // Record length: min 42, max=56

	bEnd = false;
    nLen = 34;
    nUid = 0;
	while (!bEnd)
	{
		nRead = hFile.Read(szBuff, MAX_BUFF_READ);
		if (nRead < MAX_BUFF_READ)
			bEnd = true;

		nPos = 0;
		while (nPos <= (nRead - nLen))
		{
            // Begin of Record
			if ((szBuff[nPos  ] != 0xFF) || (szBuff[nPos+1] != 0xFF) || (szBuff[nPos+2] != 0xFF))
            {
                nPos ++;
                continue;
            }

            pHome = new C2_Home();
            if (!pHome)
			{
				bEnd = true;
                break;
			}

            // Head - ID   	- 3
            nPos += 3;
            pHome->m_Uid	= (++nUid);

            // 1: Lat/Long	- 6
			pHome->m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pHome->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;

			// 2: Type/Colo	- 2
			pHome->m_Type	= szBuff[nPos++];
			pHome->m_Color	= szBuff[nPos++];

            // 4: Name      - 20
			memcpy(szName, &szBuff[nPos], 20);
			szName[20] 		= 0x00;
			pHome->m_szName = szName;
			nPos += 20;

			// 5: Backup	- 3
			nPos += 3;
			m_ListHome.AddTail(pHome);
        }
	}

    hFile.Close();
    return true;
}

//-----------------------------------------------------------------------------
//  Save file MARK
//-----------------------------------------------------------------------------
bool CC_Data::SaveMark(CString szFile)
{
	CFile		hFile;
	CFileStatus	hStat;		// handle of file status
	
	// Clear the Read Only 
	if (CFile::GetStatus(szFile, hStat))
	{
		hStat.m_attribute = 0x00;
		CFile::SetStatus(szFile, hStat);
	}
	
	if (!hFile.Open(szFile, CFile::modeCreate | CFile::modeWrite))
		return false;	
	
	
    BYTE	szBuff[MAX_BUFF_READ];
	int     nPos = 0;
	
    // Save header of file: 16 byte
    SaveHead(TYPE_DATA_MARK, szBuff);
	nPos += 16;	
	
	// Save Marker list
	C2_Mark *	pMark;
	POSITION	pPos;
	BYTE		szName[21];
    BYTE    	nLen;       	// Record length
	
	pPos = m_ListMark.GetHeadPosition();
    nLen = 34;
    while (pPos)
	{
		pMark = m_ListMark.GetNext(pPos);
        if (nPos > (MAX_BUFF_READ - nLen))  // full buffer
        {
           	hFile.Write(szBuff, nPos);
            nPos = 0;
        }

        // Head - ID    - 3
        memset(&szBuff[nPos], 0xFF, 3);
	    nPos += 3;

        // 1: Lat/Long	- 6
	    szBuff[nPos++] = BYTE (pMark->m_Lat  >>16);
		szBuff[nPos++] = BYTE (pMark->m_Lat  >> 8);
		szBuff[nPos++] = BYTE (pMark->m_Lat      );
		szBuff[nPos++] = BYTE (pMark->m_Long >>16);
		szBuff[nPos++] = BYTE (pMark->m_Long >> 8);
		szBuff[nPos++] = BYTE (pMark->m_Long     );

		// 2: Type/Colo	- 2
		szBuff[nPos++]	= pMark->m_Type ;
		szBuff[nPos++]	= pMark->m_Color;

		// 4: Name		- 20
		::ConvStrBuff(pMark->m_szName, szName, 20);
		memcpy(&szBuff[nPos], szName, 20);
		nPos += 20;

		// 5: Backup	- 3
		memset(&szBuff[nPos], 0x00, 3);
		nPos += 3;
    }

    hFile.Write(szBuff, nPos);
    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//  Save file RADA
//-----------------------------------------------------------------------------
bool CC_Data::SaveRada(CString szFile)
{
	CFile		hFile;
	CFileStatus	hStat;		// handle of file status
	
	// Clear the Read Only 
	if (CFile::GetStatus(szFile, hStat))
	{
		hStat.m_attribute = 0x00;
		CFile::SetStatus(szFile, hStat);
	}
	
	if (!hFile.Open(szFile, CFile::modeCreate | CFile::modeWrite))
		return false;	
	
	
    BYTE	szBuff[MAX_BUFF_READ];
	int     nPos = 0;
	
    // Save header of file: 16 byte
    SaveHead(TYPE_DATA_RADA, szBuff);
	nPos += 16;	
	
	// Save Radar list
	C2_Rada *	pRada;
	POSITION	pPos;
	BYTE		szName[21];
    BYTE    	nLen;       	// Record length
	
	pPos = m_ListRada.GetHeadPosition();
    nLen = 38;
    while (pPos)
	{
		pRada = m_ListRada.GetNext(pPos);
        if (nPos > (MAX_BUFF_READ - nLen))  // full buffer
        {
           	hFile.Write(szBuff, nPos);
            nPos = 0;
        }

        // Head - ID    - 3
        memset(&szBuff[nPos], 0xFF, 3);
	    nPos += 3;

        // 1: Lat/Long	- 6
	    szBuff[nPos++] = BYTE (pRada->m_Lat  >>16);
		szBuff[nPos++] = BYTE (pRada->m_Lat  >> 8);
		szBuff[nPos++] = BYTE (pRada->m_Lat      );
		szBuff[nPos++] = BYTE (pRada->m_Long >>16);
		szBuff[nPos++] = BYTE (pRada->m_Long >> 8);
		szBuff[nPos++] = BYTE (pRada->m_Long     );

		// 2: Type      - 1
		szBuff[nPos++] =(pRada->m_Type & 0x0F) | (pRada->m_Status << 4);

        // 3: Rge/Azi	- 6
		szBuff[nPos++] = BYTE (pRada->m_RgMax >> 8);
		szBuff[nPos++] = BYTE (pRada->m_RgMax     );
		szBuff[nPos++] = BYTE (pRada->m_AzBeg >> 8);
		szBuff[nPos++] = BYTE (pRada->m_AzBeg     );
		szBuff[nPos++] = BYTE (pRada->m_AzEnd >> 8);
		szBuff[nPos++] = BYTE (pRada->m_AzEnd     );

        // 5: Site name - 20
		ConvStrBuff(pRada->m_szName, szName, 20);
		memcpy(&szBuff[nPos], szName, 20);
		nPos += 20;

		// 6: Backup	- 2
		memset(&szBuff[nPos], 0x00, 2);
		nPos += 2;
    }

    hFile.Write(szBuff, nPos);
    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//  Save file BATE
//-----------------------------------------------------------------------------
bool CC_Data::SaveBate(CString szFile)
{
	CFile		hFile;
	CFileStatus	hStat;		// handle of file status
	
	// Clear the Read Only 
	if (CFile::GetStatus(szFile, hStat))
	{
		hStat.m_attribute = 0x00;
		CFile::SetStatus(szFile, hStat);
	}
	
	if (!hFile.Open(szFile, CFile::modeCreate | CFile::modeWrite))
		return false;	
	
	
    BYTE	szBuff[MAX_BUFF_READ];
	int     nPos = 0;
	
    // Save header of file: 16 byte
    SaveHead(TYPE_DATA_BATE, szBuff);
	nPos += 16;	
	
	// Save BaTe list
	C2_Bate *	pBate;
	POSITION	pPos;
	BYTE		szName[21];
    BYTE    	nLen;       	// Record length
	
	pPos = m_ListBate.GetHeadPosition();
    nLen = 40;
    while (pPos)
	{
		pBate 	= m_ListBate.GetNext(pPos);
		if (nPos > (MAX_BUFF_READ - nLen)) // full buffer
        {
           	hFile.Write(szBuff, nPos);
            nPos = 0;
        }

        // Head - ID    - 3
        memset(&szBuff[nPos], 0xFF, 3);
	    nPos += 3;

        // 1: Lat/Long  - 6
        szBuff[nPos++] = BYTE (pBate->m_Lat  >>16);
		szBuff[nPos++] = BYTE (pBate->m_Lat  >> 8);
		szBuff[nPos++] = BYTE (pBate->m_Lat      );
		szBuff[nPos++] = BYTE (pBate->m_Long >>16);
		szBuff[nPos++] = BYTE (pBate->m_Long >> 8);
		szBuff[nPos++] = BYTE (pBate->m_Long     );

        // 2: Type      - 2
        szBuff[nPos++] = (pBate->m_Type & 0x0F) | (pBate->m_Status << 4);
        szBuff[nPos++] = pBate->m_Color;

        // 3: Fire      - 6 
        szBuff[nPos++] = BYTE (pBate->m_RgMax >> 8);
        szBuff[nPos++] = BYTE (pBate->m_RgMax     );
        szBuff[nPos++] = BYTE (pBate->m_AzBeg >> 8);
        szBuff[nPos++] = BYTE (pBate->m_AzBeg     );
        szBuff[nPos++] = BYTE (pBate->m_AzEnd >> 8);
        szBuff[nPos++] = BYTE (pBate->m_AzEnd     );

        // 4: Site name	- 20
		::ConvStrBuff(pBate->m_szName, szName, 20);
		memcpy(&szBuff[nPos], szName, 20);
		nPos += 20;

		// 5: Backup    - 3
		memset(&szBuff[nPos], 0x00, 3);
		nPos += 3;
    }

    hFile.Write(szBuff, nPos);
    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//  Save file UNIT
//-----------------------------------------------------------------------------
bool CC_Data::SaveUnit(CString szFile)
{
	CFile		hFile;
	CFileStatus	hStat;		// handle of file status
	
	// Clear the Read Only 
	if (CFile::GetStatus(szFile, hStat))
	{
		hStat.m_attribute = 0x00;
		CFile::SetStatus(szFile, hStat);
	}
	
	if (!hFile.Open(szFile, CFile::modeCreate | CFile::modeWrite))
		return false;	
	
    BYTE	szBuff[MAX_BUFF_READ];
	int     nPos = 0;
	
    // Save header of file: 16 byte
    SaveHead(TYPE_DATA_UNIT, szBuff);
	nPos += 16;	
		
	// Save Unit list
	C2_Unit *	pUnit;
	POSITION	pPos;
	BYTE		szName[21];
	BYTE		nLen;       	// Record length
	
    pPos = m_ListUnit.GetHeadPosition();
    nLen = 34;
    while (pPos)
	{
		pUnit = m_ListUnit.GetNext(pPos);
		
        if (nPos > (MAX_BUFF_READ - nLen))  // full buffer
        {
           	hFile.Write(szBuff, nPos);
            nPos = 0;
        }
		
        // Head - ID    - 3
        memset(&szBuff[nPos], 0xFF, 3);
		nPos += 3;
		
		// 1: Lat/Long	- 6
		szBuff[nPos++] = BYTE (pUnit->m_Lat  >>16);
		szBuff[nPos++] = BYTE (pUnit->m_Lat  >> 8);
		szBuff[nPos++] = BYTE (pUnit->m_Lat      );
		szBuff[nPos++] = BYTE (pUnit->m_Long >>16);
		szBuff[nPos++] = BYTE (pUnit->m_Long >> 8);
		szBuff[nPos++] = BYTE (pUnit->m_Long     );
		
		// 2: Type/Colo	- 2
		szBuff[nPos++] = pUnit->m_Type ;
		szBuff[nPos++] = pUnit->m_Color;
		
		// 3: Name		- 20
		::ConvStrBuff(pUnit->m_szName, szName, 20);
		memcpy(&szBuff[nPos], szName, 20);
		nPos += 20;
		
		// 4: Backup    - 3
		memset(&szBuff[nPos], 0x00, 3);
		nPos += 3;
    }
	
    hFile.Write(szBuff, nPos);
    hFile.Close();
    return true;
}


//-----------------------------------------------------------------------------
//  Save file HomeASE
//-----------------------------------------------------------------------------
bool CC_Data::SaveHome(CString szFile)
{
	CFile		hFile;
	CFileStatus	hStat;		// handle of file status
	
	// Clear the Read Only 
	if (CFile::GetStatus(szFile, hStat))
	{
		hStat.m_attribute = 0x00;
		CFile::SetStatus(szFile, hStat);
	}
	
	if (!hFile.Open(szFile, CFile::modeCreate | CFile::modeWrite))
		return false;	
	
    BYTE	szBuff[MAX_BUFF_READ];
	int     nPos = 0;
	
    // Save header of file: 16 byte
    SaveHead(TYPE_DATA_HOME, szBuff);
	nPos += 16;	
	
	// Save Home list
	C2_Home *	pHome;
	POSITION	pPos;
	BYTE		szName[21];
	BYTE		nLen;       	// Record length
	
    pPos = m_ListHome.GetHeadPosition();
    nLen = 34;
    while (pPos)
	{
		pHome = m_ListHome.GetNext(pPos);
        if (nPos > (MAX_BUFF_READ - nLen)) // full buffer
        {
           	hFile.Write(szBuff, nPos);
            nPos = 0;
        }

        // Head - ID    - 3
        memset(&szBuff[nPos], 0xFF, 3);
	    nPos += 3;

        // 1: Lat/Long	- 6
	    szBuff[nPos++] = BYTE (pHome->m_Lat  >>16);
	    szBuff[nPos++] = BYTE (pHome->m_Lat  >> 8);
		szBuff[nPos++] = BYTE (pHome->m_Lat      );
		szBuff[nPos++] = BYTE (pHome->m_Long >>16);
		szBuff[nPos++] = BYTE (pHome->m_Long >> 8);
		szBuff[nPos++] = BYTE (pHome->m_Long     );

		// 2: Type/Colo	- 2
		szBuff[nPos++] = pHome->m_Type ;
		szBuff[nPos++] = pHome->m_Color;

        // 4: Site name	- 20
		::ConvStrBuff(pHome->m_szName, szName, 20);
		memcpy(&szBuff[nPos], szName, 20);
		nPos += 20;

		// 5: Backup	- 3
		memset(&szBuff[nPos], 0x00, 3);
		nPos += 3;
    }

    hFile.Write(szBuff, nPos);
    hFile.Close();
    return true;
}

//-----------------------------------------------------------------------------
//	Radar, Unit,
/*-----------------------------------------------------------------------------
bool CC_Data::LoadFile()
{
	if (m_szFile.Compare("") == 0)
		return false;
	
	CFile	hFile ;					// Handle of file
	CString	szPath;
	
	szPath = m_szRoot + "\\" + m_szFile;
	if (!hFile.Open(szPath, CFile::modeRead))	// No file / Not load
		return false;
	
	BYTE	szBuff[256*256];		// Max = 64 kbyte
	int		nPos, nLen, i;			//	
	bool	bOpen = true;			// Open file

	nLen = hFile.Read(szBuff, 256*256);
	hFile.Close();

	// 0: Check header: (8 byte = 0xFF)
	nPos = 0;
	while ((nPos < 8) && bOpen)
	{
		if (szBuff[nPos] != 0xFF)
			bOpen = false;		// Error of Header	
		else
			nPos ++;
	}

	// 0: Check CAT - LEN ( 3byte, 5 byte = 0x00)
	if (bOpen)
	{
		if (szBuff[nPos] != 2)		// CAT = 2: this File Data
			bOpen = false;		// Error of CAT
		else
		if (nLen != ((szBuff[nPos+1] << 8) | szBuff[nPos+2]))
			bOpen = false;	// Error of LEN
		
		nPos += 3;

		i = 0;
		while ((i < 5) && bOpen)
		{
			if (szBuff[nPos+i] != 0x00)
				bOpen = false;	// Error of End header
			else
				i ++;
		}
		nPos += 5;
	}

	if (!bOpen)
		return false; 
		
	char	szTmp[21];				// Name of Object
	int		nNum;

	// Load Radar
	nPos += 4;						// 3 byte: FF + 1 byte: Zero 
	nNum  = szBuff[nPos++];			// 1 byte: Number of Radar 

	if (nNum != 0)
	{
		C2_Rada *pRada;		// 5 + 34 byte

		for (i = 0; i < nNum; i++)
		{
			pRada = new C2_Rada();
			if (!pRada)
				continue;

			// Id
			pRada->m_Uid	= (i+1);
				
			// 1: Lat/Long	- 6
			pRada->m_Lat	= (szBuff[nPos  ] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pRada->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;
				
			// 2: Range/Az	- 6
			pRada->m_Type	= szBuff[nPos++];
			pRada->m_Rge	= (szBuff[nPos  ] << 8) | szBuff[nPos+1];
			pRada->m_AzBeg	= (szBuff[nPos+2] << 8) | szBuff[nPos+3];
			pRada->m_AzEnd	= (szBuff[nPos+4] << 8) | szBuff[nPos+5];
			nPos += 6;

			// 3: Name radar-20 byte
			memcpy(szTmp, &szBuff[nPos], 20);
			szTmp[20] = 0x00;
			pRada->m_szName = szTmp;
			nPos += 20;

			// 4: Backup	- 2
			nPos += 2;
			m_ListRada.AddTail(pRada);
		}
	}

	// Load Unit
	nPos += 4;						// 3 byte: FF + 1 byte: Zero 
	nNum  = szBuff[nPos++];			// 1 byte: Number of Unit 

	if (nNum != 0)
	{
		C2_Unit	*pUnit;				// 5 + 30 byte 
		for (i = 0; i < nNum; i++)
		{
			pUnit = new C2_Unit();
			if (!pUnit)
				continue;

			// ID
			pUnit->m_Uid	= (i+1);
				
			// 1: Lat/Long	- 6
			pUnit->m_Lat	= (szBuff[nPos+0] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pUnit->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;

			// 2: Type/Color- 2
			pUnit->m_Type	= szBuff[nPos++];
			pUnit->m_Color	= szBuff[nPos++];

			// 3: Name unit	- 20
			memcpy(szTmp, &szBuff[nPos], 20);
			szTmp[20] = 0x00;
			pUnit->m_szName = szTmp;
			nPos += 20;

			// 4: Backup	- 2
			nPos += 2;
			m_ListUnit.AddTail(pUnit);
		}
	}

	// Load Mark
	nPos += 4;						// 3 byte: FF + 1 byte: Zero 
	nNum  = szBuff[nPos++];			// 1 byte: Number of Unit 

	if (nNum != 0)
	{
		C2_Mark	*pMark;				// 5 + 30 byte 
		for (i = 0; i < nNum; i++)
		{
			pMark = new C2_Mark();
			if (!pMark)
				continue;

			// ID
			pMark->m_Uid	= (i+1);
				
			// 1: Lat/Long	- 6
			pMark->m_Lat	= (szBuff[nPos+0] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
			pMark->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
			nPos += 6;

			// 2: Name mark	- 20
			memcpy(szTmp, &szBuff[nPos], 20);
			szTmp[20] = 0x00;
			pMark->m_szName = szTmp;
			nPos += 20;

			// 4: Backup	- 2
			nPos += 2;
			m_ListMark.AddTail(pMark);
		}
	}

	return true;
} */


/*-----------------------------------------------------------------------------
bool CC_Data::SaveFile()
{
	if (m_szFile.Compare("") == 0)
		return false;
	
	CFile		hFile;
	CFileStatus	hFileStat;		// handle of file status
	CString		szPath;			//
	
	szPath = m_szRoot + "\\" + m_szFile;
	
	// Clear the Read Only 
	if (CFile::GetStatus(szPath, hFileStat))
	{
		hFileStat.m_attribute = 0x00;
		CFile::SetStatus(szPath, hFileStat);
	}
	
	if (!hFile.Open(szPath, CFile::modeCreate | CFile::modeWrite))
		return false;

	BYTE	szBuff[256*256];		// max	= 64 kbyte
	int		nNum, nPos;
	POSITION	Pos;
	
	// Header: 16 byte
	nPos = 0;
	memset(&szBuff[nPos], 0xFF, 8);
	nPos += 8;
	nPos += 3;	// CAT - LEN
	memset(&szBuff[nPos], 0x00, 5);
	nPos += 5;

	// Save Radar list
	memset(&szBuff[nPos], 0xFF, 3);
	nPos += 3;
	szBuff[nPos++]	= 0;			// Zero byte

	nNum = m_ListRada.GetCount();
	szBuff[nPos++]	= nNum;		// Number of Radar 

	if (nNum != 0)
	{
		C2_Rada *pRada;			// 5 + 34 byte 

		Pos = m_ListRada.GetHeadPosition();
		while (Pos)
		{
			pRada = m_ListRada.GetNext(Pos);
				
			// 1: Lat/Long	- 6
			szBuff[nPos++] = BYTE (pRada->m_Lat  >>16); 
			szBuff[nPos++] = BYTE (pRada->m_Lat  >> 8);
			szBuff[nPos++] = BYTE (pRada->m_Lat      );
			szBuff[nPos++] = BYTE (pRada->m_Long >>16); 
			szBuff[nPos++] = BYTE (pRada->m_Long >> 8);
			szBuff[nPos++] = BYTE (pRada->m_Long     );
			
			// 2: Range/Az	- 6
			szBuff[nPos++] = pRada->m_Type;
			szBuff[nPos++] = BYTE (pRada->m_Rge >> 8);
			szBuff[nPos++] = BYTE (pRada->m_Rge     );

			szBuff[nPos++] = BYTE (pRada->m_AzBeg >> 8);
			szBuff[nPos++] = BYTE (pRada->m_AzBeg     );
			szBuff[nPos++] = BYTE (pRada->m_AzEnd >> 8);
			szBuff[nPos++] = BYTE (pRada->m_AzEnd     );

			// Name Radar	- 20
			::ConvStrBuff(pRada->m_szName, &szBuff[nPos], 20);
			nPos += 20;
			
			// Backup		- 2
			memset(&szBuff[nPos], 0x00, 2);
			nPos += 2;
		}
	}

	// Save Unit
	memset(&szBuff[nPos], 0xFF, 3);
	nPos += 3;
	szBuff[nPos++]	= 0;			// Zero byte

	nNum = m_ListUnit.GetCount();
	szBuff[nPos++]	= nNum;		// Number of Unit

	if (nNum != 0)
	{
		C2_Unit	*pUnit;				// 5 + 30 byte 

		Pos = m_ListUnit.GetHeadPosition();
		while (Pos)
		{
			pUnit = m_ListUnit.GetNext(Pos);

			// 1: Lat/Long	- 6
			szBuff[nPos++] = BYTE (pUnit->m_Lat  >>16); 
			szBuff[nPos++] = BYTE (pUnit->m_Lat  >> 8);
			szBuff[nPos++] = BYTE (pUnit->m_Lat      );
			szBuff[nPos++] = BYTE (pUnit->m_Long >>16); 
			szBuff[nPos++] = BYTE (pUnit->m_Long >> 8);
			szBuff[nPos++] = BYTE (pUnit->m_Long     );

			// 2: Type/Color- 2
			szBuff[nPos++] = pUnit->m_Type;
			szBuff[nPos++] = pUnit->m_Color;

			// 3: Name		- 20
			::ConvStrBuff(pUnit->m_szName, &szBuff[nPos], 20);
			nPos += 20;
			
			// Backup		- 2
			memset(&szBuff[nPos], 0x00, 2);
			nPos += 2;
		}
	}

	// Save Mark list
	memset(&szBuff[nPos], 0xFF, 3);
	nPos += 3;
	szBuff[nPos++]	= 0;			// Zero byte

	nNum  = m_ListMark.GetCount();
	szBuff[nPos++]	= nNum;		// Number of Unit

	if (nNum != 0)
	{
		C2_Mark	*pMark;				// 5 + 30 byte 

		Pos = m_ListMark.GetHeadPosition();
		while (Pos)
		{
			pMark = m_ListMark.GetNext(Pos);

			// 1: Lat/Long	- 6
			szBuff[nPos++] = BYTE (pMark->m_Lat  >>16); 
			szBuff[nPos++] = BYTE (pMark->m_Lat  >> 8);
			szBuff[nPos++] = BYTE (pMark->m_Lat      );
			szBuff[nPos++] = BYTE (pMark->m_Long >>16); 
			szBuff[nPos++] = BYTE (pMark->m_Long >> 8);
			szBuff[nPos++] = BYTE (pMark->m_Long     );

			// 2: Name		- 20
			::ConvStrBuff(pMark->m_szName, &szBuff[nPos], 20);
			nPos += 20;
			
			// Backup		- 2
			memset(&szBuff[nPos], 0x00, 2);
			nPos += 2;
		}
	}

	szBuff[ 8] = 2;					// CAT = 2 Data
	szBuff[ 9] = BYTE (nPos >> 8);	// LEN
	szBuff[10] = BYTE (nPos     );

	hFile.Write(szBuff, nPos);	
	hFile.Close();

	return true;
} */

//-----------------------------------------------------------------------------
//	Home zone
/*-----------------------------------------------------------------------------
bool CC_Data::LoadAlar()
{
	if (m_szAlar.Compare("") == 0)
		return false;
	
	CFile	hFile ;
	CString	szPath;
	
	szPath = m_szRoot + "\\" + m_szAlar;
	if (!hFile.Open(szPath, CFile::modeRead))	// No file / Not load
		return false;
	
	BYTE	szBuff[256*256];			// max	= 64 kbyte
	int		nLen, nPos, i;
	bool	bOpen = true;
	
	nLen = hFile.Read(szBuff, 256*256);
	hFile.Close();

	// 0: Check header: (8 byte = 0xFF)
	nPos = 0;
	while ((nPos < 8) && bOpen)
	{
		if (szBuff[nPos] != 0xFF)
			bOpen = false;		// Error of Header	
		else
			nPos ++;
	}

	// 0: Check CAT - LEN ( 3byte, 5 byte = 0x00)
	if (bOpen)
	{
		if (szBuff[nPos] != 3)		// CAT = 3: this File Home
			bOpen = false;		// Error of CAT
		else
		if (nLen != ((szBuff[nPos+1] << 8) | szBuff[nPos+2]))
			bOpen = false;	// Error of LEN
		
		nPos += 3;

		i = 0;
		while ((i < 5) && bOpen)
		{
			if (szBuff[nPos+i] != 0x00)
				bOpen = false;	// Error of End header
			else
				i ++;
		}
		nPos += 5;
	}
	
	if (!bOpen)
		return false; 
		
	char	szTmp[21];				// Name of Object
	int		nNum;
	
	// Load Poly area
	nPos += 4;						// 3 byte FF + Zero byte
	nNum  = szBuff[nPos++];			// Number of Radar 

	if (nNum != 0)
	{
		C2_Home *pHome;			// 5 + 58 byte
		C2_Point *pPoint;
		BYTE	  nTotal, j;

		for (i = 0; i < nNum; i++)
		{
			pHome = new C2_Home();
			if (!pHome)
				continue;

			// ID
			pHome->m_Uid	= (i+1);

			// 1: Status	- 1
			pHome->m_Status= szBuff[nPos++];			
			
			// 2: Point		- 1+ 6*6
			nTotal			= szBuff[nPos++];
			for (j = 0; j < nTotal; j ++)
			{
				pPoint = new C2_Point();
				if (pPoint)
				{
				pPoint->m_Lat	= (szBuff[nPos+0] <<16) | (szBuff[nPos+1] << 8) | (szBuff[nPos+2]);
				pPoint->m_Long	= (szBuff[nPos+3] <<16) | (szBuff[nPos+4] << 8) | (szBuff[nPos+5]);
				pHome->AddTail(pPoint);
				}
				nPos += 6;
			}
			pHome->GetAreaRect();

			// 3: Name		- 20
			memcpy(szTmp, &szBuff[nPos], 20);
			szTmp[20] = 0x00;
			pHome->m_szName = szTmp;
			nPos += 20;

			// 3: Backup	- 2
			nPos += 2;			
			pHome->GetAreaRect();			
			m_ListHome.AddTail(pHome);
		}
	}

	return true;
}


//-----------------------------------------------------------------------------
bool CC_Data::SaveAlar()
{
	if (m_szAlar.Compare("") == 0)
		return false;

	CFile		hFile;
	CFileStatus	hFileStat;		// handle of file status
	CString		szPath;			//
	
	szPath = m_szRoot + "\\" + m_szAlar;
	
	// Clear the Read Only 
	if (CFile::GetStatus(szPath, hFileStat))
	{
		hFileStat.m_attribute = 0x00;
		CFile::SetStatus(szPath, hFileStat);
	}
	
	if (!hFile.Open(szPath, CFile::modeCreate | CFile::modeWrite))
		return false;

	BYTE	szBuff[256*256];		// max	= 64 kbyte
	int		nNum, nPos;
	POSITION	Pos;
	
	// Header: 16 byte
	nPos = 0;
	memset(&szBuff[nPos], 0xFF, 8);
	nPos += 8;
	nPos += 3;	// CAT - LEN
	memset(&szBuff[nPos], 0x00, 5);
	nPos += 5;

	// Save Poly area
	memset(&szBuff[nPos], 0xFF, 3);
	nPos += 3;
	szBuff[nPos++]	= 0;			// Zero byte

	nNum = m_ListHome.GetCount();
	szBuff[nPos++]	= nNum;		// Number of Sector

	if (nNum != 0)
	{
		C2_Home *pHome;		// 5 + 16 byte
		C2_Point *pPoint;
		POSITION  PosPnt;

		Pos = m_ListHome.GetHeadPosition();
		while (Pos)
		{
			pHome = m_ListHome.GetNext(Pos);

			// 1: Status	- 1
			szBuff[nPos++] = pHome->m_Status;
			
			// 2: Point		- 1+6*6
			szBuff[nPos++] = pHome->Count();
			PosPnt = pHome->GetPosH();
			while (PosPnt)
			{
				pPoint = pHome->GetNext(PosPnt);

				// Lat/Long
				szBuff[nPos++] = BYTE (pPoint->m_Lat  >>16); 
				szBuff[nPos++] = BYTE (pPoint->m_Lat  >> 8);
				szBuff[nPos++] = BYTE (pPoint->m_Lat      );
				szBuff[nPos++] = BYTE (pPoint->m_Long >>16); 
				szBuff[nPos++] = BYTE (pPoint->m_Long >> 8);
				szBuff[nPos++] = BYTE (pPoint->m_Long     );				
			}

			// Name Home	- 20
			::ConvStrBuff(pHome->m_szName, &szBuff[nPos], 20);
			nPos += 20;	
			
			// 3: Backup	2
			memset(&szBuff[nPos], 0x00, 2);
			nPos += 2;
		}
	}

	szBuff[ 8] = 3;					// CAT = 3 Home
	szBuff[ 9] = BYTE (nPos >> 8);	// LEN
	szBuff[10] = BYTE (nPos     );

	hFile.Write(szBuff, nPos);	
	hFile.Close();

	return true;
} */