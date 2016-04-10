#pragma once

using namespace System;

public ref class CFloatMatrix
{
// Property Area
public:
	property float default[ int, int ]
	{
		float get( int aiRow, int aiCol );
		void set( int aiRow, int aiCol, float adValue );
	}

	property float default[ int ]
	{
		float get( int aiCount );
		void set( int aiCount, float adValue );
	}

	property CFloatMatrix^ default[ String^ ]
	{
		CFloatMatrix^ get( String^ astrRange );
//		void set( String^ astrRange, CFloatMatrix^ amSubMat );
	}

	property CFloatMatrix^ default[ String^, String^ ]
	{
		CFloatMatrix^ get( String^ astrRow, String^ astrCol );
		void set( String^ astrRow, String^ astrCol, CFloatMatrix^ amSubMat );
	}

	property CFloatMatrix^ default[ String^, String^, String^ ]
	{
		CFloatMatrix^ get( String^ astrRow, String^ astrCol, String^ astrAny );
	}

	property float default[ String^, String^ ]
	{
		void set( String^ astrRow, String^ astrCol, float adValue );
	}

	property int Rows
	{
		int get( void ){ return m_pv_iRows; }
	}

	property int Columns
	{
		int get( void ){ return m_pv_iColumns; }
	}

	property unsigned long Length
	{
		unsigned long get( void ){ return m_pv_iColumns * m_pv_iRows; }
	}

// Data Area
private:
	Int32                   m_pv_iRows;
	Int32                   m_pv_iColumns;
	array < float, 2 >^    m_pv_ar2dData;

// Function Area
private:
	void Initialize( int aiRows, int aiColumns, float adInitValue, bool abInit );
	static float Determinant2( CFloatMatrix^ amA );
	static float Determinant3( CFloatMatrix^ amA );
	static CFloatMatrix^ Inverse2( CFloatMatrix^ amA );
	
	static void House( CFloatMatrix^ avX, CFloatMatrix^ % avV, float & adB );
	static float WilkinsonShift( CFloatMatrix^ amT );
	static void SortEigenvalue( CFloatMatrix^ avEigValue, CFloatMatrix^ amEigVector, float adEps, CFloatMatrix^ % avNewEigValue, CFloatMatrix^ % amNewEigVector, int & aiNonZero );
	static CFloatMatrix^ GEtLeftEigenVector( CFloatMatrix^ amA, CFloatMatrix^ amRightEigenVector );

public:
	static int FindChar( String^ astrSource, char achTarget );
	static int GetSubscript( String^ astrInt );
	static int ParseSubscript( String^ astrSubscript, int * apiFrom, int * apiTo );
	static int ParseSubscript( String^ astrSubscript, unsigned int * apiFrom, unsigned int * apiTo );
	static CFloatMatrix^ Inverse3( CFloatMatrix^ amA );
	static CFloatMatrix^ Inverse6( CFloatMatrix^ amA );

public:
	CFloatMatrix( void );
	CFloatMatrix( String^ astrFileName );
	CFloatMatrix( CFloatMatrix^ amMatrix );
	CFloatMatrix( int aiDimension );
	CFloatMatrix( int aiRows, int aiColumns );
	CFloatMatrix( int aiRows, int aiColumns, float adInitValue );
	virtual ~CFloatMatrix( void );

	void Assign( CFloatMatrix^ amNewMatrix );
	void Assign( float adValue );
	CFloatMatrix^ Copy( void );

	CFloatMatrix^ operator+( CFloatMatrix^ amMatrix );
	CFloatMatrix^ operator+( float adScalar );
	static CFloatMatrix^ operator+( float adScalar, CFloatMatrix^ amMatrix );
	CFloatMatrix^ operator+( void );

	CFloatMatrix^ operator-( CFloatMatrix^ amMatrix );
	CFloatMatrix^ operator-( float adScalar );
	static CFloatMatrix^ operator-( float adScalar, CFloatMatrix^ amMatrix );
	CFloatMatrix^ operator-( void );

	CFloatMatrix^ operator*( CFloatMatrix^ amMatrix );
	CFloatMatrix^ operator*( float adScalar );
	static CFloatMatrix^ operator*( float adScalar, CFloatMatrix^ amMatrix );

	CFloatMatrix^ operator&( CFloatMatrix^ amMatrix );

	CFloatMatrix^ operator/( float adScalar );

	virtual void Resize( int aiRows, int aiCols );

	int Reshape( int aiRows, int aiCols );
	static CFloatMatrix^ Reshape( CFloatMatrix^ amMatrix, int aiRows, int aiCols );

	static CFloatMatrix^ Identity( int aiDim );

	void Ones( void );
	static CFloatMatrix^ Ones( int aiRows, int aiCols );
	static CFloatMatrix^ Ones( int aiDim );

	void Zeros( void );
	static CFloatMatrix^ Zeros( int aiRows, int aiCols );
	static CFloatMatrix^ Zeros( int aiDim );

	void ConcatByRow( CFloatMatrix^ amB );
	static CFloatMatrix^ ConcatByRow( CFloatMatrix^ amA, CFloatMatrix^ amB );

	void ConcatByColumn( CFloatMatrix^ amB );
	static CFloatMatrix^ ConcatByColumn( CFloatMatrix^ amA, CFloatMatrix^ amB );

	static CFloatMatrix^ Assemble( int aiRows, int aiCols, array< CFloatMatrix^ >^ aarmMat ); 

	CFloatMatrix^ Transpose( void );
	static CFloatMatrix^ Transpose( CFloatMatrix^ amMatrix );

	CFloatMatrix^ Transpose2( void );
	static CFloatMatrix^ Transpose2( CFloatMatrix^ amMatrix );

	void ElementAddition( CFloatMatrix^ amMatrix );
	static CFloatMatrix^ ElementAddition( CFloatMatrix^ amMat1, CFloatMatrix^ amMat2 );

	void ElementMultiplication( CFloatMatrix^ amMatrix );
	static CFloatMatrix^ ElementMultiplication( CFloatMatrix^ amMat1, CFloatMatrix^ amMat2 );

	void ElementPower( float adPower );
	static CFloatMatrix^ ElementPower( CFloatMatrix^ amMatix, float adPower );

	static CFloatMatrix^ SolveEquation( CFloatMatrix^ aMAA, CFloatMatrix^ aMY );
	static CFloatMatrix^ LeastSquare( CFloatMatrix^ aMA, CFloatMatrix^ aMY );
	static CFloatMatrix^ LeastSquare2( CFloatMatrix^ aMA, CFloatMatrix^ aMY );

	static CFloatMatrix^ Product( CFloatMatrix^ amA, CFloatMatrix^ amB );

	CFloatMatrix^ Kronecker( CFloatMatrix^ amB );
	static CFloatMatrix^ Kronecker( CFloatMatrix^ amA, CFloatMatrix^ amB );

	static CFloatMatrix^ WalshMatrix( int aiDim ); 
	
	CFloatMatrix^ Diff( void );

	float Mean( void );
	static float Mean( CFloatMatrix^ amMatrix );

	float Var( void );
	static float Var( CFloatMatrix^ amMatrix );

	float Max( void );
	static float Max( CFloatMatrix^ amMatrix );
	float Max( int * apiRow, int * apiCol );
	static float Max( CFloatMatrix^ amMatrix, int * apiRow, int * apiCol );
	

	float Min( void );
	static float Min( CFloatMatrix^ amMatrix );

	void   MaxAndMin( float & adMax, float & adMin );
	static void MaxAndMin( CFloatMatrix^ amMatrix, float & adMax, float & adMin );

	static int ConvertStringToInt( String^ astrInt );
	static float ConvertStringToFloat( String^ astrDoulbe );

	static CFloatMatrix^ GetCirculantMatrix( CFloatMatrix^ amVector );
	static CFloatMatrix^ GetCosColumn( float adFrequency, int aiLength );

	static float VectorNorm( CFloatMatrix^ amVector, int aiNorm );

	static CFloatMatrix^ CrossProduct( CFloatMatrix^ amA, CFloatMatrix^ amB );
	static float DotProduct( CFloatMatrix^ amA, CFloatMatrix^ amB );

	static float BiLinearInterp( CFloatMatrix^ avPt3TL, CFloatMatrix^ avPt3TR, CFloatMatrix^ avPt3BL, CFloatMatrix^ avPt3BR, CFloatMatrix^ % apvPt3Interp );

	static float BiLinearInterp2( float adZ1, float adZ2, float adZ3, float adZ4, float adX, float adY );

	static int ART_Fact( CFloatMatrix^ amM, CFloatMatrix^ % apmA, CFloatMatrix^ % apmR, CFloatMatrix^ % apmT, float * apdM34 );

	float Sum( int aiCenterX, int aiCenterY, int aiRadius );
	static float Sum( CFloatMatrix^ amMatrix, int aiCenterX, int aiCenterY, int aiRadius );

	float Sum( void );
	static float Sum( CFloatMatrix^ amMatrix );

	void SaveAsM( String^ astrMatrixName, String^ astrFileName );
	static void SaveAsM( CFloatMatrix^ amData, String^ astrMatrixName, String^ astrFileName );

	void SaveAsBinary( String^ astrFileName );
	static void SaveAsBinary( CFloatMatrix^ amData, String^ astrFileName );

	static CFloatMatrix^ LoadFromBinary( String^ astrFileName );

	static int GetProcessorNumber( void );

	float Determinant( void );
	float Determinant( CFloatMatrix^ amMatrix );

	void UniformRandom( void );
	static CFloatMatrix^ UniformRandom( int aiRows, int aiCols );

	CFloatMatrix^ Inverse( void );
	static CFloatMatrix^ Inverse( CFloatMatrix^ amMatrix );

	void HouseholderQR( CFloatMatrix^ % amQ, CFloatMatrix^ % amR );
	static void HouseholderQR( CFloatMatrix^ amA, CFloatMatrix^ % amQ, CFloatMatrix^ % amR );

	void Bidiagonalize( CFloatMatrix^ % amU, CFloatMatrix^ % amB, CFloatMatrix^ % amV );
	static void Bidiagonalize( CFloatMatrix^ amA, CFloatMatrix^ % amU, CFloatMatrix^ % amB, CFloatMatrix^ % amV );

	void SVD( CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV );
	void SVD( float adEps, CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV );
	static void SVD( CFloatMatrix^ amA, CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV );
	static void SVD( CFloatMatrix^ amA, float adEps, CFloatMatrix^ % amU, CFloatMatrix^ % amS, CFloatMatrix^ % amV );

	static CFloatMatrix^ SolveEquationByInverseMatrix( CFloatMatrix^ amA, CFloatMatrix^ avB );

	static CFloatMatrix^ ElementAcos( CFloatMatrix^ amA );
	static CFloatMatrix^ ElementAtan2( CFloatMatrix^ amA, CFloatMatrix^ amB );
	static CFloatMatrix^ ElementCos( CFloatMatrix^ amA );
	static CFloatMatrix^ ElementSin( CFloatMatrix^ amA );
	static CFloatMatrix^ RemoveNaNWithIdx( CFloatMatrix^ amA, CFloatMatrix^ amIdx, int aiCount );
	static CFloatMatrix^ Abs( CFloatMatrix^ amA );
	static CFloatMatrix^ ImToCol( CFloatMatrix^ amA, int iR, int iS );
	static CFloatMatrix^ MeanColumn( CFloatMatrix^ amA );
	static float Mean2( CFloatMatrix^ amA );
};
