#include "StdAfx.h"
#include "Matrix.h"

CMatrix::CMatrix( void )
{
	Initialize( 1, 1, 0, false );
}

CMatrix::CMatrix( String^ astrFileName )
{
	System::IO::BinaryReader^ brMatrix = gcnew System::IO::BinaryReader( System::IO::File::Open( astrFileName, System::IO::FileMode::Open ) ); 

	m_pv_iRows = brMatrix->ReadInt32( );
	m_pv_iColumns = brMatrix->ReadInt32( );

	m_pv_ar2dData = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = brMatrix->ReadDouble( );
		}
	}

	brMatrix->Close( );
	delete brMatrix;
}

CMatrix::CMatrix( CMatrix^ amMatrix )
{
	m_pv_iRows    = amMatrix->Rows;
	m_pv_iColumns = amMatrix->Columns;
	m_pv_ar2dData = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = amMatrix[ iRow, iCol ];
		}
	}
}

CMatrix::CMatrix( CFloatMatrix^ amMatrix )
{
	m_pv_iRows    = amMatrix->Rows;
	m_pv_iColumns = amMatrix->Columns;
	m_pv_ar2dData = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = amMatrix[ iRow, iCol ];
		}
	}
}

CMatrix::CMatrix( int aiDimension )
{
	Initialize( aiDimension, aiDimension, 0, false );
}

CMatrix::CMatrix( int aiRows, int aiColumns )
{
	Initialize( aiRows, aiColumns, 0, false );
}

CMatrix::CMatrix( int aiRows, int aiColumns, double adInitValue )
{
	Initialize( aiRows, aiColumns, adInitValue, true );
}

void CMatrix::Initialize( int aiRows, int aiColumns, double adInitValue, bool abInit )
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
	m_pv_ar2dData = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

	if( ! abInit )
	{
		return;
	}

	Assign( adInitValue );

}


CMatrix::~CMatrix( void )
{
	delete [ ] m_pv_ar2dData;
}


double CMatrix::default::get( int aiRow, int aiCol )
{
	return m_pv_ar2dData[ aiRow, aiCol ];
}

void CMatrix::default::set( int aiRow, int aiCol, double adValue )
{
	m_pv_ar2dData[ aiRow, aiCol ] = adValue;
}

double CMatrix::default::get( int aiCount )
{
	int iRow = aiCount / m_pv_iColumns;
	int iCol = aiCount % m_pv_iColumns;
	return m_pv_ar2dData[ iRow, iCol ];
}

void CMatrix::default::set( int aiCount, double adValue )
{
	int iRow = aiCount / m_pv_iColumns;
	int iCol = aiCount % m_pv_iColumns;
	m_pv_ar2dData[ iRow, iCol ] = adValue;
}

/*
void CMatrix::default::set( double adValue )
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

int CMatrix::FindChar( String^ astrSource, char achTarget )
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

int CMatrix::GetSubscript( String^ astrInt )
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

int CMatrix::ParseSubscript( String^ astrSubscript, int * apiFrom, int * apiTo )
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

CMatrix^ CMatrix::default::get( String^ astrRow, String^ astrCol )
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

	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );

	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
	{
		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
		{
			mTemp[ iRow - iRowFrom, iCol - iColFrom ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return mTemp;

}

void CMatrix::default::set( String^ astrRow, String^ astrCol, CMatrix^ amSubMat )
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

void CMatrix::default::set( String^ astrRow, String^ astrCol, double adValue )
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

	for( int iRow = iRowFrom; iRow <= iRowTo; iRow ++ )
	{
		for( int iCol = iColFrom; iCol <= iColTo; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = adValue;
		}
	}

}

void CMatrix::Assign( CMatrix^ amNewMatrix )
{
	delete [ ] m_pv_ar2dData;

	m_pv_iRows = amNewMatrix->Rows;
	m_pv_iColumns = amNewMatrix->Columns;

	m_pv_ar2dData = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = amNewMatrix[ iRow, iCol ];
		}
	}

}

void CMatrix::Assign( double adValue )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = adValue;
		}
	}
}

CMatrix^ CMatrix::Copy( void )
{
	return gcnew CMatrix( this );
}

CMatrix^ CMatrix::operator+( CMatrix^ amMatrix )
{
	if( ( amMatrix->Rows != m_pv_iRows ) || ( amMatrix->Columns != m_pv_iColumns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing addition operation !");
		return nullptr;
	}

	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] + amMatrix[ iRow, iCol ];
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::operator+( double adScalar )
{
	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] + adScalar;
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::operator+( double adScalar, CMatrix^ amMatrix )
{
	return amMatrix + adScalar;
}

CMatrix^ CMatrix::operator+( void )
{
	return this;
}

CMatrix^ CMatrix::operator-( CMatrix^ amMatrix )
{
	if( ( amMatrix->Rows != m_pv_iRows ) || ( amMatrix->Columns != m_pv_iColumns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when do subtraction operation !");
		return nullptr;
	}

	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] - amMatrix[ iRow, iCol ];
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::operator-( double adScalar )
{
	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] - adScalar;
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::operator-( double adScalar, CMatrix^ amMatrix )
{
	return amMatrix * ( -1 ) + adScalar;
}

CMatrix^ CMatrix::operator-( void )
{
	return this * ( -1 );
}

CMatrix^ CMatrix::operator*( CMatrix^ amMatrix )
{
	if ( m_pv_iColumns != amMatrix->Rows )
	{
		throw gcnew Exception( "Matrics dimensions must agree when doing multiplication operation !" );
		return nullptr;
	}

	int iRows = m_pv_iRows;
	int iCols = amMatrix->Columns;
	int iLength = m_pv_iColumns;

	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );
	
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

CMatrix^ CMatrix::operator*( double adScalar )
{
	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] * adScalar;
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::operator*( double adScalar, CMatrix^ amMatrix )
{
	return amMatrix * adScalar;
}

CMatrix^ CMatrix::operator&( CMatrix^ amMatrix )
{
	if( ( amMatrix->Rows != m_pv_iRows ) || ( amMatrix->Columns != m_pv_iColumns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element multiplication operation !");
		return nullptr;
	}

	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] * amMatrix[ iRow, iCol ];
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::operator/( double adScalar )
{
	CMatrix^ mTemp = gcnew CMatrix( m_pv_iRows, m_pv_iColumns );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] / adScalar;
		}
	}

	return mTemp;
}

void CMatrix::ElementAddition( CMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	if( ( m_pv_iRows != iRows ) || ( m_pv_iColumns != iCols ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element addition operation !");
		return;
	}

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] + amMatrix[ iRow, iCol ];
		}
	}

}

CMatrix^ CMatrix::ElementAddition( CMatrix^ amMat1, CMatrix^ amMat2 )
{
	if( ( amMat1->Rows != amMat2->Rows ) || ( amMat1->Columns != amMat2->Columns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element addition operation !");
		return nullptr;
	}

	int iRows = amMat1->Rows;
	int iCols = amMat1->Columns;
	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amMat1[ iRow, iCol ] + amMat2[ iRow, iCol ];
		}
	}

	return mTemp;
}

void CMatrix::ElementMultiplication( CMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	if( ( m_pv_iRows != iRows ) || ( m_pv_iColumns != iCols ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element multiplication operation !");
		return;
	}

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = m_pv_ar2dData[ iRow, iCol ] * amMatrix[ iRow, iCol ];
		}
	}

}

CMatrix^ CMatrix::ElementMultiplication( CMatrix^ amMat1, CMatrix^ amMat2 )
{
	if( ( amMat1->Rows != amMat2->Rows ) || ( amMat1->Columns != amMat2->Columns ) )
	{
		throw gcnew Exception("Matrics\' dimensions must be the same when doing element multiplication operation !");
		return nullptr;
	}

	int iRows = amMat1->Rows;
	int iCols = amMat1->Columns;
	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amMat1[ iRow, iCol ] * amMat2[ iRow, iCol ];
		}
	}

	return mTemp;
}

void CMatrix::ElementPower( double adPower )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = Math::Pow( m_pv_ar2dData[ iRow, iCol ], adPower );
		}
	}
}

CMatrix^ CMatrix::ElementPower( CMatrix^ amMatix, double adPower )
{
	int iRows = amMatix->Rows;
	int iCols = amMatix->Columns;
	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = Math::Pow( amMatix[ iRow, iCol ], adPower );
		}
	}

	return mTemp;
}

void CMatrix::Resize( int aiRows, int aiCols )
{
	m_pv_iRows    = aiRows;
	m_pv_iColumns = aiCols;
	delete [ ] m_pv_ar2dData;
 	m_pv_ar2dData = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );
}

int CMatrix::Reshape( int aiRows, int aiCols )
{
	if( m_pv_iRows * m_pv_iColumns != aiRows * aiCols )
	{
		return -1;
	}

 	array < double, 2 >^ ar2dData = gcnew array < double, 2 >( aiRows, aiCols );

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

CMatrix^ CMatrix::Reshape( CMatrix^ amMatrix, int aiRows, int aiCols )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	if( iRows * iCols != aiRows * aiCols )
	{
		throw gcnew Exception( "Invalid number of rows or columns ( Reshape )" );
		return nullptr;
	}

 	CMatrix^ mTemp = gcnew CMatrix( aiRows, aiCols );

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

CMatrix^ CMatrix::Identity( int aiDim )
{
	CMatrix^ mTemp = gcnew CMatrix( aiDim, aiDim );

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

void CMatrix::Ones( void )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = 1;
		}
	}
}

CMatrix^ CMatrix::Ones( int aiRows, int aiCols )
{
	return gcnew CMatrix( aiRows, aiCols, 1 );
}

CMatrix^ CMatrix::Ones( int aiDim )
{
	return gcnew CMatrix( aiDim, aiDim, 1 );
}

void CMatrix::Zeros( void )
{
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			m_pv_ar2dData[ iRow, iCol ] = 0;
		}
	}
}

CMatrix^ CMatrix::Zeros( int aiRows, int aiCols )
{
	return gcnew CMatrix( aiRows, aiCols, 0 );
}

CMatrix^ CMatrix::Zeros( int aiDim )
{
	return gcnew CMatrix( aiDim, aiDim, 0 );
}

void CMatrix::ConcatByRow( CMatrix^ amB )
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

	array < double, 2 >^ ar2dTemp = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

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

CMatrix^ CMatrix::ConcatByRow(  CMatrix^ amA, CMatrix^ amB )
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

	CMatrix^ mTemp = gcnew CMatrix( iNewRows, iNewCols );

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

void CMatrix::ConcatByColumn( CMatrix^ amB )
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

	array < double, 2 >^ ar2dTemp = gcnew array < double, 2 >( m_pv_iRows, m_pv_iColumns );

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

CMatrix^ CMatrix::ConcatByColumn(  CMatrix^ amA, CMatrix^ amB )
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

	CMatrix^ mTemp = gcnew CMatrix( iNewRows, iNewCols );

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

CMatrix^ CMatrix::Assemble( int aiRows, int aiCols, array< CMatrix^ >^ aarmMat )
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

	CMatrix^ mTemp = gcnew CMatrix( iNewRows, iNewCols );
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

CMatrix^ CMatrix::SolveEquation( CMatrix^ aMAA, CMatrix^ aMY ) // Solve Ax = y => x = A \ y
{
	CMatrix^ mMA = gcnew CMatrix( aMAA );
	int iARows = mMA->Rows;
	int iACols = mMA->Columns;
	if( ( iARows != iACols ) || ( 1 != aMY->Columns ) || ( aMY->Rows != iARows ) )
	{
		throw gcnew Exception( "Dimension of A or y in A * x = y is not right !");
		return nullptr;
	}

	CMatrix^ mU = gcnew CMatrix( iARows, iACols, 0 );
	CMatrix^ mL = gcnew CMatrix( iARows, iACols, 0 );
	CMatrix^ mTemp = gcnew CMatrix( iARows, 1 );
	CMatrix^ mMX = gcnew CMatrix( iARows, 1 );

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


	double dTemp;
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

CMatrix^ CMatrix::LeastSquare( CMatrix^ aMA, CMatrix^ aMY )
{

	CMatrix^ mTransposedA = aMA->Transpose( );
	CMatrix^ mNewA = mTransposedA * aMA;
	CMatrix^ mNewY = mTransposedA * aMY;
	CMatrix^ mX = SolveEquation( mNewA, mNewY );
	delete mTransposedA;
	delete mNewA;
	delete mNewY;

	return mX;
}

CMatrix^ CMatrix::Transpose( void )
{
	CMatrix^ mTemp = gcnew CMatrix( m_pv_iColumns, m_pv_iRows ); 

	for ( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			mTemp[ iCol, iRow ] = m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::Transpose( CMatrix^ amMatrix )
{
	int iRows = amMatrix->Columns;
	int iCols = amMatrix->Rows;

	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols ); 

	for ( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = amMatrix[ iCol, iRow ];
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::Product( CMatrix^ amA, CMatrix^ amB )
{
	if ( amA->Columns != amB->Rows )
	{
		throw gcnew Exception( "Matrics dimensions must agree when doing multiplication operation !" );
		return nullptr;
	}

	int iRows = amA->Rows;
	int iCols = amB->Columns;
	int iLength = amA->Columns;;

	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );
	
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

CMatrix^ CMatrix::Kronecker( CMatrix^ amB )
{
	int iRows = m_pv_iRows;
	int iCols = m_pv_iColumns;
	int iLength = iRows * iCols;
	array< CMatrix^ >^ armMat = gcnew array< CMatrix^ >( iLength );
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			unsigned long ulLoc = iRow * iCols + iCol;
			armMat[ ulLoc ] = amB * m_pv_ar2dData[ iRow, iCol ];
		}
	}

	CMatrix^ mTemp = Assemble( iRows, iCols, armMat );

	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		delete armMat[ iCount ];
	}

	delete [ ] armMat;

	return mTemp;
}

CMatrix^ CMatrix::Kronecker( CMatrix^ amA, CMatrix^ amB )
{
	int iRows = amA->Rows;
	int iCols = amA->Columns;
	int iLength = iRows * iCols;
	array< CMatrix^ >^ armMat = gcnew array< CMatrix^ >( iLength );
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			unsigned long ulLoc = iRow * iCols + iCol;
			armMat[ ulLoc ] = amB * amA[ iRow, iCol ];
		}
	}

	CMatrix^ mTemp = Assemble( iRows, iCols, armMat );

	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		delete armMat[ iCount ];
	}

	delete [ ] armMat;

	return mTemp;
}

CMatrix^ CMatrix::WalshMatrix( int aiDim )
{
	//k = log2( Total );
	double dLength = Math::Log( aiDim, 2 );
	int iLength;
	if( Math::Floor( dLength ) != dLength ) //if( floor( k ) ~= k )
	{
		iLength = ( int )( Math::Ceiling( dLength ) ); //k = k + 1;
	}   //end
	else
	{
		iLength = ( int )dLength;
	}

	CMatrix^ mTemp = gcnew CMatrix( 1, 1, 1 ); //H = [ 1 ];

	array< CMatrix^ >^ armMat = gcnew array< CMatrix^ >( 4 );
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

double CMatrix::Mean( void )
{
	double dSum = 0;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dSum += m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return dSum / ( m_pv_iRows * m_pv_iColumns * 1.0 );
}

double CMatrix::Mean( CMatrix^ amMatrix )
{
	double dSum = 0;

	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dSum += amMatrix[ iRow, iCol ];
		}
	}

	return dSum / ( iCols * iRows * 1.0 );
}

double CMatrix::Max( void )
{
	double dMax = m_pv_ar2dData[ 0, 0 ];

	double dTemp;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dTemp = m_pv_ar2dData[ iRow, iCol ];
			if( dTemp > dMax )
			{
				dMax = dTemp;
			}
		}
	}

	return dMax;
}

double CMatrix::Max( CMatrix^ amMatrix )
{
	double dMax = amMatrix[ 0, 0 ];

	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	double dTemp;

	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dTemp = amMatrix[ iRow, iCol ];
			if( dTemp > dMax )
			{
				dMax = dTemp;
			}
		}
	}

	return dMax;
}

double CMatrix::Min( void )
{
	double dMin = m_pv_ar2dData[ 0, 0 ];

	double dTemp;

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

double CMatrix::Min( CMatrix^ amMatrix )
{
	double dMin = amMatrix[ 0, 0 ];

	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	double dTemp;

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

void CMatrix::MaxAndMin( double & adMax, double & adMin )
{
	adMax = m_pv_ar2dData[ 0, 0 ];
	adMin = m_pv_ar2dData[ 0, 0 ];

	double dTemp;

	for ( int iRow = 0; iRow < m_pv_iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dTemp = m_pv_ar2dData[ iRow, iCol ];

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

void CMatrix::MaxAndMin( CMatrix^ amMatrix, double & adMax, double & adMin )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;

	adMax = amMatrix[ 0, 0 ];
	adMin = amMatrix[ 0, 0 ];

	double dTemp;

	for ( int iRow = 0; iRow < iRows ; iRow ++ )
	{
		for ( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dTemp = amMatrix[ iRow, iCol ];

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

int CMatrix::ConvertStringToInt( String^ astrInt )
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


double CMatrix::ConvertStringToDouble( String^ astrDoulbe )
{
	double dResult;

	try
	{
		dResult = Convert::ToDouble( astrDoulbe );
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

CMatrix^ CMatrix::GetCirculantMatrix( CMatrix^ amVector )
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
	CMatrix^ mTemp  = gcnew CMatrix( iLength );

	for ( int iRow = 0; iRow < iLength; iRow ++ )
	{
		for( int iCol = 0; iCol < iLength; iCol ++ )
		{
			mTemp[ iLength - iRow - 1, iCol ] = amVector[ ( iRow + iCol + 1 ) % iLength ];
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::GetCosColumn( double adFrequency, int aiLength )
{

	if( 0 >= aiLength )
	{
		throw gcnew Exception( "Length should >= 0 ( GetCosColumn ) " );
	}

	CMatrix^ mTemp = gcnew CMatrix( aiLength, 1 );

	double dConst = 2.0 * Math::PI * adFrequency / aiLength;
	double dAmp   = 1; //127.5;
	for( int iCount = 0; iCount < aiLength; iCount ++ )
	{
		mTemp[ iCount, 0 ] = dAmp * Math::Cos( dConst * iCount );
	}

	return mTemp;
}

double CMatrix::VectorNorm( CMatrix^ amVector, int aiNorm )
{

	if( ( 1 != amVector->Rows ) && ( 1 != amVector->Columns ) )
	{
		throw gcnew Exception( "It should be a vector ( Norm )!" );
		return -1;
	}

	double dNorm = aiNorm;
	int iLength = amVector->Rows * amVector->Columns;

	if( aiNorm > 0 )
	{
		double dSum = 0;

		for( int iCount = 0; iCount < iLength; iCount ++ )
		{
			dSum += Math::Pow( Math::Abs( amVector[ iCount ] ), dNorm );
		}
		return Math::Pow( dSum, 1.0 / dNorm );
	}
	else // inf norm
	{
		double dMax = Math::Abs( amVector[ 0 ] );
		double dTemp;
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

CMatrix^ CMatrix::CrossProduct( CMatrix^ amA, CMatrix^ amB )
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

	CMatrix^ mTemp = gcnew CMatrix( amA->Rows, amA->Columns );
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

double CMatrix::DotProduct( CMatrix^ amA, CMatrix^ amB )
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

	double dSum = 0;
	for( int iCount = 0; iCount < iLength; iCount ++ )
	{
		dSum += amA[ iCount ] * amB[ iCount ];
	}

	return dSum;
}

double CMatrix::Determinant2( CMatrix^ amA )
{
	if( ( 2 != amA->Rows ) || ( 2 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 2 x 2 matrix !" );
		return 0;
	}

	return( amA[ 0, 0 ] * amA[ 1, 1 ] - amA[ 0, 1 ] * amA[ 1, 0 ] );
}

double CMatrix::Determinant3( CMatrix^ amA )
{
	if( ( 3 != amA->Rows ) || ( 3 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 3 x 3 matrix !" );
		return 0;
	}

	double dDet = amA[ 0, 0 ] * amA[ 1, 1 ] * amA[ 2, 2 ];
	dDet += amA[ 1, 0 ] * amA[ 0, 2 ] * amA[ 2, 1 ];
	dDet += amA[ 2, 0 ] * amA[ 0, 1 ] * amA[ 1, 2 ];
	dDet -= amA[ 0, 0 ] * amA[ 1, 2 ] * amA[ 2, 1 ];
	dDet -= amA[ 1, 0 ] * amA[ 0, 1 ] * amA[ 2, 2 ];
	dDet -= amA[ 2, 0 ] * amA[ 0, 2 ] * amA[ 1, 1 ];

	return dDet;
}

CMatrix^ CMatrix::Inverse2( CMatrix^ amA )
{
	if( ( 2 != amA->Rows ) || ( 2 != amA->Columns ) )
	{
		throw gcnew Exception( " It should be a 2 x 2 matrix !" );
		return nullptr;
	}

	double dDet = CMatrix::Determinant2( amA );

	if( 0 == dDet )
	{
		throw gcnew Exception( " Its determinant is zero !" );
		return nullptr;
	}

	CMatrix^ mTemp = gcnew CMatrix( 2 );
	mTemp[ 0, 0 ] = amA[ 1, 1 ] / dDet;
	mTemp[ 0, 1 ] = - amA[ 0, 1 ] / dDet;
	mTemp[ 1, 0 ] = - amA[ 1, 0 ] / dDet;
	mTemp[ 1, 1 ] = amA[ 0, 0 ] / dDet;


	return mTemp;
}

CMatrix^ CMatrix::Inverse3( CMatrix^ amA )
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
///	double dDet = amA[ 0, 0 ] * amA[ 1, 1 ] * amA[ 2, 2 ];
///	dDet += amA[ 1, 0 ] * amA[ 0, 2 ] * amA[ 2, 1 ];
///	dDet += amA[ 2, 0 ] * amA[ 0, 1 ] * amA[ 1, 2 ];
///	dDet -= amA[ 0, 0 ] * amA[ 1, 2 ] * amA[ 2, 1 ];
///	dDet -= amA[ 1, 0 ] * amA[ 0, 1 ] * amA[ 2, 2 ];
///	dDet -= amA[ 2, 0 ] * amA[ 0, 2 ] * amA[ 1, 1 ];

	double dDet = CMatrix::Determinant3( amA );

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

	CMatrix^ mTemp = gcnew CMatrix( 3 );
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

double CMatrix::BiLinearInterp( CMatrix^ avPt3TL, CMatrix^ avPt3TR, CMatrix^ avPt3BL, CMatrix^ avPt3BR, CMatrix^ % apvPt3Interp )
{
//% f(x,y) = ax + by + cxy + d

//B = zeros( 4, 1 );
	CMatrix^ mA = gcnew CMatrix( 4 );
	CMatrix^ vB = gcnew CMatrix( 4, 1 );
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
	CMatrix^ vCoef = SolveEquation( mA, vB );


//z = Coef( 1 ) * Pt2( 1 ) + Coef( 2 ) * Pt2( 2 ) + Coef( 3 ) * Pt2( 1 ) * Pt2( 2 ) + Coef( 4 );
	double dZ = vCoef[ 0, 0 ] * apvPt3Interp[ 0, 0 ];
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

double CMatrix::BiLinearInterp2( double adZ1, double adZ2, double adZ3, double adZ4, double adX, double adY )
{
	double dX0 = adX - Math::Floor( adX );
	double dY0 = adY - Math::Floor( adY );

	double dD = adZ1;
	double dA = adZ2 - dD;
	double dB = adZ3 - dD;
	double dC = adZ4 - dA - dB - dD;

	return( dA * dX0 + dB * dY0 + dC * dX0 * dY0 + dD );
}

int CMatrix::ART_Fact( CMatrix^ amM, CMatrix^ % apmA, CMatrix^ % apmR, CMatrix^ % apmT, double * apdM34 )
{
//ml1 = pml( 1, 1 : 3 )';
//ml2 = pml( 2, 1 : 3 )';
//ml3 = pml( 3, 1 : 3 )';
	if( ( 3 != amM->Rows ) && ( 4 != amM->Columns ) )
	{
		return -1;
	}

	CMatrix^ vM1 = amM[ "0", "0:2" ]->Transpose( );
	CMatrix^ vM2 = amM[ "1", "0:2" ]->Transpose( );
	CMatrix^ vM3 = amM[ "2", "0:2" ]->Transpose( );


//ml14 = pml( 1, 4 );
//ml24 = pml( 2, 4 );
	double dM14 = amM[ 0, 3 ];
	double dM24 = amM[ 1, 3 ];

//ml34 = 1 / sqrt( ml3' * ml3 );
	double dM34 = 1.0 / VectorNorm( vM3, 2 );
	* apdM34 = dM34;


//r3 = ml34 * ml3;
	CMatrix^ vR3 = vM3 * dM34;

//u0 = m34 * m1' * r3;
//v0 = m34 * m2' * r3;
	double dU0 = dM34 * DotProduct( vM1, vR3 );
	double dV0 = dM34 * DotProduct( vM2, vR3 );

//ay = m34 * norm( CrossProduct( m2, r3 ), 2 );
//r2 = ( m34 * m2 - v0 * r3 ) / ay;
	double dAy = dM34 * VectorNorm( CrossProduct( vM2, vR3 ), 2 );
	CMatrix^ vR2 = vM2 * ( dM34 / dAy ) + ( vR3 * ( -dV0 / dAy ) );


//s = m34 * m1' * r2;
	double dS = dM34 * DotProduct( vM1, vR2 );

//ax = m34 * norm( CrossProduct( CrossProduct( m1', r3' ), CrossProduct( r2', r3' ) ), 2 );
	double dAx = dM34 * VectorNorm( CrossProduct( CrossProduct( vM1, vR3 ), CrossProduct( vR2, vR3 ) ), 2 );

//r1 = ( m34 * m1 - u0 * r3 - s * r2 ) / ax;
	CMatrix^ vR1 = vM1 * ( dM34 / dAx ) + ( vR3 * ( -dU0 / dAx ) ) + ( vR2 * ( -dS / dAx ) );

//tz = m34;
	double dTz = dM34;
//ty = ( m34 * m24 - v0 * tz ) / ay;
	double dTy = ( dM34 * dM24 - dV0 * dTz ) / dAy;
//tx = ( m34 * m14 - u0 * tz - s* ty ) / ax;
	double dTx = ( dM34 * dM14 - dU0 * dTz - dS * dTy ) / dAx;

//A = [ ax, s, u0; 0, ay, v0; 0, 0, 1 ];
//R = [ r1'; r2'; r3' ];
//T = [ tx; ty; tz ];


	delete apmA;
	apmA = gcnew CMatrix( 3 );
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
	apmR = gcnew CMatrix( 3 );
	apmR[ "0", "0:2" ] = vR1->Transpose( );
	apmR[ "1", "0:2" ] = vR2->Transpose( );
	apmR[ "2", "0:2" ] = vR3->Transpose( );

	delete apmT;
	apmT = gcnew CMatrix( 3, 1 );
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

double CMatrix::Sum( int aiCenterX, int aiCenterY, int aiRadius )
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

	double dSum = 0;
	for( int iRow = iFromY; iRow <= iToY; iRow ++ )
	{
		for( int iCol = iFromX; iCol <= iToX; iCol ++ )
		{
			dSum += m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return dSum;

}

double CMatrix::Sum( CMatrix^ amMatrix, int aiCenterX, int aiCenterY, int aiRadius )
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

	double dSum = 0;
	for( int iRow = iFromY; iRow <= iToY; iRow ++ )
	{
		for( int iCol = iFromX; iCol <= iToX; iCol ++ )
		{
			dSum += amMatrix[ iRow, iCol ];
		}
	}

	return dSum;

}

double CMatrix::Sum( CMatrix^ amMatrix )
{
	int iRows = amMatrix->Rows;
	int iCols = amMatrix->Columns;
	double dSum = 0;
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			dSum += amMatrix[ iRow, iCol ];
		}
	}

	return dSum;

}

double CMatrix::Sum( void )
{
	double dSum = 0;
	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dSum += m_pv_ar2dData[ iRow, iCol ];
		}
	}

	return dSum;

}

void CMatrix::SaveAsM( String^ astrMatrixName, String^ astrFileName )
{
	CMatrix::SaveAsM( this, astrMatrixName, astrFileName );
}

void CMatrix::SaveAsM( CMatrix^ amData, String^ astrMatrixName, String^ astrFileName )
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
				swMatrix->Write( "; " );
			}
		}
	}

	swMatrix->WriteLine( " ];" );

	swMatrix->Close( );
	delete swMatrix;
}

void CMatrix::SaveAsBinary( String^ astrFileName )
{
	CMatrix::SaveAsBinary( this, astrFileName );
}

void CMatrix::SaveAsBinary( CMatrix^ amData, String^ astrFileName )
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

CMatrix^ CMatrix::LoadFromBinary( String^ astrFileName )
{
	System::IO::BinaryReader^ brMatrix = gcnew System::IO::BinaryReader( System::IO::File::Open( astrFileName, System::IO::FileMode::Open ) ); 

	Int32 iRows = brMatrix->ReadInt32( );
	Int32 iCols = brMatrix->ReadInt32( );

	CMatrix^ mTemp = gcnew CMatrix( iRows, iCols );

	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < iCols; iCol ++ )
		{
			mTemp[ iRow, iCol ] = brMatrix->ReadDouble( );
		}
	}

	brMatrix->Close( );
	delete brMatrix;

	return mTemp;
}

int CMatrix::GetProcessorNumber( void )
{
	return CMatrix::ConvertStringToInt( System::Environment::GetEnvironmentVariable( "NUMBER_OF_PROCESSORS" ) );
}

double CMatrix::Determinant( void )
{
	return CMatrix::Determinant( this );
}

double CMatrix::Determinant( CMatrix^ amMatrix )
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

	CMatrix^ mA = amMatrix->Copy( );

	double dDet = 1;
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		double dMax = Math::Abs( mA[ iRow, iRow ] );
		int iLine = iRow;
		for( int iDiagCount = iRow + 1; iDiagCount < iRows; iDiagCount ++ )
		{
			double dTemp = Math::Abs( mA[ iDiagCount, iRow ] );
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
			CMatrix^ mTemp = mA[ iRow.ToString( ), ":" ];
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

void CMatrix::UniformRandom( void )
{
	double dX = ( double )System::DateTime::Now.Second;
	int    iA = 69069;
	int    iC = 1;
	unsigned long ulM = ( unsigned long )( Math::Pow( 2, 32 )  - 1 );

	for( int iRow = 0; iRow < m_pv_iRows; iRow ++ )
	{
		for( int iCol = 0; iCol < m_pv_iColumns; iCol ++ )
		{
			dX = ( ( unsigned long )( iA * dX + iC + 0.5 ) ) % ulM;
			this[ iRow, iCol ] = dX / ulM;
		}
	}
}

CMatrix^ CMatrix::UniformRandom( int aiRows, int aiCols )
{
	CMatrix^ mTemp = gcnew CMatrix( aiRows, aiCols );

	double dX = ( double )System::DateTime::Now.Second;
	int    iA = 69069;
	int    iC = 1;
	unsigned long ulM = ( unsigned long )( Math::Pow( 2, 32 )  - 1 );

	for( int iRow = 0; iRow < aiRows; iRow ++ )
	{
		for( int iCol = 0; iCol < aiCols; iCol ++ )
		{
			dX = ( ( unsigned long )( iA * dX + iC + 0.5 ) ) % ulM;
			mTemp[ iRow, iCol ] = dX / ulM;
		}
	}

	return mTemp;
}

CMatrix^ CMatrix::Inverse( void )
{
	return CMatrix::Inverse( this );
}

CMatrix^ CMatrix::Inverse( CMatrix^ amMatrix )
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
			CMatrix^ mTemp = gcnew CMatrix( 1, 1 );
			mTemp[ 0, 0 ] = 1 / amMatrix[ 0, 0 ];
			return mTemp;
		}
		break;
	case 2:
		return CMatrix::Inverse2( amMatrix );
	case 3:
		return CMatrix::Inverse3( amMatrix );
	}

	CMatrix^ mA = gcnew CMatrix( iRows, iRows * 2 );
	mA[ ":", "0:" + ( iRows - 1 ).ToString( ) ] = amMatrix[ ":", ":" ];
	mA[ ":", iRows.ToString( ) + ":" + ( 2 * iRows - 1 ).ToString( ) ] = CMatrix::Identity( iRows );
	for( int iRow = 0; iRow < iRows; iRow ++ )
	{
		double dMax = Math::Abs( mA[ iRow, iRow ] );
		int iLine = iRow;
		for( int iDiagCount = iRow + 1; iDiagCount < iRows; iDiagCount ++ )
		{
			double dTemp = Math::Abs( mA[ iDiagCount, iRow ] );
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
			CMatrix^ mTemp = mA[ iRow.ToString( ), ":" ];
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

	CMatrix^ mTemp = mA[ ":", iRows.ToString( ) + ":" + ( 2 * iRows - 1 ).ToString( ) ];
	delete mA;

	return mTemp;

}

void CMatrix::House( CMatrix^ avX, CMatrix^ % avV, double & adB )
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

	CMatrix^ vX;
	if( 1 == iRows )
	{
		vX = avX->Transpose( )->Copy( );
	}
	else
	{
		vX = avX->Copy( );
	}

	int iN = vX->Length;
	CMatrix^ vTemp = vX[ "1:" + ( iN - 1 ).ToString( ), "0" ];
	double dA = ( vTemp->Transpose( ) * vTemp )[ 0, 0 ];

	avV = vX->Copy( );
	avV[ 0 ] = 1;
	if( 0 == dA )
	{
		adB = 0;
	}
	else
	{
		double dX0 = vX[ 0 ];
		double dV0;
		double dU = Math::Sqrt( dX0 * dX0 + dA );
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

void CMatrix::HouseholderQR( CMatrix^ % amQ, CMatrix^ % amR )
{
	CMatrix::HouseholderQR( this, amQ, amR );
}

void CMatrix::HouseholderQR( CMatrix^ amA, CMatrix^ % amQ, CMatrix^ % amR )
{
	delete amQ;
	delete amR;

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CMatrix^ mR = amA->Copy( );
	CMatrix^ mQ = CMatrix::Identity( iRows );

	int iN = Math::Min( iRows - 1, iCols );

	CMatrix^ vV;
	double dB;
	String^ strToEnd1 = ":" + ( iRows - 1 ).ToString( );
	String^ strToEnd2 = ":" + ( iCols - 1 ).ToString( );
	for( int iCount = 0; iCount < iN; iCount ++ )
	{
		CMatrix::House( mR[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) ], vV, dB );
		CMatrix^ mTemp = dB * vV * vV->Transpose( );

		CMatrix^ mQTemp = mQ[ iCount.ToString( ) + strToEnd1, ":" ];
		mQ[ iCount.ToString( ) + strToEnd1, ":" ] = mQTemp - mTemp * mQTemp;
		delete mQTemp;

		CMatrix^ mRTemp = mR[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ];
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

void CMatrix::Bidiagonalize( CMatrix^ % amU, CMatrix^ % amB, CMatrix^ % amV ) // A = U * B * V'; B is bidiagonal;
{
	CMatrix::Bidiagonalize( this, amU, amB, amV );
}

void CMatrix::Bidiagonalize( CMatrix^ amA, CMatrix^ % amU, CMatrix^ % amB, CMatrix^ % amV ) // A = U * B * V'; B is bidiagonal;
{
	delete amU;
	delete amB;
	delete amV;

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CMatrix^ mB = amA->Copy( );
	CMatrix^ mU = CMatrix::Identity( iRows );
	CMatrix^ mV = CMatrix::Identity( iCols );

	int iN = Math::Min( iRows - 1, iCols );

	CMatrix^ vV;
	double dB;
	String^ strToEnd1 = ":" + ( iRows - 1 ).ToString( );
	String^ strToEnd2 = ":" + ( iCols - 1 ).ToString( );
	for( int iCount = 0; iCount < iN; iCount ++ )
	{
		CMatrix::House( mB[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) ], vV, dB );
		CMatrix^ mTemp = dB * vV * vV->Transpose( );

		CMatrix^ mUTemp = mU[ iCount.ToString( ) + strToEnd1, ":" ];
		mU[ iCount.ToString( ) + strToEnd1, ":" ] = mUTemp - mTemp * mUTemp;
		delete mUTemp;

		CMatrix^ mBTemp = mB[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ];
		mB[ iCount.ToString( ) + strToEnd1, iCount.ToString( ) + strToEnd2 ] = mBTemp - mTemp * mBTemp;
		mB[ ( iCount + 1 ).ToString( ) + strToEnd1, iCount.ToString( ) ] = 0;
		delete mBTemp;

		delete mTemp;
	    
		if( iCount < iCols - 2 )
		{
			CMatrix::House( mB[ iCount.ToString( ), ( iCount + 1 ).ToString( ) + strToEnd2 ], vV, dB );
			CMatrix^ mTemp = dB * vV * vV->Transpose( );

			CMatrix^ mVTemp = mV[ ( iCount + 1 ).ToString( ) + strToEnd2, ":" ];
			mV[ ( iCount + 1 ).ToString( ) + strToEnd2, ":" ] = mVTemp - mTemp * mVTemp;
			delete mVTemp;

			CMatrix^ mBTemp = mB[ iCount.ToString( ) + strToEnd1, ( iCount + 1 ).ToString( ) + strToEnd2 ];
			mB[ iCount.ToString( ) + strToEnd1, ( iCount + 1 ).ToString( ) + strToEnd2 ] = mBTemp - mBTemp * mTemp;
			mB[ iCount.ToString( ), ( iCount + 2 ).ToString( ) + strToEnd2 ] = 0;
			delete mBTemp;

			delete mTemp;
		}
	}

	if( iCols > iRows + 1 )
	{
		CMatrix::House( mB[ ( iRows - 1 ).ToString( ), iRows.ToString( ) + strToEnd2 ], vV, dB );
		CMatrix^ mTemp = dB * vV * vV->Transpose( );

		CMatrix^ mVTemp = mV[ iRows.ToString( ) + strToEnd2, ":" ];
		mV[ iRows.ToString( ) + strToEnd2, ":" ] = mVTemp - mTemp * mVTemp;
		delete mVTemp;

		CMatrix^ mBTemp = mB[ ( iRows - 1 ).ToString( ), iRows.ToString( ) + strToEnd2 ];
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

double CMatrix::WilkinsonShift( CMatrix^ amT )
{
	double dA = amT[ 0, 0 ];
	double dB = amT[ 0, 1 ];
	double dC = amT[ 1, 0 ];
	double dD = amT[ 1, 1 ];
	double dE = dA + dD;
	double dSqrtDelta = Math::Sqrt( dE * dE - 4 * ( dA * dD - dB * dC ) );
	double dU1 = ( dE + dSqrtDelta ) / 2;
	double dU2 = ( dE - dSqrtDelta ) / 2;
	if( Math::Abs( dD - dU1 ) < Math::Abs( dD - dU2 ) )
	{
		return dU1;
	}
	else
	{
		return dU2;
	}
}

void CMatrix::SortEigenvalue( CMatrix^ avEigValue, CMatrix^ amEigVector, double adEps, CMatrix^ % avNewEigValue, CMatrix^ % amNewEigVector, int & aiNonZero )
{
	delete avNewEigValue;
	delete amNewEigVector;

	int iLength = avEigValue->Length;

	avNewEigValue = avEigValue->Copy( );
	amNewEigVector = amEigVector->Copy( );

	for( int iFirstCount = 0; iFirstCount < iLength - 1; iFirstCount ++ )
	{
		double dMax = avNewEigValue[ iFirstCount ];
		int iIdx = iFirstCount;
		for( int iSecondCount = iFirstCount + 1; iSecondCount < iLength; iSecondCount ++ )
		{
			double dTemp = Math::Abs( avNewEigValue[ iSecondCount ] );
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
	        
			CMatrix^ mTemp = amNewEigVector[ ":", iIdx.ToString( ) ];
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

CMatrix^ CMatrix::GEtLeftEigenVector( CMatrix^ amA, CMatrix^ amRightEigenVector )
{

	int iRows = amA->Rows;
	int iCols = amRightEigenVector->Columns;

	CMatrix^ mLeftEigenVector = gcnew CMatrix( iRows, iCols );
	for( int iCount = 0; iCount < iCols; iCount ++ )
	{
		CMatrix^ mTemp = amA * amRightEigenVector[ ":", iCount.ToString( ) ];
		mLeftEigenVector[ ":", iCount.ToString( ) ] = mTemp / CMatrix::VectorNorm( mTemp, 2 );
		delete mTemp;
	}

	if( iRows <= iCols )
	{
		return mLeftEigenVector;
	}

	CMatrix^ mTemp = CMatrix::Identity( iRows );
	mTemp[ ":", "0:" + ( iCols - 1 ).ToString( ) ] = mLeftEigenVector;
	for( int iCount = iCols; iCount < iRows; iCount ++ )
	{
		for( int iSumCount = 0; iSumCount <= iCount - 1; iSumCount ++ ) // j = 1 : i - 1
		{
			mTemp[ ":", iCount.ToString( ) ] = mTemp[ ":", iCount.ToString( ) ] - ( ( mTemp[ ":", iCount.ToString( ) ] )->Transpose( ) * mTemp[ ":", iSumCount.ToString( ) ] )[ 0, 0 ] * mTemp[ ":", iSumCount.ToString( ) ];
		}
		mTemp[ ":", iCount.ToString( ) ] = mTemp[ ":", iCount.ToString( ) ] / CMatrix::VectorNorm( mTemp[ ":", iCount.ToString( ) ], 2 );
	}

	delete mLeftEigenVector;

	return mTemp;

}

void CMatrix::SVD( CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV )
{
	CMatrix::SVD( this, 1e-16, amU, amS, amV );
}

void CMatrix::SVD( double adEps, CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV )
{
	CMatrix::SVD( this, adEps, amU, amS, amV );
}

void CMatrix::SVD( CMatrix^ amA, CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV )
{
	CMatrix::SVD( amA, 1e-16, amU, amS, amV );
}

void CMatrix::SVD( CMatrix^ amA, double adEps, CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV )
{

	delete amU;
	delete amS;
	delete amV;

	int iRows = amA->Rows;
	int iCols = amA->Columns;

	CMatrix^ mU1;
	CMatrix^ mB;
	CMatrix^ mV1;
	CMatrix::Bidiagonalize( amA, mU1, mB, mV1 );

	CMatrix^ mB1;
	if( iRows > iCols )
	{
		mB1 = mB->Transpose( ) * mB;
	}
	else
	{
		mB1 = mB * mB->Transpose( );
	}

	int iNN = Math::Min( iRows, iCols );

	CMatrix^ mEVector = CMatrix::Identity( iNN );

	for( int iCount = 0; iCount < iNN - 1; iCount ++ )
	{
		int iN = iNN - iCount - 1;
		while( Math::Abs( mB1[ iN, iN - 1 ] ) >= adEps )
		{

			double dU = CMatrix::WilkinsonShift( mB1[ ( iN - 1 ).ToString( ) + ":" + iN.ToString( ), ( iN - 1 ).ToString( ) + ":" + iN.ToString( ) ] );

			for( int iDiagCount = 0; iDiagCount <= iN; iDiagCount ++ )
			{
				mB1[ iDiagCount, iDiagCount ] = mB1[ iDiagCount, iDiagCount ] - dU;
			}
			CMatrix^ mQ;
			CMatrix^ mR;
			CMatrix::HouseholderQR( mB1[ "0:" + iN.ToString( ), "0:" + iN.ToString( ) ], mQ, mR );
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

	CMatrix^ vEigValue = gcnew CMatrix( mB1->Rows, 1 );
	for( int iCount = 0; iCount < mB1->Rows; iCount ++ )
	{
		double dTemp = mB1[ iCount, iCount ];
		if( ( dTemp < adEps ) || ( dTemp < 0 ) )
		{
			vEigValue[ iCount ] = 0;
		}
		else
		{
			vEigValue[ iCount ] = Math::Sqrt( dTemp );
		}
	}

	CMatrix^ vNewEigValue;
	CMatrix^ mU2;
	CMatrix^ mV2;
	int iNonZero;
	if( iRows > iCols )
	{
		CMatrix::SortEigenvalue( vEigValue, mEVector, adEps, vNewEigValue, mV2, iNonZero );
		mU2 = CMatrix::GEtLeftEigenVector( mB, mV2[ ":", "0:" + ( iNonZero - 1 ).ToString( ) ] );
	}
	else
	{
		CMatrix::SortEigenvalue( vEigValue, mEVector, adEps, vNewEigValue, mU2, iNonZero );
		mV2 = CMatrix::GEtLeftEigenVector( mB->Transpose( ), mU2[ ":", "0:" + ( iNonZero - 1 ).ToString( ) ] );
	}

	amS = gcnew CMatrix( iRows, iCols, 0 );
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

CMatrix^ CMatrix::SolveEquationByInverseMatrix( CMatrix^ amA, CMatrix^ avB )
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

	CMatrix^ mA;
	CMatrix^ vB;

	CMatrix^ mAT = amA->Transpose( );

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

	CMatrix^ mInvA = mA->Inverse( );
	delete mA;

	if( nullptr == mInvA )
	{
		delete vB;

		return nullptr;
	}
	
	CMatrix^ vX = mInvA * vB;

	delete vB;

	return vX;
}