//-----------------------------------------------------------------------------
// VertexDemo.fx
// 動的頂点バッファを使ったパーティクル描画用のシェーダー
//=============================================================================

// ビューポート情報
float2 ViewportSize;	// {width, height }

// テクスチャサイズ
float2 TextureSize;

// 四角形情報
// xyに頂点位置、zwにUV座標を格納する
float4 QuadInfo[] = 
{
	{ -0.5,-0.5, 0, 0 }, // 左上
	{  0.5,-0.5, 1, 0 }, // 右上
	{  0.5, 0.5, 1, 1 }, // 右下
	{ -0.5, 0.5, 0, 1 }, // 左下
};

// テクスチャサンプラ設定
texture Texture;

sampler TextureSampler = sampler_state
{
	Texture = (Texture);
};

//-----------------------------------------------------------------------------
// スクリーン座標からビューポート座標へ変換する
//=============================================================================
float2 ScreenToViewport( float2 screenPos )
{
	// DirectXのスクリーン座標の0.5ピクセルのずれを直す
	screenPos -= 0.5f;

	float2 result = screenPos.xy / ViewportSize.xy * 2 - 1;
	result.y = -result.y;
	return result;
	
}

//-----------------------------------------------------------------------------
// ピクセルシェーダー
//=============================================================================
void PS( inout float4 color : COLOR0, float2 texCoord : TEXCOORD0 )
{
	color *= tex2D(TextureSampler, texCoord);
}

//-----------------------------------------------------------------------------
// 頂点データ内にパーティクルパラメーターを格納し
// 頂点シェーダー内で回転拡大などの処理をする頂点シェーダー
//=============================================================================
void ParameterVertexVS(
	float2 position : POSITION0,
	float4 color : COLOR0,
	float4 param : TEXCOORD0,	// {x: Scale, y:Rotation, z:Depth, w:cornerIndex }
	
	out float4 outputPosition : POSITION0,
	out float2 outputTexCoord : TEXCOORD0,
	out float4 outputColor    : COLOR0 )
{
	// パラメーターレジスタを展開
	float scale = param.x;
	float rotation = param.y;
	float depth = param.z;
	int cornerIndex = param.w;

	// 回転とスケール処理
	float cs = cos(rotation);
	float sn = sin(rotation);
	
	float2 pos = position + mul( QuadInfo[cornerIndex].xy * TextureSize * scale,
														float2x2(cs, sn, -sn, cs) );
	
	pos = ScreenToViewport(pos);
	
	outputPosition = float4( pos.x, pos.y, depth, 1 );
	
	// テクスチャ座標の設定
	outputTexCoord = QuadInfo[cornerIndex].zw;

	// カラーはそのままスルー	
	outputColor = color;
}

technique ParameterVertex
{
	pass Pass1
	{
		VertexShader = compile vs_2_0 ParameterVertexVS();
		PixelShader = compile ps_2_0 PS();
	}
}