///////////////////////////////////////////////////////////////////////////
// ShaderManager.h
// 
// Author:			Doug Berg
// Last Updated:	3.7.2015
///////////////////////////////////////////////////////////////////////////
#pragma once
// DirectX
#include <DirectXMath.h>
#include <d3d11.h>

using namespace DirectX;

enum ShaderTypeEnum { Compute_Shader, Hull_Shader, Domain_Shader, Geometry_Shader, Pixel_Shader, Vertex_Shader,
	SHADER_TYPE_MAX };

enum ComputeShaderEnum { 
	COMPUTE_MAX };

enum HullShaderEnum { 
	HULL_MAX };

enum DomainShaderEnum { 
	DOMAIN_MAX };

enum GeometryShaderEnum {
	GEOMETRY_MAX };

enum PixelShaderEnum { Pixel_Default, 
	PIXEL_MAX
};

enum VertexShaderEnum { Vertex_Default, 
	VERTEX_MAX
};

class ShaderManager
{
private:
	// === Static Members
	static ShaderManager* s_Instance;

	// === Members
//	ID3D11ComputeShader  *m_ComputeShaders[COMPUTE_MAX];			{ Temporary
//	ID3D11DomainShader   *m_DomainShaders[DOMAIN_MAX];					Inactive
//	ID3D11GeometryShader *m_GeometryShaders[GEOMETRY_MAX];					Shader
//	ID3D11HullShader     *m_HullShaders[HULL_MAX];								Arrays }
	ID3D11PixelShader    *m_PixelShaders[PIXEL_MAX];
	ID3D11VertexShader   *m_VertexShaders[VERTEX_MAX];

	// === Private Interface === //
	void Initialize();
	// ========================= //

public:
	// Defaults
	ShaderManager ( void ) = default;
	~ShaderManager( void ) = default;
	ShaderManager (const ShaderManager& _copy )                = delete;
	ShaderManager& operator = ( const ShaderManager& _assign ) = delete;

	// === Static Interface === //
	static ShaderManager* GetInstance(void);
	// ======================== //

	// === Interface === //
	bool Apply(ShaderTypeEnum _shader, const int _index);
	void Terminate();
	// ================= //

	// === Accessors === //
//	inline ID3D11ComputeShader*  GetComputeShaders(const int _enumValue) {
//		return m_ComputeShaders[_enumValue];
//	}
//	inline ID3D11DomainShader*   GetDomainShaders(const int _enumValue) {
//		return m_DomainShaders[_enumValue];
//	}
//	inline ID3D11GeometryShader* GetGeometryShaders(const int _enumValue) {
//		return m_GeometryShaders[_enumValue];
//	}
//	inline ID3D11HullShader*     GetHullShaders(const int _enumValue) {
//		return m_HullShaders[_enumValue];
//	}
	inline ID3D11PixelShader*    GetPixelShaders(const int _enumValue) {
		return m_PixelShaders[_enumValue];
	}
	inline ID3D11VertexShader*   GetVertexShaders(const int _enumValue) {
		return m_VertexShaders[_enumValue];
	}
	// ================= //

	
};


