#if 0

//listing of all techniques and passes with embedded asm listings

technique ParameterVertex
{
	pass Pass1
	{
		vertexshader =
		asm {
			//
			// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
			//
			// Parameters:
			//
			//   float2 ViewportSize;
			//
			//
			// Registers:
			//
			//   Name         Reg   Size
			//   ------------ ----- ----
			//   ViewportSize c0       1
			//

			preshader
			rcp c5.x, c0.x
			rcp c5.y, c0.y

			// approximately 2 instructions used
			//
			// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
			//
			// Parameters:
			//
			//   float4 QuadInfo[4];
			//   float2 TextureSize;
			//
			//
			// Registers:
			//
			//   Name         Reg   Size
			//   ------------ ----- ----
			//   QuadInfo     c0       4
			//   TextureSize  c4       1
			//

			vs_2_0
			def c6, 0.159154937, 0.5, 6.28318548, -3.14159274
			def c7, 2, -1, 1, 0
			def c8, -1.55009923e-006, -2.17013894e-005, 0.00260416674, 0.00026041668
			def c9, -0.020833334, -0.125, 1, 0.5
			dcl_position v0
			dcl_color v1
			dcl_texcoord v2
			slt r0.x, v2.w, -v2.w
			frc r0.y, v2.w
			add r0.z, -r0.y, v2.w
			slt r0.y, -r0.y, r0.y
			mad r0.x, r0.x, r0.y, r0.z
			mova a0.x, r0.x
			mov r0.xy, c0[a0.x]
			mul r0.xyz, r0.xxyw, c4.xxyw
			mov oT0.xy, c0[a0.x].zwzw
			mul r0.xyz, r0, v2.x
			mad r0.w, v2.y, c6.x, c6.y
			frc r0.w, r0.w
			mad r0.w, r0.w, c6.z, c6.w
			sincos r1.xy, r0.w, c8, c9
			mul r0.xyw, r0.xyzz, r1.xyzx
			mad r1.x, r0.z, -r1.y, r0.x
			add r1.y, r0.w, r0.y
			add r0.xy, r1, v0
			add r0.xy, r0, -c6.y
			mul r0.xy, r0, c5
			mad r0.xy, r0, c7.x, c7.y
			mul oPos.xy, r0, c7.zyzw
			mov oPos.z, v2.z
			mov oPos.w, c7.z
			mov oD0, v1

			// approximately 32 instruction slots used
		};

		pixelshader =
		asm {
			//
			// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
			//
			// Parameters:
			//
			//   sampler2D TextureSampler;
			//
			//
			// Registers:
			//
			//   Name           Reg   Size
			//   -------------- ----- ----
			//   TextureSampler s0       1
			//

			ps_2_0
			dcl v0
			dcl t0.xy
			dcl_2d s0
			texld r0, t0, s0
			mul r0, r0, v0
			mov oC0, r0

			// approximately 3 instruction slots used (1 texture, 2 arithmetic)
		};
	}
}

#endif

const BYTE g_main[] =
{
	1,   9, 255, 254, 164,   1,
	0,   0,   0,   0,   0,   0,
	3,   0,   0,   0,   1,   0,
	0,   0,  40,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   2,   0,   0,   0,
	1,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	13,   0,   0,   0,  86, 105,
	101, 119, 112, 111, 114, 116,
	83, 105, 122, 101,   0,   0,
	0,   0,   3,   0,   0,   0,
	1,   0,   0,   0,  96,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   2,   0,
	0,   0,   1,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,  12,   0,   0,   0,
	84, 101, 120, 116, 117, 114,
	101,  83, 105, 122, 101,   0,
	3,   0,   0,   0,   1,   0,
	0,   0, 204,   0,   0,   0,
	0,   0,   0,   0,   4,   0,
	0,   0,   4,   0,   0,   0,
	1,   0,   0,   0,   0,   0,
	0, 191,   0,   0,   0, 191,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,  63,
	0,   0,   0, 191,   0,   0,
	128,  63,   0,   0,   0,   0,
	0,   0,   0,  63,   0,   0,
	0,  63,   0,   0, 128,  63,
	0,   0, 128,  63,   0,   0,
	0, 191,   0,   0,   0,  63,
	0,   0,   0,   0,   0,   0,
	128,  63,   9,   0,   0,   0,
	81, 117,  97, 100,  73, 110,
	102, 111,   0,   0,   0,   0,
	5,   0,   0,   0,   4,   0,
	0,   0, 244,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   1,   0,   0,   0,
	8,   0,   0,   0,  84, 101,
	120, 116, 117, 114, 101,   0,
	10,   0,   0,   0,   4,   0,
	0,   0,  64,   1,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   2,   0,   0,   0,
	5,   0,   0,   0,   4,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   1,   0,   0,   0,
	164,   0,   0,   0,   0,   1,
	0,   0,  24,   1,   0,   0,
	20,   1,   0,   0,  15,   0,
	0,   0,  84, 101, 120, 116,
	117, 114, 101,  83,  97, 109,
	112, 108, 101, 114,   0,   0,
	3,   0,   0,   0,  16,   0,
	0,   0,   4,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	4,   0,   0,   0,  15,   0,
	0,   0,   4,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	6,   0,   0,   0,  80,  97,
	115, 115,  49,   0,   0,   0,
	16,   0,   0,   0,  80,  97,
	114,  97, 109, 101, 116, 101,
	114,  86, 101, 114, 116, 101,
	120,   0,   5,   0,   0,   0,
	1,   0,   0,   0,   4,   0,
	0,   0,   5,   0,   0,   0,
	4,   0,   0,   0,  32,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,  60,   0,
	0,   0,  88,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0, 112,   0,   0,   0,
	140,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	220,   0,   0,   0, 240,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   1,
	0,   0,  44,   1,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0, 144,   1,   0,   0,
	0,   0,   0,   0,   1,   0,
	0,   0, 132,   1,   0,   0,
	0,   0,   0,   0,   2,   0,
	0,   0, 146,   0,   0,   0,
	0,   0,   0,   0,  88,   1,
	0,   0,  84,   1,   0,   0,
	147,   0,   0,   0,   0,   0,
	0,   0, 112,   1,   0,   0,
	108,   1,   0,   0,   1,   0,
	0,   0,   3,   0,   0,   0,
	1,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0, 255, 255,
	255, 255,   1,   0,   0,   0,
	0,   0,   0,   0, 232,   0,
	0,   0,   0,   2, 255, 255,
	254, 255,  35,   0,  67,  84,
	65,  66,  28,   0,   0,   0,
	87,   0,   0,   0,   0,   2,
	255, 255,   1,   0,   0,   0,
	28,   0,   0,   0,   0,   0,
	0,  32,  80,   0,   0,   0,
	48,   0,   0,   0,   3,   0,
	0,   0,   1,   0,   0,   0,
	64,   0,   0,   0,   0,   0,
	0,   0,  84, 101, 120, 116,
	117, 114, 101,  83,  97, 109,
	112, 108, 101, 114,   0, 171,
	4,   0,  12,   0,   1,   0,
	1,   0,   1,   0,   0,   0,
	0,   0,   0,   0, 112, 115,
	95,  50,  95,  48,   0,  77,
	105,  99, 114, 111, 115, 111,
	102, 116,  32,  40,  82,  41,
	32,  72,  76,  83,  76,  32,
	83, 104,  97, 100, 101, 114,
	32,  67, 111, 109, 112, 105,
	108, 101, 114,  32,  57,  46,
	50,  57,  46,  57,  53,  50,
	46,  51,  49,  49,  49,   0,
	31,   0,   0,   2,   0,   0,
	0, 128,   0,   0,  15, 144,
	31,   0,   0,   2,   0,   0,
	0, 128,   0,   0,   3, 176,
	31,   0,   0,   2,   0,   0,
	0, 144,   0,   8,  15, 160,
	66,   0,   0,   3,   0,   0,
	15, 128,   0,   0, 228, 176,
	0,   8, 228, 160,   5,   0,
	0,   3,   0,   0,  15, 128,
	0,   0, 228, 128,   0,   0,
	228, 144,   1,   0,   0,   2,
	0,   8,  15, 128,   0,   0,
	228, 128, 255, 255,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0, 255, 255, 255, 255,
	0,   0,   0,   0,   0,   0,
	0,   0, 104,   4,   0,   0,
	0,   2, 254, 255, 254, 255,
	66,   0,  67,  84,  65,  66,
	28,   0,   0,   0, 211,   0,
	0,   0,   0,   2, 254, 255,
	2,   0,   0,   0,  28,   0,
	0,   0,   0,   0,   0,  32,
	204,   0,   0,   0,  68,   0,
	0,   0,   2,   0,   0,   0,
	4,   0,   0,   0,  80,   0,
	0,   0,  96,   0,   0,   0,
	160,   0,   0,   0,   2,   0,
	4,   0,   1,   0,   0,   0,
	172,   0,   0,   0, 188,   0,
	0,   0,  81, 117,  97, 100,
	73, 110, 102, 111,   0, 171,
	171, 171,   1,   0,   3,   0,
	1,   0,   4,   0,   4,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0, 191,   0,   0,
	0, 191,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,  63,   0,   0,   0, 191,
	0,   0, 128,  63,   0,   0,
	0,   0,   0,   0,   0,  63,
	0,   0,   0,  63,   0,   0,
	128,  63,   0,   0, 128,  63,
	0,   0,   0, 191,   0,   0,
	0,  63,   0,   0,   0,   0,
	0,   0, 128,  63,  84, 101,
	120, 116, 117, 114, 101,  83,
	105, 122, 101,   0,   1,   0,
	3,   0,   1,   0,   2,   0,
	1,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	118, 115,  95,  50,  95,  48,
	0,  77, 105,  99, 114, 111,
	115, 111, 102, 116,  32,  40,
	82,  41,  32,  72,  76,  83,
	76,  32,  83, 104,  97, 100,
	101, 114,  32,  67, 111, 109,
	112, 105, 108, 101, 114,  32,
	57,  46,  50,  57,  46,  57,
	53,  50,  46,  51,  49,  49,
	49,   0, 254, 255,  79,   0,
	80,  82,  69,  83,   1,   2,
	88,  70, 254, 255,  38,   0,
	67,  84,  65,  66,  28,   0,
	0,   0,  99,   0,   0,   0,
	1,   2,  88,  70,   1,   0,
	0,   0,  28,   0,   0,   0,
	0,   1,   0,  32,  96,   0,
	0,   0,  48,   0,   0,   0,
	2,   0,   0,   0,   1,   0,
	0,   0,  64,   0,   0,   0,
	80,   0,   0,   0,  86, 105,
	101, 119, 112, 111, 114, 116,
	83, 105, 122, 101,   0, 171,
	171, 171,   1,   0,   3,   0,
	1,   0,   2,   0,   1,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0, 116, 120,
	0,  77, 105,  99, 114, 111,
	115, 111, 102, 116,  32,  40,
	82,  41,  32,  72,  76,  83,
	76,  32,  83, 104,  97, 100,
	101, 114,  32,  67, 111, 109,
	112, 105, 108, 101, 114,  32,
	57,  46,  50,  57,  46,  57,
	53,  50,  46,  51,  49,  49,
	49,   0, 254, 255,  12,   0,
	80,  82,  83,  73,   5,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   1,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   1,   0,
	0,   0,   5,   0,   0,   0,
	1,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	254, 255,   2,   0,  67,  76,
	73,  84,   0,   0,   0,   0,
	254, 255,  20,   0,  70,  88,
	76,  67,   2,   0,   0,   0,
	1,   0,  48,  16,   1,   0,
	0,   0,   0,   0,   0,   0,
	2,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   0,   0,
	4,   0,   0,   0,  20,   0,
	0,   0,   1,   0,  48,  16,
	1,   0,   0,   0,   0,   0,
	0,   0,   2,   0,   0,   0,
	1,   0,   0,   0,   0,   0,
	0,   0,   4,   0,   0,   0,
	21,   0,   0,   0, 240, 240,
	240, 240,  15,  15,  15,  15,
	255, 255,   0,   0,  81,   0,
	0,   5,   6,   0,  15, 160,
	131, 249,  34,  62,   0,   0,
	0,  63, 219,  15, 201,  64,
	219,  15,  73, 192,  81,   0,
	0,   5,   7,   0,  15, 160,
	0,   0,   0,  64,   0,   0,
	128, 191,   0,   0, 128,  63,
	0,   0,   0,   0,  81,   0,
	0,   5,   8,   0,  15, 160,
	1,  13, 208, 181,  97,  11,
	182, 183, 171, 170,  42,  59,
	137, 136, 136,  57,  81,   0,
	0,   5,   9,   0,  15, 160,
	171, 170, 170, 188,   0,   0,
	0, 190,   0,   0, 128,  63,
	0,   0,   0,  63,  31,   0,
	0,   2,   0,   0,   0, 128,
	0,   0,  15, 144,  31,   0,
	0,   2,  10,   0,   0, 128,
	1,   0,  15, 144,  31,   0,
	0,   2,   5,   0,   0, 128,
	2,   0,  15, 144,  12,   0,
	0,   3,   0,   0,   1, 128,
	2,   0, 255, 144,   2,   0,
	255, 145,  19,   0,   0,   2,
	0,   0,   2, 128,   2,   0,
	255, 144,   2,   0,   0,   3,
	0,   0,   4, 128,   0,   0,
	85, 129,   2,   0, 255, 144,
	12,   0,   0,   3,   0,   0,
	2, 128,   0,   0,  85, 129,
	0,   0,  85, 128,   4,   0,
	0,   4,   0,   0,   1, 128,
	0,   0,   0, 128,   0,   0,
	85, 128,   0,   0, 170, 128,
	46,   0,   0,   2,   0,   0,
	1, 176,   0,   0,   0, 128,
	1,   0,   0,   3,   0,   0,
	3, 128,   0,  32, 228, 160,
	0,   0,   0, 176,   5,   0,
	0,   3,   0,   0,   7, 128,
	0,   0, 208, 128,   4,   0,
	208, 160,   1,   0,   0,   3,
	0,   0,   3, 224,   0,  32,
	238, 160,   0,   0,   0, 176,
	5,   0,   0,   3,   0,   0,
	7, 128,   0,   0, 228, 128,
	2,   0,   0, 144,   4,   0,
	0,   4,   0,   0,   8, 128,
	2,   0,  85, 144,   6,   0,
	0, 160,   6,   0,  85, 160,
	19,   0,   0,   2,   0,   0,
	8, 128,   0,   0, 255, 128,
	4,   0,   0,   4,   0,   0,
	8, 128,   0,   0, 255, 128,
	6,   0, 170, 160,   6,   0,
	255, 160,  37,   0,   0,   4,
	1,   0,   3, 128,   0,   0,
	255, 128,   8,   0, 228, 160,
	9,   0, 228, 160,   5,   0,
	0,   3,   0,   0,  11, 128,
	0,   0, 164, 128,   1,   0,
	36, 128,   4,   0,   0,   4,
	1,   0,   1, 128,   0,   0,
	170, 128,   1,   0,  85, 129,
	0,   0,   0, 128,   2,   0,
	0,   3,   1,   0,   2, 128,
	0,   0, 255, 128,   0,   0,
	85, 128,   2,   0,   0,   3,
	0,   0,   3, 128,   1,   0,
	228, 128,   0,   0, 228, 144,
	2,   0,   0,   3,   0,   0,
	3, 128,   0,   0, 228, 128,
	6,   0,  85, 161,   5,   0,
	0,   3,   0,   0,   3, 128,
	0,   0, 228, 128,   5,   0,
	228, 160,   4,   0,   0,   4,
	0,   0,   3, 128,   0,   0,
	228, 128,   7,   0,   0, 160,
	7,   0,  85, 160,   5,   0,
	0,   3,   0,   0,   3, 192,
	0,   0, 228, 128,   7,   0,
	230, 160,   1,   0,   0,   2,
	0,   0,   4, 192,   2,   0,
	170, 144,   1,   0,   0,   2,
	0,   0,   8, 192,   7,   0,
	170, 160,   1,   0,   0,   2,
	0,   0,  15, 208,   1,   0,
	228, 144, 255, 255,   0,   0,
	255, 255, 255, 255,   4,   0,
	0,   0,   0,   0,   0,   0,
	0,   0,   0,   0,   1,   0,
	0,   0,   8,   0,   0,   0,
	84, 101, 120, 116, 117, 114,
	101,   0
};
