///////////////////////////////////////////////////////////////////////////
// ShaderManager.h
// 
// Author:			Doug Berg
// Last Updated:	3.7.2015
///////////////////////////////////////////////////////////////////////////
#pragma once

#include "Renderer.h"

// Shaders
#include "DefaultPixel.csh"
#include "DefaultVertex.csh"

// DirectX
#include <DirectXMath.h>
using namespace DirectX;
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")


enum ShaderTypeEnum
{
	Compute_Shader, Hull_Shader, Domain_Shader, Geometry_Shader, Pixel_Shader, Vertex_Shader,
	SHADER_TYPE_MAX
};

enum ComputeShaderEnum
{
	Compute_Default, 
	COMPUTE_MAX
};

enum HullShaderEnum
{
	Hull_Default, 
	HULL_MAX
};

enum DomainShaderEnum
{
	Domain_Default, 
	DOMAIN_MAX
};

enum GeometryShaderEnum
{
	Geometry_Default, 
	GEOMETRY_MAX
};

enum PixelShaderEnum
{
	Pixel_Default, 
	PIXEL_MAX
};

enum VertexShaderEnum
{
	Vertex_Default, 
	VERTEX_MAX
};

class ShaderManager
{
private:
	ID3D11ComputeShader  *computeShaders [ COMPUTE_MAX  ];
	ID3D11DomainShader   *domainShaders  [ DOMAIN_MAX   ];
	ID3D11GeometryShader *geometryShaders[ GEOMETRY_MAX ];
	ID3D11HullShader     *hullShaders    [ HULL_MAX     ];
	ID3D11PixelShader    *pixelShaders   [ PIXEL_MAX    ];
	ID3D11VertexShader   *vertexShaders  [ VERTEX_MAX   ];

	// Singleton class
	static ShaderManager* m_instance;

	// Helper Function
	void Init( void );

public:
	// Defaults
	ShaderManager ( void ) = default;
	~ShaderManager( void ) = default;
	ShaderManager (const ShaderManager& _copy )                = delete;
	ShaderManager& operator = ( const ShaderManager& _assign ) = delete;

	// Mutators
	bool Apply(ShaderTypeEnum _shader, const int _index);

	// Accessors
	ID3D11ComputeShader*  GetComputeShaders ( const int _enumValue );
	ID3D11DomainShader*   GetDomainShaders  ( const int _enumValue );
	ID3D11GeometryShader* GetGeometryShaders( const int _enumValue );
	ID3D11HullShader*     GetHullShaders    ( const int _enumValue );
	ID3D11PixelShader*    GetPixelShaders   ( const int _enumValue );
	ID3D11VertexShader*   GetVertexShaders  ( const int _enumValue );

	// Singleton
	ShaderManager* GetInstance( void );
	void Terminate( void );
};


