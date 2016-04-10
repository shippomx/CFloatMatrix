#include "StdAfx.h"
#include "FloatMatrix.h"

CFloatMatrix::CFloatMatrix( void )
{
	Initialize( 1, 1, 0, false );
}

CFloatMatrix::CFloatMatrix( String^ astrFileName )
{
	System::IO::BinaryReader^ brMatrix = gcnew System::IO::BinaryReader( System::IO::File::Open( astrFileName, System::IO::FileMode::Open ) ); 

	m_pv_iRows = brMatrix->ReadInt32( );
	m_pv_iColumns = brMatrix->ReadInt32( );

	m_pv_ar2dData = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = brMatrix->ReadSingle( );
		}
	}

	brMatrix->Close( );
	delete brMatrix;
}

CFloatMatrix::CFloatMatrix( CFloatMatrix^ amMatrix )
{
	m_pv_iRows    = amMatrix->Rows;
	m_pv_iColumns = amMatrix->Columns;
	m_pv_ar2dData = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = amMatrix[ iRow, iCol ];
		}
	}
}

CFloatMatrix::CFloatMatrix( int aiDimension )
{
	Initialize( aiDimension, aiDimension, 0, false );
}

CFloatMatrix::CFloatMatrix( int aiRows, int aiColumns )
{
	Initialize( aiRows, aiColumns, 0, false );
}

CFloatMatrix::CFloatMatrix( int aiRows, int aiColumns, float adInitValue )
{
	Initialize( aiRows, aiColumns, adInitValue, true );
}

void CFloatMatrix::Initialize( int aiRows, int aiColumns, float adInitValue, bool abInit )
{
	if( ( aiRows <= 0 ) || ( aiColumns <= 0 ) )
	{
		throw gcnew Exception( "Invalid number of rows or columns ( Constructor )" );
		m_pv_iRows    = 0;
		m_pv_iColumns = 0;
		return;
	}

	m_pv_iRows    = aiRows;
	m_pv_iColumns = aiColumns;
	m_pv_ar2dData = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );

	if( ! abInit )
	{
		return;
	}

	Assign( adInitValue );

}


CFloatMatrix::~CFloatMatrix( void )
{
	delete [ ] m_pv_ar2dData;

	GC::Collect( );
}


float CFloatMatrix::default::get( int aiRow, int aiCol )
{
	return m_pv_ar2dData[ aiRow, aiCol ];
}

void CFloatMatrix::default::set( int aiRow, int aiCol, float adValue )
{
	m_pv_ar2dData[ aiRow, aiCol ] = adValue;
}

float CFloatMatrix::default::get( int aiCount )
{
	int iRow = aiCount / m_pv_iColumns;
	int iCol = aiCount % m_pv_iColumns;
	return m_pv_ar2dData[ iRow, iCol ];
}

void CFloatMatrix::default::set( int aiCount, float adValue )
{
	int iRow = aiCount / m_pv_iColumns;
	int iCol = aiCount % m_pv_iColumns;
	m_pv_ar2dData[ iRow, iCol ] = adValue;
}

/*
void CFloatMatrix::default::set( float adValue )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = adValue;
		}
	}
}
*/

int CFloatMatrix::FindChar( String^ astrSource, char achTarget )
{
	int iPos = -1;
	int iLength = astrSource->Length;
	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		if( achTarget == astrSource[ iCount ] )
		{
			iPos = iCount;
			break;
		}
	}

	return iPos;
}

int CFloatMatrix::GetSubscript( String^ astrInt )
{
	int iResult;

	try
	{
		iResult = Convert::ToInt32( astrInt );
	}
	catch ( System::OverflowException^ ) 
	{
		iResult = -1;
	}
	catch ( System::FormatException^ ) 
	{
		iResult = -1;
	}
	catch ( System::ArgumentException^ ) 
	{
		iResult = -1;
	}

	return iResult;

}

int CFloatMatrix::ParseSubscript( String^ astrSubscript, unsigned int * apiFrom, unsigned int * apiTo )
{
	String^ strSubscript = astrSubscript->Trim( );

	int iFrom, iTo;

	if( ":" == strSubscript )
	{
		return 1; // Entire
	}

	int iPos = FindChar( strSubscript, ':' );
	if( -1 == iPos )
	{
		iFrom = GetSubscript( strSubscript );
		if( 0 > iFrom )
		{
			return -1;
		}
		* apiFrom = iFrom;
		* apiTo   = iFrom;

		return 0;
	}

	String^ strFrom = strSubscript->Substring( 0, iPos )->Trim( );
	iFrom = GetSubscript( strFrom );
	if( 0 > iFrom )
	{
		return -1;
	}

	int iLength = strSubscript->Length - iPos - 1;
	String^ strTo = strSubscript->Substring( iPos + 1, iLength )->Trim( );
	iTo = GetSubscript( strTo );
	if( 0 > iTo )
	{
		return -1;
	}

	if( iFrom > iTo )
	{
		return -1;
	}

	* apiFrom = iFrom;
	* apiTo   = iTo;

	return 0;

}


int CFloatMatrix::ParseSubscript( String^ astrSubscript, int * apiFrom, int * apiTo )
{
	String^ strSubscript = astrSubscript->Trim( );

	int iFrom, iTo;

	if( ":" == strSubscript )
	{
		return 1; // Entire
	}

	int iPos = FindChar( strSubscript, ':' );
	if( -1 == iPos )
	{
		iFrom = GetSubscript( strSubscript );
		if( 0 > iFrom )
		{
			return -1;
		}
		* apiFrom = iFrom;
		* apiTo   = iFrom;

		return 0;
	}

	String^ strFrom = strSubscript->Substring( 0, iPos )->Trim( );
	iFrom = GetSubscript( strFrom );
	if( 0 > iFrom )
	{
		return -1;
	}

	int iLength = strSubscript->Length - iPos - 1;
	String^ strTo = strSubscript->Substring( iPos + 1, iLength )->Trim( );
	iTo = GetSubscript( strTo );
	if( 0 > iTo )
	{
		return -1;
	}

	if( iFrom > iTo )
	{
		return -1;
	}

	* apiFrom = iFrom;
	* apiTo   = iTo;

	return 0;

}

/////////////////////////
CFloatMatrix^ CFloatMatrix::default::get( String^ astrRange )
{
	unsigned int iFrom, iTo;

	switch( ParseSubscript( astrRange, & iFrom, & iTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return nullptr;

	case 0:

		if( ( iFrom >= this->Length ) || ( iTo >= this->Length ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return nullptr;
		}

		break;

	case 1:

		iFrom = 0;
		iTo   = this->Length - 1;

		break;
	}

	int iRows = iTo - iFrom + 1;
	int iCols = 1;

	CFloatMatrix^ vTemp = gcnew CFloatMatrix( iRows, iCols );

	for( unsigned int iCount = iFrom; iCount <= iTo; iCount ++ )
	{
		vTemp[ iCount - iFrom ] = this[ iCount ];
	}

	return vTemp;

}

//void CFloatMatrix::default::set( String^ astrRange, CFloatMatrix^ amSubMat )
//{
//	int iRowFrom, iRowTo, iColFrom, iColTo;
//
//	switch( ParseSubscript( astrRow, & iRowFrom, & iRowTo ) )
//	{
//	case -1:
//
//		throw gcnew Exception( "Invalid Subscript !" );
//
//		return;
//
//	case 0:
//
//		if( ( iRowFrom >= m_pv_iRows ) || ( iRowTo >= m_pv_iRows ) )
//		{
//			throw gcnew Exception( "Invalid Subscript !" );
//			return;
//		}
//
//		break;
//
//	case 1:
//
//		iRowFrom = 0;
//		iRowTo   = m_pv_iRows - 1;
//
//		break;
//	}
//
//
//	int iRows = iRowTo - iRowFrom + 1;
//	int iCols = iColTo - iColFrom + 1;
//
//	if( ( amSubMat->Rows != iRows ) || ( amSubMat->Columns != iCols ) )
//	{
//		throw gcnew Exception( "Invalid Dimension !" );
//		return;
//	}
//
//	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
//	{
//		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
//		{
//			m_pv_ar2dData[ iRow, iCol ] = amSubMat[ iRow - iRowFrom, iCol - iColFrom ];
//		}
//	}
//}
/////////////////////////

CFloatMatrix^ CFloatMatrix::default::get( String^ astrRow, String^ astrCol )
{
	int iRowFrom, iRowTo, iColFrom, iColTo;

	switch( ParseSubscript( astrRow, & iRowFrom, & iRowTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return nullptr;

	case 0:

		if( ( iRowFrom >= m_pv_iRows ) || ( iRowTo >= m_pv_iRows ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return nullptr;
		}

		break;

	case 1:

		iRowFrom = 0;
		iRowTo   = m_pv_iRows - 1;

		break;
	}

	switch( ParseSubscript( astrCol, & iColFrom, & iColTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return nullptr;

	case 0:

		if( ( iColFrom >= m_pv_iColumns ) || ( iColTo >= m_pv_iColumns ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return nullptr;
		}

		break;

	case 1:

		iColFrom = 0;
		iColTo   = m_pv_iColumns - 1;

		break;
	}

	int iRows = iRowTo - iRowFrom + 1;
	int iCols = iColTo - iColFrom + 1;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );

	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
	{
		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
		{
			mTemp[ iRow - iRowFrom, iCol - iColFrom ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return mTemp;

}

void CFloatMatrix::default::set( String^ astrRow, String^ astrCol, CFloatMatrix^ amSubMat )
{
	int iRowFrom, iRowTo, iColFrom, iColTo;

	switch( ParseSubscript( astrRow, & iRowFrom, & iRowTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return;

	case 0:

		if( ( iRowFrom >= m_pv_iRows ) || ( iRowTo >= m_pv_iRows ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return;
		}

		break;

	case 1:

		iRowFrom = 0;
		iRowTo   = m_pv_iRows - 1;

		break;
	}

	switch( ParseSubscript( astrCol, & iColFrom, & iColTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return;

	case 0:

		if( ( iColFrom >= m_pv_iColumns ) || ( iColTo >= m_pv_iColumns ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return;
		}

		break;

	case 1:

		iColFrom = 0;
		iColTo   = m_pv_iColumns - 1;

		break;
	}

	int iRows = iRowTo - iRowFrom + 1;
	int iCols = iColTo - iColFrom + 1;

	if( ( amSubMat->Rows != iRows ) || ( amSubMat->Columns != iCols ) )
	{
		throw gcnew Exception( "Invalid Dimension !" );
		return;
	}

	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
	{
		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = amSubMat[ iRow - iRowFrom, iCol - iColFrom ];
		}
	}

}

CFloatMatrix^ CFloatMatrix::default::get( String^ astrRow, String^ astrCol, String^ astrAny )
{
	int iRowFrom, iRowTo, iColFrom, iColTo;

	switch( ParseSubscript( astrRow, & iRowFrom, & iRowTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return nullptr;

	case 1:

		iRowFrom = 0;
		iRowTo   = m_pv_iRows - 1;

		break;
	}

	switch( ParseSubscript( astrCol, & iColFrom, & iColTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return nullptr;

	case 1:

		iColFrom = 0;
		iColTo   = m_pv_iColumns - 1;

		break;
	}

	int iRows = iRowTo - iRowFrom + 1;
	int iCols = iColTo - iColFrom + 1;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );

	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
	{
		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
		{
			mTemp[ iRow - iRowFrom, iCol - iColFrom ] = m_pv_ar2dData[ iRow % m_pv_iRows, iCol % m_pv_iColumns ];
		}
	}

	return mTemp;

}

void CFloatMatrix::default::set( String^ astrRow, String^ astrCol, float adValue )
{
	int iRowFrom, iRowTo, iColFrom, iColTo;

	switch( ParseSubscript( astrRow, & iRowFrom, & iRowTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return;

	case 0:

		if( ( iRowFrom >= m_pv_iRows ) || ( iRowTo >= m_pv_iRows ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return;
		}

		break;

	case 1:

		iRowFrom = 0;
		iRowTo   = m_pv_iRows - 1;

		break;
	}

	switch( ParseSubscript( astrCol, & iColFrom, & iColTo ) )
	{
	case -1:

		throw gcnew Exception( "Invalid Subscript !" );

		return;

	case 0:

		if( ( iColFrom >= m_pv_iColumns ) || ( iColTo >= m_pv_iColumns ) )
		{
			throw gcnew Exception( "Invalid Subscript !" );
			return;
		}

		break;

	case 1:

		iColFrom = 0;
		iColTo   = m_pv_iColumns - 1;

		break;
	}
#pragma omp parallel for
	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
	{
		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = adValue;
		}
	}

}

void CFloatMatrix::Assign( CFloatMatrix^ amNewMatrix )
{
	delete [ ] m_pv_ar2dData;

	m_pv_iRows = amNewMatrix->Rows;
	m_pv_iColumns = amNewMatrix->Columns;

	m_pv_ar2dData = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = amNewMatrix[ iRow, iCol ];
		}
	}

}

void CFloatMatrix::Assign( float adValue )
{
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = adValue;
		}
	}
}

CFloatMatrix^ CFloatMatrix::Copy( void )
{
	return gcnew CFloatMatrix( this );
}

CFloatMatrix^ CFloatMatrix::operator+( CFloatMatrix^ amMatrix )
{
	if( ( amMatrix->Rows != m_pv_iRows ) || ( amMatrix->Columns != m_pv_iColumns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing addition operation !");
		return nullptr;
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] + amMatrix[ iRow, iCol ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator+( float adScalar )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] + adScalar;
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator+( float adScalar, CFloatMatrix^ amMatrix )
{
	return amMatrix + adScalar;
}

CFloatMatrix^ CFloatMatrix::operator+( void )
{
	return this;
}

CFloatMatrix^ CFloatMatrix::operator-( CFloatMatrix^ amMatrix )
{
	if( ( amMatrix->Rows != m_pv_iRows ) || ( amMatrix->Columns != m_pv_iColumns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when do subtraction operation !");
		return nullptr;
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] - amMatrix[ iRow, iCol ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator-( float adScalar )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] - adScalar;
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator-( float adScalar, CFloatMatrix^ amMatrix )
{
	return amMatrix * ( -1 ) + adScalar;
}

CFloatMatrix^ CFloatMatrix::operator-( void )
{
	return this * ( -1 );
}

CFloatMatrix^ CFloatMatrix::operator*( CFloatMatrix^ amMatrix )
{
	if ( m_pv_iColumns != amMatrix->Rows )
	{
		throw gcnew Exception( "Matrics dimensions must agree when doing multiplication operation !" );
		return nullptr;
	}

	int iRows = m_pv_iRows;
	int iCols = amMatrix->Columns;
	int iLength = m_pv_iColumns;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );
	
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = 0;
			for ( int iCount = 0; iCount < iLength; iCount ++ )
			{
				mTemp[ iRow, iCol ] += m_pv_ar2dData[ iRow, iCount ] * amMatrix[ iCount, iCol ];
			}
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator*( float adScalar )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] * adScalar;
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator*( float adScalar, CFloatMatrix^ amMatrix )
{
	return amMatrix * adScalar;
}

CFloatMatrix^ CFloatMatrix::operator&( CFloatMatrix^ amMatrix )
{
	if( ( amMatrix->Rows != m_pv_iRows ) || ( amMatrix->Columns != m_pv_iColumns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element multiplication operation !");
		return nullptr;
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] * amMatrix[ iRow, iCol ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::operator/( float adScalar )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iRows, m_pv_iColumns );
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] / adScalar;
		}
	}

	return mTemp;
}

void CFloatMatrix::ElementAddition( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	if( ( m_pv_iRows != iRows ) || ( m_pv_iColumns != iCols ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element addition operation !");
		return;
	}
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] + amMatrix[ iRow, iCol ];
		}
	}

}

CFloatMatrix^ CFloatMatrix::ElementAddition( CFloatMatrix^ amMat1, CFloatMatrix^ amMat2 )
{
	if( ( amMat1->Rows != amMat2->Rows ) || ( amMat1->Columns != amMat2->Columns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element addition operation !");
		return nullptr;
	}

	int iRows = amMat1->Rows;
	int iCols = amMat1->Columns;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amMat1[ iRow, iCol ] + amMat2[ iRow, iCol ];
		}
	}

	return mTemp;
}

void CFloatMatrix::ElementMultiplication( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	if( ( m_pv_iRows != iRows ) || ( m_pv_iColumns != iCols ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element multiplication operation !");
		return;
	}
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] * amMatrix[ iRow, iCol ];
		}
	}

}

CFloatMatrix^ CFloatMatrix::ElementMultiplication( CFloatMatrix^ amMat1, CFloatMatrix^ amMat2 )
{
	if( ( amMat1->Rows != amMat2->Rows ) || ( amMat1->Columns != amMat2->Columns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element multiplication operation !");
		return nullptr;
	}

	int iRows = amMat1->Rows;
	int iCols = amMat1->Columns;
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amMat1[ iRow, iCol ] * amMat2[ iRow, iCol ];
		}
	}

	return mTemp;
}

void CFloatMatrix::ElementPower( float adPower )
{
#pragma omp parallel for
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = ( float )Math::Pow( m_pv_ar2dData[ iRow, iCol ], adPower );
		}
	}
}

CFloatMatrix^ CFloatMatrix::ElementPower( CFloatMatrix^ amMatix, float adPower )
{
	int iRows = amMatix->Rows;
	int iCols = amMatix->Columns;
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = ( float )Math::Pow( amMatix[ iRow, iCol ], adPower );
		}
	}

	return mTemp;
}

void CFloatMatrix::Resize( int aiRows, int aiCols )
{
	m_pv_iRows    = aiRows;
	m_pv_iColumns = aiCols;
	delete [ ] m_pv_ar2dData;
 	m_pv_ar2dData = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );
}

int CFloatMatrix::Reshape( int aiRows, int aiCols )
{
	if( m_pv_iRows * m_pv_iColumns != aiRows * aiCols )
	{
		return -1;
	}

 	array < float, 2 >^ ar2dData = gcnew array < float, 2 >( aiRows, aiCols );

	for( int iRow = 0; iRow < aiRows; iRow ++ )
	{
		for( int iCol = 0; iCol < aiCols; iCol ++ )
		{
			unsigned long ulLoc = iRow * aiCols + iCol;
			int iOldRow = ulLoc / m_pv_iColumns;
			int iOldCol = ulLoc % m_pv_iColumns;
			ar2dData[ iRow, iCol ] = m_pv_ar2dData[ iOldRow, iOldCol ];
		}
	}

	m_pv_iRows    = aiRows;
	m_pv_iColumns = aiCols;

	delete [ ] m_pv_ar2dData;
	m_pv_ar2dData = ar2dData;

	return 0;
}

CFloatMatrix^ CFloatMatrix::Reshape( CFloatMatrix^ amMatrix, int aiRows, int aiCols )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	if( iRows * iCols != aiRows * aiCols )
	{
		throw gcnew Exception( "Invalid number of rows or columns ( Reshape )" );
		return nullptr;
	}

 	CFloatMatrix^ mTemp = gcnew CFloatMatrix( aiRows, aiCols );

	for( int iRow = 0; iRow < aiRows; iRow ++ )
	{
		for( int iCol = 0; iCol < aiCols; iCol ++ )
		{
			unsigned long ulLoc = iRow * aiCols + iCol;
			int iOldRow = ulLoc / iCols;
			int iOldCol = ulLoc % iCols;
			mTemp[ iRow, iCol ] = amMatrix[ iOldRow, iOldCol ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Identity( int aiDim )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( aiDim, aiDim );

	for( int iRow = 0; iRow < aiDim; iRow ++ )
	{
		for( int iCol = 0; iCol < aiDim; iCol ++ )
		{
			if( iRow == iCol )
			{
				mTemp[ iRow, iCol ] = 1;
			}
			else
			{
				mTemp[ iRow, iCol ] = 0;
			}
		}
	}

	return mTemp;

}

void CFloatMatrix::Ones( void )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = 1;
		}
	}
}

CFloatMatrix^ CFloatMatrix::Ones( int aiRows, int aiCols )
{
	return gcnew CFloatMatrix( aiRows, aiCols, 1 );
}

CFloatMatrix^ CFloatMatrix::Ones( int aiDim )
{
	return gcnew CFloatMatrix( aiDim, aiDim, 1 );
}

void CFloatMatrix::Zeros( void )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = 0;
		}
	}
}

CFloatMatrix^ CFloatMatrix::Zeros( int aiRows, int aiCols )
{
	return gcnew CFloatMatrix( aiRows, aiCols, 0 );
}

CFloatMatrix^ CFloatMatrix::Zeros( int aiDim )
{
	return gcnew CFloatMatrix( aiDim, aiDim, 0 );
}

void CFloatMatrix::ConcatByRow( CFloatMatrix^ amB )
{
	int iRowsA = m_pv_iRows;
	int iColsA = m_pv_iColumns;

	int iRowsB = amB->Rows;
	int iColsB = amB->Columns;

	if( iRowsB != iRowsA )
	{
		throw gcnew Exception( " Invalid number of rows ( RowConcat ) " );
		return;
	}

	m_pv_iRows    = iRowsA;
	m_pv_iColumns = iColsA + iColsB;

	array < float, 2 >^ ar2dTemp = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < iRowsA; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsA; iCol ++ )
		{
			ar2dTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	for( int iRow = 0; iRow < iRowsB; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsB; iCol ++ )
		{
			ar2dTemp[ iRow, iCol + iColsA ] = amB[ iRow, iCol ];
		}
	}

	delete [ ] m_pv_ar2dData;

	m_pv_ar2dData = ar2dTemp;

}

CFloatMatrix^ CFloatMatrix::ConcatByRow(  CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	int iRowsA = amA->Rows;
	int iColsA = amA->Columns;

	int iRowsB = amB->Rows;
	int iColsB = amB->Columns;

	if( iRowsB != iRowsA )
	{
		throw gcnew Exception( " Invalid number of rows ( RowConcat ) " );
		return nullptr;
	}

	int iNewRows = iRowsA;
	int iNewCols = iColsA + iColsB;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iNewRows, iNewCols );

	for( int iRow = 0; iRow < iRowsA; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsA; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amA[ iRow, iCol ];
		}
	}

	for( int iRow = 0; iRow < iRowsB; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsB; iCol ++ )
		{
			mTemp[ iRow, iCol + iColsA ] = amB[ iRow, iCol ];
		}
	}

	return mTemp;

}

void CFloatMatrix::ConcatByColumn( CFloatMatrix^ amB )
{
	int iRowsA = m_pv_iRows;
	int iColsA = m_pv_iColumns;

	int iRowsB = amB->Rows;
	int iColsB = amB->Columns;

	if( iColsB != iColsA )
	{
		throw gcnew Exception( " Invalid number of columns ( RowConcat ) " );
		return;
	}

	m_pv_iRows    = iRowsA + iRowsB;
	m_pv_iColumns = iColsA;

	array < float, 2 >^ ar2dTemp = gcnew array < float, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < iRowsA; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsA; iCol ++ )
		{
			ar2dTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	for( int iRow = 0; iRow < iRowsB; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsB; iCol ++ )
		{
			ar2dTemp[ iRow + iRowsA, iCol ] = amB[ iRow, iCol ];
		}
	}

	delete [ ] m_pv_ar2dData;

	m_pv_ar2dData = ar2dTemp;

}

CFloatMatrix^ CFloatMatrix::ConcatByColumn(  CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	int iRowsA = amA->Rows;
	int iColsA = amA->Columns;

	int iRowsB = amB->Rows;
	int iColsB = amB->Columns;

	if( iColsB != iColsA )
	{
		throw gcnew Exception( " Invalid number of columns ( RowConcat ) " );
		return nullptr;
	}

	int iNewRows = iRowsA + iRowsB;
	int iNewCols = iColsA;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iNewRows, iNewCols );

	for( int iRow = 0; iRow < iRowsA; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsA; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amA[ iRow, iCol ];
		}
	}

	for( int iRow = 0; iRow < iRowsB; iRow ++ )
	{
		for( int iCol = 0; iCol < iColsB; iCol ++ )
		{
			mTemp[ iRow + iRowsA, iCol ] = amB[ iRow, iCol ];
		}
	}

	return mTemp;

}

CFloatMatrix^ CFloatMatrix::Assemble( int aiRows, int aiCols, array< CFloatMatrix^ >^ aarmMat )
{
	if( ( aiRows <= 0 ) || ( aiCols <= 0 ) )
	{
		throw gcnew Exception( "Invalid number of rows or columns (Assemble)" );
		return nullptr;
	}

	if( aiRows * aiCols > aarmMat->Length )
	{
		throw gcnew Exception( "Matrics are not enough to be assembled (Assemble)" );
		return nullptr;
	}

	unsigned long ulRowLoc;
	int iNewRows = 0;
	for( int iRow = 0; iRow < aiRows; iRow ++ )
	{
		ulRowLoc = aiCols * iRow;
		int iRows = aarmMat[ ulRowLoc ]->Rows;
		iNewRows += iRows; 
		for( int iCol = 1; iCol < aiCols; iCol ++ )
		{
			unsigned long ulLoc = ulRowLoc + iCol;
			if( aarmMat[ ulLoc ]->Rows != iRows )
			{
				throw gcnew Exception( "Invalid number of rows of submatrix (Assemble)" );
				return nullptr;
			}
		}
	}

	int iNewCols = 0;
	for( int iCol = 0; iCol < aiCols; iCol ++ )
	{
		int iCols = aarmMat[ iCol ]->Columns;
		iNewCols += iCols; 
		for( int iRow = 1; iRow < aiRows; iRow ++ )
		{
			unsigned long ulLoc = iRow * aiCols + iCol;
			if( aarmMat[ ulLoc ]->Columns != iCols )
			{
				throw gcnew Exception( "Invalid number of columns of submatrix (Assemble)" );
				return nullptr;
			}
		}
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iNewRows, iNewCols );
	int iCurRowFrom = 0, iCurRowTo;
	int iCurColFrom = 0, iCurColTo;
	for( int iRow = 0; iRow < aiRows; iRow ++ )
	{
		ulRowLoc = iRow * aiCols;
		iCurRowTo = iCurRowFrom + aarmMat[ ulRowLoc ] ->Rows - 1;
		iCurColFrom = 0;
		for( int iCol = 0; iCol < aiCols; iCol ++ )
		{
			unsigned long ulLoc = ulRowLoc + iCol;
			iCurColTo = iCurColFrom + aarmMat[ ulLoc ]->Columns - 1;
			for( int iNewRow = iCurRowFrom; iNewRow <= iCurRowTo; iNewRow ++ )
			{
				for( int iNewCol = iCurColFrom; iNewCol <= iCurColTo; iNewCol ++ )
				{
					mTemp[ iNewRow, iNewCol ] = aarmMat[ ulLoc ][ iNewRow - iCurRowFrom, iNewCol - iCurColFrom ];
				}
			}

			iCurColFrom = iCurColTo + 1;
		}
		iCurRowFrom = iCurRowTo + 1;
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::SolveEquation( CFloatMatrix^ aMAA, CFloatMatrix^ aMY ) // Solve Ax = y => x = A \ y
{
	CFloatMatrix^ mMA = gcnew CFloatMatrix( aMAA );
	int iARows = mMA->Rows;
	int iACols = mMA->Columns;
	if( ( iARows != iACols ) || ( 1 != aMY->Columns ) || ( aMY->Rows != iARows ) )
	{
		throw gcnew Exception( "Dimension of A or y in A * x = y is not right !");
		return nullptr;
	}

	CFloatMatrix^ mU = gcnew CFloatMatrix( iARows, iACols, 0 );
	CFloatMatrix^ mL = gcnew CFloatMatrix( iARows, iACols, 0 );
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iARows, 1 );
	CFloatMatrix^ mMX = gcnew CFloatMatrix( iARows, 1 );

	const int iN = iARows;
	int iRow, iCol, iCount;

	for ( iRow = 0; iRow < iN; iRow ++ )  // for i = 1 : n
	{
		for ( iCol = iRow; iCol < iN; iCol ++ )   // for j = (i) : n
		{
			mL[ iCol, iRow ] = mMA[ iCol, iRow ] / mMA[ iRow, iRow ];  // L(j,i) = B(j,i)/B(i,i);
		}

		for ( iCol = iRow; iCol < iN; iCol ++ )  //for j = i : n
		{
			mU[ iRow, iCol ] = mMA[ iRow, iCol ];  //	U(i,j) = B(i,j);
		}

		for ( iCol = iRow + 1; iCol < iN; iCol ++ )  // for j = (i + 1) : n
		{
			for ( iCount = iRow + 1; iCount < iN; iCount ++ )  // for k = (i + 1) : n
			{
				mMA[ iCol, iCount ] = mMA[ iCol, iCount ] - mL[ iCol, iRow ] * mU[ iRow, iCount ];  // B(j,k) = B(j,k) - L(j,i) * U(i,k);
			}
		}
	}


	float dTemp;
	mTemp[ 0, 0 ] = aMY[ 0, 0 ]; // b(1) = y(1);
	for ( iRow = 1; iRow < iN; iRow ++ )  // for i = 2 : n
	{
		dTemp = 0; //temp = 0;
		for ( iCol = 0; iCol <= iRow - 1 ; iCol ++ )  // for j = 1 : (i - 1)
		{
			dTemp += mL[ iRow, iCol ] * mTemp[ iCol, 0 ];   // temp = temp + L(i,j)*b(j);
		}
		mTemp[ iRow, 0 ] = aMY[ iRow, 0 ] -  dTemp;   // b(i) = y(i) - temp;
	}


	mMX[ iN - 1, 0 ] = mTemp[ iN - 1, 0 ] / mU[ iN - 1, iN - 1 ];// x(n) = b(n) / U(n,n);
	for ( iRow = iN - 2; iRow >= 0; iRow -- )  // for i = ( n - 1 ): -1 : 1
	{
		dTemp = 0;  //temp = 0;
		for ( iCol = iRow + 1; iCol < iN; iCol ++ ) // for j = (i+1) : n
		{
			dTemp += mU[ iRow, iCol ] * mMX[ iCol, 0 ];  // temp = temp + U(i,j)*x(j);
		}
		mMX[ iRow, 0 ] = ( mTemp[ iRow, 0 ] - dTemp ) / mU[ iRow, iRow ];  //  x(i) = (b(i) - temp) / U(i,i);
	}

	delete mMA;
	delete mU;
	delete mL;
	delete mTemp;

	return mMX;
}

CFloatMatrix^ CFloatMatrix::LeastSquare( CFloatMatrix^ aMA, CFloatMatrix^ aMY )
{

	CFloatMatrix^ mTransposedA = aMA->Transpose( );
	CFloatMatrix^ mNewA = mTransposedA * aMA;
	CFloatMatrix^ mNewY = mTransposedA * aMY;
	CFloatMatrix^ mX = SolveEquation( mNewA, mNewY );
	delete mTransposedA;
	delete mNewA;
	delete mNewY;

	GC::Collect( );

	return mX;
}

CFloatMatrix^ CFloatMatrix::LeastSquare2( CFloatMatrix^ aMA, CFloatMatrix^ aMY )
{

	CFloatMatrix^ mTransposedA = aMA->Transpose( );
	CFloatMatrix^ mNewA = mTransposedA * aMA;
	CFloatMatrix^ mNewY = mTransposedA * aMY;
//	CFloatMatrix^ mX = SolveEquation( mNewA, mNewY );
	CFloatMatrix^ mX = CFloatMatrix::Inverse6( mNewA ) * mNewY;
	delete mTransposedA;
	delete mNewA;
	delete mNewY;

	GC::Collect( );

	return mX;
}

CFloatMatrix^ CFloatMatrix::Transpose( void )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iColumns, m_pv_iRows ); 

	for ( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iCol, iRow ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Transpose( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Columns;
	int iCols = amMatrix->Rows;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols ); 

	for ( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amMatrix[ iCol, iRow ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Transpose2( void )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( m_pv_iColumns, m_pv_iRows ); 

	for ( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ m_pv_iColumns - iCol - 1, iRow ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Transpose2( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Columns;
	int iCols = amMatrix->Rows;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols ); 

	for ( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
//			mTemp[ iRow, iCol ] = amMatrix[ iRows - iCol - 1, iRow ];
			mTemp[ iRow, iCol ] = amMatrix[ iCols - iCol - 1, iRow ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Product( CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	if ( amA->Columns != amB->Rows )
	{
		throw gcnew Exception( "Matrics dimensions must agree when doing multiplication operation !" );
		return nullptr;
	}

	int iRows = amA->Rows;
	int iCols = amB->Columns;
	int iLength = amA->Columns;;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );
	
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = 0;
			for ( int iCount = 0; iCount < iLength; iCount ++ )
			{
				mTemp[ iRow, iCol ] += amA[ iRow, iCount ] * amB[ iCount, iCol ];
			}
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Kronecker( CFloatMatrix^ amB )
{
	int iRows = m_pv_iRows;
	int iCols = m_pv_iColumns;
	int iLength = iRows * iCols;
	array< CFloatMatrix^ >^ armMat = gcnew array< CFloatMatrix^ >( iLength );
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			unsigned long ulLoc = iRow * iCols + iCol;
			armMat[ ulLoc ] = amB * m_pv_ar2dData[ iRow, iCol ];
		}
	}

	CFloatMatrix^ mTemp = Assemble( iRows, iCols, armMat );

	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		delete armMat[ iCount ];
	}

	delete [ ] armMat;

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Kronecker( CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	int iRows = amA->Rows;
	int iCols = amA->Columns;
	int iLength = iRows * iCols;
	array< CFloatMatrix^ >^ armMat = gcnew array< CFloatMatrix^ >( iLength );
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			unsigned long ulLoc = iRow * iCols + iCol;
			armMat[ ulLoc ] = amB * amA[ iRow, iCol ];
		}
	}

	CFloatMatrix^ mTemp = Assemble( iRows, iCols, armMat );

	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		delete armMat[ iCount ];
	}

	delete [ ] armMat;

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::WalshMatrix( int aiDim )
{
	//k = log2( Total );
	float dLength = ( float )Math::Log( aiDim, 2 );
	int iLength;
	if( Math::Floor( dLength ) != dLength ) //if( floor( k ) ~= k )
	{
		iLength = ( int )( Math::Ceiling( dLength ) ); //k = k + 1;
	}   //end
	else
	{
		iLength = ( int )dLength;
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( 1, 1, 1 ); //H = [ 1 ];

	array< CFloatMatrix^ >^ armMat = gcnew array< CFloatMatrix^ >( 4 );
	for( int iCount = 0; iCount < iLength; iCount ++ ) //for i = 1 : k
	{
		armMat[ 0 ] = mTemp->Copy( );
		armMat[ 1 ] = mTemp->Copy( );
		armMat[ 2 ] = mTemp->Copy( );
		armMat[ 3 ] = (-mTemp)->Copy( );
		delete mTemp;
		mTemp = Assemble( 2, 2, armMat );  //  H = [ H, H; H, -H ];
		delete armMat[ 0 ];
		delete armMat[ 1 ];
		delete armMat[ 2 ];
		delete armMat[ 3 ];
	} //end

	delete [ ] armMat;

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Diff( void )
{
	CFloatMatrix^ mDiff = nullptr;
	if( m_pv_iRows > 1 && m_pv_iColumns > 1 )
	{
		mDiff = gcnew CFloatMatrix( m_pv_iRows - 1, m_pv_iColumns );
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			for( int iRow = 1; iRow < m_pv_iRows; iRow ++ )
			{
				mDiff[ iRow - 1, iCol ] = m_pv_ar2dData[ iRow, iCol ] - m_pv_ar2dData[ iRow - 1, iCol ];
			}
		}
	}
	else if( 1 == m_pv_iRows || 1 < m_pv_iColumns )
	{
		if( 1 == m_pv_iColumns )
		{
			mDiff = gcnew CFloatMatrix( 1, m_pv_iColumns );
		}
		else
		{
			mDiff = gcnew CFloatMatrix( 1, m_pv_iColumns - 1 );
		}
		for( int iCol = 1; iCol < m_pv_iColumns; iCol ++ )
		{
			mDiff[ 0, iCol - 1 ] = m_pv_ar2dData[ 0, iCol ] - m_pv_ar2dData[ 0, iCol - 1 ];
		}
	}
	else if( 1 < m_pv_iRows && 1 == m_pv_iColumns )
	{
		mDiff = gcnew CFloatMatrix( m_pv_iRows - 1, 1 );
		for( int iRow = 1; iRow < m_pv_iRows; iRow ++ )
		{
			mDiff[ iRow - 1, 0 ] = m_pv_ar2dData[ iRow, 0 ] - m_pv_ar2dData[ iRow - 1, 0 ];
		}
	}

	return mDiff;
}

float CFloatMatrix::Mean( void )
{
	unsigned int uiLen = 0;
	double dSum = 0;
	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			double dTemp = m_pv_ar2dData[ iRow, iCol ];
			if( ! double::IsNaN( dTemp ) )
			{
				dSum += dTemp;
				uiLen ++;
			}
		}
	}

	if( 0 == uiLen )
	{
		return float::NaN;
	}
	else
	{
		return float( dSum / uiLen );
	}
}

float CFloatMatrix::Mean( CFloatMatrix^ amMatrix )
{
	unsigned int uiLen = 0;
	double dSum = 0;
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			double dTemp = amMatrix[ iRow, iCol ];
			if( ! double::IsNaN( dTemp ) )
			{
				dSum += dTemp;
				uiLen ++;
			}
		}
	}

	if( 0 == uiLen )
	{
		return float::NaN;
	}
	else
	{
		return float( dSum / uiLen );
	}
}

float CFloatMatrix::Var( void )
{
	unsigned int uiLen = 0;
	double dSum = 0;
	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			double dTemp = m_pv_ar2dData[ iRow, iCol ];
			if( ! double::IsNaN( dTemp ) )
			{
				dSum += dTemp;
				uiLen ++;
			}
		}
	}

	switch( uiLen )
	{
	case 0:
		return float::NaN;
	case 1:
		return 0;
	}

	double dMean = ( float )( dSum / uiLen );

	dSum = 0;
	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			double dTemp = m_pv_ar2dData[ iRow, iCol ];
			if( ! double::IsNaN( dTemp ) )
			{
				dTemp -= dMean;
				dSum += ( dTemp * dTemp );
			}
		}
	}

	return float( dSum / ( uiLen - 1 ) );

}

float CFloatMatrix::Var( CFloatMatrix^ amMatrix )
{
	unsigned int uiLen = 0;
	double dSum = 0;
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			double dTemp = amMatrix[ iRow, iCol ];
			if( ! double::IsNaN( dTemp ) )
			{
				dSum += dTemp;
				uiLen ++;
			}
		}
	}

	switch( uiLen )
	{
	case 0:
		return float::NaN;
	case 1:
		return 0;
	}

	double dMean = dSum / uiLen;

	dSum = 0;
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			double dTemp = amMatrix[ iRow, iCol ];
			if( ! double::IsNaN( dTemp ) )
			{
				dTemp -= dMean;
				dSum += ( dTemp * dTemp );
			}
		}
	}

	return float( dSum / ( uiLen - 1 ) );
}

float CFloatMatrix::Max( void )
{
	float fMax = float::MinValue;

	float fTemp;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			fTemp = m_pv_ar2dData[ iRow, iCol ];
			if( ( ! float::IsNaN( fTemp ) ) && ( fTemp > fMax ) )
			{
				fMax = fTemp;
			}
		}
	}

	return fMax;
}

float CFloatMatrix::Max( CFloatMatrix^ amMatrix )
{
	float fMax = float::MinValue;

	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	float fTemp;
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			fTemp = amMatrix[ iRow, iCol ];
			if( ( ! float::IsNaN( fTemp ) ) && ( fTemp > fMax ) )
			{
				fMax = fTemp;
			}
		}
	}

	return fMax;
}

float CFloatMatrix::Max( int * apiRow, int * apiCol )
{
	float dMax = m_pv_ar2dData[ 0, 0 ];
	* apiRow = 0;
	* apiCol = 0;

	float dTemp;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dTemp = m_pv_ar2dData[ iRow, iCol ];
			if( dTemp > dMax )
			{
				dMax = dTemp;
				* apiRow = iRow;
				* apiCol = iCol;
			}
		}
	}

	return dMax;
}

float CFloatMatrix::Max( CFloatMatrix^ amMatrix, int * apiRow, int * apiCol )
{
	float dMax = amMatrix[ 0, 0 ];
	* apiRow = 0;
	* apiCol = 0;

	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	float dTemp;

	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dTemp = amMatrix[ iRow, iCol ];
			if( dTemp > dMax )
			{
				dMax = dTemp;
				* apiRow = iRow;
				* apiCol = iCol;
			}
		}
	}
	return dMax;
}

float CFloatMatrix::Min( void )
{
	float dMin = m_pv_ar2dData[ 0, 0 ];

	float dTemp;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dTemp = m_pv_ar2dData[ iRow, iCol ];
			if( dTemp < dMin )
			{
				dMin = dTemp;
			}
		}
	}

	return dMin;
}

float CFloatMatrix::Min( CFloatMatrix^ amMatrix )
{
	float dMin = amMatrix[ 0, 0 ];

	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	float dTemp;

	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dTemp = amMatrix[ iRow, iCol ];
			if( dTemp < dMin )
			{
				dMin = dTemp;
			}
		}
	}

	return dMin;
}

void CFloatMatrix::MaxAndMin( float & adMax, float & adMin )
{
	adMax = Single::MinValue;
	adMin = Single::MaxValue;

	float fTemp;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			fTemp = m_pv_ar2dData[ iRow, iCol ];

			if( Single::IsInfinity( fTemp ) || Single::IsNaN( fTemp ) )
			{
				continue;
			}

			if( fTemp > adMax )
			{
				adMax = fTemp;
			}

			if( fTemp < adMin )
			{
				adMin = fTemp;
			}
		}
	}

}

void CFloatMatrix::MaxAndMin( CFloatMatrix^ amMatrix, float & adMax, float & adMin )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	adMax = Single::MinValue;
	adMin = Single::MaxValue;

	float dTemp;

	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dTemp = amMatrix[ iRow, iCol ];
			if( Single::IsInfinity( dTemp ) || Single::IsNaN( dTemp ) )
			{
				continue;
			}

			if( dTemp > adMax )
			{
				adMax = dTemp;
			}

			if( dTemp < adMin )
			{
				adMin = dTemp;
			}
		}
	}

}

int CFloatMatrix::ConvertStringToInt( String^ astrInt )
{
	int iResult;

	try
	{
		iResult = Convert::ToInt32( astrInt );
	}
	catch ( System::OverflowException^ ) 
	{
		iResult = 0;
	}
	catch ( System::FormatException^ ) 
	{
		iResult = 0;
	}
	catch ( System::ArgumentException^ ) 
	{
		iResult = 0;
	}

	return iResult;

}


float CFloatMatrix::ConvertStringToFloat( String^ astrDoulbe )
{
	float dResult;

	try
	{
		dResult = Convert::ToSingle( astrDoulbe );
	}
	catch ( System::OverflowException^ ) 
	{
		dResult = 0;
	}
	catch ( System::FormatException^ ) 
	{
		dResult = 0;
	}
	catch ( System::ArgumentException^ ) 
	{
		dResult = 0;
	}

	return dResult;

}

CFloatMatrix^ CFloatMatrix::GetCirculantMatrix( CFloatMatrix^ amVector )
{
	int iHeight = amVector->Rows;
	int iWidth  = amVector->Columns;

	if( 1 == iHeight * iWidth )
	{
		throw gcnew Exception( "It should be 1 x n or n x 1 vector ( GetCirculantMatrix ) !" );
		return nullptr;
	}

	if( ( 1 != iHeight ) && ( 1 != iWidth ) )
	{
		throw gcnew Exception( "It should be 1 x n or n x 1 vector ( GetCirculantMatrix ) !" );
		return nullptr;
	}

	int iLength = iHeight * iWidth;
	CFloatMatrix^ mTemp  = gcnew CFloatMatrix( iLength );

	for ( int iRow = 0; iRow < iLength; iRow ++ )
	{
		for( int iCol = 0; iCol < iLength; iCol ++ )
		{
			mTemp[ iLength - iRow - 1, iCol ] = amVector[ ( iRow + iCol + 1 ) % iLength ];
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::GetCosColumn( float adFrequency, int aiLength )
{

	if( 0 >= aiLength )
	{
		throw gcnew Exception( "Length should >= 0 ( GetCosColumn ) " );
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( aiLength, 1 );

	float dConst = ( float )( 2.0 * Math::PI * adFrequency / aiLength );
	float dAmp   = 1; //127.5;
	for( int iCount = 0; iCount < aiLength; iCount ++ )
	{
		mTemp[ iCount, 0 ] = ( float )( dAmp * Math::Cos( dConst * iCount ) );
	}

	return mTemp;
}

float CFloatMatrix::VectorNorm( CFloatMatrix^ amVector, int aiNorm )
{

	if( ( 1 != amVector->Rows ) && ( 1 != amVector->Columns ) )
	{
		throw gcnew Exception( "It should be a vector ( Norm )!" );
		return -1;
	}

//	float dNorm = aiNorm;
	int iLength = amVector->Rows * amVector->Columns;

	if( aiNorm > 0 )
	{
		float dSum = 0;

		for( int iCount = 0; iCount < iLength; iCount ++ )
		{
			dSum += ( float )Math::Pow( Math::Abs( amVector[ iCount ] ), aiNorm );
		}
		return ( float )Math::Pow( dSum, 1.0 / aiNorm );
	}
	else // inf norm
	{
		float dMax = Math::Abs( amVector[ 0 ] );
		float dTemp;
		for( int iCount = 1; iCount < iLength; iCount ++ )
		{
			dTemp = Math::Abs( amVector[ iCount ] );
			if( dTemp > dMax )
			{
				dMax = dTemp;
			}
		}
		return( dMax );
	}
}

CFloatMatrix^ CFloatMatrix::CrossProduct( CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	int iLengthA = amA->Rows * amA->Columns;
	if( 3 != iLengthA )
	{
		throw gcnew Exception( "It should be a 1x3 or 3x1 vector ( CrossProduct )!" );
		return nullptr;
	}

	int iLengthB = amB->Rows * amB->Columns;
	if( 3 != iLengthA )
	{
		throw gcnew Exception( "It should be a 1x3 or 3x1 vector ( CrossProduct )!" );
		return nullptr;
	}

	if( ( amA->Rows != amB->Rows ) || ( amA->Columns != amB->Columns ) )
	{
		throw gcnew Exception( "The two matrics should have the same dimension ( CrossProduct )!" );
		return nullptr;
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( amA->Rows, amA->Columns );
//[ a2*b3-a3*b2, a3*b1-a1*b3, a1*b2-a2*b1]

//c1 = a(2) * b(3) - a(3) * b(2);
	mTemp[ 0 ] = amA[ 1 ] * amB[ 2 ] - amA[ 2 ] * amB[ 1 ];
//c2 = a(3) * b(1) - a(1) * b(3);
	mTemp[ 1 ] = amA[ 2 ] * amB[ 0 ] - amA[ 0 ] * amB[ 2 ];
//c3 = a(1) * b(2) - a(2) * b(1);
	mTemp[ 2 ] = amA[ 0 ] * amB[ 1 ] - amA[ 1 ] * amB[ 0 ];

//c = [ c1; c2; c3 ];

	return mTemp;
}

float CFloatMatrix::DotProduct( CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	if( ( 1 != amA->Rows ) && ( 1 != amA->Columns ) )
	{
		throw gcnew Exception( "It should be a vector ( DotProduct )!" );
		return -9999;
	}

	if( ( 1 != amB->Rows ) && ( 1 != amB->Columns ) )
	{
		throw gcnew Exception( "It should be a vector ( DotProduct )!" );
		return -9999;
	}

	if( ( amA->Rows != amB->Rows ) || ( amA->Columns != amB->Columns ) )
	{
		throw gcnew Exception( "The two matrics should have the same dimension ( DotProduct )!" );
		return -9999;
	}

	int iLength = amA->Rows * amA->Columns;

	float dSum = 0;
	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		dSum += amA[ iCount ] * amB[ iCount ];
	}

	return dSum;
}

float CFloatMatrix::Determinant2( CFloatMatrix^ amA )
{
	if( ( 2 != amA->Rows ) || ( 2 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 2 x 2 matrix !" );
		return 0;
	}

	return( amA[ 0, 0 ] * amA[ 1, 1 ] - amA[ 0, 1 ] * amA[ 1, 0 ] );
}

float CFloatMatrix::Determinant3( CFloatMatrix^ amA )
{
	if( ( 3 != amA->Rows ) || ( 3 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 3 x 3 matrix !" );
		return 0;
	}

	float dDet = amA[ 0, 0 ] * amA[ 1, 1 ] * amA[ 2, 2 ];
	dDet += amA[ 1, 0 ] * amA[ 0, 2 ] * amA[ 2, 1 ];
	dDet += amA[ 2, 0 ] * amA[ 0, 1 ] * amA[ 1, 2 ];
	dDet -= amA[ 0, 0 ] * amA[ 1, 2 ] * amA[ 2, 1 ];
	dDet -= amA[ 1, 0 ] * amA[ 0, 1 ] * amA[ 2, 2 ];
	dDet -= amA[ 2, 0 ] * amA[ 0, 2 ] * amA[ 1, 1 ];

	return dDet;
}

CFloatMatrix^ CFloatMatrix::Inverse2( CFloatMatrix^ amA )
{
	if( ( 2 != amA->Rows ) || ( 2 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 2 x 2 matrix !" );
		return nullptr;
	}

	float dDet = CFloatMatrix::Determinant2( amA );

	if( 0 == dDet )
	{
		throw gcnew Exception( " Its determinant is zero !" );
		return nullptr;
	}

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( 2 );
	mTemp[ 0, 0 ] = amA[ 1, 1 ] / dDet;
	mTemp[ 0, 1 ] = - amA[ 0, 1 ] / dDet;
	mTemp[ 1, 0 ] = - amA[ 1, 0 ] / dDet;
	mTemp[ 1, 1 ] = amA[ 0, 0 ] / dDet;


	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Inverse3( CFloatMatrix^ amA )
{
	if( ( 3 != amA->Rows ) || ( 3 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 3 x 3 matrix !" );
		return nullptr;
	}
//Det = A( 1, 1 ) * A( 2, 2 ) * A( 3, 3 );
//Det = Det + A( 2, 1 ) * A( 1, 3 ) * A( 3, 2 );
//Det = Det + A( 3, 1 ) * A( 1, 2 ) * A( 2, 3 );
//Det = Det - A( 1, 1 ) * A( 2, 3 ) * A( 3, 2 );
//Det = Det - A( 2, 1 ) * A( 1, 2 ) * A( 3, 3 );
//Det = Det - A( 3, 1 ) * A( 1, 3 ) * A( 2, 2 );
///	float dDet = amA[ 0, 0 ] * amA[ 1, 1 ] * amA[ 2, 2 ];
///	dDet += amA[ 1, 0 ] * amA[ 0, 2 ] * amA[ 2, 1 ];
///	dDet += amA[ 2, 0 ] * amA[ 0, 1 ] * amA[ 1, 2 ];
///	dDet -= amA[ 0, 0 ] * amA[ 1, 2 ] * amA[ 2, 1 ];
///	dDet -= amA[ 1, 0 ] * amA[ 0, 1 ] * amA[ 2, 2 ];
///	dDet -= amA[ 2, 0 ] * amA[ 0, 2 ] * amA[ 1, 1 ];

	float dDet = CFloatMatrix::Determinant3( amA );

	if( 0 == dDet )
	{
		throw gcnew Exception( " Its determinant is zero !" );
		return nullptr;
	}

//InvA = zeros( 3, 3 );
//InvA( 1, 1 ) = A( 2, 2 ) * A( 3, 3 ) - A( 2, 3 ) * A( 3, 2 );
//InvA( 1, 2 ) = -A( 1, 2 ) * A( 3, 3 ) + A( 1, 3 ) * A( 3, 2 );
//InvA( 1, 3 ) = A( 1, 2 ) * A( 2, 3 ) - A( 1, 3 ) * A( 2, 2 );

//InvA( 2, 1 ) = -A( 2, 1 ) * A( 3, 3 ) + A( 2, 3 ) * A( 3, 1 );
//InvA( 2, 2 ) = A( 1, 1 ) * A( 3, 3 ) - A( 1, 3 ) * A( 3, 1 );
//InvA( 2, 3 ) = -A( 1, 1 ) * A( 2, 3 ) + A( 1, 3 ) * A( 2, 1 );

//InvA( 3, 1 ) = A( 2, 1 ) * A( 3, 2 ) - A( 2, 2 ) * A( 3, 1 );
//InvA( 3, 2 ) = -A( 1, 1 ) * A( 3, 2 ) + A( 1, 2 ) * A( 3, 1 );
//InvA( 3, 3 ) = A( 1, 1 ) * A( 2, 2 ) - A( 1, 2 ) * A( 2, 1 );
//InvA = InvA / Det;

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( 3 );
	mTemp[ 0, 0 ] = (  amA[ 1, 1 ] * amA[ 2, 2 ] - amA[ 1, 2 ] * amA[ 2, 1 ] ) / dDet;
	mTemp[ 0, 1 ] = ( -amA[ 0, 1 ] * amA[ 2, 2 ] + amA[ 0, 2 ] * amA[ 2, 1 ] ) / dDet;
	mTemp[ 0, 2 ] = (  amA[ 0, 1 ] * amA[ 1, 2 ] - amA[ 0, 2 ] * amA[ 1, 1 ] ) / dDet;
	mTemp[ 1, 0 ] = ( -amA[ 1, 0 ] * amA[ 2, 2 ] + amA[ 1, 2 ] * amA[ 2, 0 ] ) / dDet;
	mTemp[ 1, 1 ] = (  amA[ 0, 0 ] * amA[ 2, 2 ] - amA[ 0, 2 ] * amA[ 2, 0 ] ) / dDet;
	mTemp[ 1, 2 ] = ( -amA[ 0, 0 ] * amA[ 1, 2 ] + amA[ 0, 2 ] * amA[ 1, 0 ] ) / dDet;
	mTemp[ 2, 0 ] = (  amA[ 1, 0 ] * amA[ 2, 1 ] - amA[ 1, 1 ] * amA[ 2, 0 ] ) / dDet;
	mTemp[ 2, 1 ] = ( -amA[ 0, 0 ] * amA[ 2, 1 ] + amA[ 0, 1 ] * amA[ 2, 0 ] ) / dDet;
	mTemp[ 2, 2 ] = (  amA[ 0, 0 ] * amA[ 1, 1 ] - amA[ 0, 1 ] * amA[ 1, 0 ] ) / dDet;


	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Inverse6( CFloatMatrix^ amX )
{
//A = X( 1 : 3, 1 : 3 );
//B = X( 1 : 3, 4 : 6 );
//C = X( 4 : 6, 1 : 3 );
//D = X( 4 : 6, 4 : 6 );
	CFloatMatrix^ mA = amX[ "0:2", "0:2" ];
	CFloatMatrix^ mB = amX[ "0:2", "3:5" ];
	CFloatMatrix^ mC = amX[ "3:5", "0:2" ];
	CFloatMatrix^ mD = amX[ "3:5", "3:5" ];

//InvA = inv( A );
//InvB = inv( B );
//InvC = inv( C );
//InvD = inv( D );
	CFloatMatrix^ mInvA = CFloatMatrix::Inverse3( mA );
	CFloatMatrix^ mInvB = CFloatMatrix::Inverse3( mB );
	CFloatMatrix^ mInvC = CFloatMatrix::Inverse3( mC );
	CFloatMatrix^ mInvD = CFloatMatrix::Inverse3( mD );

//E = A - B * InvD * C;
//InvE = inv( E );
	CFloatMatrix^ mInvE = CFloatMatrix::Inverse3( mA - mB * mInvD * mC );


//Temp1 = B * InvD;
//Temp2 = InvD * C;
//Temp3 = InvE * Temp1;
	CFloatMatrix^ mTemp1 = mB * mInvD;
	CFloatMatrix^ mTemp2 = mInvD * mC;
	CFloatMatrix^ mTemp3 = mInvE * mTemp1;

//InvX = [ InvE, - Temp3; - Temp2 * InvE, InvD + Temp2 * Temp3 ];
	CFloatMatrix^ mInvF = - mTemp3;
	CFloatMatrix^ mInvG = - mTemp2 * mInvE;
	CFloatMatrix^ mInvH = mInvD + mTemp2 * mTemp3;

	CFloatMatrix^ mInvX = gcnew CFloatMatrix( 6, 6 );
	for( int iRow = 0; iRow < 3; iRow ++ )
	{
		for( int iCol = 0; iCol < 3; iCol ++ )
		{
			mInvX[ iRow, iCol ] = mInvE[ iRow, iCol ];
			mInvX[ iRow, iCol + 3 ] = mInvF[ iRow, iCol ];
			mInvX[ iRow + 3, iCol ] = mInvG[ iRow, iCol ];
			mInvX[ iRow + 3, iCol + 3 ] = mInvH[ iRow, iCol ];
		}
	}

	delete mA;
	delete mB;
	delete mC;
	delete mD;

	delete mInvA;
	delete mInvB;
	delete mInvC;
	delete mInvD;

	delete mInvE;
	delete mInvF;
	delete mInvG;
	delete mInvH;

	delete mTemp1;
	delete mTemp2;
	delete mTemp3;

	GC::Collect( );

	return mInvX;
}

float CFloatMatrix::BiLinearInterp( CFloatMatrix^ avPt3TL, CFloatMatrix^ avPt3TR, CFloatMatrix^ avPt3BL, CFloatMatrix^ avPt3BR, CFloatMatrix^ % apvPt3Interp )
{
//% f(x,y) = ax + by + cxy + d

//B = zeros( 4, 1 );
	CFloatMatrix^ mA = gcnew CFloatMatrix( 4 );
	CFloatMatrix^ vB = gcnew CFloatMatrix( 4, 1 );
//A1 = [ Pt3TL( 1 ), Pt3TL( 2 ), Pt3TL( 1 ) * Pt3TL( 2 ), 1 ];
//B( 1 ) = Pt3TL( 3 );
	mA[ 0, 0 ] = avPt3TL[ 0, 0 ];
	mA[ 0, 1 ] = avPt3TL[ 1, 0 ];
	mA[ 0, 2 ] = avPt3TL[ 0, 0 ] * avPt3TL[ 1, 0 ];
	mA[ 0, 3 ] = 1;
	vB[ 0, 0 ] = avPt3TL[ 2, 0 ];

//A2 = [ Pt3TR( 1 ), Pt3TR( 2 ), Pt3TR( 1 ) * Pt3TR( 2 ), 1 ];
//B( 2 ) = Pt3TR( 3 );
	mA[ 1, 0 ] = avPt3TR[ 0, 0 ];
	mA[ 1, 1 ] = avPt3TR[ 1, 0 ];
	mA[ 1, 2 ] = avPt3TR[ 0, 0 ] * avPt3TR[ 1, 0 ];
	mA[ 1, 3 ] = 1;
	vB[ 1, 0 ] = avPt3TR[ 2, 0 ];

//A3 = [ Pt3BL( 1 ), Pt3BL( 2 ), Pt3BL( 1 ) * Pt3BL( 2 ), 1 ];
//B( 3 ) = Pt3BL( 3 );
	mA[ 2, 0 ] = avPt3BL[ 0, 0 ];
	mA[ 2, 1 ] = avPt3BL[ 1, 0 ];
	mA[ 2, 2 ] = avPt3BL[ 0, 0 ] * avPt3BL[ 1, 0 ];
	mA[ 2, 3 ] = 1;
	vB[ 2, 0 ] = avPt3BL[ 2, 0 ];

//A4 = [ Pt3BR( 1 ), Pt3BR( 2 ), Pt3BR( 1 ) * Pt3BR( 2 ), 1 ];
//B( 4 ) = Pt3BR( 3 );
	mA[ 3, 0 ] = avPt3BR[ 0, 0 ];
	mA[ 3, 1 ] = avPt3BR[ 1, 0 ];
	mA[ 3, 2 ] = avPt3BR[ 0, 0 ] * avPt3BR[ 1, 0 ];
	mA[ 3, 3 ] = 1;
	vB[ 3, 0 ] = avPt3BR[ 2, 0 ];

//A = [ A1; A2; A3; A4 ];

//% A * Coef = B
//Coef = GEPP( A, B );
	CFloatMatrix^ vCoef = SolveEquation( mA, vB );


//z = Coef( 1 ) * Pt2( 1 ) + Coef( 2 ) * Pt2( 2 ) + Coef( 3 ) * Pt2( 1 ) * Pt2( 2 ) + Coef( 4 );
	float dZ = vCoef[ 0, 0 ] * apvPt3Interp[ 0, 0 ];
	dZ += vCoef[ 1, 0 ] * apvPt3Interp[ 1, 0 ];
	dZ += vCoef[ 2, 0 ] * apvPt3Interp[ 0, 0 ] * apvPt3Interp[ 1, 0 ];
	dZ += vCoef[ 3, 0 ];

	if( apvPt3Interp->Rows >= 3 )
	{
		apvPt3Interp[ 2, 0 ] = dZ;
	}

	delete mA;
	delete vB;
	delete vCoef;

	return dZ;
}

float CFloatMatrix::BiLinearInterp2( float adZ1, float adZ2, float adZ3, float adZ4, float adX, float adY )
{
	float dX0 = adX - ( float )Math::Floor( adX );
	float dY0 = adY - ( float )Math::Floor( adY );

	float dD = adZ1;
	float dA = adZ2 - dD;
	float dB = adZ3 - dD;
	float dC = adZ4 - dA - dB - dD;

	return( dA * dX0 + dB * dY0 + dC * dX0 * dY0 + dD );
}

int CFloatMatrix::ART_Fact( CFloatMatrix^ amM, CFloatMatrix^ % apmA, CFloatMatrix^ % apmR, CFloatMatrix^ % apmT, float * apdM34 )
{
//ml1 = pml( 1, 1 : 3 )';
//ml2 = pml( 2, 1 : 3 )';
//ml3 = pml( 3, 1 : 3 )';
	if( ( 3 != amM->Rows ) && ( 4 != amM->Columns ) )
	{
		return -1;
	}

	CFloatMatrix^ vM1 = amM[ "0", "0:2" ]->Transpose( );
	CFloatMatrix^ vM2 = amM[ "1", "0:2" ]->Transpose( );
	CFloatMatrix^ vM3 = amM[ "2", "0:2" ]->Transpose( );


//ml14 = pml( 1, 4 );
//ml24 = pml( 2, 4 );
	float dM14 = amM[ 0, 3 ];
	float dM24 = amM[ 1, 3 ];

//ml34 = 1 / sqrt( ml3' * ml3 );
	float dM34 = 1.0F / VectorNorm( vM3, 2 );
	* apdM34 = dM34;


//r3 = ml34 * ml3;
	CFloatMatrix^ vR3 = vM3 * dM34;

//u0 = m34 * m1' * r3;
//v0 = m34 * m2' * r3;
	float dU0 = dM34 * DotProduct( vM1, vR3 );
	float dV0 = dM34 * DotProduct( vM2, vR3 );

//ay = m34 * norm( CrossProduct( m2, r3 ), 2 );
//r2 = ( m34 * m2 - v0 * r3 ) / ay;
	float dAy = dM34 * VectorNorm( CrossProduct( vM2, vR3 ), 2 );
	CFloatMatrix^ vR2 = vM2 * ( dM34 / dAy ) + ( vR3 * ( -dV0 / dAy ) );


//s = m34 * m1' * r2;
	float dS = dM34 * DotProduct( vM1, vR2 );

//ax = m34 * norm( CrossProduct( CrossProduct( m1', r3' ), CrossProduct( r2', r3' ) ), 2 );
	float dAx = dM34 * VectorNorm( CrossProduct( CrossProduct( vM1, vR3 ), CrossProduct( vR2, vR3 ) ), 2 );

//r1 = ( m34 * m1 - u0 * r3 - s * r2 ) / ax;
	CFloatMatrix^ vR1 = vM1 * ( dM34 / dAx ) + ( vR3 * ( -dU0 / dAx ) ) + ( vR2 * ( -dS / dAx ) );

//tz = m34;
	float dTz = dM34;
//ty = ( m34 * m24 - v0 * tz ) / ay;
	float dTy = ( dM34 * dM24 - dV0 * dTz ) / dAy;
//tx = ( m34 * m14 - u0 * tz - s* ty ) / ax;
	float dTx = ( dM34 * dM14 - dU0 * dTz - dS * dTy ) / dAx;

//A = [ ax, s, u0; 0, ay, v0; 0, 0, 1 ];
//R = [ r1'; r2'; r3' ];
//T = [ tx; ty; tz ];


	delete apmA;
	apmA = gcnew CFloatMatrix( 3 );
	apmA[ 0, 0 ] = dAx;
	apmA[ 0, 1 ] = dS;
	apmA[ 0, 2 ] = dU0;
	apmA[ 1, 0 ] = 0;
	apmA[ 1, 1 ] = dAy;
	apmA[ 1, 2 ] = dV0;
	apmA[ 2, 0 ] = 0;
	apmA[ 2, 1 ] = 0;
	apmA[ 2, 2 ] = 1;

	delete apmR;
	apmR = gcnew CFloatMatrix( 3 );
	apmR[ "0", "0:2" ] = vR1->Transpose( );
	apmR[ "1", "0:2" ] = vR2->Transpose( );
	apmR[ "2", "0:2" ] = vR3->Transpose( );

	delete apmT;
	apmT = gcnew CFloatMatrix( 3, 1 );
	apmT[ 0, 0 ] = dTx;
	apmT[ 1, 0 ] = dTy;
	apmT[ 2, 0 ] = dTz;

	delete vM1;
	delete vM2;
	delete vM3;

	delete vR1;
	delete vR2;
	delete vR3;

	return 0;
}

float CFloatMatrix::Sum( int aiCenterX, int aiCenterY, int aiRadius )
{
	if( ( aiCenterX < 0 ) || ( aiCenterX > ( m_pv_iColumns - 1 ) ) )
	{
		return 0;
	}

	if( ( aiCenterY < 0 ) || ( aiCenterY > ( m_pv_iRows - 1 ) ) )
	{
		return 0;
	}

	int iFromX = aiCenterX - aiRadius;
	if( iFromX < 0 )
	{
		iFromX = 0;
	}

	int iToX = aiCenterX + aiRadius;
	if( iToX > ( m_pv_iColumns - 1 ) )
	{
		iToX = m_pv_iColumns - 1;
	}

	int iFromY = aiCenterY - aiRadius;
	if( iFromY < 0 )
	{
		iFromY = 0;
	}

	int iToY = aiCenterY + aiRadius;
	if( iToY > ( m_pv_iRows - 1 ) )
	{
		iToY = m_pv_iRows - 1;
	}

	float dSum = 0;
	for( int iRow = iFromY; iRow <= iToY; iRow ++ )
	{
		for( int iCol = iFromX; iCol <= iToX; iCol ++ )
		{
			dSum += m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return dSum;

}

float CFloatMatrix::Sum( CFloatMatrix^ amMatrix, int aiCenterX, int aiCenterY, int aiRadius )
{
	if( ( aiCenterX < 0 ) || ( aiCenterX > ( amMatrix->Columns - 1 ) ) )
	{
		return 0;
	}

	if( ( aiCenterY < 0 ) || ( aiCenterY > ( amMatrix->Rows - 1 ) ) )
	{
		return 0;
	}

	int iFromX = aiCenterX - aiRadius;
	if( iFromX < 0 )
	{
		iFromX = 0;
	}

	int iToX = aiCenterX + aiRadius;
	if( iToX > ( amMatrix->Columns - 1 ) )
	{
		iToX = amMatrix->Columns - 1;
	}

	int iFromY = aiCenterY - aiRadius;
	if( iFromY < 0 )
	{
		iFromY = 0;
	}

	int iToY = aiCenterY + aiRadius;
	if( iToY > ( amMatrix->Rows - 1 ) )
	{
		iToY = amMatrix->Rows - 1;
	}

	float dSum = 0;
	for( int iRow = iFromY; iRow <= iToY; iRow ++ )
	{
		for( int iCol = iFromX; iCol <= iToX; iCol ++ )
		{
			dSum += amMatrix[ iRow, iCol ];
		}
	}

	return dSum;

}

float CFloatMatrix::Sum( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	float dSum = 0;
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dSum += amMatrix[ iRow, iCol ];
		}
	}

	return dSum;

}

float CFloatMatrix::Sum( void )
{
	float dSum = 0;
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dSum += m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return dSum;

}

void CFloatMatrix::SaveAsM( String^ astrMatrixName, String^ astrFileName )
{
	CFloatMatrix::SaveAsM( this, astrMatrixName, astrFileName );
}

void CFloatMatrix::SaveAsM( CFloatMatrix^ amData, String^ astrMatrixName, String^ astrFileName )
{
	System::IO::StreamWriter^ swMatrix = gcnew System::IO::StreamWriter( System::IO::File::Open( astrFileName, System::IO::FileMode::Create ) ); 

	swMatrix->Write( astrMatrixName + " = [ " );

	int iRows = amData->Rows;
	int iCols = amData->Columns;
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			swMatrix->Write( amData[ iRow, iCol ].ToString( ) );
			if( iCol < iCols - 1 )
			{
				swMatrix->Write( ", " );
			}
			else
			{
				swMatrix->Write( "; \n" );
			}
		}
	}

	swMatrix->WriteLine( " ];" );

	swMatrix->Close( );
	delete swMatrix;
}

void CFloatMatrix::SaveAsBinary( String^ astrFileName )
{
	CFloatMatrix::SaveAsBinary( this, astrFileName );
}

void CFloatMatrix::SaveAsBinary( CFloatMatrix^ amData, String^ astrFileName )
{
	System::IO::BinaryWriter^ bwMatrix = gcnew System::IO::BinaryWriter( System::IO::File::Open( astrFileName, System::IO::FileMode::Create ) ); 

	Int32 iRows = amData->Rows;
	Int32 iCols = amData->Columns;

	bwMatrix->Write( iRows );
	bwMatrix->Write( iCols );

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			bwMatrix->Write( amData[ iRow, iCol ] );
		}
	}

	bwMatrix->Close( );
	delete bwMatrix;

}

CFloatMatrix^ CFloatMatrix::LoadFromBinary( String^ astrFileName )
{
	System::IO::BinaryReader^ brMatrix = gcnew System::IO::BinaryReader( System::IO::File::Open( astrFileName, System::IO::FileMode::Open ) ); 

	Int32 iRows = brMatrix->ReadInt32( );
	Int32 iCols = brMatrix->ReadInt32( );

	CFloatMatrix^ mTemp = gcnew CFloatMatrix( iRows, iCols );

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = brMatrix->ReadSingle( );
		}
	}

	brMatrix->Close( );
	delete brMatrix;

	return mTemp;
}

int CFloatMatrix::GetProcessorNumber( void )
{
	return CFloatMatrix::ConvertStringToInt( System::Environment::GetEnvironmentVariable( "NUMBER_OF_PROCESSORS" ) );
}

float CFloatMatrix::Determinant( void )
{
	return CFloatMatrix::Determinant( this );
}

float CFloatMatrix::Determinant( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	if( iRows != iCols )
	{
		throw gcnew Exception( "Invalid Dimension !" );
		return 0;
	}

	switch( iRows )
	{
	case 1:
		return amMatrix[ 0, 0 ];
	case 2:
		return Determinant2( amMatrix );
	case 3:
		return Determinant3( amMatrix );
	}

	CFloatMatrix^ mA = amMatrix->Copy( );

	float dDet = 1;
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		float dMax = Math::Abs( mA[ iRow, iRow ] );
		int iLine = iRow;
		for( int iDiagCount = iRow + 1; iDiagCount < iRows; iDiagCount ++ )
		{
			float dTemp = Math::Abs( mA[ iDiagCount, iRow ] );
			if( dTemp > dMax )
			{
				dMax = dTemp;
				iLine = iDiagCount;
			}
		}
		if( 0 == dMax )
		{
			return 0;
		}
		if( iRow != iLine )
		{
			CFloatMatrix^ mTemp = mA[ iRow.ToString( ), ":" ];
			mA[ iRow.ToString( ), ":" ] = mA[ iLine.ToString( ), ":" ];
			mA[ iLine.ToString( ), ":" ] = mTemp;
			dDet = dDet * -1;
			delete mTemp;
		}

		dDet = dDet * mA[ iRow, iRow ];
		mA[ iRow.ToString( ), ":" ] = mA[ iRow.ToString( ), ":" ] / mA[ iRow, iRow ];
		for( int iCol = iRow + 1; iCol < iCols; iCol ++ )
		{
			if( 0 == mA[ iCol, iRow ] )
			{
				continue;
			}
			mA[ iCol.ToString( ), ":" ] = mA[ iCol.ToString( ), ":" ] - mA[ iRow.ToString( ), ":" ] * mA[ iCol, iRow ];
		}
	}

	delete mA;

	return dDet;

}

void CFloatMatrix::UniformRandom( void )
{
	float dX = ( float )System::DateTime::Now.Second;
	int    iA = 69069;
	int    iC = 1;
	unsigned long ulM = ( unsigned long )( Math::Pow( 2, 32 )  - 1 );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dX = ( float )( ( ( unsigned long )( iA * dX + iC + 0.5F ) ) % ulM );
			this[ iRow, iCol ] = dX / ulM;
		}
	}
}

CFloatMatrix^ CFloatMatrix::UniformRandom( int aiRows, int aiCols )
{
	CFloatMatrix^ mTemp = gcnew CFloatMatrix( aiRows, aiCols );

	float dX = ( float )System::DateTime::Now.Second;
	int    iA = 69069;
	int    iC = 1;
	unsigned long ulM = ( unsigned long )( Math::Pow( 2, 32 )  - 1 );

	for( int iRow = 0; iRow < aiRows; iRow ++ )
	{
		for( int iCol = 0; iCol < aiCols; iCol ++ )
		{
			dX = ( float )( ( ( unsigned long )( iA * dX + iC + 0.5F ) ) % ulM );
			mTemp[ iRow, iCol ] = dX / ulM;
		}
	}

	return mTemp;
}

CFloatMatrix^ CFloatMatrix::Inverse( void )
{
	return CFloatMatrix::Inverse( this );
}

CFloatMatrix^ CFloatMatrix::Inverse( CFloatMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	if( iRows != iCols )
	{
		throw gcnew Exception( "Invalid Dimension !" );
		return nullptr;
	}

	switch( iRows )
	{
	case 1:
		if( 0 == amMatrix[ 0, 0 ] )
		{
			throw gcnew Exception( "Determinant is zero !" );;
			return nullptr;
		}
		else
		{
			CFloatMatrix^ mTemp = gcnew CFloatMatrix( 1, 1 );
			mTemp[ 0, 0 ] = 1 / amMatrix[ 0, 0 ];
			return mTemp;
		}
		break;
	case 2:
		return CFloatMatrix::Inverse2( amMatrix );
	case 3:
		return CFloatMatrix::Inverse3( amMatrix );
	}

	CFloatMatrix^ mA = gcnew CFloatMatrix( iRows, iRows * 2 );
	mA[ ":", "0:" + ( iRows - 1 ).ToString( ) ] = amMatrix[ ":", ":" ];
	mA[ ":", iRows.ToString( ) + ":" + ( 2 * iRows - 1 ).ToString( ) ] = CFloatMatrix::Identity( iRows );
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		float dMax = Math::Abs( mA[ iRow, iRow ] );
		int iLine = iRow;
		for( int iDiagCount = iRow + 1; iDiagCount < iRows; iDiagCount ++ )
		{
			float dTemp = Math::Abs( mA[ iDiagCount, iRow ] );
			if( dTemp > dMax )
			{
				dMax = dTemp;
				iLine = iDiagCount;
			}
		}
		if( 0 == dMax )
		{
			throw gcnew Exception( "Determinant is zero !" );;
			return nullptr;
		}
		if( iRow != iLine )
		{
			CFloatMatrix^ mTemp = mA[ iRow.ToString( ), ":" ];
			mA[ iRow.ToString( ), ":" ] = mA[ iLine.ToString( ), ":" ];
			mA[ iLine.ToString( ), ":" ] = mTemp;
			delete mTemp;
		}

		mA[ iRow.ToString( ), ":" ] = mA[ iRow.ToString( ), ":" ] / mA[ iRow, iRow ];
		for( int iCol = 0; iCol < iRows; iCol ++ )
		{
			if( ( iRow == iCol ) || ( 0 == mA[ iCol, iRow ] ) )
			{
				continue;
			}
			mA[ iCol.ToString( ), ":" ] = mA[ iCol.ToString( ), ":" ] - mA[ iRow.ToString( ), ":" ] * mA[ iCol, iRow ];
		}
	}

	CFloatMatrix^ mTemp = mA[ ":", iRows.ToString( ) + ":" + ( 2 * iRows - 1 ).ToString( ) ];
	delete mA;

	return mTemp;

}

void CFloatMatrix::House( CFloatMatrix^ avX, CFloatMatrix^ % avV, float & adB )
{
	delete avV;
	avV = nullptr;
	adB = 0;

	int iRows = avX->Rows;
	int iCols = avX->Columns;
	if( ( 1 != iRows ) && ( 1 != iCols ) )
	{
		return;
	}

	CFloatMatrix^ vX;
	if( 1 == iRows )
	{
		vX = avX->Transpose( )->Copy( );
	}
	else
	{
		vX = avX->Copy( );
	}

	int iN = vX->Length;
	CFloatMatrix^ vTemp = vX[ "1:" + ( iN - 1 ).ToString( ), "0" ];
	float dA = ( vTemp->Transpose( ) * vTemp )[ 0, 0 ];

	avV = vX->Copy( );
	avV[ 0 ] = 1;
	if( 0 == dA )
	{
		adB = 0;
	}
	else
	{
		float dX0 = vX[ 0 ];
		float dV0;
		float dU = ( float )Math::Sqrt( dX0 * dX0 + dA );
		if( dX0 <= 0 )
		{
			dV0 = dX0 - dU;
		}
		else
		{
			dV0 = dX0 + dU;
		}
		avV[ 0 ] = dV0;

		adB = 2 * dV0 * dV0 / ( dA + dV0 * dV0 );
		avV = avV / dV0;
	}
}

void CFloatMatrix::HouseholderQR( CFloatMatrix^ % amQ, CFloatMatrix^ % amR )
{
	CFloatMatrix::HouseholderQR( this, amQ, amR );
}

void CFloatMatrix::HouseholderQR( CFloatMatrix^ amA, CFloatMatrix^ % amQ, CFloatMatrix^ % amR )
{
	delete amQ;
	delete amR;

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ mR = amA->Copy( );
	CFloatMatrix^ mQ = CFloatMatrix::Identity( iRows );

	int iN = Math::Min( iRows - 1, iCols );

	CFloatMatrix^ vV;
	float dB;
	String^ strToEnd1 = ":" + ( iRows - 1 ).ToString( );
	String^ strToEnd2 = ":" + ( iCols - 1 ).ToString( );
	for( int iCount = 0; iCount < iN; iCount ++ )
	{
		CFloatMatrix::House( mR[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) ], vV, dB );
		CFloatMatrix^ mTemp = dB * vV * vV->Transpose( );

		CFloatMatrix^ mQTemp = mQ[ iCount.ToString( ) + strToEnd1, ":" ];
		mQ[ iCount.ToString( ) + strToEnd1, ":" ] = mQTemp - mTemp * mQTemp;
		delete mQTemp;

		CFloatMatrix^ mRTemp = mR[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ];
		mR[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ] = mRTemp - mTemp * mRTemp;
		mR[ ( iCount + 1 ).ToString( ) + strToEnd1, iCount.ToString( ) ] = 0;
		delete mRTemp;

		delete mTemp;
	}

	delete vV;

	amQ = mQ->Transpose( )->Copy( );
	amR = mR;

	delete mQ;
}

void CFloatMatrix::Bidiagonalize( CFloatMatrix^ % amU, CFloatMatrix^ % amB, CFloatMatrix^ % amV ) // A = U * B * V'; B is bidiagonal;
{
	CFloatMatrix::Bidiagonalize( this, amU, amB, amV );
}

void CFloatMatrix::Bidiagonalize( CFloatMatrix^ amA, CFloatMatrix^ % amU, CFloatMatrix^ % amB, CFloatMatrix^ % amV ) // A = U * B * V'; B is bidiagonal;
{
	delete amU;
	delete amB;
	delete amV;

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ mB = amA->Copy( );
	CFloatMatrix^ mU = CFloatMatrix::Identity( iRows );
	CFloatMatrix^ mV = CFloatMatrix::Identity( iCols );

	int iN = Math::Min( iRows - 1, iCols );

	CFloatMatrix^ vV;
	float dB;
	String^ strToEnd1 = ":" + ( iRows - 1 ).ToString( );
	String^ strToEnd2 = ":" + ( iCols - 1 ).ToString( );
	for( int iCount = 0; iCount < iN; iCount ++ )
	{
		CFloatMatrix::House( mB[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) ], vV, dB );
		CFloatMatrix^ mTemp = dB * vV * vV->Transpose( );

		CFloatMatrix^ mUTemp = mU[ iCount.ToString( ) + strToEnd1, ":" ];
		mU[ iCount.ToString( ) + strToEnd1, ":" ] = mUTemp - mTemp * mUTemp;
		delete mUTemp;

		CFloatMatrix^ mBTemp = mB[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ];
		mB[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ] = mBTemp - mTemp * mBTemp;
		mB[ ( iCount + 1 ).ToString( ) + strToEnd1, iCount.ToString( ) ] = 0;
		delete mBTemp;

		delete mTemp;
	    
		if( iCount < iCols - 2 )
		{
			CFloatMatrix::House( mB[ iCount.ToString( ), ( iCount + 1 ).ToString( ) + strToEnd2 ], vV, dB );
			CFloatMatrix^ mTemp = dB * vV * vV->Transpose( );

			CFloatMatrix^ mVTemp = mV[ ( iCount + 1 ).ToString( ) + strToEnd2, ":" ];
			mV[ ( iCount + 1 ).ToString( ) + strToEnd2, ":" ] = mVTemp - mTemp * mVTemp;
			delete mVTemp;

			CFloatMatrix^ mBTemp = mB[ iCount.ToString( ) + strToEnd1, ( iCount + 1 ).ToString( ) + strToEnd2 ];
			mB[ iCount.ToString( ) + strToEnd1, ( iCount + 1 ).ToString( ) + strToEnd2 ] = mBTemp - mBTemp * mTemp;
			mB[ iCount.ToString( ), ( iCount + 2 ).ToString( ) + strToEnd2 ] = 0;
			delete mBTemp;

			delete mTemp;
		}
	}

	if( iCols > iRows + 1 )
	{
		CFloatMatrix::House( mB[ ( iRows - 1 ).ToString( ), iRows.ToString( ) + strToEnd2 ], vV, dB );
		CFloatMatrix^ mTemp = dB * vV * vV->Transpose( );

		CFloatMatrix^ mVTemp = mV[ iRows.ToString( ) + strToEnd2, ":" ];
		mV[ iRows.ToString( ) + strToEnd2, ":" ] = mVTemp - mTemp * mVTemp;
		delete mVTemp;

		CFloatMatrix^ mBTemp = mB[ ( iRows - 1 ).ToString( ), iRows.ToString( ) + strToEnd2 ];
		mB[ ( iRows - 1 ).ToString( ), iRows.ToString( ) + strToEnd2 ] = mBTemp - mBTemp * mTemp;
		mB[ ( iRows - 1 ).ToString( ), ( iRows + 1 ).ToString( ) + strToEnd2 ] = 0;
		delete mBTemp;

		delete mTemp;
	}

	delete vV;

	amU = mU->Transpose( )->Copy( );
	amB = mB;
	amV = mV->Transpose( )->Copy( );

	delete mU;
	delete mV;

}

float CFloatMatrix::WilkinsonShift( CFloatMatrix^ amT )
{
	float dA = amT[ 0, 0 ];
	float dB = amT[ 0, 1 ];
	float dC = amT[ 1, 0 ];
	float dD = amT[ 1, 1 ];
	float dE = dA + dD;
	float dSqrtDelta = ( float )Math::Sqrt( dE * dE - 4 * ( dA * dD - dB * dC ) );
	float dU1 = ( dE + dSqrtDelta ) / 2;
	float dU2 = ( dE - dSqrtDelta ) / 2;
	if( Math::Abs( dD - dU1 ) < Math::Abs( dD - dU2 ) )
	{
		return dU1;
	}
	else
	{
		return dU2;
	}
}

void CFloatMatrix::SortEigenvalue( CFloatMatrix^ avEigValue, CFloatMatrix^ amEigVector, float adEps, CFloatMatrix^ % avNewEigValue, CFloatMatrix^ % amNewEigVector, int & aiNonZero )
{
	delete avNewEigValue;
	delete amNewEigVector;

	int iLength = avEigValue->Length;

	avNewEigValue = avEigValue->Copy( );
	amNewEigVector = amEigVector->Copy( );

	for( int iFirstCount = 0; iFirstCount < iLength - 1; iFirstCount ++ )
	{
		float dMax = avNewEigValue[ iFirstCount ];
		int iIdx = iFirstCount;
		for( int iSecondCount = iFirstCount + 1; iSecondCount < iLength; iSecondCount ++ )
		{
			float dTemp = Math::Abs( avNewEigValue[ iSecondCount ] );
			if( dTemp > dMax )
			{
				dMax = dTemp;
				iIdx = iSecondCount;
			}
		}

		if( iIdx != iFirstCount )
		{
			avNewEigValue[ iIdx ] = avNewEigValue[ iFirstCount ];
			avNewEigValue[ iFirstCount ] = dMax;
	        
			CFloatMatrix^ mTemp = amNewEigVector[ ":", iIdx.ToString( ) ];
			amNewEigVector[ ":", iIdx.ToString( ) ] = amNewEigVector[ ":", iFirstCount.ToString( ) ];
			amNewEigVector[ ":", iFirstCount.ToString( ) ] = mTemp;
			delete mTemp;
		}
	}

	aiNonZero = 0;
	for( int iCount = iLength - 1; iCount >= 0; iCount -- )
	{
		if( adEps <= Math::Abs( avNewEigValue[ iCount ] ) )
		{
			aiNonZero = iCount + 1;
			return;
		}
	}

}

CFloatMatrix^ CFloatMatrix::GEtLeftEigenVector( CFloatMatrix^ amA, CFloatMatrix^ amRightEigenVector )
{

	int iRows = amA->Rows;
	int iCols = amRightEigenVector->Columns;

	CFloatMatrix^ mLeftEigenVector = gcnew CFloatMatrix( iRows, iCols );
	for( int iCount = 0; iCount < iCols; iCount ++ )
	{
		CFloatMatrix^ mTemp = amA * amRightEigenVector[ ":", iCount.ToString( ) ];
		mLeftEigenVector[ ":", iCount.ToString( ) ] = mTemp / CFloatMatrix::VectorNorm( mTemp, 2 );
		delete mTemp;
	}

	if( iRows <= iCols )
	{
		return mLeftEigenVector;
	}

	CFloatMatrix^ mTemp = CFloatMatrix::Identity( iRows );
	mTemp[ ":", "0:" + ( iCols - 1 ).ToString( ) ] = mLeftEigenVector;
	for( int iCount = iCols; iCount < iRows; iCount ++ )
	{
		for( int iSumCount = 0; iSumCount <= iCount - 1; iSumCount ++ ) // j = 1 : i - 1
		{
			mTemp[ ":", iCount.ToString( ) ] = mTemp[ ":", iCount.ToString( ) ] - ( ( mTemp[ ":", iCount.ToString( ) ] )->Transpose( ) * mTemp[ ":", iSumCount.ToString( ) ] )[ 0, 0 ] * mTemp[ ":", iSumCount.ToString( ) ];
		}
		mTemp[ ":", iCount.ToString( ) ] = mTemp[ ":", iCount.ToString( ) ] / CFloatMatrix::VectorNorm( mTemp[ ":", iCount.ToString( ) ], 2 );
	}

	delete mLeftEigenVector;

	return mTemp;

}

void CFloatMatrix::SVD( CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV )
{
	CFloatMatrix::SVD( this, 1e-8F, amU, amS, amV );
}

void CFloatMatrix::SVD( float adEps, CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV )
{
	CFloatMatrix::SVD( this, adEps, amU, amS, amV );
}

void CFloatMatrix::SVD( CFloatMatrix^ amA, CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV )
{
	CFloatMatrix::SVD( amA, 1e-8F, amU, amS, amV );
}

void CFloatMatrix::SVD( CFloatMatrix^ amA, float adEps, CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV )
{

	delete amU;
	delete amS;
	delete amV;

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ mU1;
	CFloatMatrix^ mB;
	CFloatMatrix^ mV1;
	CFloatMatrix::Bidiagonalize( amA, mU1, mB, mV1 );

	CFloatMatrix^ mB1;
	if( iRows > iCols )
	{
		mB1 = mB->Transpose( ) * mB;
	}
	else
	{
		mB1 = mB * mB->Transpose( );
	}

	int iNN = Math::Min( iRows, iCols );

	CFloatMatrix^ mEVector = CFloatMatrix::Identity( iNN );

	for( int iCount = 0; iCount < iNN - 1; iCount ++ )
	{
		int iN = iNN - iCount - 1;
		while( Math::Abs( mB1[ iN, iN - 1 ] ) >= adEps )
		{

			float dU = CFloatMatrix::WilkinsonShift( mB1[ ( iN - 1 ).ToString( ) + ":" + iN.ToString( ), ( iN - 1 ).ToString( ) + ":" + iN.ToString( ) ] );

			for( int iDiagCount = 0; iDiagCount <= iN; iDiagCount ++ )
			{
				mB1[ iDiagCount, iDiagCount ] = mB1[ iDiagCount, iDiagCount ] - dU;
			}
			CFloatMatrix^ mQ;
			CFloatMatrix^ mR;
			CFloatMatrix::HouseholderQR( mB1[ "0:" + iN.ToString( ), "0:" + iN.ToString( ) ], mQ, mR );
			mB1[ "0:" + iN.ToString( ), "0:" + iN.ToString( ) ] = mR * mQ;
			delete mR;
			for( int iDiagCount = 0; iDiagCount <= iN; iDiagCount ++ )
			{
				mB1[ iDiagCount, iDiagCount ] = mB1[ iDiagCount, iDiagCount ] + dU;
			}
			mEVector[ ":", "0:" + iN.ToString( ) ] = mEVector[ ":", "0:" + iN.ToString( ) ] * mQ;
			delete mQ;
		}
	}

	CFloatMatrix^ vEigValue = gcnew CFloatMatrix( mB1->Rows, 1 );
	for( int iCount = 0; iCount < mB1->Rows; iCount ++ )
	{
		float dTemp = mB1[ iCount, iCount ];
		if( ( dTemp < adEps ) || ( dTemp < 0 ) )
		{
			vEigValue[ iCount ] = 0;
		}
		else
		{
			vEigValue[ iCount ] = ( float )Math::Sqrt( dTemp );
		}
	}

	CFloatMatrix^ vNewEigValue;
	CFloatMatrix^ mU2;
	CFloatMatrix^ mV2;
	int iNonZero;
	if( iRows > iCols )
	{
		CFloatMatrix::SortEigenvalue( vEigValue, mEVector, adEps, vNewEigValue, mV2, iNonZero );
		mU2 = CFloatMatrix::GEtLeftEigenVector( mB, mV2[ ":", "0:" + ( iNonZero - 1 ).ToString( ) ] );
	}
	else
	{
		CFloatMatrix::SortEigenvalue( vEigValue, mEVector, adEps, vNewEigValue, mU2, iNonZero );
		mV2 = CFloatMatrix::GEtLeftEigenVector( mB->Transpose( ), mU2[ ":", "0:" + ( iNonZero - 1 ).ToString( ) ] );
	}

	amS = gcnew CFloatMatrix( iRows, iCols, 0 );
	for( int iCount = 0; iCount < iNN; iCount ++ )
	{
		amS[ iCount, iCount ] = vNewEigValue[ iCount ];
	}

	amU = mU1 * mU2;
	amV = mV1 * mV2;

	delete mU1;
	delete mV1;
	delete mU2;
	delete mV2;
	delete mB;
	delete mB1;
	delete vEigValue;
	delete vNewEigValue;
	delete mEVector;

}

CFloatMatrix^ CFloatMatrix::SolveEquationByInverseMatrix( CFloatMatrix^ amA, CFloatMatrix^ avB )
{
	if( amA->Rows != avB->Rows )
	{
		return nullptr;
	}

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	if( iRows < iCols )
	{
		return nullptr;
	}

	CFloatMatrix^ mA;
	CFloatMatrix^ vB;

	CFloatMatrix^ mAT = amA->Transpose( );

	if( iRows > iCols )
	{
		mA = mAT * amA;
		vB = mAT * avB;
	}
	else
	{
		mA = amA->Copy( );
		vB = avB->Copy( );
	}

	delete mAT;

	CFloatMatrix^ mInvA = mA->Inverse( );
	delete mA;

	if( nullptr == mInvA )
	{
		delete vB;

		return nullptr;
	}
	
	CFloatMatrix^ vX = mInvA * vB;

	delete vB;

	return vX;
}

CFloatMatrix^ CFloatMatrix::ElementAcos( CFloatMatrix^ amA )
{
	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ amNewA = gcnew CFloatMatrix( iRows, iCols, 0.0F );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			amNewA[ iRow, iCol ] = ( float )Math::Acos( amA[iRow, iCol ] );
		}
	}
	return amNewA;
}

CFloatMatrix^ CFloatMatrix::ElementAtan2( CFloatMatrix^ amA, CFloatMatrix^ amB )
{
	if( ( amA->Columns != amB->Columns ) || ( amA->Rows != amB->Rows ) )
	{
		return nullptr;
	}
	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ amNewA = gcnew CFloatMatrix( iRows, iCols, 0.0F );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			amNewA[ iRow, iCol ] = ( float )( Math::Atan2( amA[iRow, iCol ], amB[iRow, iCol ] ) + Math::PI );
		}
	}
	return amNewA;
}

CFloatMatrix^ CFloatMatrix::ElementCos( CFloatMatrix^ amA )
{
	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ amNewA = gcnew CFloatMatrix( iRows, iCols, 0.0F );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			amNewA[ iRow, iCol ] = ( float )Math::Cos( amA[iRow, iCol ] );
		}
	}
	return amNewA;
}

CFloatMatrix^ CFloatMatrix::ElementSin( CFloatMatrix^ amA )
{
	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ amNewA = gcnew CFloatMatrix( iRows, iCols, 0.0F );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			amNewA[ iRow, iCol ] = ( float )Math::Sin( amA[iRow, iCol ] );
		}
	}
	return amNewA;
}

CFloatMatrix^ CFloatMatrix::Abs( CFloatMatrix^ amA )
{
	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CFloatMatrix^ amNewA = gcnew CFloatMatrix( iRows, iCols, 0.0F );
#pragma omp parallel for
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			amNewA[ iRow, iCol ] = ( float )Math::Abs( amA[iRow, iCol ] );
		}
	}
	return amNewA;
}
CFloatMatrix^ CFloatMatrix::ImToCol( CFloatMatrix^ amA, int iR, int iS )
{
	int iCols  = amA->Columns;
	int iRows  = amA->Rows;
	if( iR < 1 || iR > iRows || iS < 1 || iS > iCols )
	{
		return nullptr;
	}
	int iHeight = iR * iS;
	int iWidth = ( iRows - iR + 1 ) * ( iCols - iS + 1 );
	CFloatMatrix^ mNewA = gcnew CFloatMatrix( iHeight, iWidth, float::NaN );
	int iCount = 0;
	for( int iCol = 0; iCol <= ( iCols - iS ); iCol++  )
	{
		for( int iRow = 0; iRow <= ( iRows - iR ); iRow++ )
		{
			int iNum = 0;
			for( int iCol1 = iCol; iCol1 < ( iCol + iS ); iCol1++ )
			{
				for( int iRow1 = iRow; iRow1 < ( iRow + iR ); iRow1++ )
				{
					mNewA[ iNum, iCount ] = amA[ iRow1, iCol1 ];
					iNum++;
				}
			}
			iCount++;
		}
	}
	return mNewA;
}
CFloatMatrix^ CFloatMatrix::MeanColumn( CFloatMatrix^ amA )
{
	int iCols  = amA->Columns;
	int iRows  = amA->Rows;
	if( 0 == iCols || 0 == iRows )
	{
		return nullptr;
	}
	CFloatMatrix^ mNewA = gcnew CFloatMatrix( 1, iCols, float::NaN );
	for( int iCol = 0; iCol < iCols; iCol++ )
	{
		float fSum = 0.0F;
		int iCount = 0;
		for( int iRow = 0; iRow < iRows; iRow++ )
		{
			if( Single::IsNaN( amA[ iRow, iCol ] ) )
			{
				continue;
			}
			fSum += amA[ iRow, iCol ];
			iCount++;
		}
		if( 0 != iCount )
		{
			mNewA[ 0, iCol ] = fSum / ( float )( iCount );
		}
	}
	return mNewA;
}
float CFloatMatrix::Mean2( CFloatMatrix^ amA )
{
	int iCols  = amA->Columns;
	int iRows  = amA->Rows;
	if( 0 == iCols || 0 == iRows )
	{
		return float::NaN;
	}
	float fSum = 0.0F;
	int iCount = 0;
	for( int iRow = 0; iRow < iRows; iRow++ )
	{
		for( int iCol = 0; iCol < iCols; iCol++ )
		{
			if( Single::IsNaN( amA[ iRow, iCol ] ) )
			{
				continue;
			}
			fSum = fSum + amA[ iRow, iCol ];
			iCount++;
		}
	}
	float fOutput = fSum / ( float )( iCount );
	return fOutput;
}

CFloatMatrix^ CFloatMatrix::RemoveNaNWithIdx( CFloatMatrix^ amA, CFloatMatrix^ amIdx, int aiCount )
{
	int iCols  = amA->Columns;
	int iRows  = amA->Rows;
	int iCols2 = amIdx->Columns;
	int iRows2 = amIdx->Rows;
	if( 0 == iCols && 0 == iRows &&  0 == iCols2 && 0 == iRows2 )
	{
		return nullptr;
	}
	if( iCols != iCols2 )
	{
		return nullptr;
	}
	if( 0 == aiCount )
	{
		return amA;
	}

	CFloatMatrix^ mNewA = gcnew CFloatMatrix( iRows, aiCount, 0.0F );
	int iCnt = 0;

	for( int iC = 0; iC < iCols; iC ++ )
	{
		if( 1.0F == amIdx[ 0, iC ] )
		{
			mNewA[ ":", iCnt.ToString( ) ] = amA[ ":", iC.ToString() ];
			iCnt++;
		}
	}


	return mNewA;
}

