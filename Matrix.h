#pragma once
#include "FloatMatrix.h"

using namespace System;


public ref class CMatrix
{
// Property Area
public:
	property double default[ int, int ]
	{
		double get( int aiRow, int aiCol );
		void set( int aiRow, int aiCol, double adValue );
	}

	property double default[ int ]
	{
		double get( int aiCount );
		void set( int aiCount, double adValue );
	}

	property CMatrix^ default[ String^, String^ ]
	{
		CMatrix^ get( String^ astrRow, String^ astrCol );
		void set( String^ astrRow, String^ astrCol, CMatrix^ amSubMat );
	}

	property double default[ String^, String^ ]
	{
		void set( String^ astrRow, String^ astrCol, double adValue );
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
	array < Double, 2 >^    m_pv_ar2dData;

// Function Area
private:
	void Initialize( int aiRows, int aiColumns, double adInitValue, bool abInit );
	static double Determinant2( CMatrix^ amA );
	static double Determinant3( CMatrix^ amA );
	static CMatrix^ Inverse2( CMatrix^ amA );
	static CMatrix^ Inverse3( CMatrix^ amA );
	static void House( CMatrix^ avX, CMatrix^ % avV, double & adB );
	static double WilkinsonShift( CMatrix^ amT );
	static void SortEigenvalue( CMatrix^ avEigValue, CMatrix^ amEigVector, double adEps, CMatrix^ % avNewEigValue, CMatrix^ % amNewEigVector, int & aiNonZero );
	static CMatrix^ GEtLeftEigenVector( CMatrix^ amA, CMatrix^ amRightEigenVector );

public:
	static int FindChar( String^ astrSource, char achTarget );
	static int GetSubscript( String^ astrInt );
	static int ParseSubscript( String^ astrSubscript, int * apiFrom, int * apiTo );

public:
	CMatrix( void );
	CMatrix( String^ astrFileName );
	CMatrix( CMatrix^ amMatrix );
	CMatrix( CFloatMatrix^ amMatrix );
	CMatrix( int aiDimension );
	CMatrix( int aiRows, int aiColumns );
	CMatrix( int aiRows, int aiColumns, double adInitValue );
	virtual ~CMatrix( void );

	void Assign( CMatrix^ amNewMatrix );
	void Assign( double adValue );
	CMatrix^ Copy( void );

	CMatrix^ operator+( CMatrix^ amMatrix );
	CMatrix^ operator+( double adScalar );
	static CMatrix^ operator+( double adScalar, CMatrix^ amMatrix );
	CMatrix^ operator+( void );

	CMatrix^ operator-( CMatrix^ amMatrix );
	CMatrix^ operator-( double adScalar );
	static CMatrix^ operator-( double adScalar, CMatrix^ amMatrix );
	CMatrix^ operator-( void );

	CMatrix^ operator*( CMatrix^ amMatrix );
	CMatrix^ operator*( double adScalar );
	static CMatrix^ operator*( double adScalar, CMatrix^ amMatrix );

	CMatrix^ operator&( CMatrix^ amMatrix );

	CMatrix^ operator/( double adScalar );

	virtual void Resize( int aiRows, int aiCols );

	int Reshape( int aiRows, int aiCols );
	static CMatrix^ Reshape( CMatrix^ amMatrix, int aiRows, int aiCols );

	static CMatrix^ Identity( int aiDim );

	void Ones( void );
	static CMatrix^ Ones( int aiRows, int aiCols );
	static CMatrix^ Ones( int aiDim );

	void Zeros( void );
	static CMatrix^ Zeros( int aiRows, int aiCols );
	static CMatrix^ Zeros( int aiDim );

	void ConcatByRow( CMatrix^ amB );
	static CMatrix^ ConcatByRow( CMatrix^ amA, CMatrix^ amB );

	void ConcatByColumn( CMatrix^ amB );
	static CMatrix^ ConcatByColumn( CMatrix^ amA, CMatrix^ amB );

	static CMatrix^ Assemble( int aiRows, int aiCols, array< CMatrix^ >^ aarmMat ); 

	CMatrix^ Transpose( void );
	static CMatrix^ Transpose( CMatrix^ amMatrix );

	void ElementAddition( CMatrix^ amMatrix );
	static CMatrix^ ElementAddition( CMatrix^ amMat1, CMatrix^ amMat2 );

	void ElementMultiplication( CMatrix^ amMatrix );
	static CMatrix^ ElementMultiplication( CMatrix^ amMat1, CMatrix^ amMat2 );

	void ElementPower( double adPower );
	static CMatrix^ ElementPower( CMatrix^ amMatix, double adPower );

	static CMatrix^ SolveEquation( CMatrix^ aMAA, CMatrix^ aMY );
	static CMatrix^ LeastSquare( CMatrix^ aMA, CMatrix^ aMY );

	static CMatrix^ Product( CMatrix^ amA, CMatrix^ amB );

	CMatrix^ Kronecker( CMatrix^ amB );
	static CMatrix^ Kronecker( CMatrix^ amA, CMatrix^ amB );

	static CMatrix^ WalshMatrix( int aiDim ); 
	
	double Mean( void );
	static double Mean( CMatrix^ amMatrix );

	double Max( void );
	static double Max( CMatrix^ amMatrix );

	double Min( void );
	static double Min( CMatrix^ amMatrix );

	void   MaxAndMin( double & adMax, double & adMin );
	static void MaxAndMin( CMatrix^ amMatrix, double & adMax, double & adMin );

	static int ConvertStringToInt( String^ astrInt );
	static double ConvertStringToDouble( String^ astrDoulbe );

	static CMatrix^ GetCirculantMatrix( CMatrix^ amVector );
	static CMatrix^ GetCosColumn( double adFrequency, int aiLength );

	static double VectorNorm( CMatrix^ amVector, int aiNorm );

	static CMatrix^ CrossProduct( CMatrix^ amA, CMatrix^ amB );
	static double DotProduct( CMatrix^ amA, CMatrix^ amB );

	static double BiLinearInterp( CMatrix^ avPt3TL, CMatrix^ avPt3TR, CMatrix^ avPt3BL, CMatrix^ avPt3BR, CMatrix^ % apvPt3Interp );

	static double BiLinearInterp2( double adZ1, double adZ2, double adZ3, double adZ4, double adX, double adY );

	static int ART_Fact( CMatrix^ amM, CMatrix^ % apmA, CMatrix^ % apmR, CMatrix^ % apmT, double * apdM34 );

	double Sum( int aiCenterX, int aiCenterY, int aiRadius );
	static double Sum( CMatrix^ amMatrix, int aiCenterX, int aiCenterY, int aiRadius );

	double Sum( void );
	static double Sum( CMatrix^ amMatrix );

	void SaveAsM( String^ astrMatrixName, String^ astrFileName );
	static void SaveAsM( CMatrix^ amData, String^ astrMatrixName, String^ astrFileName );

	void SaveAsBinary( String^ astrFileName );
	static void SaveAsBinary( CMatrix^ amData, String^ astrFileName );

	static CMatrix^ LoadFromBinary( String^ astrFileName );

	static int GetProcessorNumber( void );

	double Determinant( void );
	double Determinant( CMatrix^ amMatrix );

	void UniformRandom( void );
	static CMatrix^ UniformRandom( int aiRows, int aiCols );

	CMatrix^ Inverse( void );
	static CMatrix^ Inverse( CMatrix^ amMatrix );

	void HouseholderQR( CMatrix^ % amQ, CMatrix^ % amR );
	static void HouseholderQR( CMatrix^ amA, CMatrix^ % amQ, CMatrix^ % amR );

	void Bidiagonalize( CMatrix^ % amU, CMatrix^ % amB, CMatrix^ % amV );
	static void Bidiagonalize( CMatrix^ amA, CMatrix^ % amU, CMatrix^ % amB, CMatrix^ % amV );

	void SVD( CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV );
	void SVD( double adEps, CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV );
	static void SVD( CMatrix^ amA, CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV );
	static void SVD( CMatrix^ amA, double adEps, CMatrix^ % amU, CMatrix^ % amS, CMatrix^ % amV );

	static CMatrix^ SolveEquationByInverseMatrix( CMatrix^ amA, CMatrix^ avB );

};
